/******************************************************************************
 * @file     window_functions.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  v1.15.0
 * @date     15 December 2022
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

 
#ifndef _WINDOW_FUNCTIONS_H_
#define _WINDOW_FUNCTIONS_H_

#include "arm_math_types.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"


#ifdef   __cplusplus
extern "C"
{
#endif

/**
 * @defgroup groupWindow Window Functions
 */

 /**
   * @brief Welch window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           21.3 dB  |
   * | Normalized equivalent noise bandwidth |          1.2 bins  |
   * | Flatness                              |        -2.2248 dB  |
   * | Recommended overlap                   |            29.3 %  |
   *
   */
  void arm_welch_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Welch window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           21.3 dB  |
   * | Normalized equivalent noise bandwidth |          1.2 bins  |
   * | Flatness                              |        -2.2248 dB  |
   * | Recommended overlap                   |            29.3 %  |
   *
   *
   */
  void arm_welch_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Bartlett window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           26.5 dB  |
   * | Normalized equivalent noise bandwidth |       1.3333 bins  |
   * | Flatness                              |        -1.8242 dB  |
   * | Recommended overlap                   |            50.0 %  |
   *
   */
  void arm_bartlett_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Bartlett window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           26.5 dB  |
   * | Normalized equivalent noise bandwidth |       1.3333 bins  |
   * | Flatness                              |        -1.8242 dB  |
   * | Recommended overlap                   |            50.0 %  |
   *
   *
   */
  void arm_bartlett_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hamming window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           42.7 dB  |
   * | Normalized equivalent noise bandwidth |       1.3628 bins  |
   * | Flatness                              |        -1.7514 dB  |
   * | Recommended overlap                   |              50 %  |
   *
   */
  void arm_hamming_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hamming window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           42.7 dB  |
   * | Normalized equivalent noise bandwidth |       1.3628 bins  |
   * | Flatness                              |        -1.7514 dB  |
   * | Recommended overlap                   |              50 %  |
   *
   *
   */
  void arm_hamming_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hanning window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           31.5 dB  |
   * | Normalized equivalent noise bandwidth |          1.5 bins  |
   * | Flatness                              |        -1.4236 dB  |
   * | Recommended overlap                   |              50 %  |
   *
   */
  void arm_hanning_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hanning window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           31.5 dB  |
   * | Normalized equivalent noise bandwidth |          1.5 bins  |
   * | Flatness                              |        -1.4236 dB  |
   * | Recommended overlap                   |              50 %  |
   *
   *
   */
  void arm_hanning_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Nuttall3 window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           46.7 dB  |
   * | Normalized equivalent noise bandwidth |       1.9444 bins  |
   * | Flatness                              |         -0.863 dB  |
   * | Recommended overlap                   |            64.7 %  |
   *
   */
  void arm_nuttall3_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Nuttall3 window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           46.7 dB  |
   * | Normalized equivalent noise bandwidth |       1.9444 bins  |
   * | Flatness                              |         -0.863 dB  |
   * | Recommended overlap                   |            64.7 %  |
   *
   *
   */
  void arm_nuttall3_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Nuttall4 window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           60.9 dB  |
   * | Normalized equivalent noise bandwidth |         2.31 bins  |
   * | Flatness                              |        -0.6184 dB  |
   * | Recommended overlap                   |            70.5 %  |
   *
   */
  void arm_nuttall4_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Nuttall4 window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           60.9 dB  |
   * | Normalized equivalent noise bandwidth |         2.31 bins  |
   * | Flatness                              |        -0.6184 dB  |
   * | Recommended overlap                   |            70.5 %  |
   *
   *
   */
  void arm_nuttall4_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Nuttall3a window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           64.2 dB  |
   * | Normalized equivalent noise bandwidth |       1.7721 bins  |
   * | Flatness                              |        -1.0453 dB  |
   * | Recommended overlap                   |            61.2 %  |
   *
   */
  void arm_nuttall3a_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Nuttall3a window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           64.2 dB  |
   * | Normalized equivalent noise bandwidth |       1.7721 bins  |
   * | Flatness                              |        -1.0453 dB  |
   * | Recommended overlap                   |            61.2 %  |
   *
   *
   */
  void arm_nuttall3a_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Nuttall3b window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           71.5 dB  |
   * | Normalized equivalent noise bandwidth |       1.7037 bins  |
   * | Flatness                              |        -1.1352 dB  |
   * | Recommended overlap                   |            59.8 %  |
   *
   */
  void arm_nuttall3b_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Nuttall3b window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           71.5 dB  |
   * | Normalized equivalent noise bandwidth |       1.7037 bins  |
   * | Flatness                              |        -1.1352 dB  |
   * | Recommended overlap                   |            59.8 %  |
   *
   *
   */
  void arm_nuttall3b_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Nuttall4a window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           82.6 dB  |
   * | Normalized equivalent noise bandwidth |       2.1253 bins  |
   * | Flatness                              |        -0.7321 dB  |
   * | Recommended overlap                   |            68.0 %  |
   *
   */
  void arm_nuttall4a_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Nuttall4a window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           82.6 dB  |
   * | Normalized equivalent noise bandwidth |       2.1253 bins  |
   * | Flatness                              |        -0.7321 dB  |
   * | Recommended overlap                   |            68.0 %  |
   *
   *
   */
  void arm_nuttall4a_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief 92 db blackman harris window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           92.0 dB  |
   * | Normalized equivalent noise bandwidth |       2.0044 bins  |
   * | Flatness                              |        -0.8256 dB  |
   * | Recommended overlap                   |            66.1 %  |
   *
   */
  void arm_blackman_harris_92db_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief 92 db blackman harris window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           92.0 dB  |
   * | Normalized equivalent noise bandwidth |       2.0044 bins  |
   * | Flatness                              |        -0.8256 dB  |
   * | Recommended overlap                   |            66.1 %  |
   *
   *
   */
  void arm_blackman_harris_92db_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Nuttall4b window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           93.3 dB  |
   * | Normalized equivalent noise bandwidth |       2.0212 bins  |
   * | Flatness                              |        -0.8118 dB  |
   * | Recommended overlap                   |            66.3 %  |
   *
   */
  void arm_nuttall4b_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Nuttall4b window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           93.3 dB  |
   * | Normalized equivalent noise bandwidth |       2.0212 bins  |
   * | Flatness                              |        -0.8118 dB  |
   * | Recommended overlap                   |            66.3 %  |
   *
   *
   */
  void arm_nuttall4b_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Nuttall4c window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           98.1 dB  |
   * | Normalized equivalent noise bandwidth |       1.9761 bins  |
   * | Flatness                              |        -0.8506 dB  |
   * | Recommended overlap                   |            65.6 %  |
   *
   */
  void arm_nuttall4c_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Nuttall4c window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           98.1 dB  |
   * | Normalized equivalent noise bandwidth |       1.9761 bins  |
   * | Flatness                              |        -0.8506 dB  |
   * | Recommended overlap                   |            65.6 %  |
   *
   *
   */
  void arm_nuttall4c_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft90d window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           90.2 dB  |
   * | Normalized equivalent noise bandwidth |       3.8832 bins  |
   * | Flatness                              |        -0.0039 dB  |
   * | Recommended overlap                   |            76.0 %  |
   *
   */
  void arm_hft90d_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft90d window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           90.2 dB  |
   * | Normalized equivalent noise bandwidth |       3.8832 bins  |
   * | Flatness                              |        -0.0039 dB  |
   * | Recommended overlap                   |            76.0 %  |
   *
   *
   */
  void arm_hft90d_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft95 window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           95.0 dB  |
   * | Normalized equivalent noise bandwidth |       3.8112 bins  |
   * | Flatness                              |         0.0044 dB  |
   * | Recommended overlap                   |            75.6 %  |
   *
   */
  void arm_hft95_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft95 window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |           95.0 dB  |
   * | Normalized equivalent noise bandwidth |       3.8112 bins  |
   * | Flatness                              |         0.0044 dB  |
   * | Recommended overlap                   |            75.6 %  |
   *
   *
   */
  void arm_hft95_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft116d window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          116.8 dB  |
   * | Normalized equivalent noise bandwidth |       4.2186 bins  |
   * | Flatness                              |        -0.0028 dB  |
   * | Recommended overlap                   |            78.2 %  |
   *
   */
  void arm_hft116d_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft116d window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          116.8 dB  |
   * | Normalized equivalent noise bandwidth |       4.2186 bins  |
   * | Flatness                              |        -0.0028 dB  |
   * | Recommended overlap                   |            78.2 %  |
   *
   *
   */
  void arm_hft116d_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft144d window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          144.1 dB  |
   * | Normalized equivalent noise bandwidth |       4.5386 bins  |
   * | Flatness                              |         0.0021 dB  |
   * | Recommended overlap                   |            79.9 %  |
   *
   */
  void arm_hft144d_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft144d window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          144.1 dB  |
   * | Normalized equivalent noise bandwidth |       4.5386 bins  |
   * | Flatness                              |         0.0021 dB  |
   * | Recommended overlap                   |            79.9 %  |
   *
   *
   */
  void arm_hft144d_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft169d window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          169.5 dB  |
   * | Normalized equivalent noise bandwidth |       4.8347 bins  |
   * | Flatness                              |         0.0017 dB  |
   * | Recommended overlap                   |            81.2 %  |
   *
   */
  void arm_hft169d_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft169d window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          169.5 dB  |
   * | Normalized equivalent noise bandwidth |       4.8347 bins  |
   * | Flatness                              |         0.0017 dB  |
   * | Recommended overlap                   |            81.2 %  |
   *
   *
   */
  void arm_hft169d_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft196d window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          196.2 dB  |
   * | Normalized equivalent noise bandwidth |       5.1134 bins  |
   * | Flatness                              |         0.0013 dB  |
   * | Recommended overlap                   |            82.3 %  |
   *
   */
  void arm_hft196d_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft196d window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          196.2 dB  |
   * | Normalized equivalent noise bandwidth |       5.1134 bins  |
   * | Flatness                              |         0.0013 dB  |
   * | Recommended overlap                   |            82.3 %  |
   *
   *
   */
  void arm_hft196d_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft223d window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          223.0 dB  |
   * | Normalized equivalent noise bandwidth |       5.3888 bins  |
   * | Flatness                              |         0.0011 dB  |
   * | Recommended overlap                   |            83.3 %  |
   *
   */
  void arm_hft223d_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft223d window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          223.0 dB  |
   * | Normalized equivalent noise bandwidth |       5.3888 bins  |
   * | Flatness                              |         0.0011 dB  |
   * | Recommended overlap                   |            83.3 %  |
   *
   *
   */
  void arm_hft223d_f32(
        float32_t * pDst,
        uint32_t blockSize);
 /**
   * @brief Hft248d window (double).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          248.4 dB  |
   * | Normalized equivalent noise bandwidth |       5.6512 bins  |
   * | Flatness                              |         0.0009 dB  |
   * | Recommended overlap                   |            84.1 %  |
   *
   */
  void arm_hft248d_f64(
        float64_t * pDst,
        uint32_t blockSize);

 /**
   * @brief Hft248d window (float).
   * @param[out] pDst       points to the output generated window
   * @param[in]  blockSize  number of samples in the window
   *
   * @par Parameters of the window
   * 
   * | Parameter                             | Value              |
   * | ------------------------------------: | -----------------: |
   * | Peak sidelobe level                   |          248.4 dB  |
   * | Normalized equivalent noise bandwidth |       5.6512 bins  |
   * | Flatness                              |         0.0009 dB  |
   * | Recommended overlap                   |            84.1 %  |
   *
   *
   */
  void arm_hft248d_f32(
        float32_t * pDst,
        uint32_t blockSize);


#ifdef   __cplusplus
}
#endif

#endif /* ifndef _BASIC_MATH_FUNCTIONS_H_ */
