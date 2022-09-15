
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_chebyshev_distance_f64.c
 * Description:  Chebyshev distance between two vectors
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

#include "dsp/distance_functions.h"
#include <limits.h>
#include <math.h>


/**
  @addtogroup Chebyshev
  @{
 */


/**
 * @brief        Chebyshev distance between two vectors
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */
float64_t arm_chebyshev_distance_f64(const float64_t *pA,const float64_t *pB, uint32_t blockSize)
{
    
    float64_t diff=0.,  maxVal,tmpA, tmpB;
    uint32_t blkCnt;
    maxVal = F64_MIN;
#if defined(ARM_MATH_NEON) && defined(__aarch64__)
    float64x2_t diffV , tmpAV , tmpBV , maxValV ;
    maxValV = vdupq_n_f64(maxVal);
    blkCnt = blockSize >> 1U ;
    while(blkCnt > 0U)
    {
        tmpAV = vld1q_f64(pA);
        tmpBV = vld1q_f64(pB);
        diffV = vabsq_f64((vsubq_f64(tmpAV, tmpBV)));
        maxValV = vmaxq_f64(maxValV, diffV);
        pA+=2;
        pB+=2;
        blkCnt--;
    }
    maxVal =vgetq_lane_f64(maxValV, 0);
    if(maxVal < vgetq_lane_f64(maxValV, 1))
    {
        maxVal = vgetq_lane_f64(maxValV, 1);
    }
    blkCnt = blockSize & 1;
    
    
#else
    blkCnt = blockSize;
#endif
    
    while(blkCnt > 0)
    {
        tmpA = *pA++;
        tmpB = *pB++;
        diff = fabs(tmpA - tmpB);
        if (diff > maxVal)
        {
            maxVal = diff;
        }
        blkCnt --;
    }
    
    return(maxVal);
}

/**
 * @} end of Chebyshev group
 */
