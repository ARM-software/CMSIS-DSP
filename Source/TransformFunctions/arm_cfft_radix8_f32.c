/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_radix8_f32.c
 * Description:  Radix-8 Decimation in Frequency CFFT & CIFFT Floating point processing function
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

#include "dsp/transform_functions.h"


/* ----------------------------------------------------------------------
 * Internal helper function used by the FFTs
 * -------------------------------------------------------------------- */

void arm_radix8_butterfly_f32(
  float32_t * pSrc,
  uint16_t fftLen,
  const float32_t * pCoef,
  uint16_t twidCoefModifier);

/**
  brief         Core function for the floating-point CFFT butterfly process.
  param[in,out] pSrc             points to the in-place buffer of floating-point data type.
  param[in]     fftLen           length of the FFT.
  param[in]     pCoef            points to the twiddle coefficient buffer.
  param[in]     twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table.
  return        none
*/
//#undef ARM_MATH_NEON
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

#include <stdio.h>

#define TW(X) \
  printf("%ld\n",(unsigned long)(X)); 

ARM_DSP_ATTRIBUTE void arm_radix8_butterfly_f32(
  float32_t * pSrc,
  uint16_t fftLen,
  const float32_t * pCoef,
  uint16_t twidCoefModifier)
{
   const float32_t *pia1, *pia2;
   float32_t *pi1, *pi2, *pi3, *pi4, *pi5, *pi6, *pi7, *pi8;
   uint32_t i1;
   uint32_t id;
   uint32_t n1, n2 ;
   //uint32_t j;

   float32_t r1, r2, r3, r4, r5, r6, r7, r8;
   float32_t t1, t2;
   float32_t s1, s2, s3, s4, s5, s6, s7, s8;
   float32_t p1, p2, p3, p4;
   float32_t co;
   float32_t si;
   const float32_t C81 = 0.70710678118f;

   n2 = fftLen;
   pia1 = pCoef;

   do
   {
      n1 = n2;
      n2 = n2 >> 3;
      pi1 = pSrc;
      pi2 = pi1 + 2*n2;
      pi3 = pi2 + 2*n2;
      pi4 = pi3 + 2*n2;
      pi5 = pi4 + 2*n2;
      pi6 = pi5 + 2*n2;
      pi7 = pi6 + 2*n2;
      pi8 = pi7 + 2*n2;

      i1 = 0;

      do
      {
         r1 = pi1[0] + pi5[0];
         r5 = pi1[0] - pi5[0];
         r2 = pi2[0] + pi6[0];
         r6 = pi2[0] - pi6[0];
         r3 = pi3[0] + pi7[0];
         r7 = pi3[0] - pi7[0];
         r4 = pi4[0] + pi8[0];
         r8 = pi4[0] - pi8[0];
         t1 = r1 - r3;
         r1 = r1 + r3;
         r3 = r2 - r4;
         r2 = r2 + r4;
         pi1[0] = r1 + r2;
         pi5[0] = r1 - r2;
         r1 = pi1[1] + pi5[1];
         s5 = pi1[1] - pi5[1];
         r2 = pi2[1] + pi6[1];
         s6 = pi2[1] - pi6[1];
         s3 = pi3[1] + pi7[1];
         s7 = pi3[1] - pi7[1];
         r4 = pi4[1] + pi8[1];
         s8 = pi4[1] - pi8[1];
         t2 = r1 - s3;
         r1 = r1 + s3;
         s3 = r2 - r4;
         r2 = r2 + r4;
         pi1[1] = r1 + r2;
         pi5[1] = r1 - r2;
         pi3[0]     = t1 + s3;
         pi7[0]     = t1 - s3;
         pi3[1] = t2 - r3;
         pi7[1] = t2 + r3;
         r1 = (r6 - r8) * C81;
         r6 = (r6 + r8) * C81;
         r2 = (s6 - s8) * C81;
         s6 = (s6 + s8) * C81;
         t1 = r5 - r1;
         r5 = r5 + r1;
         r8 = r7 - r6;
         r7 = r7 + r6;
         t2 = s5 - r2;
         s5 = s5 + r2;
         s8 = s7 - s6;
         s7 = s7 + s6;
         pi2[0]   = r5 + s7;
         pi8[0]   = r5 - s7;
         pi6[0]   = t1 + s8;
         pi4[0]   = t1 - s8;
         pi2[1] = s5 - r7;
         pi8[1] = s5 + r7;
         pi6[1] = t2 - r8;
         pi4[1] = t2 + r8;

         pi1 += (2*n1);
         pi2 += (2*n1);
         pi3 += (2*n1);
         pi4 += (2*n1);
         pi5 += (2*n1);
         pi6 += (2*n1);
         pi7 += (2*n1);
         pi8 += (2*n1);

         i1 += n1;
      } while (i1 < fftLen);

      if (n2 < 8)
         break;

      

      i1 = 0;
      pi1 = pSrc;

      pi2 = pi1 + 2*(n2);
      pi3 = pi2 + 2*(n2);
      pi4 = pi3 + 2*(n2);
      pi5 = pi4 + 2*(n2);
      pi6 = pi5 + 2*(n2);
      pi7 = pi6 + 2*(n2);
      pi8 = pi7 + 2*(n2);

      do
      {
         /*  index calculation for the input */

         pi1 = pi1 + 2;
         pi2 = pi2 + 2;
         pi3 = pi3 + 2;
         pi4 = pi4 + 2;
         pi5 = pi5 + 2;
         pi6 = pi6 + 2;
         pi7 = pi7 + 2;
         pi8 = pi8 + 2;

         //j = 1;
         

         //do
         int j;
         pia2 = pia1;

         for(int j=1;j<n2;j++)
         {
            /*  index calculation for the coefficients */
            
            r1 = *pi1 + *pi5;
            s1 = pi1[1] + pi5[1];

            r5 = *pi1 - *pi5;
            s5 = pi1[1] - pi5[1];

            r2 = *pi2 + *pi6;
            s2 = pi2[1] + pi6[1];

            r6 = *pi2 - *pi6;
            s6 = pi2[1] - pi6[1];

            r3 = *pi3 + *pi7;
            s3 = pi3[1] + pi7[1];

            r7 = *pi3 - *pi7;
            s7 = pi3[1] - pi7[1];

            r4 = *pi4 + *pi8;
            s4 = pi4[1] + pi8[1];

            r8 = *pi4 - *pi8;
            s8 = pi4[1] - pi8[1];

            t1 = r1 - r3;
            t2 = s1 - s3;

            r1 = r1 + r3;
            s1 = s1 + s3;

            r3 = r2 - r4;
            s3 = s2 - s4;

            r2 = r2 + r4;
            s2 = s2 + s4;

            *pi1++ = r1 + r2;
            *pi1++ = s1 + s2;

            r2 = r1 - r2;
            s2 = s1 - s2;

            r1 = t1 + s3;
            s1 = t2 - r3;

            t1 = t1 - s3;
            t2 = t2 + r3;

            co = *pia2++;
            si = *pia2++;

            p1 = co * r2;
            p2 = si * s2;
            p3 = co * s2;
            p4 = si * r2;
            *pi5++ = p1 + p2;
            *pi5++ = p3 - p4;
            co = *pia2++;
            si = *pia2++;
            p1 = co * r1;
            p2 = si * s1;
            p3 = co * s1;
            p4 = si * r1;
            *pi3++ = p1 + p2;
            *pi3++ = p3 - p4;
            co = *pia2++;
            si = *pia2++;
            p1 = co * t1;
            p2 = si * t2;
            p3 = co * t2;
            p4 = si * t1;
            *pi7++ = p1 + p2;
            *pi7++ = p3 - p4;
            r1 = (r6 - r8) * C81;
            s1 = (s6 - s8) * C81;

            r6 = (r6 + r8) * C81;
            s6 = (s6 + s8) * C81;

            t1 = r5 - r1;
            t2 = s5 - s1;

            r5 = r5 + r1;
            s5 = s5 + s1;

            r8 = r7 - r6;
            s8 = s7 - s6;

            r7 = r7 + r6;
            s7 = s7 + s6;

            r1 = r5 + s7;
            s1 = s5 - r7;

            r5 = r5 - s7;
            s5 = s5 + r7;

            r6 = t1 + s8;
            s6 = t2 - r8;

            t1 = t1 - s8;
            t2 = t2 + r8;
            co = *pia2++;
            si = *pia2++;
            p1 = co * r1;
            p2 = si * s1;
            p3 = co * s1;
            p4 = si * r1;
            *pi2++ = p1 + p2;
            *pi2++ = p3 - p4;
            co = *pia2++;
            si = *pia2++;
            p1 = co * r5;
            p2 = si * s5;
            p3 = co * s5;
            p4 = si * r5;
            *pi8++ = p1 + p2;
            *pi8++ = p3 - p4;
            co = *pia2++;
            si = *pia2++;
            p1 = co * r6;
            p2 = si * s6;
            p3 = co * s6;
            p4 = si * r6;
            *pi6++ = p1 + p2;
            *pi6++ = p3 - p4;
            co = *pia2++;
            si = *pia2++;
            p1 = co * t1;
            p2 = si * t2;
            p3 = co * t2;
            p4 = si * t1;
            *pi4++ = p1 + p2;
            *pi4++ = p3 - p4;

            //j++;
            
         //} while (j < n2);
         }

         
         pi1 = pi1 + 2*(n1-n2);
         pi2 = pi2 + 2*(n1-n2);
         pi3 = pi3 + 2*(n1-n2);
         pi4 = pi4 + 2*(n1-n2);
         pi5 = pi5 + 2*(n1-n2);
         pi6 = pi6 + 2*(n1-n2);
         pi7 = pi7 + 2*(n1-n2);
         pi8 = pi8 + 2*(n1-n2);

         i1 += n1;
      } while (i1 < fftLen) ;

      pia1 = pia2;
      twidCoefModifier <<= 3;
   } while (n2 > 7);
}

