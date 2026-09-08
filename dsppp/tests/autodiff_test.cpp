#include "test_config.h"

extern "C" {
    extern void autodiff_test();
}

#if defined(AUTODIFF_TEST) && defined(DYNAMIC_TEST) && \
    (defined(F32_DT) || defined(F16_DT))

#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/add.hpp>
#include <dsppp/autodiff/operators/cross_entropy.hpp>
#include <dsppp/autodiff/operators/dot.hpp>
#include <dsppp/autodiff/operators/dropout.hpp>
#include <dsppp/autodiff/operators/dequantize.hpp>
#include <dsppp/autodiff/operators/fully_connected.hpp>
#include <dsppp/autodiff/operators/matrix_multiply.hpp>
#include <dsppp/autodiff/operators/multiply.hpp>
#include <dsppp/autodiff/operators/offset.hpp>
#include <dsppp/autodiff/operators/relu.hpp>
#include <dsppp/autodiff/operators/scale.hpp>
#include <dsppp/autodiff/operators/softmax.hpp>
#include <dsppp/autodiff/operators/sub.hpp>
#include <dsppp/autodiff/operators/quadratic_error.hpp>
#include <dsppp/autodiff/operators/quantize.hpp>
#include <dsppp/autodiff/optimizers/adam.hpp>
#include <dsppp/autodiff/optimizers/rmsprop.hpp>
#include <dsppp/autodiff/optimizers/sgd.hpp>

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
        }                                                                    \
    } while (false)

#ifdef assert
#undef assert
#endif
#define assert(condition) AUTODIFF_CHECK(condition)

template <typename T>
static bool close_to(T actual, float expected, float tolerance) noexcept
{
    const float difference = static_cast<float>(actual) - expected;
    return difference > -tolerance && difference < tolerance;
}

template <typename T>
static void test1()
{
 // Vector add followed by dot. Values and outputs belong to the caller;
    // gradients and two fixed-size operation records use the tape arena.
    Arena<2048, T> *buffer_arena = new Arena<2048, T>();
    Tape<T> &buffer_tape = buffer_arena->tape();
    buffer_tape.template register_operator<AddOperator<T>>();
    buffer_tape.template register_operator<DotOperator<T>>();
    T x_value[] = {1.0F, 2.0F, 3.0F};
    T w_value[] = {4.0F, 5.0F, 6.0F};
    T sum_value[3] = {};
    T result_value[1] = {};

    BufferView x_view = buffer_tape.view(x_value);
    BufferView w_view = buffer_tape.view(w_value);
    BufferView sum_view = buffer_tape.view(sum_value);
    BufferView result_view = buffer_tape.view(result_value);
    const std::size_t gradients_end = buffer_tape.used();
    assert(gradients_end >= 10U * sizeof(T));



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
    const T sum_seed[] = {1.0F, 2.0F, 3.0F};
    assert(buffer_tape.backward(sum_view, sum_seed, 3));
    for (std::size_t i = 0; i < 3; ++i)
    {
        assert(x_view.gradient(i) == sum_seed[i]);
        assert(w_view.gradient(i) == sum_seed[i]);
    }

    delete buffer_arena;
}

template <typename T>
static void test2()
{
 

    // Only parameters receive final gradients. The x input has no gradient
    // allocation, while scale and offset are trainable scalar parameters.
    Arena<2048, T> *parameter_arena = new Arena<2048, T>();
    Tape<T> &parameter_tape = parameter_arena->tape();
    parameter_tape.template register_operator<ScaleOperator<T>>();
    parameter_tape.template register_operator<OffsetOperator<T>>();
    parameter_tape.template register_operator<DotOperator<T>>();
    T input_value[] = {1.0F, 2.0F, 3.0F};
    T alpha_value = 2.0F;
    T beta_value = 10.0F;
    T scaled_value[3] = {};
    T added_value[3] = {};
    T loss_value[1] = {};

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

    scaled_view = scale(input_view, alpha_view);
    added_view = offset(scaled_view, beta_view);
    loss_view = dot(added_view, input_view);
    assert(loss_value[0] == 88.0F);
    assert(parameter_tape.backward(loss_view));
    assert(alpha_view.gradient(0) == 14.0F);
    assert(beta_view.gradient(0) == 6.0F);
    for (std::size_t i = 0; i < 3; ++i)
        assert(input_view.gradient(i) == 0.0F);

    delete parameter_arena;
}

