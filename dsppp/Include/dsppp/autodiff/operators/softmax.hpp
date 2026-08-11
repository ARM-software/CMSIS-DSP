#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/matrix.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/fast_math_functions.h>
#include <dsp/statistics_functions.h>
#include <dsp/support_functions.h>

namespace arm_cmsis_dsp {
namespace autodiff {

class SoftmaxOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *output_value;
        float *input_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        arm_fill_f32(0.0F, record.output_gradient, record.length);
        if (record.input_gradient != nullptr)
            arm_fill_f32(0.0F, record.input_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.input_gradient == nullptr) return;

        ::arm_cmsis_dsp::VectorView<float> output_gradient(
            record.output_gradient, 0, record.length);
        ::arm_cmsis_dsp::VectorView<float> output_value(
            const_cast<float *>(record.output_value), 0, record.length);
        ::arm_cmsis_dsp::VectorView<float> input_gradient(
            record.input_gradient, 0, record.length);
        const float projection =
            ::arm_cmsis_dsp::dot(output_gradient, output_value);
        input_gradient += output_value * (output_gradient - projection);
    }

public:
    static bool evaluate(BufferView &output, const BufferView &input) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<SoftmaxOperator>(*tape))
            return false;
        if (!OperatorAccess::compatible(*tape, output, input) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::values(output) == OperatorAccess::values(input) ||
            OperatorAccess::gradients(output) ==
                OperatorAccess::gradients(input))
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }

        const std::size_t length = OperatorAccess::length(input);
        if (length == 0U)
            return OperatorAccess::status(*tape) == Status::ok;

        const float log_sum = arm_logsumexp_f32(
            OperatorAccess::values(input), length);
        arm_offset_f32(OperatorAccess::values(input), -log_sum,
                       OperatorAccess::values(output), length);
        arm_vexp_f32(OperatorAccess::values(output),
                     OperatorAccess::values(output), length);
        if (!OperatorAccess::recording(*tape))
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->output_value = OperatorAccess::values(output);
        record->input_gradient = OperatorAccess::gradients(input);
        record->length = length;
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class SoftmaxExpression
{
public:
    explicit SoftmaxExpression(const BufferView &input) noexcept
        : input_(input) {}
    void evaluate(BufferView &output) const noexcept
    {
        SoftmaxOperator::evaluate(output, input_);
    }
private:
    BufferView input_;
};

inline SoftmaxExpression softmax(const BufferView &input) noexcept
{
    return SoftmaxExpression(input);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
