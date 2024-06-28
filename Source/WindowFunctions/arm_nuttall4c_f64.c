/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_nuttall4c_f64.c
 * Description:  Floating-point (f64) Nuttall4c window
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
  @ingroup WindowNUTTALL4C
 */

/**
  @brief         Nuttall4c window generating function (f64).
  @param[out]    pDst       points to the output generated window
  @param[in]     blockSize  number of samples in the window
 
  @par Parameters of the window
  
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           98.1 dB  |
  | Normalized equivalent noise bandwidth |       1.9761 bins  |
  | 3 dB bandwidth                        |       1.8687 bins  |
  | Flatness                              |        -0.8506 dB  |
  | Recommended overlap                   |            65.6 %  |

 */



ARM_DSP_ATTRIBUTE void arm_nuttall4c_f64(
        float64_t * pDst,
        uint32_t blockSize)
{
   float64_t k = 2.0 / ((float64_t) blockSize);
   float64_t w;

   for(uint32_t i=0;i<blockSize;i++)
   {
    w = PI_F64 * i * k;
        w = 0.3635819 - 0.4891775 * cos (w) +
    0.1365995 * cos (2 * w) - 0.0106411 * cos (3 * w);
        
     pDst[i] = w;
   }
}

/**
  @} end of WindowNormal group
 */

