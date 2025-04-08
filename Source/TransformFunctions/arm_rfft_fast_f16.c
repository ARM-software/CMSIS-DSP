/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_fast_f16.c
 * Description:  RFFT & RIFFT Floating point process function
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

#include "dsp/transform_functions_f16.h"
#include "arm_common_tables_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)


#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

static void stage_rfft_f16(
  const arm_rfft_fast_instance_f16 * S,
  const float16_t * p,
        float16_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float16_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float16_t * pCoeff = S->pTwiddleRFFT;       /* Points to RFFT Twiddle factors */
  const float16_t *pA = p;                          /* increasing pointer */
  const float16_t *pB = p;                          /* decreasing pointer */
        float16_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float16_t t1a, t1b;                         /* temporary variables */
        float16_t p0, p1, p2, p3;                   /* temporary variables */

        float16x8x2_t tw,xA,xB;
        float16x8x2_t tmp1, tmp2, res;

        uint16x8_t     vecStridesBkwd;

        vecStridesBkwd = vddupq_u16((uint16_t)14, 2);


        int blockCnt;


   k = (S->Sint).fftLen - 1;

   /* Pack first and last sample of the frequency domain together */

   xBR = pB[0];
   xBI = pB[1];
   xAR = pA[0];
   xAI = pA[1];

   twR = *pCoeff++ ;
   twI = *pCoeff++ ;

   // U1 = XA(1) + XB(1); % It is real
   t1a = (_Float16)xBR + (_Float16)xAR  ;

   // U2 = XB(1) - XA(1); % It is imaginary
   t1b = (_Float16)xBI + (_Float16)xAI  ;

   // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
   // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
   *pOut++ = 0.5f16 * ( (_Float16)t1a + (_Float16)t1b );
   *pOut++ = 0.5f16 * ( (_Float16)t1a - (_Float16)t1b );

   // XA(1) = 1/2*( U1 - imag(U2) +  i*( U1 +imag(U2) ));
   pB  = p + 2*k - 14;
   pA += 2;

   blockCnt = k >> 3;
   while (blockCnt > 0)
   {
      /*
         function X = my_split_rfft(X, ifftFlag)
         % X is a series of real numbers
         L  = length(X);
         XC = X(1:2:end) +i*X(2:2:end);
         XA = fft(XC);
         XB = conj(XA([1 end:-1:2]));
         TW = i*exp(-2*pi*i*[0:L/2-1]/L).';
         for l = 2:L/2
            XA(l) = 1/2 * (XA(l) + XB(l) + TW(l) * (XB(l) - XA(l)));
         end
         XA(1) = 1/2* (XA(1) + XB(1) + TW(1) * (XB(1) - XA(1))) + i*( 1/2*( XA(1) + XB(1) + i*( XA(1) - XB(1))));
         X = XA;
      */


      xA = vld2q_f16(pA);
      pA += 16;

      xB = vld2q_f16(pB);

      xB.val[0] = vldrhq_gather_shifted_offset_f16(pB, vecStridesBkwd);
      xB.val[1] = vldrhq_gather_shifted_offset_f16(&pB[1], vecStridesBkwd);

      xB.val[1] = vnegq_f16(xB.val[1]);
      pB -= 16;


      tw = vld2q_f16(pCoeff);
      pCoeff += 16;


      tmp1.val[0] = vaddq_f16(xA.val[0],xB.val[0]);
      tmp1.val[1] = vaddq_f16(xA.val[1],xB.val[1]);

      tmp2.val[0] = vsubq_f16(xB.val[0],xA.val[0]);
      tmp2.val[1] = vsubq_f16(xB.val[1],xA.val[1]);

      res.val[0] = vmulq(tw.val[0], tmp2.val[0]);
      res.val[0] = vfmsq(res.val[0],tw.val[1], tmp2.val[1]);

      res.val[1] = vmulq(tw.val[0], tmp2.val[1]);
      res.val[1] = vfmaq(res.val[1], tw.val[1], tmp2.val[0]);

      res.val[0] = vaddq_f16(res.val[0],tmp1.val[0] );
      res.val[1] = vaddq_f16(res.val[1],tmp1.val[1] );

      res.val[0] = vmulq_n_f16(res.val[0], 0.5f);
      res.val[1] = vmulq_n_f16(res.val[1], 0.5f);


      vst2q_f16(pOut, res);
      pOut += 16;

    
      blockCnt--;
   } 

   pB += 14;
   blockCnt = k & 7;
   while (blockCnt > 0)
   {
      /*
         function X = my_split_rfft(X, ifftFlag)
         % X is a series of real numbers
         L  = length(X);
         XC = X(1:2:end) +i*X(2:2:end);
         XA = fft(XC);
         XB = conj(XA([1 end:-1:2]));
         TW = i*exp(-2*pi*i*[0:L/2-1]/L).';
         for l = 2:L/2
            XA(l) = 1/2 * (XA(l) + XB(l) + TW(l) * (XB(l) - XA(l)));
         end
         XA(1) = 1/2* (XA(1) + XB(1) + TW(1) * (XB(1) - XA(1))) + i*( 1/2*( XA(1) + XB(1) + i*( XA(1) - XB(1))));
         X = XA;
      */

      xBI = pB[1];
      xBR = pB[0];
      xAR = pA[0];
      xAI = pA[1];

      twR = *pCoeff++;
      twI = *pCoeff++;

      t1a = (_Float16)xBR - (_Float16)xAR ;
      t1b = (_Float16)xBI + (_Float16)xAI ;

      // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
      // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
      p0 = (_Float16)twR * (_Float16)t1a;
      p1 = (_Float16)twI * (_Float16)t1a;
      p2 = (_Float16)twR * (_Float16)t1b;
      p3 = (_Float16)twI * (_Float16)t1b;

      *pOut++ = 0.5f16 * ((_Float16)xAR + (_Float16)xBR + (_Float16)p0 + (_Float16)p3 ); //xAR
      *pOut++ = 0.5f16 * ((_Float16)xAI - (_Float16)xBI + (_Float16)p1 - (_Float16)p2 ); //xAI

      pA += 2;
      pB -= 2;
      blockCnt--;
   }
}