template <typename T>
static void test3()
{

    // Fully connected followed by ReLU. Only the positive first neuron
    // contributes to the matrix and bias parameter gradients.
    Arena<2048, T> *network_arena = new Arena<2048, T>();
    Tape<T> &network_tape = network_arena->tape();
    network_tape.template register_operator<FullyConnectedOperator<T>>();
    network_tape.template register_operator<ReluOperator<T>>();
    T network_input_value[] = {2.0F, -1.0F};
    T matrix_value[2][2] = {{1.0F, 2.0F}, {-3.0F, 1.0F}};
    T bias_value[] = {1.0F, 0.0F};
    T linear_value[2] = {};
    T activation_value[2] = {};

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

    const T activation_seed[] = {1.0F, 1.0F};
    assert(network_tape.backward(activation, activation_seed, 2));
    assert(matrix.gradient(0, 0) == 2.0F);
    assert(matrix.gradient(0, 1) == -1.0F);
    assert(matrix.gradient(1, 0) == 0.0F);
    assert(matrix.gradient(1, 1) == 0.0F);
    assert(bias.gradient(0) == 1.0F);
    assert(bias.gradient(1) == 0.0F);
    assert(!network_input.has_gradient());
    delete network_arena;

   
}

template <typename T>
static void test4()
{
     // ReLU uses a zero derivative at exactly zero.
    Arena<512, T> *relu_arena = new Arena<512, T>();
    Tape<T> &relu_tape = relu_arena->tape();
    relu_tape.template register_operator<ReluOperator<T>>();
    T relu_parameter_value[] = {-1.0F, 0.0F, 2.0F};
    T relu_output_value[3] = {};
    BufferView relu_parameter = relu_tape.parameter(relu_parameter_value);
    BufferView relu_output = relu_tape.output(relu_output_value);
    relu_output = relu(relu_parameter);
    const T relu_seed[] = {1.0F, 1.0F, 1.0F};
    assert(relu_tape.backward(relu_output, relu_seed, 3));
    assert(relu_parameter.gradient(0) == 0.0F);
    assert(relu_parameter.gradient(1) == 0.0F);
    assert(relu_parameter.gradient(2) == 1.0F);

    delete relu_arena;
}

template <typename T>
static void test5()
{
    
    // Softmax is normalized and its vector-Jacobian product has zero sum.
    Arena<512, T> *softmax_arena = new Arena<512, T>();
    Tape<T> &softmax_tape = softmax_arena->tape();
    softmax_tape.template register_operator<SoftmaxOperator<T>>();
    T logits_value[] = {0.0F, 0.0F, 0.0F};
    T probability_value[3] = {};
    BufferView logits = softmax_tape.parameter(logits_value);
    BufferView probability = softmax_tape.output(probability_value);
    probability = softmax(logits);
    const T probability_sum = static_cast<T>(
        static_cast<float>(probability_value[0]) +
        static_cast<float>(probability_value[1]) +
        static_cast<float>(probability_value[2]));
    const float probability_tolerance =
        std::is_same<T, float>::value ? 2.0e-4F : 2.0e-3F;
    assert(close_to(probability_sum, 1.0F, probability_tolerance));
    for (std::size_t i = 0; i < 3U; ++i)
        assert(close_to(probability_value[i], 1.0F / 3.0F,
                        probability_tolerance));
    const T softmax_seed[] = {1.0F, 2.0F, 3.0F};
    assert(softmax_tape.backward(probability, softmax_seed, 3U));
    assert(close_to(logits.gradient(0), -1.0F / 3.0F,
                    probability_tolerance));
    assert(close_to(logits.gradient(1), 0.0F, probability_tolerance));
    assert(close_to(logits.gradient(2), 1.0F / 3.0F,
                    probability_tolerance));
    delete softmax_arena;
}

template <typename T>
static void test6()
{
    
    // Including an operator does not enable it. Evaluation fails until that
    // operator type is explicitly registered on this tape.
    Arena<256, T> *registry_arena = new Arena<256, T>();
    Tape<T> &registry_tape = registry_arena->tape();
    T registry_left_value[] = {1.0F};
    T registry_right_value[] = {2.0F};
    T registry_output_value[] = {0.0F};
    BufferView registry_left = registry_tape.input(registry_left_value);
    BufferView registry_right = registry_tape.input(registry_right_value);
    BufferView registry_output = registry_tape.output(registry_output_value);
    registry_output = registry_left + registry_right;
    assert(registry_tape.status() == Status::operator_not_registered);
    assert(registry_output_value[0] == 0.0F);
    registry_tape.clear_status();
    assert(registry_tape.template register_operator<AddOperator<T>>());
    registry_output = registry_left + registry_right;
    assert(registry_tape.good());
    assert(registry_output_value[0] == 3.0F);
    delete registry_arena;
}

