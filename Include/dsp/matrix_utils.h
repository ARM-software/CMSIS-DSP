/******************************************************************************
 * @file     matrix_utils.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.11.0
 * @date     30 May 2022
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2022 Arm Limited or its affiliates. All rights reserved.
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

 
#ifndef MATRIX_UTILS_H_
#define MATRIX_UTILS_H_

#include "arm_math_types.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"

#ifdef   __cplusplus
extern "C"
{
#endif

#define ELEM(A,ROW,COL) &((A)->pData[(A)->numCols* (ROW) + (COL)])

#define SCALE_COL_T(T,CAST,A,ROW,v,i)        \
{                                       \
  int32_t _w;                            \
  T *data = (A)->pData;                 \
  const int32_t _numCols = (A)->numCols; \
  const int32_t nb = (A)->numRows - ROW;\
                                        \
  data += i + _numCols * (ROW);          \
                                        \
  for(_w=0;_w < nb; _w++)                  \
  {                                     \
     *data = CAST *data * CAST v;                   \
     data += _numCols;                   \
  }                                     \
}

#define COPY_COL_T(T,A,ROW,COL,DST)               \
{                                                 \
    uint32_t _row;                                \
    T *_pb=DST;                                    \
    T *_pa = (A)->pData + ROW * (A)->numCols + COL;\
    for(_row = ROW; _row < (A)->numRows; _row ++) \
    {                                             \
         *_pb++ = *_pa;                             \
         _pa += (A)->numCols;                      \
    }                                             \
}

#if defined(ARM_FLOAT16_SUPPORTED)
#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#define SWAP_ROWS_F16(A,COL,i,j)                  \
  {                                               \
    int cnt = ((A)->numCols)-(COL);               \
    int32_t _w;                                    \
    float16_t *data = (A)->pData;                 \
    const int32_t _numCols = (A)->numCols;        \
                                                  \
    for(_w=(COL);_w < _numCols; _w+=8)               \
    {                                             \
       f16x8_t tmpa,tmpb;                         \
       mve_pred16_t p0 = vctp16q(cnt);            \
                                                  \
       tmpa=vldrhq_z_f16(&data[i*_numCols + _w],p0);\
       tmpb=vldrhq_z_f16(&data[j*_numCols + _w],p0);\
                                                  \
       vstrhq_p(&data[i*_numCols + _w], tmpb, p0);  \
       vstrhq_p(&data[j*_numCols + _w], tmpa, p0);  \
                                                  \
       cnt -= 8;                                  \
    }                                             \
  }

#define SCALE_ROW_F16(A,COL,v,i)                   \
{                                                   \
  int cnt = ((A)->numCols)-(COL);                   \
  int32_t _w;                                       \
  float16_t *data = (A)->pData;                     \
  const int32_t _numCols = (A)->numCols;            \
                                                    \
  for(_w=(COL);_w < _numCols; _w+=8)                    \
  {                                                 \
       f16x8_t tmpa;                                \
       mve_pred16_t p0 = vctp16q(cnt);              \
       tmpa = vldrhq_z_f16(&data[i*_numCols + _w],p0);\
       tmpa = vmulq_n_f16(tmpa,(_Float16)v);                  \
       vstrhq_p(&data[i*_numCols + _w], tmpa, p0);    \
       cnt -= 8;                                    \
  }                                                 \
                                                    \
}

#define MAC_ROW_F16(COL,A,i,v,B,j)                   \
{                                                    \
  int cnt = ((A)->numCols)-(COL);                    \
  int32_t _w;                                        \
  float16_t *dataA = (A)->pData;                     \
  float16_t *dataB = (B)->pData;                     \
  const int32_t _numCols = (A)->numCols;             \
                                                     \
  for(_w=(COL);_w < _numCols; _w+=8)                     \
  {                                                  \
       f16x8_t tmpa,tmpb;                            \
       mve_pred16_t p0 = vctp16q(cnt);               \
       tmpa = vldrhq_z_f16(&dataA[i*_numCols + _w],p0);\
       tmpb = vldrhq_z_f16(&dataB[j*_numCols + _w],p0);\
       tmpa = vfmaq_n_f16(tmpa,tmpb,v);              \
       vstrhq_p(&dataA[i*_numCols + _w], tmpa, p0);    \
       cnt -= 8;                                     \
  }                                                  \
                                                     \
}

#define MAS_ROW_F16(COL,A,i,v,B,j)                   \
{                                                    \
  int cnt = ((A)->numCols)-(COL);                    \
  int32_t _w;                                        \
  float16_t *dataA = (A)->pData;                     \
  float16_t *dataB = (B)->pData;                     \
  const int32_t _numCols = (A)->numCols;             \
  f16x8_t vec=vdupq_n_f16(v);                        \
                                                     \
  for(_w=(COL);_w < _numCols; _w+=8)                     \
  {                                                  \
       f16x8_t tmpa,tmpb;                            \
       mve_pred16_t p0 = vctp16q(cnt);               \
       tmpa = vldrhq_z_f16(&dataA[i*_numCols + _w],p0);\
       tmpb = vldrhq_z_f16(&dataB[j*_numCols + _w],p0);\
       tmpa = vfmsq_f16(tmpa,tmpb,vec);              \
       vstrhq_p(&dataA[i*_numCols + _w], tmpa, p0);    \
       cnt -= 8;                                     \
  }                                                  \
                                                     \
}

#else


#define SWAP_ROWS_F16(A,COL,i,j)       \
{                                      \
  int32_t _w;                           \
  float16_t *dataI = (A)->pData;       \
  float16_t *dataJ = (A)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);    \
                                       \
  dataI += i*_numCols + (COL);          \
  dataJ += j*_numCols + (COL);          \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     float16_t tmp;                    \
     tmp = *dataI;                     \
     *dataI++ = *dataJ;                \
     *dataJ++ = tmp;                   \
  }                                    \
}

#define SCALE_ROW_F16(A,COL,v,i)        \
{                                       \
  int32_t _w;                           \
  float16_t *data = (A)->pData;         \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);    \
                                        \
  data += i*_numCols + (COL);           \
                                        \
  _Float16 sum;                         \
  for(_w=0;_w < nb; _w++)               \
  {                                     \
     sum = *data;                       \
     sum *= (_Float16)v;                \
     *data++ = sum;                     \
  }                                     \
}


#define MAC_ROW_F16(COL,A,i,v,B,j)           \
{                                            \
  int32_t _w;                                \
  float16_t *dataA = (A)->pData;             \
  float16_t *dataB = (B)->pData;             \
  const int32_t _numCols = (A)->numCols;     \
  const int32_t nb = _numCols-(COL);         \
                                             \
  dataA += i*_numCols + (COL);               \
  dataB += j*_numCols + (COL);               \
                                             \
  _Float16 sum ;                             \
  for(_w=0;_w < nb; _w++)                    \
  {                                          \
     sum = *dataA;                           \
     sum += (_Float16)v * (_Float16)*dataB++;\
     *dataA++ = sum;                         \
  }                                          \
}

#define MAS_ROW_F16(COL,A,i,v,B,j)           \
{                                            \
  int32_t _w;                                \
  float16_t *dataA = (A)->pData;             \
  float16_t *dataB = (B)->pData;             \
  const int32_t _numCols = (A)->numCols;     \
  const int32_t nb = _numCols-(COL);         \
                                             \
  dataA += i*_numCols + (COL);               \
  dataB += j*_numCols + (COL);               \
                                             \
  _Float16 sum ;                             \
  for(_w=0;_w < nb; _w++)                    \
  {                                          \
     sum = *dataA;                           \
     sum -= (_Float16)v * (_Float16)*dataB++;\
     *dataA++ = sum;                         \
  }                                          \
}

#endif /*defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)*/

