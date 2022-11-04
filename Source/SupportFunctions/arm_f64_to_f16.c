/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_f64_to_f16.c
 * Description:  Converts the elements of the 64 bit floating-point vector to 16 bit floating-point vector
 *
 * $Date:        18 August 2022
 * $Revision:    V1.0.0
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

#include "dsp/support_functions_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)

/**
  @ingroup groupSupport
 */

/**
 * @defgroup f64_to_x  Convert 64-bit floating point value
 */

/**
  @addtogroup f64_to_x
  @{
 */

/**
  @brief         Converts the elements of the f64 vector to f16 vector.
  @param[in]     pSrc       points to the f64 input vector
  @param[out]    pDst       points to the f16 output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

 */


void arm_f64_to_f16(
  const float64_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
    const float64_t *pIn = pSrc;      /* Src pointer */
    uint32_t  blkCnt;           /* loop counter */

    /*
     * Loop over blockSize number of values
     */
    blkCnt = blockSize;

    while (blkCnt > 0U)
    {

        *pDst++ = (float16_t) * pIn++;
        /*
         * Decrement the loop counter
         */
        blkCnt--;
    }
}
/**
  @} end of f64_to_x group
 */

#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */ 
