/******************************************************************************
 * @file     bayes_functions.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.10.0
 * @date     08 July 2021
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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

 
#ifndef BAYES_FUNCTIONS_H_
#define BAYES_FUNCTIONS_H_

#include "arm_math_types.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"

#include "dsp/statistics_functions.h"

/**
 * @defgroup groupBayes Bayesian estimators
 *
 * Implement the naive gaussian Bayes estimator.
 * The training must be done from scikit-learn.
 *
 * The parameters can be easily
 * generated from the scikit-learn object. Some examples are given in
 * DSP/Testing/PatternGeneration/Bayes.py
 *
 * Also implement SEFR, a linear classifier that can be trained on the device.
 * Training does not iterate and has no gradients or learning rate.
 *
 * arm_sefr_train_f32 makes one pass over the labelled samples.
 * arm_sefr_lda_f32 is an optional second pass that fits linear discriminant
 * analysis on the class scores and writes the result into the same weights.
 * It is usually more accurate, and the model size and prediction cost stay
 * the same.
 *
 * The caller provides all the memory: the model buffers for arm_sefr_init_f32
 * and the temporary buffers for the training functions.
 *
 * The method comes from: H. Keshavarz, M. S. Abadeh, R. Rawassizadeh,
 * "SEFR: A Fast Linear-Time Classifier for Ultra-Low Power Devices",
 * https://arxiv.org/abs/2006.04620
 */

#ifdef   __cplusplus
extern "C"
{
#endif

/**
 * @brief Instance structure for Naive Gaussian Bayesian estimator.
 */
typedef struct
{
  uint32_t vectorDimension;  /**< Dimension of vector space */
  uint32_t numberOfClasses;  /**< Number of different classes  */
  const float32_t *theta;          /**< Mean values for the Gaussians */
  const float32_t *sigma;          /**< Variances for the Gaussians */
  const float32_t *classPriors;    /**< Class prior probabilities */
  float32_t epsilon;         /**< Additive value to variances */
} arm_gaussian_naive_bayes_instance_f32;

/**
 * @brief Naive Gaussian Bayesian Estimator
 *
 * @param[in]  S                        points to a naive bayes instance structure
 * @param[in]  in                       points to the elements of the input vector.
 * @param[out] *pOutputProbabilities    points to a buffer of length numberOfClasses containing estimated probabilities
 * @param[out] *pBufferB                points to a temporary buffer of length numberOfClasses
 * @return The predicted class
 */
uint32_t arm_gaussian_naive_bayes_predict_f32(const arm_gaussian_naive_bayes_instance_f32 *S, 
   const float32_t * in, 
   float32_t *pOutputProbabilities,
   float32_t *pBufferB);

/**
 * @brief Instance structure for the SEFR classifier.
 */
typedef struct
{
  uint32_t vectorDimension;  /**< Dimension of vector space */
  uint32_t numberOfClasses;  /**< Number of different classes */
  float32_t *pWeights;       /**< numberOfClasses * vectorDimension weights, row major */
  float32_t *pBias;          /**< numberOfClasses biases */
} arm_sefr_instance_f32;

/** Length in float32_t of the arm_sefr_train_f32 temporary buffer, usable for static arrays */
#define ARM_SEFR_TRAIN_TMP_BUFFER_SIZE(vectorDimension, numberOfClasses) \
  (2U * ((vectorDimension) + (numberOfClasses)))

/** Length in float32_t of the arm_sefr_lda_f32 temporary buffer, usable for static arrays */
#define ARM_SEFR_LDA_TMP_BUFFER_SIZE(numberOfClasses) \
  (5U * (numberOfClasses) * (numberOfClasses) + 2U * (numberOfClasses))

/**
 * @brief Length of the temporary buffer of arm_sefr_train_f32
 *
 * @param[in]  vectorDimension   dimension of the input vectors
 * @param[in]  numberOfClasses   number of classes
 * @return Length in float32_t
 */
__STATIC_INLINE uint32_t arm_sefr_train_tmp_buffer_size(uint32_t vectorDimension, uint32_t numberOfClasses)
{
  return ARM_SEFR_TRAIN_TMP_BUFFER_SIZE(vectorDimension, numberOfClasses);
}

/**
 * @brief Length of the temporary buffer of arm_sefr_lda_f32
 *
 * @param[in]  numberOfClasses   number of classes
 * @return Length in float32_t
 */
__STATIC_INLINE uint32_t arm_sefr_lda_tmp_buffer_size(uint32_t numberOfClasses)
{
  return ARM_SEFR_LDA_TMP_BUFFER_SIZE(numberOfClasses);
}

/**
 * @brief Initialization of a SEFR classifier instance
 *
 * @param[out]  S                  points to a SEFR instance structure
 * @param[in]   vectorDimension    dimension of the input vectors
 * @param[in]   numberOfClasses    number of classes
 * @param[in]   pWeights           points to a buffer of numberOfClasses * vectorDimension values
 * @param[in]   pBias              points to a buffer of numberOfClasses values
 */
void arm_sefr_init_f32(arm_sefr_instance_f32 *S,
   uint32_t vectorDimension,
   uint32_t numberOfClasses,
   float32_t *pWeights,
   float32_t *pBias);

/**
 * @brief SEFR training
 *
 * @param[in,out]  S            points to a SEFR instance structure
 * @param[in]      pSrc         points to numSamples * vectorDimension training values, row major
 * @param[in]      pLabels      points to numSamples class indices in [0, numberOfClasses)
 * @param[in]      numSamples   number of training samples
 * @param[out]     pTmp         points to a temporary buffer of arm_sefr_train_tmp_buffer_size() values
 * @return ARM_MATH_SUCCESS or ARM_MATH_ARGUMENT_ERROR
 */
arm_status arm_sefr_train_f32(const arm_sefr_instance_f32 *S,
   const float32_t *pSrc,
   const uint32_t *pLabels,
   uint32_t numSamples,
   float32_t *pTmp);

/**
 * @brief SEFR LDA stage
 *
 * @param[in,out]  S            points to a SEFR instance structure trained by arm_sefr_train_f32
 * @param[in]      pSrc         points to numSamples * vectorDimension training values, row major
 * @param[in]      pLabels      points to numSamples class indices in [0, numberOfClasses)
 * @param[in]      numSamples   number of training samples
 * @param[out]     pTmp         points to a temporary buffer of arm_sefr_lda_tmp_buffer_size() values
 * @return ARM_MATH_SUCCESS, ARM_MATH_ARGUMENT_ERROR or an error from a matrix function
 */
arm_status arm_sefr_lda_f32(const arm_sefr_instance_f32 *S,
   const float32_t *pSrc,
   const uint32_t *pLabels,
   uint32_t numSamples,
   float32_t *pTmp);

/**
 * @brief SEFR prediction
 *
 * @param[in]   S         points to a SEFR instance structure
 * @param[in]   pIn       points to the elements of the input vector
 * @param[out]  pScores   points to a buffer of numberOfClasses values receiving the class scores
 * @return The predicted class
 */
uint32_t arm_sefr_predict_f32(const arm_sefr_instance_f32 *S,
   const float32_t *pIn,
   float32_t *pScores);


#ifdef   __cplusplus
}
#endif

#endif /* ifndef _BAYES_FUNCTIONS_H_ */
