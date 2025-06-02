/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mse_f64.c
 * Description:  Double floating point mean square error
 *
 * $Date:        10 August 2022
 * $Revision:    V1.10.1
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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
  @addtogroup MSE
  @{
 */

/**
  @brief         Mean square error between two double floating point vectors.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     blockSize   number of samples in input vector
  @param[out]    pResult      mean square error
 */

ARM_DSP_ATTRIBUTE void arm_mse_f64(
    const float64_t * pSrcA,
    const float64_t * pSrcB,
    uint32_t blockSize,
    float64_t * pResult)

{
    
    uint32_t blkCnt;                               /* Loop counter */
    float64_t inA, inB;
    float64_t sum = 0.0;
#if defined(ARM_MATH_NEON) && defined(__aarch64__) 
    
    float64x2_t inAV , inBV , subV, sumV;
    sumV = vdupq_n_f64(0.0);
    
    blkCnt = blockSize >> 1U ;
    
    while (blkCnt > 0U)
    {
        inAV = vld1q_f64(pSrcA);
        pSrcA+=2;
        inBV = vld1q_f64(pSrcB);
        pSrcB+=2;
        subV = vsubq_f64(inAV, inBV);
        sumV = vmlaq_f64(sumV, subV, subV);
        
        blkCnt--;
        
    }
    sum = vaddvq_f64(sumV);
    blkCnt = (blockSize) & 1;
    
#else
    /* Temporary return variable */
#if defined (ARM_MATH_LOOPUNROLL)
    blkCnt = (blockSize) >> 1;
    
    while (blkCnt > 0U)
    {
        
        
        inA = *pSrcA++;
        inB = *pSrcB++;
        inA = inA - inB;
        sum += inA * inA;
        
        inA = *pSrcA++;
        inB = *pSrcB++;
        inA = inA - inB;
        sum += inA * inA;
        
        /* Decrement loop counter */
        blkCnt--;
    }
    
    
    /* Loop unrolling: Compute remaining outputs */
    blkCnt = (blockSize) & 1;
#else
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;
#endif
#endif

#if defined(__clang__) && defined(ARM_MATH_NEON) && defined(__aarch64__) 
    #pragma clang loop vectorize(enable) unroll(disable)
#endif
    while (blkCnt > 0U)
    {
        inA = *pSrcA++;
        inB = *pSrcB++;
        inA = inA - inB;
        sum += inA * inA;
        
        /* Decrement loop counter */
        blkCnt--;
    }
    
    /* Store result in destination buffer */
    *pResult = sum / blockSize;
}



/**
  @} end of MSE group
 */
