#pragma once

#include <cmath>
#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/optimizers/common.hpp>
#include <dsppp/matrix.hpp>

#include <dsp/support_functions.h>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Fixed-storage Adam optimizer for use in a user-written training loop. */
template <std::size_t MaximumElements,
          std::size_t MaximumParameters = 16U>
class Adam
{
    static_assert(MaximumElements > 0U, "Adam needs state storage");
    static_assert(MaximumParameters > 0U, "Adam needs parameter slots");
    struct Entry
    {
        float *values;
        float *gradients;
        std::size_t length;
        std::size_t offset;
        bool trainable;
    };

public:
    explicit Adam(float learning_rate = 1.0e-3F, float beta1 = 0.9F,
                  float beta2 = 0.999F, float epsilon = 1.0e-8F) noexcept
        : learning_rate_(learning_rate), beta1_(beta1), beta2_(beta2),
          epsilon_(epsilon), beta1_power_(1.0F), beta2_power_(1.0F),
          parameter_count_(0U), element_count_(0U), status_(OptimizerStatus::ok),
          entries_{}, first_moment_{}, second_moment_{}
    {
    }

    bool add(BufferView parameter) noexcept
    {
        if (parameter.role() != BufferRole::parameter ||
            !parameter.has_gradient())
            return fail(OptimizerStatus::invalid_parameter);
        return add_impl(parameter.values(), parameter.gradients(),
                        parameter.length());
    }

    bool add(MatrixView parameter) noexcept
    {
        return add_impl(parameter.values(), parameter.gradients(),
                        parameter.length());
    }

    bool freeze(BufferView parameter, bool frozen = true) noexcept
    {
        return set_trainable(parameter.values(), !frozen);
    }
    bool freeze(MatrixView parameter, bool frozen = true) noexcept
    {
        return set_trainable(parameter.values(), !frozen);
    }

    void zero_grad() noexcept
    {
        for (std::size_t p = 0; p < parameter_count_; ++p)
            arm_fill_f32(0.0F, entries_[p].gradients,
                         entries_[p].length);
    }

    bool step() noexcept
    {
        if (status_ != OptimizerStatus::ok) return false;
        beta1_power_ *= beta1_;
        beta2_power_ *= beta2_;
        const float first_correction = 1.0F - beta1_power_;
        const float second_correction = 1.0F - beta2_power_;
        for (std::size_t p = 0; p < parameter_count_; ++p)
        {
            Entry &entry = entries_[p];
            if (!entry.trainable) continue;
            ::arm_cmsis_dsp::VectorView<float> gradients(
                entry.gradients, 0, entry.length);
            ::arm_cmsis_dsp::VectorView<float> first_moment(
                first_moment_ + entry.offset, 0, entry.length);
            ::arm_cmsis_dsp::VectorView<float> second_moment(
                second_moment_ + entry.offset, 0, entry.length);
            first_moment = first_moment * beta1_ +
                gradients * (1.0F - beta1_);
            second_moment = second_moment * beta2_ +
                gradients * gradients * (1.0F - beta2_);
            for (std::size_t i = 0; i < entry.length; ++i)
            {
                const std::size_t state = entry.offset + i;
                const float corrected_first =
                    first_moment_[state] / first_correction;
                const float corrected_second =
                    second_moment_[state] / second_correction;
                entry.values[i] -= learning_rate_ * corrected_first /
                    (std::sqrt(corrected_second) + epsilon_);
            }
        }
        return true;
    }

    OptimizerStatus status() const noexcept { return status_; }
    bool good() const noexcept { return status_ == OptimizerStatus::ok; }

private:
    bool add_impl(float *values, float *gradients, std::size_t length) noexcept
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

    bool set_trainable(const float *values, bool trainable) noexcept
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

    float learning_rate_;
    float beta1_;
    float beta2_;
    float epsilon_;
    float beta1_power_;
    float beta2_power_;
    std::size_t parameter_count_;
    std::size_t element_count_;
    OptimizerStatus status_;
    Entry entries_[MaximumParameters];
    float first_moment_[MaximumElements];
    float second_moment_[MaximumElements];
};

} // namespace autodiff
} // namespace arm_cmsis_dsp
