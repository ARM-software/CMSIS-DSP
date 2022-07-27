/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_sin_cos_f32.c
 * Description:  Sine and Cosine calculation for floating-point values
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

#include "dsp/controller_functions.h"
#include "arm_common_tables.h"

/**
  @addtogroup SinCos
  @{
 */

/**
  @brief         Floating-point sin_cos function.
  @param[in]     theta    input value in degrees
  @param[out]    pSinVal  points to processed sine output
  @param[out]    pCosVal  points to processed cosine output
  @return        none
 */

void arm_sin_cos_f32(
  float32_t theta,
  float32_t * pSinVal,
  float32_t * pCosVal)
{
  float32_t fract, in;                             /* Temporary input, output variables */
  uint16_t indexS, indexC;                         /* Index variable */
  float32_t f1, f2, d1, d2;                        /* Two nearest output values */
  float32_t Dn, Df;
  float32_t temp, findex;

  /* input x is in degrees */
  /* Scale input, divide input by 360, for cosine add 0.25 (pi/2) to read sine table */
  in = theta * 0.00277777777778f;

  if (in < 0.0f)
  {
    in = -in;
  }

  in = in - (int32_t)in;

  /* Calculate the nearest index */
  findex = (float32_t)FAST_MATH_TABLE_SIZE * in;
  indexS = ((uint16_t)findex) & 0x1ff;
  indexC = (indexS + (FAST_MATH_TABLE_SIZE / 4)) & 0x1ff;

  /* Calculation of fractional value */
  fract = findex - (float32_t) indexS;

  /* Read two nearest values of input value from the cos & sin tables */
  f1 =  sinTable_f32[indexC  ];
  f2 =  sinTable_f32[indexC+1];
  d1 = -sinTable_f32[indexS  ];
  d2 = -sinTable_f32[indexS+1];

  Dn = 0.0122718463030f; /* delta between the two points (fixed), in this case 2*pi/FAST_MATH_TABLE_SIZE */
  Df = f2 - f1;          /* delta between the values of the functions */

  temp = Dn * (d1 + d2) - 2 * Df;
  temp = fract * temp + (3 * Df - (d2 + 2 * d1) * Dn);
  temp = fract * temp + d1 * Dn;

  /* Calculation of cosine value */
  *pCosVal = fract * temp + f1;

  /* Read two nearest values of input value from the cos & sin tables */
  f1 = sinTable_f32[indexS  ];
  f2 = sinTable_f32[indexS+1];
  d1 = sinTable_f32[indexC  ];
  d2 = sinTable_f32[indexC+1];


  Df = f2 - f1; // delta between the values of the functions
  temp = Dn * (d1 + d2) - 2 * Df;
  temp = fract * temp + (3 * Df - (d2 + 2 * d1) * Dn);
  temp = fract * temp + d1 * Dn;

  /* Calculation of sine value */
  *pSinVal = fract * temp + f1;

  if (theta < 0.0f)
  {
    *pSinVal = -*pSinVal;
  }
}

/**
  @} end of SinCos group
 */
