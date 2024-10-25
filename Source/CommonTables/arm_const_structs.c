/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_const_structs.c
 * Description:  Constant structs that are initialized for user convenience.
 *               For example, some can be given as arguments to the arm_cfft_f32() or arm_rfft_f32() functions.
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

#include "arm_math_types.h"
#include "arm_const_structs.h"

/*
ALLOW TABLE is true when config table is enabled and the Tramsform folder is included 
for compilation.
*/

/* Floating-point structs */
const arm_cfft_instance_f64 arm_cfft_sR_f64_len16 ARM_DSP_TABLE_ATTRIBUTE = {
  16, (const float64_t *)twiddleCoefF64_16, armBitRevIndexTableF64_16, ARMBITREVINDEXTABLEF64_16_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  32, (const float64_t *)twiddleCoefF64_32, armBitRevIndexTableF64_32, ARMBITREVINDEXTABLEF64_32_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len64 ARM_DSP_TABLE_ATTRIBUTE = {
  64, (const float64_t *)twiddleCoefF64_64, armBitRevIndexTableF64_64, ARMBITREVINDEXTABLEF64_64_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  128, (const float64_t *)twiddleCoefF64_128, armBitRevIndexTableF64_128, ARMBITREVINDEXTABLEF64_128_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  256, (const float64_t *)twiddleCoefF64_256, armBitRevIndexTableF64_256, ARMBITREVINDEXTABLEF64_256_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  512, (const float64_t *)twiddleCoefF64_512, armBitRevIndexTableF64_512, ARMBITREVINDEXTABLEF64_512_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  1024, (const float64_t *)twiddleCoefF64_1024, armBitRevIndexTableF64_1024, ARMBITREVINDEXTABLEF64_1024_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  2048, (const float64_t *)twiddleCoefF64_2048, armBitRevIndexTableF64_2048, ARMBITREVINDEXTABLEF64_2048_TABLE_LENGTH
};

const arm_cfft_instance_f64 arm_cfft_sR_f64_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  4096, (const float64_t *)twiddleCoefF64_4096, armBitRevIndexTableF64_4096, ARMBITREVINDEXTABLEF64_4096_TABLE_LENGTH
};

/* Floating-point structs */
#if (!defined(ARM_MATH_MVEF) && !defined(ARM_MATH_NEON)) || defined(ARM_MATH_AUTOVECTORIZE)


const arm_cfft_instance_f32 arm_cfft_sR_f32_len16 ARM_DSP_TABLE_ATTRIBUTE = {
  16, twiddleCoef_16, armBitRevIndexTable16, ARMBITREVINDEXTABLE_16_TABLE_LENGTH
};


const arm_cfft_instance_f32 arm_cfft_sR_f32_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  32, twiddleCoef_32, armBitRevIndexTable32, ARMBITREVINDEXTABLE_32_TABLE_LENGTH
};

const arm_cfft_instance_f32 arm_cfft_sR_f32_len64 ARM_DSP_TABLE_ATTRIBUTE = {
  64, twiddleCoef_64, armBitRevIndexTable64, ARMBITREVINDEXTABLE_64_TABLE_LENGTH
};

const arm_cfft_instance_f32 arm_cfft_sR_f32_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  128, twiddleCoef_128, armBitRevIndexTable128, ARMBITREVINDEXTABLE_128_TABLE_LENGTH
};

const arm_cfft_instance_f32 arm_cfft_sR_f32_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  256, twiddleCoef_256, armBitRevIndexTable256, ARMBITREVINDEXTABLE_256_TABLE_LENGTH
};

const arm_cfft_instance_f32 arm_cfft_sR_f32_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  512, twiddleCoef_512, armBitRevIndexTable512, ARMBITREVINDEXTABLE_512_TABLE_LENGTH
};

const arm_cfft_instance_f32 arm_cfft_sR_f32_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  1024, twiddleCoef_1024, armBitRevIndexTable1024, ARMBITREVINDEXTABLE_1024_TABLE_LENGTH
};

