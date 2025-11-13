/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_absmax_no_idx_f64.c
 * Description:  Maximum value of absolute values of a floating-point vector
 *
 * $Date:        10 August 2022
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
#include "arm_compiler_specific.h"


#include "dsp/statistics_functions.h"



/**
  @ingroup groupStats
 */

/**
  @addtogroup AbsMax
  @{
 */

/**
  @brief         Maximum value of absolute values of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
 */

#if defined(ARM_MATH_NEON) && defined(__aarch64__)
ARM_DSP_ATTRIBUTE void arm_absmax_no_idx_f64(
    const float64_t * pSrc,
    uint32_t blockSize,
    float64_t * pResult)
{
    float64_t maxVal , in;                         /* Temporary variables to store the output value. */
    uint32_t blkCnt;                     /* Loop counter */
    
    float64x2_t maxV;
    float64x2_t pSrcV ;
    pSrcV = vld1q_f64(pSrc);
    pSrc += 2 ;
    maxV = vabsq_f64(pSrcV);

    
    
    
    /* Load first input value that act as reference value for comparison */

    
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 2U) >> 1U;
    
    while (blkCnt > 0U)
    {
        /* Initialize maxVal to the next consecutive values one by one */
        pSrcV = vld1q_f64(pSrc);
        maxV = vmaxq_f64(maxV, vabsq_f64(pSrcV));
        
        pSrc += 2 ;
        
        /* Decrement loop counter */
        blkCnt--;
    }
    maxVal =vgetq_lane_f64(maxV, 0);
    if(maxVal < vgetq_lane_f64(maxV, 1))
    {
        maxVal = vgetq_lane_f64(maxV, 1);
    }
    blkCnt = (blockSize - 2U) & 1;
    
    while (blkCnt > 0U)
    {
        /* Initialize maxVal to the next consecutive values one by one */
        in = fabs(*pSrc++);
        
        /* compare for the maximum value */
        if (maxVal < in)
        {
            /* Update the maximum value and it's index */
            maxVal = in;
        }
        
        /* Decrement loop counter */
        blkCnt--;
    }
    *pResult = maxVal;
    
    
    /* Store the maximum value and it's index into destination pointers */

}
#else
ARM_DSP_ATTRIBUTE void arm_absmax_no_idx_f64(
    const float64_t * pSrc,
    uint32_t blockSize,
    float64_t * pResult)
{
    float64_t maxVal, out;                         /* Temporary variables to store the output value. */
    uint32_t blkCnt;                     /* Loop counter */
    
    
    
    
    
    /* Load first input value that act as reference value for comparison */
    out = fabs(*pSrc++);
    
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);
    
    while (blkCnt > 0U)
    {
        /* Initialize maxVal to the next consecutive values one by one */
        maxVal = fabs(*pSrc++);
        
        /* compare for the maximum value */
        if (out < maxVal)
        {
            /* Update the maximum value and it's index */
            out = maxVal;
        }
        
        /* Decrement loop counter */
        blkCnt--;
    }
    
    /* Store the maximum value and it's index into destination pointers */
    *pResult = out;
}
#endif

/**
  @} end of AbsMax group
 */
