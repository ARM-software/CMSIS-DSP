/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_accumulate_f64.c
 * Description:  Accumulation value of a floating-point vector
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
#include "arm_compiler_specific.h"


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
#if defined(ARM_MATH_NEON) && defined(__aarch64__) 
ARM_DSP_ATTRIBUTE void arm_accumulate_f64(
                        const float64_t * pSrc,
                        uint32_t blockSize,
                        float64_t * pResult)
{
  uint32_t blkCnt;                               /* Loop counter */
  
  /*Neon buffers*/
  float64x2_t vSum = vdupq_n_f64(0.0);
  float64x2_t afterLoad ;
  
  float64_t sum = 0.;                            /* Temporary result storage */
  
  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize >> 1U;
  
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    
    afterLoad = vld1q_f64(pSrc);
    vSum = vaddq_f64(vSum, afterLoad);
    
    /* Decrement loop counter */
    blkCnt--;
    
    pSrc += 2;
  }
  sum = vaddvq_f64(vSum);
  
  /* Tail */
  blkCnt = blockSize & 1 ;
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;
    
    /* Decrement loop counter */
    blkCnt--;
  }
  
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1])  */
  /* Store result to destination */
  *pResult = sum;
}
#else
ARM_DSP_ATTRIBUTE void arm_accumulate_f64(
                        const float64_t * pSrc,
                        uint32_t blockSize,
                        float64_t *  pResult)
{
  uint32_t blkCnt;                               /* Loop counter */
  float64_t sum = 0.;                            /* Temporary result storage */
  
  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;
    
    /* Decrement loop counter */
    blkCnt--;
  }
  
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1])  */
  /* Store result to destination */
  *pResult = sum;
}

#endif


/**
 @} end of Accumulation group
 */
