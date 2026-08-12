#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <arm_math_types_f16.h>
#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <limits>

namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class ReluOperator
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

    static void clip(const T *input, T *output, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_clip_f32(input, output, T{}, maximum_value(),
                         static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_clip_f16(input, output, T{}, maximum_value(),
                         static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i)
                output[i] = input[i] > T{} ? input[i] : T{};
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *input_value;
        T *input_gradient;
        std::size_t length;
    };

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
        for (std::size_t i = 0; i < record.length; ++i)
        {
            if (static_cast<float>(record.input_value[i]) > 0.0F)
            {
                if constexpr (std::is_same<T, float>::value)
                    record.input_gradient[i] += record.output_gradient[i];
                else
                    record.input_gradient[i] = static_cast<T>(
                        static_cast<float>(record.input_gradient[i]) +
                        static_cast<float>(record.output_gradient[i]));
            }
        }
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess<T>::template require<ReluOperator<T>>(*tape))
            return false;
        if (!OperatorAccess<T>::compatible(*tape, output, input) ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(input) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(input))
        {
            OperatorAccess<T>::fail(*tape, Status::tape_mismatch);
            return false;
        }
        clip(OperatorAccess<T>::values(input), OperatorAccess<T>::values(output),
             OperatorAccess<T>::length(input));
        if (!OperatorAccess<T>::recording(*tape) ||
            OperatorAccess<T>::length(output) == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->input_value = OperatorAccess<T>::values(input);
        record->input_gradient = OperatorAccess<T>::gradients(input);
        record->length = OperatorAccess<T>::length(input);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class ReluExpression
{
public:
    explicit ReluExpression(const BufferView<T> &input) noexcept : input_(input) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        ReluOperator<T>::evaluate(output, input_);
    }
private:
    BufferView<T> input_;
};

template <typename T = float>
inline ReluExpression<T> relu(const BufferView<T> &input) noexcept
{
    return ReluExpression<T>(input);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
