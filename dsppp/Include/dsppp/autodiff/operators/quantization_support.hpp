#pragma once

#include <arm_math_types_f16.h>
#include <dsp/basic_math_functions.h>
#include <dsp/basic_math_functions_f16.h>
#include <dsp/support_functions.h>
#include <dsp/support_functions_f16.h>

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <limits>
#include <type_traits>

namespace arm_cmsis_dsp {
namespace autodiff {

/** LiteRT/CMSIS-NN/Ethos-U signed int8 quantization layout. */
class Int8Quantization
{
public:
    constexpr Int8Quantization() noexcept
        : quant_min_(-128), quant_max_(127), parameter_count_(1U),
          inner_size_(1U), asymmetric_(true)
    {
    }

    /** Per-tensor asymmetric activation quantization. */
    static constexpr Int8Quantization activation() noexcept
    {
        return Int8Quantization(-128, 127, 1U, 1U, true);
    }

    /**
     * Symmetric per-axis weight quantization.
     *
     * axis_size is the number of scales and inner_size is the product of the
     * dimensions following the quantized axis in a row-major tensor. Thus a
     * fully-connected [output, input] matrix uses (output, input), Conv2D
     * OHWI uses (output, H*W*I), and depthwise Conv2D uses (channels, 1).
     */
    static constexpr Int8Quantization weights(std::size_t axis_size,
                                               std::size_t inner_size) noexcept
    {
        return Int8Quantization(-127, 127, axis_size, inner_size, false);
    }

    constexpr int quant_min() const noexcept { return quant_min_; }
    constexpr int quant_max() const noexcept { return quant_max_; }
    constexpr std::size_t parameter_count() const noexcept
    {
        return parameter_count_;
    }
    constexpr std::size_t inner_size() const noexcept { return inner_size_; }
    constexpr bool asymmetric() const noexcept { return asymmetric_; }

    constexpr std::size_t parameter_index(std::size_t element) const noexcept
    {
        return parameter_count_ == 1U
                   ? 0U
                   : (element / inner_size_) % parameter_count_;
    }

    constexpr bool valid_for(std::size_t length) const noexcept
    {
        return parameter_count_ != 0U && inner_size_ != 0U &&
               (parameter_count_ == 1U ||
                ((length % parameter_count_) == 0U &&
                 (length / parameter_count_) % inner_size_ == 0U));
    }

    /** Keep learned scales in the numerically safe, strictly-positive range. */
    template <typename T>
    void constrain_scale(T &scale) const noexcept
    {
        if (static_cast<float>(scale) < minimum_scale())
            scale = static_cast<T>(minimum_scale());
    }

    template <typename T, std::size_t Length>
    void constrain_scales(T (&scales)[Length]) const noexcept
    {
        for (std::size_t i = 0; i < Length; ++i)
            constrain_scale(scales[i]);
    }

    /**
     * Keep learned zero-points in this scheme's integer domain. Symmetric
     * weight quantization always forces them to zero.
     */
    template <typename T>
    void constrain_zero_point(T &zero_point) const noexcept
    {
        if (!asymmetric_)
        {
            zero_point = T{};
            return;
        }
        if (static_cast<float>(zero_point) < static_cast<float>(quant_min_))
            zero_point = static_cast<T>(quant_min_);
        if (static_cast<float>(zero_point) > static_cast<float>(quant_max_))
            zero_point = static_cast<T>(quant_max_);
    }

    template <typename T, std::size_t Length>
    void constrain_zero_points(T (&zero_points)[Length]) const noexcept
    {
        for (std::size_t i = 0; i < Length; ++i)
            constrain_zero_point(zero_points[i]);
    }

    /** Project all learned parameters into this backend quantization scheme. */
    template <typename T>
    void constrain_parameters(T *scales, T *zero_points) const noexcept
    {
        if constexpr (std::is_same<T, float>::value)
        {
            arm_clip_f32(scales, scales, minimum_scale(),
                         std::numeric_limits<float>::max(),
                         static_cast<uint32_t>(parameter_count_));
            if (asymmetric_)
                arm_clip_f32(zero_points, zero_points,
                             static_cast<float>(quant_min_),
                             static_cast<float>(quant_max_),
                             static_cast<uint32_t>(parameter_count_));
            else
                arm_fill_f32(0.0F, zero_points,
                             static_cast<uint32_t>(parameter_count_));
        }
#if defined(ARM_FLOAT16_SUPPORTED)
        else if constexpr (std::is_same<T, float16_t>::value)
        {
            arm_clip_f16(scales, scales, static_cast<float16_t>(minimum_scale()),
                         F16_MAX, static_cast<uint32_t>(parameter_count_));
            if (asymmetric_)
                arm_clip_f16(zero_points, zero_points,
                             static_cast<float16_t>(quant_min_),
                             static_cast<float16_t>(quant_max_),
                             static_cast<uint32_t>(parameter_count_));
            else
                arm_fill_f16(static_cast<float16_t>(0.0F), zero_points,
                             static_cast<uint32_t>(parameter_count_));
        }
#endif
        else
            for (std::size_t i = 0; i < parameter_count_; ++i)
            {
                constrain_scale(scales[i]);
                constrain_zero_point(zero_points[i]);
            }
    }

private:
    // The backend only requires scale > 0. This small floor additionally
    // prevents division by zero after a finite-precision optimizer update.
    static constexpr float minimum_scale() noexcept { return 1.0e-4F; }

    constexpr Int8Quantization(int quant_min, int quant_max,
                               std::size_t parameter_count,
                               std::size_t inner_size,
                               bool asymmetric) noexcept
        : quant_min_(quant_min), quant_max_(quant_max),
          parameter_count_(parameter_count), inner_size_(inner_size),
          asymmetric_(asymmetric)
    {
    }

    int quant_min_;
    int quant_max_;
    std::size_t parameter_count_;
    std::size_t inner_size_;
    bool asymmetric_;
};

/** CMSIS-NN APIs use offset = -zero_point. */
inline int32_t cmsis_nn_offset(float zero_point) noexcept
{
    return -static_cast<int32_t>(std::nearbyint(zero_point));
}

} // namespace autodiff
} // namespace arm_cmsis_dsp
