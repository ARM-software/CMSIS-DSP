/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_vec_mult_q7.c
 * Description:  Q7 matrix and vector multiplication
 *
 * $Date:        23 April 2021
 *
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
#include "arm_compiler_specific.h"


#include "dsp/matrix_functions.h"

/**
 * @ingroup groupMatrix
 */



/**
 * @addtogroup MatrixVectMult
 * @{
 */

/**
 * @brief Q7 matrix and vector multiplication.
 * @param[in]       *pSrcMat points to the input matrix structure
 * @param[in]       *pVec points to the input vector
 * @param[out]      *pDst points to the output vector
 */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

ARM_DSP_ATTRIBUTE void arm_mat_vec_mult_q7(
    const arm_matrix_instance_q7 * pSrcMat,
    const q7_t     *pSrcVec,
    q7_t           *pDstVec)
{
    const q7_t *pMatSrc = pSrcMat->pData;
    const q7_t *pMat0, *pMat1;
    uint32_t     numRows = pSrcMat->numRows;
    uint32_t     numCols = pSrcMat->numCols;
    q7_t       *px;
    int32_t      row;
    uint16_t     blkCnt;           /* loop counters */

    row = numRows;
    px = pDstVec;

    /*
     * compute 4x64-bit accumulators per loop
     */
    while (row >= 4)
    {
        q7_t const *pMat0Vec, *pMat1Vec, *pMat2Vec, *pMat3Vec, *pVec;
        const q7_t  *pMat2, *pMat3;
        q7_t const  *pSrcVecPtr = pSrcVec;
        q31_t        acc0, acc1, acc2, acc3;
        q7x16_t      vecMatA0, vecMatA1, vecMatA2, vecMatA3, vecIn;

        pVec = pSrcVec;
        /*
         * Initialize the pointer pIn1 to point to the starting address of the column being processed
         */
        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;
        pMat2 = pMat1 + numCols;
        pMat3 = pMat2 + numCols;

        acc0 = 0L;
        acc1 = 0L;
        acc2 = 0L;
        acc3 = 0L;

        pMat0Vec = pMat0;
        pMat1Vec = pMat1;
        pMat2Vec = pMat2;
        pMat3Vec = pMat3;
        pVec = pSrcVecPtr;

        blkCnt = numCols >> 4;
        while (blkCnt > 0U)
        {

            vecMatA0 = vld1q(pMat0Vec); 
            pMat0Vec += 16;
            vecMatA1 = vld1q(pMat1Vec); 
            pMat1Vec += 16;
            vecMatA2 = vld1q(pMat2Vec); 
            pMat2Vec += 16;
            vecMatA3 = vld1q(pMat3Vec); 
            pMat3Vec += 16;
            vecIn = vld1q(pVec);        
            pVec += 16;

            acc0 = vmladavaq(acc0, vecIn, vecMatA0);
            acc1 = vmladavaq(acc1, vecIn, vecMatA1);
            acc2 = vmladavaq(acc2, vecIn, vecMatA2);
            acc3 = vmladavaq(acc3, vecIn, vecMatA3);

            blkCnt--;
        }
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 0xF;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp8q(blkCnt);

            vecMatA0 = vld1q(pMat0Vec);
            vecMatA1 = vld1q(pMat1Vec);
            vecMatA2 = vld1q(pMat2Vec);
            vecMatA3 = vld1q(pMat3Vec);
            vecIn = vldrbq_z_s8(pVec, p0);

            acc0 = vmladavaq(acc0, vecIn, vecMatA0);
            acc1 = vmladavaq(acc1, vecIn, vecMatA1);
            acc2 = vmladavaq(acc2, vecIn, vecMatA2);
            acc3 = vmladavaq(acc3, vecIn, vecMatA3);
        }

        *px++ = __SSAT(acc0 >> 7, 8);
        *px++ = __SSAT(acc1 >> 7, 8);
        *px++ = __SSAT(acc2 >> 7, 8);
        *px++ = __SSAT(acc3 >> 7, 8);

        pMatSrc += numCols * 4;
        /*
         * Decrement the row loop counter
         */
        row -= 4;
    }

    /*
     * process any remaining rows pair
     */
    if (row >= 2)
    {
        q7_t const  *pMat0Vec, *pMat1Vec, *pVec;
        q7_t const  *pSrcVecPtr = pSrcVec;
        q31_t         acc0, acc1;
        q7x16_t     vecMatA0, vecMatA1, vecIn;

        /*
         * For every row wise process, the pInVec pointer is set
         * to the starting address of the vector
         */
        pVec = pSrcVec;

        /*
         * Initialize the pointer pIn1 to point to the starting address of the column being processed
         */
        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;

        acc0 = 0;
        acc1 = 0;

        pMat0Vec = pMat0;
        pMat1Vec = pMat1;
        pVec = pSrcVecPtr;

        blkCnt = numCols >> 4;
        while (blkCnt > 0U)
        {
            vecMatA0 = vld1q(pMat0Vec); 
            pMat0Vec += 16;
            vecMatA1 = vld1q(pMat1Vec); 
            pMat1Vec += 16;
            vecIn = vld1q(pVec);        
            pVec += 16;

            acc0 = vmladavaq(acc0, vecIn, vecMatA0);
            acc1 = vmladavaq(acc1, vecIn, vecMatA1);

            blkCnt--;
        }

        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 0xF;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp8q(blkCnt);

            vecMatA0 = vld1q(pMat0Vec);
            vecMatA1 = vld1q(pMat1Vec);
            vecIn = vldrbq_z_s8(pVec, p0);

            acc0 = vmladavaq(acc0, vecIn, vecMatA0);
            acc1 = vmladavaq(acc1, vecIn, vecMatA1);
        }

        *px++ = __SSAT(acc0 >> 7, 8);
        *px++ = __SSAT(acc1 >> 7, 8);

        pMatSrc += numCols * 2;
        /*
         * Decrement the row loop counter
         */
        row -= 2;
    }

    if (row >= 1)
    {
        q7_t const  *pMat0Vec, *pVec;
        q7_t const  *pSrcVecPtr = pSrcVec;
        q31_t         acc0;
        q7x16_t     vecMatA0, vecIn;

        /*
         * For every row wise process, the pInVec pointer is set
         * to the starting address of the vector
         */
        pVec = pSrcVec;

        /*
         * Initialize the pointer pIn1 to point to the starting address of the column being processed
         */
        pMat0 = pMatSrc;

        acc0 = 0LL;

        pMat0Vec = pMat0;
        pVec = pSrcVecPtr;

        blkCnt = numCols >> 4;
        while (blkCnt > 0U)
        {
            vecMatA0 = vld1q(pMat0Vec); 
            pMat0Vec += 16;
            vecIn = vld1q(pVec);        
            pVec += 16;

            acc0 = vmladavaq(acc0, vecIn, vecMatA0);
            blkCnt--;
        }
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 0xF;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp8q(blkCnt);

            vecMatA0 = vld1q(pMat0Vec);
            vecIn = vldrbq_z_s8(pVec, p0);
            acc0 = vmladavaq(acc0, vecIn, vecMatA0);
        }
        *px++ = __SSAT(acc0 >> 7, 8);
    }
}