/* Functions with only a scalar version */
#define COPY_COL_F16(A,ROW,COL,DST) \
  COPY_COL_T(float16_t,A,ROW,COL,DST)

#define SCALE_COL_F16(A,ROW,v,i)        \
  SCALE_COL_T(float16_t,(_Float16),A,ROW,v,i)
  
#endif /* defined(ARM_FLOAT16_SUPPORTED)*/

#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#define SWAP_ROWS_F32(A,COL,i,j)                  \
  {                                               \
    int cnt = ((A)->numCols)-(COL);               \
    float32_t *data = (A)->pData;                 \
    const int32_t _numCols = (A)->numCols;        \
    int32_t _w;                                   \
                                                  \
    for(_w=(COL);_w < _numCols; _w+=4)                \
    {                                             \
       f32x4_t tmpa,tmpb;                         \
       mve_pred16_t p0 = vctp32q(cnt);            \
                                                  \
       tmpa=vldrwq_z_f32(&data[i*_numCols + _w],p0);\
       tmpb=vldrwq_z_f32(&data[j*_numCols + _w],p0);\
                                                  \
       vstrwq_p(&data[i*_numCols + _w], tmpb, p0);  \
       vstrwq_p(&data[j*_numCols + _w], tmpa, p0);  \
                                                  \
       cnt -= 4;                                  \
    }                                             \
  }

