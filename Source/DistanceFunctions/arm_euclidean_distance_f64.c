

/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_euclidean_distance_f64.c
 * Description:  Euclidean distance between two vectors
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


#include "dsp/distance_functions.h"
#include <limits.h>
#include <math.h>



/**
  @addtogroup Euclidean
  @{
 */


/**
 * @brief        Euclidean distance between two vectors
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */
ARM_DSP_ATTRIBUTE float64_t arm_euclidean_distance_f64(const float64_t *pA,const float64_t *pB, uint32_t blockSize)
{
    float64_t accum=0.,tmp;
    uint32_t blkCnt;
#if defined(ARM_MATH_NEON) && defined(__aarch64__) 
    float64x2_t accumV,tmpV , pAV ,pBV;
    accumV = vdupq_n_f64(0.0);
    blkCnt = blockSize >> 1U;
    while(blkCnt > 0U)
    {
        pAV = vld1q_f64(pA);
        pBV = vld1q_f64(pB);
        tmpV = vsubq_f64(pAV, pBV);
        accumV = vmlaq_f64(accumV, tmpV, tmpV);
        pA+=2;
        pB+=2;
        blkCnt--;
    }
    accum = vaddvq_f64(accumV);
    blkCnt = blockSize & 1;
#else
    blkCnt = blockSize;
#endif
    while(blkCnt > 0)
    {
        tmp = *pA++ - *pB++;
        accum += ARM_SQ(tmp);
        blkCnt --;
    }
    tmp = sqrt(accum);
    return(tmp);
}

/**
 * @} end of Euclidean group
 */
