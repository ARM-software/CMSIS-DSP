/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_solve_upper_triangular_f64.c
 * Description:  Solve linear system UT X = A with UT upper triangular matrix
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
#include "arm_compiler_specific.h"


#include "dsp/matrix_functions.h"


/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixInv
  @{
 */

/**
   * @brief Solve UT . X = A where UT is an upper triangular matrix
   * @param[in]  ut  The upper triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of UT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
  */

#if defined(ARM_MATH_NEON) && defined(__aarch64__)
ARM_DSP_ATTRIBUTE arm_status arm_mat_solve_upper_triangular_f64(
    const arm_matrix_instance_f64 * ut,
    const arm_matrix_instance_f64 * a,
    arm_matrix_instance_f64 * dst)
{
    arm_status status;                             /* status of matrix inverse */
    
    
#ifdef ARM_MATH_MATRIX_CHECK
    
    /* Check for matrix mismatch condition */
    if ((ut->numRows != ut->numCols) ||
        (ut->numRows != a->numRows)   )
    {
        /* Set status as ARM_MATH_SIZE_MISMATCH */
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
        
#endif /* #ifdef ARM_MATH_MATRIX_CHECK */
        
    {
        
        int i,j,k,n,cols;
        
        n = dst->numRows;
        cols = dst->numCols;
        
        float64_t *pX = dst->pData;
        float64_t *pUT = ut->pData;
        float64_t *pA = a->pData;
        
        float64_t *ut_row;
        float64_t *a_col;
        
        float64_t invUT;
        
        float64x2_t vecA;
        float64x2_t vecX;
        
        for(i=n-1; i >= 0 ; i--)
        {
            for(j=0; j+1 < cols; j +=2)
            {
                vecA = vld1q_f64(&pA[i * cols + j]);
                
                for(k=n-1; k > i; k--)
                {
                    vecX = vld1q_f64(&pX[cols*k+j]);
                    vecA = vfmsq_f64(vecA,vdupq_n_f64(pUT[n*i + k]),vecX);
                }
                
                if (pUT[n*i + i]==0.0)
                {
                    return(ARM_MATH_SINGULAR);
                }
                
                invUT = 1.0 / pUT[n*i + i];
                vecA = vmulq_f64(vecA,vdupq_n_f64(invUT));
                
                
                vst1q_f64(&pX[i*cols+j],vecA);
            }
            
            for(; j < cols; j ++)
            {
                a_col = &pA[j];
                
                ut_row = &pUT[n*i];
                
                float64_t tmp=a_col[i * cols];
                
                for(k=n-1; k > i; k--)
                {
                    tmp -= ut_row[k] * pX[cols*k+j];
                }
                
                if (ut_row[i]==0.0)
                {
                    return(ARM_MATH_SINGULAR);
                }
                tmp = tmp / ut_row[i];
                pX[i*cols+j] = tmp;
            }
            
        }
        status = ARM_MATH_SUCCESS;
        
    }
    
    
    /* Return to application */
    return (status);
}

#else
ARM_DSP_ATTRIBUTE arm_status arm_mat_solve_upper_triangular_f64(
    const arm_matrix_instance_f64 * ut,
    const arm_matrix_instance_f64 * a,
    arm_matrix_instance_f64 * dst)
{
    arm_status status;                             /* status of matrix inverse */
    
    
#ifdef ARM_MATH_MATRIX_CHECK
    
    /* Check for matrix mismatch condition */
    if ((ut->numRows != ut->numCols) ||
        (ut->numRows != a->numRows)   )
    {
        /* Set status as ARM_MATH_SIZE_MISMATCH */
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
        
#endif /* #ifdef ARM_MATH_MATRIX_CHECK */
        
    {
        
        int i,j,k,n,cols;
        
        float64_t *pX = dst->pData;
        float64_t *pUT = ut->pData;
        float64_t *pA = a->pData;
        
        float64_t *ut_row;
        float64_t *a_col;
        
        n = dst->numRows;
        cols = dst->numCols;
        
        for(j=0; j < cols; j ++)
        {
            a_col = &pA[j];
            
            for(i=n-1; i >= 0 ; i--)
            {
                float64_t tmp=a_col[i * cols];
                
                ut_row = &pUT[n*i];
                
                for(k=n-1; k > i; k--)
                {
                    tmp -= ut_row[k] * pX[cols*k+j];
                }
                
                if (ut_row[i]==0.0)
                {
                    return(ARM_MATH_SINGULAR);
                }
                tmp = tmp / ut_row[i];
                pX[i*cols+j] = tmp;
            }
            
        }
        status = ARM_MATH_SUCCESS;
        
    }
    
    
    /* Return to application */
    return (status);
}
#endif

/**
  @} end of MatrixInv group
 */
