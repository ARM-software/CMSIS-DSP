#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/cross_entropy.hpp>
#include <dsppp/autodiff/operators/fully_connected.hpp>
#include <dsppp/autodiff/operators/relu.hpp>
#include <dsppp/autodiff/operators/softmax.hpp>
#include <dsppp/autodiff/optimizers/adam.hpp>

#include "iris_data.hpp"

#include <dsp/statistics_functions.h>
#include <dsp/statistics_functions_f16.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>

using namespace arm_cmsis_dsp::autodiff;

/* Set to 0 to build the same example with float32 autodiff. It may also be
 * overridden from the compiler command line with -D. */
#ifndef DSPPP_AUTODIFF_IRIS_USE_FLOAT16
#define DSPPP_AUTODIFF_IRIS_USE_FLOAT16 1
#endif

#if !DSPPP_AUTODIFF_IRIS_USE_FLOAT16 || defined(ARM_FLOAT16_SUPPORTED)
namespace {

#if DSPPP_AUTODIFF_IRIS_USE_FLOAT16
using IrisType = float16_t;
#else
using IrisType = float;
#endif

constexpr std::size_t input_size = 4U;
constexpr std::size_t hidden_size = 8U;
constexpr std::size_t class_count = 3U;
constexpr std::size_t training_count = 120U;
constexpr std::size_t epoch_count = 120U;

struct Model
{
    IrisType hidden_weight[hidden_size][input_size];
    IrisType hidden_bias[hidden_size];
    IrisType output_weight[class_count][hidden_size];
    IrisType output_bias[class_count];
};

struct TrainingState
{
    Model model{};
    Arena<2048, IrisType> arena{};
    Adam<67U, 4U, IrisType> optimizer{static_cast<IrisType>(1.0e-2F)};
    IrisType input[input_size]{};
    IrisType hidden_linear[hidden_size]{};
    IrisType hidden[hidden_size]{};
    IrisType logits[class_count]{};
    IrisType probability[class_count]{};
    IrisType target[class_count]{};
    IrisType loss{};
    // Keep normalized float32 patterns for the original example path. The
    // additional float16 array avoids converting samples inside the loop when
    // the half-precision path is selected.
    float f32_patterns[iris_data::sample_count][input_size]{};
#if defined(ARM_FLOAT16_SUPPORTED)
    float16_t f16_patterns[iris_data::sample_count][input_size]{};
#endif
    std::uint8_t training_index[training_count]{};
};

static std::uint32_t random_state = 0x12345678U;

static std::uint32_t random_u32() noexcept
{
    random_state = random_state * 1664525U + 1013904223U;
    return random_state;
}

static IrisType random_weight() noexcept
{
    const float unit = static_cast<float>((random_u32() >> 8) & 0xffffU) /
                       65535.0F;
    return static_cast<IrisType>((unit - 0.5F) * 0.5F);
}

static void initialize(Model &model) noexcept
{
    for (std::size_t row = 0; row < hidden_size; ++row)
    {
        model.hidden_bias[row] = 0.0F;
        for (std::size_t column = 0; column < input_size; ++column)
            model.hidden_weight[row][column] = random_weight();
    }
    for (std::size_t row = 0; row < class_count; ++row)
    {
        model.output_bias[row] = 0.0F;
        for (std::size_t column = 0; column < hidden_size; ++column)
            model.output_weight[row][column] = random_weight();
    }
}

static bool is_test_sample(std::size_t index) noexcept
{
    // Every fifth member of each class is held out: 10 per class.
    return (index % 50U) % 5U == 0U;
}

static std::uint32_t predicted_class(
    const IrisType (&probability)[class_count]) noexcept
{
    IrisType maximum;
    std::uint32_t index;
#if DSPPP_AUTODIFF_IRIS_USE_FLOAT16
    arm_max_f16(probability, class_count, &maximum, &index);
#else
    arm_max_f32(probability, class_count, &maximum, &index);
#endif
    return index;
}

static const char *label_name(unsigned label) noexcept
{
    static constexpr const char *names[class_count] = {
        "Iris-setosa", "Iris-versicolor", "Iris-virginica"};
    return label < class_count ? names[label] : "unknown";
}

} // namespace

