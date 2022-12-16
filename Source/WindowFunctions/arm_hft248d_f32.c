/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_hft248d_f32.c
 * Description:  Floating-point (f32) Hft248d window
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
  @defgroup WindowHFT248D Hft248d window function (248.4 dB)

 */

/**
  @ingroup WindowHFT248D
 */

/**
  @brief         Hft248d window generating function (f32).
  @param[out]    pDst       points to the output generated window
  @param[in]     blockSize  number of samples in the window
  @return        none
 
  @par Parameters of the window
  
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |          248.4 dB  |
  | Normalized equivalent noise bandwidth |       5.6512 bins  |
  | 3 dB bandwidth                        |       5.5567 bins  |
  | Flatness                              |         0.0009 dB  |
  | Recommended overlap                   |            84.1 %  |

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



void arm_hft248d_f32(
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
     1.985844164102f * cosf (w) +
     1.791176438506f * cosf (2.f * w) -
     1.282075284005f * cosf (3.f * w) +
     0.667777530266f * cosf (4.f * w) -
     0.240160796576f * cosf (5.f * w) +
     0.056656381764f * cosf (6.f * w) -
     0.008134974479f * cosf (7.f * w) +
     0.000624544650f * cosf (8.f * w) -
     0.000019808998f * cosf (9.f * w) +
     0.000000132974f * cosf (10.f * w));
     pDst[i] = w;
   }
}

/**
  @} end of WindowFlat group
 */

