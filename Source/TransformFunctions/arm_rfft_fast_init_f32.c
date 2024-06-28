/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_fast_init_f32.c
 * Description:  Split Radix Decimation in Frequency CFFT Floating point processing function
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/transform_functions.h"
#include "arm_common_tables.h"

/**
  @ingroup RealFFT
 */

/**
  @addtogroup RealFFTF32
  @{
 */


/**
  @brief         Initialization function for the 32pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_32_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_16_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }

  S->fftLenRFFT = 32U;
  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_32;

  return ARM_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 64pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_64_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_32_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 64U;

  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_64;

  return ARM_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 128pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_128_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_64_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 128;

  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_128;

  return ARM_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 256pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
*/

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_256_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_128_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 256U;

  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_256;

  return ARM_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 512pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_512_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_256_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 512U;

  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_512;

  return ARM_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 1024pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_1024_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_512_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 1024U;

  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_1024;

  return ARM_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 2048pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */
ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_2048_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_1024_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 2048U;

  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_2048;

  return ARM_MATH_SUCCESS;
}

/**
* @brief         Initialization function for the 4096pt floating-point real FFT.
* @param[in,out] S  points to an arm_rfft_fast_instance_f32 structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_4096_f32( arm_rfft_fast_instance_f32 * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_2048_f32(&(S->Sint));
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 4096U;

  S->pTwiddleRFFT    = (float32_t *) twiddleCoef_rfft_4096;

  return ARM_MATH_SUCCESS;
}

/**
  @brief         Generic initialization function for the floating-point real FFT.
  @param[in,out] S       points to an arm_rfft_fast_instance_f32 structure
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

ARM_DSP_ATTRIBUTE arm_status arm_rfft_fast_init_f32(
  arm_rfft_fast_instance_f32 * S,
  uint16_t fftLen)
{
  arm_status status;


  switch (fftLen)
  {
  case 4096U:
    status = arm_rfft_fast_init_4096_f32(S);
    break;
  case 2048U:
    status = arm_rfft_fast_init_2048_f32(S);
    break;
  case 1024U:
    status = arm_rfft_fast_init_1024_f32(S);
    break;
  case 512U:
    status = arm_rfft_fast_init_512_f32(S);
    break;
  case 256U:
    status = arm_rfft_fast_init_256_f32(S);
    break;
  case 128U:
    status = arm_rfft_fast_init_128_f32(S);
    break;
  case 64U:
    status = arm_rfft_fast_init_64_f32(S);
    break;
  case 32U:
    status = arm_rfft_fast_init_32_f32(S);
    break;
  default:
    return(ARM_MATH_ARGUMENT_ERROR);
    break;
  }

  return(status);

}

/**
  @} end of RealFFTF32 group
 */
