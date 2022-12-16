/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_hft169d_f32.c
 * Description:  Floating-point (f32) Hft169d window
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
  @addtogroup WindowFlat
  @{
 */

/**
  @defgroup WindowHFT169D Hft169d window function (169.5 dB)

 */

/**
  @ingroup WindowHFT169D
 */

/**
  @brief         Hft169d window generating function (f32).
  @param[out]    pDst       points to the output generated window
  @param[in]     blockSize  number of samples in the window
  @return        none
 
  @par Parameters of the window
  
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |          169.5 dB  |
  | Normalized equivalent noise bandwidth |       4.8347 bins  |
  | 3 dB bandwidth                        |       4.7588 bins  |
  | Flatness                              |         0.0017 dB  |
  | Recommended overlap                   |            81.2 %  |

Included in CMSIS-DSP with authorization from professor
Gerhard Heinzel.

@par Original article:
Spectrum and spectral density estimation by the Discrete Fourier
transform (DFT), including a comprehensive list of window
functions and some new 
flat-top windows.

@par Authors: 
G. Heinzel, A. Rudiger and R. Schilling,
Max-Planck-Institut fur Gravitationsphysik
(Albert-Einstein-Institut)
Teilinstitut Hannover
 */



void arm_hft169d_f32(
        float32_t * pDst,
        uint32_t blockSize)
{
   float32_t k = 2.0f / ((float32_t) blockSize);
   float32_t w;

   for(uint32_t i=0;i<blockSize;i++)
   {
     w = PI * (i * k);
        w =
    (1.0f -
     1.97441843f * cosf (w) +
     1.65409889f * cosf (2.f * w) -
     0.95788187f * cosf (3.f * w) +
     0.33673420f * cosf (4.f * w) -
     0.06364622f * cosf (5.f * w) +
     0.00521942f * cosf (6.f * w) - 0.00010599f * cosf (7.f * w));
  
     pDst[i] = w;
   }
}

/**
  @} end of WindowFlat group
 */

