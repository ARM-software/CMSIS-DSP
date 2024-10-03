/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_neon_tables_f16.h
 * Description:  common tables like fft twiddle factors, Bitreverse, reciprocal etc
 *               used for NEON implementation only
 *
 * @date     02 October 2024
 *
 * Target Processor: Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2024 ARM Limited or its affiliates. All rights reserved.
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

 #ifndef ARM_NEON_TABLES_F16_H
 #define ARM_NEON_TABLES_F16_H

#include "arm_math_types_f16.h"

#ifdef   __cplusplus
extern "C"
{
#endif


 

#if defined(ARM_MATH_NEON_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)


extern const float16_t arm_neon_twiddles_16_f16[32];
extern const uint32_t arm_neon_factors_16_f16[4];
extern const float16_t arm_neon_twiddles_32_f16[64];
extern const uint32_t arm_neon_factors_32_f16[4];
extern const float16_t arm_neon_twiddles_64_f16[128];
extern const uint32_t arm_neon_factors_64_f16[4];
extern const float16_t arm_neon_twiddles_128_f16[256];
extern const uint32_t arm_neon_factors_128_f16[4];
extern const float16_t arm_neon_twiddles_256_f16[512];
extern const uint32_t arm_neon_factors_256_f16[4];
extern const float16_t arm_neon_twiddles_512_f16[1024];
extern const uint32_t arm_neon_factors_512_f16[4];
extern const float16_t arm_neon_twiddles_1024_f16[2048];
extern const uint32_t arm_neon_factors_1024_f16[4];
extern const float16_t arm_neon_twiddles_2048_f16[4096];
extern const uint32_t arm_neon_factors_2048_f16[4];
extern const float16_t arm_neon_twiddles_4096_f16[8192];
extern const uint32_t arm_neon_factors_4096_f16[4];


#endif /* defined(ARM_MATH_NEON_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE) */



#ifdef   __cplusplus
}
#endif

#endif /*ARM_NEON_TABLES_F16_H*/

