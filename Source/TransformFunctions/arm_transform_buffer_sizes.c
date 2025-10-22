#include "dsp/transform_functions.h"

/**
  @ingroup groupTransforms
*/

/**
  @defgroup TransformBuffers Transform Buffer Sizes
  @par
        Functions to get the sizes of the buffers in the different transform functions
        according to the datatype and target architecture.

  @par
        The current architecture for the build is defined in `ARM_MATH_DEFAULT_TARGET_ARCH`

  @note All returned lengths are in real numbers and never in complex. So, for a complex buffer of S samples, the 
        returned length will be 2*S.

*/

/**
  @ingroup TransformBuffers
*/

/**
  @defgroup CFFTBuffers Buffer Sizes for CFFT
*/



/**
  @addtogroup CFFTBuffers
  @{
*/

/** 
 * @brief Calculate required length for the temporary buffer
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples
 * @param[in] buf_id Identification for the temporary buffer
 * @return Length in datatype elements (real numbers) for the temporary buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t arm_cfft_tmp_buffer_size(arm_math_target_arch arch,arm_math_datatype dt,
                              uint32_t nb_samples,
                              uint32_t buf_id)
{
    if (arch == ARM_MATH_NEON_ARCH)
    {
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
    }
    else
    {
        return(0);
    }
}

/** 
 * @brief Calculate required length for the output buffer
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @return Length in datatype elements (real numbers) for the output buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t arm_cfft_output_buffer_size(arm_math_target_arch arch,arm_math_datatype dt,
    uint32_t nb_samples)
{
    (void)arch;
    switch(dt)
    {
        default:
            return(nb_samples * 2U);
    }
}

/** 
 * @brief Calculate required length for the output buffer
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @return Length in datatype elements (real numbers) for the output buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t arm_cifft_output_buffer_size(arm_math_target_arch arch,arm_math_datatype dt,
    uint32_t nb_samples)
{
    (void)arch;
    switch(dt)
    {
        default:
            return(nb_samples * 2U);
    }
}

/**
* @} end of CFFTBuffers group
*/

/**
  @ingroup TransformBuffers
*/

/**
  @defgroup RFFTBuffers Buffer Sizes for RFFT
*/


/**
  @addtogroup RFFTBuffers
  @{
*/

/** 
 * @brief Calculate required length for the temporary buffer for both RFFT and RIFFT
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @param[in] buf_id Identification for the temporary buffer
 * @return Length in datatype elements (real numbers) for the temporary buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t arm_rfft_tmp_buffer_size(arm_math_target_arch arch,arm_math_datatype dt,
    uint32_t nb_samples,
    uint32_t buf_id)
{
    switch(dt)
    {
        case ARM_MATH_F32:
        case ARM_MATH_F16:
            if (arch == ARM_MATH_NEON_ARCH)
            {
                        if (buf_id != 1)
                        {
                            return 0;
                        }
                        return(nb_samples);
            }
            else
            {
                        return 0;
            }
        break;
        case ARM_MATH_Q31:
        case ARM_MATH_Q15:
            if (arch == ARM_MATH_NEON_ARCH)
            {
                    if (buf_id != 1)
                    {
                        return 0;
                    }
                    return(2*nb_samples);
            }
            else
            {
                    return 0;
            }
        break;
        default:
            return(0);
    }
}

/** 
 * @brief Calculate required length for the output buffer
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @return Length in datatype elements (real numbers) for the output buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t arm_rfft_output_buffer_size(arm_math_target_arch arch,arm_math_datatype dt,
    uint32_t nb_samples)
{
    switch(dt)
    {
        case ARM_MATH_F64:
        case ARM_MATH_F32:
        case ARM_MATH_F16:
           return(nb_samples);
        break;
        case ARM_MATH_Q31:
        case ARM_MATH_Q15:
            if (arch == ARM_MATH_NEON_ARCH)
            {
                        return(nb_samples+2);
            }
            else if (arch == ARM_MATH_HELIUM_ARCH)
            {
                        return(nb_samples+2);
            }
            else
            {
                return(2*nb_samples);
            }

        break;
        default:
           return 0;
    }
}

/** 
 * @brief Calculate required length for the input buffer
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples RFFT length in samples
 * @return Length in datatype elements (real numbers) for the input buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t arm_rifft_input_buffer_size(arm_math_target_arch arch,arm_math_datatype dt,
    uint32_t nb_samples)
{
    (void)arch;
    switch(dt)
    {
        case ARM_MATH_F64:
        case ARM_MATH_F32:
        case ARM_MATH_F16:
           return(nb_samples);
        break;
        case ARM_MATH_Q31:
        case ARM_MATH_Q15:
            return(nb_samples+2);
        break;
        default:
           return 0;
    }
}

/**
* @} end of RFFTBuffers group
*/

/**
  @ingroup TransformBuffers
*/

/**
  @defgroup MFCCBuffers Buffer Sizes for MFCC
*/


/**
  @addtogroup MFCCBuffers
  @{
*/

/** 
 * @brief Calculate required length for the temporary buffer
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @param[in] buf_id Identification for the temporary buffer
 * @param[in] use_cfft 1 if implementastion uses CFFT, 0 if RFFT
 * @return Length in datatype elements (real numbers) for the temporary buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 * @note The define `ARM_MFCC_USE_CFFT` can be checked to know the current MFCC mode (CFFT or RFFT) for the build
 */
int32_t arm_mfcc_tmp_buffer_size(arm_math_target_arch arch,
    arm_math_datatype dt,
    uint32_t nb_samples,
    uint32_t buf_id,
    uint32_t use_cfft)
{
    if (arch == ARM_MATH_NEON_ARCH)
    {
        if (use_cfft==1)
        {
          // Neon implementation is forcing the RFFT mode
          // So an error is returned for this configuration 
           return(-1);
        }

        if ((buf_id != 1) && (buf_id != 2))
        {
            return 0;
        }
    }
    else
    { 
        if (buf_id != 1)
        {
            return 0;
        }
    }


    switch(buf_id)
    {
           case 1:
              if (use_cfft == 1)
              { 
                return(arm_cfft_output_buffer_size(arch,dt, nb_samples));
              }
              else
              {
                return(arm_rfft_output_buffer_size(arch,dt, nb_samples));
              }
           case 2:
               return(arm_rfft_tmp_buffer_size(arch,dt, nb_samples, 1));
           default:
               return(0);
    }


}

/**
* @} end of MFCCBuffers group
*/