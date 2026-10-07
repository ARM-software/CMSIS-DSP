/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_mult_real_f64.c
 * Description:  Floating-point complex by real multiplication
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */

/*
 * Copyright (C) 2010-2026 ARM Limited or its affiliates. All rights reserved.
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


#include "dsp/complex_math_functions.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @addtogroup CmplxByRealMult
  @{
 */

/**
  @brief         Floating-point complex-by-real multiplication.
  @param[in]     pSrcCmplx   points to complex input vector
  @param[in]     pSrcReal    points to real input vector
  @param[out]    pCmplxDst   points to complex output vector
  @param[in]     numSamples  number of samples in each vector
 */

ARM_DSP_ATTRIBUTE void arm_cmplx_mult_real_f64(
  const float64_t * pSrcCmplx,
  const float64_t * pSrcReal,
        float64_t * pCmplxDst,
        uint32_t numSamples)
{
  uint32_t blkCnt;                               /* Loop counter */
  float64_t in;                                  /* Temporary variable */

  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i    ] * B[i]. */
    /* C[2 * i + 1] = A[2 * i + 1] * B[i]. */

    in = *pSrcReal++;
    /* store result in destination buffer. */
    *pCmplxDst++ = *pSrcCmplx++ * in;
    *pCmplxDst++ = *pSrcCmplx++ * in;

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of CmplxByRealMult group
 */
