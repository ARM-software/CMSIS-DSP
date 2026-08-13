#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/dot.hpp>
#include <dsppp/autodiff/operators/quadratic_error.hpp>
#include <dsppp/autodiff/optimizers/sgd.hpp>

#include <cstdint>
#include <cstdio>

using namespace arm_cmsis_dsp::autodiff;

/*
 * Educational example only.
 *
 * This example deliberately expresses a least mean square (LMS) adaptive
 * filter with the generic autodiff API to show how per-sample reverse-mode
 * differentiation becomes online stochastic gradient descent. For a real LMS
 * filter, a specialized implementation such as arm_lms_f32 is more efficient:
 * it combines FIR state handling, output calculation, error calculation, and
 * coefficient updates directly, without tape records or a separate backward
 * pass. arm_lms_f32 accepts blocks but updates its coefficients after every
 * sample inside the block.
 */

static float next_excitation(std::uint32_t &state) noexcept
{
    state = state * 1664525U + 1013904223U;
    const std::uint32_t value = state >> 8;
    return static_cast<float>(value) * (2.0F / 16777215.0F) - 1.0F;
}

int main()
{
    constexpr std::size_t tap_count = 4U;
    constexpr std::size_t sample_count = 2000U;
    constexpr std::size_t measurement_count = 100U;
    constexpr float mu = 0.05F;

    // Unknown FIR system that the adaptive filter will identify. Element zero
    // multiplies the newest sample, element one the preceding sample, and so on.
    const float reference_coefficients[tap_count] = {
        0.6F, -0.3F, 0.2F, 0.1F};
    float coefficients_value[tap_count] = {};
    float input_state_value[tap_count] = {};
    float desired_value[1] = {};
    float output_value[1] = {};
    float loss_value[1] = {};

    Arena<512> arena;
    Tape<float> &tape = arena.tape();
    if (!tape.register_operator<DotOperator<float>>() ||
        !tape.register_operator<QuadraticErrorOperator<float>>())
    {
        std::printf("Failed to register autodiff operators\n");
        return 1;
    }

    BufferView coefficients = tape.parameter(coefficients_value);
    BufferView input_state = tape.input(input_state_value);
    BufferView desired = tape.input(desired_value);
    BufferView output = tape.output(output_value);
    BufferView loss = tape.output(loss_value);
    if (!tape.good())
    {
        std::printf("Autodiff setup failed (status=%u)\n",
                    static_cast<unsigned>(tape.status()));
        return 1;
    }

    // quadratic_error computes (output - desired)^2, whose coefficient
    // gradient contains a factor of two. learning_rate = mu/2 therefore gives
    // the classical LMS update: coefficient += mu * error * input_state.
    SGD<tap_count, 1U> optimizer(mu * 0.5F);
    if (!optimizer.add(coefficients))
    {
        std::printf("Failed to add LMS coefficients to SGD\n");
        return 1;
    }

    tape.begin_graph();
    std::uint32_t random_state = 1U;
    float final_error_sum = 0.0F;

    for (std::size_t sample = 0; sample < sample_count; ++sample)
    {
        // Maintain the FIR delay line in caller-owned memory.
        for (std::size_t tap = tap_count - 1U; tap > 0U; --tap)
            input_state_value[tap] = input_state_value[tap - 1U];
        input_state_value[0] = next_excitation(random_state);

        desired_value[0] = 0.0F;
        for (std::size_t tap = 0; tap < tap_count; ++tap)
            desired_value[0] +=
                reference_coefficients[tap] * input_state_value[tap];

        // Each sample creates a fresh two-node graph. Rewinding releases the
        // preceding sample's records while retaining all persistent views and
        // gradient buffers.
        if (!tape.rewind_graph()) return 1;
        output = dot(input_state, coefficients);
        loss = quadratic_error(output, desired);

        optimizer.zero_grad();
        if (!tape.backward(loss) || !optimizer.step()) return 1;

        if (sample >= sample_count - measurement_count)
            final_error_sum += loss_value[0];
    }

    std::printf("reference coefficients: {%g, %g, %g, %g}\n",
                static_cast<double>(reference_coefficients[0]),
                static_cast<double>(reference_coefficients[1]),
                static_cast<double>(reference_coefficients[2]),
                static_cast<double>(reference_coefficients[3]));
    std::printf("learned coefficients:   {%g, %g, %g, %g}\n",
                static_cast<double>(coefficients_value[0]),
                static_cast<double>(coefficients_value[1]),
                static_cast<double>(coefficients_value[2]),
                static_cast<double>(coefficients_value[3]));
    std::printf("mean squared error over final %u samples: %g\n",
                static_cast<unsigned>(measurement_count),
                static_cast<double>(final_error_sum / measurement_count));

    return tape.good() && optimizer.good() ? 0 : 1;
}
