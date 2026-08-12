#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class AddOperator
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

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        T *left_gradient;
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
        if (record.left_gradient != nullptr)
            add(record.left_gradient, record.output_gradient,
                record.left_gradient, record.length);
        if (record.right_gradient != nullptr)
            add(record.right_gradient, record.output_gradient,
                record.right_gradient, record.length);
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &left,
                         const BufferView<T> &right) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess<T>::template require<AddOperator<T>>(*tape))
            return false;
        if (!OperatorAccess<T>::compatible(*tape, output, left) ||
            !OperatorAccess<T>::compatible(*tape, output, right) ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(left) ||
            OperatorAccess<T>::values(output) == OperatorAccess<T>::values(right) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(left) ||
            OperatorAccess<T>::gradients(output) == OperatorAccess<T>::gradients(right))
        {
            OperatorAccess<T>::fail(*tape, Status::tape_mismatch);
            return false;
        }
        add(OperatorAccess<T>::values(left), OperatorAccess<T>::values(right),
            OperatorAccess<T>::values(output), OperatorAccess<T>::length(output));
        if (!OperatorAccess<T>::recording(*tape) || OperatorAccess<T>::length(output) == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->left_gradient = OperatorAccess<T>::gradients(left);
        record->right_gradient = OperatorAccess<T>::gradients(right);
        record->length = OperatorAccess<T>::length(output);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class AddExpression
{
public:
    AddExpression(const BufferView<T> &left, const BufferView<T> &right) noexcept
        : left_(left), right_(right) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        AddOperator<T>::evaluate(output, left_, right_);
    }
private:
    BufferView<T> left_;
    BufferView<T> right_;
};

template <typename T = float>
inline AddExpression<T> operator+(const BufferView<T> &left,
                                  const BufferView<T> &right) noexcept
{
    return AddExpression<T>(left, right);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