const arm_cfft_instance_f32 arm_cfft_sR_f32_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  2048, twiddleCoef_2048, armBitRevIndexTable2048, ARMBITREVINDEXTABLE_2048_TABLE_LENGTH
};

const arm_cfft_instance_f32 arm_cfft_sR_f32_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  4096, twiddleCoef_4096, armBitRevIndexTable4096, ARMBITREVINDEXTABLE_4096_TABLE_LENGTH
};

#endif /* !defined(ARM_MATH_MVEF) || defined(ARM_MATH_AUTOVECTORIZE) */

/* Fixed-point structs */

#if (!defined(ARM_MATH_MVEI) && !defined(ARM_MATH_NEON)) || defined(ARM_MATH_AUTOVECTORIZE)

/* 

Those structures cannot be used to initialize the MVE version of the FFT Q31 instances.
So they are not compiled when MVE is defined.

For the MVE version, the new arm_cfft_init_f32 must be used.


*/

const arm_cfft_instance_q31 arm_cfft_sR_q31_len16 ARM_DSP_TABLE_ATTRIBUTE = {
  16, twiddleCoef_16_q31, armBitRevIndexTable_fixed_16, ARMBITREVINDEXTABLE_FIXED_16_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  32, twiddleCoef_32_q31, armBitRevIndexTable_fixed_32, ARMBITREVINDEXTABLE_FIXED_32_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len64 ARM_DSP_TABLE_ATTRIBUTE = {
  64, twiddleCoef_64_q31, armBitRevIndexTable_fixed_64, ARMBITREVINDEXTABLE_FIXED_64_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  128, twiddleCoef_128_q31, armBitRevIndexTable_fixed_128, ARMBITREVINDEXTABLE_FIXED_128_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  256, twiddleCoef_256_q31, armBitRevIndexTable_fixed_256, ARMBITREVINDEXTABLE_FIXED_256_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  512, twiddleCoef_512_q31, armBitRevIndexTable_fixed_512, ARMBITREVINDEXTABLE_FIXED_512_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  1024, twiddleCoef_1024_q31, armBitRevIndexTable_fixed_1024, ARMBITREVINDEXTABLE_FIXED_1024_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  2048, twiddleCoef_2048_q31, armBitRevIndexTable_fixed_2048, ARMBITREVINDEXTABLE_FIXED_2048_TABLE_LENGTH
};

const arm_cfft_instance_q31 arm_cfft_sR_q31_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  4096, twiddleCoef_4096_q31, armBitRevIndexTable_fixed_4096, ARMBITREVINDEXTABLE_FIXED_4096_TABLE_LENGTH
};
#endif 

#if (!defined(ARM_MATH_MVEI) && !defined(ARM_MATH_NEON)) || defined(ARM_MATH_AUTOVECTORIZE)


const arm_cfft_instance_q15 arm_cfft_sR_q15_len16 ARM_DSP_TABLE_ATTRIBUTE = {
  16, twiddleCoef_16_q15, armBitRevIndexTable_fixed_16, ARMBITREVINDEXTABLE_FIXED_16_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  32, twiddleCoef_32_q15, armBitRevIndexTable_fixed_32, ARMBITREVINDEXTABLE_FIXED_32_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len64 ARM_DSP_TABLE_ATTRIBUTE = {
  64, twiddleCoef_64_q15, armBitRevIndexTable_fixed_64, ARMBITREVINDEXTABLE_FIXED_64_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  128, twiddleCoef_128_q15, armBitRevIndexTable_fixed_128, ARMBITREVINDEXTABLE_FIXED_128_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  256, twiddleCoef_256_q15, armBitRevIndexTable_fixed_256, ARMBITREVINDEXTABLE_FIXED_256_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  512, twiddleCoef_512_q15, armBitRevIndexTable_fixed_512, ARMBITREVINDEXTABLE_FIXED_512_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  1024, twiddleCoef_1024_q15, armBitRevIndexTable_fixed_1024, ARMBITREVINDEXTABLE_FIXED_1024_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  2048, twiddleCoef_2048_q15, armBitRevIndexTable_fixed_2048, ARMBITREVINDEXTABLE_FIXED_2048_TABLE_LENGTH
};

const arm_cfft_instance_q15 arm_cfft_sR_q15_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  4096, twiddleCoef_4096_q15, armBitRevIndexTable_fixed_4096, ARMBITREVINDEXTABLE_FIXED_4096_TABLE_LENGTH
};

