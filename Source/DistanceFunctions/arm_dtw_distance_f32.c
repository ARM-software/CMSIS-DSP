
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_braycurtis_distance_f32.c
 * Description:  Bray-Curtis distance between two vectors
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

#include "dsp/distance_functions.h"
#include "dsp/matrix_utils.h"
#include <limits.h>
#include <math.h>

#define E(MAT,R,C) \
  (*((MAT)->pData + (MAT)->numCols*(R) + (C)))

#define WIN(R,C)                                             \
  ((pWindow == NULL) ? 1 :                                   \
   ((*((pWindow)->pData + (pWindow)->numCols*(R) + (C)))==1))

/**
  @ingroup FloatDist
 */

/**
  @defgroup DTW Dynamic Time Warping Distance

  Dynamic Time Warping Distance.

  This is not really a distance since triangular inequality is
  not respected.

  The step pattern used is symmetric2.
  Future versions of this function will provide more customization options.

 */


/**
  @addtogroup DTW
  @{
 */


/**
 * @brief         Dynamic Time Warping distance
 * @param[in]     pDistance  Distance matrix (Query rows * Template columns)
 * @param[in]     pWindow  Windowing matrix (can be NULL if no windowing used)
 * @param[out]    pDTW Temporary cost buffer (same size)
 * @param[out]    distance Distance
 * @return ARM_MATH_ARGUMENT_ERROR in case no path can be found with window constraint
 *
 * @par Windowing matrix
 * 
 * The windowing matrix is used to impose some
 * constraints on the search for a path.
 * The algorithm will run faster (smaller search
 * path) but may not be able to find a solution.
 * 
 * The distance matrix must be initialized only
 * where the windowing matrix is containing 1.
 * Thus, use of a window also decreases the number
 * of distances which must be computed.
 */
ARM_DSP_ATTRIBUTE arm_status arm_dtw_distance_f32(const arm_matrix_instance_f32 *pDistance,
                                const arm_matrix_instance_q7 *pWindow,
                                arm_matrix_instance_f32 *pDTW,
                                float32_t *distance)
{
   const uint32_t queryLength = pDistance -> numRows;
   const uint32_t templateLength = pDistance -> numCols;
   float32_t result;

   float32_t *temp = pDTW->pData;
   for(uint32_t q=0 ; q < queryLength; q++)
   {
     for(uint32_t t= 0; t < templateLength; t++)
     {
        *temp++ = F32_MAX;
     }
   }

   pDTW->pData[0] = E(pDistance,0,0);
   for(uint32_t q = 1; q < queryLength; q++)
   {
     if (!WIN(q,0))
     {
        continue;
     }
     E(pDTW,q,0) = E(pDTW,q-1,0) + E(pDistance,q,0);
   }

   for(uint32_t t = 1; t < templateLength; t++)
   {
     if (!WIN(0,t))
     {
        continue;
     }
     E(pDTW,0,t) = E(pDTW,0,t-1) + E(pDistance,0,t);
   }


   for(uint32_t q = 1; q < queryLength; q++)
   {
     for(uint32_t t = 1; t < templateLength; t++)
     {
       if (!WIN(q,t))
       {
          continue;
       }
       E(pDTW,q,t) = 
            MIN(E(pDTW,q-1,t-1) + 2.0f * E(pDistance,q,t),
            MIN(E(pDTW,q,t-1)   +        E(pDistance,q,t),
                E(pDTW,q-1,t)   +        E(pDistance,q,t)));
     }
   }

   if (E(pDTW,queryLength-1,templateLength-1) == F32_MAX)
   {
     return(ARM_MATH_ARGUMENT_ERROR);
   }

   result = E(pDTW,queryLength-1,templateLength-1);
   result = result / (queryLength + templateLength);
   *distance = result;

   return(ARM_MATH_SUCCESS);
}

/**
 * @} end of DTW group
 */

