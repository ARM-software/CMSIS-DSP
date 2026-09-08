#pragma once

#include <dsppp/autodiff/reverse.hpp>

#include <dsppp/memory_pool.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/matrix_functions.h>
#include <dsp/matrix_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <limits>

namespace arm_cmsis_dsp {
namespace autodiff {

template <typename T = float> class FullyConnectedOperator
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
        T *input_gradient;
        const T *weight_value;
        T *weight_gradient;
        T *bias_gradient;
        std::size_t rows;
        std::size_t columns;
    };

    static void reset(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        fill(record.output_gradient, record.rows);
        fill(record.bias_gradient, record.rows);
        fill(record.weight_gradient, record.rows * record.columns);
        if (record.input_gradient != nullptr && record.columns != 0U)
            fill(record.input_gradient, record.columns);
    }

    static void backward(detail::Node &node) noexcept
    {
        Record &record = reinterpret_cast<Record &>(node);
        if (record.rows == 0U) return;

        ::arm_cmsis_dsp::VectorView<T> output_gradient(
            record.output_gradient, 0, record.rows);
        ::arm_cmsis_dsp::VectorView<T> bias_gradient(
            record.bias_gradient, 0, record.rows);

        // Bias is shared by every input sample, so its gradient must be
        // accumulated instead of overwritten.
        bias_gradient += output_gradient;

        if (record.columns != 0U)
        {
            ::arm_cmsis_dsp::VectorView<T> input_value(
                const_cast<T *>(record.input_value), 0, record.columns);
            ::arm_cmsis_dsp::MatrixView<T,
                                        ::arm_cmsis_dsp::DYNAMIC>
                weight_gradient(record.weight_gradient, record.rows,
                                record.columns, record.columns);

            // dW = dy outer x. The C++ expression engine fuses the multiply
            // and accumulation without constructing an outer-product buffer.
            weight_gradient +=
                ::arm_cmsis_dsp::outer(output_gradient, input_value);

            if (record.input_gradient == nullptr) return;

            ::arm_cmsis_dsp::VectorView<T> input_gradient(
                record.input_gradient, 0, record.columns);
            ::arm_cmsis_dsp::MatrixView<T,
                                        ::arm_cmsis_dsp::DYNAMIC>
                weight_value(const_cast<T *>(record.weight_value),
                             record.rows, record.columns, record.columns);
            input_gradient += ::arm_cmsis_dsp::dot(
                ::arm_cmsis_dsp::transpose_view(weight_value),
                output_gradient);
        }
    }

