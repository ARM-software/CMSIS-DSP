/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_householder_f64.c
 * Description:  Double floating-point Householder transform
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
#include "dsp/basic_math_functions.h"
#include "dsp/fast_math_functions.h"

#include "dsp/matrix_utils.h"

#include <math.h>



/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixHouseholder
  @{
 */

/**
  @brief         Householder transform of a double floating point vector.
  @param[in]     pSrc        points to the input vector.
  @param[in]     threshold   norm2 threshold.  
  @param[in]     blockSize   dimension of the vector space.
  @param[out]    pOut        points to the output vector.
  @return        beta        return the scaling factor beta
 */




float64_t arm_householder_f64(
    const float64_t * pSrc,
    const float64_t threshold,
    uint32_t    blockSize,
    float64_t * pOut
    )

{
  uint32_t i;
  float64_t epsilon;
  float64_t x1norm2,alpha;
  float64_t beta,tau,r;

  epsilon = threshold;

  alpha = pSrc[0];

  for(i=1; i < blockSize; i++)
  {
    pOut[i] = pSrc[i];
  }
  pOut[0] = 1.0;

  arm_dot_prod_f64(pSrc+1,pSrc+1,blockSize-1,&x1norm2);

  if (x1norm2<=epsilon)
  {
     tau = 0.0;
     memset(pOut,0,blockSize * sizeof(float64_t));
  }
  else
  {
    beta =  alpha * alpha + x1norm2;
    beta=sqrt(beta);

    if (alpha > 0.0)
    {
      beta = -beta;
    }

    r = 1.0 / (alpha -beta);
    arm_scale_f64(pOut,r,pOut,blockSize);
    pOut[0] = 1.0;

    
    tau = (beta - alpha) / beta;

  }

  return(tau);

}


/**
  @} end of MatrixHouseholder group
 */
