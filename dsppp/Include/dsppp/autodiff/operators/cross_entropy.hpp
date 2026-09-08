#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <arm_math_types_f16.h>
#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/fast_math_functions.h>
#include <dsp/fast_math_functions_f16.h>
#include <dsp/statistics_functions.h>
#include <dsp/statistics_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <limits>
#include <cmath>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Categorical cross entropy: -sum(target[i] * log(probability[i])). */
template <typename T = float> class CrossEntropyOperator
{
    static constexpr T maximum_value() noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            return std::numeric_limits<float>::max();
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            return F16_MAX;
#else
        else
            return T{};
#endif
    }

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

    static void clip(const T *input, T *output, T low, T high,
                     std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_clip_f32(input, output, low, high, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_clip_f16(input, output, low, high, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i)
                output[i] = input[i] < low ? low : (input[i] > high ? high : input[i]);
#endif
    }

    static void vlog(T *data, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_vlog_f32(data, data, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_vlog_f16(data, data, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i)
                data[i] = static_cast<T>(std::log(static_cast<float>(data[i])));
#endif
    }

    static void dot(const T *left, const T *right, std::size_t length,
                    T *result) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_dot_prod_f32(left, right, static_cast<uint32_t>(length), result);
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_dot_prod_f16(left, right, static_cast<uint32_t>(length), result);
#else
        else
        {
            *result = T{};
            for (std::size_t i = 0; i < length; ++i) *result += left[i] * right[i];
        }
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *probability_value;
        T *probability_gradient;
        const T *target_value;
        std::size_t length;
    };

    static constexpr T minimum_probability() noexcept
    {
        // 1e-7 is useful for float32, but its reciprocal is too large for a
        // finite float16 gradient. Keep the half-precision derivative inside
        // its representable range before the softmax reverse rule consumes it.
        if constexpr (std::is_same<T, float>::value)
            return T{1.0e-7F};
        return T{1.0e-4F};
    }

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        record.output_gradient[0] = T{};
        fill(record.probability_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        const T seed = record.output_gradient[0];
        if (static_cast<float>(seed) == 0.0F) return;
        clip(record.probability_value, record.probability_gradient,
             minimum_probability(), maximum_value(), record.length);
        for (std::size_t i = 0; i < record.length; ++i)
        {
            const float denominator = static_cast<float>(record.probability_gradient[i]);
            float gradient = -static_cast<float>(seed) *
                             static_cast<float>(record.target_value[i]) /
                             denominator;
            if constexpr (!std::is_same<T, float>::value)
            {
                const float limit = static_cast<float>(maximum_value());
                if (gradient > limit) gradient = limit;
                if (gradient < -limit) gradient = -limit;
            }
            record.probability_gradient[i] = static_cast<T>(gradient);
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &probability,
                         const BufferView<T> &target) noexcept
    {
        if (!OperatorAccess<T>::valid(tape, output) ||
            OperatorAccess<T>::length(output) != 1U ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            !OperatorAccess<T>::compatible(tape, probability, target) ||
            OperatorAccess<T>::gradients(probability) == nullptr ||
            OperatorAccess<T>::role(target) != BufferRole::input)
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        return true;
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &probability,
                         const BufferView<T> &target) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess<T>::template require<CrossEntropyOperator<T>>(*tape))
            return false;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, probability, target))
            return false;
#endif

        const std::size_t length = OperatorAccess<T>::length(probability);
        T result = T{};
        if (length != 0U)
        {
            // Reuse the probability gradient as forward scratch. backward()
            // resets it before accumulating any gradient.
            T *scratch = OperatorAccess<T>::gradients(probability);
            clip(OperatorAccess<T>::values(probability), scratch, minimum_probability(),
                 maximum_value(), length);
            vlog(scratch, length);
            dot(OperatorAccess<T>::values(target), scratch, length, &result);
        }
        OperatorAccess<T>::values(output)[0] = static_cast<T>(-static_cast<float>(result));
        if (!OperatorAccess<T>::recording(*tape))
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->probability_value = OperatorAccess<T>::values(probability);
        record->probability_gradient = OperatorAccess<T>::gradients(probability);
        record->target_value = OperatorAccess<T>::values(target);
        record->length = length;
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class CrossEntropyExpression
{
public:
    CrossEntropyExpression(const BufferView<T> &probability,
                           const BufferView<T> &target) noexcept
        : probability_(probability), target_(target) {}

    void evaluate(BufferView<T> &output) const noexcept
    {
        CrossEntropyOperator<T>::evaluate(output, probability_, target_);
    }

private:
    BufferView<T> probability_;
    BufferView<T> target_;
};

template <typename T = float>
inline CrossEntropyExpression<T> cross_entropy(
    const BufferView<T> &probability, const BufferView<T> &target) noexcept
{
    return CrossEntropyExpression<T>(probability, target);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