/* Prepares data for inverse cfft */
static void merge_rfft_f16(
  const arm_rfft_fast_instance_f16 * S,
  const float16_t * p,
        float16_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float16_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float16_t *pCoeff = S->pTwiddleRFFT;        /* Points to RFFT Twiddle factors */
  const float16_t *pA = p;                          /* increasing pointer */
  const float16_t *pB = p;                          /* decreasing pointer */
        float16_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float16_t t1a, t1b, r, s, t, u;             /* temporary variables */

        float16x8x2_t tw,xA,xB;
        float16x8x2_t tmp1, tmp2, res;
        uint16x8_t     vecStridesBkwd;

        vecStridesBkwd = vddupq_u16((uint16_t)14, 2);

        int blockCnt;
        

   k = (S->Sint).fftLen - 1;

   xAR = pA[0];
   xAI = pA[1];

   pCoeff += 2 ;

   *pOut++ = 0.5f16 * ( (_Float16)xAR + (_Float16)xAI );
   *pOut++ = 0.5f16 * ( (_Float16)xAR - (_Float16)xAI );

   pB  =  p + 2*k - 14;
   pA +=  2    ;

   blockCnt = k >> 3;
   while (blockCnt > 0)
   {
      /* G is half of the frequency complex spectrum */
      //for k = 2:N
      //    Xk(k) = 1/2 * (G(k) + conj(G(N-k+2)) + Tw(k)*( G(k) - conj(G(N-k+2))));
      xA = vld2q_f16(pA);
      pA += 16;

      xB = vld2q_f16(pB);

      xB.val[0] = vldrhq_gather_shifted_offset_f16(pB, vecStridesBkwd);
      xB.val[1] = vldrhq_gather_shifted_offset_f16(&pB[1], vecStridesBkwd);

      xB.val[1] = vnegq_f16(xB.val[1]);
      pB -= 16;


      tw = vld2q_f16(pCoeff);
      tw.val[1] = vnegq_f16(tw.val[1]);
      pCoeff += 16;


      tmp1.val[0] = vaddq_f16(xA.val[0],xB.val[0]);
      tmp1.val[1] = vaddq_f16(xA.val[1],xB.val[1]);

      tmp2.val[0] = vsubq_f16(xB.val[0],xA.val[0]);
      tmp2.val[1] = vsubq_f16(xB.val[1],xA.val[1]);

      res.val[0] = vmulq(tw.val[0], tmp2.val[0]);
      res.val[0] = vfmsq(res.val[0],tw.val[1], tmp2.val[1]);

      res.val[1] = vmulq(tw.val[0], tmp2.val[1]);
      res.val[1] = vfmaq(res.val[1], tw.val[1], tmp2.val[0]);

      res.val[0] = vaddq_f16(res.val[0],tmp1.val[0] );
      res.val[1] = vaddq_f16(res.val[1],tmp1.val[1] );

      res.val[0] = vmulq_n_f16(res.val[0], 0.5f);
      res.val[1] = vmulq_n_f16(res.val[1], 0.5f);


      vst2q_f16(pOut, res);
      pOut += 16;

    
      blockCnt--;
   }

   pB += 14;
   blockCnt = k & 7;
   while (blockCnt > 0)
   {
      /* G is half of the frequency complex spectrum */
      //for k = 2:N
      //    Xk(k) = 1/2 * (G(k) + conj(G(N-k+2)) + Tw(k)*( G(k) - conj(G(N-k+2))));
      xBI =   pB[1]    ;
      xBR =   pB[0]    ;
      xAR =  pA[0];
      xAI =  pA[1];

      twR = *pCoeff++;
      twI = *pCoeff++;

      t1a = (_Float16)xAR - (_Float16)xBR ;
      t1b = (_Float16)xAI + (_Float16)xBI ;

      r = (_Float16)twR * (_Float16)t1a;
      s = (_Float16)twI * (_Float16)t1b;
      t = (_Float16)twI * (_Float16)t1a;
      u = (_Float16)twR * (_Float16)t1b;

      // real(tw * (xA - xB)) = twR * (xAR - xBR) - twI * (xAI - xBI);
      // imag(tw * (xA - xB)) = twI * (xAR - xBR) + twR * (xAI - xBI);
      *pOut++ = 0.5f16 * ((_Float16)xAR + (_Float16)xBR - (_Float16)r - (_Float16)s ); //xAR
      *pOut++ = 0.5f16 * ((_Float16)xAI - (_Float16)xBI + (_Float16)t - (_Float16)u ); //xAI

      pA += 2;
      pB -= 2;
      blockCnt--;
   }

}
#elif defined(ARM_MATH_NEON_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)
/*

No stage merge functions defined here for Neon.

*/
#else
static void stage_rfft_f16(
  const arm_rfft_fast_instance_f16 * S,
  const float16_t * p,
        float16_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float16_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float16_t * pCoeff = S->pTwiddleRFFT;       /* Points to RFFT Twiddle factors */
  const float16_t *pA = p;                          /* increasing pointer */
  const float16_t *pB = p;                          /* decreasing pointer */
        float16_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float16_t t1a, t1b;                         /* temporary variables */
        float16_t p0, p1, p2, p3;                   /* temporary variables */


   k = (S->Sint).fftLen - 1;

   /* Pack first and last sample of the frequency domain together */

   xBR = pB[0];
   xBI = pB[1];
   xAR = pA[0];
   xAI = pA[1];

   twR = *pCoeff++ ;
   twI = *pCoeff++ ;


   // U1 = XA(1) + XB(1); % It is real
   t1a = (_Float16)xBR + (_Float16)xAR  ;

   // U2 = XB(1) - XA(1); % It is imaginary
   t1b = (_Float16)xBI + (_Float16)xAI  ;

   // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
   // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
   *pOut++ = 0.5f16 * ( (_Float16)t1a + (_Float16)t1b );
   *pOut++ = 0.5f16 * ( (_Float16)t1a - (_Float16)t1b );

   // XA(1) = 1/2*( U1 - imag(U2) +  i*( U1 +imag(U2) ));
   pB  = p + 2*k;
   pA += 2;

   do
   {
      /*
         function X = my_split_rfft(X, ifftFlag)
         % X is a series of real numbers
         L  = length(X);
         XC = X(1:2:end) +i*X(2:2:end);
         XA = fft(XC);
         XB = conj(XA([1 end:-1:2]));
         TW = i*exp(-2*pi*i*[0:L/2-1]/L).';
         for l = 2:L/2
            XA(l) = 1/2 * (XA(l) + XB(l) + TW(l) * (XB(l) - XA(l)));
         end
         XA(1) = 1/2* (XA(1) + XB(1) + TW(1) * (XB(1) - XA(1))) + i*( 1/2*( XA(1) + XB(1) + i*( XA(1) - XB(1))));
         X = XA;
      */

      xBI = pB[1];
      xBR = pB[0];
      xAR = pA[0];
      xAI = pA[1];

      twR = *pCoeff++;
      twI = *pCoeff++;

      t1a = (_Float16)xBR - (_Float16)xAR ;
      t1b = (_Float16)xBI + (_Float16)xAI ;

      // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
      // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
      p0 = (_Float16)twR * (_Float16)t1a;
      p1 = (_Float16)twI * (_Float16)t1a;
      p2 = (_Float16)twR * (_Float16)t1b;
      p3 = (_Float16)twI * (_Float16)t1b;

      *pOut++ = 0.5f16 * ((_Float16)xAR + (_Float16)xBR + (_Float16)p0 + (_Float16)p3 ); //xAR
      *pOut++ = 0.5f16 * ((_Float16)xAI - (_Float16)xBI + (_Float16)p1 - (_Float16)p2 ); //xAI


      pA += 2;
      pB -= 2;
      k--;
   } while (k > 0);
}

