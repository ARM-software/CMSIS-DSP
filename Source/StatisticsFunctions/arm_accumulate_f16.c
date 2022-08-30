/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_accumulate_f16.c
 * Description:  accumulation value of a floating-point vector
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

#include "dsp/statistics_functions_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)


/**
 @ingroup groupStats
 */

/**
 @defgroup Accumulation Accumulation functions
 
 Calculates the accumulation of the input vector. Sum is defined as the addition of the elements in the vector.
 The underlying algorithm is used:
 
 <pre>
 Result = (pSrc[0] + pSrc[1] + pSrc[2] + ... + pSrc[blockSize-1]);
 </pre>
 
 There are separate functions for floating-point, Q31, Q15, and Q7 data types.
 */

/**
 @addtogroup Accumulation
 @{
 */

/**
 @brief         accumulate value of a floating-point vector.
 @param[in]     pSrc       points to the input vector.
 @param[in]     blockSize  number of samples in input vector.
 @param[out]    pResult    sum of values in input vector.
 @return        none
 */

void arm_accumulate_f16(
                        const float16_t * pSrc,
                        uint32_t blockSize,
                        float16_t * pResult)
{
  uint32_t blkCnt;                               /* Loop counter */
  float16_t sum = 0.0f16;                          /* Temporary result storage */
  
#if defined (ARM_MATH_LOOPUNROLL) && !defined(ARM_MATH_AUTOVECTORIZE)
  
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += (_Float16)*pSrc++;
    
    sum += (_Float16)*pSrc++;
    
    sum += (_Float16)*pSrc++;
    
    sum += (_Float16)*pSrc++;
    
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
    sum += (_Float16)*pSrc++;
    
    /* Decrement loop counter */
    blkCnt--;
  }
  
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
  /* Store result to destination */
  *pResult = sum ;
}
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
 @} end of Accumulation group
 */


