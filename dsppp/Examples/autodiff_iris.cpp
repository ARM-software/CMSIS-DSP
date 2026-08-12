#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/cross_entropy.hpp>
#include <dsppp/autodiff/operators/fully_connected.hpp>
#include <dsppp/autodiff/operators/relu.hpp>
#include <dsppp/autodiff/operators/softmax.hpp>
#include <dsppp/autodiff/optimizers/adam.hpp>

#include "iris_data.hpp"

#include <dsp/statistics_functions.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>

using namespace arm_cmsis_dsp::autodiff;

namespace {

constexpr std::size_t input_size = 4U;
constexpr std::size_t hidden_size = 8U;
constexpr std::size_t class_count = 3U;
constexpr std::size_t training_count = 120U;
constexpr std::size_t epoch_count = 120U;

struct Model
{
    float hidden_weight[hidden_size][input_size];
    float hidden_bias[hidden_size];
    float output_weight[class_count][hidden_size];
    float output_bias[class_count];
};

struct TrainingState
{
    Model model{};
    Arena<2048> arena{};
    Adam<67U, 4U> optimizer{1.0e-2F};
    float input[input_size]{};
    float hidden_linear[hidden_size]{};
    float hidden[hidden_size]{};
    float logits[class_count]{};
    float probability[class_count]{};
    float target[class_count]{};
    float loss{};
    std::uint8_t training_index[training_count]{};
};

static std::uint32_t random_state = 0x12345678U;

static std::uint32_t random_u32() noexcept
{
    random_state = random_state * 1664525U + 1013904223U;
    return random_state;
}

static float random_weight() noexcept
{
    const float unit = static_cast<float>((random_u32() >> 8) & 0xffffU) /
                       65535.0F;
    return (unit - 0.5F) * 0.5F;
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
    const float (&probability)[class_count]) noexcept
{
    float maximum;
    std::uint32_t index;
    arm_max_f32(probability, class_count, &maximum, &index);
    return index;
}

} // namespace

int main()
{
    std::printf("Iris classification with CMSIS-DSP autodiff\n");
    // Keep the training buffers and optimizer state off the limited stack.
    TrainingState *state = new TrainingState;
    initialize(state->model);

    Tape &tape = state->arena.tape();
    tape.register_operator<FullyConnectedOperator>();
    tape.register_operator<ReluOperator>();
    tape.register_operator<SoftmaxOperator>();
    tape.register_operator<CrossEntropyOperator>();

    BufferView input = tape.input(state->input);
    MatrixView hidden_weight = tape.parameter(state->model.hidden_weight);
    BufferView hidden_bias = tape.parameter(state->model.hidden_bias);
    MatrixView output_weight = tape.parameter(state->model.output_weight);
    BufferView output_bias = tape.parameter(state->model.output_bias);
    BufferView hidden_linear = tape.output(state->hidden_linear);
    BufferView hidden = tape.output(state->hidden);
    BufferView logits = tape.output(state->logits);
    BufferView probability = tape.output(state->probability);
    BufferView target = tape.input(state->target);
    BufferView loss = tape.output(state->loss);

    
    if (!tape.good()) {
      if (tape.status() == Status::out_of_memory)
        std::printf("Autodiff arena is too small\n");
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
            iris_data::normalized_features(sample, state->input);
            for (std::size_t i = 0; i < class_count; ++i)
                state->target[i] = i == iris_data::samples[sample].label
                                       ? 1.0F : 0.0F;

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
            epoch_loss += state->loss;
        }

        if ((epoch + 1U) % 20U == 0U)
            std::printf("epoch %u: mean loss=%g\n",
                        static_cast<unsigned>(epoch + 1U),
                        static_cast<double>(epoch_loss / training_count));
    }

    // Final check on the 30 samples that were kept out of training.
    unsigned correct = 0U;
    {
        RecordingScope inference(tape, false);
        for (std::size_t sample = 0; sample < iris_data::sample_count;
             ++sample)
        {
            if (!is_test_sample(sample)) continue;
            iris_data::normalized_features(sample, state->input);
            hidden_linear = fully_connected(input, hidden_weight, hidden_bias);
            hidden = relu(hidden_linear);
            logits = fully_connected(hidden, output_weight, output_bias);
            probability = softmax(logits);
            if (predicted_class(state->probability) ==
                iris_data::samples[sample].label)
                ++correct;
        }
    }
    std::printf("final test accuracy=%u/30\n", correct);

    delete state;
    return 0;
}
