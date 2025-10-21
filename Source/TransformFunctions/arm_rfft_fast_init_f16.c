/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_fast_init_f16.c
 * Description:  Split Radix Decimation in Frequency CFFT Floating point processing function
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */

/*
 * Copyright (C) 2010-2023 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "arm_compiler_specific.h"


#include "dsp/transform_functions_f16.h"
#include "arm_common_tables_f16.h"
#include "arm_const_structs_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)

/**
  @ingroup RealFFT
 */

/**
  @addtogroup RealFFTF16
  @{
 */


/**
  @brief         Initialization function for the 32pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

#if defined(ARM_MATH_NEON_FLOAT16)
#include "arm_neon_tables_f16.h"
#define RFFT_INIT(LEN)                                                                              \
  S->nfft = LEN;                                                                                    \
  S->r_twiddles = arm_neon_rfft_twiddles_##LEN##_f16;                                               \
  S->r_factors = arm_neon_rfft_factors_##LEN##_f16;                                                 \
                                                                                                    \
  S->r_twiddles_neon = arm_neon_rfft_twiddles_neon_##LEN##_f16;                                     \
  S->r_factors_neon = arm_neon_rfft_factors_neon_##LEN##_f16;                                       \
                                                                                                    \
  S->r_twiddles_backward = S->r_twiddles + 2*ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_##LEN##_F16;               \
  S->r_twiddles_neon_backward = S->r_twiddles_neon + 2*ARM_NE10_OFFSET_BACKWARD_TWID_NEON_##LEN##_F16;\
                                                                                                    \
  S->r_super_twiddles_neon = arm_neon_rfft_super_twiddles_neon_##LEN##_f16;

#define FAST_INIT_FUNC(LEN)                                                                  \
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_##LEN##_f16( arm_rfft_fast_instance_f16 * S )\
{                                                                                            \
                                                                                             \
  if( !S ) return ARM_MATH_ARGUMENT_ERROR;                                                   \
                                                                                             \
  RFFT_INIT(LEN);                                                                            \
                                                                                             \
  return ARM_MATH_SUCCESS;                                                                   \
}

#endif 

#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(32)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_32_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_16_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }

  S->fftLenRFFT = 32U;
  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_32;

  return ARM_MATH_SUCCESS;
}
#endif 

/**
  @brief         Initialization function for the 64pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(64)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_64_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_32_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 64U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_64;

  return ARM_MATH_SUCCESS;
}
#endif 

/**
  @brief         Initialization function for the 128pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(128)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_128_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_64_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 128;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_128;

  return ARM_MATH_SUCCESS;
}
#endif 

/**
  @brief         Initialization function for the 256pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
*/
#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(256)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_256_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_128_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 256U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_256;

  return ARM_MATH_SUCCESS;
}
#endif
/**
  @brief         Initialization function for the 512pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(512)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_512_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_256_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 512U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_512;

  return ARM_MATH_SUCCESS;
}
#endif 

/**
  @brief         Initialization function for the 1024pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(1024)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_1024_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_512_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 1024U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_1024;

  return ARM_MATH_SUCCESS;
}
#endif
/**
  @brief         Initialization function for the 2048pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */
#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(2048)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_2048_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_1024_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 2048U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_2048;

  return ARM_MATH_SUCCESS;
}
#endif
/**
* @brief         Initialization function for the 4096pt floating-point real FFT.
* @param[in,out] S  points to an arm_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */
#if defined(ARM_MATH_NEON_FLOAT16) 

FAST_INIT_FUNC(4096)

#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_4096_f16( arm_rfft_fast_instance_f16 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_2048_f16(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 4096U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_4096;

  return ARM_MATH_SUCCESS;
}
#endif 

/**
  @brief         Generic initialization function for the floating-point real FFT.
  @param[in,out] S       points to an arm_rfft_fast_instance_f16 structure
  @param[in]     fftLen  length of the Real Sequence
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLen</code> is not a supported length

  @par           Description
                   The parameter <code>fftLen</code> specifies the length of RFFT/CIFFT process.
                   Supported FFT Lengths are 32, 64, 128, 256, 512, 1024, 2048, 4096.

  @par
                This Function also initializes Twiddle factor table pointer and Bit reversal table pointer.

  @par
                This function should be used only if you don't know the FFT sizes that 
                you'll need at build time. The use of this function will prevent the 
                linker from removing the FFT tables that are not needed and the library 
                code size will be bigger than needed.

  @par
                If you use CMSIS-DSP as a static library, and if you know the FFT sizes 
                that you need at build time, then it is better to use the initialization
                functions defined for each FFT size.

 */

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_f16(
  arm_rfft_fast_instance_f16 * S,
  uint16_t fftLen)
{
  arm_status status;


  switch (fftLen)
  {
  case 4096U:
    status = arm_rfft_fast_init_4096_f16(S);
    break;
  case 2048U:
    status = arm_rfft_fast_init_2048_f16(S);
    break;
  case 1024U:
    status = arm_rfft_fast_init_1024_f16(S);
    break;
  case 512U:
    status = arm_rfft_fast_init_512_f16(S);
    break;
  case 256U:
    status = arm_rfft_fast_init_256_f16(S);
    break;
  case 128U:
    status = arm_rfft_fast_init_128_f16(S);
    break;
  case 64U:
    status = arm_rfft_fast_init_64_f16(S);
    break;
  case 32U:
    status = arm_rfft_fast_init_32_f16(S);
    break;
  default:
    return(ARM_MATH_ARGUMENT_ERROR);
    break;
  }

  return(status);

}

/**
  @} end of RealFFTF16 group
 */

 #if defined(RFFT_INIT)
 #undef RFFT_INIT
 #endif

 #if defined(FAST_INIT_FUNC)
 #undef FAST_INIT_FUNC
 #endif
#endif /*  #if defined(ARM_FLOAT16_SUPPORTED) */
