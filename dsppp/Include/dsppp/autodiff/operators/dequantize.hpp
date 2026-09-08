#pragma once

#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/quantization_support.hpp>

#include <dsppp/matrix.hpp>

#include <dsp/statistics_functions.h>
#include <dsp/statistics_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <cmath>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Dequantize floating-point int8 codes back to the tape's floating type. */
template <typename T = float> class DequantizeOperator
{
    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *input_value;
        T *input_gradient;
        const T *scale_value;
        const T *zero_point_value;
        T *scale_gradient;
        T *zero_point_gradient;
        std::size_t length;
        Int8Quantization quantization;
    };

    static float rounded(float value) noexcept { return std::nearbyint(value); }

    static void add(T &destination, float contribution) noexcept
    {
        destination = static_cast<T>(static_cast<float>(destination) +
                                     contribution);
    }

    static void fill(T *data, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_fill_f32(0.0F, data, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else if constexpr (std::is_same<T, float16_t>::value)
            arm_fill_f16(static_cast<float16_t>(0.0F), data,
                         static_cast<uint32_t>(length));
#endif
        else
            for (std::size_t i = 0; i < length; ++i) data[i] = T{};
    }

    static T sum(const T *data, std::size_t length) noexcept
    {
        T result = T{};
        if constexpr (std::is_same<T, float>::value)
            arm_accumulate_f32(data, static_cast<uint32_t>(length), &result);
#if defined(ARM_FLOAT16_SUPPORTED)
        else if constexpr (std::is_same<T, float16_t>::value)
            arm_accumulate_f16(data, static_cast<uint32_t>(length), &result);
#endif
        else
            for (std::size_t i = 0; i < length; ++i) result += data[i];
        return result;
    }

    static std::size_t block_length(const Int8Quantization &quantization,
                                    std::size_t remaining) noexcept
    {
        if (quantization.parameter_count() == 1U)
            return remaining;
        return remaining < quantization.inner_size()
                   ? remaining
                   : quantization.inner_size();
    }

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        fill(record.output_gradient, record.length);
        if (record.input_gradient != nullptr)
            fill(record.input_gradient, record.length);
        fill(record.scale_gradient, record.quantization.parameter_count());
        if (record.zero_point_gradient != nullptr)
            fill(record.zero_point_gradient,
                 record.quantization.parameter_count());
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        for (std::size_t offset = 0; offset < record.length;)
        {
            const std::size_t p = record.quantization.parameter_index(offset);
            const std::size_t length = block_length(record.quantization,
                                                    record.length - offset);
            const T scale = record.scale_value[p];
            const T zero = record.quantization.asymmetric()
                               ? static_cast<T>(rounded(static_cast<float>(
                                     record.zero_point_value[p])))
                               : T{};
            ::arm_cmsis_dsp::VectorView<T> output_gradient(
                record.output_gradient + offset, 0, length);
            ::arm_cmsis_dsp::VectorView<T> input_value(
                const_cast<T *>(record.input_value) + offset, 0, length);
            if (record.input_gradient != nullptr)
            {
                ::arm_cmsis_dsp::VectorView<T> input_gradient(
                    record.input_gradient + offset, 0, length);
                input_gradient += output_gradient * scale;
            }
            add(record.scale_gradient[p], static_cast<float>(
                ::arm_cmsis_dsp::dot(output_gradient, input_value - zero)));
            if (record.zero_point_gradient != nullptr &&
                record.quantization.asymmetric())
                add(record.zero_point_gradient[p],
                    -static_cast<float>(scale) *
                     static_cast<float>(sum(record.output_gradient + offset,
                                            length)));
            offset += length;
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &input,
                         const BufferView<T> &scale,
                         const BufferView<T> &zero_point,
                         const Int8Quantization &quantization) noexcept
    {
        if (!OperatorAccess<T>::compatible(tape, output, input) ||
            !OperatorAccess<T>::valid(tape, scale) ||
            !OperatorAccess<T>::valid(tape, zero_point) ||
            OperatorAccess<T>::length(scale) != quantization.parameter_count() ||
            OperatorAccess<T>::length(zero_point) != quantization.parameter_count() ||
            OperatorAccess<T>::role(scale) != BufferRole::parameter ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::gradients(scale) == nullptr ||
            !quantization.valid_for(OperatorAccess<T>::length(input)) ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(input))
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        if (quantization.asymmetric() &&
            (OperatorAccess<T>::role(zero_point) != BufferRole::parameter ||
             OperatorAccess<T>::gradients(zero_point) == nullptr))
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        return true;
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input,
                         const BufferView<T> &scale,
                         const BufferView<T> &zero_point,
                         Int8Quantization quantization) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess<T>::template require<DequantizeOperator<T>>(*tape))
            return false;
        if (!quantization.valid_for(OperatorAccess<T>::length(input)))
        {
            OperatorAccess<T>::fail(*tape, Status::tape_mismatch);
            return false;
        }
        quantization.constrain_parameters(
            const_cast<T *>(OperatorAccess<T>::values(scale)),
            const_cast<T *>(OperatorAccess<T>::values(zero_point)));
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, input, scale, zero_point, quantization))
            return false;
#endif
        const T *input_value = OperatorAccess<T>::values(input);
        T *output_value = OperatorAccess<T>::values(output);
        const std::size_t output_length = OperatorAccess<T>::length(output);
        for (std::size_t offset = 0; offset < output_length;)
        {
            const std::size_t p = quantization.parameter_index(offset);
            const std::size_t length = block_length(
                quantization, output_length - offset);
            const T zero = quantization.asymmetric()
                               ? static_cast<T>(rounded(static_cast<float>(
                                     OperatorAccess<T>::values(zero_point)[p])))
                               : T{};
            ::arm_cmsis_dsp::VectorView<T> input_block(
                const_cast<T *>(input_value) + offset, 0, length);
            ::arm_cmsis_dsp::VectorView<T> output_block(
                output_value + offset, 0, length);
            output_block = (input_block - zero) *
                           OperatorAccess<T>::values(scale)[p];
            offset += length;
        }
        if (!OperatorAccess<T>::recording(*tape) ||
            OperatorAccess<T>::length(output) == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(
            *tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->input_value = OperatorAccess<T>::values(input);
        record->input_gradient = OperatorAccess<T>::gradients(input);
        record->scale_value = OperatorAccess<T>::values(scale);
        record->zero_point_value = OperatorAccess<T>::values(zero_point);
        record->scale_gradient = OperatorAccess<T>::gradients(scale);
        record->zero_point_gradient = quantization.asymmetric()
                                          ? OperatorAccess<T>::gradients(zero_point)
                                          : nullptr;
        record->length = OperatorAccess<T>::length(output);
        record->quantization = quantization;
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class DequantizeExpression
{
public:
    DequantizeExpression(const BufferView<T> &input, const BufferView<T> &scale,
                         const BufferView<T> &zero_point,
                         Int8Quantization quantization) noexcept
        : input_(input), scale_(scale), zero_point_(zero_point),
          quantization_(quantization) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        DequantizeOperator<T>::evaluate(output, input_, scale_, zero_point_,
                                        quantization_);
    }
private:
    BufferView<T> input_;
    BufferView<T> scale_;
    BufferView<T> zero_point_;
    Int8Quantization quantization_;
};

template <typename T = float>
inline DequantizeExpression<T> dequantize(
    const BufferView<T> &input, const BufferView<T> &scale,
    const BufferView<T> &zero_point,
    Int8Quantization quantization = Int8Quantization::activation()) noexcept
{
    return DequantizeExpression<T>(input, scale, zero_point, quantization);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
