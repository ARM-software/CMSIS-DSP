#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/support_functions.h>

#include <cstdint>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Caller-owned deterministic random state used by dropout. */
class DropoutGenerator
{
public:
    explicit DropoutGenerator(std::uint32_t seed = 0x6D2B79F5U) noexcept
        : state_(seed == 0U ? 0x6D2B79F5U : seed) {}

    void seed(std::uint32_t value) noexcept
    {
        state_ = value == 0U ? 0x6D2B79F5U : value;
    }

private:
    std::uint32_t state_;
    friend class DropoutOperator;
};

/** Inverted dropout during recording, identity when recording is disabled. */
class DropoutOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        float *input_gradient;
        std::size_t length;
        std::uint32_t random_state;
        float drop_probability;
        float scale;
    };

    static std::uint32_t next(std::uint32_t &state) noexcept
    {
        state ^= state << 13U;
        state ^= state >> 17U;
        state ^= state << 5U;
        return state;
    }

    static bool keep(std::uint32_t &state, float drop_probability) noexcept
    {
        constexpr float inverse_24_bit_range = 1.0F / 16777216.0F;
        const float uniform =
            static_cast<float>(next(state) >> 8U) * inverse_24_bit_range;
        return uniform >= drop_probability;
    }

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

        if (record.drop_probability == 0.0F)
        {
            arm_add_f32(record.input_gradient, record.output_gradient,
                        record.input_gradient, record.length);
            return;
        }

        std::uint32_t state = record.random_state;
        for (std::size_t i = 0; i < record.length; ++i)
            if (keep(state, record.drop_probability))
                record.input_gradient[i] +=
                    record.output_gradient[i] * record.scale;
    }

public:
    static bool evaluate(BufferView &output, const BufferView &input,
                         DropoutGenerator &generator,
                         float drop_probability) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr || !OperatorAccess::require<DropoutOperator>(*tape))
            return false;
        if (!OperatorAccess::compatible(*tape, output, input) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::values(output) == OperatorAccess::values(input) ||
            OperatorAccess::gradients(output) ==
                OperatorAccess::gradients(input) ||
            !(drop_probability >= 0.0F && drop_probability < 1.0F))
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }

        const std::size_t length = OperatorAccess::length(output);
        if (!OperatorAccess::recording(*tape))
        {
            arm_copy_f32(OperatorAccess::values(input),
                         OperatorAccess::values(output), length);
            return OperatorAccess::status(*tape) == Status::ok;
        }
        if (length == 0U)
            return OperatorAccess::status(*tape) == Status::ok;

        const float scale = 1.0F / (1.0F - drop_probability);
        const std::uint32_t initial_state = generator.state_;
        if (drop_probability == 0.0F)
        {
            arm_copy_f32(OperatorAccess::values(input),
                         OperatorAccess::values(output), length);
        }
        else
        {
            for (std::size_t i = 0; i < length; ++i)
                OperatorAccess::values(output)[i] =
                    keep(generator.state_, drop_probability)
                        ? OperatorAccess::values(input)[i] * scale
                        : 0.0F;
        }

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr)
        {
            generator.state_ = initial_state;
            return false;
        }
        record->output_gradient = OperatorAccess::gradients(output);
        record->input_gradient = OperatorAccess::gradients(input);
        record->length = length;
        record->random_state = initial_state;
        record->drop_probability = drop_probability;
        record->scale = scale;
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class DropoutExpression
{
public:
    DropoutExpression(const BufferView &input, DropoutGenerator &generator,
                      float drop_probability) noexcept
        : input_(input), generator_(generator),
          drop_probability_(drop_probability) {}

    void evaluate(BufferView &output) const noexcept
    {
        DropoutOperator::evaluate(output, input_, generator_,
                                  drop_probability_);
    }

private:
    BufferView input_;
    DropoutGenerator &generator_;
    float drop_probability_;
};

inline DropoutExpression dropout(const BufferView &input,
                                 DropoutGenerator &generator,
                                 float drop_probability) noexcept
{
    return DropoutExpression(input, generator, drop_probability);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
