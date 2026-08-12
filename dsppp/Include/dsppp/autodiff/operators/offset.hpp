#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/statistics_functions.h>
#include <dsp/statistics_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class OffsetOperator
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

    static void apply_offset(const T *input, T value, T *output,
                             std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_offset_f32(input, value, output, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_offset_f16(input, value, output, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i) output[i] = input[i] + value;
#endif
    }

    static void accumulate(const T *input, std::size_t length, T *result) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_accumulate_f32(input, static_cast<uint32_t>(length), result);
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_accumulate_f16(input, static_cast<uint32_t>(length), result);
#else
        else
            for (std::size_t i = 0; i < length; ++i) *result += input[i];
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        T *input_gradient;
        T *offset_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        fill(record.output_gradient, record.length);
        if (record.input_gradient != nullptr)
            fill(record.input_gradient, record.length);
        record.offset_gradient[0] = T{};
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.input_gradient != nullptr)
            add(record.input_gradient, record.output_gradient,
                record.input_gradient, record.length);
        T gradient_sum = T{};
        accumulate(record.output_gradient, record.length, &gradient_sum);
        if constexpr (std::is_same<T, float>::value)
            record.offset_gradient[0] += gradient_sum;
        else
            record.offset_gradient[0] = static_cast<T>(
                static_cast<float>(record.offset_gradient[0]) +
                static_cast<float>(gradient_sum));
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input,
                         const BufferView<T> &offset) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess<T>::template require<OffsetOperator<T>>(*tape))
            return false;
        if (!OperatorAccess<T>::compatible(*tape, output, input) ||
            !OperatorAccess<T>::valid(*tape, offset) ||
            OperatorAccess<T>::length(offset) != 1U ||
            OperatorAccess<T>::role(offset) != BufferRole::parameter ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::gradients(offset) == nullptr ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(input) ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(offset) ||
            OperatorAccess<T>::gradients(output) ==
                OperatorAccess<T>::gradients(input) ||
            OperatorAccess<T>::gradients(output) ==
                OperatorAccess<T>::gradients(offset))
        {
            OperatorAccess<T>::fail(*tape, Status::tape_mismatch);
            return false;
        }
        apply_offset(OperatorAccess<T>::values(input), OperatorAccess<T>::values(offset)[0],
                     OperatorAccess<T>::values(output), OperatorAccess<T>::length(output));
        if (!OperatorAccess<T>::recording(*tape) ||
            OperatorAccess<T>::length(output) == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->input_gradient = OperatorAccess<T>::gradients(input);
        record->offset_gradient = OperatorAccess<T>::gradients(offset);
        record->length = OperatorAccess<T>::length(output);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class OffsetExpression
{
public:
    OffsetExpression(const BufferView<T> &input, const BufferView<T> &offset) noexcept
        : input_(input), offset_(offset) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        OffsetOperator<T>::evaluate(output, input_, offset_);
    }
private:
    BufferView<T> input_;
    BufferView<T> offset_;
};

template <typename T = float>
inline OffsetExpression<T> offset(const BufferView<T> &input,
                                  const BufferView<T> &constant) noexcept
{
    return OffsetExpression<T>(input, constant);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
