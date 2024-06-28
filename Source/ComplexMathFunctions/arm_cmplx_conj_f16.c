/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_conj_f16.c
 * Description:  Floating-point complex conjugate
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

#include "dsp/complex_math_functions_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)
/**
  @ingroup groupCmplxMath
 */


/**
  @addtogroup cmplx_conj
  @{
 */

/**
  @brief         Floating-point complex conjugate.
  @param[in]     pSrc        points to the input vector
  @param[out]    pDst        points to the output vector
  @param[in]     numSamples  number of samples in each vector
 */

#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

ARM_DSP_ATTRIBUTE void arm_cmplx_conj_f16(
    const float16_t * pSrc,
    float16_t * pDst,
    uint32_t numSamples)
{
    static const float16_t cmplx_conj_sign[8] = { 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f };
    uint32_t blockSize = numSamples * CMPLX_DIM;   /* loop counters */
    uint32_t blkCnt;
    f16x8_t vecSrc;
    f16x8_t vecSign;

    /*
     * load sign vector
     */
    vecSign = *(f16x8_t *) cmplx_conj_sign;

    /* Compute 4 real samples at a time */
    blkCnt = blockSize >> 3U;

    while (blkCnt > 0U)
    {
        vecSrc = vld1q(pSrc);
        vst1q(pDst,vmulq(vecSrc, vecSign));
        /*
         * Decrement the blkCnt loop counter
         * Advance vector source and destination pointers
         */
        pSrc += 8;
        pDst += 8;
        blkCnt--;
    }

     /* Tail */
    blkCnt = (blockSize & 0x7) >> 1;

    while (blkCnt > 0U)
    {
      /* C[0] + jC[1] = A[0]+ j(-1)A[1] */
  
      /* Calculate Complex Conjugate and store result in destination buffer. */
      *pDst++ =  *pSrc++;
      *pDst++ = -(_Float16)*pSrc++;
  
      /* Decrement loop counter */
      blkCnt--;
    }

}

#else
ARM_DSP_ATTRIBUTE void arm_cmplx_conj_f16(
  const float16_t * pSrc,
        float16_t * pDst,
        uint32_t numSamples)
{
        uint32_t blkCnt;                               /* Loop counter */

#if defined (ARM_MATH_LOOPUNROLL) && !defined(ARM_MATH_AUTOVECTORIZE)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] + jC[1] = A[0]+ j(-1)A[1] */

    /* Calculate Complex Conjugate and store result in destination buffer. */
    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = numSamples % 0x4U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C[0] + jC[1] = A[0]+ j(-1)A[1] */

    /* Calculate Complex Conjugate and store result in destination buffer. */
    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    /* Decrement loop counter */
    blkCnt--;
  }

}
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
  @} end of cmplx_conj group
 */
#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */
