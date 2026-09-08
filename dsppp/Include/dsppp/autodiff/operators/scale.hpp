#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/matrix.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class ScaleOperator
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

    static void apply_scale(const T *input, T value, T *output,
                            std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_scale_f32(input, value, output, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_scale_f16(input, value, output, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i) output[i] = input[i] * value;
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *input_value;
        T *input_gradient;
        const T *scale_value;
        T *scale_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        fill(record.output_gradient, record.length);
        if (record.input_gradient != nullptr)
            fill(record.input_gradient, record.length);
        record.scale_gradient[0] = T{};
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        ::arm_cmsis_dsp::VectorView<T> output_gradient(
            record.output_gradient, 0, record.length);
        ::arm_cmsis_dsp::VectorView<T> input_value(
            const_cast<T *>(record.input_value), 0, record.length);
        const T contribution = static_cast<T>(::arm_cmsis_dsp::dot(output_gradient, input_value));
        if constexpr (std::is_same<T, float>::value)
            record.scale_gradient[0] += contribution;
        else
            record.scale_gradient[0] = static_cast<T>(
                static_cast<float>(record.scale_gradient[0]) +
                static_cast<float>(contribution));
        if (record.input_gradient != nullptr)
        {
            ::arm_cmsis_dsp::VectorView<T> input_gradient(
                record.input_gradient, 0, record.length);
            input_gradient += output_gradient * record.scale_value[0];
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &input,
                         const BufferView<T> &scale) noexcept
    {
        if (!OperatorAccess<T>::compatible(tape, output, input) ||
            !OperatorAccess<T>::valid(tape, scale) ||
            OperatorAccess<T>::length(scale) != 1U ||
            OperatorAccess<T>::role(scale) != BufferRole::parameter ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::gradients(scale) == nullptr ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(input) ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(scale) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(input) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(scale))
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        return true;
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input,
                         const BufferView<T> &scale) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess<T>::template require<ScaleOperator<T>>(*tape))
            return false;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, input, scale))
            return false;
#endif
        apply_scale(OperatorAccess<T>::values(input), OperatorAccess<T>::values(scale)[0],
                    OperatorAccess<T>::values(output), OperatorAccess<T>::length(output));
        if (!OperatorAccess<T>::recording(*tape) ||
            OperatorAccess<T>::length(output) == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->input_value = OperatorAccess<T>::values(input);
        record->input_gradient = OperatorAccess<T>::gradients(input);
        record->scale_value = OperatorAccess<T>::values(scale);
        record->scale_gradient = OperatorAccess<T>::gradients(scale);
        record->length = OperatorAccess<T>::length(output);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class ScaleExpression
{
public:
    ScaleExpression(const BufferView<T> &input, const BufferView<T> &scale) noexcept
        : input_(input), scale_(scale) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        ScaleOperator<T>::evaluate(output, input_, scale_);
    }
private:
    BufferView<T> input_;
    BufferView<T> scale_;
};

template <typename T = float>
inline ScaleExpression<T> scale(const BufferView<T> &input,
                                const BufferView<T> &constant) noexcept
{
    return ScaleExpression<T>(input, constant);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
