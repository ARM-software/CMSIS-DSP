/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_householder_f32.c
 * Description:  Floating-point Householder transform
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
  @defgroup MatrixHouseholder Householder transform of a vector

    Computes the Householder transform of a vector x.

    The Householder transform of x is a vector v with 

    \f[
        v_0 = 1
    \f]

    and a scalar \f$\beta\f$ such that:

    \f[
    P = I - \beta v v^T 
    \f]

    is an orthogonal matrix and 

    \f[
    P x = ||x||_2 e_1
    \f]

    So P is an hyperplane reflection such that the image of x 
    is proportional to \f$e_1\f$. 

    \f$e_1\f$ is the vector of coordinates:

    \f[
    \begin{pmatrix}
        1       \\
        0       \\
        \vdots  \\
    \end{pmatrix}
    \f]

    If x is already proportional to \f$e_1\f$ then 
    the matrix P should be the identity. 

    Thus, \f$\beta\f$ should be 0 and in this case the vector v
    can also be null.

    But how do we detect that x is already proportional to 
    \f$e_1\f$.

    If x
    \f[
    x =
    \begin{pmatrix}
        x_0     \\
        xr      \\
    \end{pmatrix}
    \f]

    where \f$xr\f$ is a vector.

    The algorithm is computing the norm squared of this vector:

    \f[
        ||xr||^2
    \f]

    and this value is compared to a `threshold`. If the value
    is smaller than the `threshold`, the algorithm is
    returning 0 for \f$\beta\f$ and the householder vector. 

    This `threshold` is an argument of the function.

    Default values are provided in the header 
    `dsp/matrix_functions.h` like for instance
    `DEFAULT_HOUSEHOLDER_THRESHOLD_F32`



 */

/**
  @addtogroup MatrixHouseholder
  @{
 */

/**
  @brief         Householder transform of a floating point vector.
  @param[in]     pSrc        points to the input vector.
  @param[in]     threshold   norm2 threshold.  
  @param[in]     blockSize   dimension of the vector space.
  @param[out]    pOut        points to the output vector.
  @return        beta        return the scaling factor beta
 */




float32_t arm_householder_f32(
    const float32_t * pSrc,
    const float32_t threshold,
    uint32_t    blockSize,
    float32_t * pOut
    )

{
  uint32_t i;
  float32_t epsilon;
  float32_t x1norm2,alpha;
  float32_t beta,tau,r;

  epsilon = threshold;

  alpha = pSrc[0];

  for(i=1; i < blockSize; i++)
  {
    pOut[i] = pSrc[i];
  }
  pOut[0] = 1.0f;

  arm_dot_prod_f32(pSrc+1,pSrc+1,blockSize-1,&x1norm2);

  if (x1norm2<=epsilon)
  {
     tau = 0.0f;
     memset(pOut,0,blockSize * sizeof(float32_t));
  }
  else
  {
    beta =  alpha * alpha + x1norm2;
    (void)arm_sqrt_f32(beta,&beta);

    if (alpha > 0.0f)
    {
      beta = -beta;
    }

    r = 1.0f / (alpha -beta);
    arm_scale_f32(pOut,r,pOut,blockSize);
    pOut[0] = 1.0f;

    
    tau = (beta - alpha) / beta;

  }

  return(tau);

}


/**
  @} end of MatrixHouseholder group
 */