template <typename T>
static void test7()
{
    
    // Quadratic loss, reusable graph records, Adam, and selective freezing.
    Arena<1024, T> *training_arena = new Arena<1024, T>();
    Tape<T> &training_tape = training_arena->tape();
    training_tape.template register_operator<DotOperator<T>>();
    training_tape.template register_operator<AddOperator<T>>();
    training_tape.template register_operator<QuadraticErrorOperator<T>>();
    T feature_value[] = {2.0F};
    T coefficient_value[] = {3.0F};
    T bias_parameter_value[] = {1.0F};
    T dot_value[1] = {};
    T prediction_value[1] = {};
    T target_value[] = {0.0F};
    T training_loss_value[1] = {};
    BufferView feature = training_tape.input(feature_value);
    BufferView coefficient = training_tape.parameter(coefficient_value);
    BufferView bias_parameter = training_tape.parameter(bias_parameter_value);
    BufferView dot_output = training_tape.output(dot_value);
    BufferView prediction = training_tape.output(prediction_value);
    BufferView target = training_tape.input(target_value);
    BufferView loss = training_tape.output(training_loss_value);
    Adam<2, 16U, T> adam(1.0e-2F);
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
    delete training_arena;
}

template <typename T>
static void test8()
{
    
    // A single vector loss accumulates contributions from every sample into
    // shared polynomial parameters before an optimizer step.
    Arena<2048, T> *batch_arena = new Arena<2048, T>();
    Tape<T> &batch_tape = batch_arena->tape();
    batch_tape.template register_operator<DotOperator<T>>();
    batch_tape.template register_operator<AddOperator<T>>();
    batch_tape.template register_operator<QuadraticErrorOperator<T>>();
    T batch_feature_value[2][1] = {{1.0F}, {2.0F}};
    T batch_coefficient_value = 3.0F;
    T batch_bias_value = 1.0F;
    T batch_polynomial_value[2] = {};
    T batch_prediction_value[2] = {};
    T batch_target_value[2] = {};
    T batch_loss_value = 0.0F;
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
    delete batch_arena;
}

template <typename T>
static void test9()
{
    // RMSProp uses the same parameter registration and freezing API.
    Arena<128, T> *rms_arena = new Arena<128, T>();
    Tape<T> &rms_tape = rms_arena->tape();
    T rms_value = 1.0F;
    BufferView rms_parameter = rms_tape.parameter(rms_value);
    RMSProp<1, 16U, T> rmsprop(1.0e-2F);
    assert(rmsprop.add(rms_parameter));
    rms_parameter.gradients()[0] = 2.0F;
    assert(rmsprop.step());
    assert(rms_value < 1.0F);
    delete rms_arena;
}

template <typename T>
static void test10()
{
    // Subtraction and elementwise multiplication have data operands only.
    Arena<1024, T> arena;
    Tape<T> &tape = arena.tape();
    tape.template register_operator<SubOperator<T>>();
    tape.template register_operator<MultiplyOperator<T>>();
    T left_value[] = {2.0F, 4.0F, 6.0F};
    T left_gradient[3] = {};
    T right_value[] = {1.0F, 2.0F, 3.0F};
    T right_gradient[3] = {};
    T difference_value[3] = {};
    T product_value[3] = {};
    BufferView left = tape.view(left_value, left_gradient, 3U);
    BufferView right = tape.view(right_value, right_gradient, 3U);
    BufferView difference = tape.output(difference_value);
    BufferView product = tape.output(product_value);
    difference = left - right;
    product = difference * right;
    assert(product_value[0] == 1.0F);
    assert(product_value[1] == 4.0F);
    assert(product_value[2] == 9.0F);
    const T seed[] = {1.0F, 1.0F, 1.0F};
    assert(tape.backward(product, seed, 3U));
    for (std::size_t i = 0; i < 3U; ++i)
    {
        assert(left.gradient(i) == right_value[i]);
        assert(right.gradient(i) == 0.0F);
    }
}

