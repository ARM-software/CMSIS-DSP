/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_qr_f32.c
 * Description:  Floating-point matrix QR decomposition.
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

#include "dsp/matrix_functions.h"
#include "dsp/matrix_utils.h"


#if !defined(ARM_MATH_AUTOVECTORIZE)
#if defined(ARM_MATH_MVEF)
#include "arm_helium_utils.h"
#endif
#endif

/**
  @ingroup groupMatrix
 */

/**
  @defgroup MatrixQR QR decomposition of a Matrix

    Computes the QR decomposition of a matrix M using Householder algorithm.

    \f[
        M = Q R
    \f]

    where Q is an orthogonal matrix and R is upper triangular.
    No pivoting strategy is used.

    The returned value for R is using a format a bit similar
    to LAPACK : it is not just containing the matrix R but
    also the Householder reflectors. 

    The function is also returning a vector \f$\tau\f$
    that is containing the scaling factor for the reflectors.

    Returned value R has the structure:

    \f[
    \begin{pmatrix}
    r_{11} & r_{12}     & \dots  & r_{1n} \\
    v_{12} & r_{22}     & \dots  & r_{2n} \\
    v_{13} & v_{22}     & \dots  & r_{3n} \\
    \vdots & \vdots     & \ddots & \vdots   \\
    v_{1m} & v_{2(m-1)} & \dots  & r_{mn} \\
    \end{pmatrix}
    \f]

    where 

    \f[
    v_1 = 
    \begin{pmatrix}
    1       \\
    v_{12}  \\
    \vdots  \\
    v_{1m}  \\
    \end{pmatrix}
    \f]

    is the first householder reflector.
    
    The Householder Matrix is given by \f$H_1\f$

    \f[
    H_1 = I - \tau_1 v_1 v_1^T
    \f]

    The Matrix Q is the product of the Householder matrices:
    
    \f[
    Q = H_1 H_2 \dots H_n
    \f]

    The computation of the matrix Q by this function is
    optional.

    And the matrix R, would be the returned value R without the
    householder reflectors:

    \f[
    \begin{pmatrix}
    r_{11} & r_{12} & \dots  & r_{1n} \\
    0      & r_{22} & \dots  & r_{2n} \\
    0      & 0      & \dots  & r_{3n} \\
    \vdots & \vdots & \ddots & \vdots   \\
    0      & 0      & \dots  & r_{mn} \\
    \end{pmatrix}
    \f]


 */

/**
  @addtogroup MatrixQR
  @{
 */

