#pragma once

#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/matrix_functions.h>
#include <dsp/support_functions.h>

#include <cstdint>
#include <limits>

#define dbgInst(imm) __asm volatile("DBG %0\n\t" : :"Ir" ((imm)) )
#define startSectionNB(num) dbgInst(((num) & 0x7) | 0x0)
#define stopSectionNB(num)  dbgInst(((num) & 0x7) | 0x8)

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

        ::arm_cmsis_dsp::MatrixView<float,::arm_cmsis_dsp::DYNAMIC>
            input_value(const_cast<float *>(record.input_value), record.inner,
                        record.columns, record.columns);

        // dW = dY X^T. For each row of dY, this is X times that row. The lazy
        // matvec expression fuses the product with gradient accumulation.

        startSectionNB(1);
        for (std::size_t row = 0; row < record.rows; ++row)
        {
            ::arm_cmsis_dsp::VectorView<float> output_gradient(
                record.output_gradient + row * record.columns, 0,
                record.columns);
            ::arm_cmsis_dsp::VectorView<float> weight_gradient(
                record.weight_gradient + row * record.inner, 0,
                record.inner);
            weight_gradient +=
                ::arm_cmsis_dsp::matvec(input_value, output_gradient);
        }
        stopSectionNB(1);
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
