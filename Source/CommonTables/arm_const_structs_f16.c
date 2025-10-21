/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_const_structs_f16.c
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
#include "arm_compiler_specific.h"


#include "arm_math_types_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)

#include "arm_const_structs_f16.h"




/* Floating-point structs */
#if !defined(ARM_MATH_MVE_FLOAT16)  && !defined(ARM_MATH_NEON_FLOAT16)


/* 

Those structures cannot be used to initialize the MVE version of the FFT F32 instances.
So they are not compiled when MVE is defined.

For the MVE version, the new arm_cfft_init_f16 must be used.


*/

#if !defined(__CC_ARM)
 
const arm_cfft_instance_f16 arm_cfft_sR_f16_len16 ARM_DSP_TABLE_ATTRIBUTE = {
  16, twiddleCoefF16_16, armBitRevIndexTable_fixed_16, ARMBITREVINDEXTABLE_FIXED_16_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len32 ARM_DSP_TABLE_ATTRIBUTE = {
  32, twiddleCoefF16_32, armBitRevIndexTable_fixed_32, ARMBITREVINDEXTABLE_FIXED_32_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len64 ARM_DSP_TABLE_ATTRIBUTE = {
  64, twiddleCoefF16_64, armBitRevIndexTable_fixed_64, ARMBITREVINDEXTABLE_FIXED_64_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len128 ARM_DSP_TABLE_ATTRIBUTE = {
  128, twiddleCoefF16_128, armBitRevIndexTable_fixed_128, ARMBITREVINDEXTABLE_FIXED_128_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len256 ARM_DSP_TABLE_ATTRIBUTE = {
  256, twiddleCoefF16_256, armBitRevIndexTable_fixed_256, ARMBITREVINDEXTABLE_FIXED_256_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len512 ARM_DSP_TABLE_ATTRIBUTE = {
  512, twiddleCoefF16_512, armBitRevIndexTable_fixed_512, ARMBITREVINDEXTABLE_FIXED_512_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len1024 ARM_DSP_TABLE_ATTRIBUTE = {
  1024, twiddleCoefF16_1024, armBitRevIndexTable_fixed_1024, ARMBITREVINDEXTABLE_FIXED_1024_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len2048 ARM_DSP_TABLE_ATTRIBUTE = {
  2048, twiddleCoefF16_2048, armBitRevIndexTable_fixed_2048, ARMBITREVINDEXTABLE_FIXED_2048_TABLE_LENGTH
};

const arm_cfft_instance_f16 arm_cfft_sR_f16_len4096 ARM_DSP_TABLE_ATTRIBUTE = {
  4096, twiddleCoefF16_4096, armBitRevIndexTable_fixed_4096, ARMBITREVINDEXTABLE_FIXED_4096_TABLE_LENGTH
};



#endif

#endif

#endif
