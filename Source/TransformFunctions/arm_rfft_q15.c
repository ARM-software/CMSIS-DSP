/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_q15.c
 * Description:  RFFT & RIFFT Q15 process function
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
 * Internal functions prototypes
 * -------------------------------------------------------------------- */

void arm_split_rfft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier);

void arm_split_rifft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier);

/**
  @addtogroup RealFFTQ15
  @{
 */

/**
  @brief         Processing function for the Q15 RFFT/RIFFT.
  @param[in]     S     points to an instance of the Q15 RFFT/RIFFT structure
  @param[in]     pSrc  points to input buffer (Source buffer is modified by this function.)
  @param[out]    pDst  points to output buffer
  @return        none

  @par           Input an output formats
                   Internally input is downscaled by 2 for every stage to avoid saturations inside CFFT/CIFFT process.
                   Hence the output format is different for different RFFT sizes.
                   The input and output formats for different RFFT sizes and number of bits to upscale are mentioned in the tables below for RFFT and RIFFT:
  @par             Input and Output formats for RFFT Q15

| RFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ---------: | ------------: | -------------: | ------------------------: |
| 32         | 1.15          | 6.10           | 5                         |
| 64         | 1.15          | 7.9            | 6                         |
| 128        | 1.15          | 8.8            | 7                         |
| 256        | 1.15          | 9.7            | 8                         |
| 512        | 1.15          | 10.6           | 9                         |
| 1024       | 1.15          | 11.5           | 10                        |
| 2048       | 1.15          | 12.4           | 11                        |
| 4096       | 1.15          | 13.3           | 12                        |
| 8192       | 1.15          | 14.2           | 13                        |
             
  @par             Input and Output formats for RIFFT Q15

| RIFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ----------: | ------------: | -------------: | ------------------------: |
| 32          | 1.15          | 6.10           | 0                         |
| 64          | 1.15          | 7.9            | 0                         |
| 128         | 1.15          | 8.8            | 0                         |
| 256         | 1.15          | 9.7            | 0                         |
| 512         | 1.15          | 10.6           | 0                         |
| 1024        | 1.15          | 11.5           | 0                         |
| 2048        | 1.15          | 12.4           | 0                         |
| 4096        | 1.15          | 13.3           | 0                         |
| 8192        | 1.15          | 14.2           | 0                         |
  
  @par
                   If the input buffer is of length N (fftLenReal), the output buffer must have length 2N
                   since it is containing the conjugate part (except for MVE version where N+2 is enough).
                   The input buffer is modified by this function.
  @par
                   For the RIFFT, the source buffer must have length N+2 since the Nyquist frequency value
                   is needed but conjugate part is ignored. 
                   It is not using the packing trick of the float version.
 */

