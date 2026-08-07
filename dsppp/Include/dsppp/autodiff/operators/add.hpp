#pragma once

#include <dsppp/autodiff/reverse.hpp>

namespace arm_cmsis_dsp {
namespace autodiff {

class AddOperator
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
        for (std::size_t i = 0; i < record.length; ++i)
        {
            record.output_gradient[i] = 0.0F;
            if (record.left_gradient != nullptr) record.left_gradient[i] = 0.0F;
            if (record.right_gradient != nullptr) record.right_gradient[i] = 0.0F;
        }
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        for (std::size_t i = 0; i < record.length; ++i)
        {
            if (record.left_gradient != nullptr)
                record.left_gradient[i] += record.output_gradient[i];
            if (record.right_gradient != nullptr)
                record.right_gradient[i] += record.output_gradient[i];
        }
    }

public:
    static bool evaluate(BufferView &output, const BufferView &left,
                         const BufferView &right) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<AddOperator>(*tape))
            return false;
        if (!OperatorAccess::compatible(*tape, output, left) ||
            !OperatorAccess::compatible(*tape, output, right) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::values(output) == OperatorAccess::values(left) ||
            OperatorAccess::values(output) == OperatorAccess::values(right) ||
            OperatorAccess::gradients(output) == OperatorAccess::gradients(left) ||
            OperatorAccess::gradients(output) == OperatorAccess::gradients(right))
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }
        for (std::size_t i = 0; i < OperatorAccess::length(output); ++i)
            OperatorAccess::values(output)[i] =
                OperatorAccess::values(left)[i] + OperatorAccess::values(right)[i];
        if (!OperatorAccess::recording(*tape) || OperatorAccess::length(output) == 0U)
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

class AddExpression
{
public:
    AddExpression(const BufferView &left, const BufferView &right) noexcept
        : left_(left), right_(right) {}
    void evaluate(BufferView &output) const noexcept
    {
        AddOperator::evaluate(output, left_, right_);
    }
private:
    BufferView left_;
    BufferView right_;
};

inline AddExpression operator+(const BufferView &left,
                               const BufferView &right) noexcept
{
    return AddExpression(left, right);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp

