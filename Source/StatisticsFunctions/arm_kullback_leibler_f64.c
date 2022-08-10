/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_logsumexp_f64.c
 * Description:  LogSumExp
 *
 * $Date:        10 August 2022
 * $Revision:    V1.9.1
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
#include <limits.h>
#include <math.h>

/**
 * @addtogroup Kullback-Leibler
 * @{
 */

/**
 * @brief Kullback-Leibler
 *
 * @param[in]  *pSrcA         points to an array of input values for probaility distribution A.
 * @param[in]  *pSrcB         points to an array of input values for probaility distribution B.
 * @param[in]  blockSize      number of samples in the input array.
 * @return Kullback-Leibler divergence D(A || B)
 *
 */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "NEMath.h"

float64_t arm_kullback_leibler_f64(const float64_t * pSrcA,const float64_t * pSrcB,uint32_t blockSize)
{
    const float64_t *pInA, *pInB;
    uint32_t blkCnt;
    float64_t accum, pA,pB;

    float64x2_t accumV;
    float64x2_t tmpVA, tmpVB,tmpV;
 
    pInA = pSrcA;
    pInB = pSrcB;

    accum = 0.0f;
    accumV = vdupq_n_f64(0.0f);

    blkCnt = blockSize >> 1;
    while(blkCnt > 0)
    {
      tmpVA = vld1q_f64(pInA);
      pInA += 2;

      tmpVB = vld1q_f64(pInB);
      pInB += 2;

      tmpV = vinvq_f64(tmpVA);
      tmpVB = vmulq_f64(tmpVB, tmpV);
      tmpVB = vlogq_f64(tmpVB);

      accumV = vmlaq_f64(accumV, tmpVA, tmpVB);
       
      blkCnt--;
    
    }


  accum = vaddvq_f64(accumV);

    blkCnt = blockSize & 1;
    while(blkCnt > 0)
    {
       pA = *pInA++;
       pB = *pInB++;
       accum += pA * logf(pB/pA);
       
       blkCnt--;
    
    }

    return(-accum);
}
#else

float64_t arm_kullback_leibler_f64(const float64_t * pSrcA, const float64_t * pSrcB, uint32_t blockSize)
{
    const float64_t *pInA, *pInB;
    uint32_t blkCnt;
    float64_t accum, pA,pB;
 
    pInA = pSrcA;
    pInB = pSrcB;
    blkCnt = blockSize;

    accum = 0.0;

    while(blkCnt > 0)
    {
        pA = *pInA++;
        pB = *pInB++;

        accum += pA * log(pB / pA);
       
        blkCnt--;
    }

    return(-accum);
}
#endif
/**
 * @} end of Kullback-Leibler group
 */
