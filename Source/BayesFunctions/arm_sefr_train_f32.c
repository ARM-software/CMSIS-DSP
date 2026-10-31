/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_sefr_train_f32
 * Description:  One pass training of a SEFR classifier
 *
 * $Date:        31 October 2026
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */

/*
 * Copyright (C) 2026 Hamidreza Keshavarz. All rights reserved.
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

#include "dsp/bayes_functions.h"
#include "dsp/basic_math_functions.h"
#include "dsp/support_functions.h"

/**
 * @addtogroup groupBayes
 * @{
 */

/**
 * @brief SEFR training
 *
 * @param[in,out]  S            points to a SEFR instance structure
 * @param[in]      pSrc         points to numSamples * vectorDimension training values, row major
 * @param[in]      pLabels      points to numSamples class indices in [0, numberOfClasses)
 * @param[in]      numSamples   number of training samples
 * @param[out]     pTmp         points to a temporary buffer of arm_sefr_train_tmp_buffer_size() values
 * @return ARM_MATH_SUCCESS, or ARM_MATH_ARGUMENT_ERROR if there are no samples, if a label is out of
 *         range, or if a class has no samples or holds all of them. Do not use the model after an error.
 *
 * @par  Algorithm
 *       One pass over the data collects the sum of each class, the class counts and the minimum and
 *       maximum of each feature. Features are scaled to [0, 1]. For class k and feature j, p is the mean
 *       of the class, r is the mean of all other samples, and the weight is (p - r) / (p + r). The
 *       threshold is the mean of w . p and w . r, each weighted by the size of the other group.
 *       The final weights and bias include the scaling, so the predictor takes raw features.
 */
ARM_DSP_ATTRIBUTE arm_status arm_sefr_train_f32(const arm_sefr_instance_f32 *S,
   const float32_t *pSrc,
   const uint32_t *pLabels,
   uint32_t numSamples,
   float32_t *pTmp)
{
  const uint32_t d = S->vectorDimension, K = S->numberOfClasses;
  float32_t *W = S->pWeights, *b = S->pBias;
  float32_t *lo = pTmp, *hi = pTmp + d, *n = pTmp + 2U * d, *sn = pTmp + 2U * d + K;
  const float32_t N = (float32_t)numSamples;
  uint32_t i, j, k;

  if (numSamples == 0U)
  {
    return ARM_MATH_ARGUMENT_ERROR;
  }

  /* One pass: class sums go straight into the weight buffer. */
  arm_fill_f32(0.0f, W, K * d);
  arm_fill_f32(0.0f, n, K);
  arm_copy_f32(pSrc, lo, d);
  arm_copy_f32(pSrc, hi, d);
  for (i = 0U; i < numSamples; i++)
  {
    const float32_t *x = pSrc + i * d;
    k = pLabels[i];
    if (k >= K)
    {
      return ARM_MATH_ARGUMENT_ERROR;
    }
    arm_add_f32(W + k * d, x, W + k * d, d);
    n[k] += 1.0f;
    for (j = 0U; j < d; j++)
    {
      lo[j] = x[j] < lo[j] ? x[j] : lo[j];
      hi[j] = x[j] > hi[j] ? x[j] : hi[j];
    }
  }
  for (k = 0U; k < K; k++)
  {
    if (n[k] < 1.0f || n[k] > N - 1.0f)
    {
      return ARM_MATH_ARGUMENT_ERROR;
    }
  }

  /* Per feature: mean of the class (p) and of the rest (r) in min-max scaled space, w = (p - r) / (p + r).
     b and sn accumulate w . p and w . r for the threshold. */
  arm_fill_f32(0.0f, b, K);
  arm_fill_f32(0.0f, sn, K);
  for (j = 0U; j < d; j++)
  {
    float32_t total = 0.0f;
    for (k = 0U; k < K; k++)
    {
      total += W[k * d + j];
    }
    hi[j] = hi[j] - lo[j] > 0.0f ? hi[j] - lo[j] : 1.0f; /* hi now holds the range */
    for (k = 0U; k < K; k++)
    {
      const float32_t p = (W[k * d + j] / n[k] - lo[j]) / hi[j];
      const float32_t r = ((total - W[k * d + j]) / (N - n[k]) - lo[j]) / hi[j];
      const float32_t w = (p - r) / (p + r + 1e-7f);
      W[k * d + j] = w;
      b[k] += w * p;
      sn[k] += w * r;
    }
  }

  /* Threshold t = ((N - n) w.p + n w.r) / N, weighted by class counts; then fold the scaling into W, b. */
  for (k = 0U; k < K; k++)
  {
    float32_t *w = W + k * d;
    float32_t wlo = 0.0f;
    const float32_t t = ((N - n[k]) * b[k] + n[k] * sn[k]) / N;
    for (j = 0U; j < d; j++)
    {
      w[j] /= hi[j];
      wlo += w[j] * lo[j];
    }
    b[k] = -(t + wlo);
  }
  return ARM_MATH_SUCCESS;
}

/**
 * @} end of groupBayes group
 */
