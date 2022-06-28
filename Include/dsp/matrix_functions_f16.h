/******************************************************************************
 * @file     matrix_functions_f16.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.10.0
 * @date     08 July 2021
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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

 
#ifndef _MATRIX_FUNCTIONS_F16_H_
#define _MATRIX_FUNCTIONS_F16_H_

#ifdef   __cplusplus
extern "C"
{
#endif


#include "arm_math_types_f16.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"
    
#if defined(ARM_FLOAT16_SUPPORTED)

  #define DEFAULT_HOUSEHOLDER_THRESHOLD_F16 (1.0e-3f)

 /**
   * @brief Instance structure for the floating-point matrix structure.
   */
  typedef struct
  {
    uint16_t numRows;     /**< number of rows of the matrix.     */
    uint16_t numCols;     /**< number of columns of the matrix.  */
    float16_t *pData;     /**< points to the data of the matrix. */
  } arm_matrix_instance_f16;

 /**
   * @brief Floating-point matrix addition.
   * @param[in]  pSrcA  points to the first input matrix structure
   * @param[in]  pSrcB  points to the second input matrix structure
   * @param[out] pDst   points to output matrix structure
   * @return     The function returns either
   * <code>ARM_MATH_SIZE_MISMATCH</code> or <code>ARM_MATH_SUCCESS</code> based on the outcome of size checking.
   */
arm_status arm_mat_add_f16(
  const arm_matrix_instance_f16 * pSrcA,
  const arm_matrix_instance_f16 * pSrcB,
        arm_matrix_instance_f16 * pDst);

  /**
   * @brief Floating-point, complex, matrix multiplication.
   * @param[in]  pSrcA  points to the first input matrix structure
   * @param[in]  pSrcB  points to the second input matrix structure
   * @param[out] pDst   points to output matrix structure
   * @return     The function returns either
   * <code>ARM_MATH_SIZE_MISMATCH</code> or <code>ARM_MATH_SUCCESS</code> based on the outcome of size checking.
   */
arm_status arm_mat_cmplx_mult_f16(
  const arm_matrix_instance_f16 * pSrcA,
  const arm_matrix_instance_f16 * pSrcB,
        arm_matrix_instance_f16 * pDst);

  /**
   * @brief Floating-point matrix transpose.
   * @param[in]  pSrc  points to the input matrix
   * @param[out] pDst  points to the output matrix
   * @return    The function returns either  <code>ARM_MATH_SIZE_MISMATCH</code>
   * or <code>ARM_MATH_SUCCESS</code> based on the outcome of size checking.
   */
arm_status arm_mat_trans_f16(
  const arm_matrix_instance_f16 * pSrc,
        arm_matrix_instance_f16 * pDst);

  /**
   * @brief Floating-point complex matrix transpose.
   * @param[in]  pSrc  points to the input matrix
   * @param[out] pDst  points to the output matrix
   * @return    The function returns either  <code>ARM_MATH_SIZE_MISMATCH</code>
   * or <code>ARM_MATH_SUCCESS</code> based on the outcome of size checking.
   */
arm_status arm_mat_cmplx_trans_f16(
  const arm_matrix_instance_f16 * pSrc,
  arm_matrix_instance_f16 * pDst);

  /**
   * @brief Floating-point matrix multiplication
   * @param[in]  pSrcA  points to the first input matrix structure
   * @param[in]  pSrcB  points to the second input matrix structure
   * @param[out] pDst   points to output matrix structure
   * @return     The function returns either
   * <code>ARM_MATH_SIZE_MISMATCH</code> or <code>ARM_MATH_SUCCESS</code> based on the outcome of size checking.
   */
arm_status arm_mat_mult_f16(
  const arm_matrix_instance_f16 * pSrcA,
  const arm_matrix_instance_f16 * pSrcB,
        arm_matrix_instance_f16 * pDst);
  /**
   * @brief Floating-point matrix and vector multiplication
   * @param[in]  pSrcMat  points to the input matrix structure
   * @param[in]  pVec     points to vector
   * @param[out] pDst     points to output vector
   */
void arm_mat_vec_mult_f16(
  const arm_matrix_instance_f16 *pSrcMat, 
  const float16_t *pVec, 
  float16_t *pDst);

  /**
   * @brief Floating-point matrix subtraction
   * @param[in]  pSrcA  points to the first input matrix structure
   * @param[in]  pSrcB  points to the second input matrix structure
   * @param[out] pDst   points to output matrix structure
   * @return     The function returns either
   * <code>ARM_MATH_SIZE_MISMATCH</code> or <code>ARM_MATH_SUCCESS</code> based on the outcome of size checking.
   */