#else

#if defined(ARM_MATH_NEON)

#define TMP_DEFINE_AND_INIT(TMP) \
   int32x4_t TMP = vdupq_n_s32(0); \
   int16x8_t TMP##16 = vdupq_n_s32(0);

#define REDUCE(sum,accum)                        \
    tmp = vqaddq_s32(accum.val[0],accum.val[1]); \
    tmp = vqaddq_s32(tmp,accum.val[2]);          \
    tmp = vqaddq_s32(tmp,accum.val[3]);          \
    sum = vgetq_lane_s32(tmp,0) +          \
    vgetq_lane_s32(tmp,1) +                      \
    vgetq_lane_s32(tmp,2) +                      \
    vgetq_lane_s32(tmp,3) 

#define MAT_SCALAR_DT q7_t 
#define VEC_SCALAR_DT q7_t 
#define VECTOR_ACC struct { \
    int32x4_t val[4];       \
}

#define VECTOR_DT int8x16_t
#define SCALAR_ACC int32_t
#define HALF_VECTOR_ACC int8x4_t
#define NBLANE 16
#define NBLANE_SHIFT 4

#define VECTOR_ACC_INIT(acc)      \
    acc.val[0] = vdupq_n_s32(0) ; \
    acc.val[1] = vdupq_n_s32(0) ; \
    acc.val[2] = vdupq_n_s32(0) ; \
    acc.val[3] = vdupq_n_s32(0) ;

#define SCALAR_ACC_INIT(acc) \
    acc = 0
  
#define VEC_LOAD(v,p) \
    v = vld1q_s8((p))


#define VMAC(ACC,VA,VB)                                              \
   tmp16 = vmull_s8(vget_low_s8(VA),vget_low_s8(VB));                 \
   ACC.val[0] = vaddq_s32(ACC.val[0],vmovl_s16(vget_low_s16(tmp16)));  \
   ACC.val[1] = vaddq_s32(ACC.val[1],vmovl_s16(vget_high_s16(tmp16))); \
   tmp16 = vmull_s8(vget_high_s8(VA),vget_high_s8(VB));               \
   ACC.val[2] = vaddq_s32(ACC.val[2],vmovl_s16(vget_low_s16(tmp16)));  \
   ACC.val[3] = vaddq_s32(ACC.val[3],vmovl_s16(vget_high_s16(tmp16)));



#define SCALAR_MAC(ACC,MAT,VEC) \
    ACC = ACC + (int32_t)(MAT) * (int32_t)(VEC)

#define STORE_SCALAR_ACC(DST,ACC) \
  DST = __SSAT(ACC>>7,8)

#define FUNCNAME arm_mat_vec_mult_q7
#define MATRIX_TYPE arm_matrix_instance_q7

