#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/fast_math_functions.h>
#include <dsp/statistics_functions.h>
#include <dsp/support_functions.h>

#include <limits>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Categorical cross entropy: -sum(target[i] * log(probability[i])). */
class CrossEntropyOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *probability_value;
        float *probability_gradient;
        const float *target_value;
        std::size_t length;
    };

    static constexpr float minimum_probability = 1.0e-7F;

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        record.output_gradient[0] = 0.0F;
        arm_fill_f32(0.0F, record.probability_gradient, record.length);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        const float seed = record.output_gradient[0];
        if (seed == 0.0F) return;
        arm_clip_f32(record.probability_value, record.probability_gradient,
                     minimum_probability,
                     std::numeric_limits<float>::max(), record.length);
        for (std::size_t i = 0; i < record.length; ++i)
        {
            record.probability_gradient[i] =
                -seed * record.target_value[i] /
                record.probability_gradient[i];
        }
    }

public:
    static bool evaluate(BufferView &output, const BufferView &probability,
                         const BufferView &target) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess::require<CrossEntropyOperator>(*tape))
            return false;
        if (!OperatorAccess::valid(*tape, output) ||
            OperatorAccess::length(output) != 1U ||
            OperatorAccess::gradients(output) == nullptr ||
            !OperatorAccess::compatible(*tape, probability, target) ||
            OperatorAccess::gradients(probability) == nullptr ||
            OperatorAccess::role(target) != BufferRole::input)
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }

        const std::size_t length = OperatorAccess::length(probability);
        float result = 0.0F;
        if (length != 0U)
        {
            // Reuse the probability gradient as forward scratch. backward()
            // resets it before accumulating any gradient.
            float *scratch = OperatorAccess::gradients(probability);
            arm_clip_f32(OperatorAccess::values(probability), scratch,
                         minimum_probability,
                         std::numeric_limits<float>::max(), length);
            arm_vlog_f32(scratch, scratch, length);
            arm_dot_prod_f32(OperatorAccess::values(target), scratch, length,
                             &result);
        }
        OperatorAccess::values(output)[0] = -result;
        if (!OperatorAccess::recording(*tape))
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->probability_value = OperatorAccess::values(probability);
        record->probability_gradient = OperatorAccess::gradients(probability);
        record->target_value = OperatorAccess::values(target);
        record->length = length;
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class CrossEntropyExpression
{
public:
    CrossEntropyExpression(const BufferView &probability,
                           const BufferView &target) noexcept
        : probability_(probability), target_(target) {}

    void evaluate(BufferView &output) const noexcept
    {
        CrossEntropyOperator::evaluate(output, probability_, target_);
    }

private:
    BufferView probability_;
    BufferView target_;
};

inline CrossEntropyExpression cross_entropy(
    const BufferView &probability, const BufferView &target) noexcept
{
    return CrossEntropyExpression(probability, target);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
