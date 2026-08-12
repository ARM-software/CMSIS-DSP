#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/matrix.hpp>

#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Sum-of-squared-errors loss: sum((prediction - target)^2). */
template <typename T = float> class QuadraticErrorOperator
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

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *prediction_value;
        T *prediction_gradient;
        const T *target_value;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        record.output_gradient[0] = T{};
        fill(record.prediction_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        const T seed = record.output_gradient[0];
        if (static_cast<float>(seed) == 0.0F) return;
        ::arm_cmsis_dsp::VectorView<T> prediction_gradient(
            record.prediction_gradient, 0, record.length);
        ::arm_cmsis_dsp::VectorView<T> prediction_value(
            const_cast<T *>(record.prediction_value), 0, record.length);
        ::arm_cmsis_dsp::VectorView<T> target_value(
            const_cast<T *>(record.target_value), 0, record.length);
        prediction_gradient +=
            (prediction_value - target_value) * static_cast<T>(2.0F * static_cast<float>(seed));
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &prediction,
                         const BufferView<T> &target) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess<T>::template require<QuadraticErrorOperator<T>>(*tape))
            return false;
        if (!OperatorAccess<T>::valid(*tape, output) ||
            OperatorAccess<T>::length(output) != 1U ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            !OperatorAccess<T>::compatible(*tape, prediction, target) ||
            OperatorAccess<T>::gradients(prediction) == nullptr ||
            OperatorAccess<T>::role(target) != BufferRole::input)
        {
            OperatorAccess<T>::fail(*tape, Status::tape_mismatch);
            return false;
        }

        const std::size_t length = OperatorAccess<T>::length(prediction);
        ::arm_cmsis_dsp::VectorView<T> prediction_value(
            const_cast<T *>(OperatorAccess<T>::values(prediction)), 0,
            length);
        ::arm_cmsis_dsp::VectorView<T> target_value(
            const_cast<T *>(OperatorAccess<T>::values(target)), 0, length);
        const auto error = prediction_value - target_value;
        OperatorAccess<T>::values(output)[0] =
            ::arm_cmsis_dsp::dot(error, error);
        if (!OperatorAccess<T>::recording(*tape))
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->prediction_value = OperatorAccess<T>::values(prediction);
        record->prediction_gradient = OperatorAccess<T>::gradients(prediction);
        record->target_value = OperatorAccess<T>::values(target);
        record->length = OperatorAccess<T>::length(prediction);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class QuadraticErrorExpression
{
public:
    QuadraticErrorExpression(const BufferView<T> &prediction,
                             const BufferView<T> &target) noexcept
        : prediction_(prediction), target_(target) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        QuadraticErrorOperator<T>::evaluate(output, prediction_, target_);
    }
private:
    BufferView<T> prediction_;
    BufferView<T> target_;
};

template <typename T = float>
inline QuadraticErrorExpression<T> quadratic_error(
    const BufferView<T> &prediction, const BufferView<T> &target) noexcept
{
    return QuadraticErrorExpression<T>(prediction, target);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