#define MAC_ROW_F32(COL,A,i,v,B,j)                   \
{                                                    \
  int cnt = ((A)->numCols)-(COL);                    \
  float32_t *dataA = (A)->pData;                     \
  float32_t *dataB = (B)->pData;                     \
  const int32_t _numCols = (A)->numCols;             \
  int32_t _w;                                        \
                                                     \
  for(_w=(COL);_w < _numCols; _w+=4)                     \
  {                                                  \
       f32x4_t tmpa,tmpb;                            \
       mve_pred16_t p0 = vctp32q(cnt);               \
       tmpa = vldrwq_z_f32(&dataA[i*_numCols + _w],p0);\
       tmpb = vldrwq_z_f32(&dataB[j*_numCols + _w],p0);\
       tmpa = vfmaq_n_f32(tmpa,tmpb,v);              \
       vstrwq_p(&dataA[i*_numCols + _w], tmpa, p0);    \
       cnt -= 4;                                     \
  }                                                  \
                                                     \
}

#define MAS_ROW_F32(COL,A,i,v,B,j)                   \
{                                                    \
  int cnt = ((A)->numCols)-(COL);                    \
  float32_t *dataA = (A)->pData;                     \
  float32_t *dataB = (B)->pData;                     \
  const int32_t _numCols = (A)->numCols;             \
  int32_t _w;                                        \
  f32x4_t vec=vdupq_n_f32(v);                        \
                                                     \
  for(_w=(COL);_w < _numCols; _w+=4)                     \
  {                                                  \
       f32x4_t tmpa,tmpb;                            \
       mve_pred16_t p0 = vctp32q(cnt);               \
       tmpa = vldrwq_z_f32(&dataA[i*_numCols + _w],p0);\
       tmpb = vldrwq_z_f32(&dataB[j*_numCols + _w],p0);\
       tmpa = vfmsq_f32(tmpa,tmpb,vec);              \
       vstrwq_p(&dataA[i*_numCols + _w], tmpa, p0);    \
       cnt -= 4;                                     \
  }                                                  \
                                                     \
}

#define SCALE_ROW_F32(A,COL,v,i)                    \
{                                                   \
  int cnt = ((A)->numCols)-(COL);                   \
  float32_t *data = (A)->pData;                     \
  const int32_t _numCols = (A)->numCols;            \
  int32_t _w;                                       \
                                                    \
  for(_w=(COL);_w < _numCols; _w+=4)                    \
  {                                                 \
       f32x4_t tmpa;                                \
       mve_pred16_t p0 = vctp32q(cnt);              \
       tmpa = vldrwq_z_f32(&data[i*_numCols + _w],p0);\
       tmpa = vmulq_n_f32(tmpa,v);                  \
       vstrwq_p(&data[i*_numCols + _w], tmpa, p0);    \
       cnt -= 4;                                    \
  }                                                 \
                                                    \
}

#elif defined(ARM_MATH_NEON)

#define SWAP_ROWS_F32(A,COL,i,j)       \
{                                      \
  int32_t _w;                           \
  float32_t *dataI = (A)->pData;       \
  float32_t *dataJ = (A)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols - COL;    \
                                       \
  dataI += i*_numCols + (COL);          \
  dataJ += j*_numCols + (COL);          \
                                       \
  float32_t tmp;                       \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     tmp = *dataI;                     \
     *dataI++ = *dataJ;                \
     *dataJ++ = tmp;                   \
  }                                    \
}

