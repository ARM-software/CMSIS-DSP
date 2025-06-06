/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_cholesky_f64.c
 * Description:  Floating-point Cholesky decomposition
 *
 * $Date:        10 August 2022
 * $Revision:    V1.9.1
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
#include "dsp/matrix_utils.h"

/**
  @ingroup groupMatrix
 */

/**
  @addtogroup MatrixChol
  @{
 */

/**
   * @brief Floating-point Cholesky decomposition of positive-definite matrix.
   * @param[in]  pSrc   points to the instance of the input floating-point matrix structure.
   * @param[out] pDst   points to the instance of the output floating-point matrix structure.
   * @return The function returns ARM_MATH_SIZE_MISMATCH, if the dimensions do not match.
   * @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
                   - \ref ARM_MATH_DECOMPOSITION_FAILURE      : Input matrix cannot be decomposed
   * @par
   * If the matrix is ill conditioned or only semi-definite, then it is better using the LDL^t decomposition.
   * The decomposition of A is returning a lower triangular matrix L such that A = L L^t
   *
   * @par
   * The destination matrix should be set to 0 before calling the functions because
   * the function may not overwrite all output elements.
   */

#if defined(ARM_MATH_NEON) && defined(__aarch64__)

ARM_DSP_ATTRIBUTE arm_status arm_mat_cholesky_f64(
    const arm_matrix_instance_f64 * pSrc,
    arm_matrix_instance_f64 * pDst)
{
    
    arm_status status;                             /* status of matrix inverse */
    
    
#ifdef ARM_MATH_MATRIX_CHECK
    
    /* Check for matrix mismatch condition */
    if ((pSrc->numRows != pSrc->numCols) ||
        (pDst->numRows != pDst->numCols) ||
        (pSrc->numRows != pDst->numRows)   )
    {
        /* Set status as ARM_MATH_SIZE_MISMATCH */
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
        
#endif /* #ifdef ARM_MATH_MATRIX_CHECK */
        
    {
        int i,j,k;
        int n = pSrc->numRows;
        float64_t invSqrtVj;
        float64_t *pA,*pG;
        int kCnt;
        
        
        float64x2_t acc, acc0, acc1, acc2, acc3;
        float64x2_t vecGi;
        float64x2_t vecGj,vecGj0,vecGj1,vecGj2,vecGj3;
        
        
        float64_t sum=0.0;
        float64_t sum0=0.0,sum1=0.0,sum2=0.0,sum3=0.0;
        
        
        pA = pSrc->pData;
        pG = pDst->pData;
        
        for(i=0 ;i < n ; i++)
        {
            for(j=i ; j+3 < n ; j+=4)
            {
                pG[(j + 0) * n + i] = pA[(j + 0) * n + i];
                pG[(j + 1) * n + i] = pA[(j + 1) * n + i];
                pG[(j + 2) * n + i] = pA[(j + 2) * n + i];
                pG[(j + 3) * n + i] = pA[(j + 3) * n + i];
                
                acc0 = vdupq_n_f64(0.0);
                acc1 = vdupq_n_f64(0.0);
                acc2 = vdupq_n_f64(0.0);
                acc3 = vdupq_n_f64(0.0);
                
                kCnt = i >> 1U;
                k=0;
                while(kCnt > 0)
                {
                    
                    vecGi=vld1q_f64(&pG[i * n + k]);
                    
                    vecGj0=vld1q_f64(&pG[(j + 0) * n + k]);
                    vecGj1=vld1q_f64(&pG[(j + 1) * n + k]);
                    vecGj2=vld1q_f64(&pG[(j + 2) * n + k]);
                    vecGj3=vld1q_f64(&pG[(j + 3) * n + k]);
                    
                    acc0 = vfmaq_f64(acc0, vecGi, vecGj0);
                    acc1 = vfmaq_f64(acc1, vecGi, vecGj1);
                    acc2 = vfmaq_f64(acc2, vecGi, vecGj2);
                    acc3 = vfmaq_f64(acc3, vecGi, vecGj3);
                    
                    kCnt--;
                    k+=2;
                }
                
                
                sum0 = vaddvq_f64(acc0);
                sum1 = vaddvq_f64(acc1);
                sum2 = vaddvq_f64(acc2);
                sum3 = vaddvq_f64(acc3);
                
                
                kCnt = i & 1;
                while(kCnt > 0)
                {
                    
                    sum0 = sum0 + pG[i * n + k] * pG[(j + 0) * n + k];
                    sum1 = sum1 + pG[i * n + k] * pG[(j + 1) * n + k];
                    sum2 = sum2 + pG[i * n + k] * pG[(j + 2) * n + k];
                    sum3 = sum3 + pG[i * n + k] * pG[(j + 3) * n + k];
                    kCnt--;
                    k++;
                }
                
                pG[(j + 0) * n + i] -= sum0;
                pG[(j + 1) * n + i] -= sum1;
                pG[(j + 2) * n + i] -= sum2;
                pG[(j + 3) * n + i] -= sum3;
            }
            
            for(; j < n ; j++)
            {
                pG[j * n + i] = pA[j * n + i];
                
                acc = vdupq_n_f64(0.0);
                
                kCnt = i >> 1U;
                k=0;
                while(kCnt > 0)
                {
                    
                    vecGi=vld1q_f64(&pG[i * n + k]);
                    vecGj=vld1q_f64(&pG[j * n + k]);
                    
                    acc = vfmaq_f64(acc, vecGi, vecGj);
                    
                    kCnt--;
                    k+=2;
                }
                
                
                sum = vaddvq_f64(acc);
                
                kCnt = i & 1;
                while(kCnt > 0)
                {
                    sum = sum + pG[i * n + k] * pG[(j + 0) * n + k];
                    
                    
                    kCnt--;
                    k++;
                }
                
                pG[j * n + i] -= sum;
            }
            
            if (pG[i * n + i] <= 0.0)
            {
                return(ARM_MATH_DECOMPOSITION_FAILURE);
            }
            
            invSqrtVj = 1.0/sqrt(pG[i * n + i]);
            SCALE_COL_F64(pDst,i,invSqrtVj,i);
        }
        
        status = ARM_MATH_SUCCESS;
        
    }
    
    
    /* Return to application */
    return (status);
}
#else
ARM_DSP_ATTRIBUTE arm_status arm_mat_cholesky_f64(
                                const arm_matrix_instance_f64 * pSrc,
                                arm_matrix_instance_f64 * pDst)
{
    
    arm_status status;                             /* status of matrix inverse */
    
    
#ifdef ARM_MATH_MATRIX_CHECK
    
    /* Check for matrix mismatch condition */
    if ((pSrc->numRows != pSrc->numCols) ||
        (pDst->numRows != pDst->numCols) ||
        (pSrc->numRows != pDst->numRows)   )
    {
        /* Set status as ARM_MATH_SIZE_MISMATCH */
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
        
#endif /* #ifdef ARM_MATH_MATRIX_CHECK */
        
    {
        int i,j,k;
        int n = pSrc->numRows;
        float64_t invSqrtVj;
        float64_t *pA,*pG;
        
        pA = pSrc->pData;
        pG = pDst->pData;
        
        
        for(i=0 ; i < n ; i++)
        {
            for(j=i ; j < n ; j++)
            {
                pG[j * n + i] = pA[j * n + i];
                
                for(k=0; k < i ; k++)
                {
                    pG[j * n + i] = pG[j * n + i] - pG[i * n + k] * pG[j * n + k];
                }
            }
            
            if (pG[i * n + i] <= 0.0)
            {
                return(ARM_MATH_DECOMPOSITION_FAILURE);
            }
            
            invSqrtVj = 1.0/sqrt(pG[i * n + i]);
            SCALE_COL_F64(pDst,i,invSqrtVj,i);
            
        }
        
        status = ARM_MATH_SUCCESS;
        
    }
    
    
    /* Return to application */
    return (status);
}
#endif

/**
  @} end of MatrixChol group
 */
