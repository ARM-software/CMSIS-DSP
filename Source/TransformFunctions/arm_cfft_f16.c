/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_f32.c
 * Description:  Combined Radix Decimation in Frequency CFFT Floating point processing function
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


#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"
#include "arm_vec_fft.h"
#include "arm_mve_tables_f16.h"


static float16_t arm_inverse_fft_length_f16(uint16_t fftLen)
{
  float16_t retValue=1.0;

  switch (fftLen)
  {

  case 4096U:
    retValue = (float16_t)0.000244140625f;
    break;

  case 2048U:
    retValue = (float16_t)0.00048828125f;
    break;

  case 1024U:
    retValue = (float16_t)0.0009765625f;
    break;

  case 512U:
    retValue = (float16_t)0.001953125f;
    break;

  case 256U:
    retValue = (float16_t)0.00390625f;
    break;

  case 128U:
    retValue = (float16_t)0.0078125f;
    break;

  case 64U:
    retValue = (float16_t)0.015625f;
    break;

  case 32U:
    retValue = (float16_t)0.03125f;
    break;

  case 16U:
    retValue = (float16_t)0.0625f;
    break;


  default:
    break;
  }
  return(retValue);
}


static void _arm_radix4_butterfly_f16_mve(const arm_cfft_instance_f16 * S,float16_t * pSrc, uint32_t fftLen)
{
    f16x8_t vecTmp0, vecTmp1;
    f16x8_t vecSum0, vecDiff0, vecSum1, vecDiff1;
    f16x8_t vecA, vecB, vecC, vecD;
    uint32_t  blkCnt;
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] =
       { ( 0 - 16) * (int32_t)sizeof(float16_t *)
       , ( 4 - 16) * (int32_t)sizeof(float16_t *)
       , ( 8 - 16) * (int32_t)sizeof(float16_t *)
       , (12 - 16) * (int32_t)sizeof(float16_t *)};

    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;
    for (int k = fftLen / 4u; k > 1; k >>= 2)
    {
        float16_t const     *p_rearranged_twiddle_tab_stride1 =
                            &S->rearranged_twiddle_stride1[
                            S->rearranged_twiddle_tab_stride1_arr[stage]];
        float16_t const     *p_rearranged_twiddle_tab_stride2 =
                            &S->rearranged_twiddle_stride2[
                            S->rearranged_twiddle_tab_stride2_arr[stage]];
        float16_t const     *p_rearranged_twiddle_tab_stride3 =
                            &S->rearranged_twiddle_stride3[
                            S->rearranged_twiddle_tab_stride3_arr[stage]];
        float16_t * pBase = pSrc;
        for (int i = 0; i < iter; i++)
        {
            float16_t    *inA = pBase;
            float16_t    *inB = inA + n2 * CMPLX_DIM;
            float16_t    *inC = inB + n2 * CMPLX_DIM;
            float16_t    *inD = inC + n2 * CMPLX_DIM;
            float16_t const *pW1 = p_rearranged_twiddle_tab_stride1;
            float16_t const *pW2 = p_rearranged_twiddle_tab_stride2;
            float16_t const *pW3 = p_rearranged_twiddle_tab_stride3;
            f16x8_t       vecW;

            blkCnt = n2 / 4;
            /*
             * load 2 f16 complex pair
             */
            vecA = vldrhq_f16(inA);
            vecC = vldrhq_f16(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrhq_f16(inB);
                vecD = vldrhq_f16(inD);

                vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
                vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

                vecSum1 = vecB + vecD;
                vecDiff1 = vecB - vecD;
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vecSum0 + vecSum1;
                vst1q(inA, vecTmp0);
                inA += 8;

                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vecSum0 - vecSum1;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(pW2);
                pW2 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inB, vecTmp1);
                inB += 8;

                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(pW1);
                pW1 +=8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inC, vecTmp1);
                inC += 8;

                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(pW3);
                pW3 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inD, vecTmp1);
                inD += 8;

                vecA = vldrhq_f16(inA);
                vecC = vldrhq_f16(inC);

                blkCnt--;
            }
            pBase +=  CMPLX_DIM * n1;
        }
        n1 = n2;
        n2 >>= 2u;
        iter = iter << 2;
        stage++;
    }

    /*
     * start of Last stage process
     */
    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /* load scheduling */
    vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
    vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

    blkCnt = (fftLen >> 4);
    while (blkCnt > 0U)
    {
        vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
        vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

        vecB = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 4);
        vecD = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 12);

        vecSum1 = vecB + vecD;
        vecDiff1 = vecB - vecD;

        /* pre-load for next iteration */
        vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
        vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

        vecTmp0 = vecSum0 + vecSum1;
        vstrwq_scatter_base_f32(vecScGathAddr, -64, (f32x4_t)vecTmp0);

        vecTmp0 = vecSum0 - vecSum1;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 4, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 8, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 12, (f32x4_t)vecTmp0);

        blkCnt--;
    }

    /*
     * End of last stage process
     */
}