#endif /* !defined(ARM_MATH_MVEI) */

/* Structure for real-value inputs */
/* Double precision strucs */

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  { 16, (const float64_t *)twiddleCoefF64_16, armBitRevIndexTableF64_16, ARMBITREVINDEXTABLEF64_16_TABLE_LENGTH },
  32U,
  (float64_t *)twiddleCoefF64_rfft_32
};

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len64 ARM_DSP_TABLE_ATTRIBUTE = {
   { 32, (const float64_t *)twiddleCoefF64_32, armBitRevIndexTableF64_32, ARMBITREVINDEXTABLEF64_32_TABLE_LENGTH },
  64U,
  (float64_t *)twiddleCoefF64_rfft_64
};

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  { 64, (const float64_t *)twiddleCoefF64_64, armBitRevIndexTableF64_64, ARMBITREVINDEXTABLEF64_64_TABLE_LENGTH },
  128U,
  (float64_t *)twiddleCoefF64_rfft_128
};

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  { 128, (const float64_t *)twiddleCoefF64_128, armBitRevIndexTableF64_128, ARMBITREVINDEXTABLEF64_128_TABLE_LENGTH },
  256U,
  (float64_t *)twiddleCoefF64_rfft_256
};

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  { 256, (const float64_t *)twiddleCoefF64_256, armBitRevIndexTableF64_256, ARMBITREVINDEXTABLEF64_256_TABLE_LENGTH },
  512U,
  (float64_t *)twiddleCoefF64_rfft_512
};

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  { 512, (const float64_t *)twiddleCoefF64_512, armBitRevIndexTableF64_512, ARMBITREVINDEXTABLEF64_512_TABLE_LENGTH },
  1024U,
  (float64_t *)twiddleCoefF64_rfft_1024
};

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  { 1024, (const float64_t *)twiddleCoefF64_1024, armBitRevIndexTableF64_1024, ARMBITREVINDEXTABLEF64_1024_TABLE_LENGTH },
  2048U,
  (float64_t *)twiddleCoefF64_rfft_2048
};

const arm_rfft_fast_instance_f64 arm_rfft_fast_sR_f64_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  { 2048, (const float64_t *)twiddleCoefF64_2048, armBitRevIndexTableF64_2048, ARMBITREVINDEXTABLEF64_2048_TABLE_LENGTH },
  4096U,
  (float64_t *)twiddleCoefF64_rfft_4096
};

/* Floating-point structs */

