#pragma once

#include <dsppp/autodiff/reverse.hpp>

namespace arm_cmsis_dsp {
namespace autodiff {

class ScaleOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *input_value;
        float *alpha_gradient;
        float *beta_gradient;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        record.alpha_gradient[0] = 0.0F;
        for (std::size_t i = 0; i < record.length; ++i)
        {
            record.output_gradient[i] = 0.0F;
            record.beta_gradient[i] = 0.0F;
        }
    }
    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        for (std::size_t i = 0; i < record.length; ++i)
        {
            const float gradient = record.output_gradient[i];
            if (gradient != 0.0F)
                record.alpha_gradient[0] += gradient * record.input_value[i];
            record.beta_gradient[i] += gradient;
        }
    }

public:
    static bool evaluate(BufferView &output, const BufferView &input,
                         const BufferView &alpha,
                         const BufferView &beta) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<ScaleOperator>(*tape))
            return false;
        if (!OperatorAccess::valid(*tape, output) ||
            !OperatorAccess::valid(*tape, input) ||
            !OperatorAccess::valid(*tape, alpha) ||
            !OperatorAccess::valid(*tape, beta) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::length(output) != OperatorAccess::length(input) ||
            OperatorAccess::length(beta) != OperatorAccess::length(input) ||
            OperatorAccess::length(alpha) != 1U ||
            OperatorAccess::role(input) != BufferRole::input ||
            OperatorAccess::role(alpha) != BufferRole::parameter ||
            OperatorAccess::role(beta) != BufferRole::parameter)
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }
        const float alpha_value = OperatorAccess::values(alpha)[0];
        for (std::size_t i = 0; i < OperatorAccess::length(output); ++i)
            OperatorAccess::values(output)[i] = alpha_value *
                OperatorAccess::values(input)[i] + OperatorAccess::values(beta)[i];
        if (!OperatorAccess::recording(*tape) || OperatorAccess::length(output) == 0U)
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->input_value = OperatorAccess::values(input);
        record->alpha_gradient = OperatorAccess::gradients(alpha);
        record->beta_gradient = OperatorAccess::gradients(beta);
        record->length = OperatorAccess::length(output);
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class ScaleExpression
{
public:
    ScaleExpression(const BufferView &input, const BufferView &alpha,
                    const BufferView &beta) noexcept
        : input_(input), alpha_(alpha), beta_(beta) {}
    void evaluate(BufferView &output) const noexcept
    {
        ScaleOperator::evaluate(output, input_, alpha_, beta_);
    }
private:
    BufferView input_;
    BufferView alpha_;
    BufferView beta_;
};

inline ScaleExpression scale(const BufferView &input, const BufferView &alpha,
                             const BufferView &beta) noexcept
{
    return ScaleExpression(input, alpha, beta);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
