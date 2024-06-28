/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_householder_f16.c
 * Description:  Half floating-point Householder transform
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

#include "dsp/matrix_functions_f16.h"
#include "dsp/basic_math_functions_f16.h"
#include "dsp/fast_math_functions_f16.h"

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
  @brief         Householder transform of a half floating point vector.
  @param[in]     pSrc        points to the input vector.
  @param[in]     threshold   norm2 threshold.  
  @param[in]     blockSize   dimension of the vector space.
  @param[out]    pOut        points to the output vector.
  @return        beta        return the scaling factor beta
 */


#if defined(ARM_FLOAT16_SUPPORTED)



ARM_DSP_ATTRIBUTE float16_t arm_householder_f16(
    const float16_t * pSrc,
    const float16_t threshold,
    uint32_t    blockSize,
    float16_t * pOut
    )

{
  uint32_t i;
  float16_t epsilon;
  float16_t x1norm2,alpha;
  float16_t beta,tau,r;

  epsilon = threshold;

  alpha = pSrc[0];

  for(i=1; i < blockSize; i++)
  {
    pOut[i] = pSrc[i];
  }
  pOut[0] = 1.0f16;

  arm_dot_prod_f16(pSrc+1,pSrc+1,blockSize-1,&x1norm2);

  if ((_Float16)x1norm2<=(_Float16)epsilon)
  {
     tau = 0.0f16;
     memset(pOut,0,blockSize * sizeof(float16_t));
  }
  else
  {
    beta =  (_Float16)alpha * (_Float16)alpha + (_Float16)x1norm2;
    (void)arm_sqrt_f16(beta,&beta);

    if ((_Float16)alpha > 0.0f16)
    {
      beta = -(_Float16)beta;
    }

    r = 1.0f16 / ((_Float16)alpha -(_Float16)beta);
    arm_scale_f16(pOut,r,pOut,blockSize);
    pOut[0] = 1.0f16;

    
    tau = ((_Float16)beta - (_Float16)alpha) / (_Float16)beta;

  }

  return(tau);

}


#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */
/**
  @} end of MatrixHouseholder group
 */