#define MAC_ROW_F32(COL,A,i,v,B,j)     \
{                                      \
  float32_t *dataA = (A)->pData;       \
  float32_t *dataB = (B)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols - (COL);  \
  int32_t nbElems;                     \
  f32x4_t vec = vdupq_n_f32(v);        \
                                       \
  nbElems = nb >> 2;                   \
                                       \
  dataA += i*_numCols + (COL);          \
  dataB += j*_numCols + (COL);          \
                                       \
  while(nbElems>0)                     \
  {                                    \
       f32x4_t tmpa,tmpb;              \
       tmpa = vld1q_f32(dataA,p0);     \
       tmpb = vld1q_f32(dataB,p0);     \
       tmpa = vmlaq_f32(tmpa,tmpb,vec);\
       vst1q_f32(dataA, tmpa, p0);     \
       nbElems--;                      \
       dataA += 4;                     \
       dataB += 4;                     \
  }                                    \
                                       \
  nbElems = nb & 3;                    \
  while(nbElems > 0)                   \
  {                                    \
     *dataA++ += v* *dataB++;          \
     nbElems--;                        \
  }                                    \
}

#define MAS_ROW_F32(COL,A,i,v,B,j)     \
{                                      \
  float32_t *dataA = (A)->pData;       \
  float32_t *dataB = (B)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols - (COL);  \
  int32_t nbElems;                     \
  f32x4_t vec = vdupq_n_f32(v);        \
                                       \
  nbElems = nb >> 2;                   \
                                       \
  dataA += i*_numCols + (COL);          \
  dataB += j*_numCols + (COL);          \
                                       \
  while(nbElems>0)                     \
  {                                    \
       f32x4_t tmpa,tmpb;              \
       tmpa = vld1q_f32(dataA);        \
       tmpb = vld1q_f32(dataB);        \
       tmpa = vmlsq_f32(tmpa,tmpb,vec);\
       vst1q_f32(dataA, tmpa);         \
       nbElems--;                      \
       dataA += 4;                     \
       dataB += 4;                     \
  }                                    \
                                       \
  nbElems = nb & 3;                    \
  while(nbElems > 0)                   \
  {                                    \
     *dataA++ -= v* *dataB++;          \
     nbElems--;                        \
  }                                    \
}

#define SCALE_ROW_F32(A,COL,v,i)        \
{                                       \
  float32_t *data = (A)->pData;         \
  const int32_t _numCols = (A)->numCols; \
  const int32_t nb = _numCols - (COL);   \
  int32_t nbElems;                      \
  f32x4_t vec = vdupq_n_f32(v);         \
                                        \
  nbElems = nb >> 2;                    \
                                        \
  data += i*_numCols + (COL);            \
  while(nbElems>0)                      \
  {                                     \
       f32x4_t tmpa;                    \
       tmpa = vld1q_f32(data);          \
       tmpa = vmulq_f32(tmpa,vec);      \
       vst1q_f32(data, tmpa);           \
       data += 4;                       \
       nbElems --;                      \
  }                                     \
                                        \
  nbElems = nb & 3;                     \
  while(nbElems > 0)                    \
  {                                     \
     *data++ *= v;                      \
     nbElems--;                         \
  }                                     \
                                        \
}

#else

#define SWAP_ROWS_F32(A,COL,i,j)       \
{                                      \
  int32_t _w;                           \
  float32_t tmp;                       \
  float32_t *dataI = (A)->pData;       \
  float32_t *dataJ = (A)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols - COL;    \
                                       \
  dataI += i*_numCols + (COL);          \
  dataJ += j*_numCols + (COL);          \
                                       \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     tmp = *dataI;                     \
     *dataI++ = *dataJ;                \
     *dataJ++ = tmp;                   \
  }                                    \
}

#define SCALE_ROW_F32(A,COL,v,i)       \
{                                      \
  int32_t _w;                           \
  float32_t *data = (A)->pData;        \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols - COL;    \
                                       \
  data += i*_numCols + (COL);           \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     *data++ *= v;                     \
  }                                    \
}


#define MAC_ROW_F32(COL,A,i,v,B,j)     \
{                                      \
  int32_t _w;                           \
  float32_t *dataA = (A)->pData;       \
  float32_t *dataB = (B)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);    \
                                       \
  dataA = dataA + i*_numCols + (COL);   \
  dataB = dataB + j*_numCols + (COL);   \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     *dataA++ += v* *dataB++;          \
  }                                    \
}

