/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_vec_mult_q15.c
 * Description:  Q15 matrix and vector multiplication
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
 * @brief Q15 matrix and vector multiplication.
 * @param[in]       *pSrcMat points to the input matrix structure
 * @param[in]       *pVec points to input vector
 * @param[out]      *pDst points to output vector
 */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

ARM_DSP_ATTRIBUTE void arm_mat_vec_mult_q15(
    const arm_matrix_instance_q15 * pSrcMat,
    const q15_t     *pSrcVec,
    q15_t           *pDstVec)
{
    const q15_t *pMatSrc = pSrcMat->pData;
    const q15_t *pMat0, *pMat1;
    uint32_t     numRows = pSrcMat->numRows;
    uint32_t     numCols = pSrcMat->numCols;
    q15_t       *px;
    int32_t      row;
    uint16_t     blkCnt;           /* loop counters */

    row = numRows;
    px = pDstVec;

    /*
     * compute 3x64-bit accumulators per loop
     */
    while (row >= 3)
    {
        q15_t const *pMat0Vec, *pMat1Vec, *pMat2Vec, *pVec;
        const q15_t  *pMat2;
        q15_t const  *pSrcVecPtr = pSrcVec;
        q63_t         acc0, acc1, acc2;
        q15x8_t     vecMatA0, vecMatA1, vecMatA2, vecIn;


        pVec = pSrcVec;
        /*
         * Initialize the pointer pIn1 to point to the starting address of the column being processed
         */
        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;
        pMat2 = pMat1 + numCols;

        acc0 = 0LL;
        acc1 = 0LL;
        acc2 = 0LL;

        pMat0Vec = pMat0;
        pMat1Vec = pMat1;
        pMat2Vec = pMat2;
        pVec = pSrcVecPtr;

        blkCnt = numCols >> 3;
        while (blkCnt > 0U)
        {
            vecMatA0 = vld1q(pMat0Vec); 
            pMat0Vec += 8;
            vecMatA1 = vld1q(pMat1Vec); 
            pMat1Vec += 8;
            vecMatA2 = vld1q(pMat2Vec); 
            pMat2Vec += 8;
            vecIn = vld1q(pVec);        
            pVec += 8;

            acc0 = vmlaldavaq(acc0, vecIn, vecMatA0);
            acc1 = vmlaldavaq(acc1, vecIn, vecMatA1);
            acc2 = vmlaldavaq(acc2, vecIn, vecMatA2);

            blkCnt--;
        }
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 7;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp16q(blkCnt);

            vecMatA0 = vld1q(pMat0Vec);
            vecMatA1 = vld1q(pMat1Vec);
            vecMatA2 = vld1q(pMat2Vec);
            vecIn = vldrhq_z_s16(pVec, p0);

            acc0 = vmlaldavaq(acc0, vecIn, vecMatA0);
            acc1 = vmlaldavaq(acc1, vecIn, vecMatA1);
            acc2 = vmlaldavaq(acc2, vecIn, vecMatA2);
        }

        *px++ = MVE_ASRL_SAT16(acc0, 15);
        *px++ = MVE_ASRL_SAT16(acc1, 15);
        *px++ = MVE_ASRL_SAT16(acc2, 15);

        pMatSrc += numCols * 3;
        /*
         * Decrement the row loop counter
         */
        row -= 3;
    }

    /*
     * process any remaining rows pair
     */
    if (row >= 2)
    {
        q15_t const *pMat0Vec, *pMat1Vec, *pVec;
        q15_t const  *pSrcVecPtr = pSrcVec;
        q63_t         acc0, acc1;
        q15x8_t     vecMatA0, vecMatA1, vecIn;

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

        acc0 = 0LL;
        acc1 = 0LL;

        pMat0Vec = pMat0;
        pMat1Vec = pMat1;
        pVec = pSrcVecPtr;

        blkCnt = numCols >> 3;
        while (blkCnt > 0U)
        {
            vecMatA0 = vld1q(pMat0Vec); 
            pMat0Vec += 8;
            vecMatA1 = vld1q(pMat1Vec); 
            pMat1Vec += 8;
            vecIn = vld1q(pVec);        
            pVec += 8;

            acc0 = vmlaldavaq(acc0, vecIn, vecMatA0);
            acc1 = vmlaldavaq(acc1, vecIn, vecMatA1);

            blkCnt--;
        }

        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 7;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp16q(blkCnt);

            vecMatA0 = vld1q(pMat0Vec);
            vecMatA1 = vld1q(pMat1Vec);
            vecIn = vldrhq_z_s16(pVec, p0);

            acc0 = vmlaldavaq(acc0, vecIn, vecMatA0);
            acc1 = vmlaldavaq(acc1, vecIn, vecMatA1);
        }

        *px++ = MVE_ASRL_SAT16(acc0, 15);
        *px++ = MVE_ASRL_SAT16(acc1, 15);

        pMatSrc += numCols * 2;
        /*
         * Decrement the row loop counter
         */
        row -= 2;
    }

    if (row >= 1)
    {
        q15_t const *pMat0Vec, *pVec;
        q15_t const  *pSrcVecPtr = pSrcVec;
        q63_t         acc0;
        q15x8_t     vecMatA0, vecIn;

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

        blkCnt = numCols >> 3;
        while (blkCnt > 0U)
        {
            vecMatA0 = vld1q(pMat0Vec); 
            pMat0Vec += 8;
            vecIn = vld1q(pVec);        
            pVec += 8;
            acc0 = vmlaldavaq(acc0, vecIn, vecMatA0);
            blkCnt--;
        }
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 7;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp16q(blkCnt);

            vecMatA0 = vld1q(pMat0Vec);
            vecIn = vldrhq_z_s16(pVec, p0);
            acc0 = vmlaldavaq(acc0, vecIn, vecMatA0);
        }
        *px++ = MVE_ASRL_SAT16(acc0, 15);
    }
}