    static bool validate(Tape<T> &tape, const BufferView<T> &output,
                         const BufferView<T> &input,
                         const MatrixView<T> &weights,
                         const BufferView<T> &bias) noexcept
    {
        const BufferView<T> &weight_buffer = OperatorAccess<T>::buffer(weights);
        if (!OperatorAccess<T>::valid(tape, output) ||
            !OperatorAccess<T>::valid(tape, input) ||
            !OperatorAccess<T>::valid(tape, weight_buffer) ||
            !OperatorAccess<T>::valid(tape, bias) ||
            OperatorAccess<T>::gradients(output) == nullptr ||
            OperatorAccess<T>::length(output) != OperatorAccess<T>::rows(weights) ||
            OperatorAccess<T>::length(input) != OperatorAccess<T>::columns(weights) ||
            OperatorAccess<T>::length(bias) != OperatorAccess<T>::rows(weights) ||
            OperatorAccess<T>::rows(weights) >
                std::numeric_limits<std::uint16_t>::max() ||
            OperatorAccess<T>::columns(weights) >
                std::numeric_limits<std::uint16_t>::max() ||
            OperatorAccess<T>::role(weight_buffer) != BufferRole::parameter ||
            OperatorAccess<T>::role(bias) != BufferRole::parameter)
        {
            OperatorAccess<T>::fail(tape, Status::tape_mismatch);
            return false;
        }
        return true;
    }

public:
    static bool evaluate(BufferView<T> &output, const BufferView<T> &input,
                         const MatrixView<T> &weights,
                         const BufferView<T> &bias) noexcept
    {
        Tape<T> *tape = OperatorAccess<T>::tape(output);
        OperatorAccess<T>::set_producer(output, nullptr);
        if (tape == nullptr ||
            !OperatorAccess<T>::template require<FullyConnectedOperator<T>>(*tape))
            return false;
#if DSPPP_AUTODIFF_ENABLE_VALIDATION
        if (!validate(*tape, output, input, weights, bias))
            return false;
#endif
        const BufferView<T> &weight_buffer = OperatorAccess<T>::buffer(weights);
        const std::size_t rows = OperatorAccess<T>::rows(weights);
        const std::size_t columns = OperatorAccess<T>::columns(weights);
        if (rows != 0U)
        {
            ::arm_cmsis_dsp::VectorView<T> output_value(
                OperatorAccess<T>::values(output), 0, rows);
            ::arm_cmsis_dsp::VectorView<T> bias_value(
                const_cast<T *>(OperatorAccess<T>::values(bias)), 0, rows);

            if (columns != 0U)
            {
                if constexpr (std::is_same<T, float>::value)
                {
                arm_matrix_instance_f32 weight_matrix;
                arm_mat_init_f32(
                    &weight_matrix, static_cast<std::uint16_t>(rows),
                    static_cast<std::uint16_t>(columns),
                    const_cast<float *>(OperatorAccess<T>::values(weight_buffer)));
                arm_mat_vec_mult_f32(&weight_matrix,
                                     OperatorAccess<T>::values(input),
                                     OperatorAccess<T>::values(output));
                }
                else
                {
                arm_matrix_instance_f16 weight_matrix;
                arm_mat_init_f16(
                    &weight_matrix, static_cast<std::uint16_t>(rows),
                    static_cast<std::uint16_t>(columns),
                    const_cast<float16_t *>(OperatorAccess<T>::values(weight_buffer)));
                arm_mat_vec_mult_f16(&weight_matrix,
                                     OperatorAccess<T>::values(input),
                                     OperatorAccess<T>::values(output));
                }
                output_value += bias_value;
            }
            else
            {
                // VectorView deliberately deletes copy assignment. This
                // dimension-zero edge has no matrix product to optimize.
                for (std::size_t row = 0; row < rows; ++row)
                        OperatorAccess<T>::values(output)[row] =
                        OperatorAccess<T>::values(bias)[row];
            }
        }
        if (!OperatorAccess<T>::recording(*tape) ||
            OperatorAccess<T>::length(output) == 0U)
            return OperatorAccess<T>::status(*tape) == Status::ok;

        Record *record = OperatorAccess<T>::template append<Record>(*tape, backward, reset);
        if (record == nullptr) return false;
        record->output_gradient = OperatorAccess<T>::gradients(output);
        record->input_value = OperatorAccess<T>::values(input);
        record->input_gradient = OperatorAccess<T>::gradients(input);
        record->weight_value = OperatorAccess<T>::values(weight_buffer);
        record->weight_gradient = OperatorAccess<T>::gradients(weight_buffer);
        record->bias_gradient = OperatorAccess<T>::gradients(bias);
        record->rows = OperatorAccess<T>::rows(weights);
        record->columns = OperatorAccess<T>::columns(weights);
        OperatorAccess<T>::set_producer(output, &record->node);
        return true;
    }
};

template <typename T = float> class FullyConnectedExpression
{
public:
    FullyConnectedExpression(const BufferView<T> &input, const MatrixView<T> &weights,
                             const BufferView<T> &bias) noexcept
        : input_(input), weights_(weights), bias_(bias) {}
    void evaluate(BufferView<T> &output) const noexcept
    {
        FullyConnectedOperator<T>::evaluate(output, input_, weights_, bias_);
    }
private:
    BufferView<T> input_;
    MatrixView<T> weights_;
    BufferView<T> bias_;
};

template <typename T = float>
inline FullyConnectedExpression<T> fully_connected(
    const BufferView<T> &input, const MatrixView<T> &weights,
    const BufferView<T> &bias) noexcept
{
    return FullyConnectedExpression<T>(input, weights, bias);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
