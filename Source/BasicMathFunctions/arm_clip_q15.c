/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_clip_q15.c
 * Description:  Floating-point vector addition
 *
 *
 * Target Processor: Cortex-M cores
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */


/**
  @addtogroup BasicClip
  @{
 */

/**
  @brief         Elementwise fixed-point clipping
  @param[in]     pSrc          points to input values
  @param[out]    pDst          points to output clipped values
  @param[in]     low           lower bound
  @param[in]     high          higher bound
  @param[in]     numSamples    number of samples to clip
  @return        none
 */

void arm_clip_q15(const q15_t * pSrc, 
  q15_t * pDst, 
  q15_t low, 
  q15_t high, 
  uint32_t numSamples)
{
    for (uint32_t i = 0; i < numSamples; i++)
    {                                        
        if (pSrc[i] > high)                  
            pDst[i] = high;                  
        else if (pSrc[i] < low)              
            pDst[i] = low;                   
        else                                 
            pDst[i] = pSrc[i];               
    }
}

/**
  @} end of BasicClip group
 */