#else

#if defined(ARM_MATH_NEON)

#define TMP_DEFINE_AND_INIT(TMP) \
   int32x4_t TMP##1 = vdupq_n_s32(0); 
   

#define REDUCE(sum,accum)                                    \
    tmp1 = vqaddq_s64(accum.val[0],accum.val[1]);            \
    tmp1 = vqaddq_s64(tmp1,accum.val[2]);                    \
    tmp1 = vqaddq_s64(tmp1,accum.val[3]);                    \
    sum = vgetq_lane_s64(tmp1,0) +   vgetq_lane_s64(tmp1,1);


#define MAT_SCALAR_DT q15_t 
#define VEC_SCALAR_DT q15_t 
#define VECTOR_ACC struct { \
    int64x2_t val[4];       \
}

#define VECTOR_DT int16x8_t
#define SCALAR_ACC int64_t
#define HALF_VECTOR_ACC int16x4_t
#define NBLANE 8
#define NBLANE_SHIFT 3

#define VECTOR_ACC_INIT(acc)      \
    acc.val[0] = vdupq_n_s64(0) ; \
    acc.val[1] = vdupq_n_s64(0) ; \
    acc.val[2] = vdupq_n_s64(0) ; \
    acc.val[3] = vdupq_n_s64(0) ;

#define SCALAR_ACC_INIT(acc) \
    acc = 0
  
#define VEC_LOAD(v,p) \
    v = vld1q_s16((p))


#define VMAC(ACC,VA,VB)                                               \
   tmp1 = vmull_s16(vget_low_s32(VA),vget_low_s32(VB));                \
   ACC.val[0] = vaddq_s64(ACC.val[0],vmovl_s32(vget_low_s32(tmp1)));   \
   ACC.val[1] = vaddq_s64(ACC.val[1],vmovl_s32(vget_high_s32(tmp1)));  \
   tmp1 = vmull_s16(vget_high_s32(VA),vget_high_s32(VB));              \
   ACC.val[2] = vaddq_s64(ACC.val[2],vmovl_s32(vget_low_s32(tmp1)));   \
   ACC.val[3] = vaddq_s64(ACC.val[3],vmovl_s32(vget_high_s32(tmp1)));



#define SCALAR_MAC(ACC,MAT,VEC) \
    ACC = ACC + (int64_t)(MAT) * (int64_t)(VEC)

#define STORE_SCALAR_ACC(DST,ACC) \
  DST = __SSAT(ACC>>15,16)

