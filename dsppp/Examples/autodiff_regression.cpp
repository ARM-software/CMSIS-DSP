#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/add.hpp>
#include <dsppp/autodiff/operators/dot.hpp>
#include <dsppp/autodiff/operators/quadratic_error.hpp>
#include <dsppp/autodiff/optimizers/rmsprop.hpp>

#include <cmath>
#include <cstdio>

using namespace arm_cmsis_dsp::autodiff;

/* C-compatible representation suitable for RAM, flash, or application-defined
 * nonvolatile storage. It contains values only; optimizer state is not needed
 * for inference on a later run.
 */
struct SinePolynomialParameters
{
    float coefficients[3]; // x, x^2, x^3
    float bias;
};

static void save_parameters(SinePolynomialParameters &destination,
                            const SinePolynomialParameters &source) noexcept
{
    for (std::size_t i = 0; i < 3; ++i)
        destination.coefficients[i] = source.coefficients[i];
    destination.bias = source.bias;
}

static void restore_parameters(SinePolynomialParameters &destination,
                               const SinePolynomialParameters &source) noexcept
{
    save_parameters(destination, source);
}

static float infer(const SinePolynomialParameters &parameters,
                   float x) noexcept
{
    const float x2 = x * x;
    return parameters.bias + parameters.coefficients[0] * x +
           parameters.coefficients[1] * x2 +
           parameters.coefficients[2] * x2 * x;
}

int main()
{
    
    constexpr std::size_t sample_count = 100U;
    constexpr std::size_t training_steps = 4000U;
    constexpr float pi = 3.14159265358979323846F;

    SinePolynomialParameters model = {{0.0F, 0.0F, 0.0F}, 0.0F};
    // One training graph contains two scalar operations per sample and one
    // global loss record. Values stay in caller storage; this arena contains
    // gradients and operation records only.
    Arena<32768> *arena = new Arena<32768>();
    Tape &tape = arena->tape();
    tape.register_operator<DotOperator>();
    tape.register_operator<AddOperator>();
    tape.register_operator<QuadraticErrorOperator>();

    float feature_value[sample_count][3] = {};
    float polynomial_value[sample_count] = {};
    float prediction_value[sample_count] = {};
    float target_value[sample_count] = {};
    float loss_value[1] = {};

    // Construct the complete fixed training set once. No sample storage is
    // allocated by the autodiff implementation.
    for (std::size_t sample = 0; sample < sample_count; ++sample)
    {
        const float x = -pi + 2.0F * pi * static_cast<float>(sample) /
                                      static_cast<float>(sample_count - 1U);
        const float x2 = x * x;
        feature_value[sample][0] = x;
        feature_value[sample][1] = x2;
        feature_value[sample][2] = x2 * x;
        target_value[sample] = std::sin(x);
    }

    BufferView coefficients = tape.parameter(model.coefficients);
    BufferView bias = tape.parameter(model.bias);
    BufferView polynomial = tape.output(polynomial_value);
    BufferView prediction = tape.output(prediction_value);
    BufferView target = tape.input(target_value);
    BufferView loss = tape.output(loss_value);

    // Match the optimizer chosen by the PyTorch example. Adam can be used
    // here instead by including adam.hpp and changing only this type.
    RMSProp<4, 2> optimizer(1.0e-3F);
    optimizer.add(coefficients);
    optimizer.add(bias);

    /* Set this to true for bias-only fine tuning. The same mechanism freezes
     * all parameters belonging to any selected layer/operator.
     */
    constexpr bool fine_tune_bias_only = false;
    if (fine_tune_bias_only)
        freeze_parameters(optimizer, coefficients);

    /* Views and gradient buffers are persistent. Only operation records after
     * this point are rewound for every complete batch.
     */
    tape.begin_graph();
    for (std::size_t step = 0; step < training_steps; ++step)
    {
        tape.rewind_graph();

        // Build all 100 predictions before constructing the loss. The scalar
        // views below share slices of the two arena-managed vector gradients;
        // creating them allocates no additional gradient buffers.
        for (std::size_t sample = 0; sample < sample_count; ++sample)
        {
            BufferView features = tape.input(feature_value[sample]);
            BufferView polynomial_element = tape.output(
                &polynomial_value[sample], &polynomial.gradients()[sample], 1U);
            BufferView prediction_element = tape.output(
                &prediction_value[sample], &prediction.gradients()[sample], 1U);

            polynomial_element = dot(features, coefficients);
            prediction_element = polynomial_element + bias;
        }

        // One scalar loss represents the whole sampled period. Consequently
        // backward() accumulates parameter gradients from every sample and
        // the optimizer performs exactly one global update per step.
        loss = quadratic_error(prediction, target);
        optimizer.zero_grad();
        if (!tape.backward(loss) || !optimizer.step()) return 1;

        if ((step + 1U) % 100U == 0U)
            std::printf("step %u, global quadratic error = %g, mean = %g\n",
                        static_cast<unsigned>(step + 1U),
                        static_cast<double>(loss_value[0]),
                        static_cast<double>(loss_value[0] / sample_count));
    }

    SinePolynomialParameters checkpoint{};
    save_parameters(checkpoint, model);
    std::printf("learned: y = %g + %g*x + %g*x^2 + %g*x^3\n",
                static_cast<double>(checkpoint.bias),
                static_cast<double>(checkpoint.coefficients[0]),
                static_cast<double>(checkpoint.coefficients[1]),
                static_cast<double>(checkpoint.coefficients[2]));

    /* Simulate a later program run. In a real target, checkpoint would first
     * be persisted and later loaded from platform-specific nonvolatile memory.
     */
    SinePolynomialParameters restored{};
    restore_parameters(restored, checkpoint);
    std::printf("restored inference at pi/2: predicted=%g, reference=%g\n",
                static_cast<double>(infer(restored, pi * 0.5F)),
                static_cast<double>(std::sin(pi * 0.5F)));

    delete arena;
    return 0;
}
