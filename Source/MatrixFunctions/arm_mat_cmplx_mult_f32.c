/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_cmplx_mult_f32.c
 * Description:  Floating-point matrix multiplication
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

#include "dsp/matrix_functions.h"

/**
  @ingroup groupMatrix
 */

/**
  @defgroup CmplxMatrixMult  Complex Matrix Multiplication

  Complex Matrix multiplication is only defined if the number of columns of the
  first matrix equals the number of rows of the second matrix.
  Multiplying an <code>M x N</code> matrix with an <code>N x P</code> matrix results
  in an <code>M x P</code> matrix.
  @par
  When matrix size checking is enabled, the functions check:
   - that the inner dimensions of <code>pSrcA</code> and <code>pSrcB</code> are equal;
   - that the size of the output matrix equals the outer dimensions of <code>pSrcA</code> and <code>pSrcB</code>.
 */


/**
  @addtogroup CmplxMatrixMult
  @{
 */

/**
  @brief         Floating-point Complex matrix multiplication.
  @param[in]     pSrcA      points to first input complex matrix structure
  @param[in]     pSrcB      points to second input complex matrix structure
  @param[out]    pDst       points to output complex matrix structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
 */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

#define MATRIX_DIM2 2
#define MATRIX_DIM3 3
#define MATRIX_DIM4 4

__STATIC_INLINE arm_status arm_mat_cmplx_mult_f32_2x2_mve(
    const arm_matrix_instance_f32 * pSrcA,
    const arm_matrix_instance_f32 * pSrcB,
    arm_matrix_instance_f32 * pDst)
{
    float32_t const *pInB = pSrcB->pData;  /* input data matrix pointer B */
    float32_t       *pInA = pSrcA->pData;  /* input data matrix pointer A */
    float32_t       *pOut = pDst->pData;   /* output data matrix pointer */
    uint32x4_t   vecColBOffs0;
    float32_t       *pInA0 = pInA;
    float32_t       *pInA1 = pInA0 + CMPLX_DIM * MATRIX_DIM2;
    f32x4_t    acc0, acc1;
    f32x4_t    vecB, vecA;

    static const uint32_t offsetB0[4] = { 0, 1,
        MATRIX_DIM2 * CMPLX_DIM, MATRIX_DIM2 * CMPLX_DIM + 1
    };

    vecColBOffs0 = vldrwq_u32((uint32_t const *) offsetB0);

    pInB = (float32_t const *)pSrcB->pData;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    pOut[0 * CMPLX_DIM * MATRIX_DIM2 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM2 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM2 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM2 + 1] = acc1[1] + acc1[3];
    pOut += CMPLX_DIM;

    /*
     * move to next B column
     */
    pInB = pInB + CMPLX_DIM;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    pOut[0 * CMPLX_DIM * MATRIX_DIM2 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM2 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM2 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM2 + 1] = acc1[1] + acc1[3];
    /*
     * Return to application
     */
    return (ARM_MATH_SUCCESS);
}