#define FUNCNAME arm_mat_vec_mult_q15
#define MATRIX_TYPE arm_matrix_instance_q15

#include "_arm_mat_vec_mult_neon.c"


#else
ARM_DSP_ATTRIBUTE void arm_mat_vec_mult_q15(const arm_matrix_instance_q15 *pSrcMat, const q15_t *pVec, q15_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const q15_t *pSrcA = pSrcMat->pData;
    const q15_t *pInA1;      /* input data matrix pointer A of Q15 type */
    const q15_t *pInA2;      /* input data matrix pointer A of Q15 type */
    const q15_t *pInA3;      /* input data matrix pointer A of Q15 type */
    const q15_t *pInA4;      /* input data matrix pointer A of Q15 type */
    const q15_t *pInVec;     /* input data matrix pointer B of Q15 type */
    q15_t *px;               /* Temporary output data matrix pointer */
    uint16_t i, row, colCnt; /* loop counters */
    q31_t matData, matData2, vecData, vecData2;


    /* Process 4 rows at a time */
    row = numRows >> 2;
    i = 0u;
    px = pDst;

    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    /* row loop */
    while (row > 0) {
        /* Initialize accumulators */
        q63_t sum1 = 0;
        q63_t sum2 = 0;
        q63_t sum3 = 0;
        q63_t sum4 = 0;

        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */
        pInVec = pVec;

        /* Loop unrolling: process 2 columns per iteration */
        colCnt = numCols >> 1;

        /* Initialize pointers to the starting address of the column being processed */
        pInA1 = pSrcA + i;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        pInA4 = pInA3 + numCols;

        // Main loop: matrix-vector multiplication
        while (colCnt > 0u) {
            // Read 2 values from vector
            vecData = read_q15x2_ia (&pInVec);

            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData =  read_q15x2_ia (&pInA1);
            sum1 = __SMLALD(matData, vecData, sum1);
            matData = read_q15x2_ia (&pInA2);
            sum2 = __SMLALD(matData, vecData, sum2);
            matData = read_q15x2_ia (&pInA3);
            sum3 = __SMLALD(matData, vecData, sum3);
            matData = read_q15x2_ia (&pInA4);
            sum4 = __SMLALD(matData, vecData, sum4);

            // Decrement the loop counter
            colCnt--;
        }

        /* process any remaining columns */
        colCnt = numCols & 1u;
        if (numCols & 1u) {
            vecData = *pInVec++;
            sum1 += (q63_t)*pInA1++ * vecData;
            sum2 += (q63_t)*pInA2++ * vecData;
            sum3 += (q63_t)*pInA3++ * vecData;
            sum4 += (q63_t)*pInA4++ * vecData;
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = (q15_t)(__SSAT((sum1 >> 15), 16));
        *px++ = (q15_t)(__SSAT((sum2 >> 15), 16));
        *px++ = (q15_t)(__SSAT((sum3 >> 15), 16));
        *px++ = (q15_t)(__SSAT((sum4 >> 15), 16));

        i = i + numCols * 4;

        /* Decrement the row loop counter */
        row--;
    }

    /* process any remaining rows */
    row = numRows & 3u;
    while (row > 0) {

        q63_t sum = 0;
        pInVec = pVec;
        pInA1 = pSrcA + i;

        // loop unrolling - process 4 elements at a time
        colCnt = numCols >> 2;

        while (colCnt > 0) {
            vecData = read_q15x2_ia (&pInVec);
            vecData2 = read_q15x2_ia (&pInVec);
            matData = read_q15x2_ia (&pInA1);
            matData2 = read_q15x2_ia (&pInA1);
            sum = __SMLALD(matData, vecData, sum);
            sum = __SMLALD(matData2, vecData2, sum);
            colCnt--;
        }

        // process remainder of row
        colCnt = numCols & 3u;
        while (colCnt > 0) {
            sum += (q63_t)*pInA1++ * *pInVec++;
            colCnt--;
        }
        *px++ = (q15_t)(__SSAT((sum >> 15), 16));
        i = i + numCols;
        row--;
    }
}
#endif /* ARM_MATH_NEON */
#endif /* defined(ARM_MATH_MVEI) */

/**
 * @} end of MatrixMult group
 */
