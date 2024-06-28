
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_dtw_path_f32.c
 * Description:  Warping path
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
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

#include "dsp/distance_functions.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>


/**
  @addtogroup DTW
  @{
 */


/**
 * @brief        Window for dynamic time warping computation
 * @param[in]    windowType  Type of window
 * @param[in]    windowSize  Window size
 * @param[in,out] pWindow Window
 * @return Error if window type not recognized
 *
 * @par Windowing matrix
 * The window matrix will contain 1 for the
 * position which are accepted and 0 for the
 * positions which are rejected.
 * 
 * The input matrix must already contain a buffer
 * and the number of rows (query length) and columns
 * (template length) must be initialized.
 * The function will fill the matrix with 0 and 1.
 *
 */
ARM_DSP_ATTRIBUTE arm_status arm_dtw_init_window_q7(const arm_dtw_window windowType,
                            const int32_t windowSize,
                            arm_matrix_instance_q7 *pWindow)
{
  const int32_t queryLength = pWindow -> numRows;
  const int32_t templateLength = pWindow -> numCols;
   
  switch(windowType)
  {
     case ARM_DTW_SAKOE_CHIBA_WINDOW:
     {
        for(int32_t q = 0; q < queryLength; q++)
        {
           for(int32_t t = 0; t < templateLength; t++)
           {
              pWindow->pData[templateLength*q + t] = (q7_t)(abs(q-t) <= windowSize);
           }
        }
     }
     break;
/*
     case ARM_DTW_ITAKURA_WINDOW:
     {
        for(int32_t q = 0; q < queryLength; q++)
        {
           for(int32_t t = 0; t < templateLength; t++)
           {
              pWindow->pData[templateLength*q + t] = (q7_t)(
                 (t < 2 * q) && 
                 (q <= 2 * t) && 
                 (q >= queryLength - 1 - 2 * (templateLength - t)) &&
                 (t > templateLength - 1 - 2 * (queryLength - q)));
           }
        }
     }
     break;
*/
     case ARM_DTW_SLANTED_BAND_WINDOW:
     {
        for(int32_t q = 0; q < queryLength; q++)
        {
           for(int32_t t = 0; t < templateLength; t++)
           {
              float32_t diag = (1.0f * q * templateLength / queryLength);
              pWindow->pData[templateLength*q + t] = (q7_t)(fabsf((float32_t)t - diag) <= (float32_t)windowSize);
           }
        }
     }
     break;

     default:
      return(ARM_MATH_ARGUMENT_ERROR);
  }

  return(ARM_MATH_SUCCESS);
}

/**
 * @} end of DTW group
 */

