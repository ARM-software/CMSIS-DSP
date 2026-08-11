#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsp/basic_math_functions.h>
#include <dsp/matrix_functions.h>
#include <dsp/support_functions.h>

#include <cstdint>
#include <limits>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Matrix product Y = W X, differentiating only the parameter matrix W. */
class MatrixMultiplyOperator
{
    struct Record
    {
        detail::Node node;
        float *output_gradient;
        const float *input_value;
        float *weight_gradient;
        std::size_t rows;
        std::size_t inner;
        std::size_t columns;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        arm_fill_f32(0.0F, record.output_gradient,
                     record.rows * record.columns);
        arm_fill_f32(0.0F, record.weight_gradient,
                     record.rows * record.inner);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);

        // dW = dY X^T. X and dY are row-major, so every term needed for one
        // element of dW is read from one row of each matrix.
        for (std::size_t row = 0; row < record.rows; ++row)
        {
            const float *output_gradient =
                record.output_gradient + row * record.columns;
            // Unrolling could improve the performances but there are not yet any abstraction 
            // in C++ API to do it.
            // We do not want to write a custom not generic implementation
            // using low level intrinscis.
            for (std::size_t inner = 0; inner < record.inner; ++inner)
            {
                const float *input =
                    record.input_value + inner * record.columns;
                float sum = 0.0F;
                arm_dot_prod_f32(output_gradient, input, record.columns,
                                 &sum);
                record.weight_gradient[row * record.inner + inner] += sum;
            }
        }
    }

public:
    static bool evaluate(BufferView &output, const BufferView &input,
                         const MatrixView &weights) noexcept
    {
        Tape *tape = OperatorAccess::tape(output);
        OperatorAccess::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess::require<MatrixMultiplyOperator>(*tape))
            return false;

        const BufferView &weight_buffer = OperatorAccess::buffer(weights);
        const std::size_t rows = OperatorAccess::rows(weights);
        const std::size_t inner = OperatorAccess::columns(weights);
        const std::size_t input_length = OperatorAccess::length(input);
        const std::size_t columns = inner == 0U ? 0U : input_length / inner;
        if (!OperatorAccess::valid(*tape, output) ||
            !OperatorAccess::valid(*tape, input) ||
            !OperatorAccess::valid(*tape, weight_buffer) ||
            OperatorAccess::gradients(output) == nullptr ||
            OperatorAccess::role(input) != BufferRole::input ||
            OperatorAccess::role(weight_buffer) != BufferRole::parameter ||
            OperatorAccess::gradients(weight_buffer) == nullptr ||
            rows == 0U || inner == 0U || columns == 0U ||
            input_length % inner != 0U ||
            rows > std::numeric_limits<std::uint16_t>::max() ||
            inner > std::numeric_limits<std::uint16_t>::max() ||
            columns > std::numeric_limits<std::uint16_t>::max() ||
            OperatorAccess::length(output) != rows * columns)
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }

        arm_matrix_instance_f32 weight_matrix;
        arm_matrix_instance_f32 input_matrix;
        arm_matrix_instance_f32 output_matrix;
        arm_mat_init_f32(&weight_matrix, static_cast<std::uint16_t>(rows),
                         static_cast<std::uint16_t>(inner),
                         const_cast<float *>(OperatorAccess::values(
                             weight_buffer)));
        arm_mat_init_f32(&input_matrix, static_cast<std::uint16_t>(inner),
                         static_cast<std::uint16_t>(columns),
                         const_cast<float *>(OperatorAccess::values(input)));
        arm_mat_init_f32(&output_matrix, static_cast<std::uint16_t>(rows),
                         static_cast<std::uint16_t>(columns),
                         OperatorAccess::values(output));
        if (arm_mat_mult_f32(&weight_matrix, &input_matrix, &output_matrix) !=
            ARM_MATH_SUCCESS)
        {
            OperatorAccess::fail(*tape, Status::tape_mismatch);
            return false;
        }

        if (!OperatorAccess::recording(*tape))
            return OperatorAccess::status(*tape) == Status::ok;

        Record *record = OperatorAccess::append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess::gradients(output);
        record->input_value = OperatorAccess::values(input);
        record->weight_gradient = OperatorAccess::gradients(weight_buffer);
        record->rows = rows;
        record->inner = inner;
        record->columns = columns;
        OperatorAccess::set_producer(output, &record->node);
        return true;
    }
};

class MatrixMultiplyExpression
{
public:
    MatrixMultiplyExpression(const BufferView &input,
                             const MatrixView &weights) noexcept
        : input_(input), weights_(weights) {}

    void evaluate(BufferView &output) const noexcept
    {
        MatrixMultiplyOperator::evaluate(output, input_, weights_);
    }

private:
    BufferView input_;
    MatrixView weights_;
};

inline MatrixMultiplyExpression matrix_multiply(
    const BufferView &input, const MatrixView &weights) noexcept
{
    return MatrixMultiplyExpression(input, weights);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
