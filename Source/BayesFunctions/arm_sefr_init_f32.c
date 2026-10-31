/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_sefr_init_f32
 * Description:  Initialization of a SEFR classifier instance
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

/**
 * @addtogroup groupBayes
 * @{
 */

/**
 * @brief Initialization of a SEFR classifier instance
 *
 * @param[out]  S                  points to a SEFR instance structure
 * @param[in]   vectorDimension    dimension of the input vectors
 * @param[in]   numberOfClasses    number of classes
 * @param[in]   pWeights           points to a buffer of numberOfClasses * vectorDimension values
 * @param[in]   pBias              points to a buffer of numberOfClasses values
 */
ARM_DSP_ATTRIBUTE void arm_sefr_init_f32(arm_sefr_instance_f32 *S,
   uint32_t vectorDimension,
   uint32_t numberOfClasses,
   float32_t *pWeights,
   float32_t *pBias)
{
  S->vectorDimension = vectorDimension;
  S->numberOfClasses = numberOfClasses;
  S->pWeights = pWeights;
  S->pBias = pBias;
}

/**
 * @} end of groupBayes group
 */
