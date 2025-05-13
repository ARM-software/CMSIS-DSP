/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_vec_mult_f32.c
 * Description:  Floating-point matrix and vector multiplication
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

#include "dsp/matrix_functions.h"


/**
 * @ingroup groupMatrix
 */

/**
 * @defgroup MatrixVectMult Matrix Vector Multiplication
 *
 * Multiplies a matrix and a vector.
 *
 */

/**
 * @addtogroup MatrixVectMult
 * @{
 */

/**
 * @brief Floating-point matrix and vector multiplication.
 * @param[in]       *pSrcMat points to the input matrix structure
 * @param[in]       *pVec points to input vector
 * @param[out]      *pDst points to output vector
 */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

ARM_DSP_ATTRIBUTE void arm_mat_vec_mult_f32(
    const arm_matrix_instance_f32   *pSrcMat,
    const float32_t                 *pSrcVec,
    float32_t                       *pDstVec)
{
    uint32_t         numRows = pSrcMat->numRows;
    uint32_t         numCols = pSrcMat->numCols;
    const float32_t *pSrcA = pSrcMat->pData;
    const float32_t *pInA0;
    const float32_t *pInA1;
    float32_t       *px;
    int32_t          row;
    uint32_t         blkCnt;           /* loop counters */

    row = numRows;
    px = pDstVec;

    /*
     * compute 4 rows in parallel
     */
    while (row >= 4)
    {
        const float32_t     *pInA2, *pInA3;
        float32_t const    *pSrcA0Vec, *pSrcA1Vec, *pSrcA2Vec, *pSrcA3Vec, *pInVec;
        f32x4_t            vecIn, acc0, acc1, acc2, acc3;
        float32_t const     *pSrcVecPtr = pSrcVec;

        /*
         * Initialize the pointers to 4 consecutive MatrixA rows
         */
        pInA0 = pSrcA;
        pInA1 = pInA0 + numCols;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        /*
         * Initialize the vector pointer
         */
        pInVec =  pSrcVecPtr;
        /*
         * reset accumulators
         */
        acc0 = vdupq_n_f32(0.0f);
        acc1 = vdupq_n_f32(0.0f);
        acc2 = vdupq_n_f32(0.0f);
        acc3 = vdupq_n_f32(0.0f);

        pSrcA0Vec = pInA0;
        pSrcA1Vec = pInA1;
        pSrcA2Vec = pInA2;
        pSrcA3Vec = pInA3;

        blkCnt = numCols >> 2;
        while (blkCnt > 0U)
        {
            f32x4_t vecA;

            vecIn = vld1q(pInVec);      
            pInVec += 4;
            vecA = vld1q(pSrcA0Vec);    
            pSrcA0Vec += 4;
            acc0 = vfmaq(acc0, vecIn, vecA);
            vecA = vld1q(pSrcA1Vec);  
            pSrcA1Vec += 4;
            acc1 = vfmaq(acc1, vecIn, vecA);
            vecA = vld1q(pSrcA2Vec);  
            pSrcA2Vec += 4;
            acc2 = vfmaq(acc2, vecIn, vecA);
            vecA = vld1q(pSrcA3Vec);  
            pSrcA3Vec += 4;
            acc3 = vfmaq(acc3, vecIn, vecA);

            blkCnt--;
        }
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 3;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp32q(blkCnt);
            f32x4_t vecA;

            vecIn = vldrwq_z_f32(pInVec, p0);
            vecA = vld1q(pSrcA0Vec);
            acc0 = vfmaq(acc0, vecIn, vecA);
            vecA = vld1q(pSrcA1Vec);
            acc1 = vfmaq(acc1, vecIn, vecA);
            vecA = vld1q(pSrcA2Vec);
            acc2 = vfmaq(acc2, vecIn, vecA);
            vecA = vld1q(pSrcA3Vec);
            acc3 = vfmaq(acc3, vecIn, vecA);
        }
        /*
         * Sum the partial parts
         */
        *px++ = vecAddAcrossF32Mve(acc0);
        *px++ = vecAddAcrossF32Mve(acc1);
        *px++ = vecAddAcrossF32Mve(acc2);
        *px++ = vecAddAcrossF32Mve(acc3);

        pSrcA += numCols * 4;
        /*
         * Decrement the row loop counter
         */
        row -= 4;
    }

    /*
     * compute 2 rows in parallel
     */
    if (row >= 2)
    {
        float32_t const    *pSrcA0Vec, *pSrcA1Vec, *pInVec;
        f32x4_t            vecIn, acc0, acc1;
        float32_t const     *pSrcVecPtr = pSrcVec;

        /*
         * Initialize the pointers to 2 consecutive MatrixA rows
         */
        pInA0 = pSrcA;
        pInA1 = pInA0 + numCols;
        /*
         * Initialize the vector pointer
         */
        pInVec = pSrcVecPtr;
        /*
         * reset accumulators
         */
        acc0 = vdupq_n_f32(0.0f);
        acc1 = vdupq_n_f32(0.0f);
        pSrcA0Vec = pInA0;
        pSrcA1Vec = pInA1;

        blkCnt = numCols >> 2;
        while (blkCnt > 0U)
        {
            f32x4_t vecA;

            vecIn = vld1q(pInVec);      
            pInVec += 4;
            vecA = vld1q(pSrcA0Vec);    
            pSrcA0Vec += 4;
            acc0 = vfmaq(acc0, vecIn, vecA);
            vecA = vld1q(pSrcA1Vec);    
            pSrcA1Vec += 4;
            acc1 = vfmaq(acc1, vecIn, vecA);

            blkCnt--;
        }
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 3;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp32q(blkCnt);
            f32x4_t vecA;

            vecIn = vldrwq_z_f32(pInVec, p0);
            vecA = vld1q(pSrcA0Vec);
            acc0 = vfmaq(acc0, vecIn, vecA);
            vecA = vld1q(pSrcA1Vec);
            acc1 = vfmaq(acc1, vecIn, vecA);
        }
        /*
         * Sum the partial parts
         */
        *px++ = vecAddAcrossF32Mve(acc0);
        *px++ = vecAddAcrossF32Mve(acc1);

        pSrcA += numCols * 2;
        row -= 2;
    }

    if (row >= 1)
    {
        f32x4_t             vecIn, acc0;
        float32_t const     *pSrcA0Vec, *pInVec;
        float32_t const      *pSrcVecPtr = pSrcVec;
        /*
         * Initialize the pointers to last MatrixA row
         */
        pInA0 = pSrcA;
        /*
         * Initialize the vector pointer
         */
        pInVec = pSrcVecPtr;
        /*
         * reset accumulators
         */
        acc0 = vdupq_n_f32(0.0f);

        pSrcA0Vec = pInA0;

        blkCnt = numCols >> 2;
        while (blkCnt > 0U)
        {
            f32x4_t vecA;

            vecIn = vld1q(pInVec);      
            pInVec += 4;
            vecA = vld1q(pSrcA0Vec);    
            pSrcA0Vec += 4;
            acc0 = vfmaq(acc0, vecIn, vecA);

            blkCnt--;
        }
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 3;
        if (blkCnt > 0U)
        {
            mve_pred16_t p0 = vctp32q(blkCnt);
            f32x4_t vecA;

            vecIn = vldrwq_z_f32(pInVec, p0);
            vecA = vld1q(pSrcA0Vec);
            acc0 = vfmaq(acc0, vecIn, vecA);
        }
        /*
         * Sum the partial parts
         */
        *px++ = vecAddAcrossF32Mve(acc0);
    }
}
#else

