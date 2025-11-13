/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_qr_f64.c
 * Description:  Double floating-point matrix QR decomposition.
 *
 * $Date:        15 June 2022
 * $Revision:    V1.11.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */

/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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
#include "dsp/matrix_utils.h"



/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixQR
  @{
 */

/**
  @brief         QR decomposition of a m x n double floating point matrix with m >= n.
  @param[in]     pSrc      points to input matrix structure. The source matrix is modified by the function.
  @param[in]     threshold norm2 threshold.    
  @param[out]    pOutR     points to output R matrix structure of dimension m x n
  @param[out]    pOutQ     points to output Q matrix structure of dimension m x m (can be NULL)
  @param[out]    pOutTau   points to Householder scaling factors of dimension n
  @param[inout]  pTmpA     points to a temporary vector of dimension m.
  @param[inout]  pTmpB     points to a temporary vector of dimension m.
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
  
  @par           pOutQ is optional:
                 pOutQ can be a NULL pointer.
                 In this case, the argument will be ignored
                 and the output Q matrix won't be computed.


  @par           Norm2 threshold 
                 For the meaning of this argument please 
                 refer to the \ref MatrixHouseholder documentation

 */




ARM_DSP_ATTRIBUTE arm_status arm_mat_qr_f64(
    const arm_matrix_instance_f64 * pSrc,
    const float64_t threshold,
    arm_matrix_instance_f64 * pOutR,
    arm_matrix_instance_f64 * pOutQ,
    float64_t * pOutTau,
    float64_t *pTmpA,
    float64_t *pTmpB
    )

{
  int32_t col=0;
  int32_t nb,pos;
  float64_t *pa,*pc;
  float64_t beta;
  float64_t *pv;
  float64_t *pdst;
  float64_t *p;

  if (pSrc->numRows < pSrc->numCols)
  {
    return(ARM_MATH_SIZE_MISMATCH);
  }

  memcpy(pOutR->pData,pSrc->pData,pSrc->numCols * pSrc->numRows*sizeof(float64_t));
  pOutR->numCols = pSrc->numCols;
  pOutR->numRows = pSrc->numRows;
  
  p = pOutR->pData;
  
  pc = pOutTau;
  for(col=0 ; col < pSrc->numCols; col++)
  {
      int32_t i,j,k,blkCnt;
      float64_t *pa0,*pa1,*pa2,*pa3;
      COPY_COL_F64(pOutR,col,col,pTmpA);

      beta = arm_householder_f64(pTmpA,threshold,pSrc->numRows - col,pTmpA);
      *pc++ = beta;
    
      pdst = pTmpB;

      /* v.T A(col:,col:) -> tmpb */
      pv = pTmpA;
      pa = p;
      for(j=0;j<pSrc->numCols-col; j++)
      {
              *pdst++ = *pv * *pa++; 
      }
      pa += col;
      pv++;
      pdst = pTmpB;

      pa0 = pa;
      pa1 = pa0 + pSrc->numCols;
      pa2 = pa1 + pSrc->numCols;
      pa3 = pa2 + pSrc->numCols;

      /* Unrolled loop */
      blkCnt = (pSrc->numRows-col - 1) >> 2;
      k=1;
      while(blkCnt > 0)
      {
          float64_t sum;

          for(j=0;j<pSrc->numCols-col; j++)
          {
              sum = *pdst;

              sum += pv[0] * *pa0++;
              sum += pv[1] * *pa1++;
              sum += pv[2] * *pa2++;
              sum += pv[3] * *pa3++;
              
              *pdst++ = sum; 
          }
          pa0 += col + 3*pSrc->numCols;
          pa1 += col + 3*pSrc->numCols;
          pa2 += col + 3*pSrc->numCols;
          pa3 += col + 3*pSrc->numCols;
          pv  += 4;
          pdst = pTmpB;
          k += 4;
          blkCnt--;
      }

      pa = pa0;
      for(;k<pSrc->numRows-col; k++)
      {
          for(j=0;j<pSrc->numCols-col; j++)
          {
              *pdst++ += *pv * *pa++; 
          }
          pa += col;
          pv++;
          pdst = pTmpB;
      }

      /* A(col:,col:) - beta v tmpb */
      pa = p;
      for(j=0;j<pSrc->numRows-col; j++)
      {
        float64_t f = beta * pTmpA[j];

        for(i=0;i<pSrc->numCols-col; i++)
        {
          *pa = *pa - f * pTmpB[i] ;
          pa++;
        }
        pa += col;
      } 

      /* Copy Householder reflectors into R matrix */
      pa = p + pOutR->numCols;
      for(k=0;k<pSrc->numRows-col-1; k++)
      {
         *pa = pTmpA[k+1];
         pa += pOutR->numCols;
      }

      p += 1 + pOutR->numCols;
  }

  /* Generate Q if requested by user matrix */

  if (pOutQ != NULL)
  {
     /* Initialize Q matrix to identity */
     memset(pOutQ->pData,0,sizeof(float64_t)*pOutQ->numRows*pOutQ->numRows);
     
     pa = pOutQ->pData;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
        *pa = 1.0;
        pa += pOutQ->numCols+1;
     }
   
     nb = pOutQ->numRows - pOutQ->numCols + 1;
   
     pc = pOutTau + pOutQ->numCols - 1;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
       int32_t i,j,k, blkCnt;
       float64_t *pa0,*pa1,*pa2,*pa3;
       pos = pSrc->numRows - nb;
       p = pOutQ->pData + pos + pOutQ->numCols*pos ;
   
       
       COPY_COL_F64(pOutR,pos,pos,pTmpA);
       pTmpA[0] = 1.0;
       pdst = pTmpB;
      
       /* v.T A(col:,col:) -> tmpb */
       
       pv = pTmpA;
       pa = p;
       for(j=0;j<pOutQ->numRows-pos; j++)
       {
               *pdst++ = *pv * *pa++; 
       }
       pa += pos;
       pv++;
       pdst = pTmpB;
       pa0 = pa;
       pa1 = pa0 + pOutQ->numRows;
       pa2 = pa1 + pOutQ->numRows;
       pa3 = pa2 + pOutQ->numRows;

       /* Unrolled loop */
       blkCnt = (pOutQ->numRows-pos - 1) >> 2;
       k=1;
       while(blkCnt > 0)
       {
           float64_t sum;

           for(j=0;j<pOutQ->numRows-pos; j++)
           {
              sum = *pdst;

              sum += pv[0] * *pa0++;
              sum += pv[1] * *pa1++;
              sum += pv[2] * *pa2++;
              sum += pv[3] * *pa3++;
              
              *pdst++ = sum; 
           }
           pa0 += pos + 3*pOutQ->numRows;
           pa1 += pos + 3*pOutQ->numRows;
           pa2 += pos + 3*pOutQ->numRows;
           pa3 += pos + 3*pOutQ->numRows;
           pv  += 4;
           pdst = pTmpB;
           k += 4;
           blkCnt--;
       }

       pa = pa0;
       for(;k<pOutQ->numRows-pos; k++)
       {
           for(j=0;j<pOutQ->numRows-pos; j++)
           {
               *pdst++ += *pv * *pa++; 
           }
           pa += pos;
           pv++;
           pdst = pTmpB;
       }
   
       pa = p;
       beta = *pc--;
       for(j=0;j<pOutQ->numRows-pos; j++)
       {
           float64_t f = beta * pTmpA[j];

           for(i=0;i<pOutQ->numCols-pos; i++)
           {
             *pa = *pa - f * pTmpB[i] ;
             pa++;
           }
           pa += pos;
       } 
   
   
       nb++;
     }
  }

  arm_status status = ARM_MATH_SUCCESS;
  /* Return to application */
  return (status);
}


/**
  @} end of MatrixQR group
 */