void arm_rfft_q15(
  const arm_rfft_instance_q15 * S,
        q15_t * pSrc,
        q15_t * pDst)
{
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
  const arm_cfft_instance_q15 *S_CFFT = &(S->cfftInst);
#else
  const arm_cfft_instance_q15 *S_CFFT = S->pCfft;
#endif
        uint32_t L2 = S->fftLenReal >> 1U;

  /* Calculation of RIFFT of input */
  if (S->ifftFlagR == 1U)
  {
     /*  Real IFFT core process */
     arm_split_rifft_q15 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);

     /* Complex IFFT process */
     arm_cfft_q15 (S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

     arm_shift_q15(pDst, 1, pDst, S->fftLenReal);
  }
  else
  {
     /* Calculation of RFFT of input */

     /* Complex FFT process */
     arm_cfft_q15 (S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

     /*  Real FFT core process */
     arm_split_rfft_q15 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
  }

}

/**
  @} end of RealFFTQ15 group
 */

/**
  @brief         Core Real FFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
  @return        none

  @par
                   The function implements a Real FFT
 */

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"
#include "arm_vec_fft.h"

#if defined(__CMSIS_GCC_H)
#define MVE_CMPLX_MULT_FX_AxB_S16(A,B)          vqdmladhxq_s16(vqdmlsdhq_s16((__typeof(A))vuninitializedq_s16(), A, B), A, B)
#define MVE_CMPLX_MULT_FX_AxConjB_S16(A,B)      vqdmladhq_s16(vqdmlsdhxq_s16((__typeof(A))vuninitializedq_s16(), A, B), A, B)

#endif 

void arm_split_rfft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{
   uint32_t        i;          /* Loop Counter */
    const q15_t    *pCoefA, *pCoefB;    /* Temporary pointers for twiddle factors */
    q15_t          *pOut1 = &pDst[2];
    q15_t          *pIn1 = &pSrc[2];
    uint16x8_t      offsetIn = { 6, 7, 4, 5, 2, 3, 0, 1 };
    uint16x8_t      offsetCoef;
    const uint16_t  offsetCoefArr[16] = {
        0, 0, 2, 2, 4, 4, 6, 6,
        0, 1, 0, 1, 0, 1, 0, 1
    };

    offsetCoef = vmulq_n_u16(vld1q_u16(offsetCoefArr), modifier) + vld1q_u16(offsetCoefArr + 8);
    offsetIn = vaddq_n_u16(offsetIn, (2 * fftLen - 8));

    /* Init coefficient pointers */
    pCoefA = &pATable[modifier * 2];
    pCoefB = &pBTable[modifier * 2];

    const q15_t    *pCoefAb, *pCoefBb;
    pCoefAb = pCoefA;
    pCoefBb = pCoefB;

    pIn1 = &pSrc[2];

    i = fftLen - 1U;
    i = i / 4 + 1;
    while (i > 0U) {
        q15x8_t         in1 = vld1q_s16(pIn1);
        q15x8_t         in2 = vldrhq_gather_shifted_offset_s16(pSrc, offsetIn);
        q15x8_t         coefA = vldrhq_gather_shifted_offset_s16(pCoefAb, offsetCoef);
        q15x8_t         coefB = vldrhq_gather_shifted_offset_s16(pCoefBb, offsetCoef);

#if defined(__CMSIS_GCC_H)
        q15x8_t         out = vhaddq_s16(MVE_CMPLX_MULT_FX_AxB_S16(in1, coefA),
                                     MVE_CMPLX_MULT_FX_AxConjB_S16(coefB, in2));
#else
        q15x8_t         out = vhaddq_s16(MVE_CMPLX_MULT_FX_AxB(in1, coefA, q15x8_t),
                                         MVE_CMPLX_MULT_FX_AxConjB(coefB, in2, q15x8_t));
#endif
        vst1q_s16(pOut1, out);
        pOut1 += 8;

        offsetCoef = vaddq_n_u16(offsetCoef, modifier * 8);
        offsetIn -= 8;
        pIn1 += 8;
        i -= 1;
    }

    pDst[2 * fftLen] = (pSrc[0] - pSrc[1]) >> 1U;
    pDst[2 * fftLen + 1] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1U;
    pDst[1] = 0;
}
#else
void arm_split_rfft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{       
        uint32_t i;                                    /* Loop Counter */
        q31_t outR, outI;                              /* Temporary variables for output */
  const q15_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q15_t *pSrc1, *pSrc2;
#if defined (ARM_MATH_DSP)
        q15_t *pD1, *pD2;
#endif

  /* Init coefficient pointers */
  pCoefA = &pATable[modifier * 2];
  pCoefB = &pBTable[modifier * 2];

  pSrc1 = &pSrc[2];
  pSrc2 = &pSrc[(2U * fftLen) - 2U];

#if defined (ARM_MATH_DSP)

    i = 1U;
    pD1 = pDst + 2;
    pD2 = pDst + (4U * fftLen) - 2;

    for (i = fftLen - 1; i > 0; i--)
    {
        /*
          outR = (  pSrc[2 * i]             * pATable[2 * i]
                  - pSrc[2 * i + 1]         * pATable[2 * i + 1]
                  + pSrc[2 * n - 2 * i]     * pBTable[2 * i]
                  + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

          outI = (  pIn[2 * i + 1]         * pATable[2 * i]
                  + pIn[2 * i]             * pATable[2 * i + 1]
                  + pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
                  - pIn[2 * n - 2 * i + 1] * pBTable[2 * i])
         */


#ifndef ARM_MATH_BIG_ENDIAN
        /* pSrc[2 * i] * pATable[2 * i] - pSrc[2 * i + 1] * pATable[2 * i + 1] */
        outR = __SMUSD(read_q15x2 (pSrc1), read_q15x2((q15_t *) pCoefA));
#else
        /* -(pSrc[2 * i + 1] * pATable[2 * i + 1] - pSrc[2 * i] * pATable[2 * i]) */
        outR = -(__SMUSD(read_q15x2 (pSrc1), read_q15x2((q15_t *) pCoefA)));
#endif /* #ifndef ARM_MATH_BIG_ENDIAN */

        /* pSrc[2 * n - 2 * i] * pBTable[2 * i] + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]) */
        outR = __SMLAD(read_q15x2 (pSrc2), read_q15x2((q15_t *) pCoefB), outR) >> 16U;

        /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] - pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
#ifndef ARM_MATH_BIG_ENDIAN
        outI = __SMUSDX(read_q15x2_da (&pSrc2), read_q15x2((q15_t *) pCoefB));
#else
        outI = __SMUSDX(read_q15x2 ((q15_t *) pCoefB), read_q15x2_da (&pSrc2));
#endif /* #ifndef ARM_MATH_BIG_ENDIAN */

        /* (pIn[2 * i + 1] * pATable[2 * i] + pIn[2 * i] * pATable[2 * i + 1] */
        outI = __SMLADX(read_q15x2_ia (&pSrc1), read_q15x2 ((q15_t *) pCoefA), outI);

        /* write output */
        *pD1++ = (q15_t) outR;
        *pD1++ = outI >> 16U;

        /* write complex conjugate output */
        pD2[0] = (q15_t) outR;
        pD2[1] = -(outI >> 16U);
        pD2 -= 2;

        /* update coefficient pointer */
        pCoefB = pCoefB + (2U * modifier);
        pCoefA = pCoefA + (2U * modifier);
    }

    pDst[2U * fftLen]      = (pSrc[0] - pSrc[1]) >> 1U;
    pDst[2U * fftLen + 1U] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1U;
    pDst[1] = 0;

#else

    i = 1U;

    while (i < fftLen)
    {
        /*
          outR = (  pSrc[2 * i]             * pATable[2 * i]
                  - pSrc[2 * i + 1]         * pATable[2 * i + 1]
                  + pSrc[2 * n - 2 * i]     * pBTable[2 * i]
                  + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);
        */

        outR = *pSrc1 * *pCoefA;
        outR = outR - (*(pSrc1 + 1) * *(pCoefA + 1));
        outR = outR + (*pSrc2 * *pCoefB);
        outR = (outR + (*(pSrc2 + 1) * *(pCoefB + 1))) >> 16;

        /*
          outI = (  pIn[2 * i + 1]         * pATable[2 * i]
                  + pIn[2 * i]             * pATable[2 * i + 1]
                  + pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
                  - pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
        */

        outI = *pSrc2 * *(pCoefB + 1);
        outI = outI - (*(pSrc2 + 1) * *pCoefB);
        outI = outI + (*(pSrc1 + 1) * *pCoefA);
        outI = outI + (*pSrc1 * *(pCoefA + 1));

        /* update input pointers */
        pSrc1 += 2U;
        pSrc2 -= 2U;

        /* write output */
        pDst[2U * i] = (q15_t) outR;
        pDst[2U * i + 1U] = outI >> 16U;

        /* write complex conjugate output */
        pDst[(4U * fftLen) - (2U * i)] = (q15_t) outR;
        pDst[((4U * fftLen) - (2U * i)) + 1U] = -(outI >> 16U);

        /* update coefficient pointer */
        pCoefB = pCoefB + (2U * modifier);
        pCoefA = pCoefA + (2U * modifier);

        i++;
    }

    pDst[2U * fftLen] = (pSrc[0] - pSrc[1]) >> 1;
    pDst[2U * fftLen + 1U] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1;
    pDst[1] = 0;

#endif /* #if defined (ARM_MATH_DSP) */
}
#endif /* defined(ARM_MATH_MVEI) */

