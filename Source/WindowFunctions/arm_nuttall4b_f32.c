/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_nuttall4b_f32.c
 * Description:  Floating-point (f32) Nuttall4b window
 *
 * $Date:        14 December 2022
 * $Revision:    v1.15.0
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

#include "dsp/window_functions.h"
#include "dsp/fast_math_functions.h"
#include <math.h>

/**
  @ingroup groupWindow
 */




/**
  @addtogroup WindowNormal
  @{
 */

/**
  @defgroup WindowNUTTALL4B Nuttall4b window function (93.3 dB)

 */

/**
  @ingroup WindowNUTTALL4B
 */

/**
  @brief         Nuttall4b window generating function (f32).
  @param[out]    pDst       points to the output generated window
  @param[in]     blockSize  number of samples in the window
 
  @par Parameters of the window
  
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           93.3 dB  |
  | Normalized equivalent noise bandwidth |       2.0212 bins  |
  | 3 dB bandwidth                        |       1.9122 bins  |
  | Flatness                              |        -0.8118 dB  |
  | Recommended overlap                   |            66.3 %  |

 */



ARM_DSP_ATTRIBUTE void arm_nuttall4b_f32(
        float32_t * pDst,
        uint32_t blockSize)
{
   float32_t k = 2.0f / ((float32_t) blockSize);
   float32_t w;

   for(uint32_t i=0;i<blockSize;i++)
   {
     w = PI * i * k;
        w = 0.355768f - 0.487396f * cosf (w) +
    0.144232f * cosf (2.f * w) - 0.012604f * cosf (3.f * w);
        
     pDst[i] = w;
   }
}

/**
  @} end of WindowNormal group
 */