/* Prepares data for inverse cfft */
static void merge_rfft_f16(
  const arm_rfft_fast_instance_f16 * S,
  const float16_t * p,
        float16_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float16_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float16_t *pCoeff = S->pTwiddleRFFT;        /* Points to RFFT Twiddle factors */
  const float16_t *pA = p;                          /* increasing pointer */
  const float16_t *pB = p;                          /* decreasing pointer */
        float16_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float16_t t1a, t1b, r, s, t, u;             /* temporary variables */

   k = (S->Sint).fftLen - 1;

   xAR = pA[0];
   xAI = pA[1];

   pCoeff += 2 ;

   *pOut++ = 0.5f16 * ( (_Float16)xAR + (_Float16)xAI );
   *pOut++ = 0.5f16 * ( (_Float16)xAR - (_Float16)xAI );

   pB  =  p + 2*k ;
   pA +=  2	   ;

   while (k > 0)
   {
      /* G is half of the frequency complex spectrum */
      //for k = 2:N
      //    Xk(k) = 1/2 * (G(k) + conj(G(N-k+2)) + Tw(k)*( G(k) - conj(G(N-k+2))));
      xBI =   pB[1]    ;
      xBR =   pB[0]    ;
      xAR =  pA[0];
      xAI =  pA[1];

      twR = *pCoeff++;
      twI = *pCoeff++;

      t1a = (_Float16)xAR - (_Float16)xBR ;
      t1b = (_Float16)xAI + (_Float16)xBI ;

      r = (_Float16)twR * (_Float16)t1a;
      s = (_Float16)twI * (_Float16)t1b;
      t = (_Float16)twI * (_Float16)t1a;
      u = (_Float16)twR * (_Float16)t1b;

      // real(tw * (xA - xB)) = twR * (xAR - xBR) - twI * (xAI - xBI);
      // imag(tw * (xA - xB)) = twI * (xAR - xBR) + twR * (xAI - xBI);
      *pOut++ = 0.5f16 * ((_Float16)xAR + (_Float16)xBR - (_Float16)r - (_Float16)s ); //xAR
      *pOut++ = 0.5f16 * ((_Float16)xAI - (_Float16)xBI + (_Float16)t - (_Float16)u ); //xAI

      pA += 2;
      pB -= 2;
      k--;
   }

}

