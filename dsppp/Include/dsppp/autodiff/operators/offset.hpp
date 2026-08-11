#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/statistics_functions.h>
#include <dsp/support_functions.h>

namespace arm_cmsis_dsp {
namespace autodiff {

class OffsetOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        float *input_gradient;
        float *offset_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        arm_fill_f32(0.0F, record.output_gradient, record.length);
        if (record.input_gradient != nullptr)
            arm_fill_f32(0.0F, record.input_gradient, record.length);
        record.offset_gradient[0] = 0.0F;
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.input_gradient != nullptr)
            arm_add_f32(record.input_gradient, record.output_gradient,
                        record.input_gradient, record.length);
        float gradient_sum = 0.0F;
        arm_accumulate_f32(record.output_gradient, record.length,
                           &gradient_sum);
        record.offset_gradient[0] += gradient_sum;
    }

public:
    static bool evaluate(BufferView &output, const BufferView &input,
                         const BufferView &offset) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<OffsetOperator>(*tape))
            return false;
        if (!OperatorAccess::compatible(*tape, output, input) ||
            !OperatorAccess::valid(*tape, offset) ||
            OperatorAccess::length(offset) != 1U ||
            OperatorAccess::role(offset) != BufferRole::parameter ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::gradients(offset) == nullptr ||
            OperatorAccess::values(output) == OperatorAccess::values(input) ||
            OperatorAccess::values(output) == OperatorAccess::values(offset) ||
            OperatorAccess::gradients(output) ==
                OperatorAccess::gradients(input) ||
            OperatorAccess::gradients(output) ==
                OperatorAccess::gradients(offset))
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }
        arm_offset_f32(OperatorAccess::values(input),
                       OperatorAccess::values(offset)[0],
                       OperatorAccess::values(output),
                       OperatorAccess::length(output));
        if (!OperatorAccess::recording(*tape) ||
            OperatorAccess::length(output) == 0U)
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->input_gradient = OperatorAccess::gradients(input);
        record->offset_gradient = OperatorAccess::gradients(offset);
        record->length = OperatorAccess::length(output);
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class OffsetExpression
{
public:
    OffsetExpression(const BufferView &input, const BufferView &offset) noexcept
        : input_(input), offset_(offset) {}
    void evaluate(BufferView &output) const noexcept
    {
        OffsetOperator::evaluate(output, input_, offset_);
    }
private:
    BufferView input_;
    BufferView offset_;
};

inline OffsetExpression offset(const BufferView &input,
                               const BufferView &constant) noexcept
{
    return OffsetExpression(input, constant);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