template<typename T>
static void test_transposed_dot_type()
{
    const T one = ::arm_cmsis_dsp::number_traits<T>::one();
    T matrix_value[2][5] = {};
    for (std::size_t column = 0; column < 5; ++column)
        matrix_value[0][column] = one;
    T vector_value[2] = {one,T{}};

    ::arm_cmsis_dsp::MatrixView<T, ::arm_cmsis_dsp::DYNAMIC> matrix(
        &matrix_value[0][0], 2, 5, 5);
    ::arm_cmsis_dsp::VectorView<T> vector(vector_value, 0, 2);
    ::arm_cmsis_dsp::Matrix<T, ::arm_cmsis_dsp::DYNAMIC,
                            ::arm_cmsis_dsp::DYNAMIC>
        materialized_transpose(5,2);
    ::arm_cmsis_dsp::transposeTo(materialized_transpose,matrix);
    auto reference = ::arm_cmsis_dsp::dot(materialized_transpose,vector);
    auto result = ::arm_cmsis_dsp::dot(
        ::arm_cmsis_dsp::transpose_view(matrix),vector);
    for (std::size_t column = 0; column < 5; ++column)
    {
        if constexpr (std::is_same<T,float16_t>::value)
            assert(static_cast<float>(result[column]) ==
                   static_cast<float>(reference[column]));
        else
            assert(result[column] == reference[column]);
    }
}

template <typename T>
static void test11()
{
    // A transposed view selects the fused matrix-vector kernel without
    // materializing the transpose. Five columns exercise the MVE tail.
    T matrix_value[3][5] = {
        {1.0F, 2.0F, 3.0F, 4.0F, 5.0F},
        {6.0F, 7.0F, 8.0F, 9.0F, 10.0F},
        {11.0F, 12.0F, 13.0F, 14.0F, 15.0F}};
    T vector_value[] = {2.0F, -1.0F, 0.5F};
    const T expected[] = {1.5F, 3.0F, 4.5F, 6.0F, 7.5F};

    ::arm_cmsis_dsp::MatrixView<T, ::arm_cmsis_dsp::DYNAMIC> matrix(
        &matrix_value[0][0], 3, 5, 5);
    ::arm_cmsis_dsp::VectorView<T> vector(vector_value, 0, 3);
    const auto transposed = ::arm_cmsis_dsp::transpose_view(matrix);
    assert(transposed.rows() == 5);
    assert(transposed.columns() == 3);
    assert(transposed(4, 2) == 15.0F);

    auto result = ::arm_cmsis_dsp::dot(transposed, vector);
    for (std::size_t column = 0; column < 5; ++column)
        assert(result[column] == expected[column]);

    ::arm_cmsis_dsp::Matrix<T,2,5> static_matrix;
    ::arm_cmsis_dsp::Vector<T,2> static_vector;
    for (std::size_t row = 0; row < 2; ++row)
        for (std::size_t column = 0; column < 5; ++column)
            static_matrix(row,column) = 1.0F;
    static_vector = static_cast<T>(1.0F);
    ::arm_cmsis_dsp::Vector<T,5> static_result =
        ::arm_cmsis_dsp::dot(
            ::arm_cmsis_dsp::transpose_view(static_matrix),static_vector);
    for (std::size_t column = 0; column < 5; ++column)
        assert(static_result[column] == 2.0F);

    test_transposed_dot_type<T>();

    // Exercise the same kernel through the fully connected backward pass.
    Arena<4096, T> arena;
    Tape<T> &tape = arena.tape();
    tape.template register_operator<FullyConnectedOperator<T>>();
    T input_value[5] = {};
    T input_gradient[5] = {};
    T weight_gradient[3][5] = {};
    T bias_value[3] = {};
    T bias_gradient[3] = {};
    T output_value[3] = {};
    T output_gradient[3] = {};
    BufferView input = tape.view(input_value, input_gradient, 5);
    MatrixView weights = tape.parameter(
        &matrix_value[0][0], &weight_gradient[0][0], 3, 5);
    BufferView bias = tape.parameter(bias_value, bias_gradient, 3);
    BufferView output = tape.view(output_value, output_gradient, 3);
    output = fully_connected(input, weights, bias);
    assert(tape.backward(output, vector_value, 3));
    for (std::size_t column = 0; column < 5; ++column)
        assert(input.gradient(column) == expected[column]);
}