#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
  @ingroup RealFFT
*/

/**
  @defgroup RealFFTF16 Real FFT F16 Functions
*/

/**
  @addtogroup RealFFTF16
  @{
*/

/**
  @brief         Processing function for the floating-point real FFT.
  @param[in]     S         points to an arm_rfft_fast_instance_f16 structure
  @param[in]     p         points to input buffer (Source buffer is modified by this function.)
  @param[in]     pOut      points to output buffer
  @param[in]     ifftFlag
                   - value = 0: RFFT
                   - value = 1: RIFFT

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
*/
#if defined(ARM_MATH_NEON_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_fft.h"

ARM_DSP_ATTRIBUTE void arm_rfft_fast_f16(
  const arm_rfft_fast_instance_f16 * S,
  const float16_t * p,
  float16_t * pOut,
  float16_t *tmpbuf,
  uint8_t ifftFlag)
{
/* Calculation of Real FFT */
   if (!ifftFlag)
   {
     arm_ne10_fft_r2c_1d_float16_neon (S,p,pOut,tmpbuf);
   }
   else 
   {
     arm_ne10_fft_c2r_1d_float16_neon (S,p,pOut,tmpbuf);
   }
}

#else
ARM_DSP_ATTRIBUTE void arm_rfft_fast_f16(
  const arm_rfft_fast_instance_f16 * S,
  float16_t * p,
  float16_t * pOut,
  uint8_t ifftFlag)
{
   const arm_cfft_instance_f16 * Sint = &(S->Sint);


   /* Calculation of Real FFT */
   if (ifftFlag)
   {
      /*  Real FFT compression */
      merge_rfft_f16(S, p, pOut);
      /* Complex radix-4 IFFT process */
      arm_cfft_f16( Sint, pOut, ifftFlag, 1);
   }
   else
   {

      /* Calculation of RFFT of input */
      arm_cfft_f16( Sint, p, ifftFlag, 1);

      /*  Real FFT extraction */
      stage_rfft_f16(S, p, pOut);
   }
}
#endif
/**
* @} end of RealFFTF16 group
*/

#endif /*  #if defined(ARM_FLOAT16_SUPPORTED) */