static void arm_cfft_radix4by2_f16_mve(const arm_cfft_instance_f16 * S, float16_t *pSrc, uint32_t fftLen)
{
    float16_t const *pCoefVec;
    float16_t const  *pCoef = S->pTwiddle;
    float16_t        *pIn0, *pIn1;
    uint32_t          n2;
    uint32_t          blkCnt;
    f16x8_t         vecIn0, vecIn1, vecSum, vecDiff;
    f16x8_t         vecCmplxTmp, vecTw;


    n2 = fftLen >> 1;
    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;
    pCoefVec = pCoef;

    blkCnt = n2 / 4;
    while (blkCnt > 0U)
    {
        vecIn0 = *(f16x8_t *) pIn0;
        vecIn1 = *(f16x8_t *) pIn1;
        vecTw = vld1q(pCoefVec);
        pCoefVec += 8;

        vecSum = vaddq(vecIn0, vecIn1);
        vecDiff = vsubq(vecIn0, vecIn1);

        vecCmplxTmp = MVE_CMPLX_MULT_FLT_Conj_AxB(vecTw, vecDiff);

        vst1q(pIn0, vecSum);
        pIn0 += 8;
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 8;

        blkCnt--;
    }

    _arm_radix4_butterfly_f16_mve(S, pSrc, n2);

    _arm_radix4_butterfly_f16_mve(S, pSrc + fftLen, n2);

    pIn0 = pSrc;
}

static void _arm_radix4_butterfly_inverse_f16_mve(const arm_cfft_instance_f16 * S,float16_t * pSrc, uint32_t fftLen, float16_t onebyfftLen)
{
    f16x8_t vecTmp0, vecTmp1;
    f16x8_t vecSum0, vecDiff0, vecSum1, vecDiff1;
    f16x8_t vecA, vecB, vecC, vecD;
    uint32_t  blkCnt;
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] = {
        ( 0 - 16) * (int32_t)sizeof(q31_t *),
        ( 4 - 16) * (int32_t)sizeof(q31_t *),
        ( 8 - 16) * (int32_t)sizeof(q31_t *),
        (12 - 16) * (int32_t)sizeof(q31_t *)
    };

    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;
    for (int k = fftLen / 4; k > 1; k >>= 2)
    {
        float16_t const *p_rearranged_twiddle_tab_stride1 =
                &S->rearranged_twiddle_stride1[
                S->rearranged_twiddle_tab_stride1_arr[stage]];
        float16_t const *p_rearranged_twiddle_tab_stride2 =
                &S->rearranged_twiddle_stride2[
                S->rearranged_twiddle_tab_stride2_arr[stage]];
        float16_t const *p_rearranged_twiddle_tab_stride3 =
                &S->rearranged_twiddle_stride3[
                S->rearranged_twiddle_tab_stride3_arr[stage]];

        float16_t * pBase = pSrc;
        for (int i = 0; i < iter; i++)
        {
            float16_t    *inA = pBase;
            float16_t    *inB = inA + n2 * CMPLX_DIM;
            float16_t    *inC = inB + n2 * CMPLX_DIM;
            float16_t    *inD = inC + n2 * CMPLX_DIM;
            float16_t const *pW1 = p_rearranged_twiddle_tab_stride1;
            float16_t const *pW2 = p_rearranged_twiddle_tab_stride2;
            float16_t const *pW3 = p_rearranged_twiddle_tab_stride3;
            f16x8_t       vecW;

            blkCnt = n2 / 4;
            /*
             * load 2 f32 complex pair
             */
            vecA = vldrhq_f16(inA);
            vecC = vldrhq_f16(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrhq_f16(inB);
                vecD = vldrhq_f16(inD);

                vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
                vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

                vecSum1 = vecB + vecD;
                vecDiff1 = vecB - vecD;
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vecSum0 + vecSum1;
                vst1q(inA, vecTmp0);
                inA += 8;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vecSum0 - vecSum1;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(pW2);
                pW2 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inB, vecTmp1);
                inB += 8;

                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(pW1);
                pW1 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inC, vecTmp1);
                inC += 8;

                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(pW3);
                pW3 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inD, vecTmp1);
                inD += 8;

                vecA = vldrhq_f16(inA);
                vecC = vldrhq_f16(inC);

                blkCnt--;
            }
            pBase +=  CMPLX_DIM * n1;
        }
        n1 = n2;
        n2 >>= 2u;
        iter = iter << 2;
        stage++;
    }

    /*
     * start of Last stage process
     */
    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /*
     * load scheduling
     */
    vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
    vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

    blkCnt = (fftLen >> 4);
    while (blkCnt > 0U)
    {
        vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
        vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

        vecB = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 4);
        vecD = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 12);

        vecSum1 = vecB + vecD;
        vecDiff1 = vecB - vecD;

        vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
        vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

        vecTmp0 = vecSum0 + vecSum1;
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64, (f32x4_t)vecTmp0);

        vecTmp0 = vecSum0 - vecSum1;
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 4, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 8, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 12, (f32x4_t)vecTmp0);

        blkCnt--;
    }

    /*
     * End of last stage process
     */
}