template <typename T>
static void test12()
{
    // Categorical cross entropy consumes probabilities and a one-hot target.
    Arena<512, T> arena;
    Tape<T> &tape = arena.tape();
    tape.template register_operator<CrossEntropyOperator<T>>();
    T probability_value[] = {0.1F, 0.7F, 0.2F};
    T target_value[] = {0.0F, 1.0F, 0.0F};
    T loss_value = 0.0F;
    BufferView probability = tape.parameter(probability_value);
    BufferView target = tape.input(target_value);
    BufferView loss = tape.output(loss_value);
    loss = cross_entropy(probability, target);
    const float loss_tolerance =
        std::is_same<T, float>::value ? 2.0e-4F : 3.0e-3F;
    const float gradient_tolerance =
        std::is_same<T, float>::value ? 2.0e-4F : 1.0e-2F;
    assert(close_to(loss_value, 0.35667494F, loss_tolerance));
    assert(tape.backward(loss));
    assert(probability.gradient(0) == 0.0F);
    assert(close_to(probability.gradient(1), -1.42857143F,
                    gradient_tolerance));
    assert(probability.gradient(2) == 0.0F);
}

template <typename T>
static void test13()
{
    // Training applies inverted dropout and backward regenerates the same
    // mask. Disabling recording makes dropout an identity for inference.
    Arena<1024, T> arena;
    Tape<T> &tape = arena.tape();
    tape.template register_operator<DropoutOperator<T>>();
    T input_value[16];
    T output_value[16] = {};
    for (std::size_t i = 0; i < 16U; ++i) input_value[i] = 1.0F;
    BufferView input = tape.parameter(input_value);
    BufferView output = tape.output(output_value);
    DropoutGenerator generator(1234U);
    output = dropout(input, generator, 0.5F);

    unsigned dropped = 0U;
    unsigned kept = 0U;
    for (std::size_t i = 0; i < 16U; ++i)
    {
        assert(output_value[i] == 0.0F || output_value[i] == 2.0F);
        output_value[i] == 0.0F ? ++dropped : ++kept;
    }
    assert(dropped != 0U && kept != 0U);

    T seed[16];
    for (std::size_t i = 0; i < 16U; ++i) seed[i] = 1.0F;
    assert(tape.backward(output, seed, 16U));
    for (std::size_t i = 0; i < 16U; ++i)
        assert(input.gradient(i) == output_value[i]);

    {
        RecordingScope inference(tape, false);
        output = dropout(input, generator, 0.5F);
        for (std::size_t i = 0; i < 16U; ++i)
            assert(output_value[i] == input_value[i]);
    }
}

template <typename T>
static void test14()
{
    // Y = W X uses CMSIS-DSP matrix multiplication in the forward pass and
    // computes only dW = dY X^T in the backward pass.
    Arena<1024, T> arena;
    Tape<T> &tape = arena.tape();
    tape.template register_operator<MatrixMultiplyOperator<T>>();
    T weight_value[2][3] = {
        {1.0F, 2.0F, 3.0F}, {4.0F, 5.0F, 6.0F}};
    T input_value[3][2] = {
        {1.0F, 2.0F}, {3.0F, 4.0F}, {5.0F, 6.0F}};
    T output_value[2][2] = {};
    MatrixView weights = tape.parameter(weight_value);
    BufferView input = tape.input(&input_value[0][0], 6U);
    BufferView output = tape.output(&output_value[0][0], 4U);
    output = matrix_multiply(input, weights);
    assert(output_value[0][0] == 22.0F);
    assert(output_value[0][1] == 28.0F);
    assert(output_value[1][0] == 49.0F);
    assert(output_value[1][1] == 64.0F);

    const T seed[] = {1.0F, 2.0F, 3.0F, 4.0F};
    assert(tape.backward(output, seed, 4U));
    const T expected_gradient[2][3] = {
        {5.0F, 11.0F, 17.0F}, {11.0F, 25.0F, 39.0F}};
    for (std::size_t row = 0; row < 2U; ++row)
        for (std::size_t column = 0; column < 3U; ++column)
            assert(weights.gradient(row, column) ==
                   expected_gradient[row][column]);
    assert(!input.has_gradient());
}

