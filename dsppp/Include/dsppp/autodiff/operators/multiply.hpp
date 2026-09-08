#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/matrix.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class MultiplyOperator
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

    static void multiply(const T *left, const T *right, T *output,
                         std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_mult_f32(left, right, output, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_mult_f16(left, right, output, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i) output[i] = left[i] * right[i];
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *left_value;
        T *left_gradient;
        const T *right_value;
        T *right_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        fill(record.output_gradient, record.length);
        if (record.left_gradient != nullptr)
            fill(record.left_gradient, record.length);
        if (record.right_gradient != nullptr)
            fill(record.right_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        ::arm_cmsis_dsp::VectorView<T> output_gradient(
            record.output_gradient, 0, record.length);
        if (record.left_gradient != nullptr)
        {
            ::arm_cmsis_dsp::VectorView<T> left_gradient(
                record.left_gradient, 0, record.length);
            ::arm_cmsis_dsp::VectorView<T> right_value(
                const_cast<T *>(record.right_value), 0, record.length);
            left_gradient += output_gradient * right_value;
        }
        if (record.right_gradient != nullptr)
        {
            ::arm_cmsis_dsp::VectorView<T> right_gradient(
                record.right_gradient, 0, record.length);
            ::arm_cmsis_dsp::VectorView<T> left_value(
                const_cast<T *>(record.left_value), 0, record.length);
            right_gradient += output_gradient * left_value;
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &left,
                         const BufferView<T> &right) noexcept
    {
        if (!OperatorAccess<T>::compatible(tape, output, left) ||
            !OperatorAccess<T>::compatible(tape, output, right) ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(left) ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(right) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(left) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(right))
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        return true;
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &left,
                         const BufferView<T> &right) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess<T>::template require<MultiplyOperator<T>>(*tape))
            return false;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, left, right))
            return false;
#endif
        multiply(OperatorAccess<T>::values(left), OperatorAccess<T>::values(right),
                 OperatorAccess<T>::values(output), OperatorAccess<T>::length(output));
        if (!OperatorAccess<T>::recording(*tape) ||
            OperatorAccess<T>::length(output) == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->left_value = OperatorAccess<T>::values(left);
        record->left_gradient = OperatorAccess<T>::gradients(left);
        record->right_value = OperatorAccess<T>::values(right);
        record->right_gradient = OperatorAccess<T>::gradients(right);
        record->length = OperatorAccess<T>::length(output);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class MultiplyExpression
{
public:
    MultiplyExpression(const BufferView<T> &left, const BufferView<T> &right) noexcept
        : left_(left), right_(right) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        MultiplyOperator<T>::evaluate(output, left_, right_);
    }
private:
    BufferView<T> left_;
    BufferView<T> right_;
};

template <typename T = float>
inline MultiplyExpression<T> operator*(const BufferView<T> &left,
                                       const BufferView<T> &right) noexcept
{
    return MultiplyExpression<T>(left, right);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
