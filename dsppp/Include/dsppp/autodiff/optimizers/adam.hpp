#pragma once

#include <cmath>
#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/optimizers/common.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Fixed-storage Adam optimizer for use in a user-written training loop. */
template <std::size_t MaximumElements,
          std::size_t MaximumParameters = 16U,
          typename T = float>
class Adam
{
    static_assert(MaximumElements > 0U, "Adam needs state storage");
    static_assert(MaximumParameters > 0U, "Adam needs parameter slots");

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
    explicit Adam(T learning_rate = T{1.0e-3F}, T beta1 = T{0.9F},
                  T beta2 = T{0.999F}, T epsilon = default_epsilon()) noexcept
        : learning_rate_(learning_rate), beta1_(beta1), beta2_(beta2),
          epsilon_(epsilon), beta1_power_(T{1}), beta2_power_(T{1}),
          parameter_count_(0U), element_count_(0U), status_(OptimizerStatus::ok),
          entries_{}, first_moment_{}, second_moment_{}
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
        beta1_power_ = mul(beta1_power_, beta1_);
        beta2_power_ = mul(beta2_power_, beta2_);
        const T first_correction =
            static_cast<T>(1.0F - static_cast<float>(beta1_power_));
        const T second_correction =
            static_cast<T>(1.0F - static_cast<float>(beta2_power_));
        const T one_minus_beta1 =
            static_cast<T>(1.0F - static_cast<float>(beta1_));
        const T one_minus_beta2 =
            static_cast<T>(1.0F - static_cast<float>(beta2_));
        for (std::size_t p = 0; p < parameter_count_; ++p)
        {
            Entry &entry = entries_[p];
            if (!entry.trainable) continue;
            ::arm_cmsis_dsp::VectorView<T> gradients(
                entry.gradients, 0, entry.length);
            ::arm_cmsis_dsp::VectorView<T> first_moment(
                first_moment_ + entry.offset, 0, entry.length);
            ::arm_cmsis_dsp::VectorView<T> second_moment(
                second_moment_ + entry.offset, 0, entry.length);
            first_moment = first_moment * beta1_ + gradients * one_minus_beta1;
            second_moment = second_moment * beta2_ +
                gradients * gradients * one_minus_beta2;
            for (std::size_t i = 0; i < entry.length; ++i)
            {
                const std::size_t state = entry.offset + i;
                const T corrected_first = div(first_moment_[state], first_correction);
                const T corrected_second = div(second_moment_[state], second_correction);
                const T denominator = add(
                    static_cast<T>(std::sqrt(static_cast<float>(corrected_second))),
                    epsilon_);
                entry.values[i] = sub(
                    entry.values[i],
                    div(mul(learning_rate_, corrected_first), denominator));
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
    T beta1_;
    T beta2_;
    T epsilon_;
    T beta1_power_;
    T beta2_power_;
    std::size_t parameter_count_;
    std::size_t element_count_;
    OptimizerStatus status_;
    Entry entries_[MaximumParameters];
    T first_moment_[MaximumElements];
    T second_moment_[MaximumElements];
};

} // namespace autodiff
} // namespace arm_cmsis_dsp