#if (!defined(ARM_MATH_MVEF) && !defined(ARM_MATH_NEON)) || defined(ARM_MATH_AUTOVECTORIZE)

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  { 16, twiddleCoef_16, armBitRevIndexTable16, ARMBITREVINDEXTABLE_16_TABLE_LENGTH },
  32U,
  (float32_t *)twiddleCoef_rfft_32
};

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len64 ARM_DSP_TABLE_ATTRIBUTE = {
   { 32, twiddleCoef_32, armBitRevIndexTable32, ARMBITREVINDEXTABLE_32_TABLE_LENGTH },
  64U,
  (float32_t *)twiddleCoef_rfft_64
};

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  { 64, twiddleCoef_64, armBitRevIndexTable64, ARMBITREVINDEXTABLE_64_TABLE_LENGTH },
  128U,
  (float32_t *)twiddleCoef_rfft_128
};

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  { 128, twiddleCoef_128, armBitRevIndexTable128, ARMBITREVINDEXTABLE_128_TABLE_LENGTH },
  256U,
  (float32_t *)twiddleCoef_rfft_256
};

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  { 256, twiddleCoef_256, armBitRevIndexTable256, ARMBITREVINDEXTABLE_256_TABLE_LENGTH },
  512U,
  (float32_t *)twiddleCoef_rfft_512
};

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  { 512, twiddleCoef_512, armBitRevIndexTable512, ARMBITREVINDEXTABLE_512_TABLE_LENGTH },
  1024U,
  (float32_t *)twiddleCoef_rfft_1024
};

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  { 1024, twiddleCoef_1024, armBitRevIndexTable1024, ARMBITREVINDEXTABLE_1024_TABLE_LENGTH },
  2048U,
  (float32_t *)twiddleCoef_rfft_2048
};

const arm_rfft_fast_instance_f32 arm_rfft_fast_sR_f32_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  { 2048, twiddleCoef_2048, armBitRevIndexTable2048, ARMBITREVINDEXTABLE_2048_TABLE_LENGTH },
  4096U,
  (float32_t *)twiddleCoef_rfft_4096
};

#endif /* #if !defined(ARM_MATH_MVEF) || defined(ARM_MATH_AUTOVECTORIZE) */

/* Fixed-point structs */
/* q31_t */

#if (!defined(ARM_MATH_MVEI) && !defined(ARM_MATH_NEON)) || defined(ARM_MATH_AUTOVECTORIZE)

/* 

Those structures cannot be used to initialize the MVE version of the FFT Q31 instances.
So they are not compiled when MVE is defined.

For the MVE version, the new arm_cfft_init_f32 must be used.


*/

const arm_rfft_instance_q31 arm_rfft_sR_q31_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  32U,
  0,
  1,
  256U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len16
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len64 ARM_DSP_TABLE_ATTRIBUTE = {
  64U,
  0,
  1,
  128U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len32
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  128U,
  0,
  1,
  64U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len64
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  256U,
  0,
  1,
  32U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len128
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  512U,
  0,
  1,
  16U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len256
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  1024U,
  0,
  1,
  8U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len512
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  2048U,
  0,
  1,
  4U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len1024
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  4096U,
  0,
  1,
  2U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len2048
};

const arm_rfft_instance_q31 arm_rfft_sR_q31_len8192 ARM_DSP_TABLE_ATTRIBUTE = {
  8192U,
  0,
  1,
  1U,
  (q31_t*)realCoefAQ31,
  (q31_t*)realCoefBQ31,
  &arm_cfft_sR_q31_len4096
};

#endif 

#if (!defined(ARM_MATH_MVEI) && !defined(ARM_MATH_NEON)) || defined(ARM_MATH_AUTOVECTORIZE)


/* q15_t */
const arm_rfft_instance_q15 arm_rfft_sR_q15_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  32U,
  0,
  1,
  256U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len16
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len64 ARM_DSP_TABLE_ATTRIBUTE = {
  64U,
  0,
  1,
  128U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len32
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  128U,
  0,
  1,
  64U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len64
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  256U,
  0,
  1,
  32U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len128
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  512U,
  0,
  1,
  16U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len256
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  1024U,
  0,
  1,
  8U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len512
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  2048U,
  0,
  1,
  4U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len1024
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  4096U,
  0,
  1,
  2U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len2048
};

const arm_rfft_instance_q15 arm_rfft_sR_q15_len8192 ARM_DSP_TABLE_ATTRIBUTE = {
  8192U,
  0,
  1,
  1U,
  (q15_t*)realCoefAQ15,
  (q15_t*)realCoefBQ15,
  &arm_cfft_sR_q15_len4096
};

#endif /* !defined(ARM_MATH_MVEI) */