/**
  @brief         QR decomposition of a m x n floating point matrix with m >= n.
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

#if !defined(ARM_MATH_AUTOVECTORIZE)
#if defined(ARM_MATH_MVEF)

arm_status arm_mat_qr_f32(
    const arm_matrix_instance_f32 * pSrc,
    const float32_t threshold,
    arm_matrix_instance_f32 * pOutR,
    arm_matrix_instance_f32 * pOutQ,
    float32_t * pOutTau,
    float32_t *pTmpA,
    float32_t *pTmpB
    )

{
  int32_t col=0;
  int32_t nb,pos;
  float32_t *pa,*pc;
  float32_t beta;
  float32_t *pv;
  float32_t *pdst;
  float32_t *p;

  if (pSrc->numRows < pSrc->numCols)
  {
    return(ARM_MATH_SIZE_MISMATCH);
  }

  memcpy(pOutR->pData,pSrc->pData,pSrc->numCols * pSrc->numRows*sizeof(float32_t));
  pOutR->numCols = pSrc->numCols;
  pOutR->numRows = pSrc->numRows;
  
  p = pOutR->pData;
  
  pc = pOutTau;
  for(col=0 ; col < pSrc->numCols; col++)
  {
      int32_t j,k,blkCnt,blkCnt2;
      float32_t *pa0,*pa1,*pa2,*pa3,*ptemp;
      float32_t temp;
      float32x4_t v1,v2,vtemp;

      COPY_COL_F32(pOutR,col,col,pTmpA);

      beta = arm_householder_f32(pTmpA,threshold,pSrc->numRows - col,pTmpA);
      *pc++ = beta;
    
      pdst = pTmpB;

      /* v.T A(col:,col:) -> tmpb */
      pv = pTmpA;
      pa = p;

      temp = *pv;
      blkCnt = (pSrc->numCols-col) >> 2;
      while (blkCnt > 0)
      {
          v1 = vld1q_f32(pa);
          v2 = vmulq_n_f32(v1,temp);
          vst1q_f32(pdst,v2);

          pa += 4;
          pdst += 4;
          blkCnt--;
      }
      blkCnt = (pSrc->numCols-col) & 3;
      if (blkCnt > 0)
      {
          mve_pred16_t p0 = vctp32q(blkCnt);
          v1 = vld1q_f32(pa);
          v2 = vmulq_n_f32(v1,temp);
          vst1q_p_f32(pdst,v2,p0);

          pa += blkCnt;
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
          vtemp=vld1q_f32(pv);

          blkCnt2 = (pSrc->numCols-col) >> 2;
          while (blkCnt2 > 0)
          {
              v1 = vld1q_f32(pdst);

              v2 = vld1q_f32(pa0);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,0));

              v2 = vld1q_f32(pa1);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,1));

              v2 = vld1q_f32(pa2);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,2));

              v2 = vld1q_f32(pa3);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,3));

              vst1q_f32(pdst,v1);

              pdst += 4;
              pa0 += 4;
              pa1 += 4;
              pa2 += 4;
              pa3 += 4;
              blkCnt2--;
          }
          blkCnt2 = (pSrc->numCols-col) & 3;
          if (blkCnt2 > 0)
          {
              mve_pred16_t p0 = vctp32q(blkCnt2);

              v1 = vld1q_f32(pdst);

              v2 = vld1q_f32(pa0);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,0));

              v2 = vld1q_f32(pa1);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,1));

              v2 = vld1q_f32(pa2);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,2));

              v2 = vld1q_f32(pa3);
              v1 = vfmaq_n_f32(v1,v2,vgetq_lane(vtemp,3));

              vst1q_p_f32(pdst,v1,p0);

              pa0 += blkCnt2;
              pa1 += blkCnt2;
              pa2 += blkCnt2;
              pa3 += blkCnt2;
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
          temp = *pv;
          blkCnt2 = (pSrc->numCols-col) >> 2;
          while (blkCnt2 > 0)
          {
              v1 = vld1q_f32(pa);
              v2 = vld1q_f32(pdst);
              v2 = vfmaq_n_f32(v2,v1,temp);
              vst1q_f32(pdst,v2);

              pa += 4;
              pdst += 4;
              blkCnt2--;
          }
          blkCnt2 = (pSrc->numCols-col) & 3;
          if (blkCnt2 > 0)
          {
              mve_pred16_t p0 = vctp32q(blkCnt2);
              v1 = vld1q_f32(pa);
              v2 = vld1q_f32(pdst);
              v2 = vfmaq_n_f32(v2,v1,temp);
              vst1q_p_f32(pdst,v2,p0);

              pa += blkCnt2;
          }
          
          pa += col;
          pv++;
          pdst = pTmpB;
      }

      /* A(col:,col:) - beta v tmpb */
      pa = p;
      for(j=0;j<pSrc->numRows-col; j++)
      {
        float32_t f = -beta * pTmpA[j];
        ptemp = pTmpB; 

        blkCnt2 = (pSrc->numCols-col) >> 2;
        while (blkCnt2 > 0)
        {
            v1 = vld1q_f32(pa);
            v2 = vld1q_f32(ptemp);
            v1 = vfmaq_n_f32(v1,v2,f);
            vst1q_f32(pa,v1);

            pa += 4;
            ptemp += 4;

            blkCnt2--;
        }
        blkCnt2 = (pSrc->numCols-col) & 3;
        if (blkCnt2 > 0)
        {
            mve_pred16_t p0 = vctp32q(blkCnt2);

            v1 = vld1q_f32(pa);
            v2 = vld1q_f32(ptemp);
            v1 = vfmaq_n_f32(v1,v2,f);
            vst1q_p_f32(pa,v1,p0);

            pa += blkCnt2;
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
     memset(pOutQ->pData,0,sizeof(float32_t)*pOutQ->numRows*pOutQ->numRows);
     
     pa = pOutQ->pData;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
        *pa = 1.0f;
        pa += pOutQ->numCols+1;
     }
   
     nb = pOutQ->numRows - pOutQ->numCols + 1;
   
     pc = pOutTau + pOutQ->numCols - 1;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
       int32_t j,k, blkCnt, blkCnt2;
       float32_t *pa0,*pa1,*pa2,*pa3,*ptemp;
       float32_t temp;
       float32x4_t v1,v2,vtemp;

       pos = pSrc->numRows - nb;
       p = pOutQ->pData + pos + pOutQ->numCols*pos ;
   
       
       COPY_COL_F32(pOutR,pos,pos,pTmpA);
       pTmpA[0] = 1.0f;
       pdst = pTmpB;
      
       /* v.T A(col:,col:) -> tmpb */
       
       pv = pTmpA;
       pa = p;

       temp = *pv;
       blkCnt2 = (pOutQ->numRows-pos) >> 2;
       while (blkCnt2 > 0)
       {
           v1 = vld1q_f32(pa);
           v1 = vmulq_n_f32(v1, temp);
           vst1q_f32(pdst,v1);

           pa += 4;
           pdst += 4;

           blkCnt2--;
       }
       blkCnt2 = (pOutQ->numRows-pos) & 3;
       if (blkCnt2 > 0)
       {
           mve_pred16_t p0 = vctp32q(blkCnt2);

           v1 = vld1q_f32(pa);
           v1 = vmulq_n_f32(v1, temp);
           vst1q_p_f32(pdst,v1,p0);

           pa += blkCnt2;
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

           vtemp = vld1q_f32(pv);
           blkCnt2 = (pOutQ->numRows-pos) >> 2;
           while (blkCnt2 > 0)
           {
               v1 = vld1q_f32(pdst);

               v2 = vld1q_f32(pa0);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,0));

               v2 = vld1q_f32(pa1);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,1));

               v2 = vld1q_f32(pa2);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,2));

               v2 = vld1q_f32(pa3);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,3));

               vst1q_f32(pdst,v1);

               pa0 += 4;
               pa1 += 4;
               pa2 += 4;
               pa3 += 4;
               pdst += 4;

               blkCnt2--;
           }
           blkCnt2 = (pOutQ->numRows-pos) & 3;
           if (blkCnt2 > 0)
           {
               mve_pred16_t p0 = vctp32q(blkCnt2);

               v1 = vld1q_f32(pdst);

               v2 = vld1q_f32(pa0);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,0));

               v2 = vld1q_f32(pa1);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,1));

               v2 = vld1q_f32(pa2);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,2));

               v2 = vld1q_f32(pa3);
               v1 = vfmaq_n_f32(v1, v2, vgetq_lane(vtemp,3));

               vst1q_p_f32(pdst,v1,p0);

               pa0 += blkCnt2;
               pa1 += blkCnt2;
               pa2 += blkCnt2;
               pa3 += blkCnt2;

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
           temp = *pv;
           blkCnt2 = (pOutQ->numRows-pos) >> 2;
           while (blkCnt2 > 0)
           {
               v1 = vld1q_f32(pdst);
               v2 = vld1q_f32(pa);
               v1 = vfmaq_n_f32(v1, v2, temp);
               vst1q_f32(pdst,v1);

               pdst += 4;
               pa += 4;

               blkCnt2--;
           }
           blkCnt2 = (pOutQ->numRows-pos) & 3;
           if (blkCnt2 > 0)
           {
               mve_pred16_t p0 = vctp32q(blkCnt2);
               v1 = vld1q_f32(pdst);
               v2 = vld1q_f32(pa);
               v1 = vfmaq_n_f32(v1, v2, temp);
               vst1q_p_f32(pdst,v1,p0);

               pa += blkCnt2;
           }
               
           pa += pos;
           pv++;
           pdst = pTmpB;
       }
   
       pa = p;
       beta = *pc--;
       for(j=0;j<pOutQ->numRows-pos; j++)
       {
           float32_t f = -beta * pTmpA[j];
           ptemp = pTmpB;

           blkCnt2 = (pOutQ->numCols-pos) >> 2;
           while (blkCnt2 > 0)
           {
               v1 = vld1q_f32(pa);
               v2 = vld1q_f32(ptemp);
               v1 = vfmaq_n_f32(v1,v2,f);
               vst1q_f32(pa,v1);

               pa += 4;
               ptemp += 4;

               blkCnt2--;
           }
           blkCnt2 = (pOutQ->numCols-pos) & 3;
           if (blkCnt2 > 0)
           {
               mve_pred16_t p0 = vctp32q(blkCnt2);

               v1 = vld1q_f32(pa);
               v2 = vld1q_f32(ptemp);
               v1 = vfmaq_n_f32(v1,v2,f);
               vst1q_p_f32(pa,v1,p0);

               pa += blkCnt2;
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

#endif /*#if !defined(ARM_MATH_MVEF)*/


#endif /*#if !defined(ARM_MATH_AUTOVECTORIZE)*/



#if (!defined(ARM_MATH_MVEF)) || defined(ARM_MATH_AUTOVECTORIZE)

arm_status arm_mat_qr_f32(
    const arm_matrix_instance_f32 * pSrc,
    const float32_t threshold,
    arm_matrix_instance_f32 * pOutR,
    arm_matrix_instance_f32 * pOutQ,
    float32_t * pOutTau,
    float32_t *pTmpA,
    float32_t *pTmpB
    )

{
  int32_t col=0;
  int32_t nb,pos;
  float32_t *pa,*pc;
  float32_t beta;
  float32_t *pv;
  float32_t *pdst;
  float32_t *p;

  if (pSrc->numRows < pSrc->numCols)
  {
    return(ARM_MATH_SIZE_MISMATCH);
  }

  memcpy(pOutR->pData,pSrc->pData,pSrc->numCols * pSrc->numRows*sizeof(float32_t));
  pOutR->numCols = pSrc->numCols;
  pOutR->numRows = pSrc->numRows;
  
  p = pOutR->pData;
  
  pc = pOutTau;
  for(col=0 ; col < pSrc->numCols; col++)
  {
      int32_t i,j,k,blkCnt;
      float32_t *pa0,*pa1,*pa2,*pa3;
      COPY_COL_F32(pOutR,col,col,pTmpA);

      beta = arm_householder_f32(pTmpA,threshold,pSrc->numRows - col,pTmpA);
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
          float32_t sum;

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
        float32_t f = beta * pTmpA[j];

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
     memset(pOutQ->pData,0,sizeof(float32_t)*pOutQ->numRows*pOutQ->numRows);
     
     pa = pOutQ->pData;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
        *pa = 1.0f;
        pa += pOutQ->numCols+1;
     }
   
     nb = pOutQ->numRows - pOutQ->numCols + 1;
   
     pc = pOutTau + pOutQ->numCols - 1;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
       int32_t i,j,k, blkCnt;
       float32_t *pa0,*pa1,*pa2,*pa3;
       pos = pSrc->numRows - nb;
       p = pOutQ->pData + pos + pOutQ->numCols*pos ;
   
       
       COPY_COL_F32(pOutR,pos,pos,pTmpA);
       pTmpA[0] = 1.0f;
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
           float32_t sum;

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
           float32_t f = beta * pTmpA[j];

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

#endif /* end of test for Helium or Neon availability */

/**
  @} end of MatrixQR group
 */