__STATIC_INLINE arm_status arm_mat_cmplx_mult_f32_3x3_mve(
    const arm_matrix_instance_f32 * pSrcA,
    const arm_matrix_instance_f32 * pSrcB,
    arm_matrix_instance_f32 * pDst)
{
    float32_t const *pInB = pSrcB->pData;  /* input data matrix pointer B */
    float32_t       *pInA = pSrcA->pData;  /* input data matrix pointer A */
    float32_t       *pOut = pDst->pData;   /* output data matrix pointer */
    uint32x4_t   vecColBOffs0, vecColBOffs1;
    float32_t       *pInA0 = pInA;
    float32_t       *pInA1 = pInA0 + CMPLX_DIM * MATRIX_DIM3;
    float32_t       *pInA2 = pInA1 + CMPLX_DIM * MATRIX_DIM3;
    f32x4_t    acc0, acc1, acc2;
    f32x4_t    vecB, vecA;
    /* enable predication to disable upper half complex vector element */
    mve_pred16_t p0 = vctp32q(CMPLX_DIM);

    static const uint32_t offsetB0[4] = { 0, 1,
        MATRIX_DIM3 * CMPLX_DIM, MATRIX_DIM3 * CMPLX_DIM + 1
    };
    static const uint32_t offsetB1[4] = { 2 * MATRIX_DIM3 * CMPLX_DIM, 2 * MATRIX_DIM3 * CMPLX_DIM + 1,
       INACTIVELANE, INACTIVELANE
    };

    vecColBOffs0 = vldrwq_u32((uint32_t const *) offsetB0);
    vecColBOffs1 = vldrwq_u32((uint32_t const *) offsetB1);

    pInB = (float32_t const *)pSrcB->pData;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(pInA2);
    acc2 = vcmulq(vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);


    vecB = vldrwq_gather_shifted_offset_z(pInB, vecColBOffs1, p0);

    vecA = vldrwq_f32(&pInA0[4]);
    acc0 = vcmlaq(acc0, vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(&pInA1[4]);
    acc1 = vcmlaq(acc1, vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(&pInA2[4]);
    acc2 = vcmlaq(acc2, vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);


    pOut[0 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc1[1] + acc1[3];
    pOut[2 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc2[0] + acc2[2];
    pOut[2 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc2[1] + acc2[3];
    pOut += CMPLX_DIM;

    /*
     * move to next B column
     */
    pInB = pInB + CMPLX_DIM;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(pInA2);
    acc2 = vcmulq(vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

     vecB = vldrwq_gather_shifted_offset_z(pInB, vecColBOffs1, p0);

    vecA = vldrwq_f32(&pInA0[4]);
    acc0 = vcmlaq(acc0, vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(&pInA1[4]);
    acc1 = vcmlaq(acc1, vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(&pInA2[4]);
    acc2 = vcmlaq(acc2, vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);


    pOut[0 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc1[1] + acc1[3];
    pOut[2 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc2[0] + acc2[2];
    pOut[2 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc2[1] + acc2[3];
    pOut += CMPLX_DIM;

    /*
     * move to next B column
     */
    pInB = pInB + CMPLX_DIM;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(pInA2);
    acc2 = vcmulq(vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

     vecB = vldrwq_gather_shifted_offset_z(pInB, vecColBOffs1, p0);

    vecA = vldrwq_f32(&pInA0[4]);
    acc0 = vcmlaq(acc0, vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(&pInA1[4]);
    acc1 = vcmlaq(acc1, vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(&pInA2[4]);
    acc2 = vcmlaq(acc2, vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);


    pOut[0 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc1[1] + acc1[3];
    pOut[2 * CMPLX_DIM * MATRIX_DIM3 + 0] = acc2[0] + acc2[2];
    pOut[2 * CMPLX_DIM * MATRIX_DIM3 + 1] = acc2[1] + acc2[3];
    /*
     * Return to application
     */
    return (ARM_MATH_SUCCESS);
}



__STATIC_INLINE arm_status arm_mat_cmplx_mult_f32_4x4_mve(
    const arm_matrix_instance_f32 * pSrcA,
    const arm_matrix_instance_f32 * pSrcB,
    arm_matrix_instance_f32 * pDst)
{
    float32_t const *pInB = pSrcB->pData;  /* input data matrix pointer B */
    float32_t       *pInA = pSrcA->pData;  /* input data matrix pointer A */
    float32_t       *pOut = pDst->pData;   /* output data matrix pointer */
    uint32x4_t   vecColBOffs0, vecColBOffs1;
    float32_t       *pInA0 = pInA;
    float32_t       *pInA1 = pInA0 + CMPLX_DIM * MATRIX_DIM4;
    float32_t       *pInA2 = pInA1 + CMPLX_DIM * MATRIX_DIM4;
    float32_t       *pInA3 = pInA2 + CMPLX_DIM * MATRIX_DIM4;
    f32x4_t    acc0, acc1, acc2, acc3;
    f32x4_t    vecB, vecA;

    static const uint32_t offsetB0[4] = { 0, 1,
        MATRIX_DIM4 * CMPLX_DIM, MATRIX_DIM4 * CMPLX_DIM + 1
    };
    static const uint32_t offsetB1[4] = { 2 * MATRIX_DIM4 * CMPLX_DIM, 2 * MATRIX_DIM4 * CMPLX_DIM + 1,
        3 * MATRIX_DIM4 * CMPLX_DIM, 3 * MATRIX_DIM4 * CMPLX_DIM + 1
    };

    vecColBOffs0 = vldrwq_u32((uint32_t const *) offsetB0);
    vecColBOffs1 = vldrwq_u32((uint32_t const *) offsetB1);

    pInB = (float32_t const *)pSrcB->pData;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(pInA2);
    acc2 = vcmulq(vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(pInA3);
    acc3 = vcmulq(vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs1);

    vecA = vldrwq_f32(&pInA0[4]);
    acc0 = vcmlaq(acc0, vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(&pInA1[4]);
    acc1 = vcmlaq(acc1, vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(&pInA2[4]);
    acc2 = vcmlaq(acc2, vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(&pInA3[4]);
    acc3 = vcmlaq(acc3, vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc1[1] + acc1[3];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc2[0] + acc2[2];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc2[1] + acc2[3];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc3[0] + acc3[2];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc3[1] + acc3[3];
    pOut += CMPLX_DIM;

    /*
     * move to next B column
     */
    pInB = pInB + CMPLX_DIM;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(pInA2);
    acc2 = vcmulq(vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(pInA3);
    acc3 = vcmulq(vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs1);

    vecA = vldrwq_f32(&pInA0[4]);
    acc0 = vcmlaq(acc0, vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(&pInA1[4]);
    acc1 = vcmlaq(acc1, vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(&pInA2[4]);
    acc2 = vcmlaq(acc2, vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(&pInA3[4]);
    acc3 = vcmlaq(acc3, vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc1[1] + acc1[3];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc2[0] + acc2[2];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc2[1] + acc2[3];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc3[0] + acc3[2];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc3[1] + acc3[3];
    pOut += CMPLX_DIM;

    /*
     * move to next B column
     */
    pInB = pInB + CMPLX_DIM;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(pInA2);
    acc2 = vcmulq(vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(pInA3);
    acc3 = vcmulq(vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs1);

    vecA = vldrwq_f32(&pInA0[4]);
    acc0 = vcmlaq(acc0, vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(&pInA1[4]);
    acc1 = vcmlaq(acc1, vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(&pInA2[4]);
    acc2 = vcmlaq(acc2, vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(&pInA3[4]);
    acc3 = vcmlaq(acc3, vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc1[1] + acc1[3];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc2[0] + acc2[2];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc2[1] + acc2[3];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc3[0] + acc3[2];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc3[1] + acc3[3];
    pOut += CMPLX_DIM;

    /*
     * move to next B column
     */
    pInB = pInB + CMPLX_DIM;

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs0);

    vecA = vldrwq_f32(pInA0);
    acc0 = vcmulq(vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(pInA1);
    acc1 = vcmulq(vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(pInA2);
    acc2 = vcmulq(vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(pInA3);
    acc3 = vcmulq(vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    vecB = vldrwq_gather_shifted_offset(pInB, vecColBOffs1);

    vecA = vldrwq_f32(&pInA0[4]);
    acc0 = vcmlaq(acc0, vecA, vecB);
    acc0 = vcmlaq_rot90(acc0, vecA, vecB);

    vecA = vldrwq_f32(&pInA1[4]);
    acc1 = vcmlaq(acc1, vecA, vecB);
    acc1 = vcmlaq_rot90(acc1, vecA, vecB);

    vecA = vldrwq_f32(&pInA2[4]);
    acc2 = vcmlaq(acc2, vecA, vecB);
    acc2 = vcmlaq_rot90(acc2, vecA, vecB);

    vecA = vldrwq_f32(&pInA3[4]);
    acc3 = vcmlaq(acc3, vecA, vecB);
    acc3 = vcmlaq_rot90(acc3, vecA, vecB);

    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc0[0] + acc0[2];
    pOut[0 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc0[1] + acc0[3];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc1[0] + acc1[2];
    pOut[1 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc1[1] + acc1[3];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc2[0] + acc2[2];
    pOut[2 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc2[1] + acc2[3];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 0] = acc3[0] + acc3[2];
    pOut[3 * CMPLX_DIM * MATRIX_DIM4 + 1] = acc3[1] + acc3[3];
    /*
     * Return to application
     */
    return (ARM_MATH_SUCCESS);
}

ARM_DSP_ATTRIBUTE arm_status arm_mat_cmplx_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
    float32_t const *pInB = (float32_t const *) pSrcB->pData;   /* input data matrix pointer B */
    float32_t const *pInA = (float32_t const *) pSrcA->pData;   /* input data matrix pointer A */
    float32_t *pOut = pDst->pData;  /* output data matrix pointer */
    float32_t *px;              /* Temporary output data matrix pointer */
    uint16_t  numRowsA = pSrcA->numRows;    /* number of rows of input matrix A    */
    uint16_t  numColsB = pSrcB->numCols;    /* number of columns of input matrix B */
    uint16_t  numColsA = pSrcA->numCols;    /* number of columns of input matrix A */
    uint16_t  col, i = 0U, row = numRowsA;  /* loop counters */
    arm_status status;          /* status of matrix multiplication */
    uint32x4_t vecOffs, vecColBOffs;
    uint32_t  blkCnt, rowCnt;           /* loop counters */

  #ifdef ARM_MATH_MATRIX_CHECK


  /* Check for matrix mismatch condition */
  if ((pSrcA->numCols != pSrcB->numRows) ||
     (pSrcA->numRows != pDst->numRows) || (pSrcB->numCols != pDst->numCols))
  {

    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else
#endif /*      #ifdef ARM_MATH_MATRIX_CHECK    */

  {
    /*
     * small squared matrix specialized routines
     */
    if (numRowsA == numColsB && numColsB == numColsA)
    {
        if (numRowsA == 1)
        {
            pOut[0] = pInA[0] * pInB[0] - pInA[1] * pInB[1];
            pOut[1] = pInA[0] * pInB[1] + pInA[1] * pInB[0];
            return (ARM_MATH_SUCCESS);
        }
        else if (numRowsA == 2)
            return arm_mat_cmplx_mult_f32_2x2_mve(pSrcA, pSrcB, pDst);
        else if (numRowsA == 3)
            return arm_mat_cmplx_mult_f32_3x3_mve(pSrcA, pSrcB, pDst);
        else if (numRowsA == 4)
            return arm_mat_cmplx_mult_f32_4x4_mve(pSrcA, pSrcB, pDst);
    }

    vecColBOffs[0] = 0;
    vecColBOffs[1] = 1;
    vecColBOffs[2] = numColsB * CMPLX_DIM;
    vecColBOffs[3] = (numColsB * CMPLX_DIM) + 1;

    /*
     * The following loop performs the dot-product of each row in pSrcA with each column in pSrcB
     */

    /*
     * row loop
     */
    rowCnt = row >> 2;
    while (rowCnt > 0u)
    {
        /*
         * Output pointer is set to starting address of the row being processed
         */
        px = pOut + i * CMPLX_DIM;
        i = i + 4 * numColsB;
        /*
         * For every row wise process, the column loop counter is to be initiated
         */
        col = numColsB;
        /*
         * For every row wise process, the pInB pointer is set
         * to the starting address of the pSrcB data
         */
        pInB = (float32_t const *) pSrcB->pData;
        /*
         * column loop
         */
        while (col > 0u)
        {
            /*
             * generate 4 columns elements
             */
            /*
             * Matrix A columns number of MAC operations are to be performed
             */

            float32_t const *pSrcA0Vec, *pSrcA1Vec, *pSrcA2Vec, *pSrcA3Vec;
            float32_t const *pInA0 = pInA;
            float32_t const *pInA1 = pInA0 + numColsA * CMPLX_DIM;
            float32_t const *pInA2 = pInA1 + numColsA * CMPLX_DIM;
            float32_t const *pInA3 = pInA2 + numColsA * CMPLX_DIM;
            f32x4_t acc0, acc1, acc2, acc3;

            acc0 = vdupq_n_f32(0.0f);
            acc1 = vdupq_n_f32(0.0f);
            acc2 = vdupq_n_f32(0.0f);
            acc3 = vdupq_n_f32(0.0f);

            pSrcA0Vec = (float32_t const *) pInA0;
            pSrcA1Vec = (float32_t const *) pInA1;
            pSrcA2Vec = (float32_t const *) pInA2;
            pSrcA3Vec = (float32_t const *) pInA3;

            vecOffs = vecColBOffs;

            /*
             * process 1 x 4 block output
             */
            blkCnt = (numColsA * CMPLX_DIM) >> 2;
            while (blkCnt > 0U)
            {
                f32x4_t vecB, vecA;

                vecB = vldrwq_gather_shifted_offset(pInB, vecOffs);
                /*
                 * move Matrix B read offsets, 4 rows down
                 */
                vecOffs = vecOffs + (uint32_t) (numColsB * 2 * CMPLX_DIM);

                vecA = vld1q(pSrcA0Vec);  pSrcA0Vec += 4;
                acc0 = vcmlaq(acc0, vecA, vecB);
                acc0 = vcmlaq_rot90(acc0, vecA, vecB);
                vecA = vld1q(pSrcA1Vec); pSrcA1Vec += 4;
                acc1 = vcmlaq(acc1, vecA, vecB);
                acc1 = vcmlaq_rot90(acc1, vecA, vecB);
                vecA = vld1q(pSrcA2Vec);  pSrcA2Vec += 4;
                acc2 = vcmlaq(acc2, vecA, vecB);
                acc2 = vcmlaq_rot90(acc2, vecA, vecB);
                vecA = vld1q(pSrcA3Vec);  pSrcA3Vec += 4;
                acc3 = vcmlaq(acc3, vecA, vecB);
                acc3 = vcmlaq_rot90(acc3, vecA, vecB);

                blkCnt--;
            }
            

            /*
             * tail
             * (will be merged thru tail predication)
             */
            blkCnt = (numColsA * CMPLX_DIM) & 3;
            if (blkCnt > 0U)
            {
                mve_pred16_t p0 = vctp32q(blkCnt);
                f32x4_t vecB, vecA;

                vecB = vldrwq_gather_shifted_offset_z(pInB, vecOffs, p0);
                /*
                 * move Matrix B read offsets, 4 rows down
                 */
                vecOffs = vecOffs + (uint32_t) (numColsB * 2 * CMPLX_DIM);

                vecA = vld1q(pSrcA0Vec);
                acc0 = vcmlaq(acc0, vecA, vecB);
                acc0 = vcmlaq_rot90(acc0, vecA, vecB);
                vecA = vld1q(pSrcA1Vec);
                acc1 = vcmlaq(acc1, vecA, vecB);
                acc1 = vcmlaq_rot90(acc1, vecA, vecB);
                vecA = vld1q(pSrcA2Vec);
                acc2 = vcmlaq(acc2, vecA, vecB);
                acc2 = vcmlaq_rot90(acc2, vecA, vecB);
                vecA = vld1q(pSrcA3Vec);
                acc3 = vcmlaq(acc3, vecA, vecB);
                acc3 = vcmlaq_rot90(acc3, vecA, vecB);

            }

            px[0 * CMPLX_DIM * numColsB + 0] = acc0[0] + acc0[2];
            px[0 * CMPLX_DIM * numColsB + 1] = acc0[1] + acc0[3];
            px[1 * CMPLX_DIM * numColsB + 0] = acc1[0] + acc1[2];
            px[1 * CMPLX_DIM * numColsB + 1] = acc1[1] + acc1[3];
            px[2 * CMPLX_DIM * numColsB + 0] = acc2[0] + acc2[2];
            px[2 * CMPLX_DIM * numColsB + 1] = acc2[1] + acc2[3];
            px[3 * CMPLX_DIM * numColsB + 0] = acc3[0] + acc3[2];
            px[3 * CMPLX_DIM * numColsB + 1] = acc3[1] + acc3[3];
            px += CMPLX_DIM;
            /*
             * Decrement the column loop counter
             */
            col--;
            /*
             * Update the pointer pInB to point to the  starting address of the next column
             */
            pInB = (float32_t const *) pSrcB->pData + (numColsB - col) * CMPLX_DIM;
        }

        /*
         * Update the pointer pInA to point to the  starting address of the next row
         */
        pInA += (numColsA * 4) * CMPLX_DIM;
        /*
         * Decrement the row loop counter
         */
        rowCnt --;

    }

    rowCnt = row & 3;
    while (rowCnt > 0u)
    {
           /*
         * Output pointer is set to starting address of the row being processed
         */
        px = pOut + i * CMPLX_DIM;
        i = i + numColsB;
        /*
         * For every row wise process, the column loop counter is to be initiated
         */
        col = numColsB;
        /*
         * For every row wise process, the pInB pointer is set
         * to the starting address of the pSrcB data
         */
        pInB = (float32_t const *) pSrcB->pData;
        /*
         * column loop
         */
        while (col > 0u)
        {
            /*
             * generate 4 columns elements
             */
            /*
             * Matrix A columns number of MAC operations are to be performed
             */

            float32_t const *pSrcA0Vec;
            float32_t const *pInA0 = pInA;
            f32x4_t acc0;

            acc0 = vdupq_n_f32(0.0f);

            pSrcA0Vec = (float32_t const *) pInA0;
           
            vecOffs = vecColBOffs;

            /*
             * process 1 x 4 block output
             */
            blkCnt = (numColsA * CMPLX_DIM) >> 2;
            while (blkCnt > 0U)
            {
                f32x4_t vecB, vecA;

                vecB = vldrwq_gather_shifted_offset(pInB, vecOffs);
                /*
                 * move Matrix B read offsets, 4 rows down
                 */
                vecOffs = vecOffs + (uint32_t) (numColsB * 2 * CMPLX_DIM);

                vecA = vld1q(pSrcA0Vec);  
                pSrcA0Vec += 4;
                acc0 = vcmlaq(acc0, vecA, vecB);
                acc0 = vcmlaq_rot90(acc0, vecA, vecB);
                

                blkCnt--;
            }


            /*
             * tail
             */
            blkCnt = (numColsA * CMPLX_DIM) & 3;
            if (blkCnt > 0U)
            {
                mve_pred16_t p0 = vctp32q(blkCnt);
                f32x4_t vecB, vecA;

                vecB = vldrwq_gather_shifted_offset_z(pInB, vecOffs, p0);
               
                vecA = vld1q(pSrcA0Vec);
                acc0 = vcmlaq(acc0, vecA, vecB);
                acc0 = vcmlaq_rot90(acc0, vecA, vecB);

            }

            px[0] = acc0[0] + acc0[2];
            px[1] = acc0[1] + acc0[3];
           
            px += CMPLX_DIM;
            /*
             * Decrement the column loop counter
             */
            col--;
            /*
             * Update the pointer pInB to point to the  starting address of the next column
             */
            pInB = (float32_t const *) pSrcB->pData + (numColsB - col) * CMPLX_DIM;
        }

        /*
         * Update the pointer pInA to point to the  starting address of the next row
         */
        pInA += numColsA  * CMPLX_DIM;
        rowCnt--;
    }

    
      /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);

}

#else
#if defined(ARM_MATH_NEON)

/**
  @brief         Floating-point Complex matrix multiplication.
  @param[in]     pSrcA      points to first input complex matrix structure
  @param[in]     pSrcB      points to second input complex matrix structure
  @param[out]    pDst       points to output complex matrix structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
 */


 struct _arm_complex_f32 { \
  float32_t re;       \
  float32_t im;       \
};

#define LANE 4
#define DTYPE struct _arm_complex_f32
#define HEADERTYPE float32_t
#define VEC float32x4x2_t
#define VECACC float32x4x2_t
#define FLOATALGO

//#define LOGKERNEL(A,B) printf("%s\n", A)

#define CLEAR_ACC(tmp) tmp.val[0] = vdupq_n_f32(0.0f); \
tmp.val[1] = vdupq_n_f32(0.0f)

#define SCALARACC struct _arm_complex_f32

#define DEF_AND_CLEAR_SCALARACC(tmp) const SCALARACC tmp={.re=0.0f, .im = 0.0f}

#define TMPLD
#define TMPST
#define TMPMAC


#define SCALAR_LOAD_AND_WIDEN(DST,PTR) DST = *(PTR)

#define SCALAR_STORE_AND_NARROW(PTR,VAL) *(PTR) = (VAL)

#define SCALAR_MAC_N(ACC,VEC,SCALAR) ACC.re = ACC.re + (VEC).re * (SCALAR).re - (VEC).im * (SCALAR).im; \
ACC.im = ACC.im + (VEC).re * (SCALAR).im + (VEC).im * (SCALAR).re; 
#define HVEC float32x2_t
#define VLOAD(DST,PTR) DST = vld2q_f32((float32_t*)(PTR))
#define VSTORE(PTR,VAL) vst2q_f32((float32_t*)(PTR),(VAL))

#define VLOAD_ACC(DST,PTR) DST = vld2q_f32((float32_t*)(PTR))
#define VSTORE_ACC(PTR,VAL) vst2q_f32((float32_t*)(PTR),(VAL))

#define VLOAD_AND_WIDEN(DST,PTR) DST = vld2q_f32((float32_t*)(PTR))
#define VSTORE_AND_NARROW(PTR,VAL) vst2q_f32((float32_t*)(PTR),(VAL))

#if defined(__ARM_FEATURE_FMA)
#define VMAC_N(ACC,VEC,SCALAR) ACC.val[0] = vfmaq_n_f32(ACC.val[0],(VEC).val[0],(SCALAR).re); \
                               ACC.val[0] = vfmsq_n_f32(ACC.val[0], (VEC).val[1], (SCALAR).im);                   \
                               ACC.val[1] = vfmaq_n_f32(ACC.val[1],(VEC).val[0],(SCALAR).im);                                \
                               ACC.val[1] = vfmaq_n_f32(ACC.val[1], (VEC).val[1], (SCALAR).re); 
#else
#define VMAC_N(ACC,VEC,SCALAR) ACC.val[0] = vmlaq_n_f32(ACC.val[0],(VEC).val[0],(SCALAR).re); \
                               ACC.val[0] = vmlsq_n_f32(ACC.val[0], (VEC).val[1], (SCALAR).im);                   \
                               ACC.val[1] = vmlaq_n_f32(ACC.val[1],(VEC).val[0],(SCALAR).im);                                \
                               ACC.val[1] = vmlaq_n_f32(ACC.val[1], (VEC).val[1], (SCALAR).re);
#endif

#define MATTYPE arm_matrix_instance_f32
#define EXT(A) A##_cf32

#define FUNCNAME arm_mat_cmplx_mult_f32


#include "_arm_mat_mult_neon.c"

#else
ARM_DSP_ATTRIBUTE arm_status arm_mat_cmplx_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
        arm_matrix_instance_f32 * pDst)
{
  float32_t *pIn1 = pSrcA->pData;                /* Input data matrix pointer A */
  float32_t *pIn2 = pSrcB->pData;                /* Input data matrix pointer B */
  float32_t *pInA = pSrcA->pData;                /* Input data matrix pointer A */
  float32_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  float32_t *px;                                 /* Temporary output data matrix pointer */
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  float32_t sumReal, sumImag;                    /* Accumulator */
  float32_t a1, b1, c1, d1;
  uint32_t col, i = 0U, j, row = numRowsA, colCnt; /* loop counters */
  arm_status status;                             /* status of matrix multiplication */

#if defined (ARM_MATH_LOOPUNROLL)
  float32_t a0, b0, c0, d0;
#endif

#ifdef ARM_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrcA->numCols != pSrcB->numRows) ||
      (pSrcA->numRows != pDst->numRows)  ||
      (pSrcB->numCols != pDst->numCols)    )
  {
    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef ARM_MATH_MATRIX_CHECK */

  {
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of the row being processed */
      px = pOut + 2 * i;

      /* For every row wise process, the column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, the pIn2 pointer is set
       ** to the starting address of the pSrcB data */
      pIn2 = pSrcB->pData;

      j = 0U;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sumReal = 0.0f;
        sumImag = 0.0f;

        /* Initiate pointer pIn1 to point to starting address of column being processed */
        pIn1 = pInA;

#if defined (ARM_MATH_LOOPUNROLL)

        /* Apply loop unrolling and compute 4 MACs simultaneously. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {

          /* Reading real part of complex matrix A */
          a0 = *pIn1;

          /* Reading real part of complex matrix B */
          c0 = *pIn2;

          /* Reading imaginary part of complex matrix A */
          b0 = *(pIn1 + 1U);

          /* Reading imaginary part of complex matrix B */
          d0 = *(pIn2 + 1U);

          /* Multiply and Accumulates */
          sumReal += a0 * c0;
          sumImag += b0 * c0;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumulates */
          sumReal -= b0 * d0;
          sumImag += a0 * d0;

          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* read real and imag values from pSrcA and pSrcB buffer */
          a1 = *(pIn1     );
          c1 = *(pIn2     );
          b1 = *(pIn1 + 1U);
          d1 = *(pIn2 + 1U);

          /* Multiply and Accumulates */
          sumReal += a1 * c1;
          sumImag += b1 * c1;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumulates */
          sumReal -= b1 * d1;
          sumImag += a1 * d1;

          a0 = *(pIn1     );
          c0 = *(pIn2     );
          b0 = *(pIn1 + 1U);
          d0 = *(pIn2 + 1U);

          /* Multiply and Accumulates */
          sumReal += a0 * c0;
          sumImag += b0 * c0;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumulates */
          sumReal -= b0 * d0;
          sumImag += a0 * d0;

          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          a1 = *(pIn1     );
          c1 = *(pIn2     );
          b1 = *(pIn1 + 1U);
          d1 = *(pIn2 + 1U);

          /* Multiply and Accumulates */
          sumReal += a1 * c1;
          sumImag += b1 * c1;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumulates */
          sumReal -= b1 * d1;
          sumImag += a1 * d1;

          /* Decrement loop count */
          colCnt--;
        }

        /* If the columns of pSrcA is not a multiple of 4, compute any remaining MACs here.
         ** No loop unrolling is used. */
        colCnt = numColsA % 0x4U;

#else

        /* Initialize blkCnt with number of samples */
        colCnt = numColsA;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
          a1 = *(pIn1     );
          c1 = *(pIn2     );
          b1 = *(pIn1 + 1U);
          d1 = *(pIn2 + 1U);

          /* Multiply and Accumulates */
          sumReal += a1 * c1;
          sumImag += b1 * c1;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumulates */
          sumReal -= b1 * d1;
          sumImag += a1 * d1;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Store result in destination buffer */
        *px++ = sumReal;
        *px++ = sumImag;

        /* Update pointer pIn2 to point to starting address of next column */
        j++;
        pIn2 = pSrcB->pData + 2U * j;

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      /* Update pointer pInA to point to starting address of next row */
      i = i + numColsB;
      pInA = pInA + 2 * numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

#endif /* #if defined(ARM_MATH_NEON) */
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
  @} end of MatrixMult group
 */
