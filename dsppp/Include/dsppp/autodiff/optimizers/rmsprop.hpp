#pragma once

#include <cmath>
#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/optimizers/common.hpp>

namespace arm_cmsis_dsp {
namespace autodiff {

/** Fixed-storage RMSProp optimizer without momentum or centering. */
template <std::size_t MaximumElements,
          std::size_t MaximumParameters = 16U>
class RMSProp
{
    static_assert(MaximumElements > 0U, "RMSProp needs state storage");
    static_assert(MaximumParameters > 0U, "RMSProp needs parameter slots");

    struct Entry
    {
        float *values;
        float *gradients;
        std::size_t length;
        std::size_t offset;
        bool trainable;
    };

public:
    explicit RMSProp(float learning_rate = 1.0e-3F, float alpha = 0.99F,
                     float epsilon = 1.0e-8F) noexcept
        : learning_rate_(learning_rate), alpha_(alpha), epsilon_(epsilon),
          parameter_count_(0U), element_count_(0U),
          status_(OptimizerStatus::ok), entries_{}, square_average_{}
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
            for (std::size_t i = 0; i < entries_[p].length; ++i)
                entries_[p].gradients[i] = 0.0F;
    }

    bool step() noexcept
    {
        if (status_ != OptimizerStatus::ok) return false;
        for (std::size_t p = 0; p < parameter_count_; ++p)
        {
            Entry &entry = entries_[p];
            if (!entry.trainable) continue;
            for (std::size_t i = 0; i < entry.length; ++i)
            {
                const std::size_t state = entry.offset + i;
                const float gradient = entry.gradients[i];
                square_average_[state] = alpha_ * square_average_[state] +
                    (1.0F - alpha_) * gradient * gradient;
                entry.values[i] -= learning_rate_ * gradient /
                    (std::sqrt(square_average_[state]) + epsilon_);
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
    float alpha_;
    float epsilon_;
    std::size_t parameter_count_;
    std::size_t element_count_;
    OptimizerStatus status_;
    Entry entries_[MaximumParameters];
    float square_average_[MaximumElements];
};

} // namespace autodiff
} // namespace arm_cmsis_dsp
