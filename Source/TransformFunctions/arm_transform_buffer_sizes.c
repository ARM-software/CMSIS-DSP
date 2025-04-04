#include "dsp/transform_functions.h"

/** 
 * @brief Calculate required length in samples for the temporary buffer
 * @param dt Data type of the input data
 * @param nb_samples Number of samples in the input data
 * @param buf_id Identification for the temporary buffer
 * @return Length in samples (real numbers) for the temporary buffer
 */
uint32_t arm_cfft_tmp_buffer_size(arm_math_datatype dt,
                              uint32_t nb_samples,
                              uint32_t buf_id)
{
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
    if (buf_id != 1)
    {
        return 0;
    }
    switch(dt)
    {
        case ARM_MATH_F16:
            /* Return values in float hence factor 2 */
            return(nb_samples * 2U);
        case ARM_MATH_F32:
            /* Return values in float hence factor 2 */
            return(nb_samples * 2U);
        case ARM_MATH_Q31:
            /* Return values in float hence factor 2 */
            return(nb_samples * 2U);
        case ARM_MATH_Q15:
            /* Return values in float hence factor 2 */
            return(nb_samples * 2U);
        default:
            return(0);
    }
#else
    return(0);
#endif
}

/** 
 * @brief Calculate required length in samples for the output buffer
 * @param dt Data type of the input data
 * @param nb_samples Number of samples in the input data
 * @return Length in samples (real numbers) for the output buffer
 */
uint32_t arm_cfft_output_buffer_size(arm_math_datatype dt,
    uint32_t nb_samples)
{
    switch(dt)
    {
        default:
            return(nb_samples * 2U);
    }
}

/** 
 * @brief Calculate required length in samples for the output buffer
 * @param dt Data type of the input data
 * @param nb_samples Number of samples in the input data
 * @return Length in samples (real numbers) for the output buffer
 */
uint32_t arm_cifft_output_buffer_size(arm_math_datatype dt,
    uint32_t nb_samples)
{
    switch(dt)
    {
        default:
            return(nb_samples * 2U);
    }
}

/** 
 * @brief Calculate required length in samples for the temporary buffer
 * @param dt Data type of the input data
 * @param nb_samples Number of samples in the input data
 * @param buf_id Identification for the temporary buffer
 * @return Length in samples (real numbers) for the temporary buffer
 */
uint32_t arm_rfft_tmp_buffer_size(arm_math_datatype dt,
    uint32_t nb_samples,
    uint32_t buf_id)
{
    switch(dt)
    {
        case ARM_MATH_F32:
        case ARM_MATH_F16:
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
            if (buf_id != 1)
            {
                return 0;
            }
            return(nb_samples);
#else
            return 0;
#endif
        break;
        case ARM_MATH_Q31:
        case ARM_MATH_Q15:
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
        if (buf_id != 1)
        {
            return 0;
        }
        return(2*nb_samples);
#else
        return 0;
#endif
        break;
        default:
            return(0);
    }
    return(0);
}

/** 
 * @brief Calculate required length in samples for the output buffer
 * @param dt Data type of the input data
 * @param nb_samples Number of samples in the input data
 * @param buf_id Identification for the temporary buffer
 * @return Length in samples (real numbers) for the output buffer
 */
uint32_t arm_rfft_output_buffer_size(arm_math_datatype dt,
    uint32_t nb_samples)
{
    switch(dt)
    {
        case ARM_MATH_F32:
        case ARM_MATH_F16:
           return(nb_samples);
        break;
        case ARM_MATH_Q31:
        case ARM_MATH_Q15:
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
        return(nb_samples+2);
#elif defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
        return(nb_samples+2);
#else
        return(2*nb_samples);
#endif
        break;
        default:
           return 0;
    }
}

/** 
 * @brief Calculate required length in samples for the input buffer
 * @param dt Data type of the input data
 * @param nb_samples RFFT length in samples
 * @param buf_id Identification for the temporary buffer
 * @return Length in samples (real numbers) for the input buffer
 */
uint32_t arm_rifft_input_buffer_size(arm_math_datatype dt,
    uint32_t nb_samples)
{
    return(arm_rfft_output_buffer_size(dt, nb_samples));
}

/** 
 * @brief Calculate required length in samples for the temporary buffer
 * @param dt Data type of the input data
 * @param nb_samples Number of samples in the input data
 * @param buf_id Identification for the temporary buffer
 * @return Length in samples (real numbers) for the temporary buffer
 */
uint32_t arm_mfcc_tmp_buffer_size(arm_math_datatype dt,
    uint32_t nb_samples,
    uint32_t buf_id)
{
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
 if ((buf_id != 1) && (buf_id != 2))
 {
     return 0;
 }
#else
 if (buf_id != 1)
 {
     return 0;
 }
#endif

 switch(buf_id)
 {
        case 1:
#if defined(ARM_MFCC_USE_CFFT)
            return(arm_cfft_output_buffer_size(dt, nb_samples));
#else
            return(arm_rfft_output_buffer_size(dt, nb_samples));
#endif
        case 2:
            return(arm_rfft_tmp_buffer_size(dt, nb_samples, 1));
        default:
            return(0);
 }


    return(0);
}