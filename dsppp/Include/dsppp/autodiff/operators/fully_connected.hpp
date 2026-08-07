#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/memory_pool.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/matrix_functions.h>

#include <limits>

namespace arm_cmsis_dsp {
namespace autodiff {

class FullyConnectedOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *input_value;
        float *input_gradient;
        const float *weight_value;
        float *weight_gradient;
        float *bias_gradient;
        std::size_t rows;
        std::size_t columns;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.rows != 0U)
        {
            ::arm_cmsis_dsp::VectorView<float> output_gradient(
                record.output_gradient, 0, record.rows);
            ::arm_cmsis_dsp::VectorView<float> bias_gradient(
                record.bias_gradient, 0, record.rows);
            output_gradient = 0.0F;
            bias_gradient = 0.0F;
        }
        if (record.rows != 0U && record.columns != 0U)
        {
            ::arm_cmsis_dsp::MatrixView<float,
                                        ::arm_cmsis_dsp::DYNAMIC>
                weight_gradient(record.weight_gradient, record.rows,
                                record.columns, record.columns);
            weight_gradient = 0.0F;
        }
        if (record.input_gradient != nullptr && record.columns != 0U)
        {
            ::arm_cmsis_dsp::VectorView<float> input_gradient(
                record.input_gradient, 0, record.columns);
            input_gradient = 0.0F;
        }
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.rows == 0U) return;

        ::arm_cmsis_dsp::VectorView<float> output_gradient(
            record.output_gradient, 0, record.rows);
        ::arm_cmsis_dsp::VectorView<float> bias_gradient(
            record.bias_gradient, 0, record.rows);

        // Bias is shared by every input sample, so its gradient must be
        // accumulated instead of overwritten.
        bias_gradient += output_gradient;

        if (record.columns != 0U)
        {
            ::arm_cmsis_dsp::VectorView<float> input_value(
                const_cast<float *>(record.input_value), 0, record.columns);
            ::arm_cmsis_dsp::MatrixView<float,
                                        ::arm_cmsis_dsp::DYNAMIC>
                weight_gradient(record.weight_gradient, record.rows,
                                record.columns, record.columns);

            // dW = dy outer x. The C++ expression engine fuses the multiply
            // and accumulation without constructing an outer-product buffer.
            weight_gradient +=
                ::arm_cmsis_dsp::outer(output_gradient, input_value);

            if (record.input_gradient == nullptr) return;

            ::arm_cmsis_dsp::VectorView<float> input_gradient(
                record.input_gradient, 0, record.columns);
            for (std::size_t column = 0; column < record.columns; ++column)
            {
                // A column is strided in the row-major weight matrix. The
                // C++ dot implementation handles that view directly, so no
                // transposed matrix or temporary vector is needed.
                ::arm_cmsis_dsp::VectorView<float,
                                            ::arm_cmsis_dsp::DYNAMIC>
                    weight_column(const_cast<float *>(record.weight_value),
                                  column, record.rows * record.columns,
                                  record.columns);
                input_gradient[column] +=
                    ::arm_cmsis_dsp::dot(weight_column, output_gradient);
            }
        }
    }

public:
    static bool evaluate(BufferView &output, const BufferView &input,
                         const MatrixView &weights,
                         const BufferView &bias) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess::require<FullyConnectedOperator>(*tape))
            return false;
        const BufferView &weight_buffer = OperatorAccess::buffer(weights);
        if (!OperatorAccess::valid(*tape, output) ||
            !OperatorAccess::valid(*tape, input) ||
            !OperatorAccess::valid(*tape, weight_buffer) ||
            !OperatorAccess::valid(*tape, bias) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::length(output) != OperatorAccess::rows(weights) ||
            OperatorAccess::length(input) != OperatorAccess::columns(weights) ||
            OperatorAccess::length(bias) != OperatorAccess::rows(weights) ||
            OperatorAccess::rows(weights) >
                std::numeric_limits<std::uint16_t>::max() ||
            OperatorAccess::columns(weights) >
                std::numeric_limits<std::uint16_t>::max() ||
            OperatorAccess::role(weight_buffer) != BufferRole::parameter ||
            OperatorAccess::role(bias) != BufferRole::parameter)
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }

        const std::size_t rows = OperatorAccess::rows(weights);
        const std::size_t columns = OperatorAccess::columns(weights);
        if (rows != 0U)
        {
            ::arm_cmsis_dsp::VectorView<float> output_value(
                OperatorAccess::values(output), 0, rows);
            ::arm_cmsis_dsp::VectorView<float> bias_value(
                const_cast<float *>(OperatorAccess::values(bias)), 0, rows);

            if (columns != 0U)
            {
                arm_matrix_instance_f32 weight_matrix;
                arm_mat_init_f32(
                    &weight_matrix, static_cast<std::uint16_t>(rows),
                    static_cast<std::uint16_t>(columns),
                    const_cast<float *>(OperatorAccess::values(weight_buffer)));
                arm_mat_vec_mult_f32(&weight_matrix,
                                     OperatorAccess::values(input),
                                     OperatorAccess::values(output));
                output_value += bias_value;
            }
            else
            {
                // VectorView deliberately deletes copy assignment. This
                // dimension-zero edge has no matrix product to optimize.
                for (std::size_t row = 0; row < rows; ++row)
                    OperatorAccess::values(output)[row] =
                        OperatorAccess::values(bias)[row];
            }
        }
        if (!OperatorAccess::recording(*tape) ||
            OperatorAccess::length(output) == 0U)
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->input_value = OperatorAccess::values(input);
        record->input_gradient = OperatorAccess::gradients(input);
        record->weight_value = OperatorAccess::values(weight_buffer);
        record->weight_gradient = OperatorAccess::gradients(weight_buffer);
        record->bias_gradient = OperatorAccess::gradients(bias);
        record->rows = OperatorAccess::rows(weights);
        record->columns = OperatorAccess::columns(weights);
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class FullyConnectedExpression
{
public:
    FullyConnectedExpression(const BufferView &input, const MatrixView &weights,
                             const BufferView &bias) noexcept
        : input_(input), weights_(weights), bias_(bias) {}
    void evaluate(BufferView &output) const noexcept
    {
        FullyConnectedOperator::evaluate(output, input_, weights_, bias_);
    }
private:
    BufferView input_;
    MatrixView weights_;
    BufferView bias_;
};

inline FullyConnectedExpression fully_connected(
    const BufferView &input, const MatrixView &weights,
    const BufferView &bias) noexcept
{
    return FullyConnectedExpression(input, weights, bias);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