#if defined(ARM_MATH_NEON)

#if defined(__aarch64__)
#define vecAccrossF32Neon(sum,accum) \
    sum = vpadds_f32(vpadd_f32(vget_low_f32(accum), vget_high_f32(accum)));
#else
#define vecAccrossF32Neon(sum,accum)                                \
    tmp = vpadd_f32(vget_low_f32(accum), vget_high_f32(accum)); \
    sum = vget_lane_f32(tmp, 0) + vget_lane_f32(tmp, 1);
#endif 

ARM_DSP_ATTRIBUTE void arm_mat_vec_mult_f32(const arm_matrix_instance_f32 *pSrcMat, 
                                            const float32_t *pSrcVec, 
                                            float32_t *pDstVec)
{
    uint32_t         numRows = pSrcMat->numRows;
    uint32_t         numCols = pSrcMat->numCols;
    const float32_t *pSrcA = pSrcMat->pData;
    const float32_t *pInA0;
    const float32_t *pInA1;
    int32_t          row;
    uint32_t         blkCnt;           /* loop counters */
#if !defined(__aarch64__)
    float32x2_t tmp = vdup_n_f32(0); 
#endif  
    

    row = numRows;

    /*
     * compute 4 rows in parallel
     */
    while (row >= 4)
    {
        const float32_t     *pInA2, *pInA3;
        float32_t const     *pSrcA0, *pSrcA1, *pSrcA2, *pSrcA3, *pInVec;
        float32x4_t         vecIn, acc0, acc1, acc2, acc3;
        float32_t           tmpScalarAcc0;
        float32_t           tmpScalarAcc1;
        float32_t           tmpScalarAcc2;
        float32_t           tmpScalarAcc3;

        /*
         * Initialize the pointers to 4 consecutive MatrixA rows
         */
        pInA0 = pSrcA;
        pInA1 = pInA0 + numCols;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        /*
         * Initialize the vector pointer
         */
        pInVec =  pSrcVec;
        /*
         * reset accumulators
         */
        acc0 = vdupq_n_f32(0.0f);
        acc1 = vdupq_n_f32(0.0f);
        acc2 = vdupq_n_f32(0.0f);
        acc3 = vdupq_n_f32(0.0f);

        tmpScalarAcc0 = 0.0f;
        tmpScalarAcc1 = 0.0f;
        tmpScalarAcc2 = 0.0f;
        tmpScalarAcc3 = 0.0f;

        pSrcA0 = pInA0;
        pSrcA1 = pInA1;
        pSrcA2 = pInA2;
        pSrcA3 = pInA3;

        blkCnt = numCols >> 2;
        while (blkCnt > 0U)
        {
            float32x4_t vecA0, vecA1, vecA2, vecA3;

            vecIn = vld1q_f32(pInVec);      
            pInVec += 4;

            vecA0 = vld1q_f32(pSrcA0);    
            pSrcA0 += 4;
            vecA1 = vld1q_f32(pSrcA1);  
            pSrcA1 += 4;
            vecA2 = vld1q_f32(pSrcA2);  
            pSrcA2 += 4;
            vecA3 = vld1q_f32(pSrcA3);  
            pSrcA3 += 4;

            acc0 = vmlaq_f32(acc0, vecIn, vecA0);
            acc1 = vmlaq_f32(acc1, vecIn, vecA1);
            acc2 = vmlaq_f32(acc2, vecIn, vecA2);
            acc3 = vmlaq_f32(acc3, vecIn, vecA3);

            blkCnt--;
        }

        vecAccrossF32Neon(tmpScalarAcc0 , acc0);
        vecAccrossF32Neon(tmpScalarAcc1 , acc1);
        vecAccrossF32Neon(tmpScalarAcc2 , acc2);
        vecAccrossF32Neon(tmpScalarAcc3 , acc3);
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 3;
        while (blkCnt > 0U)
        {
            tmpScalarAcc0 += *pSrcA0++ * *pInVec;
            tmpScalarAcc1 += *pSrcA1++ * *pInVec;
            tmpScalarAcc2 += *pSrcA2++ * *pInVec;
            tmpScalarAcc3 += *pSrcA3++ * *pInVec;
            pInVec++;

            blkCnt--;
        }
        /*
         * Sum the partial parts
         */
        *pDstVec++ = tmpScalarAcc0;
        *pDstVec++ = tmpScalarAcc1;
        *pDstVec++ = tmpScalarAcc2;
        *pDstVec++ = tmpScalarAcc3;

        pSrcA += numCols * 4;
        /*
         * Decrement the row loop counter
         */
        row -= 4;
    }

    /*
     * compute 2 rows in parallel
     */
    if (row >= 2)
    {
        float32_t const    *pSrcA0, *pSrcA1, *pInVec;
        float32x4_t         vecIn, acc0, acc1;
        float32_t           tmpScalarAcc0;
        float32_t           tmpScalarAcc1;

        /*
         * Initialize the pointers to 2 consecutive MatrixA rows
         */
        pInA0 = pSrcA;
        pInA1 = pInA0 + numCols;
        /*
         * Initialize the vector pointer
         */
        pInVec = pSrcVec;
        /*
         * reset accumulators
         */
        acc0 = vdupq_n_f32(0.0f);
        acc1 = vdupq_n_f32(0.0f);

        tmpScalarAcc0 = 0.0f;
        tmpScalarAcc1 = 0.0f;

        pSrcA0 = pInA0;
        pSrcA1 = pInA1;

        blkCnt = numCols >> 2;
        while (blkCnt > 0U)
        {
            float32x4_t vecA0, vecA1;

            vecIn = vld1q_f32(pInVec);      
            pInVec += 4;

            vecA0 = vld1q_f32(pSrcA0);    
            pSrcA0 += 4;
            vecA1 = vld1q_f32(pSrcA1);    
            pSrcA1 += 4;

            acc0 = vmlaq_f32(acc0, vecIn, vecA0);
            acc1 = vmlaq_f32(acc1, vecIn, vecA1);

            blkCnt--;
        }

        vecAccrossF32Neon(tmpScalarAcc0 , acc0);
        vecAccrossF32Neon(tmpScalarAcc1 , acc1);

        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 3;
        while (blkCnt > 0U)
        {
            tmpScalarAcc0 += *pSrcA0++ * *pInVec;
            tmpScalarAcc1 += *pSrcA1++ * *pInVec;
           
            pInVec++;

            blkCnt--;
        }
        /*
         * Sum the partial parts
         */
        *pDstVec++ = tmpScalarAcc0;
        *pDstVec++ = tmpScalarAcc1;

        pSrcA += numCols * 2;
        row -= 2;
    }

    if (row >= 1)
    {
        float32x4_t             vecIn, acc0;
        float32_t const         *pSrcA0, *pInVec;
        float32_t               tmpScalarAcc0;

        /*
         * Initialize the pointers to last MatrixA row
         */
        pInA0 = pSrcA;
        /*
         * Initialize the vector pointer
         */
        pInVec = pSrcVec;
        /*
         * reset accumulators
         */
        acc0 = vdupq_n_f32(0.0f);
        tmpScalarAcc0 = 0.0f;

        pSrcA0 = pInA0;

        blkCnt = numCols >> 2;
        while (blkCnt > 0U)
        {
            float32x4_t vecA0;

            vecIn = vld1q_f32(pInVec);      
            pInVec += 4;

            vecA0 = vld1q_f32(pSrcA0);    
            pSrcA0 += 4;

            acc0 = vmlaq_f32(acc0, vecIn, vecA0);

            blkCnt--;
        }
        vecAccrossF32Neon(tmpScalarAcc0 , acc0);

        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & 3;
        while (blkCnt > 0U)
        {

            tmpScalarAcc0 += *pSrcA0++ * *pInVec++;
            
            blkCnt--;
        }
        /*
         * Sum the partial parts
         */
        *pDstVec++ = tmpScalarAcc0;
    }
}

