#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/dequantize.hpp>
#include <dsppp/autodiff/operators/fully_connected.hpp>
#include <dsppp/autodiff/operators/quadratic_error.hpp>
#include <dsppp/autodiff/operators/quantize.hpp>
#include <dsppp/autodiff/optimizers/adam.hpp>

#include <cmath>
#include <cstdint>
#include <cstdio>

using namespace arm_cmsis_dsp::autodiff;

namespace {

constexpr std::size_t input_count = 3U;
constexpr std::size_t output_count = 2U;
constexpr std::size_t sample_count = 4U;

} // namespace

int main()
{
    constexpr std::size_t training_steps = 400U;
    constexpr Int8Quantization activation_quantization =
        Int8Quantization::activation();
    constexpr Int8Quantization weight_quantization =
        Int8Quantization::weights(output_count, input_count);

    // A small fixed training set and the outputs of a reference 3-to-2
    // affine layer. The network below learns these targets while every input,
    // weight, and output forward pass is subjected to signed int8 Q/DQ.
    float input_value[sample_count][input_count] = {
        {-1.0F, -0.5F, 0.25F},
        {0.5F, 1.0F, -1.0F},
        {1.0F, 0.25F, 0.5F},
        {-0.25F, -1.0F, 1.0F}};
    float target_value[sample_count][output_count] = {
        {0.1F, 0.425F},
        {-1.275F, -1.2F},
        {0.7875F, -0.1375F},
        {1.6625F, 0.675F}};

    float weight_value[output_count][input_count] = {
        {0.2F, -0.1F, 0.3F}, {-0.2F, 0.1F, 0.2F}};
    float weight_gradient[output_count][input_count] = {};
    float bias_value[output_count] = {};

    // LiteRT/CMSIS-NN/Ethos-U weight quantization is symmetric and uses one
    // scale per output channel. Its zero-point is fixed at zero.
    float weight_scale_value[output_count] = {0.01F, 0.01F};
    float weight_zero_point_value[output_count] = {};

    // Activations are asymmetric per-tensor. These are optimizer parameters;
    // Q/DQ automatically keeps them in the deployable signed-int8 domain.
    float input_scale_value = 0.01F;
    float input_zero_point_value = 0.0F;
    float output_scale_value = 0.01F;
    float output_zero_point_value = 0.0F;

    // Quantize produces the integer int8 values, represented as float so the
    // autodiff graph remains floating point. Dequantize reconstructs the
    // floating values that the layer would observe during int8 inference.
    float quantized_weight_value[output_count][input_count] = {};
    float dequantized_weight_value[output_count][input_count] = {};
    float dequantized_weight_gradient[output_count][input_count] = {};
    float quantized_input_value[input_count] = {};
    float dequantized_input_value[input_count] = {};
    float linear_value[output_count] = {};
    float quantized_output_value[output_count] = {};
    float prediction_value[output_count] = {};
    float loss_value = 0.0F;

    // Keep the arena out of the small embedded process stack. Static storage
    // also makes the fixed memory cost explicit in the linker map.
    static Arena<8192> arena;
    Tape<float> &tape = arena.tape();
    tape.register_operator<QuantizeOperator<float>>();
    tape.register_operator<DequantizeOperator<float>>();
    tape.register_operator<FullyConnectedOperator<float>>();
    tape.register_operator<QuadraticErrorOperator<float>>();

    BufferView weights = tape.parameter(
        &weight_value[0][0], &weight_gradient[0][0],
        output_count * input_count);
    BufferView bias = tape.parameter(bias_value);
    BufferView weight_scale = tape.parameter(weight_scale_value);
    BufferView weight_zero_point = tape.input(weight_zero_point_value);
    BufferView input_scale = tape.parameter(input_scale_value);
    BufferView input_zero_point = tape.parameter(input_zero_point_value);
    BufferView output_scale = tape.parameter(output_scale_value);
    BufferView output_zero_point = tape.parameter(output_zero_point_value);

    BufferView quantized_weight = tape.output(
        &quantized_weight_value[0][0], output_count * input_count);
    BufferView dequantized_weight = tape.output(
        &dequantized_weight_value[0][0], &dequantized_weight_gradient[0][0],
        output_count * input_count);

    // The FC operator consumes a MatrixView. This matrix aliases the Q/DQ
    // output above, so its gradients continue through dequantize and quantize
    // to the original floating weights and their per-channel scales.
    MatrixView dequantized_weight_matrix = tape.parameter(
        &dequantized_weight_value[0][0], &dequantized_weight_gradient[0][0],
        output_count, input_count);

    BufferView quantized_input = tape.output(quantized_input_value);
    BufferView dequantized_input = tape.output(dequantized_input_value);
    BufferView linear = tape.output(linear_value);
    BufferView quantized_output = tape.output(quantized_output_value);
    BufferView prediction = tape.output(prediction_value);
    BufferView loss = tape.output(loss_value);

    Adam<14U, 8U> optimizer(2.0e-2F);
    if (!optimizer.add(weights) || !optimizer.add(bias) ||
        !optimizer.add(weight_scale) || !optimizer.add(input_scale) ||
        !optimizer.add(input_zero_point) || !optimizer.add(output_scale) ||
        !optimizer.add(output_zero_point) || !tape.good())
    {
        std::printf("QAT setup failed\n");
        return 1;
    }

    tape.begin_graph();
    std::printf("Fully connected signed-int8 QAT\n");
    for (std::size_t step = 0; step < training_steps; ++step)
    {
        if (!tape.rewind_graph()) return 1;
        const std::size_t sample = step % sample_count;
        BufferView input = tape.input(input_value[sample]);
        BufferView target = tape.input(target_value[sample]);

        quantized_weight = quantize(weights, weight_scale,
                                    weight_zero_point, weight_quantization);
        dequantized_weight = dequantize(quantized_weight, weight_scale,
                                        weight_zero_point,
                                        weight_quantization);
        quantized_input = quantize(input, input_scale, input_zero_point,
                                   activation_quantization);
        dequantized_input = dequantize(quantized_input, input_scale,
                                       input_zero_point,
                                       activation_quantization);
        linear = fully_connected(dequantized_input,
                                 dequantized_weight_matrix, bias);
        quantized_output = quantize(linear, output_scale, output_zero_point,
                                    activation_quantization);
        prediction = dequantize(quantized_output, output_scale,
                                output_zero_point,
                                activation_quantization);

        loss = quadratic_error(prediction, target);
        optimizer.zero_grad();
        if (!tape.backward(loss) || !optimizer.step()) return 1;

        if ((step + 1U) % 100U == 0U)
            std::printf("step %u, sample quadratic error = %g\n",
                        static_cast<unsigned>(step + 1U),
                        static_cast<double>(loss_value));
    }

    // Refresh the integer-valued floating weights after the final optimizer
    // update. Casting these values to int8_t is exact.
    {
        RecordingScope inference(tape, false);
        quantized_weight = quantize(weights, weight_scale,
                                    weight_zero_point, weight_quantization);
    }
    std::int8_t exported_weight[output_count][input_count] = {};
    for (std::size_t row = 0; row < output_count; ++row)
        for (std::size_t column = 0; column < input_count; ++column)
            exported_weight[row][column] = static_cast<std::int8_t>(
                std::nearbyint(quantized_weight_value[row][column]));

    std::int32_t exported_bias[output_count] = {};
    for (std::size_t row = 0; row < output_count; ++row)
        exported_bias[row] = static_cast<std::int32_t>(std::nearbyint(
            bias_value[row] /
            (input_scale_value * weight_scale_value[row])));

    std::printf("input: scale=%g, zero_point=%g, CMSIS-NN offset=%d\n",
                static_cast<double>(input_scale_value),
                static_cast<double>(std::nearbyint(input_zero_point_value)),
                static_cast<int>(cmsis_nn_offset(input_zero_point_value)));
    std::printf("output: scale=%g, zero_point=%g, CMSIS-NN offset=%d\n",
                static_cast<double>(output_scale_value),
                static_cast<double>(std::nearbyint(output_zero_point_value)),
                static_cast<int>(cmsis_nn_offset(output_zero_point_value)));
    for (std::size_t row = 0; row < output_count; ++row)
    {
        std::printf("channel %u: weight_scale=%g, weights={%d, %d, %d}, "
                    "bias=%ld\n",
                    static_cast<unsigned>(row),
                    static_cast<double>(weight_scale_value[row]),
                    static_cast<int>(exported_weight[row][0]),
                    static_cast<int>(exported_weight[row][1]),
                    static_cast<int>(exported_weight[row][2]),
                    static_cast<long>(exported_bias[row]));
    }
    return 0;
}
