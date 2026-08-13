#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/memory_pool.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/statistics_functions.h>
#include <dsp/statistics_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>


namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class DotOperator
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
            for (std::size_t i = 0; i < length; ++i)
                *result += left[i] * right[i];
        }
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
        record.output_gradient[0] = T{};
        if (record.left_gradient != nullptr)
            fill(record.left_gradient, record.length);
        if (record.right_gradient != nullptr)
            fill(record.right_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        const T gradient = record.output_gradient[0];
        if (static_cast<float>(gradient) == 0.0F) return;
   
        if (record.left_gradient != nullptr)
        {
            VectorView<T> left_grad(record.left_gradient, 0, record.length);
            VectorView<T> right_val(const_cast<T *>(record.right_value), 0, record.length);
            left_grad += right_val * gradient;
        }
        if (record.right_gradient != nullptr)
        {
            VectorView<T> right_grad(record.right_gradient, 0, record.length);
            VectorView<T> left_val(const_cast<T *>(record.left_value), 0, record.length);
            right_grad += left_val * gradient;
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &left,
                         const BufferView<T> &right) noexcept
    {
        if (!OperatorAccess<T>::valid(tape, output) ||
            OperatorAccess<T>::length(output) != 1U ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            !OperatorAccess<T>::compatible(tape, left, right) ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(left) ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(right))
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
        if (tape == nullptr || !OperatorAccess<T>::template require<DotOperator<T>>(*tape))
            return false;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, left, right))
            return false;
#endif
        T value = T{};
        dot(OperatorAccess<T>::values(left), OperatorAccess<T>::values(right),
            OperatorAccess<T>::length(left), &value);
        OperatorAccess<T>::values(output)[0] = value;
        if (!OperatorAccess<T>::recording(*tape))
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->left_value = OperatorAccess<T>::values(left);
        record->left_gradient = OperatorAccess<T>::gradients(left);
        record->right_value = OperatorAccess<T>::values(right);
        record->right_gradient = OperatorAccess<T>::gradients(right);
        record->length = OperatorAccess<T>::length(left);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class DotExpression
{
public:
    DotExpression(const BufferView<T> &left, const BufferView<T> &right) noexcept
        : left_(left), right_(right) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        DotOperator<T>::evaluate(output, left_, right_);
    }
private:
    BufferView<T> left_;
    BufferView<T> right_;
};

template <typename T = float>
inline DotExpression<T> dot(const BufferView<T> &left, const BufferView<T> &right) noexcept
{
    return DotExpression<T>(left, right);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
