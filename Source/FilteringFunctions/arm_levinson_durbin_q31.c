/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_levinson_durbin_q31.c
 * Description:  q31 version of Levinson Durbin algorithm
 *
 *
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

#include "dsp/filtering_functions.h"

#define ONE_Q31 0x7FFFFFFFL
#define TWO_Q30 0x7FFFFFFFL

#define HALF_Q31 0x00008000L
#define ONE_Q15 0x7FFF
#define HALF_Q15 0x3FFF
#define LOWPART_MASK 0x07FFF

#include <stdio.h>

#define FQ31(X) (1.0*(X) / ONE_Q31)
#define FQ15(X) (1.0*(X) / ONE_Q15)

#define PQ31(X)                                  \
  if ((X)>=0)                                       \
  {                                              \
     printf("%08X (%f)",(X),FQ31(X));  \
  }                                              \
  else                                           \
  {                                              \
     printf("-%08X (%f)",-(X),FQ31(X));\
  }

#define PQ15(X)                                  \
  if ((X)>=0)                                       \
  {                                              \
     printf("%04X (%f)",(X),FQ15(X));  \
  }                                              \
  else                                           \
  {                                              \
     printf("-%04X (%f)",-(X),FQ15(X));\
  }


__STATIC_FORCEINLINE q31_t mul32x16(q31_t a, q15_t b)
{
  q31_t r = ((q63_t)a * (q63_t)b) >> 15;

  return(r);
  
}

__STATIC_FORCEINLINE q31_t mul32x32(q31_t a, q31_t b)
{
  //q31_t r = __SSAT(((q63_t)a * b) >> 31,31);
  q31_t r = ((q63_t)a * b) >> 31;

  return(r);
  
}

__STATIC_FORCEINLINE q31_t divide(q31_t n, q31_t d)
{
  arm_status status;
  int16_t shift;
  q15_t inverse;
  q31_t r;
  // We are computing:
  // n / d = n / (h + l) where h and l are the high end and low end part.
  // 1 / (h + l) = 1 / h (1 - l / h)
  // Our division algorithm has a shift. So it is returning a scaled value sh.
  // So we need a << shift to convert 1/ sh to 1/h.
  // In below code, we are organizing the computation differently. Instead of computing:
  // 1 / h (1 - l / h) 
  // we are computing
  // 1 / h (2 - (l + h) / h) 
  // 1 / h (2 - d / h)
  // Also, we are not computing 1/h in Q15 but in Q14.
  // 2 is expressed in Q30.
  // So at the end of all computation we need a << 2

  // Result is in Q14 because of use of HALF_Q15 instead of ONE_Q15.
  status=arm_divide_q15(HALF_Q15,d>>16,&inverse,&shift);
  
  // d is used instead of l
  // So we will need to substract to 2 instead of 1.
  r = mul32x16(d,inverse);
  r = TWO_Q30 - (r << shift);
  r = mul32x16(r, inverse);
  r = mul32x32(r,n) ;
  r = r << (shift + 2);
  
  return(r);
  
}


/**
  @ingroup groupFilters
 */

/**
  @defgroup LD Levinson Durbin Algorithm

 */

/**
  @addtogroup LD
  @{
 */

/**
  @brief         Levinson Durbin
  @param[in]     phi      autocovariance vector starting with lag 0 (length is nbCoefs + 1)
  @param[out]    a        autoregressive coefficients
  @param[out]    err      prediction error (variance)
  @param[in]     nbCoefs  number of autoregressive coefficients
  @return        none
 */

void arm_levinson_durbin_q31(const q31_t *phi,
  q31_t *a, 
  q31_t *err,
  int nbCoefs)
{
   q31_t e;

   //a[0] = phi[1] / phi[0];
   a[0] = divide(phi[1], phi[0]);
   //printf("%08X %08X\n",phi[1],phi[0]);
   //printf("%f / %f = %f\n",FQ31(phi[1]),FQ31(phi[0]),FQ31(a[0]));
   

   //e = phi[0] - phi[1] * a[0];
   e = phi[0] - mul32x32(phi[1],a[0]);

   for(int p=1; p < nbCoefs; p++)
   {
      q63_t suma=0;
      q63_t sumb=0;
      q31_t k;
      int nb,j;

      for(int i=0; i < p; i++)
      {
         suma += ((q63_t)a[i] * phi[p - i]);
         sumb += ((q63_t)a[i] * phi[i + 1]);
      }

      suma = suma >> 31;
      sumb = sumb >> 31;

      //printf("suma = %08X, sumb=%08X\n",(q31_t)(suma & 0x0FFFFFFFF),(q31_t)(sumb & 0x0FFFFFFFF));


      //k = (phi[p+1]-suma)/(phi[0] - sumb);
      k = divide(phi[p+1]-(q31_t)suma,phi[0] - (q31_t)sumb);


      nb = p >> 1;
      j=0;
      for(int i =0;i < nb ; i++)
      {
          q31_t x,y;

          //x = a[j] - k * a[p-1-j];
          x = a[j] - mul32x32(k,a[p-1-j]);

          //y = a[p-1-j] - k * a[j];
          y = a[p-1-j] - mul32x32(k , a[j]);

          a[j] = x;
          a[p-1-j] = y;

          j++;
      }

      nb = p & 1;
      if (nb)
      {
            //a[j] = a[j]- k * a[p-1-j];
            a[j] = a[j] - mul32x32(k,a[p-1-j]);
      }

      a[p] = k;

      // e = e * (1 - k*k);
      e = mul32x32(e,ONE_Q31 - mul32x32(k,k));


   }
   *err = e;
}

/**
  @} end of LD group
 */