#if 0
ARM_DSP_ATTRIBUTE void arm_radix8_butterfly_f32(
  float32_t * pSrc,
  uint16_t fftLen,
  const float32_t * pCoef,
  uint16_t twidCoefModifier)
{
   uint32_t ia1, ia2, ia3, ia4, ia5, ia6, ia7;
   uint32_t i1, i2, i3, i4, i5, i6, i7, i8;
   uint32_t id;
   uint32_t n1, n2;
   int32_t j;

   float32_t r1, r2, r3, r4, r5, r6, r7, r8;
   float32_t t1, t2;
   float32_t s1, s2, s3, s4, s5, s6, s7, s8;
   float32_t p1, p2, p3, p4;
   float32_t co2, co3, co4, co5, co6, co7, co8;
   float32_t si2, si3, si4, si5, si6, si7, si8;

   float32x4x2_t expi2V, expi3V, expi4V, expi5V, expi6V, expi7V, expi8V;
   float32x4x2_t rs1V, rs2V, rs3V, rs4V, rs5V, rs6V, rs7V, rs8V;
   float32x4_t tmp;
   float32x4x2_t tmpRes;
   float32x4_t t1V, t2V;
   float32x4_t p1V, p2V, p3V, p4V;

   const float32_t C81 = 0.70710678118f;

   n2 = fftLen;

   do
   {
      n1 = n2;
      n2 = n2 >> 3;
      i1 = 0;

      do
      {
         i2 = i1 + n2;
         i3 = i2 + n2;
         i4 = i3 + n2;
         i5 = i4 + n2;
         i6 = i5 + n2;
         i7 = i6 + n2;
         i8 = i7 + n2;
         r1 = pSrc[2 * i1] + pSrc[2 * i5];
         r5 = pSrc[2 * i1] - pSrc[2 * i5];
         r2 = pSrc[2 * i2] + pSrc[2 * i6];
         r6 = pSrc[2 * i2] - pSrc[2 * i6];
         r3 = pSrc[2 * i3] + pSrc[2 * i7];
         r7 = pSrc[2 * i3] - pSrc[2 * i7];
         r4 = pSrc[2 * i4] + pSrc[2 * i8];
         r8 = pSrc[2 * i4] - pSrc[2 * i8];
         t1 = r1 - r3;
         r1 = r1 + r3;
         r3 = r2 - r4;
         r2 = r2 + r4;
         pSrc[2 * i1] = r1 + r2;
         pSrc[2 * i5] = r1 - r2;
         r1 = pSrc[2 * i1 + 1] + pSrc[2 * i5 + 1];
         s5 = pSrc[2 * i1 + 1] - pSrc[2 * i5 + 1];
         r2 = pSrc[2 * i2 + 1] + pSrc[2 * i6 + 1];
         s6 = pSrc[2 * i2 + 1] - pSrc[2 * i6 + 1];
         s3 = pSrc[2 * i3 + 1] + pSrc[2 * i7 + 1];
         s7 = pSrc[2 * i3 + 1] - pSrc[2 * i7 + 1];
         r4 = pSrc[2 * i4 + 1] + pSrc[2 * i8 + 1];
         s8 = pSrc[2 * i4 + 1] - pSrc[2 * i8 + 1];
         t2 = r1 - s3;
         r1 = r1 + s3;
         s3 = r2 - r4;
         r2 = r2 + r4;
         pSrc[2 * i1 + 1] = r1 + r2;
         pSrc[2 * i5 + 1] = r1 - r2;
         pSrc[2 * i3]     = t1 + s3;
         pSrc[2 * i7]     = t1 - s3;
         pSrc[2 * i3 + 1] = t2 - r3;
         pSrc[2 * i7 + 1] = t2 + r3;
         r1 = (r6 - r8) * C81;
         r6 = (r6 + r8) * C81;
         r2 = (s6 - s8) * C81;
         s6 = (s6 + s8) * C81;
         t1 = r5 - r1;
         r5 = r5 + r1;
         r8 = r7 - r6;
         r7 = r7 + r6;
         t2 = s5 - r2;
         s5 = s5 + r2;
         s8 = s7 - s6;
         s7 = s7 + s6;
         pSrc[2 * i2]     = r5 + s7;
         pSrc[2 * i8]     = r5 - s7;
         pSrc[2 * i6]     = t1 + s8;
         pSrc[2 * i4]     = t1 - s8;
         pSrc[2 * i2 + 1] = s5 - r7;
         pSrc[2 * i8 + 1] = s5 + r7;
         pSrc[2 * i6 + 1] = t2 - r8;
         pSrc[2 * i4 + 1] = t2 + r8;

         i1 += n1;
      } while (i1 < fftLen);

      if (n2 < 8)
         break;

      //ia1 = 0;
      j = 1;

      //do
      const float32_t *pc=pCoef;

      //for(;j<=(n2-4);j+=4)
      if (0)
      {
         /*  index calculation for the coefficients */

         expi2V = vld2q_f32(pc);
         pc += 8;
         expi3V = vld2q_f32(pc);
         pc += 8;
         expi4V = vld2q_f32(pc);
         pc += 8;
         expi5V = vld2q_f32(pc);
         pc += 8;
         expi6V = vld2q_f32(pc);
         pc += 8;
         expi7V = vld2q_f32(pc);
         pc += 8;
         expi8V = vld2q_f32(pc);
         pc += 8;
         

         i1 = j;         
         do
         {
            /*  index calculation for the input */
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;

            // r1 = pSrc[2 * i1]     + pSrc[2 * i5];
            // r5 = pSrc[2 * i1]     - pSrc[2 * i5];
            // s1 = pSrc[2 * i1 + 1] + pSrc[2 * i5 + 1];
            // s5 = pSrc[2 * i1 + 1] - pSrc[2 * i5 + 1];
            rs1V = vld2q_f32(pSrc+2 * i1);
            rs5V = vld2q_f32(pSrc+2 * i5);

            tmp         = vaddq_f32(rs1V.val[0],rs5V.val[0]);
            rs5V.val[0] = vsubq_f32(rs1V.val[0],rs5V.val[0]);
            rs1V.val[0] = tmp;

            tmp         = vaddq_f32(rs1V.val[1],rs5V.val[1]);
            rs5V.val[1] = vsubq_f32(rs1V.val[1],rs5V.val[1]);
            rs1V.val[1] = tmp;

            // r2 = pSrc[2 * i2] + pSrc[2 * i6]
            // r6 = pSrc[2 * i2] - pSrc[2 * i6]
            // s2 = pSrc[2 * i2 + 1] + pSrc[2 * i6 + 1];
            // s6 = pSrc[2 * i2 + 1] - pSrc[2 * i6 + 1];
            rs2V = vld2q_f32(pSrc+2 * i2);
            rs6V = vld2q_f32(pSrc+2 * i6);

            tmp         = vaddq_f32(rs2V.val[0],rs6V.val[0]);
            rs6V.val[0] = vsubq_f32(rs2V.val[0],rs6V.val[0]);
            rs2V.val[0] = tmp;

            tmp         = vaddq_f32(rs2V.val[1],rs6V.val[1]);
            rs6V.val[1] = vsubq_f32(rs2V.val[1],rs6V.val[1]);
            rs2V.val[1] = tmp;

            // r3 = pSrc[2 * i3] + pSrc[2 * i7];
            // r7 = pSrc[2 * i3] - pSrc[2 * i7];
            // s3 = pSrc[2 * i3 + 1] + pSrc[2 * i7 + 1];
            // s7 = pSrc[2 * i3 + 1] - pSrc[2 * i7 + 1];
            rs3V = vld2q_f32(pSrc+2 * i3);
            rs7V = vld2q_f32(pSrc+2 * i7);

            tmp         = vaddq_f32(rs3V.val[0],rs7V.val[0]);
            rs7V.val[0] = vsubq_f32(rs3V.val[0],rs7V.val[0]);
            rs3V.val[0] = tmp;

            tmp         = vaddq_f32(rs3V.val[1],rs7V.val[1]);
            rs7V.val[1] = vsubq_f32(rs3V.val[1],rs7V.val[1]);
            rs3V.val[1] = tmp;

            // r4 = pSrc[2 * i4] + pSrc[2 * i8];
            // r8 = pSrc[2 * i4] - pSrc[2 * i8];
            // s4 = pSrc[2 * i4 + 1] + pSrc[2 * i8 + 1];
            // s8 = pSrc[2 * i4 + 1] - pSrc[2 * i8 + 1];
            rs8V = vld2q_f32(pSrc+2 * i8);
            rs4V = vld2q_f32(pSrc+2 * i4);

            tmp         = vaddq_f32(rs4V.val[0],rs8V.val[0]);
            rs8V.val[0] = vsubq_f32(rs4V.val[0],rs8V.val[0]);
            rs4V.val[0] = tmp;

            tmp         = vaddq_f32(rs4V.val[1],rs8V.val[1]);
            rs8V.val[1] = vsubq_f32(rs4V.val[1],rs8V.val[1]);
            rs4V.val[1] = tmp;

            //t1 = r1 - r3;
            t1V           = vsubq_f32(rs1V.val[0] , rs3V.val[0]);
            //r1 = r1 + r3;
            rs1V.val[0]   = vaddq_f32(rs1V.val[0] , rs3V.val[0]);
            //r3 = r2 - r4;
            rs3V.val[0]   = vsubq_f32(rs2V.val[0] , rs4V.val[0]);
            //r2 = r2 + r4;
            rs2V.val[0]   = vaddq_f32(rs2V.val[0] , rs4V.val[0]);
            //pSrc[2 * i1] = r1 + r2;
            tmpRes.val[0] = vaddq_f32(rs1V.val[0] , rs2V.val[0]);
            //r2 = r1 - r2;
            rs2V.val[0]   = vsubq_f32(rs1V.val[0] , rs2V.val[0]);
            
            //t2 = s1 - s3;
            t2V           = vsubq_f32(rs1V.val[1] , rs3V.val[1]);
            //s1 = s1 + s3;
            rs1V.val[1]   = vaddq_f32(rs1V.val[1] , rs3V.val[1]);
            //s3 = s2 - s4;
            rs3V.val[1]   = vsubq_f32(rs2V.val[1] , rs4V.val[1]);
            //s2 = s2 + s4;
            rs2V.val[1]   = vaddq_f32(rs2V.val[1] , rs4V.val[1]);
            //r1 = t1 + s3;
            rs1V.val[0]   = vaddq_f32(t1V , rs3V.val[1]);
            //t1 = t1 - s3;
            t1V           = vsubq_f32(t1V , rs3V.val[1]);
            //pSrc[2 * i1 + 1] = s1 + s2;
            tmpRes.val[1] = vaddq_f32(rs1V.val[1] , rs2V.val[1]);
            vst2q_f32(pSrc+2*i1,tmpRes);
            //s2 = s1 - s2;
            rs2V.val[1] = vsubq_f32(rs1V.val[1] , rs2V.val[1]);
            //s1 = t2 - r3;
            rs1V.val[1] = vsubq_f32(t2V , rs3V.val[0]);
            //t2 = t2 + r3;
            t2V = vaddq_f32(t2V , rs3V.val[0]);
            //p1 = co5 * r2;
            //p2 = si5 * s2;
            //p3 = co5 * s2;
            //p4 = si5 * r2;
            p1V = vmulq_f32(expi5V.val[0] , rs2V.val[0]);
            p2V = vmulq_f32(expi5V.val[1] , rs2V.val[1]);
            p3V = vmulq_f32(expi5V.val[0] , rs2V.val[1]);
            p4V = vmulq_f32(expi5V.val[1] , rs2V.val[0]);
            //pSrc[2 * i5]     = p1 + p2;
            //pSrc[2 * i5 + 1] = p3 - p4;
            tmpRes.val[0] = vaddq_f32(p1V , p2V);
            tmpRes.val[1] = vsubq_f32(p3V , p4V);
            vst2q_f32(pSrc + 2*i5,tmpRes);
            //p1 = co3 * r1;
            //p2 = si3 * s1;
            //p3 = co3 * s1;
            //p4 = si3 * r1;
            p1V = vmulq_f32(expi3V.val[0] , rs1V.val[0]);
            p2V = vmulq_f32(expi3V.val[1] , rs1V.val[1]);
            p3V = vmulq_f32(expi3V.val[0] , rs1V.val[1]);
            p4V = vmulq_f32(expi3V.val[1] , rs1V.val[0]);
            //pSrc[2 * i3]     = p1 + p2;
            //pSrc[2 * i3 + 1] = p3 - p4;
            tmpRes.val[0]     = vaddq_f32(p1V , p2V);
            tmpRes.val[1]     = vsubq_f32(p3V , p4V);
            vst2q_f32(pSrc + 2*i3,tmpRes);
            //p1 = co7 * t1;
            //p2 = si7 * t2;
            //p3 = co7 * t2;
            //p4 = si7 * t1;
            p1V = vmulq_f32(expi7V.val[0] , t1V);
            p2V = vmulq_f32(expi7V.val[1] , t2V);
            p3V = vmulq_f32(expi7V.val[0] , t2V);
            p4V = vmulq_f32(expi7V.val[1] , t1V);
            //pSrc[2 * i7]     = p1 + p2;
            //pSrc[2 * i7 + 1] = p3 - p4;
            tmpRes.val[0]     = vaddq_f32(p1V , p2V);
            tmpRes.val[1]     = vsubq_f32(p3V , p4V);
            vst2q_f32(pSrc + 2*i7,tmpRes);
            //r1 = (r6 - r8) * C81;
            //r6 = (r6 + r8) * C81;
            //s1 = (s6 - s8) * C81;
            //s6 = (s6 + s8) * C81;
            rs1V.val[0] = vmulq_n_f32(vsubq_f32(rs6V.val[0] , rs8V.val[0]) , C81);
            rs6V.val[0] = vmulq_n_f32(vaddq_f32(rs6V.val[0] , rs8V.val[0]) , C81);
            rs1V.val[1] = vmulq_n_f32(vsubq_f32(rs6V.val[1] , rs8V.val[1]) , C81);
            rs6V.val[1] = vmulq_n_f32(vaddq_f32(rs6V.val[1] , rs8V.val[1]) , C81);
            //t1 = r5 - r1;
            t1V         = vsubq_f32(rs5V.val[0] , rs1V.val[0]);
            //r5 = r5 + r1;
            rs5V.val[0] = vaddq_f32(rs5V.val[0] , rs1V.val[0]);
            //r8 = r7 - r6;
            rs8V.val[0] = vsubq_f32(rs7V.val[0] , rs6V.val[0]);
            //r7 = r7 + r6;
            rs7V.val[0] = vaddq_f32(rs7V.val[0] , rs6V.val[0]);
            //t2 = s5 - s1;
            t2V         = vsubq_f32(rs5V.val[1] , rs1V.val[1]);
            //s5 = s5 + s1;
            rs5V.val[1] = vaddq_f32(rs5V.val[1] , rs1V.val[1]);
            //s8 = s7 - s6;
            rs8V.val[1] = vsubq_f32(rs7V.val[1] , rs6V.val[1]);
            //s7 = s7 + s6;
            rs7V.val[1] = vaddq_f32(rs7V.val[1] , rs6V.val[1]);
            //r1 = r5 + s7;
            rs1V.val[0] = vaddq_f32(rs5V.val[0] , rs7V.val[1]);
            //r5 = r5 - s7;
            rs5V.val[0] = vsubq_f32(rs5V.val[0] , rs7V.val[1]);
            //r6 = t1 + s8;
            rs6V.val[0] = vaddq_f32(t1V , rs8V.val[1]);
            //t1 = t1 - s8;
            t1V         = vsubq_f32(t1V , rs8V.val[1]);
            //s1 = s5 - r7;
            rs1V.val[1] = vsubq_f32(rs5V.val[1] , rs7V.val[0]);
            //s5 = s5 + r7;
            rs5V.val[1] = vaddq_f32(rs5V.val[1] , rs7V.val[0]);
            //s6 = t2 - r8;
            rs6V.val[1] = vsubq_f32(t2V , rs8V.val[0]);
            //t2 = t2 + r8;
            t2V         = vaddq_f32(t2V , rs8V.val[0]);
            //p1 = co2 * r1;
            //p2 = si2 * s1;
            //p3 = co2 * s1;
            //p4 = si2 * r1;
            p1V         = vmulq_f32(expi2V.val[0] , rs1V.val[0]);
            p2V         = vmulq_f32(expi2V.val[1] , rs1V.val[1]);
            p3V         = vmulq_f32(expi2V.val[0] , rs1V.val[1]);
            p4V         = vmulq_f32(expi2V.val[1] , rs1V.val[0]);
            //pSrc[2 * i2]     = p1 + p2;
            //pSrc[2 * i2 + 1] = p3 - p4;
            tmpRes.val[0] = vaddq_f32(p1V , p2V);
            tmpRes.val[1] = vsubq_f32(p3V , p4V);
            vst2q_f32(pSrc + 2*i2,tmpRes);
            //p1 = co8 * r5;
            //p2 = si8 * s5;
            //p3 = co8 * s5;
            //p4 = si8 * r5;
            p1V        = vmulq_f32(expi8V.val[0] , rs5V.val[0]);
            p2V        = vmulq_f32(expi8V.val[1] , rs5V.val[1]);
            p3V        = vmulq_f32(expi8V.val[0] , rs5V.val[1]);
            p4V        = vmulq_f32(expi8V.val[1] , rs5V.val[0]);
            //pSrc[2 * i8]     = p1 + p2;
            //pSrc[2 * i8 + 1] = p3 - p4;
            tmpRes.val[0] = vaddq_f32(p1V , p2V);
            tmpRes.val[1] = vsubq_f32(p3V , p4V);
            vst2q_f32(pSrc + 2*i8,tmpRes);
            //p1 = co6 * r6;
            //p2 = si6 * s6;
            //p3 = co6 * s6;
            //p4 = si6 * r6;
            p1V        = vmulq_f32(expi6V.val[0] , rs6V.val[0]);
            p2V        = vmulq_f32(expi6V.val[1] , rs6V.val[1]);
            p3V        = vmulq_f32(expi6V.val[0] , rs6V.val[1]);
            p4V        = vmulq_f32(expi6V.val[1] , rs6V.val[0]);
            //pSrc[2 * i6]     = p1 + p2;
            //pSrc[2 * i6 + 1] = p3 - p4;
            tmpRes.val[0] = vaddq_f32(p1V , p2V);
            tmpRes.val[1] = vsubq_f32(p3V , p4V);
            vst2q_f32(pSrc + 2*i6,tmpRes);
            //p1 = co4 * t1;
            //p2 = si4 * t2;
            //p3 = co4 * t2;
            //p4 = si4 * t1;
            p1V        = vmulq_f32(expi4V.val[0] , t1V);
            p2V        = vmulq_f32(expi4V.val[1] , t2V);
            p3V        = vmulq_f32(expi4V.val[0] , t2V);
            p4V        = vmulq_f32(expi4V.val[1] , t1V);
            //pSrc[2 * i4]     = p1 + p2;
            //pSrc[2 * i4 + 1] = p3 - p4;
            tmpRes.val[0]     = vaddq_f32(p1V , p2V);
            tmpRes.val[1] = vsubq_f32(p3V , p4V);
            vst2q_f32(pSrc + 2*i4,tmpRes);

            i1 += n1;
         } while (i1 < fftLen);


      };

      for(;j<n2;j++)
      {
         co2 = *pc++;
         si2 = *pc++;

         co3 = *pc++;
         si3 = *pc++;

         co4 = *pc++;
         si4 = *pc++;

         co5 = *pc++;
         si5 = *pc++;

         co6 = *pc++;
         si6 = *pc++;

         co7 = *pc++;
         si7 = *pc++;

         co8 = *pc++;
         si8 = *pc++;

         i1 = j;

         do
         {
            /*  index calculation for the input */
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = pSrc[2 * i1] + pSrc[2 * i5];
            r5 = pSrc[2 * i1] - pSrc[2 * i5];
            r2 = pSrc[2 * i2] + pSrc[2 * i6];
            r6 = pSrc[2 * i2] - pSrc[2 * i6];
            r3 = pSrc[2 * i3] + pSrc[2 * i7];
            r7 = pSrc[2 * i3] - pSrc[2 * i7];
            r4 = pSrc[2 * i4] + pSrc[2 * i8];
            r8 = pSrc[2 * i4] - pSrc[2 * i8];
            t1 = r1 - r3;
            r1 = r1 + r3;
            r3 = r2 - r4;
            r2 = r2 + r4;
            pSrc[2 * i1] = r1 + r2;
            r2 = r1 - r2;
            s1 = pSrc[2 * i1 + 1] + pSrc[2 * i5 + 1];
            s5 = pSrc[2 * i1 + 1] - pSrc[2 * i5 + 1];
            s2 = pSrc[2 * i2 + 1] + pSrc[2 * i6 + 1];
            s6 = pSrc[2 * i2 + 1] - pSrc[2 * i6 + 1];
            s3 = pSrc[2 * i3 + 1] + pSrc[2 * i7 + 1];
            s7 = pSrc[2 * i3 + 1] - pSrc[2 * i7 + 1];
            s4 = pSrc[2 * i4 + 1] + pSrc[2 * i8 + 1];
            s8 = pSrc[2 * i4 + 1] - pSrc[2 * i8 + 1];
            t2 = s1 - s3;
            s1 = s1 + s3;
            s3 = s2 - s4;
            s2 = s2 + s4;
            r1 = t1 + s3;
            t1 = t1 - s3;
            pSrc[2 * i1 + 1] = s1 + s2;
            s2 = s1 - s2;
            s1 = t2 - r3;
            t2 = t2 + r3;
            p1 = co5 * r2;
            p2 = si5 * s2;
            p3 = co5 * s2;
            p4 = si5 * r2;
            pSrc[2 * i5]     = p1 + p2;
            pSrc[2 * i5 + 1] = p3 - p4;
            p1 = co3 * r1;
            p2 = si3 * s1;
            p3 = co3 * s1;
            p4 = si3 * r1;
            pSrc[2 * i3]     = p1 + p2;
            pSrc[2 * i3 + 1] = p3 - p4;
            p1 = co7 * t1;
            p2 = si7 * t2;
            p3 = co7 * t2;
            p4 = si7 * t1;
            pSrc[2 * i7]     = p1 + p2;
            pSrc[2 * i7 + 1] = p3 - p4;
            r1 = (r6 - r8) * C81;
            r6 = (r6 + r8) * C81;
            s1 = (s6 - s8) * C81;
            s6 = (s6 + s8) * C81;
            t1 = r5 - r1;
            r5 = r5 + r1;
            r8 = r7 - r6;
            r7 = r7 + r6;
            t2 = s5 - s1;
            s5 = s5 + s1;
            s8 = s7 - s6;
            s7 = s7 + s6;
            r1 = r5 + s7;
            r5 = r5 - s7;
            r6 = t1 + s8;
            t1 = t1 - s8;
            s1 = s5 - r7;
            s5 = s5 + r7;
            s6 = t2 - r8;
            t2 = t2 + r8;
            p1 = co2 * r1;
            p2 = si2 * s1;
            p3 = co2 * s1;
            p4 = si2 * r1;
            pSrc[2 * i2]     = p1 + p2;
            pSrc[2 * i2 + 1] = p3 - p4;
            p1 = co8 * r5;
            p2 = si8 * s5;
            p3 = co8 * s5;
            p4 = si8 * r5;
            pSrc[2 * i8]     = p1 + p2;
            pSrc[2 * i8 + 1] = p3 - p4;
            p1 = co6 * r6;
            p2 = si6 * s6;
            p3 = co6 * s6;
            p4 = si6 * r6;
            pSrc[2 * i6]     = p1 + p2;
            pSrc[2 * i6 + 1] = p3 - p4;
            p1 = co4 * t1;
            p2 = si4 * t2;
            p3 = co4 * t2;
            p4 = si4 * t1;
            pSrc[2 * i4]     = p1 + p2;
            pSrc[2 * i4 + 1] = p3 - p4;

            i1 += n1;
         } while (i1 < fftLen);

        // ia1 += 7;


         //j++;
      } ;

      twidCoefModifier <<= 3;
   } while (n2 > 7);
}
#endif
#else
ARM_DSP_ATTRIBUTE void arm_radix8_butterfly_f32(
  float32_t * pSrc,
  uint16_t fftLen,
  const float32_t * pCoef,
  uint16_t twidCoefModifier)
{
   uint32_t ia1, ia2, ia3, ia4, ia5, ia6, ia7;
   uint32_t i1, i2, i3, i4, i5, i6, i7, i8;
   uint32_t id;
   uint32_t n1, n2, j;

   float32_t r1, r2, r3, r4, r5, r6, r7, r8;
   float32_t t1, t2;
   float32_t s1, s2, s3, s4, s5, s6, s7, s8;
   float32_t p1, p2, p3, p4;
   float32_t co2, co3, co4, co5, co6, co7, co8;
   float32_t si2, si3, si4, si5, si6, si7, si8;
   const float32_t C81 = 0.70710678118f;

   n2 = fftLen;

   do
   {
      n1 = n2;
      n2 = n2 >> 3;
      i1 = 0;

      do
      {
         i2 = i1 + n2;
         i3 = i2 + n2;
         i4 = i3 + n2;
         i5 = i4 + n2;
         i6 = i5 + n2;
         i7 = i6 + n2;
         i8 = i7 + n2;
         r1 = pSrc[2 * i1] + pSrc[2 * i5];
         r5 = pSrc[2 * i1] - pSrc[2 * i5];
         r2 = pSrc[2 * i2] + pSrc[2 * i6];
         r6 = pSrc[2 * i2] - pSrc[2 * i6];
         r3 = pSrc[2 * i3] + pSrc[2 * i7];
         r7 = pSrc[2 * i3] - pSrc[2 * i7];
         r4 = pSrc[2 * i4] + pSrc[2 * i8];
         r8 = pSrc[2 * i4] - pSrc[2 * i8];
         t1 = r1 - r3;
         r1 = r1 + r3;
         r3 = r2 - r4;
         r2 = r2 + r4;
         pSrc[2 * i1] = r1 + r2;
         pSrc[2 * i5] = r1 - r2;
         r1 = pSrc[2 * i1 + 1] + pSrc[2 * i5 + 1];
         s5 = pSrc[2 * i1 + 1] - pSrc[2 * i5 + 1];
         r2 = pSrc[2 * i2 + 1] + pSrc[2 * i6 + 1];
         s6 = pSrc[2 * i2 + 1] - pSrc[2 * i6 + 1];
         s3 = pSrc[2 * i3 + 1] + pSrc[2 * i7 + 1];
         s7 = pSrc[2 * i3 + 1] - pSrc[2 * i7 + 1];
         r4 = pSrc[2 * i4 + 1] + pSrc[2 * i8 + 1];
         s8 = pSrc[2 * i4 + 1] - pSrc[2 * i8 + 1];
         t2 = r1 - s3;
         r1 = r1 + s3;
         s3 = r2 - r4;
         r2 = r2 + r4;
         pSrc[2 * i1 + 1] = r1 + r2;
         pSrc[2 * i5 + 1] = r1 - r2;
         pSrc[2 * i3]     = t1 + s3;
         pSrc[2 * i7]     = t1 - s3;
         pSrc[2 * i3 + 1] = t2 - r3;
         pSrc[2 * i7 + 1] = t2 + r3;
         r1 = (r6 - r8) * C81;
         r6 = (r6 + r8) * C81;
         r2 = (s6 - s8) * C81;
         s6 = (s6 + s8) * C81;
         t1 = r5 - r1;
         r5 = r5 + r1;
         r8 = r7 - r6;
         r7 = r7 + r6;
         t2 = s5 - r2;
         s5 = s5 + r2;
         s8 = s7 - s6;
         s7 = s7 + s6;
         pSrc[2 * i2]     = r5 + s7;
         pSrc[2 * i8]     = r5 - s7;
         pSrc[2 * i6]     = t1 + s8;
         pSrc[2 * i4]     = t1 - s8;
         pSrc[2 * i2 + 1] = s5 - r7;
         pSrc[2 * i8 + 1] = s5 + r7;
         pSrc[2 * i6 + 1] = t2 - r8;
         pSrc[2 * i4 + 1] = t2 + r8;

         i1 += n1;
      } while (i1 < fftLen);

      if (n2 < 8)
         break;

      ia1 = 0;
      j = 1;

      do
      {
         /*  index calculation for the coefficients */
         id  = ia1 + twidCoefModifier;
         ia1 = id;
         ia2 = ia1 + id;
         ia3 = ia2 + id;
         ia4 = ia3 + id;
         ia5 = ia4 + id;
         ia6 = ia5 + id;
         ia7 = ia6 + id;

         co2 = pCoef[2 * ia1];
         co3 = pCoef[2 * ia2];
         co4 = pCoef[2 * ia3];
         co5 = pCoef[2 * ia4];
         co6 = pCoef[2 * ia5];
         co7 = pCoef[2 * ia6];
         co8 = pCoef[2 * ia7];
         si2 = pCoef[2 * ia1 + 1];
         si3 = pCoef[2 * ia2 + 1];
         si4 = pCoef[2 * ia3 + 1];
         si5 = pCoef[2 * ia4 + 1];
         si6 = pCoef[2 * ia5 + 1];
         si7 = pCoef[2 * ia6 + 1];
         si8 = pCoef[2 * ia7 + 1];

         i1 = j;

         do
         {
            /*  index calculation for the input */
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = pSrc[2 * i1] + pSrc[2 * i5];
            r5 = pSrc[2 * i1] - pSrc[2 * i5];
            r2 = pSrc[2 * i2] + pSrc[2 * i6];
            r6 = pSrc[2 * i2] - pSrc[2 * i6];
            r3 = pSrc[2 * i3] + pSrc[2 * i7];
            r7 = pSrc[2 * i3] - pSrc[2 * i7];
            r4 = pSrc[2 * i4] + pSrc[2 * i8];
            r8 = pSrc[2 * i4] - pSrc[2 * i8];
            t1 = r1 - r3;
            r1 = r1 + r3;
            r3 = r2 - r4;
            r2 = r2 + r4;
            pSrc[2 * i1] = r1 + r2;
            r2 = r1 - r2;
            s1 = pSrc[2 * i1 + 1] + pSrc[2 * i5 + 1];
            s5 = pSrc[2 * i1 + 1] - pSrc[2 * i5 + 1];
            s2 = pSrc[2 * i2 + 1] + pSrc[2 * i6 + 1];
            s6 = pSrc[2 * i2 + 1] - pSrc[2 * i6 + 1];
            s3 = pSrc[2 * i3 + 1] + pSrc[2 * i7 + 1];
            s7 = pSrc[2 * i3 + 1] - pSrc[2 * i7 + 1];
            s4 = pSrc[2 * i4 + 1] + pSrc[2 * i8 + 1];
            s8 = pSrc[2 * i4 + 1] - pSrc[2 * i8 + 1];
            t2 = s1 - s3;
            s1 = s1 + s3;
            s3 = s2 - s4;
            s2 = s2 + s4;
            r1 = t1 + s3;
            t1 = t1 - s3;
            pSrc[2 * i1 + 1] = s1 + s2;
            s2 = s1 - s2;
            s1 = t2 - r3;
            t2 = t2 + r3;
            p1 = co5 * r2;
            p2 = si5 * s2;
            p3 = co5 * s2;
            p4 = si5 * r2;
            pSrc[2 * i5]     = p1 + p2;
            pSrc[2 * i5 + 1] = p3 - p4;
            p1 = co3 * r1;
            p2 = si3 * s1;
            p3 = co3 * s1;
            p4 = si3 * r1;
            pSrc[2 * i3]     = p1 + p2;
            pSrc[2 * i3 + 1] = p3 - p4;
            p1 = co7 * t1;
            p2 = si7 * t2;
            p3 = co7 * t2;
            p4 = si7 * t1;
            pSrc[2 * i7]     = p1 + p2;
            pSrc[2 * i7 + 1] = p3 - p4;
            r1 = (r6 - r8) * C81;
            r6 = (r6 + r8) * C81;
            s1 = (s6 - s8) * C81;
            s6 = (s6 + s8) * C81;
            t1 = r5 - r1;
            r5 = r5 + r1;
            r8 = r7 - r6;
            r7 = r7 + r6;
            t2 = s5 - s1;
            s5 = s5 + s1;
            s8 = s7 - s6;
            s7 = s7 + s6;
            r1 = r5 + s7;
            r5 = r5 - s7;
            r6 = t1 + s8;
            t1 = t1 - s8;
            s1 = s5 - r7;
            s5 = s5 + r7;
            s6 = t2 - r8;
            t2 = t2 + r8;
            p1 = co2 * r1;
            p2 = si2 * s1;
            p3 = co2 * s1;
            p4 = si2 * r1;
            pSrc[2 * i2]     = p1 + p2;
            pSrc[2 * i2 + 1] = p3 - p4;
            p1 = co8 * r5;
            p2 = si8 * s5;
            p3 = co8 * s5;
            p4 = si8 * r5;
            pSrc[2 * i8]     = p1 + p2;
            pSrc[2 * i8 + 1] = p3 - p4;
            p1 = co6 * r6;
            p2 = si6 * s6;
            p3 = co6 * s6;
            p4 = si6 * r6;
            pSrc[2 * i6]     = p1 + p2;
            pSrc[2 * i6 + 1] = p3 - p4;
            p1 = co4 * t1;
            p2 = si4 * t2;
            p3 = co4 * t2;
            p4 = si4 * t1;
            pSrc[2 * i4]     = p1 + p2;
            pSrc[2 * i4 + 1] = p3 - p4;

            i1 += n1;
         } while (i1 < fftLen);

         j++;
      } while (j < n2);

      twidCoefModifier <<= 3;
   } while (n2 > 7);
}
#endif