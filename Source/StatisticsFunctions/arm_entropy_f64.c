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
#if defined(ARM_MATH_NEON) && defined(__aarch64__)
#include "arm_vec_math.h"
#endif

/**
 * @addtogroup Entropy
 * @{
 */

/**
 * @brief Entropy
 *
 * @param[in]  pSrcA        Array of input values.
 * @param[in]  blockSize    Number of samples in the input array.
 * @return     Entropy      -Sum(p ln p)
 *
 */

ARM_DSP_ATTRIBUTE float64_t arm_entropy_f64(const float64_t * pSrcA, uint32_t blockSize)
{
    const float64_t *pIn;
    uint32_t blkCnt;
    float64_t accum, p;
    
    pIn = pSrcA;
    
    accum = 0.0;

    blkCnt = blockSize;
    
    while(blkCnt > 0)
    {
        p = *pIn++;
        
        accum += p * log(p);
        
        blkCnt--;
        
    }
    
    return(-accum);
}

/**
 * @} end of Entropy group
 */
