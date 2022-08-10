/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_dot_prod_f64.c
 * Description:  Floating-point dot product
 *
 * $Date:        03 June 2022
 * $Revision:    V1.10.1
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicDotProd
  @{
 */

/**
  @brief         Dot product of floating-point vectors.
  @param[in]     pSrcA      points to the first input vector.
  @param[in]     pSrcB      points to the second input vector.
  @param[in]     blockSize  number of samples in each vector.
  @param[out]    result     output result returned here.
  @return        none
 */
#if defined(ARM_MATH_NEON) && defined(__aarch64__)
void arm_dot_prod_f64(
    const float64_t * pSrcA,
    const float64_t * pSrcB,
    uint32_t blockSize,
    float64_t * result)
{
    uint32_t blkCnt;                               /* Loop counter */
    float64_t sum = 0.;                            /* Temporary return variable */
    /* Neon Buffer Initialisation */
    float64x2_t sumV = vdupq_n_f64(0.0);                              /* Neon buffer for sum variable */
    

    /* Neon Buffer for sources */
    float64x2_t pSrcAV;
    float64x2_t pSrcBV;
    
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize >> 1U;
    
    while (blkCnt > 0U)
    {
        /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */
        
        /* Load source value in Neon Buffer */
        pSrcAV = vld1q_f64(pSrcA);
        pSrcBV = vld1q_f64(pSrcB);
        /* Calculate dot product and store result in a temporary buffer. */
        sumV = vmlaq_f64(sumV, pSrcAV, pSrcBV);
        
        pSrcA+=2;
        pSrcB+=2;
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Sum both 64 bits part in the float64x2 */
    sum = vaddvq_f64(sumV);
    
    
    /* Tail */
    blkCnt = blockSize & 1 ;
    
    while(blkCnt > 0U)
    {
        sum += (*pSrcA++) * (*pSrcB++);
        
        /* Decrement loop counter */
        blkCnt--;
    }
    
    /* Store result in destination buffer */
    *result = sum;
}
#else
void arm_dot_prod_f64(
    const float64_t * pSrcA,
    const float64_t * pSrcB,
    uint32_t blockSize,
    float64_t * result)
{
    uint32_t blkCnt;                               /* Loop counter */
    float64_t sum = 0.;                            /* Temporary return variable */
    
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;
    
    while (blkCnt > 0U)
    {
        /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */
        
        /* Calculate dot product and store result in a temporary buffer. */
        sum += (*pSrcA++) * (*pSrcB++);
        
        /* Decrement loop counter */
        blkCnt--;
    }
    
    /* Store result in destination buffer */
    *result = sum;
}
#endif

/**
  @} end of BasicDotProd group
 */