#define MAS_ROW_F32(COL,A,i,v,B,j)     \
{                                      \
  int32_t _w;                           \
  float32_t *dataA = (A)->pData;       \
  float32_t *dataB = (B)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);    \
                                       \
  dataA = dataA + i*_numCols + (COL);   \
  dataB = dataB + j*_numCols + (COL);   \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     *dataA++ -= v* *dataB++;          \
  }                                    \
}

#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */


/* Functions _with only a scalar version */

#define COPY_COL_F32(A,ROW,COL,DST) \
  COPY_COL_T(float32_t,A,ROW,COL,DST)

#define COPY_COL_F64(A,ROW,COL,DST) \
  COPY_COL_T(float64_t,A,ROW,COL,DST)

#define SWAP_COLS_F32(A,COL,i,j)               \
{                                              \
  int32_t _w;                                  \
  float32_t *data = (A)->pData;                \
  const int32_t _numCols = (A)->numCols;       \
  for(_w=(COL);_w < _numCols; _w++)                \
  {                                            \
     float32_t tmp;                            \
     tmp = data[_w*_numCols + i];                \
     data[_w*_numCols + i] = data[_w*_numCols + j];\
     data[_w*_numCols + j] = tmp;                \
  }                                            \
}

#define SCALE_COL_F32(A,ROW,v,i)        \
  SCALE_COL_T(float32_t,,A,ROW,v,i)

#define SWAP_ROWS_F64(A,COL,i,j)       \
{                                      \
  int32_t _w;                           \
  float64_t *dataI = (A)->pData;       \
  float64_t *dataJ = (A)->pData;       \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);    \
                                       \
  dataI += i*_numCols + (COL);          \
  dataJ += j*_numCols + (COL);          \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     float64_t tmp;                    \
     tmp = *dataI;                     \
     *dataI++ = *dataJ;                \
     *dataJ++ = tmp;                   \
  }                                    \
}

#define SWAP_COLS_F64(A,COL,i,j)               \
{                                              \
  int32_t _w;                                  \
  float64_t *data = (A)->pData;                \
  const int32_t _numCols = (A)->numCols;       \
  for(_w=(COL);_w < _numCols; _w++)                \
  {                                            \
     float64_t tmp;                            \
     tmp = data[_w*_numCols + i];                \
     data[_w*_numCols + i] = data[_w*_numCols + j];\
     data[_w*_numCols + j] = tmp;                \
  }                                            \
}

#define SCALE_ROW_F64(A,COL,v,i)       \
{                                      \
  int32_t _w;                           \
  float64_t *data = (A)->pData;        \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);    \
                                       \
  data += i*_numCols + (COL);           \
                                       \
  for(_w=0;_w < nb; _w++)                 \
  {                                    \
     *data++ *= v;                     \
  }                                    \
}

#define SCALE_COL_F64(A,ROW,v,i)        \
  SCALE_COL_T(float64_t,,A,ROW,v,i)

#define MAC_ROW_F64(COL,A,i,v,B,j)      \
{                                       \
  int32_t _w;                           \
  float64_t *dataA = (A)->pData;        \
  float64_t *dataB = (B)->pData;        \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);     \
                                        \
  dataA += i*_numCols + (COL);           \
  dataB += j*_numCols + (COL);           \
                                        \
  for(_w=0;_w < nb; _w++)                  \
  {                                     \
     *dataA++ += v* *dataB++;           \
  }                                     \
}

#define MAS_ROW_F64(COL,A,i,v,B,j)      \
{                                       \
  int32_t _w;                           \
  float64_t *dataA = (A)->pData;        \
  float64_t *dataB = (B)->pData;        \
  const int32_t _numCols = (A)->numCols;\
  const int32_t nb = _numCols-(COL);     \
                                        \
  dataA += i*_numCols + (COL);           \
  dataB += j*_numCols + (COL);           \
                                        \
  for(_w=0;_w < nb; _w++)                  \
  {                                     \
     *dataA++ -= v* *dataB++;           \
  }                                     \
}

#ifdef   __cplusplus
}
#endif

#endif /* ifndef _MATRIX_UTILS_H_ */