#include "_arm_mat_vec_mult_neon.c"


#else

ARM_DSP_ATTRIBUTE void arm_mat_vec_mult_q7(const arm_matrix_instance_q7 *pSrcMat, const q7_t *pVec, q7_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const q7_t *pSrcA = pSrcMat->pData;
    const q7_t *pInA1;       /* input data matrix pointer of Q7 type */
    const q7_t *pInA2;       /* input data matrix pointer of Q7 type */
    const q7_t *pInA3;       /* input data matrix pointer of Q7 type */
    const q7_t *pInA4;       /* input data matrix pointer of Q7 type */
    const q7_t *pInVec;      /* input data vector pointer of Q7 type */
    q7_t *px;                /* output data pointer */
    uint32_t i, row, colCnt; /* loop counters */

    q31_t matData, matData2, vecData, vecData2;


    /* Process 4 rows at a time */
    row = numRows >> 2;
    i = 0u;
    px = pDst;



    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    while (row > 0) {
        /* Initialize accumulators */
        q31_t sum1 = 0;
        q31_t sum2 = 0;
        q31_t sum3 = 0;
        q31_t sum4 = 0;

        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */
        pInVec = pVec;

        /* Loop unrolling: process 4 columns per iteration */
        colCnt = numCols >> 2;

        /* Initialize row pointers so we can track 4 rows at once */
        pInA1 = pSrcA + i;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        pInA4 = pInA3 + numCols;


        // Inner loop: matrix-vector multiplication

        while (colCnt > 0u) {
            // Read 4 values from vector
            vecData = read_q7x4_ia (&pInVec);
            vecData2 = __SXTB16(__ROR(vecData, 8));
            vecData = __SXTB16(vecData);
            // Read 16 values from the matrix - 4 values from each of 4 rows, and do multiply accumulate
            matData = read_q7x4_ia (&pInA1);
            matData2 = __SXTB16(__ROR(matData, 8));
            matData = __SXTB16(matData);
            sum1 = __SMLAD(matData, vecData, sum1);
            sum1 = __SMLAD(matData2, vecData2, sum1);
            matData = read_q7x4_ia (&pInA2);
            matData2 = __SXTB16(__ROR(matData, 8));
            matData = __SXTB16(matData);
            sum2 = __SMLAD(matData, vecData, sum2);
            sum2 = __SMLAD(matData2, vecData2, sum2);
            matData = read_q7x4_ia (&pInA3);
            matData2 = __SXTB16(__ROR(matData, 8));
            matData = __SXTB16(matData);
            sum3 = __SMLAD(matData, vecData, sum3);
            sum3 = __SMLAD(matData2, vecData2, sum3);
            matData = read_q7x4_ia (&pInA4);
            matData2 = __SXTB16(__ROR(matData, 8));
            matData = __SXTB16(matData);
            sum4 = __SMLAD(matData, vecData, sum4);
            sum4 = __SMLAD(matData2, vecData2, sum4);

            // Decrement the loop counter
            colCnt--;
        }

        /* process any remaining columns */

        colCnt = numCols & 3u;

        while (colCnt > 0) {
            vecData = *pInVec++;
            sum1 += *pInA1++ * vecData;
            sum2 += *pInA2++ * vecData;
            sum3 += *pInA3++ * vecData;
            sum4 += *pInA4++ * vecData;
            colCnt--;
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = (q7_t)(__SSAT((sum1 >> 7), 8));
        *px++ = (q7_t)(__SSAT((sum2 >> 7), 8));
        *px++ = (q7_t)(__SSAT((sum3 >> 7), 8));
        *px++ = (q7_t)(__SSAT((sum4 >> 7), 8));

        i = i + numCols * 4;

        /* Decrement the row loop counter */
        row--;
    }

    /* process any remaining rows */
    row = numRows & 3u;
    while (row > 0) {

        q31_t sum = 0;
        pInVec = pVec;
        pInA1 = pSrcA + i;

        // loop unrolling - process 4 elements at a time
        colCnt = numCols >> 2;

        while (colCnt > 0) {
            vecData = read_q7x4_ia (&pInVec);
            vecData2 = __SXTB16(__ROR(vecData, 8));
            vecData = __SXTB16(vecData);
            matData = read_q7x4_ia (&pInA1);
            matData2 = __SXTB16(__ROR(matData, 8));
            matData = __SXTB16(matData);
            sum = __SMLAD(matData, vecData, sum);
            sum = __SMLAD(matData2, vecData2, sum);
            colCnt--;
        }

        // process remainder of row
        colCnt = numCols & 3u;
        while (colCnt > 0) {
            sum += *pInA1++ * *pInVec++;
            colCnt--;
        }
        *px++ = (q7_t)(__SSAT((sum >> 7), 8));
        i = i + numCols;
        row--;
    }
}
#endif /* defined(ARM_MATH_NEON) */
#endif /* defined(ARM_MATH_MVEI) */

/**
 * @} end of MatrixMult group
 */
