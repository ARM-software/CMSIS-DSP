/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_trans_f64.c
 * Description:  Floating-point matrix transpose
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

/**
  @ingroup groupMatrix
 */

/**
  @defgroup MatrixTrans Matrix Transpose

  Tranposes a matrix.

  Transposing an <code>M x N</code> matrix flips it around the center diagonal and results in an <code>N x M</code> matrix.
  \image html MatrixTranspose.gif "Transpose of a 3 x 3 matrix"
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
#if defined(ARM_MATH_NEON) && defined(__aarch64__)

ARM_DSP_ATTRIBUTE arm_status arm_mat_trans_f64(
    const arm_matrix_instance_f64 * pSrc,
    arm_matrix_instance_f64 * pDst)
{
    float64_t *pIn = pSrc->pData;                  /* input data matrix pointer */
    float64_t *pOut = pDst->pData;                 /* output data matrix pointer */
    float64_t *px;                                 /* Temporary output data matrix pointer */
    uint16_t nRows = pSrc->numRows;                /* number of rows */
    uint16_t nColumns = pSrc->numCols;             /* number of columns */
    
    uint16_t blkCnt, rowCnt, i = 0U, row = nRows;          /* loop counters */
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
        /* Matrix transpose by exchanging the rows with columns */
        /* Row loop */
        rowCnt = row >> 1;
        while (rowCnt > 0U)
        {
            float64_t *row0,*row1;
            float64x2x4_t raV;
            
            blkCnt = nColumns >> 2;
            
            /* The pointer px is set to starting address of the column being processed */
            px = pOut + i;
            
            /* Compute 4 outputs at a time.
             ** a second loop below computes the remaining 1 to 3 samples. */
            while (blkCnt > 0U)        /* Column loop */
            {
                row0 = pIn;
                row1 = pIn+nColumns;
                pIn+=4;
                raV = vld4q_lane_f64(row0, raV, 0);
                raV = vld4q_lane_f64(row1, raV, 1);
                
                vst1q_f64(px,raV.val[0]);
                px += nRows;
                
                vst1q_f64(px,raV.val[1]);
                px += nRows;
                
                vst1q_f64(px,raV.val[2]);
                px += nRows;
                
                vst1q_f64(px,raV.val[3]);
                px += nRows;
                
                /* Decrement the column loop counter */
                blkCnt--;
            }
            
            /* Perform matrix transpose for last 3 samples here. */
            blkCnt = nColumns % 0x4U;
            
            while (blkCnt > 0U)
            {
                /* Read and store the input element in the destination */
                *px++ = *pIn;
                *px++ = *(pIn + 1 * nColumns);
                
                px += (nRows - 2);
                pIn++;
                
                /* Decrement the column loop counter */
                blkCnt--;
            }
            
            i += 2;
            pIn += 1 * nColumns;
            
            /* Decrement the row loop counter */
            rowCnt--;
            
        }         /* Row loop end  */
        
        rowCnt = row & 1;
        while (rowCnt > 0U)
        {
            blkCnt = nColumns ;
            /* The pointer px is set to starting address of the column being processed */
            px = pOut + i;
            
            while (blkCnt > 0U)
            {
                /* Read and store the input element in the destination */
                *px = *pIn++;
                
                /* Update the pointer px to point to the next row of the transposed matrix */
                px += nRows;
                
                /* Decrement the column loop counter */
                blkCnt--;
            }
            i++;
            rowCnt -- ;
        }
        
        /* Set status as ARM_MATH_SUCCESS */
        status = ARM_MATH_SUCCESS;
    }
    
    /* Return to application */
    return (status);
}
#else
ARM_DSP_ATTRIBUTE arm_status arm_mat_trans_f64(
    const arm_matrix_instance_f64 * pSrc,
    arm_matrix_instance_f64 * pDst)
{
    float64_t *pIn = pSrc->pData;                  /* input data matrix pointer */
    float64_t *pOut = pDst->pData;                 /* output data matrix pointer */
    float64_t *px;                                 /* Temporary output data matrix pointer */
    uint16_t nRows = pSrc->numRows;                /* number of rows */
    uint16_t nCols = pSrc->numCols;                /* number of columns */
    uint64_t col, row = nRows, i = 0U;             /* Loop counters */
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
#endif
/**
 * @} end of MatrixTrans group
 */