static void arm_cfft_radix4by2_inverse_f16_mve(const arm_cfft_instance_f16 * S,float16_t *pSrc, uint32_t fftLen)
{
    float16_t const *pCoefVec;
    float16_t const  *pCoef = S->pTwiddle;
    float16_t        *pIn0, *pIn1;
    uint32_t          n2;
    float16_t         onebyfftLen = arm_inverse_fft_length_f16(fftLen);
    uint32_t          blkCnt;
    f16x8_t         vecIn0, vecIn1, vecSum, vecDiff;
    f16x8_t         vecCmplxTmp, vecTw;


    n2 = fftLen >> 1;
    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;
    pCoefVec = pCoef;

    blkCnt = n2 / 4;
    while (blkCnt > 0U)
    {
        vecIn0 = *(f16x8_t *) pIn0;
        vecIn1 = *(f16x8_t *) pIn1;
        vecTw = vld1q(pCoefVec);
        pCoefVec += 8;

        vecSum = vaddq(vecIn0, vecIn1);
        vecDiff = vsubq(vecIn0, vecIn1);

        vecCmplxTmp = MVE_CMPLX_MULT_FLT_AxB(vecTw, vecDiff);

        vst1q(pIn0, vecSum);
        pIn0 += 8;
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 8;

        blkCnt--;
    }

    _arm_radix4_butterfly_inverse_f16_mve(S, pSrc, n2, onebyfftLen);

    _arm_radix4_butterfly_inverse_f16_mve(S, pSrc + fftLen, n2, onebyfftLen);
}


/**
  @addtogroup ComplexFFTF16
  @{
 */

/**
  @brief         Processing function for the floating-point complex FFT.
  @param[in]     S              points to an instance of the floating-point CFFT structure
  @param[in,out] p1             points to the complex data buffer. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output

@par Neon version
                     The neon version has a different API.
                     The input and output buffers must be
                     different.
                     There is a temporary buffer.
                     The bit reverse flag is not more 
                     available in Neon version.

  @code
        void arm_cfft_f16(
                const arm_cfft_instance_f16 * S,
                      const float16_t * pIn,
                      float16_t * pOut,
                      float16_t * pBuffer, 
                      uint8_t ifftFlag);
  @endcode

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
 */


ARM_DSP_ATTRIBUTE void arm_cfft_f16(
  const arm_cfft_instance_f16 * S,
        float16_t * pSrc,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag)
{
        uint32_t fftLen = S->fftLen;

        if (ifftFlag == 1U) {

            switch (fftLen) {
            case 16:
            case 64:
            case 256:
            case 1024:
            case 4096:
                _arm_radix4_butterfly_inverse_f16_mve(S, pSrc, fftLen, arm_inverse_fft_length_f16(S->fftLen));
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_inverse_f16_mve(S, pSrc, fftLen);
                break;
            }
        } else {
            switch (fftLen) {
            case 16:
            case 64:
            case 256:
            case 1024:
            case 4096:
                _arm_radix4_butterfly_f16_mve(S, pSrc, fftLen);
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_f16_mve(S, pSrc, fftLen);
                break;
            }
        }


        if (bitReverseFlag)
        {

            arm_bitreversal_16_inpl_mve((uint16_t*)pSrc, S->bitRevLength, S->pBitRevTable);

        }
}
#elif defined(ARM_MATH_NEON_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)
#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_fft.h"



