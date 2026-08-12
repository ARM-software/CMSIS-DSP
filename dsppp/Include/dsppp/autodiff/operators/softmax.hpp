#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/matrix.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/fast_math_functions.h>
#include <dsp/fast_math_functions_f16.h>
#include <dsp/statistics_functions.h>
#include <dsp/statistics_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <cmath>

namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class SoftmaxOperator
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

    static T logsumexp(const T *input, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            return arm_logsumexp_f32(input, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            return arm_logsumexp_f16(input, static_cast<uint32_t>(length));
#else
        else
        {
            float sum = 0.0F;
            for (std::size_t i = 0; i < length; ++i)
                sum += std::exp(static_cast<float>(input[i]));
            return static_cast<T>(std::log(sum));
        }
#endif
    }

    static void offset(const T *input, T value, T *output,
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

    static void exp(T *data, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_vexp_f32(data, data, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_vexp_f16(data, data, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i)
                data[i] = static_cast<T>(std::exp(static_cast<float>(data[i])));
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *output_value;
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

        ::arm_cmsis_dsp::VectorView<T> output_gradient(
            record.output_gradient, 0, record.length);
        ::arm_cmsis_dsp::VectorView<T> output_value(
            const_cast<T *>(record.output_value), 0, record.length);
        ::arm_cmsis_dsp::VectorView<T> input_gradient(
            record.input_gradient, 0, record.length);
        const T projection =
            ::arm_cmsis_dsp::dot(output_gradient, output_value);
        input_gradient += output_value * (output_gradient - projection);
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess<T>::template require<SoftmaxOperator<T>>(*tape))
            return false;
        if (!OperatorAccess<T>::compatible(*tape, output, input) ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(input) ||
            OperatorAccess<T>::gradients(output) ==
                OperatorAccess<T>::gradients(input))
        {
            OperatorAccess<T>::fail(*tape, Status::tape_mismatch);
            return false;
        }

        const std::size_t length = OperatorAccess<T>::length(input);
        if (length == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        const T log_sum = logsumexp(OperatorAccess<T>::values(input), length);
        offset(OperatorAccess<T>::values(input), static_cast<T>(-static_cast<float>(log_sum)),
               OperatorAccess<T>::values(output), length);
        exp(OperatorAccess<T>::values(output), length);
        if (!OperatorAccess<T>::recording(*tape))
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->output_value = OperatorAccess<T>::values(output);
        record->input_gradient = OperatorAccess<T>::gradients(input);
        record->length = length;
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class SoftmaxExpression
{
public:
    explicit SoftmaxExpression(const BufferView<T> &input) noexcept
        : input_(input) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        SoftmaxOperator<T>::evaluate(output, input_);
    }
private:
    BufferView<T> input_;
};

template <typename T = float>
inline SoftmaxExpression<T> softmax(const BufferView<T> &input) noexcept
{
    return SoftmaxExpression<T>(input);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
