/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_abs_f16.c
 * Description:  Floating-point vector absolute value
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

#include "dsp/basic_math_functions_f16.h"
#include <math.h>

/**
  @ingroup groupMath
 */


/**
  @addtogroup BasicAbs
  @{
 */

/**
  @brief         Floating-point vector absolute value.
  @param[in]     pSrc       points to the input vector
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
 */


#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

ARM_DSP_ATTRIBUTE void arm_abs_f16(
  const float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
    uint32_t blkCnt;                               /* Loop counter */
    f16x8_t vec1;
    f16x8_t res;


    /* Compute 4 outputs at a time */
    blkCnt = blockSize >> 3U;

    while (blkCnt > 0U)
    {
        /* C = |A| */

        /* Calculate absolute values and then store the results in the destination buffer. */
        vec1 = vld1q(pSrc);
        res = vabsq(vec1);
        vst1q(pDst, res);

        /* Increment pointers */
        pSrc += 8;
        pDst += 8;
        
        /* Decrement the loop counter */
        blkCnt--;
    }

    /* Tail */
    blkCnt = blockSize & 0x7;


    if (blkCnt > 0U)
    {
      /* C = |A| */
      mve_pred16_t p0 = vctp16q(blkCnt);
      vec1 = vld1q(pSrc);
      vstrhq_p(pDst, vabsq(vec1), p0);
    }

}

#else
#if defined(ARM_FLOAT16_SUPPORTED)
ARM_DSP_ATTRIBUTE void arm_abs_f16(
  const float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */

#if defined(ARM_MATH_NEON_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)
    f16x8_t vec1;
    f16x8_t res;

    /* Compute 4 outputs at a time */
    blkCnt = blockSize >> 2U;

    while (blkCnt > 0U)
    {
        /* C = |A| */

    	/* Calculate absolute values and then store the results in the destination buffer. */
        vec1 = vld1q_f16(pSrc);
        res = vabsq_f16(vec1);
        vst1q_f16(pDst, res);

        /* Increment pointers */
        pSrc += 4;
        pDst += 4;
        
        /* Decrement the loop counter */
        blkCnt--;
    }

    /* Tail */
    blkCnt = blockSize & 0x3;

#else
#if defined (ARM_MATH_LOOPUNROLL) && !defined(ARM_MATH_AUTOVECTORIZE)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute and store result in destination buffer. */
    *pDst++ = (_Float16)fabsf((float32_t)*pSrc++);

    *pDst++ = (_Float16)fabsf((float32_t)*pSrc++);

    *pDst++ = (_Float16)fabsf((float32_t)*pSrc++);

    *pDst++ = (_Float16)fabsf((float32_t)*pSrc++);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize % 0x4U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */
#endif /* #if defined(ARM_MATH_NEON) */

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute and store result in destination buffer. */
    *pDst++ = (_Float16)fabsf((float32_t)*pSrc++);

    /* Decrement loop counter */
    blkCnt--;
  }

}
#endif /* defined(ARM_FLOAT16_SUPPORTED */
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */
/**
  @} end of BasicAbs group
 */
