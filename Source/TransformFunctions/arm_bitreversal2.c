/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_bitreversal2.c
 * Description:  Bitreversal functions
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2019 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/transform_functions.h"
#include "arm_common_tables.h"

void arm_bitreversal_64(
        uint64_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab);


/**
  @brief         In-place 64 bit reversal function.
  @param[in,out] pSrc        points to in-place buffer of unknown 64-bit data type
  @param[in]     bitRevLen   bit reversal table length
  @param[in]     pBitRevTab  points to bit reversal table
*/

ARM_DSP_ATTRIBUTE void arm_bitreversal_64(
        uint64_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
  uint64_t a, b, tmp;
  uint32_t i;

  for (i = 0; i < bitRevLen; )
  {
     a = pBitRevTab[i    ] >> 2;
     b = pBitRevTab[i + 1] >> 2;

     //real
     tmp = pSrc[a];
     pSrc[a] = pSrc[b];
     pSrc[b] = tmp;

     //complex
     tmp = pSrc[a+1];
     pSrc[a+1] = pSrc[b+1];
     pSrc[b+1] = tmp;

    i += 2;
  }
}

void arm_bitreversal_32(
        uint32_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab);

/**
  @brief         In-place 32 bit reversal function.
  @param[in,out] pSrc        points to in-place buffer of unknown 32-bit data type
  @param[in]     bitRevLen   bit reversal table length
  @param[in]     pBitRevTab  points to bit reversal table
*/


#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

#define SWAP(I) \
  ltmp = p[vgetq_lane_u16(v.val[0],I)];\
  p[vgetq_lane_u16(v.val[0],I)] = p[vgetq_lane_u16(v.val[1],I)];\
  p[vgetq_lane_u16(v.val[1],I)] = ltmp;

ARM_DSP_ATTRIBUTE void arm_bitreversal_32(
        uint32_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
  uint32_t a, b, i;
  uint16x8x2_t v;
  uint64_t ltmp;
  uint64_t *p = (uint64_t*)pSrc;

  for (i = 0; i <= bitRevLen - 16; i+= 16)
  {
     //a = *pBitRevTab++ >> 2;
     //b = *pBitRevTab++ >> 2;
     v = vld2q_u16(pBitRevTab);
     v.val[0] = vshrq_n_u16(v.val[0],3);
     v.val[1] = vshrq_n_u16(v.val[1],3);
     pBitRevTab += 16;

     //real / complex
     SWAP(0);
     SWAP(1);
     SWAP(2);
     SWAP(3);
     SWAP(4);
     SWAP(5);
     SWAP(6);
     SWAP(7);

    
  }

  for (;i < bitRevLen; i+= 2)
  {
     a = *pBitRevTab++ >> 3;
     b = *pBitRevTab++ >> 3;

     //real / complex
     ltmp = p[a];
     p[a] = p[b];
     p[b] = ltmp;
    

  }
}
#else

ARM_DSP_ATTRIBUTE void arm_bitreversal_32(
        uint32_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
  uint32_t a, b, i, tmp;

  for (i = 0; i < bitRevLen; )
  {
     a = pBitRevTab[i    ] >> 2;
     b = pBitRevTab[i + 1] >> 2;

     //real
     tmp = pSrc[a];
     pSrc[a] = pSrc[b];
     pSrc[b] = tmp;

     //complex
     tmp = pSrc[a+1];
     pSrc[a+1] = pSrc[b+1];
     pSrc[b+1] = tmp;

    i += 2;
  }
}
#endif
void arm_bitreversal_16(
        uint16_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab);


/**
  @brief         In-place 16 bit reversal function.
  @param[in,out] pSrc        points to in-place buffer of unknown 16-bit data type
  @param[in]     bitRevLen   bit reversal table length
  @param[in]     pBitRevTab  points to bit reversal table
*/

ARM_DSP_ATTRIBUTE void arm_bitreversal_16(
        uint16_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
  uint16_t a, b, tmp;
  uint32_t i;

  for (i = 0; i < bitRevLen; )
  {
     a = pBitRevTab[i    ] >> 2;
     b = pBitRevTab[i + 1] >> 2;

     //real
     tmp = pSrc[a];
     pSrc[a] = pSrc[b];
     pSrc[b] = tmp;

     //complex
     tmp = pSrc[a+1];
     pSrc[a+1] = pSrc[b+1];
     pSrc[b+1] = tmp;

    i += 2;
  }
}
