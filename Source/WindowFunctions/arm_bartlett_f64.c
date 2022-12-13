/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_bartlett_f64.c
 * Description:  Floating-point (f64) Bartlett window
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

/**
  @ingroup groupWindow
 */




/**
  @addtogroup WindowNormal
  @{
 */


/**
  @ingroup WindowBARTLETT
 */

/**
  @brief         Bartlett window generating function (f64).
  @param[out]    pDst       points to the output generated window
  @param[in]     blockSize  number of samples in the window
  @return        none
 
  @par Parameters of the window
  
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           26.5 dB  |
  | Normalized equivalent noise bandwidth |       1.3333 bins  |
  | 3 dB bandwidth                        |       1.2736 bins  |
  | Flatness                              |        -1.8242 dB  |
  | Recommended overlap                   |            50.0 %  |

 */



void arm_bartlett_f64(
        float64_t * pDst,
        uint32_t blockSize)
{
   float64_t k = 2. / ((float64_t) blockSize);
   float64_t w;

   for(uint32_t i=0;i<blockSize;i++)
   {
     w = i * k ;
     if (i * k > 1.0)
     {
       w = 2.0 - w;
     }
     pDst[i] = w;
   }
}

/**
  @} end of WindowNormal group
 */

