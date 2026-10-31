/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_sefr_predict_f32
 * Description:  Prediction with a SEFR classifier
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

/**
 * @addtogroup groupBayes
 * @{
 */

/**
 * @brief SEFR prediction
 *
 * @param[in]   S         points to a SEFR instance structure
 * @param[in]   pIn       points to the elements of the input vector
 * @param[out]  pScores   points to a buffer of numberOfClasses values receiving the class scores
 * @return The predicted class
 */
ARM_DSP_ATTRIBUTE uint32_t arm_sefr_predict_f32(const arm_sefr_instance_f32 *S,
   const float32_t *pIn,
   float32_t *pScores)
{
  arm_matrix_instance_f32 mW;
  float32_t best;
  uint32_t index;

  arm_mat_init_f32(&mW, (uint16_t)S->numberOfClasses, (uint16_t)S->vectorDimension, S->pWeights);
  arm_mat_vec_mult_f32(&mW, pIn, pScores);
  arm_add_f32(pScores, S->pBias, pScores, S->numberOfClasses);
  arm_max_f32(pScores, S->numberOfClasses, &best, &index);
  return index;
}

/**
 * @} end of groupBayes group
 */