int main()
{
    std::printf("Iris classification with CMSIS-DSP autodiff\n");
    // Keep the training buffers and optimizer state off the limited stack.
    TrainingState *state = new TrainingState;
    initialize(state->model);
    for (std::size_t sample = 0; sample < iris_data::sample_count; ++sample)
    {
        iris_data::normalized_features(sample, state->f32_patterns[sample]);
#if defined(ARM_FLOAT16_SUPPORTED)
        for (std::size_t feature = 0; feature < input_size; ++feature)
            state->f16_patterns[sample][feature] = static_cast<float16_t>(
                state->f32_patterns[sample][feature]);
#endif
    }

    Tape<IrisType> &tape = state->arena.tape();
    tape.register_operator<FullyConnectedOperator<IrisType>>();
    tape.register_operator<ReluOperator<IrisType>>();
    tape.register_operator<SoftmaxOperator<IrisType>>();
    tape.register_operator<CrossEntropyOperator<IrisType>>();

    BufferView<IrisType> input = tape.input(state->input);
    MatrixView<IrisType> hidden_weight = tape.parameter(state->model.hidden_weight);
    BufferView<IrisType> hidden_bias = tape.parameter(state->model.hidden_bias);
    MatrixView<IrisType> output_weight = tape.parameter(state->model.output_weight);
    BufferView<IrisType> output_bias = tape.parameter(state->model.output_bias);
    BufferView<IrisType> hidden_linear = tape.output(state->hidden_linear);
    BufferView<IrisType> hidden = tape.output(state->hidden);
    BufferView<IrisType> logits = tape.output(state->logits);
    BufferView<IrisType> probability = tape.output(state->probability);
    BufferView<IrisType> target = tape.input(state->target);
    BufferView<IrisType> loss = tape.output(state->loss);

    if (!tape.good())
    {
        if (tape.status() == Status::out_of_memory)
            std::printf("Autodiff arena is too small\n");
        else
            std::printf("Autodiff setup failed (status=%u)\n",
                        static_cast<unsigned>(tape.status()));
        delete state;
        return 1;
    }

    if (!state->optimizer.add(hidden_weight) ||
        !state->optimizer.add(hidden_bias) ||
        !state->optimizer.add(output_weight) ||
        !state->optimizer.add(output_bias))
    {
        delete state;
        std::printf("Failed to add parameters to optimizer\n");
        return 1;
    }

    // Keep 30 samples for testing. They are never used during training.
    std::size_t training_position = 0U;
    for (std::size_t sample = 0; sample < iris_data::sample_count; ++sample)
        if (!is_test_sample(sample))
            state->training_index[training_position++] =
                static_cast<std::uint8_t>(sample);

    tape.begin_graph();
    for (std::size_t epoch = 0; epoch < epoch_count; ++epoch)
    {
        // Shuffle the training patterns before each epoch.
        for (std::size_t i = training_count - 1U; i > 0U; --i)
        {
            const std::size_t other = random_u32() % (i + 1U);
            const std::uint8_t temporary = state->training_index[i];
            state->training_index[i] = state->training_index[other];
            state->training_index[other] = temporary;
        }

        float epoch_loss = 0.0F;
        for (std::size_t position = 0; position < training_count; ++position)
        {
            const std::size_t sample = state->training_index[position];
#if DSPPP_AUTODIFF_IRIS_USE_FLOAT16
            for (std::size_t feature = 0; feature < input_size; ++feature)
                state->input[feature] = state->f16_patterns[sample][feature];
#else
            for (std::size_t feature = 0; feature < input_size; ++feature)
                state->input[feature] = state->f32_patterns[sample][feature];
#endif
            for (std::size_t i = 0; i < class_count; ++i)
                state->target[i] = i == iris_data::samples[sample].label
                                       ? static_cast<IrisType>(1.0F)
                                       : static_cast<IrisType>(0.0F);

            if (!tape.rewind_graph())
            {
                delete state;
                return 1;
            }
            hidden_linear = fully_connected(input, hidden_weight, hidden_bias);
            hidden = relu(hidden_linear);
            logits = fully_connected(hidden, output_weight, output_bias);
            probability = softmax(logits);
            loss = cross_entropy(probability, target);

            state->optimizer.zero_grad();
            if (!tape.backward(loss) || !state->optimizer.step())
            {
                delete state;
                return 1;
            }
            epoch_loss += static_cast<float>(state->loss);
        }

        if ((epoch + 1U) % 20U == 0U)
            std::printf("epoch %u: mean loss=%g\n",
                        static_cast<unsigned>(epoch + 1U),
                        static_cast<double>(epoch_loss / training_count));
    }

    // Final check on the 30 samples that were kept out of training.
    unsigned correct = 0U;
    unsigned test_number = 0U;
    {
        RecordingScope<IrisType> inference(tape, false);
        for (std::size_t sample = 0; sample < iris_data::sample_count;
             ++sample)
        {
            if (!is_test_sample(sample)) continue;
#if DSPPP_AUTODIFF_IRIS_USE_FLOAT16
            for (std::size_t feature = 0; feature < input_size; ++feature)
                state->input[feature] = state->f16_patterns[sample][feature];
#else
            for (std::size_t feature = 0; feature < input_size; ++feature)
                state->input[feature] = state->f32_patterns[sample][feature];
#endif
            hidden_linear = fully_connected(input, hidden_weight, hidden_bias);
            hidden = relu(hidden_linear);
            logits = fully_connected(hidden, output_weight, output_bias);
            probability = softmax(logits);
            const unsigned expected = iris_data::samples[sample].label;
            const unsigned detected = predicted_class(state->probability);
            const bool match = detected == expected;
            if (!match)
                std::printf("\033[31m");
            std::printf("Test %u:\n  Expected \"%s\"\n  Detected \"%s\"\n",
                        ++test_number, label_name(expected),
                        label_name(detected));
            if (!match)
                std::printf("\033[0m");
            if (match)
                ++correct;
        }
    }
    std::printf("final test accuracy=%u/30 tests\n", correct);

    delete state;
    return 0;
}

#else

int main()
{
    std::printf("Iris float16 example requires ARM_FLOAT16_SUPPORTED\n");
    return 0;
}

#endif
