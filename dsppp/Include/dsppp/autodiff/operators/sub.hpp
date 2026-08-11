#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/support_functions.h>

namespace arm_cmsis_dsp {
namespace autodiff {

class SubOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        float *left_gradient;
        float *right_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        arm_fill_f32(0.0F, record.output_gradient, record.length);
        if (record.left_gradient != nullptr)
            arm_fill_f32(0.0F, record.left_gradient, record.length);
        if (record.right_gradient != nullptr)
            arm_fill_f32(0.0F, record.right_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.left_gradient != nullptr)
            arm_add_f32(record.left_gradient, record.output_gradient,
                        record.left_gradient, record.length);
        if (record.right_gradient != nullptr)
            arm_sub_f32(record.right_gradient, record.output_gradient,
                        record.right_gradient, record.length);
    }

public:
    static bool evaluate(BufferView &output, const BufferView &left,
                         const BufferView &right) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<SubOperator>(*tape))
            return false;
        if (!OperatorAccess::compatible(*tape, output, left) ||
            !OperatorAccess::compatible(*tape, output, right) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::values(output) == OperatorAccess::values(left) ||
            OperatorAccess::values(output) == OperatorAccess::values(right) ||
            OperatorAccess::gradients(output) ==
                OperatorAccess::gradients(left) ||
            OperatorAccess::gradients(output) ==
                OperatorAccess::gradients(right))
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }
        arm_sub_f32(OperatorAccess::values(left), OperatorAccess::values(right),
                    OperatorAccess::values(output),
                    OperatorAccess::length(output));
        if (!OperatorAccess::recording(*tape) ||
            OperatorAccess::length(output) == 0U)
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->left_gradient = OperatorAccess::gradients(left);
        record->right_gradient = OperatorAccess::gradients(right);
        record->length = OperatorAccess::length(output);
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class SubExpression
{
public:
    SubExpression(const BufferView &left, const BufferView &right) noexcept
        : left_(left), right_(right) {}
    void evaluate(BufferView &output) const noexcept
    {
        SubOperator::evaluate(output, left_, right_);
    }
private:
    BufferView left_;
    BufferView right_;
};

inline SubExpression operator-(const BufferView &left,
                               const BufferView &right) noexcept
{
    return SubExpression(left, right);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