ARM_DSP_ATTRIBUTE void arm_cfft_f16(
  const arm_cfft_instance_f16 * S,
        const float16_t * pIn,
        float16_t * pOut,
        float16_t * pBuffer, /* When used, in is not modified */
        uint8_t ifftFlag)
{
    if (S->algorithm_flag == ARM_MIXED_RADIX_FFT)
    {
        if (ifftFlag)
        {
            arm_ne10_mixed_radix_generic_butterfly_inverse_float16_neon (S,
                (ne10_fft_cpx_float16_t *)pIn, 
                (ne10_fft_cpx_float16_t *)pOut,
                (ne10_fft_cpx_float16_t *)pBuffer);
        }
        else
        {
            arm_ne10_mixed_radix_generic_butterfly_float16_neon (S,
                (ne10_fft_cpx_float16_t *)pIn, 
                (ne10_fft_cpx_float16_t *)pOut,
                (ne10_fft_cpx_float16_t *)pBuffer);
        }
    }
    else 
    {
        if (ifftFlag == 0)
        {
               arm_ne10_mixed_radix_fft_forward_float16_neon (S,
                   (ne10_fft_cpx_float16_t *)pIn,
                   (ne10_fft_cpx_float16_t *)pOut,
                   (ne10_fft_cpx_float16_t *)pBuffer);
        }
        else 
        {
                arm_ne10_mixed_radix_fft_backward_float16_neon (S,
                    (ne10_fft_cpx_float16_t *)pIn,
                    (ne10_fft_cpx_float16_t *)pOut,
                    (ne10_fft_cpx_float16_t *)pBuffer);
        }
    }
}
#else

#if defined(ARM_FLOAT16_SUPPORTED)

extern void arm_bitreversal_16(
        uint16_t * pSrc,
  const uint16_t bitRevLen,
  const uint16_t * pBitRevTable);


extern void arm_cfft_radix4by2_f16(
    float16_t * pSrc,
    uint32_t fftLen,
    const float16_t * pCoef);

extern void arm_radix4_butterfly_f16(
        float16_t * pSrc,
        uint16_t fftLen,
  const float16_t * pCoef,
        uint16_t twidCoefModifier);

/**
  @addtogroup ComplexFFTF16
  @{
 */

/**
  @brief         Processing function for the floating-point complex FFT.
  @param[in]     S              points to an instance of the floating-point CFFT structure
  @param[in,out] p1             points to the complex data buffer. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
 */

ARM_DSP_ATTRIBUTE void arm_cfft_f16(
    const arm_cfft_instance_f16 * S,
    float16_t * p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag)
{
    uint32_t  L = S->fftLen, l;
    float16_t invL, * pSrc;

    if (ifftFlag == 1U)
    {
        /*  Conjugate input data  */
        pSrc = p1 + 1;
        for(l=0; l<L; l++)
        {
            *pSrc = -(_Float16)*pSrc;
            pSrc += 2;
        }
    }

    switch (L)
    {

        case 16:
        case 64:
        case 256:
        case 1024:
        case 4096:
        arm_radix4_butterfly_f16  (p1, L, (float16_t*)S->pTwiddle, 1U);
        break;

        case 32:
        case 128:
        case 512:
        case 2048:
        arm_cfft_radix4by2_f16  ( p1, L, (float16_t*)S->pTwiddle);
        break;

    }

    if ( bitReverseFlag )
        arm_bitreversal_16((uint16_t*)p1, S->bitRevLength,(uint16_t*)S->pBitRevTable);

    if (ifftFlag == 1U)
    {
        invL = 1.0f16/(_Float16)L;
        /*  Conjugate and scale output data */
        pSrc = p1;
        for(l=0; l<L; l++)
        {
            *pSrc++ *=   (_Float16)invL ;
            *pSrc  = -(_Float16)(*pSrc) * (_Float16)invL;
            pSrc++;
        }
    }
}
#endif /* if defined(ARM_FLOAT16_SUPPORTED) */
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
  @} end of ComplexFFTF16 group
 */