template <typename T>
static void test15()
{
    // SGD performs one fused parameter -= learning_rate * gradient update.
    Arena<128, T> arena;
    Tape<T> &tape = arena.tape();
    T value[] = {1.0F, -2.0F};
    BufferView parameter = tape.parameter(value);
    SGD<2, 1, T> optimizer(0.25F);
    assert(optimizer.add(parameter));
    parameter.gradients()[0] = 2.0F;
    parameter.gradients()[1] = -4.0F;
    assert(optimizer.step());
    assert(value[0] == 0.5F);
    assert(value[1] == -1.0F);
    optimizer.zero_grad();
    assert(parameter.gradient(0) == 0.0F);
    assert(parameter.gradient(1) == 0.0F);
    assert(freeze_parameters(optimizer, parameter));
    parameter.gradients()[0] = 1.0F;
    assert(optimizer.step());
    assert(value[0] == 0.5F);
}

template <typename T>
static void test16()
{
    // C++ extension primitives used by QAT: nearest-even rounding and
    // range-masked accumulation/reduction. Half-way cases verify ties-to-even.
    T rounding_input[] = {-2.5F, -1.5F, -0.5F, 0.5F, 1.5F, 2.5F};
    T rounding_output[6] = {};
    T rounding_accumulator[6] = {};
    T rounding_one[] = {1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F};
    ::arm_cmsis_dsp::VectorView<T> rounding_input_view(rounding_input, 0, 6U);
    ::arm_cmsis_dsp::VectorView<T> rounding_output_view(rounding_output, 0, 6U);
    ::arm_cmsis_dsp::VectorView<T> rounding_accumulator_view(
        rounding_accumulator, 0, 6U);
    ::arm_cmsis_dsp::VectorView<T> rounding_one_view(rounding_one, 0, 6U);
    ::arm_cmsis_dsp::round_to_nearest(rounding_output_view,
                                      rounding_input_view);
    const float expected_rounding[] = {-2.0F, -2.0F, 0.0F,
                                       0.0F, 2.0F, 2.0F};
    for (std::size_t i = 0; i < 6U; ++i)
        assert(static_cast<float>(rounding_output[i]) == expected_rounding[i]);
    const auto central = ::arm_cmsis_dsp::nearest_even_range_mask(
        rounding_input_view, 1.0F, static_cast<T>(0.0F),
        static_cast<T>(-1.0F), static_cast<T>(1.0F));
    ::arm_cmsis_dsp::masked_scale_add(
        rounding_accumulator_view, rounding_one_view, central,
        static_cast<T>(2.0F));
    const auto central_reduction = ::arm_cmsis_dsp::masked_dot_sum(
        rounding_one_view, rounding_input_view, central);
    for (std::size_t i = 0; i < 6U; ++i)
        assert(static_cast<float>(rounding_accumulator[i]) ==
               (i == 2U || i == 3U ? 2.0F : 0.0F));
    assert(static_cast<float>(central_reduction.dot) == 0.0F);
    assert(static_cast<float>(central_reduction.sum) == 2.0F);

    // Q/DQ keeps float storage while reproducing LiteRT/CMSIS-NN signed int8
    // activation codes. Its combined backward pass is an STE in range and
    // also exposes gradients for scale and zero-point learning.
    Arena<2048, T> arena;
    Tape<T> &tape = arena.tape();
    tape.template register_operator<QuantizeOperator<T>>();
    tape.template register_operator<DequantizeOperator<T>>();
    T input_value[] = {-20.0F, -0.26F, 0.24F, 30.0F};
    T input_gradient[4] = {};
    T scale_value = 0.1F;
    T zero_point_value = -3.0F;
    T code_value[4] = {};
    T output_value[4] = {};
    BufferView input = tape.view(input_value, input_gradient, 4U);
    BufferView scale_parameter = tape.parameter(scale_value);
    BufferView zero_point_parameter = tape.parameter(zero_point_value);
    BufferView code = tape.output(code_value);
    BufferView output = tape.output(output_value);
    code = quantize(input, scale_parameter, zero_point_parameter);
    output = dequantize(code, scale_parameter, zero_point_parameter);

    const float expected_code[] = {-128.0F, -6.0F, -1.0F, 127.0F};
    const float expected_output[] = {-12.5F, -0.3F, 0.2F, 13.0F};
    const float tolerance = std::is_same<T, float>::value ? 1.0e-5F : 2.0e-2F;
    for (std::size_t i = 0; i < 4U; ++i)
    {
        assert(close_to(code_value[i], expected_code[i], tolerance));
        assert(close_to(output_value[i], expected_output[i], tolerance));
    }

    const T seed[] = {1.0F, 1.0F, 1.0F, 1.0F};
    assert(tape.backward(output, seed, 4U));
    assert(close_to(input.gradient(0), 0.0F, tolerance));
    assert(close_to(input.gradient(1), 1.0F, tolerance));
    assert(close_to(input.gradient(2), 1.0F, tolerance));
    assert(close_to(input.gradient(3), 0.0F, tolerance));
    assert(close_to(scale_parameter.gradient(0), 4.2F,
                    std::is_same<T, float>::value ? 2.0e-4F : 8.0e-2F));
    assert(close_to(zero_point_parameter.gradient(0), -0.2F,
                    std::is_same<T, float>::value ? 2.0e-4F : 8.0e-3F));

    // Weight quantization follows the backend's symmetric per-output-axis
    // contract: [-127, 127], zero-point zero, one scale per row here.
    Arena<1536, T> weight_arena;
    Tape<T> &weight_tape = weight_arena.tape();
    weight_tape.template register_operator<QuantizeOperator<T>>();
    weight_tape.template register_operator<DequantizeOperator<T>>();
    T weight_value[] = {-1.2F, -0.2F, 0.6F, -2.0F, 0.8F, 2.6F};
    T weight_scale_value[] = {0.1F, 0.2F};
    T weight_zero_value[] = {4.0F, -4.0F};
    T weight_code_value[6] = {};
    T weight_output_value[6] = {};
    BufferView weight = weight_tape.input(weight_value);
    BufferView weight_scale = weight_tape.parameter(weight_scale_value);
    BufferView weight_zero = weight_tape.input(weight_zero_value);
    BufferView weight_code = weight_tape.output(weight_code_value);
    BufferView weight_output = weight_tape.output(weight_output_value);
    const Int8Quantization weight_quantization =
        Int8Quantization::weights(2U, 3U);
    weight_code = quantize(weight, weight_scale, weight_zero,
                           weight_quantization);
    weight_output = dequantize(weight_code, weight_scale, weight_zero,
                               weight_quantization);
    const float expected_weight_code[] = {-12.0F, -2.0F, 6.0F,
                                          -10.0F, 4.0F, 13.0F};
    for (std::size_t i = 0; i < 6U; ++i)
    {
        assert(close_to(weight_code_value[i], expected_weight_code[i],
                        tolerance));
        assert(close_to(weight_output_value[i],
                        static_cast<float>(weight_value[i]), tolerance));
    }
    assert(cmsis_nn_offset(-3.0F) == 3);
    assert(static_cast<float>(weight_zero_value[0]) == 0.0F);
    assert(static_cast<float>(weight_zero_value[1]) == 0.0F);

    // Q/DQ owns parameter projection; training loops do not need to clamp
    // values after an optimizer step.
    scale_value = -1.0F;
    zero_point_value = 200.0F;
    {
        RecordingScope inference(tape, false);
        code = quantize(input, scale_parameter, zero_point_parameter);
    }
    assert(static_cast<float>(scale_value) > 0.0F);
    assert(static_cast<float>(zero_point_value) == 127.0F);
}

template <typename T>
static void run_autodiff_tests()
{
    test1<T>();
    test2<T>();
    test3<T>();
    test4<T>();
    test5<T>();
    test6<T>();
    test7<T>();
    test8<T>();
    test9<T>();
    test10<T>();
    test11<T>();
    test12<T>();
    test13<T>();
    test14<T>();
    test15<T>();
    test16<T>();
    
    // Arena exhaustion is explicit and backward cannot return partial results.
    alignas(std::max_align_t) unsigned char tiny_memory[1];
    Tape<T> tiny(tiny_memory, sizeof(tiny_memory));
    tiny.template register_operator<AddOperator<T>>();
    T tiny_input_value[1] = {2.0F};
    T tiny_input_gradient[1] = {};
    T tiny_output_value[1] = {};
    T tiny_output_gradient[1] = {};
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
#if defined(AUTODIFF_TEST) && defined(DYNAMIC_TEST)
#if defined(F32_DT)
    printf("Running float32 autodiff tests...\r\n");
    run_autodiff_tests<float>();
#elif defined(F16_DT) && defined(ARM_FLOAT16_SUPPORTED)
    printf("Running float16 autodiff tests...\r\n");
    run_autodiff_tests<float16_t>();
#endif
#endif
}
