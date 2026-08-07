#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/memory_pool.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>


namespace arm_cmsis_dsp {
namespace autodiff {

class DotOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *left_value;
        float *left_gradient;
        const float *right_value;
        float *right_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        record.output_gradient[0] = 0.0F;
        
        if (record.left_gradient != nullptr)
        {
            VectorView<float> left_grad(const_cast<float *>(record.left_gradient), 0, record.length);
            left_grad = 0.0F;
        }
        if (record.right_gradient != nullptr)
        {
            VectorView<float> right_grad(const_cast<float *>(record.right_gradient), 0, record.length);
            right_grad = 0.0F;
        }
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        const float gradient = record.output_gradient[0];
        if (gradient == 0.0F) return;
   
        if (record.left_gradient != nullptr)
        {
            VectorView<float> left_grad(const_cast<float *>(record.left_gradient)   , 0, record.length);
            VectorView<float> right_val(const_cast<float *>(record.right_value), 0, record.length);
            left_grad += right_val * gradient;
        }
        if (record.right_gradient != nullptr)
        {
            VectorView<float> right_grad(const_cast<float *>(record.right_gradient), 0, record.length);
            VectorView<float> left_val(const_cast<float *>(record.left_value), 0, record.length);
            right_grad += left_val * gradient;
        }
    }

public:
    static bool evaluate(BufferView &output, const BufferView &left,
                         const BufferView &right) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<DotOperator>(*tape))
            return false;
        if (!OperatorAccess::valid(*tape, output) ||
            OperatorAccess::length(output) != 1U ||
            OperatorAccess::gradients(output) == nullptr ||
            !OperatorAccess::compatible(*tape, left, right) ||
            OperatorAccess::values(output) == OperatorAccess::values(left) ||
            OperatorAccess::values(output) == OperatorAccess::values(right))
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }
        float value = 0.0F;
        //for (std::size_t i = 0; i < OperatorAccess::length(left); ++i)
        //    value += OperatorAccess::values(left)[i] * OperatorAccess::values(right)[i];

        arm_dot_prod_f32(OperatorAccess::values(left), OperatorAccess::values(right), OperatorAccess::length(left),&value);
        OperatorAccess::values(output)[0] = value;
        if (!OperatorAccess::recording(*tape))
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->left_value = OperatorAccess::values(left);
        record->left_gradient = OperatorAccess::gradients(left);
        record->right_value = OperatorAccess::values(right);
        record->right_gradient = OperatorAccess::gradients(right);
        record->length = OperatorAccess::length(left);
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class DotExpression
{
public:
    DotExpression(const BufferView &left, const BufferView &right) noexcept
        : left_(left), right_(right) {}
    void evaluate(BufferView &output) const noexcept
    {
        DotOperator::evaluate(output, left_, right_);
    }
private:
    BufferView left_;
    BufferView right_;
};

inline DotExpression dot(const BufferView &left, const BufferView &right) noexcept
{
    return DotExpression(left, right);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp

