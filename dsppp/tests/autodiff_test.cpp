#include "test_config.h"

extern "C" {
    extern void autodiff_test();
}

#if defined(AUTODIFF_TEST) && defined(F32_DT) && defined(DYNAMIC_TEST)

#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/add.hpp>
#include <dsppp/autodiff/operators/dot.hpp>
#include <dsppp/autodiff/operators/fully_connected.hpp>
#include <dsppp/autodiff/operators/relu.hpp>
#include <dsppp/autodiff/operators/scale.hpp>
#include <dsppp/autodiff/operators/quadratic_error.hpp>
#include <dsppp/autodiff/optimizers/adam.hpp>
#include <dsppp/autodiff/optimizers/rmsprop.hpp>

#include <cstdio>

using namespace arm_cmsis_dsp::autodiff;

// Board Release builds define NDEBUG, so the standard assert macro cannot be
// used as a test oracle. Keep the existing concise checks but make failures
// visible to run_all.py, which treats output containing "Error" as a failure.
#define AUTODIFF_CHECK(condition)                                            \
    do                                                                       \
    {                                                                        \
        if (!(condition))                                                    \
        {                                                                    \
            std::printf("Error: autodiff check failed at line %u\r\n",       \
                        static_cast<unsigned>(__LINE__));                    \
            return;                                                          \
        }                                                                    \
    } while (false)

#ifdef assert
#undef assert
#endif
#define assert(condition) AUTODIFF_CHECK(condition)

static void run_autodiff_tests()
{
    // Vector add followed by dot. Values and outputs belong to the caller;
    // gradients and two fixed-size operation records use the tape arena.
    Arena<2048> buffer_arena;
    Tape &buffer_tape = buffer_arena.tape();
    buffer_tape.register_operator<AddOperator>();
    buffer_tape.register_operator<DotOperator>();
    float x_value[] = {1.0F, 2.0F, 3.0F};
    float w_value[] = {4.0F, 5.0F, 6.0F};
    float sum_value[3] = {};
    float result_value[1] = {};

    BufferView x_view = buffer_tape.view(x_value);
    BufferView w_view = buffer_tape.view(w_value);
    BufferView sum_view = buffer_tape.view(sum_value);
    BufferView result_view = buffer_tape.view(result_value);
    const std::size_t gradients_end = buffer_tape.used();
    assert(gradients_end >= 10U * sizeof(float));

    {
        RecordingScope no_gradient(buffer_tape, false);
        sum_view = x_view + w_view;
        result_view = dot(sum_view, w_view);
        assert(result_value[0] == 109.0F);
    }
    assert(buffer_tape.used() == gradients_end);

    sum_view = x_view + w_view;
    result_view = dot(sum_view, w_view);
    assert(buffer_tape.good());
    assert(buffer_tape.backward(result_view));
    for (std::size_t i = 0; i < 3; ++i)
    {
        assert(x_view.gradient(i) == w_value[i]);
        assert(w_view.gradient(i) == x_value[i] + 2.0F * w_value[i]);
    }

    // A vector output accepts a caller-provided vector-Jacobian seed.
    const float sum_seed[] = {1.0F, 2.0F, 3.0F};
    assert(buffer_tape.backward(sum_view, sum_seed, 3));
    for (std::size_t i = 0; i < 3; ++i)
    {
        assert(x_view.gradient(i) == sum_seed[i]);
        assert(w_view.gradient(i) == sum_seed[i]);
    }

    // Only parameters receive final gradients. The x input has no gradient
    // allocation, while alpha and beta are trainable parameters.
    Arena<2048> parameter_arena;
    Tape &parameter_tape = parameter_arena.tape();
    parameter_tape.register_operator<ScaleOperator>();
    parameter_tape.register_operator<AddOperator>();
    parameter_tape.register_operator<DotOperator>();
    float input_value[] = {1.0F, 2.0F, 3.0F};
    float alpha_value = 2.0F;
    float beta_value[] = {10.0F, 20.0F, 30.0F};
    float scaled_value[3] = {};
    float added_value[3] = {};
    float loss_value[1] = {};

    BufferView input_view = parameter_tape.input(input_value);
    const std::size_t after_input = parameter_tape.used();
    BufferView alpha_view = parameter_tape.parameter(alpha_value);
    BufferView beta_view = parameter_tape.parameter(beta_value);
    BufferView scaled_view = parameter_tape.output(scaled_value);
    BufferView added_view = parameter_tape.output(added_value);
    BufferView loss_view = parameter_tape.output(loss_value);

    assert(after_input == 0U);
    assert(!input_view.has_gradient());
    assert(alpha_view.role() == BufferRole::parameter);
    assert(beta_view.role() == BufferRole::parameter);

    scaled_view = scale(input_view, alpha_view, beta_view);
    added_view = scaled_view + input_view;
    loss_view = dot(added_view, input_view);
    assert(loss_value[0] == 182.0F);
    assert(parameter_tape.backward(loss_view));
    assert(alpha_view.gradient(0) == 14.0F);
    for (std::size_t i = 0; i < 3; ++i)
    {
        assert(beta_view.gradient(i) == input_value[i]);
        assert(input_view.gradient(i) == 0.0F);
    }

    // Fully connected followed by ReLU. Only the positive first neuron
    // contributes to the matrix and bias parameter gradients.
    Arena<2048> network_arena;
    Tape &network_tape = network_arena.tape();
    network_tape.register_operator<FullyConnectedOperator>();
    network_tape.register_operator<ReluOperator>();
    float network_input_value[] = {2.0F, -1.0F};
    float matrix_value[2][2] = {{1.0F, 2.0F}, {-3.0F, 1.0F}};
    float bias_value[] = {1.0F, 0.0F};
    float linear_value[2] = {};
    float activation_value[2] = {};

    BufferView network_input = network_tape.input(network_input_value);
    MatrixView matrix = network_tape.parameter(matrix_value);
    BufferView bias = network_tape.parameter(bias_value);
    BufferView linear = network_tape.output(linear_value);
    BufferView activation = network_tape.output(activation_value);

    linear = fully_connected(network_input, matrix, bias);
    activation = relu(linear);
    assert(linear_value[0] == 1.0F);
    assert(linear_value[1] == -7.0F);
    assert(activation_value[0] == 1.0F);
    assert(activation_value[1] == 0.0F);

    const float activation_seed[] = {1.0F, 1.0F};
    assert(network_tape.backward(activation, activation_seed, 2));
    assert(matrix.gradient(0, 0) == 2.0F);
    assert(matrix.gradient(0, 1) == -1.0F);
    assert(matrix.gradient(1, 0) == 0.0F);
    assert(matrix.gradient(1, 1) == 0.0F);
    assert(bias.gradient(0) == 1.0F);
    assert(bias.gradient(1) == 0.0F);
    assert(!network_input.has_gradient());

    // ReLU uses a zero derivative at exactly zero.
    Arena<512> relu_arena;
    Tape &relu_tape = relu_arena.tape();
    relu_tape.register_operator<ReluOperator>();
    float relu_parameter_value[] = {-1.0F, 0.0F, 2.0F};
    float relu_output_value[3] = {};
    BufferView relu_parameter = relu_tape.parameter(relu_parameter_value);
    BufferView relu_output = relu_tape.output(relu_output_value);
    relu_output = relu(relu_parameter);
    const float relu_seed[] = {1.0F, 1.0F, 1.0F};
    assert(relu_tape.backward(relu_output, relu_seed, 3));
    assert(relu_parameter.gradient(0) == 0.0F);
    assert(relu_parameter.gradient(1) == 0.0F);
    assert(relu_parameter.gradient(2) == 1.0F);

    // Including an operator does not enable it. Evaluation fails until that
    // operator type is explicitly registered on this tape.
    Arena<256> registry_arena;
    Tape &registry_tape = registry_arena.tape();
    float registry_left_value[] = {1.0F};
    float registry_right_value[] = {2.0F};
    float registry_output_value[] = {0.0F};
    BufferView registry_left = registry_tape.input(registry_left_value);
    BufferView registry_right = registry_tape.input(registry_right_value);
    BufferView registry_output = registry_tape.output(registry_output_value);
    registry_output = registry_left + registry_right;
    assert(registry_tape.status() == Status::operator_not_registered);
    assert(registry_output_value[0] == 0.0F);
    registry_tape.clear_status();
    assert(registry_tape.register_operator<AddOperator>());
    registry_output = registry_left + registry_right;
    assert(registry_tape.good());
    assert(registry_output_value[0] == 3.0F);

    // Quadratic loss, reusable graph records, Adam, and selective freezing.
    Arena<1024> training_arena;
    Tape &training_tape = training_arena.tape();
    training_tape.register_operator<DotOperator>();
    training_tape.register_operator<AddOperator>();
    training_tape.register_operator<QuadraticErrorOperator>();
    float feature_value[] = {2.0F};
    float coefficient_value[] = {3.0F};
    float bias_parameter_value[] = {1.0F};
    float dot_value[1] = {};
    float prediction_value[1] = {};
    float target_value[] = {0.0F};
    float training_loss_value[1] = {};
    BufferView feature = training_tape.input(feature_value);
    BufferView coefficient = training_tape.parameter(coefficient_value);
    BufferView bias_parameter = training_tape.parameter(bias_parameter_value);
    BufferView dot_output = training_tape.output(dot_value);
    BufferView prediction = training_tape.output(prediction_value);
    BufferView target = training_tape.input(target_value);
    BufferView loss = training_tape.output(training_loss_value);
    Adam<2> adam(1.0e-2F);
    assert(adam.add(coefficient));
    assert(adam.add(bias_parameter));
    assert(freeze_parameters(adam, coefficient));
    training_tape.begin_graph();
    const std::size_t persistent_training_bytes = training_tape.used();
    dot_output = dot(feature, coefficient);
    prediction = dot_output + bias_parameter;
    loss = quadratic_error(prediction, target);
    adam.zero_grad();
    assert(training_tape.backward(loss));
    assert(training_loss_value[0] == 49.0F);
    assert(coefficient.gradient(0) == 28.0F);
    assert(bias_parameter.gradient(0) == 14.0F);
    assert(adam.step());
    assert(coefficient_value[0] == 3.0F); // Frozen.
    assert(bias_parameter_value[0] < 1.0F);
    assert(training_tape.rewind_graph());
    assert(training_tape.used() == persistent_training_bytes);
    assert(unfreeze_parameters(adam, coefficient));

    // A single vector loss accumulates contributions from every sample into
    // shared polynomial parameters before an optimizer step.
    Arena<2048> batch_arena;
    Tape &batch_tape = batch_arena.tape();
    batch_tape.register_operator<DotOperator>();
    batch_tape.register_operator<AddOperator>();
    batch_tape.register_operator<QuadraticErrorOperator>();
    float batch_feature_value[2][1] = {{1.0F}, {2.0F}};
    float batch_coefficient_value = 3.0F;
    float batch_bias_value = 1.0F;
    float batch_polynomial_value[2] = {};
    float batch_prediction_value[2] = {};
    float batch_target_value[2] = {};
    float batch_loss_value = 0.0F;
    BufferView batch_coefficient =
        batch_tape.parameter(batch_coefficient_value);
    BufferView batch_bias = batch_tape.parameter(batch_bias_value);
    BufferView batch_polynomial =
        batch_tape.output(batch_polynomial_value);
    BufferView batch_prediction =
        batch_tape.output(batch_prediction_value);
    BufferView batch_target = batch_tape.input(batch_target_value);
    BufferView batch_loss = batch_tape.output(batch_loss_value);
    for (std::size_t sample = 0; sample < 2U; ++sample)
    {
        BufferView batch_feature =
            batch_tape.input(batch_feature_value[sample]);
        BufferView batch_polynomial_element = batch_tape.output(
            &batch_polynomial_value[sample],
            &batch_polynomial.gradients()[sample], 1U);
        BufferView batch_prediction_element = batch_tape.output(
            &batch_prediction_value[sample],
            &batch_prediction.gradients()[sample], 1U);
        batch_polynomial_element = dot(batch_feature, batch_coefficient);
        batch_prediction_element = batch_polynomial_element + batch_bias;
    }
    batch_loss = quadratic_error(batch_prediction, batch_target);
    assert(batch_tape.backward(batch_loss));
    assert(batch_loss_value == 65.0F); // 4^2 + 7^2.
    assert(batch_coefficient.gradient(0) == 36.0F);
    assert(batch_bias.gradient(0) == 22.0F);

    // RMSProp uses the same parameter registration and freezing API.
    Arena<128> rms_arena;
    Tape &rms_tape = rms_arena.tape();
    float rms_value = 1.0F;
    BufferView rms_parameter = rms_tape.parameter(rms_value);
    RMSProp<1> rmsprop(1.0e-2F);
    assert(rmsprop.add(rms_parameter));
    rms_parameter.gradients()[0] = 2.0F;
    assert(rmsprop.step());
    assert(rms_value < 1.0F);

    // Arena exhaustion is explicit and backward cannot return partial results.
    alignas(std::max_align_t) unsigned char tiny_memory[1];
    Tape tiny(tiny_memory, sizeof(tiny_memory));
    tiny.register_operator<AddOperator>();
    float tiny_input_value[1] = {2.0F};
    float tiny_input_gradient[1] = {};
    float tiny_output_value[1] = {};
    float tiny_output_gradient[1] = {};
    BufferView tiny_input =
        tiny.view(tiny_input_value, tiny_input_gradient, 1);
    BufferView tiny_output =
        tiny.view(tiny_output_value, tiny_output_gradient, 1);
    tiny_output = tiny_input + tiny_input;
    assert(tiny_output_value[0] == 4.0F);
    assert(tiny.status() == Status::out_of_memory);
    assert(!tiny.backward(tiny_output));

}

#undef assert
#undef AUTODIFF_CHECK

#endif

void autodiff_test()
{
#if defined(AUTODIFF_TEST) && defined(F32_DT) && defined(DYNAMIC_TEST)
    run_autodiff_tests();
#endif
}
