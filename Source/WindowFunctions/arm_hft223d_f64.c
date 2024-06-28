/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_hft223d_f64.c
 * Description:  Floating-point (f64) Hft223d window
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
  @ingroup WindowHFT223D
 */

/**
  @brief         Hft223d window generating function (f64).
  @param[out]    pDst       points to the output generated window
  @param[in]     blockSize  number of samples in the window
 
  @par Parameters of the window
  
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |          223.0 dB  |
  | Normalized equivalent noise bandwidth |       5.3888 bins  |
  | 3 dB bandwidth                        |       5.3000 bins  |
  | Flatness                              |         0.0011 dB  |
  | Recommended overlap                   |            83.3 %  |

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



ARM_DSP_ATTRIBUTE void arm_hft223d_f64(
        float64_t * pDst,
        uint32_t blockSize)
{
   float64_t k = 2. / ((float64_t) blockSize);
   float64_t w;

   for(uint32_t i=0;i<blockSize;i++)
   {
    w = PI_F64 * (i * k);
        w =
    (1.0 -
     1.98298997309 * cos (w) +
     1.75556083063 * cos (2. * w) -
     1.19037717712 * cos (3. * w) +
     0.56155440797 * cos (4. * w) -
     0.17296769663 * cos (5. * w) +
     0.03233247087 * cos (6. * w) -
     0.00324954578 * cos (7. * w) +
     0.00013801040 * cos (8. * w) - 0.00000132725 * cos (9. * w));
        
     pDst[i] = w;
   }
}

/**
  @} end of WindowFlat group
 */