/**
  @brief         Core Real IFFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
  @return        none

  @par
                   The function implements a Real IFFT
 */

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"
#include "arm_vec_fft.h"

void arm_split_rifft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{
   uint32_t        i;                  /* Loop Counter */
    const q15_t    *pCoefA, *pCoefB;    /* Temporary pointers for twiddle factors */
    q15_t          *pIn1;
    uint16x8_t      offset = { 6, 7, 4, 5, 2, 3, 0, 1 };
    uint16x8_t      offsetCoef;
    int16x8_t       conj = { 1, -1, 1, -1, 1, -1, 1, -1 }; /* conjugate */
    const uint16_t  offsetCoefArr[16] = {
        0, 0, 2, 2, 4, 4, 6, 6,
        0, 1, 0, 1, 0, 1, 0, 1
    };

    offsetCoef = vmulq_n_u16(vld1q_u16(offsetCoefArr), modifier) + vld1q_u16(offsetCoefArr + 8);

    offset = vaddq_n_u16(offset, (2 * fftLen - 6));

    /* Init coefficient pointers */
    pCoefA = &pATable[0];
    pCoefB = &pBTable[0];

    const q15_t    *pCoefAb, *pCoefBb;
    pCoefAb = pCoefA;
    pCoefBb = pCoefB;

    pIn1 = &pSrc[0];

    i = fftLen;
    i = i / 4;

    while (i > 0U) {
        q15x8_t         in1 = vld1q_s16(pIn1);
        q15x8_t         in2 = vldrhq_gather_shifted_offset_s16(pSrc, offset);
        q15x8_t         coefA = vldrhq_gather_shifted_offset_s16(pCoefAb, offsetCoef);
        q15x8_t         coefB = vldrhq_gather_shifted_offset_s16(pCoefBb, offsetCoef);

        /* can we avoid the conjugate here ? */
        q15x8_t         out = vhaddq_s16(MVE_CMPLX_MULT_FX_AxConjB(in1, coefA, q15x8_t),
                                         vmulq(conj, MVE_CMPLX_MULT_FX_AxB(in2, coefB, q15x8_t)));

        vst1q_s16(pDst, out);
        pDst += 8;

        offsetCoef = vaddq_n_u16(offsetCoef, modifier * 8);
        offset -= 8;

        pIn1 += 8;
        i -= 1;
    }
}
#else
void arm_split_rifft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{
        uint32_t i;                                    /* Loop Counter */
        q31_t outR, outI;                              /* Temporary variables for output */
  const q15_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q15_t *pSrc1, *pSrc2;
        q15_t *pDst1 = &pDst[0];

  pCoefA = &pATable[0];
  pCoefB = &pBTable[0];

  pSrc1 = &pSrc[0];
  pSrc2 = &pSrc[2 * fftLen];

  i = fftLen;
  while (i > 0U)
  {
      /*
        outR = (  pIn[2 * i]             * pATable[2 * i]
                + pIn[2 * i + 1]         * pATable[2 * i + 1]
                + pIn[2 * n - 2 * i]     * pBTable[2 * i]
                - pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

        outI = (  pIn[2 * i + 1]         * pATable[2 * i]
                - pIn[2 * i]             * pATable[2 * i + 1]
                - pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
                - pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
       */

#if defined (ARM_MATH_DSP)

#ifndef ARM_MATH_BIG_ENDIAN
      /* pIn[2 * n - 2 * i] * pBTable[2 * i] - pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1]) */
      outR = __SMUSD(read_q15x2(pSrc2), read_q15x2((q15_t *) pCoefB));
#else
      /* -(-pIn[2 * n - 2 * i] * pBTable[2 * i] + pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1])) */
      outR = -(__SMUSD(read_q15x2(pSrc2), read_q15x2((q15_t *) pCoefB)));
#endif /* #ifndef ARM_MATH_BIG_ENDIAN */

      /* pIn[2 * i] * pATable[2 * i] + pIn[2 * i + 1] * pATable[2 * i + 1] + pIn[2 * n - 2 * i] * pBTable[2 * i] */
      outR = __SMLAD(read_q15x2(pSrc1), read_q15x2 ((q15_t *) pCoefA), outR) >> 16U;

      /* -pIn[2 * n - 2 * i] * pBTable[2 * i + 1] + pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
      outI = __SMUADX(read_q15x2_da (&pSrc2), read_q15x2((q15_t *) pCoefB));

      /* pIn[2 * i + 1] * pATable[2 * i] - pIn[2 * i] * pATable[2 * i + 1] */
#ifndef ARM_MATH_BIG_ENDIAN
      outI = __SMLSDX(read_q15x2 ((q15_t *) pCoefA), read_q15x2_ia (&pSrc1), -outI);
#else
      outI = __SMLSDX(read_q15x2_ia (&pSrc1), read_q15x2 ((q15_t *) pCoefA), -outI);
#endif /* #ifndef ARM_MATH_BIG_ENDIAN */

      /* write output */
#ifndef ARM_MATH_BIG_ENDIAN
      write_q15x2_ia (&pDst1, __PKHBT(outR, (outI >> 16U), 16));
#else
      write_q15x2_ia (&pDst1, __PKHBT((outI >> 16U), outR, 16));
#endif /* #ifndef ARM_MATH_BIG_ENDIAN */


#else  /* #if defined (ARM_MATH_DSP) */

      outR = *pSrc2 * *pCoefB;
      outR = outR - (*(pSrc2 + 1) * *(pCoefB + 1));
      outR = outR + (*pSrc1 * *pCoefA);
      outR = (outR + (*(pSrc1 + 1) * *(pCoefA + 1))) >> 16;

      outI = *(pSrc1 + 1) * *pCoefA;
      outI = outI - (*pSrc1 * *(pCoefA + 1));
      outI = outI - (*pSrc2 * *(pCoefB + 1));
      outI = outI - (*(pSrc2 + 1) * *(pCoefB));

      /* update input pointers */
      pSrc1 += 2U;
      pSrc2 -= 2U;

      /* write output */
      *pDst1++ = (q15_t) outR;
      *pDst1++ = (q15_t) (outI >> 16);

#endif /* #if defined (ARM_MATH_DSP) */

      /* update coefficient pointer */
      pCoefB = pCoefB + (2 * modifier);
      pCoefA = pCoefA + (2 * modifier);

      i--;
  }

}
#endif /* defined(ARM_MATH_MVEI) */