#else
ARM_DSP_ATTRIBUTE void arm_mat_vec_mult_f32(const arm_matrix_instance_f32 *pSrcMat, const float32_t *pVec, float32_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const float32_t *pSrcA = pSrcMat->pData;
    const float32_t *pInA1;      /* input data matrix pointer A of Q31 type */
    const float32_t *pInA2;      /* input data matrix pointer A of Q31 type */
    const float32_t *pInA3;      /* input data matrix pointer A of Q31 type */
    const float32_t *pInA4;      /* input data matrix pointer A of Q31 type */
    const float32_t *pInVec;     /* input data matrix pointer B of Q31 type */
    float32_t *px;               /* Temporary output data matrix pointer */
    uint32_t i;
    uint16_t row, colCnt; /* loop counters */
    float32_t matData, matData2, vecData, vecData2;


    /* Process 4 rows at a time */
    row = numRows >> 2;
    i = 0u;
    px = pDst;

    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    /* row loop */
    while (row > 0) {
        /* Initialize accumulators */
        float32_t sum1 = 0.0f;
        float32_t sum2 = 0.0f;
        float32_t sum3 = 0.0f;
        float32_t sum4 = 0.0f;

        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */
        pInVec = pVec;

        /* Loop unrolling: process 2 columns per iteration */
        colCnt = numCols;

        /* Initialize pointers to the starting address of the column being processed */
        pInA1 = pSrcA + i;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        pInA4 = pInA3 + numCols;


        // Main loop: matrix-vector multiplication
        while (colCnt > 0u) {
            // Read 2 values from vector
            vecData = *(pInVec)++;
            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData = *(pInA1)++;
            sum1 += matData * vecData;
            matData = *(pInA2)++;
            sum2 += matData * vecData;
            matData = *(pInA3)++;
            sum3 += matData * vecData;
            matData = *(pInA4)++;
            sum4 += matData * vecData;

            // Decrement the loop counter
            colCnt--;
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = sum1;
        *px++ = sum2;
        *px++ = sum3;
        *px++ = sum4;

        i = i + numCols * 4;

        /* Decrement the row loop counter */
        row--;
    }

    /* process any remaining rows */
    row = numRows & 3u;
    while (row > 0) {

        float32_t sum = 0.0f;
        pInVec = pVec;
        pInA1 = pSrcA + i;

        colCnt = numCols >> 1;
        while (colCnt > 0) {
            vecData = *(pInVec)++;
            vecData2 = *(pInVec)++;
            matData = *(pInA1)++;
            matData2 = *(pInA1)++;
            sum += matData * vecData;
            sum += matData2 * vecData2;
            colCnt--;
        }
        // process remainder of row
        colCnt = numCols & 1u;


        while (colCnt > 0) {
            sum += *pInA1++ * *pInVec++;
            colCnt--;
        }

        *px++ = sum;
        i = i + numCols;
        row--;
    }
}
#endif /* defined(ARM_MATH_NEON) */
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
 * @} end of MatrixMult group
 */
