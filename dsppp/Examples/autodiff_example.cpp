#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/fully_connected.hpp>
#include <dsppp/autodiff/operators/relu.hpp>

#include <cstdio>

using namespace arm_cmsis_dsp::autodiff;

int main()
{
    Arena<2048> arena;
    Tape &tape = arena.tape();
    tape.register_operator<FullyConnectedOperator>();
    tape.register_operator<ReluOperator>();

    float x_value[] = {2.0F, -1.0F};
    float matrix_value[2][2] = {{1.0F, 2.0F}, {-3.0F, 1.0F}};
    float bias_value[] = {1.0F, 0.0F};
    float linear_value[2] = {};
    float activation_value[2] = {};

    BufferView x = tape.input(x_value);
    MatrixView matrix = tape.parameter(matrix_value);
    BufferView bias = tape.parameter(bias_value);
    BufferView linear = tape.output(linear_value);
    BufferView activation = tape.output(activation_value);

    linear = fully_connected(x, matrix, bias);
    activation = relu(linear);

    // Seed both ReLU outputs. The second neuron is negative, so ReLU blocks
    // its gradient during the backward pass.
    const float seed[] = {1.0F, 1.0F};
    if (!tape.backward(activation, seed, 2))
    {
        return 1;
    }

    std::printf("linear = {%g, %g}\n", static_cast<double>(linear[0]),
                static_cast<double>(linear[1]));
    std::printf("relu = {%g, %g}\n", static_cast<double>(activation[0]),
                static_cast<double>(activation[1]));
    std::printf("db = {%g, %g}\n", static_cast<double>(bias.gradient(0)),
                static_cast<double>(bias.gradient(1)));
    for (std::size_t row = 0; row < matrix.rows(); ++row)
    {
        for (std::size_t column = 0; column < matrix.columns(); ++column)
        {
            std::printf("dm[%u][%u] = %g\n", static_cast<unsigned>(row),
                        static_cast<unsigned>(column),
                        static_cast<double>(matrix.gradient(row, column)));
        }
    }
    return 0;
}
