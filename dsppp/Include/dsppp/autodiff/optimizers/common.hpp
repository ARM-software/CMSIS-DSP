#pragma once

namespace arm_cmsis_dsp {
namespace autodiff {

enum class OptimizerStatus
{
    ok,
    too_many_parameters,
    too_many_elements,
    invalid_parameter
};

/** Freeze all parameter views belonging to one logical operator/layer. */
template <typename Optimizer, typename... Parameters>
bool freeze_parameters(Optimizer &optimizer, Parameters... parameters) noexcept
{
    return (optimizer.freeze(parameters, true) && ...);
}

/** Re-enable updates for a previously frozen operator/layer. */
template <typename Optimizer, typename... Parameters>
bool unfreeze_parameters(Optimizer &optimizer,
                         Parameters... parameters) noexcept
{
    return (optimizer.freeze(parameters, false) && ...);
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
