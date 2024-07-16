
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_dtw_path_f32.c
 * Description:  Warping path
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
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

#include "dsp/distance_functions.h"
#include <limits.h>
#include <math.h>

#define E(MAT,R,C) \
  (*((MAT)->pData + (MAT)->numCols*(R) + (C)))



/**
  @addtogroup DTW
  @{
 */


/**
 * @brief        Mapping between query and template
 * @param[in]    pDTW  Cost matrix (Query rows * Template columns)
 * @param[out]   pPath Warping path in cost matrix 2*(nb rows + nb columns)
 * @param[out]   pathLength Length of path in number of points
 * 
 * @par Warping path
 * 
 * The warping path has length which is at most
 * 2*(query length + template length) in float.
 * 2 because it is a list of coordinates : 
 * (query index, template index) coordinate.
 * 
 * The buffer pPath must be big enough to contain
 * the warping path.
 * 
 * pathLength is the number of points in 
 * the returned path. The resturned path
 * may be smaller than query + template.
 *
 */
ARM_DSP_ATTRIBUTE void arm_dtw_path_f32(const arm_matrix_instance_f32 *pDTW,
                      int16_t *pPath,
                      uint32_t *pathLength)
{
  int q,t;
  float32_t temp;

  *pathLength = 0;
  q=pDTW->numRows-1;
  t=pDTW->numCols-1;
  while((q>0) || (t>0))
  {
    int p=-1;
    float32_t current=F32_MAX;
 
    if (q>0)
    {
      temp = E(pDTW,q-1,t);
      if (temp<current)
      {
        current=temp;
        p=2;
      }
    }

    if (t>0) 
    { 
      temp = E(pDTW,q,t-1);
      if (temp<current)
      {
        current=temp;
        p=0;
      }
    }

    if ((q>0) && (t>0))
    {
      temp = E(pDTW,q-1,t-1);
      if (temp<current)
      {
        current=temp;
        p=1;
      }
    }

    

   

   

    pPath[2 * (*pathLength)] = q;
    pPath[2 * (*pathLength) + 1] = t;

    *pathLength = *pathLength + 1;

    switch(p)
    {
      case 0:
        t = t-1;
      break;
      case 1:
        t=t-1;
        q=q-1;
      break;
      case 2:
        q=q-1;
      break;
    }

  }
  
  pPath[2 * (*pathLength)] = 0;
  pPath[2 * (*pathLength) + 1] = 0;
  *pathLength = *pathLength + 1;

  /* Reverse the path */
  int16_t *fh,*sh;
  int16_t itemp;
  fh = pPath;
  sh = pPath + 2* (*pathLength)-2;
  int halfLength = (*pathLength)>>1;
  for(int i = 0; i< halfLength; i++)
  {
     itemp = fh[0];
     fh[0] = sh[0];
     sh[0] = itemp;

     itemp = fh[1];
     fh[1] = sh[1];
     sh[1] = itemp;

     fh += 2;
     sh -= 2;
  }
}

/**
 * @} end of DTW group
 */

