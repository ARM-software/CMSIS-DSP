#pragma once

#include <cmath>
#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/optimizers/common.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Fixed-storage RMSProp optimizer without momentum or centering. */
template <std::size_t MaximumElements,
          std::size_t MaximumParameters = 16U,
          typename T = float>
class RMSProp
{
    static_assert(MaximumElements > 0U, "RMSProp needs state storage");
    static_assert(MaximumParameters > 0U, "RMSProp needs parameter slots");

    static constexpr T default_epsilon() noexcept
    {
        // 1e-8 is below the useful range of half precision and rounds to zero.
        if constexpr (std::is_same<T, float>::value)
            return T{1.0e-8F};
        return T{1.0e-4F};
    }

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
    static T add(T left, T right) noexcept
    {
        if constexpr (std::is_same<T, float>::value) return left + right;
        return static_cast<T>(static_cast<float>(left) + static_cast<float>(right));
    }
    static T sub(T left, T right) noexcept
    {
        if constexpr (std::is_same<T, float>::value) return left - right;
        return static_cast<T>(static_cast<float>(left) - static_cast<float>(right));
    }
    static T mul(T left, T right) noexcept
    {
        if constexpr (std::is_same<T, float>::value) return left * right;
        return static_cast<T>(static_cast<float>(left) * static_cast<float>(right));
    }
    static T div(T left, T right) noexcept
    {
        if constexpr (std::is_same<T, float>::value) return left / right;
        return static_cast<T>(static_cast<float>(left) / static_cast<float>(right));
    }

    struct Entry
    {
        T *values;
        T *gradients;
        std::size_t length;
        std::size_t offset;
        bool trainable;
    };

public:
    explicit RMSProp(T learning_rate = T{1.0e-3F}, T alpha = T{0.99F},
                     T epsilon = default_epsilon()) noexcept
        : learning_rate_(learning_rate), alpha_(alpha), epsilon_(epsilon),
          parameter_count_(0U), element_count_(0U),
          status_(OptimizerStatus::ok), entries_{}, square_average_{}
    {
    }

    bool add(BufferView<T> parameter) noexcept
    {
        if (parameter.role() != BufferRole::parameter ||
            !parameter.has_gradient())
            return fail(OptimizerStatus::invalid_parameter);
        return add_impl(parameter.values(), parameter.gradients(),
                        parameter.length());
    }
    bool add(MatrixView<T> parameter) noexcept
    {
        return add_impl(parameter.values(), parameter.gradients(),
                        parameter.length());
    }
    bool freeze(BufferView<T> parameter, bool frozen = true) noexcept
    {
        return set_trainable(parameter.values(), !frozen);
    }
    bool freeze(MatrixView<T> parameter, bool frozen = true) noexcept
    {
        return set_trainable(parameter.values(), !frozen);
    }

    void zero_grad() noexcept
    {
        for (std::size_t p = 0; p < parameter_count_; ++p)
            fill(entries_[p].gradients, entries_[p].length);
    }

    bool step() noexcept
    {
        if (status_ != OptimizerStatus::ok) return false;
        for (std::size_t p = 0; p < parameter_count_; ++p)
        {
            Entry &entry = entries_[p];
            if (!entry.trainable) continue;
            ::arm_cmsis_dsp::VectorView<T> gradients(
                entry.gradients, 0, entry.length);
            ::arm_cmsis_dsp::VectorView<T> square_average(
                square_average_ + entry.offset, 0, entry.length);
            const T one_minus_alpha =
                static_cast<T>(1.0F - static_cast<float>(alpha_));
            square_average = square_average * alpha_ +
                gradients * gradients * one_minus_alpha;
            for (std::size_t i = 0; i < entry.length; ++i)
            {
                const std::size_t state = entry.offset + i;
                const T gradient = entry.gradients[i];
                const T denominator = add(
                    static_cast<T>(std::sqrt(
                        static_cast<float>(square_average_[state]))),
                    epsilon_);
                entry.values[i] = sub(
                    entry.values[i],
                    div(mul(learning_rate_, gradient), denominator));
            }
        }
        return true;
    }

    OptimizerStatus status() const noexcept { return status_; }
    bool good() const noexcept { return status_ == OptimizerStatus::ok; }

private:
    bool add_impl(T *values, T *gradients, std::size_t length) noexcept
    {
        if (values == nullptr || gradients == nullptr)
            return fail(OptimizerStatus::invalid_parameter);
        for (std::size_t i = 0; i < parameter_count_; ++i)
            if (entries_[i].values == values) return true;
        if (parameter_count_ == MaximumParameters)
            return fail(OptimizerStatus::too_many_parameters);
        if (length > MaximumElements - element_count_)
            return fail(OptimizerStatus::too_many_elements);
        entries_[parameter_count_++] =
            Entry{values, gradients, length, element_count_, true};
        element_count_ += length;
        return true;
    }
    bool set_trainable(const T *values, bool trainable) noexcept
    {
        for (std::size_t i = 0; i < parameter_count_; ++i)
            if (entries_[i].values == values)
            {
                entries_[i].trainable = trainable;
                return true;
            }
        return fail(OptimizerStatus::invalid_parameter);
    }
    bool fail(OptimizerStatus status) noexcept
    {
        if (status_ == OptimizerStatus::ok) status_ = status;
        return false;
    }

    T learning_rate_;
    T alpha_;
    T epsilon_;
    std::size_t parameter_count_;
    std::size_t element_count_;
    OptimizerStatus status_;
    Entry entries_[MaximumParameters];
    T square_average_[MaximumElements];
};

} // namespace autodiff
} // namespace arm_cmsis_dsp
