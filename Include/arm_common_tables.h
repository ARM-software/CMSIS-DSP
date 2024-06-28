/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_common_tables.h
 * Description:  Extern declaration for common tables
 *
 * @version  V1.10.0
 * @date     08 July 2021
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

#ifndef ARM_COMMON_TABLES_H
#define ARM_COMMON_TABLES_H

#include "arm_math_types.h"
#include "dsp/fast_math_functions.h"

#ifdef   __cplusplus
extern "C"
{
#endif

  /* Double Precision Float CFFT twiddles */
    extern const uint16_t armBitRevTable[1024];

    extern const uint64_t twiddleCoefF64_16[32];

    extern const uint64_t twiddleCoefF64_32[64];

    extern const uint64_t twiddleCoefF64_64[128];

    extern const uint64_t twiddleCoefF64_128[256];

    extern const uint64_t twiddleCoefF64_256[512];

    extern const uint64_t twiddleCoefF64_512[1024];

    extern const uint64_t twiddleCoefF64_1024[2048];

    extern const uint64_t twiddleCoefF64_2048[4096];

    extern const uint64_t twiddleCoefF64_4096[8192];

    extern const float32_t twiddleCoef_16[32];

    extern const float32_t twiddleCoef_32[64];

    extern const float32_t twiddleCoef_64[128];

    extern const float32_t twiddleCoef_128[256];

    extern const float32_t twiddleCoef_256[512];

    extern const float32_t twiddleCoef_512[1024];

    extern const float32_t twiddleCoef_1024[2048];

    extern const float32_t twiddleCoef_2048[4096];

    extern const float32_t twiddleCoef_4096[8192];
    #define twiddleCoef twiddleCoef_4096

  /* Q31 */

    extern const q31_t twiddleCoef_16_q31[24];

    extern const q31_t twiddleCoef_32_q31[48];

    extern const q31_t twiddleCoef_64_q31[96];

    extern const q31_t twiddleCoef_128_q31[192];

    extern const q31_t twiddleCoef_256_q31[384];

    extern const q31_t twiddleCoef_512_q31[768];

    extern const q31_t twiddleCoef_1024_q31[1536];

    extern const q31_t twiddleCoef_2048_q31[3072];

    extern const q31_t twiddleCoef_4096_q31[6144];

    extern const q15_t twiddleCoef_16_q15[24];

    extern const q15_t twiddleCoef_32_q15[48];

    extern const q15_t twiddleCoef_64_q15[96];

    extern const q15_t twiddleCoef_128_q15[192];

    extern const q15_t twiddleCoef_256_q15[384];

    extern const q15_t twiddleCoef_512_q15[768];

    extern const q15_t twiddleCoef_1024_q15[1536];

    extern const q15_t twiddleCoef_2048_q15[3072];

    extern const q15_t twiddleCoef_4096_q15[6144];

  /* Double Precision Float RFFT twiddles */
    extern const uint64_t twiddleCoefF64_rfft_32[32];

    extern const uint64_t twiddleCoefF64_rfft_64[64];

    extern const uint64_t twiddleCoefF64_rfft_128[128];

    extern const uint64_t twiddleCoefF64_rfft_256[256];

    extern const uint64_t twiddleCoefF64_rfft_512[512];

    extern const uint64_t twiddleCoefF64_rfft_1024[1024];

    extern const uint64_t twiddleCoefF64_rfft_2048[2048];

    extern const uint64_t twiddleCoefF64_rfft_4096[4096];

    extern const float32_t twiddleCoef_rfft_32[32];

    extern const float32_t twiddleCoef_rfft_64[64];

    extern const float32_t twiddleCoef_rfft_128[128];

    extern const float32_t twiddleCoef_rfft_256[256];

    extern const float32_t twiddleCoef_rfft_512[512];

    extern const float32_t twiddleCoef_rfft_1024[1024];

    extern const float32_t twiddleCoef_rfft_2048[2048];

    extern const float32_t twiddleCoef_rfft_4096[4096];

  /* Double precision floating-point bit reversal tables */

    #define ARMBITREVINDEXTABLEF64_16_TABLE_LENGTH ((uint16_t)12)
    extern const uint16_t armBitRevIndexTableF64_16[ARMBITREVINDEXTABLEF64_16_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_32_TABLE_LENGTH ((uint16_t)24)
    extern const uint16_t armBitRevIndexTableF64_32[ARMBITREVINDEXTABLEF64_32_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_64_TABLE_LENGTH ((uint16_t)56)
    extern const uint16_t armBitRevIndexTableF64_64[ARMBITREVINDEXTABLEF64_64_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_128_TABLE_LENGTH ((uint16_t)112)
    extern const uint16_t armBitRevIndexTableF64_128[ARMBITREVINDEXTABLEF64_128_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_256_TABLE_LENGTH ((uint16_t)240)
    extern const uint16_t armBitRevIndexTableF64_256[ARMBITREVINDEXTABLEF64_256_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_512_TABLE_LENGTH ((uint16_t)480)
    extern const uint16_t armBitRevIndexTableF64_512[ARMBITREVINDEXTABLEF64_512_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_1024_TABLE_LENGTH ((uint16_t)992)
    extern const uint16_t armBitRevIndexTableF64_1024[ARMBITREVINDEXTABLEF64_1024_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_2048_TABLE_LENGTH ((uint16_t)1984)
    extern const uint16_t armBitRevIndexTableF64_2048[ARMBITREVINDEXTABLEF64_2048_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLEF64_4096_TABLE_LENGTH ((uint16_t)4032)
    extern const uint16_t armBitRevIndexTableF64_4096[ARMBITREVINDEXTABLEF64_4096_TABLE_LENGTH];
  /* floating-point bit reversal tables */

    #define ARMBITREVINDEXTABLE_16_TABLE_LENGTH ((uint16_t)20)
    extern const uint16_t armBitRevIndexTable16[ARMBITREVINDEXTABLE_16_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_32_TABLE_LENGTH ((uint16_t)48)
    extern const uint16_t armBitRevIndexTable32[ARMBITREVINDEXTABLE_32_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_64_TABLE_LENGTH ((uint16_t)56)
    extern const uint16_t armBitRevIndexTable64[ARMBITREVINDEXTABLE_64_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_128_TABLE_LENGTH ((uint16_t)208)
    extern const uint16_t armBitRevIndexTable128[ARMBITREVINDEXTABLE_128_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_256_TABLE_LENGTH ((uint16_t)440)
    extern const uint16_t armBitRevIndexTable256[ARMBITREVINDEXTABLE_256_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_512_TABLE_LENGTH ((uint16_t)448)
    extern const uint16_t armBitRevIndexTable512[ARMBITREVINDEXTABLE_512_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_1024_TABLE_LENGTH ((uint16_t)1800)
    extern const uint16_t armBitRevIndexTable1024[ARMBITREVINDEXTABLE_1024_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_2048_TABLE_LENGTH ((uint16_t)3808)
    extern const uint16_t armBitRevIndexTable2048[ARMBITREVINDEXTABLE_2048_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_4096_TABLE_LENGTH ((uint16_t)4032)
    extern const uint16_t armBitRevIndexTable4096[ARMBITREVINDEXTABLE_4096_TABLE_LENGTH];


  /* fixed-point bit reversal tables */

    #define ARMBITREVINDEXTABLE_FIXED_16_TABLE_LENGTH ((uint16_t)12)
    extern const uint16_t armBitRevIndexTable_fixed_16[ARMBITREVINDEXTABLE_FIXED_16_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_32_TABLE_LENGTH ((uint16_t)24)
    extern const uint16_t armBitRevIndexTable_fixed_32[ARMBITREVINDEXTABLE_FIXED_32_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_64_TABLE_LENGTH ((uint16_t)56)
    extern const uint16_t armBitRevIndexTable_fixed_64[ARMBITREVINDEXTABLE_FIXED_64_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_128_TABLE_LENGTH ((uint16_t)112)
    extern const uint16_t armBitRevIndexTable_fixed_128[ARMBITREVINDEXTABLE_FIXED_128_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_256_TABLE_LENGTH ((uint16_t)240)
    extern const uint16_t armBitRevIndexTable_fixed_256[ARMBITREVINDEXTABLE_FIXED_256_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_512_TABLE_LENGTH ((uint16_t)480)
    extern const uint16_t armBitRevIndexTable_fixed_512[ARMBITREVINDEXTABLE_FIXED_512_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_1024_TABLE_LENGTH ((uint16_t)992)
    extern const uint16_t armBitRevIndexTable_fixed_1024[ARMBITREVINDEXTABLE_FIXED_1024_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_2048_TABLE_LENGTH ((uint16_t)1984)
    extern const uint16_t armBitRevIndexTable_fixed_2048[ARMBITREVINDEXTABLE_FIXED_2048_TABLE_LENGTH];

    #define ARMBITREVINDEXTABLE_FIXED_4096_TABLE_LENGTH ((uint16_t)4032)
    extern const uint16_t armBitRevIndexTable_fixed_4096[ARMBITREVINDEXTABLE_FIXED_4096_TABLE_LENGTH];

    extern const float32_t realCoefA[8192];
    extern const float32_t realCoefB[8192];

    extern const q31_t realCoefAQ31[8192];
    extern const q31_t realCoefBQ31[8192];

    extern const q15_t realCoefAQ15[8192];
    extern const q15_t realCoefBQ15[8192];

    extern const float32_t Weights_128[256];
    extern const float32_t cos_factors_128[128];

    extern const float32_t Weights_512[1024];
    extern const float32_t cos_factors_512[512];

    extern const float32_t Weights_2048[4096];
    extern const float32_t cos_factors_2048[2048];

    extern const float32_t Weights_8192[16384];
    extern const float32_t cos_factors_8192[8192];

    extern const q15_t WeightsQ15_128[256];
    extern const q15_t cos_factorsQ15_128[128];

    extern const q15_t WeightsQ15_512[1024];
    extern const q15_t cos_factorsQ15_512[512];

    extern const q15_t WeightsQ15_2048[4096];
    extern const q15_t cos_factorsQ15_2048[2048];

    extern const q15_t WeightsQ15_8192[16384];
    extern const q15_t cos_factorsQ15_8192[8192];

    extern const q31_t WeightsQ31_128[256];
    extern const q31_t cos_factorsQ31_128[128];

    extern const q31_t WeightsQ31_512[1024];
    extern const q31_t cos_factorsQ31_512[512];

    extern const q31_t WeightsQ31_2048[4096];
    extern const q31_t cos_factorsQ31_2048[2048];

    extern const q31_t WeightsQ31_8192[16384];
    extern const q31_t cos_factorsQ31_8192[8192];


    extern const q15_t armRecipTableQ15[64];

    extern const q31_t armRecipTableQ31[64];

  /* Tables for Fast Math Sine and Cosine */
    extern const float32_t sinTable_f32[FAST_MATH_TABLE_SIZE + 1];

    extern const q31_t sinTable_q31[FAST_MATH_TABLE_SIZE + 1];

    extern const q15_t sinTable_q15[FAST_MATH_TABLE_SIZE + 1];


  /* Accurate scalar sqrt */
       extern const q31_t sqrt_initial_lut_q31[32];

       extern const q15_t sqrt_initial_lut_q15[16];

#if (defined(ARM_MATH_MVEI) || defined(ARM_MATH_HELIUM)) && !defined(ARM_MATH_AUTOVECTORIZE)
       extern const q15_t sqrtTable_Q15[256];
       extern const q31_t sqrtTable_Q31[256];
       extern const unsigned char hwLUT[256];
#endif 

#if (defined(ARM_MATH_MVEF) || defined(ARM_MATH_HELIUM)) && !defined(ARM_MATH_AUTOVECTORIZE)
       extern const float32_t exp_tab[8];
       extern const float32_t __logf_lut_f32[8];
#endif 


#ifdef   __cplusplus
}
#endif

#endif /*  ARM_COMMON_TABLES_H */

