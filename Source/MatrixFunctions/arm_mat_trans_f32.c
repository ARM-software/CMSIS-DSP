/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_trans_f32.c
 * Description:  Floating-point matrix transpose
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
  @defgroup MatrixTrans Matrix Transpose

  Transposes a matrix.

  Transposing an <code>M x N</code> matrix flips it around the center diagonal and results in an <code>N x M</code> matrix.

  @par Transpose of a 3 x 3 matrix
  
  \f[
  \begin{pmatrix}
   a_{1,1} & a_{1,2} & a_{1,3} \\
   a_{2,1} & a_{2,2} & a_{2,3} \\
   a_{3,1} & a_{3,2} & a_{3,3} \\
  \end{pmatrix}^T
   =
  \begin{pmatrix}
   a_{1,1} & a_{2,1} & a_{3,1} \\
   a_{1,2} & a_{2,2} & a_{3,2} \\
   a_{1,3} & a_{2,3} & a_{3,3} \\
  \end{pmatrix}
  \f]
  
 */

/**
  @addtogroup MatrixTrans
  @{
 */

/**
  @brief         Floating-point matrix transpose.
  @param[in]     pSrc      points to input matrix
  @param[out]    pDst      points to output matrix
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
 */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

ARM_DSP_ATTRIBUTE arm_status arm_mat_trans_f32(
  const arm_matrix_instance_f32 * pSrc,
  arm_matrix_instance_f32 * pDst)
{
  arm_status status;                             /* status of matrix transpose  */

#ifdef ARM_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
  {
    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef ARM_MATH_MATRIX_CHECK    */
  {
    if (pDst->numRows == pDst->numCols)
    {
        if (pDst->numCols == 2)
            return arm_mat_trans_32bit_2x2_mve((uint32_t *)pSrc->pData, (uint32_t *)pDst->pData);
        if (pDst->numCols == 3)
            return arm_mat_trans_32bit_3x3_mve((uint32_t *)pSrc->pData, (uint32_t *)pDst->pData);
        if (pDst->numCols == 4)
            return arm_mat_trans_32bit_4x4_mve((uint32_t *)pSrc->pData, (uint32_t *)pDst->pData);
    }

    arm_mat_trans_32bit_generic_mve(pSrc->numRows, pSrc->numCols, (uint32_t *)pSrc->pData, (uint32_t *)pDst->pData);
    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

#else
#if defined(ARM_MATH_NEON)

#define BLOCK_ROWS 4
#define BLOCK_ROWS_SHIFT 2
#define LANE 4
#define LANE_SHIFT 2

#include "_arm_mat_trans_neon.c"

ARM_DSP_ATTRIBUTE arm_status arm_mat_trans_f32(
  const arm_matrix_instance_f32 * pSrc,
  arm_matrix_instance_f32 * pDst)
{
  ARM_MAT_TRANS_NEON_INIT_U32

#ifdef ARM_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
  {
    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef ARM_MATH_MATRIX_CHECK    */

  {
    ARM_MAT_TRANS_NEON_U32

    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

#include "_arm_mat_trans_undef_neon.c"

#else
ARM_DSP_ATTRIBUTE arm_status arm_mat_trans_f32(
  const arm_matrix_instance_f32 * pSrc,
        arm_matrix_instance_f32 * pDst)
{
  float32_t *pIn = pSrc->pData;                  /* input data matrix pointer */
  float32_t *pOut = pDst->pData;                 /* output data matrix pointer */
  float32_t *px;                                 /* Temporary output data matrix pointer */
  uint16_t nRows = pSrc->numRows;                /* number of rows */
  uint16_t nCols = pSrc->numCols;                /* number of columns */
  uint32_t col, row = nRows, i = 0U;             /* Loop counters */
  arm_status status;                             /* status of matrix transpose */

#ifdef ARM_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrc->numRows != pDst->numCols) ||
      (pSrc->numCols != pDst->numRows)   )
  {
    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef ARM_MATH_MATRIX_CHECK */

  {
    /* Matrix transpose by exchanging the rows with columns */
    /* row loop */
    do
    {
      /* Pointer px is set to starting address of column being processed */
      px = pOut + i;

#if defined (ARM_MATH_LOOPUNROLL)

      /* Loop unrolling: Compute 4 outputs at a time */
      col = nCols >> 2U;

      while (col > 0U)        /* column loop */
      {
        /* Read and store input element in destination */
        *px = *pIn++;
        /* Update pointer px to point to next row of transposed matrix */
        px += nRows;

        *px = *pIn++;
        px += nRows;

        *px = *pIn++;
        px += nRows;

        *px = *pIn++;
        px += nRows;

        /* Decrement column loop counter */
        col--;
      }

      /* Loop unrolling: Compute remaining outputs */
      col = nCols % 0x4U;

#else

      /* Initialize col with number of samples */
      col = nCols;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

      while (col > 0U)
      {
        /* Read and store input element in destination */
        *px = *pIn++;

        /* Update pointer px to point to next row of transposed matrix */
        px += nRows;

        /* Decrement column loop counter */
        col--;
      }

      i++;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);          /* row loop end */

    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}
#endif /* #if defined(ARM_MATH_NEON) */
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
 * @} end of MatrixTrans group
 */
