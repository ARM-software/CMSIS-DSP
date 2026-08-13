#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <cstdint>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Caller-owned deterministic random state used by dropout. */
class DropoutGenerator
{
public:
    explicit DropoutGenerator(std::uint32_t seed = 0x6D2B79F5U) noexcept
        : state_(seed == 0U ? 0x6D2B79F5U : seed) {}

    void seed(std::uint32_t value) noexcept
    {
        state_ = value == 0U ? 0x6D2B79F5U : value;
    }

private:
    std::uint32_t state_;
    template <typename> friend class DropoutOperator;
};

/** Inverted dropout during recording, identity when recording is disabled. */
template <typename T = float> class DropoutOperator
{
    static void fill(T *data, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_fill_f32(T{}, data, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_fill_f16(T{}, data, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i) data[i] = T{};
#endif
    }

    static void add(const T *left, const T *right, T *output,
                    std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_add_f32(left, right, output, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_add_f16(left, right, output, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i) output[i] = left[i] + right[i];
#endif
    }

    static void copy(const T *input, T *output, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_copy_f32(input, output, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_copy_f16(input, output, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i) output[i] = input[i];
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        T *input_gradient;
        std::size_t length;
        std::uint32_t random_state;
        float drop_probability;
        T scale;
    };

    static std::uint32_t next(std::uint32_t &state) noexcept
    {
        state ^= state << 13U;
        state ^= state >> 17U;
        state ^= state << 5U;
        return state;
    }

    static bool keep(std::uint32_t &state, float drop_probability) noexcept
    {
        constexpr float inverse_24_bit_range = 1.0F / 16777216.0F;
        const float uniform =
            static_cast<float>(next(state) >> 8U) * inverse_24_bit_range;
        return uniform >= drop_probability;
    }

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        fill(record.output_gradient, record.length);
        if (record.input_gradient != nullptr)
            fill(record.input_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.input_gradient == nullptr) return;

        if (record.drop_probability == 0.0F)
        {
            add(record.input_gradient, record.output_gradient,
                record.input_gradient, record.length);
            return;
        }

        std::uint32_t state = record.random_state;
        for (std::size_t i = 0; i < record.length; ++i)
        {
            if (keep(state, record.drop_probability))
            {
                if constexpr (std::is_same<T, float>::value)
                    record.input_gradient[i] += record.output_gradient[i] * record.scale;
                else
                    record.input_gradient[i] = static_cast<T>(
                        static_cast<float>(record.input_gradient[i]) +
                        static_cast<float>(record.output_gradient[i]) *
                        static_cast<float>(record.scale));
            }
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &input,
                         float drop_probability) noexcept
    {
        if (!OperatorAccess<T>::compatible(tape, output, input) ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(input) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(input) ||
            !(drop_probability >= 0.0F && drop_probability < 1.0F))
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        return true;
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input,
                         DropoutGenerator &generator,
                         float drop_probability) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess<T>::template require<DropoutOperator<T>>(*tape))
            return false;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, input, drop_probability))
            return false;
#endif

        const std::size_t length = OperatorAccess<T>::length(output);
        if (!OperatorAccess<T>::recording(*tape))
        {
            copy(OperatorAccess<T>::values(input), OperatorAccess<T>::values(output), length);
            return OperatorAccess<T>::status(*tape) == Status::ok;
        }
        if (length == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        const float scale = 1.0F / (1.0F - drop_probability);
        const std::uint32_t initial_state = generator.state_;
        if (drop_probability == 0.0F)
        {
            copy(OperatorAccess<T>::values(input), OperatorAccess<T>::values(output), length);
        }
        else
        {
            for (std::size_t i = 0; i < length; ++i)
                OperatorAccess<T>::values(output)[i] =
                    keep(generator.state_, drop_probability)
                        ? OperatorAccess<T>::values(input)[i] * static_cast<T>(scale)
                        : T{};
        }

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr)
        {
            generator.state_ = initial_state;
            return false;
        }
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->input_gradient = OperatorAccess<T>::gradients(input);
        record->length = length;
        record->random_state = initial_state;
        record->drop_probability = drop_probability;
        record->scale = static_cast<T>(scale);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class DropoutExpression
{
public:
    DropoutExpression(const BufferView<T> &input, DropoutGenerator &generator,
                      float drop_probability) noexcept
        : input_(input), generator_(generator),
          drop_probability_(drop_probability) {}

    void evaluate(BufferView<T> &output) const noexcept
    {
        DropoutOperator<T>::evaluate(output, input_, generator_,
                                     drop_probability_);
    }

private:
    BufferView<T> input_;
    DropoutGenerator &generator_;
    float drop_probability_;
};

template <typename T = float>
inline DropoutExpression<T> dropout(const BufferView<T> &input,
                                    DropoutGenerator &generator,
                                    float drop_probability) noexcept
{
    return DropoutExpression<T>(input, generator, drop_probability);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
