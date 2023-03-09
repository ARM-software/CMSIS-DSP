/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_linear_interp_f16.c
 * Description:  Floating-point linear interpolation
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
   * @addtogroup LinearInterpolate
   * @{
   */

  /**
   * @brief  Process function for the floating-point Linear Interpolation Function.
   * @param[in,out] S  is an instance of the floating-point Linear Interpolation structure
   * @param[in]     x  input sample to process
   * @return y processed output sample.
   *
   */
  float16_t arm_linear_interp_f16(
  arm_linear_interp_instance_f16 * S,
  float16_t x)
  {
    float16_t y;
    float16_t x0, x1;                            /* Nearest input values */
    float16_t y0, y1;                            /* Nearest output values */
    float16_t xSpacing = S->xSpacing;            /* spacing between input values */
    int32_t i;                                   /* Index variable */
    const float16_t *pYData = S->pYData;               /* pointer to output table */

    /* Calculation of index */
    i = (int32_t) (((_Float16)x - (_Float16)S->x1) / (_Float16)xSpacing);

    if (((_Float16)x < (_Float16)S->x1))
    {
      /* Iniatilize output for below specified range as least output value of table */
      y = pYData[0];
    }
    else if ((uint32_t)i >= (S->nValues - 1))
    {
      /* Iniatilize output for above specified range as last output value of table */
      y = pYData[S->nValues - 1];
    }
    else
    {
      /* Calculation of nearest input values */
      x0 = (_Float16)S->x1 +  (_Float16)i      * (_Float16)xSpacing;
      x1 = (_Float16)S->x1 + (_Float16)(i + 1) * (_Float16)xSpacing;

      /* Read of nearest output values */
      y0 = pYData[i];
      y1 = pYData[i + 1];

      /* Calculation of output */
      y = (_Float16)y0 + ((_Float16)x - (_Float16)x0) * 
      (((_Float16)y1 - (_Float16)y0) / ((_Float16)x1 - (_Float16)x0));

    }

    /* returns output value */
    return (y);
  }

  /**
   * @} end of LinearInterpolate group
   */


#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */ 

