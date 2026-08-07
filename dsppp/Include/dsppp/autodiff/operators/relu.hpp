#pragma once

#include <dsppp/autodiff/reverse.hpp>

namespace arm_cmsis_dsp {
namespace autodiff {

class ReluOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *input_value;
        float *input_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        for (std::size_t i = 0; i < record.length; ++i)
        {
            record.output_gradient[i] = 0.0F;
            if (record.input_gradient != nullptr) record.input_gradient[i] = 0.0F;
        }
    }
    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.input_gradient == nullptr) return;
        for (std::size_t i = 0; i < record.length; ++i)
            if (record.input_value[i] > 0.0F)
                record.input_gradient[i] += record.output_gradient[i];
    }

public:
    static bool evaluate(BufferView &output, const BufferView &input) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<ReluOperator>(*tape))
            return false;
        if (!OperatorAccess::compatible(*tape, output, input) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::values(output) == OperatorAccess::values(input) ||
            OperatorAccess::gradients(output) == OperatorAccess::gradients(input))
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }
        for (std::size_t i = 0; i < OperatorAccess::length(input); ++i)
            OperatorAccess::values(output)[i] =
                OperatorAccess::values(input)[i] > 0.0F ?
                OperatorAccess::values(input)[i] : 0.0F;
        if (!OperatorAccess::recording(*tape) ||
            OperatorAccess::length(output) == 0U)
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->input_value = OperatorAccess::values(input);
        record->input_gradient = OperatorAccess::gradients(input);
        record->length = OperatorAccess::length(input);
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class ReluExpression
{
public:
    explicit ReluExpression(const BufferView &input) noexcept : input_(input) {}
    void evaluate(BufferView &output) const noexcept
    {
        ReluOperator::evaluate(output, input_);
    }
private:
    BufferView input_;
};

inline ReluExpression relu(const BufferView &input) noexcept
{
    return ReluExpression(input);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
