#pragma once

#include <dsppp/autodiff/reverse.hpp>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Sum-of-squared-errors loss: sum((prediction - target)^2). */
class QuadraticErrorOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *prediction_value;
        float *prediction_gradient;
        const float *target_value;
        std::size_t length;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        record.output_gradient[0] = 0.0F;
        for (std::size_t i = 0; i < record.length; ++i)
            record.prediction_gradient[i] = 0.0F;
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        const float seed = record.output_gradient[0];
        if (seed == 0.0F) return;
        for (std::size_t i = 0; i < record.length; ++i)
            record.prediction_gradient[i] += 2.0F * seed *
                (record.prediction_value[i] - record.target_value[i]);
    }

public:
    static bool evaluate(BufferView &output, const BufferView &prediction,
                         const BufferView &target) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess::require<QuadraticErrorOperator>(*tape))
            return false;
        if (!OperatorAccess::valid(*tape, output) ||
            OperatorAccess::length(output) != 1U ||
            OperatorAccess::gradients(output) == nullptr ||
            !OperatorAccess::compatible(*tape, prediction, target) ||
            OperatorAccess::gradients(prediction) == nullptr ||
            OperatorAccess::role(target) != BufferRole::input)
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }

        float value = 0.0F;
        for (std::size_t i = 0; i < OperatorAccess::length(prediction); ++i)
        {
            const float error = OperatorAccess::values(prediction)[i] -
                                OperatorAccess::values(target)[i];
            value += error * error;
        }
        OperatorAccess::values(output)[0] = value;
        if (!OperatorAccess::recording(*tape))
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->prediction_value = OperatorAccess::values(prediction);
        record->prediction_gradient = OperatorAccess::gradients(prediction);
        record->target_value = OperatorAccess::values(target);
        record->length = OperatorAccess::length(prediction);
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class QuadraticErrorExpression
{
public:
    QuadraticErrorExpression(const BufferView &prediction,
                             const BufferView &target) noexcept
        : prediction_(prediction), target_(target) {}
    void evaluate(BufferView &output) const noexcept
    {
        QuadraticErrorOperator::evaluate(output, prediction_, target_);
    }
private:
    BufferView prediction_;
    BufferView target_;
};

inline QuadraticErrorExpression quadratic_error(
    const BufferView &prediction, const BufferView &target) noexcept
{
    return QuadraticErrorExpression(prediction, target);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
