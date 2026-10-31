/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_sefr_lda_f32
 * Description:  Optional LDA stage on the class scores of a SEFR classifier
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
#include "dsp/matrix_functions.h"
#include "dsp/support_functions.h"
#include <math.h>

/* Maximum number of Cholesky attempts, each with a larger ridge than the last */
#define SEFR_LDA_MAX_RIDGE_TRIALS 6U

/**
 * @addtogroup groupBayes
 * @{
 */

/**
 * @brief SEFR LDA stage
 *
 * @param[in,out]  S            points to a SEFR instance structure trained by arm_sefr_train_f32
 * @param[in]      pSrc         points to numSamples * vectorDimension training values, row major
 * @param[in]      pLabels      points to numSamples class indices in [0, numberOfClasses)
 * @param[in]      numSamples   number of training samples
 * @param[out]     pTmp         points to a temporary buffer of arm_sefr_lda_tmp_buffer_size() values
 * @return ARM_MATH_SUCCESS, ARM_MATH_ARGUMENT_ERROR if a label is out of range or a class has no
 *         samples, or the error from the matrix function that failed. An error leaves the model
 *         unchanged.
 *
 * @par  Algorithm
 *       One more pass over the data fits linear discriminant analysis on the numberOfClasses scores
 *       of the trained model. It uses a Gaussian for each class with a shared covariance, and takes
 *       the class priors from the data. The result goes into the same weights and bias, so the model
 *       size and the cost of arm_sefr_predict_f32 stay the same.
 *
 * @par  Ridge
 *       The ridge added to the covariance starts at 1e-6 of the mean score variance. It grows by a
 *       factor of 10 each time the Cholesky decomposition fails, which can happen in float32 when
 *       the scores are nearly collinear.
 */
ARM_DSP_ATTRIBUTE arm_status arm_sefr_lda_f32(const arm_sefr_instance_f32 *S,
   const float32_t *pSrc,
   const uint32_t *pLabels,
   uint32_t numSamples,
   float32_t *pTmp)
{
  const uint32_t d = S->vectorDimension, K = S->numberOfClasses;
  float32_t *M = pTmp;                /* K x K class means of the scores */
  float32_t *C = M + K * K;           /* K x K within-class scatter, later the solution A^T */
  float32_t *L = C + K * K;           /* Cholesky factor */
  float32_t *Lt = L + K * K;          /* its transpose */
  float32_t *Y = Lt + K * K;          /* intermediate solve */
  float32_t *n = Y + K * K;           /* class counts */
  float32_t *z = n + K;               /* scores of one sample, then one weight column */
  arm_matrix_instance_f32 mM, mC, mL, mLt, mY;
  const float32_t N = (float32_t)numSamples;
  float32_t trace = 0.0f, ridge, added = 0.0f;
  uint32_t i, j, k, l;
  arm_status status = ARM_MATH_SUCCESS;

  /* One pass (Welford): running class means of the scores and the pooled within-class scatter. */
  arm_fill_f32(0.0f, M, 2U * K * K);
  arm_fill_f32(0.0f, n, K);
  for (i = 0U; i < numSamples; i++)
  {
    float32_t *m;
    k = pLabels[i];
    if (k >= K)
    {
      return ARM_MATH_ARGUMENT_ERROR;
    }
    m = M + k * K;
    (void)arm_sefr_predict_f32(S, pSrc + i * d, z);
    n[k] += 1.0f;
    /* L, Lt and Y are free until the solve, so they hold per-sample temporaries here. */
    arm_sub_f32(z, m, L, K);                   /* z - old mean */
    arm_scale_f32(L, 1.0f / n[k], Lt, K);
    arm_add_f32(m, Lt, m, K);                  /* new mean */
    arm_sub_f32(z, m, Lt, K);                  /* z - new mean */
    for (l = 0U; l < K; l++)
    {
      arm_scale_f32(Lt, L[l], Y, K);
      arm_add_f32(C + l * K, Y, C + l * K, K);
    }
  }
  for (k = 0U; k < K; k++)
  {
    if (n[k] < 1.0f)
    {
      return ARM_MATH_ARGUMENT_ERROR;
    }
  }
  arm_scale_f32(C, 1.0f / N, C, K * K);
  for (k = 0U; k < K; k++)
  {
    trace += C[k * K + k];
  }

  /* Solve C A^T = M^T with C = L L^T. */
  arm_mat_init_f32(&mC, (uint16_t)K, (uint16_t)K, C);
  arm_mat_init_f32(&mL, (uint16_t)K, (uint16_t)K, L);
  arm_mat_init_f32(&mLt, (uint16_t)K, (uint16_t)K, Lt);
  arm_mat_init_f32(&mY, (uint16_t)K, (uint16_t)K, Y);
  arm_mat_init_f32(&mM, (uint16_t)K, (uint16_t)K, M);
  ridge = 1e-6f * trace / (float32_t)K + 1e-30f;
  for (l = 0U; l < SEFR_LDA_MAX_RIDGE_TRIALS; l++)
  {
    for (k = 0U; k < K; k++)
    {
      C[k * K + k] += ridge - added;
    }
    added = ridge;
    ridge *= 10.0f;
    status = arm_mat_cholesky_f32(&mC, &mL);
    if (status == ARM_MATH_SUCCESS)
    {
      break;
    }
  }
  if (status != ARM_MATH_SUCCESS)
  {
    return status;
  }
  (void)arm_mat_trans_f32(&mL, &mLt);
  (void)arm_mat_trans_f32(&mM, &mC);          /* C now holds M^T */
  status = arm_mat_solve_lower_triangular_f32(&mL, &mC, &mY);
  if (status != ARM_MATH_SUCCESS)
  {
    return status;
  }
  status = arm_mat_solve_upper_triangular_f32(&mLt, &mY, &mC);  /* C = A^T */
  if (status != ARM_MATH_SUCCESS)
  {
    return status;
  }

  /* New bias: log prior + A b - 0.5 a_k . m_k, using the old b before it is overwritten. */
  for (k = 0U; k < K; k++)
  {
    float32_t ab = 0.0f, am = 0.0f;
    for (l = 0U; l < K; l++)
    {
      ab += C[l * K + k] * S->pBias[l];
      am += C[l * K + k] * M[k * K + l];
    }
    z[k] = logf(n[k] / N) + ab - 0.5f * am;
  }
  arm_copy_f32(z, S->pBias, K);

  /* W <- A W, one weight column at a time, so no K x d temporary is needed. */
  for (j = 0U; j < d; j++)
  {
    for (k = 0U; k < K; k++)
    {
      float32_t s = 0.0f;
      for (l = 0U; l < K; l++)
      {
        s += C[l * K + k] * S->pWeights[l * d + j];
      }
      z[k] = s;
    }
    for (k = 0U; k < K; k++)
    {
      S->pWeights[k * d + j] = z[k];
    }
  }
  return ARM_MATH_SUCCESS;
}

/**
 * @} end of groupBayes group
 */
