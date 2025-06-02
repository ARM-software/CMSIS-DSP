/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_accumulate_f32.c
 * Description:  Sum value of a floating-point vector
 *
 * $Date:        14 July 2022
 * $Revision:    V1.0.0
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

#include "dsp/statistics_functions.h"

/**
 @ingroup groupStats
 */


/**
 @addtogroup Accumulation
 @{
 */

/**
 @brief         Accumulation value of a floating-point vector.
 @param[in]     pSrc       points to the input vector.
 @param[in]     blockSize  number of samples in input vector.
 @param[out]    pResult    sum of values in input vector.
 */

#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

ARM_DSP_ATTRIBUTE void arm_accumulate_f32(
                        const float32_t * pSrc,
                        uint32_t blockSize,
                        float32_t * pResult)
{
    f32x4_t vecA;
    f32x4_t vecSum;
    uint32_t blkCnt; 
    float32_t sum = 0.0f;  
    vecSum = vdupq_n_f32(0.0f);

    /* Compute 4 outputs at a time */
    blkCnt = blockSize >> 2U;
    while (blkCnt > 0U)
    {
        /*
         * C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1]
         * Calculate dot product and then store the result in a temporary buffer.
         * and advance vector source and destination pointers
         */
        vecA = vld1q_f32(pSrc);
        pSrc += 4;
        
        vecSum = vaddq_f32(vecSum, vecA);
        /*
         * Decrement the blockSize loop counter
         */
        blkCnt --;
    }


    blkCnt = blockSize & 3;
    if (blkCnt > 0U)
    {
        /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */

        mve_pred16_t p0 = vctp32q(blkCnt);
        vecA = vld1q(pSrc);
        vecSum = vaddq_m(vecSum,vecSum, vecA, p0);
    }

    sum = vecAddAcrossF32Mve(vecSum);

    /* Store result in destination buffer */
    *pResult = sum;
}

#else

#if defined(ARM_MATH_NEON)
ARM_DSP_ATTRIBUTE void arm_accumulate_f32(
                        const float32_t * pSrc,
                        uint32_t blockSize,
                        float32_t * pResult)
{
  float32_t sum = 0.0f;                          /* Temporary result storage */
  float32x4_t sumV = vdupq_n_f32(0.0f);                          /* Temporary result storage */
  float32x2_t sumV2;
  
  uint32_t blkCnt;                               /* Loop counter */
  
  float32x4_t inV;
  
  blkCnt = blockSize >> 2U;
  
  /* Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    inV = vld1q_f32(pSrc);
    sumV = vaddq_f32(sumV, inV);
    
    pSrc += 4;
    /* Decrement the loop counter */
    blkCnt--;
  }
  
  sumV2 = vpadd_f32(vget_low_f32(sumV),vget_high_f32(sumV));
  sum = vget_lane_f32(sumV2, 0) + vget_lane_f32(sumV2, 1);
  
  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = blockSize & 3;
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;
    
    /* Decrement the loop counter */
    blkCnt--;
  }
  
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1])  */
  /* Store the result to the destination */
  *pResult = sum;
}

#else
ARM_DSP_ATTRIBUTE void arm_accumulate_f32(
                        const float32_t * pSrc,
                        uint32_t blockSize,
                        float32_t * pResult)
{
  uint32_t blkCnt;                               /* Loop counter */
  float32_t sum = 0.0f;                          /* Temporary result storage */
  
#if defined (ARM_MATH_LOOPUNROLL) && !defined(ARM_MATH_AUTOVECTORIZE)
  
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;
    
    sum += *pSrc++;
    
    sum += *pSrc++;
    
    sum += *pSrc++;
    
    /* Decrement the loop counter */
    blkCnt--;
  }
  
  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize % 0x4U;
  
#else
  
  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;
  
#endif /* #if defined (ARM_MATH_LOOPUNROLL) */
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;
    
    /* Decrement loop counter */
    blkCnt--;
  }
  
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1])  */
  /* Store result to destination */
  *pResult = sum ;
}
#endif /* #if defined(ARM_MATH_NEON) */

#endif /* #if defined(ARM_MATH_MVEF) */
/**
 @} end of Accumulation group
 */
