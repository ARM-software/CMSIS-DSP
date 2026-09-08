#pragma once

#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/matrix_functions.h>
#include <dsp/matrix_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <cstdint>
#include <limits>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Matrix product Y = W X, differentiating only the parameter matrix W. */
template <typename T = float> class MatrixMultiplyOperator
{
    static void fill(T *data, std::size_t length) noexcept
    {
        if constexpr (std::is_same<T, float>::value)
            arm_fill_f32(T{}, data, static_cast<uint32_t>(length));
#if defined(ARM_FLOAT16_SUPPORTED)
        else
            arm_fill_f16(T{}, data, static_cast<uint32_t>(length));
#else
        else
            for (std::size_t i = 0; i < length; ++i) data[i] = T{};
#endif
    }

    struct Record
    {
        detail::Node node;
        T *output_gradient;
        const T *input_value;
        T *weight_gradient;
        std::size_t rows;
        std::size_t inner;
        std::size_t columns;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        fill(record.output_gradient, record.rows * record.columns);
        fill(record.weight_gradient, record.rows * record.inner);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);

        ::arm_cmsis_dsp::MatrixView<T,::arm_cmsis_dsp::DYNAMIC>
            input_value(const_cast<T *>(record.input_value), record.inner,
                        record.columns, record.columns);

        // dW = dY X^T. For each row of dY, this is X times that row. The lazy
        // matvec expression fuses the product with gradient accumulation.

        for (std::size_t row = 0; row < record.rows; ++row)
        {
            ::arm_cmsis_dsp::VectorView<T> output_gradient(
                record.output_gradient + row * record.columns, 0,
                record.columns);
            ::arm_cmsis_dsp::VectorView<T> weight_gradient(
                record.weight_gradient + row * record.inner, 0,
                record.inner);
            weight_gradient +=
                ::arm_cmsis_dsp::matvec(input_value, output_gradient);
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &input,
                         const MatrixView<T> &weights) noexcept
    {
        const BufferView<T> &weight_buffer = OperatorAccess<T>::buffer(weights);
        const std::size_t rows = OperatorAccess<T>::rows(weights);
        const std::size_t inner = OperatorAccess<T>::columns(weights);
        const std::size_t input_length = OperatorAccess<T>::length(input);
        const std::size_t columns = inner == 0U ? 0U : input_length / inner;
        if (!OperatorAccess<T>::valid(tape, output) ||
            !OperatorAccess<T>::valid(tape, input) ||
            !OperatorAccess<T>::valid(tape, weight_buffer) ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::role(input) != BufferRole::input ||
            OperatorAccess<T>::role(weight_buffer) != BufferRole::parameter ||
            OperatorAccess<T>::gradients(weight_buffer) == nullptr ||
            rows == 0U || inner == 0U || columns == 0U ||
            input_length % inner != 0U ||
            rows > std::numeric_limits<std::uint16_t>::max() ||
            inner > std::numeric_limits<std::uint16_t>::max() ||
            columns > std::numeric_limits<std::uint16_t>::max() ||
            OperatorAccess<T>::length(output) != rows * columns)
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        return true;
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input,
                         const MatrixView<T> &weights) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess<T>::template require<MatrixMultiplyOperator<T>>(*tape))
            return false;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, input, weights))
            return false;
#endif

        const BufferView<T> &weight_buffer = OperatorAccess<T>::buffer(weights);
        const std::size_t rows = OperatorAccess<T>::rows(weights);
        const std::size_t inner = OperatorAccess<T>::columns(weights);
        const std::size_t input_length = OperatorAccess<T>::length(input);
        const std::size_t columns = inner == 0U ? 0U : input_length / inner;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        arm_status matrix_status;
#endif
        if constexpr (std::is_same<T, float>::value)
        {
        arm_matrix_instance_f32 weight_matrix;
        arm_matrix_instance_f32 input_matrix;
        arm_matrix_instance_f32 output_matrix;
        arm_mat_init_f32(&weight_matrix, static_cast<std::uint16_t>(rows),
                         static_cast<std::uint16_t>(inner),
                         const_cast<float *>(OperatorAccess<T>::values(weight_buffer)));
        arm_mat_init_f32(&input_matrix, static_cast<std::uint16_t>(inner),
                         static_cast<std::uint16_t>(columns),
                         const_cast<float *>(OperatorAccess<T>::values(input)));
        arm_mat_init_f32(&output_matrix, static_cast<std::uint16_t>(rows),
                         static_cast<std::uint16_t>(columns),
                         OperatorAccess<T>::values(output));
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        matrix_status =
#endif
            arm_mat_mult_f32(&weight_matrix, &input_matrix, &output_matrix);
        }
        else
        {
        arm_matrix_instance_f16 weight_matrix;
        arm_matrix_instance_f16 input_matrix;
        arm_matrix_instance_f16 output_matrix;
        arm_mat_init_f16(&weight_matrix, static_cast<std::uint16_t>(rows),
                         static_cast<std::uint16_t>(inner),
                         const_cast<float16_t *>(OperatorAccess<T>::values(weight_buffer)));
        arm_mat_init_f16(&input_matrix, static_cast<std::uint16_t>(inner),
                         static_cast<std::uint16_t>(columns),
                         const_cast<float16_t *>(OperatorAccess<T>::values(input)));
        arm_mat_init_f16(&output_matrix, static_cast<std::uint16_t>(rows),
                         static_cast<std::uint16_t>(columns),
                         OperatorAccess<T>::values(output));
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        matrix_status =
#endif
            arm_mat_mult_f16(&weight_matrix, &input_matrix, &output_matrix);
        }
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (matrix_status != ARM_MATH_SUCCESS)
        {
            OperatorAccess<T>::fail(*tape, Status::tape_mismatch);
            return false;
        }
#endif

        if (!OperatorAccess<T>::recording(*tape))
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->input_value = OperatorAccess<T>::values(input);
        record->weight_gradient = OperatorAccess<T>::gradients(weight_buffer);
        record->rows = rows;
        record->inner = inner;
        record->columns = columns;
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class MatrixMultiplyExpression
{
public:
    MatrixMultiplyExpression(const BufferView<T> &input,
                             const MatrixView<T> &weights) noexcept
        : input_(input), weights_(weights) {}

    void evaluate(BufferView<T> &output) const noexcept
    {
        MatrixMultiplyOperator<T>::evaluate(output, input_, weights_);
    }

private:
    BufferView<T> input_;
    MatrixView<T> weights_;
};

template <typename T = float>
inline MatrixMultiplyExpression<T> matrix_multiply(
    const BufferView<T> &input, const MatrixView<T> &weights) noexcept
{
    return MatrixMultiplyExpression<T>(input, weights);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
