
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cityblock_distance_f64.c
 * Description:  Cityblock (Manhattan) distance between two vectors
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
  @addtogroup Manhattan
  @{
 */


/**
 * @brief        Cityblock (Manhattan) distance between two vectors
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */
ARM_DSP_ATTRIBUTE float64_t arm_cityblock_distance_f64(const float64_t *pA,const float64_t *pB, uint32_t blockSize)
{
    float64_t accum,tmpA, tmpB;
    uint32_t blkCnt;
    accum = 0.;
    
#if defined(ARM_MATH_NEON) && defined(__aarch64__)
    float64x2_t tmpAV, tmpBV,accumV , subV;
    accumV = vdupq_n_f64(0.0);
    blkCnt = blockSize >> 1U;
    
    while(blkCnt > 0U)
    {
        tmpAV = vld1q_f64(pA);
        tmpBV = vld1q_f64(pB);
        subV = vabdq_f64(tmpAV, tmpBV);
        accumV = vaddq_f64(accumV, subV);
        pA+=2;
        pB+=2;
        blkCnt--;
    }
    accum = vaddvq_f64(accumV);
    blkCnt = blockSize & 1 ; 
    
#else
    blkCnt = blockSize;
#endif
    while(blkCnt > 0)
    {
        tmpA = *pA++;
        tmpB = *pB++;
        accum  += fabs(tmpA - tmpB);
        
        blkCnt--;
    }
    
    return(accum);
}

/**
 * @} end of Manhattan group
 */
