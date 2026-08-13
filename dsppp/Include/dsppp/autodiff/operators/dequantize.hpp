#pragma once

#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/quantization_support.hpp>

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

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        for (std::size_t i = 0; i < record.length; ++i)
        {
            record.output_gradient[i] = T{};
            if (record.input_gradient != nullptr)
                record.input_gradient[i] = T{};
        }
        for (std::size_t i = 0; i < record.quantization.parameter_count(); ++i)
        {
            record.scale_gradient[i] = T{};
            if (record.zero_point_gradient != nullptr)
                record.zero_point_gradient[i] = T{};
        }
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        for (std::size_t i = 0; i < record.length; ++i)
        {
            const std::size_t p = record.quantization.parameter_index(i);
            const float gradient = static_cast<float>(record.output_gradient[i]);
            const float scale = static_cast<float>(record.scale_value[p]);
            const float zero = record.quantization.asymmetric()
                                   ? rounded(static_cast<float>(
                                         record.zero_point_value[p]))
                                   : 0.0F;
            if (record.input_gradient != nullptr)
                add(record.input_gradient[i], gradient * scale);
            add(record.scale_gradient[p],
                gradient * (static_cast<float>(record.input_value[i]) - zero));
            if (record.zero_point_gradient != nullptr &&
                record.quantization.asymmetric())
                add(record.zero_point_gradient[p], -gradient * scale);
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
        for (std::size_t i = 0; i < OperatorAccess<T>::length(output); ++i)
        {
            const std::size_t p = quantization.parameter_index(i);
            const float zero = quantization.asymmetric()
                                   ? rounded(static_cast<float>(
                                         OperatorAccess<T>::values(zero_point)[p]))
                                   : 0.0F;
            OperatorAccess<T>::values(output)[i] = static_cast<T>(
                (static_cast<float>(OperatorAccess<T>::values(input)[i]) - zero) *
                static_cast<float>(OperatorAccess<T>::values(scale)[p]));
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
