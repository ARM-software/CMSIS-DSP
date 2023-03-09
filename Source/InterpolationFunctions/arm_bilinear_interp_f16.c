/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_bilinear_interp_f16.c
 * Description:  Floating-point bilinear interpolation
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
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

#include "dsp/interpolation_functions_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)


/**
  @ingroup groupInterpolation
 */



  /**
   * @addtogroup BilinearInterpolate
   * @{
   */


  /**
  * @brief  Floating-point bilinear interpolation.
  * @param[in,out] S  points to an instance of the interpolation structure.
  * @param[in]     X  interpolation coordinate.
  * @param[in]     Y  interpolation coordinate.
  * @return out interpolated value.
  */
  float16_t arm_bilinear_interp_f16(
  const arm_bilinear_interp_instance_f16 * S,
  float16_t X,
  float16_t Y)
  {
    float16_t out;
    float16_t f00, f01, f10, f11;
    const float16_t *pData = S->pData;
    int32_t xIndex, yIndex, index;
    float16_t xdiff, ydiff;
    float16_t b1, b2, b3, b4;

    xIndex = (int32_t) X;
    yIndex = (int32_t) Y;

    /* Care taken for table outside boundary */
    /* Returns zero output when values are outside table boundary */
    if (xIndex < 0 || xIndex > (S->numCols - 2) || yIndex < 0 || yIndex > (S->numRows - 2))
    {
      return (0);
    }

    /* Calculation of index for two nearest points in X-direction */
    index = (xIndex ) + (yIndex ) * S->numCols;


    /* Read two nearest points in X-direction */
    f00 = pData[index];
    f01 = pData[index + 1];

    /* Calculation of index for two nearest points in Y-direction */
    index = (xIndex ) + (yIndex+1) * S->numCols;


    /* Read two nearest points in Y-direction */
    f10 = pData[index];
    f11 = pData[index + 1];

    /* Calculation of intermediate values */
    b1 = f00;
    b2 = (_Float16)f01 - (_Float16)f00;
    b3 = (_Float16)f10 - (_Float16)f00;
    b4 = (_Float16)f00 - (_Float16)f01 - (_Float16)f10 + (_Float16)f11;

    /* Calculation of fractional part in X */
    xdiff = (_Float16)X - (_Float16)xIndex;

    /* Calculation of fractional part in Y */
    ydiff = (_Float16)Y - (_Float16)yIndex;

    /* Calculation of bi-linear interpolated output */
    out = (_Float16)b1 + (_Float16)b2 * (_Float16)xdiff + 
    (_Float16)b3 * (_Float16)ydiff + (_Float16)b4 * (_Float16)xdiff * (_Float16)ydiff;

    /* return to application */
    return (out);
  }

  /**
   * @} end of BilinearInterpolate group
   */


#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */ 