arm_status arm_mat_sub_f16(
  const arm_matrix_instance_f16 * pSrcA,
  const arm_matrix_instance_f16 * pSrcB,
        arm_matrix_instance_f16 * pDst);

  /**
   * @brief Floating-point matrix scaling.
   * @param[in]  pSrc   points to the input matrix
   * @param[in]  scale  scale factor
   * @param[out] pDst   points to the output matrix
   * @return     The function returns either
   * <code>ARM_MATH_SIZE_MISMATCH</code> or <code>ARM_MATH_SUCCESS</code> based on the outcome of size checking.
   */
arm_status arm_mat_scale_f16(
  const arm_matrix_instance_f16 * pSrc,
        float16_t scale,
        arm_matrix_instance_f16 * pDst);

  /**
   * @brief  Floating-point matrix initialization.
   * @param[in,out] S         points to an instance of the floating-point matrix structure.
   * @param[in]     nRows     number of rows in the matrix.
   * @param[in]     nColumns  number of columns in the matrix.
   * @param[in]     pData     points to the matrix data array.
   */
void arm_mat_init_f16(
        arm_matrix_instance_f16 * S,
        uint16_t nRows,
        uint16_t nColumns,
        float16_t * pData);


  /**
   * @brief Floating-point matrix inverse.
   * @param[in]  src   points to the instance of the input floating-point matrix structure.
   * @param[out] dst   points to the instance of the output floating-point matrix structure.
   * @return The function returns ARM_MATH_SIZE_MISMATCH, if the dimensions do not match.
   * If the input matrix is singular (does not have an inverse), then the algorithm terminates and returns error status ARM_MATH_SINGULAR.
   */
  arm_status arm_mat_inverse_f16(
  const arm_matrix_instance_f16 * src,
  arm_matrix_instance_f16 * dst);


 /**
   * @brief Floating-point Cholesky decomposition of Symmetric Positive Definite Matrix.
   * @param[in]  src   points to the instance of the input floating-point matrix structure.
   * @param[out] dst   points to the instance of the output floating-point matrix structure.
   * @return The function returns ARM_MATH_SIZE_MISMATCH, if the dimensions do not match.
   * If the input matrix does not have a decomposition, then the algorithm terminates and returns error status ARM_MATH_DECOMPOSITION_FAILURE.
   * If the matrix is ill conditioned or only semi-definite, then it is better using the LDL^t decomposition.
   * The decomposition is returning a lower triangular matrix.
   */
  arm_status arm_mat_cholesky_f16(
  const arm_matrix_instance_f16 * src,
  arm_matrix_instance_f16 * dst);

 /**
   * @brief Solve UT . X = A where UT is an upper triangular matrix
   * @param[in]  ut  The upper triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of UT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
  */
  arm_status arm_mat_solve_upper_triangular_f16(
  const arm_matrix_instance_f16 * ut,
  const arm_matrix_instance_f16 * a,
  arm_matrix_instance_f16 * dst);

 /**
   * @brief Solve LT . X = A where LT is a lower triangular matrix
   * @param[in]  lt  The lower triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of LT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
   */
  arm_status arm_mat_solve_lower_triangular_f16(
  const arm_matrix_instance_f16 * lt,
  const arm_matrix_instance_f16 * a,
  arm_matrix_instance_f16 * dst);


/**
  @brief         QR decomposition of a m x n floating point matrix with m >= n.
  @param[in]     pSrc      points to input matrix structure. The source matrix is modified by the function.
  @param[in]     threshold norm2 threshold.  
  @param[out]    pOutR     points to output R matrix structure of dimension m x n
  @param[out]    pOutQ     points to output Q matrix structure of dimension m x m
  @param[out]    pOutTau   points to Householder scaling factors of dimension n
  @param[inout]  pTmpA     points to a temporary vector of dimension m.
  @param[inout]  pTmpB     points to a temporary vector of dimension n.
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
                   - \ref ARM_MATH_SINGULAR      : Input matrix is found to be singular (non-invertible)
 */

arm_status arm_mat_qr_f16(
    const arm_matrix_instance_f16 * pSrc,
    const float16_t threshold,
    arm_matrix_instance_f16 * pOutR,
    arm_matrix_instance_f16 * pOutQ,
    float16_t * pOutTau,
    float16_t *pTmpA,
    float16_t *pTmpB
    );

/**
  @brief         Householder transform of a half floating point vector.
  @param[in]     pSrc        points to the input vector.
  @param[in]     threshold   norm2 threshold.  
  @param[in]     blockSize   dimension of the vector space.
  @param[outQ]   pOut        points to the output vector.
  @return        beta        return the scaling factor beta
 */

float16_t arm_householder_f16(
    const float16_t * pSrc,
    const float16_t threshold,
    uint32_t    blockSize,
    float16_t * pOut
    );

#endif /*defined(ARM_FLOAT16_SUPPORTED)*/
#ifdef   __cplusplus
}
#endif

#endif /* ifndef _MATRIX_FUNCTIONS_F16_H_ */
