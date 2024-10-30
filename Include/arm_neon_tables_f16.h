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


#define ARM_NEON_TWIDDLES_16_F16_LEN 24
extern const float16_t arm_neon_twiddles_16_f16[ARM_NEON_TWIDDLES_16_F16_LEN];
#define ARM_NEON_FACTORS_16_F16_LEN 4
extern const uint32_t arm_neon_factors_16_f16[ARM_NEON_FACTORS_16_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_16_F16 12
#define ARM_NEON_TWIDDLES_32_F16_LEN 48
extern const float16_t arm_neon_twiddles_32_f16[ARM_NEON_TWIDDLES_32_F16_LEN];
#define ARM_NEON_FACTORS_32_F16_LEN 4
extern const uint32_t arm_neon_factors_32_f16[ARM_NEON_FACTORS_32_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_32_F16 24
#define ARM_NEON_TWIDDLES_64_F16_LEN 120
extern const float16_t arm_neon_twiddles_64_f16[ARM_NEON_TWIDDLES_64_F16_LEN];
#define ARM_NEON_FACTORS_64_F16_LEN 4
extern const uint32_t arm_neon_factors_64_f16[ARM_NEON_FACTORS_64_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_64_F16 60
#define ARM_NEON_TWIDDLES_128_F16_LEN 240
extern const float16_t arm_neon_twiddles_128_f16[ARM_NEON_TWIDDLES_128_F16_LEN];
#define ARM_NEON_FACTORS_128_F16_LEN 4
extern const uint32_t arm_neon_factors_128_f16[ARM_NEON_FACTORS_128_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_128_F16 120
#define ARM_NEON_TWIDDLES_256_F16_LEN 504
extern const float16_t arm_neon_twiddles_256_f16[ARM_NEON_TWIDDLES_256_F16_LEN];
#define ARM_NEON_FACTORS_256_F16_LEN 4
extern const uint32_t arm_neon_factors_256_f16[ARM_NEON_FACTORS_256_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_256_F16 252
#define ARM_NEON_TWIDDLES_512_F16_LEN 1008
extern const float16_t arm_neon_twiddles_512_f16[ARM_NEON_TWIDDLES_512_F16_LEN];
#define ARM_NEON_FACTORS_512_F16_LEN 4
extern const uint32_t arm_neon_factors_512_f16[ARM_NEON_FACTORS_512_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_512_F16 504
#define ARM_NEON_TWIDDLES_1024_F16_LEN 2040
extern const float16_t arm_neon_twiddles_1024_f16[ARM_NEON_TWIDDLES_1024_F16_LEN];
#define ARM_NEON_FACTORS_1024_F16_LEN 4
extern const uint32_t arm_neon_factors_1024_f16[ARM_NEON_FACTORS_1024_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_1024_F16 1020
#define ARM_NEON_TWIDDLES_2048_F16_LEN 4080
extern const float16_t arm_neon_twiddles_2048_f16[ARM_NEON_TWIDDLES_2048_F16_LEN];
#define ARM_NEON_FACTORS_2048_F16_LEN 4
extern const uint32_t arm_neon_factors_2048_f16[ARM_NEON_FACTORS_2048_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_2048_F16 2040
#define ARM_NEON_TWIDDLES_4096_F16_LEN 8184
extern const float16_t arm_neon_twiddles_4096_f16[ARM_NEON_TWIDDLES_4096_F16_LEN];
#define ARM_NEON_FACTORS_4096_F16_LEN 4
extern const uint32_t arm_neon_factors_4096_f16[ARM_NEON_FACTORS_4096_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_4096_F16 4092
#define ARM_NEON_RFFT_TWIDDLES_16_F16_LEN 24
extern const float16_t arm_neon_rfft_twiddles_16_f16[ARM_NEON_RFFT_TWIDDLES_16_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_16_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_16_f16[ARM_NEON_RFFT_FACTORS_16_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_16_F16_LEN 0
extern const float16_t arm_neon_rfft_twiddles_neon_16_f16[ARM_NEON_RFFT_TWIDDLES_NEON_16_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_16_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_16_f16[ARM_NEON_RFFT_FACTORS_NEON_16_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_16_F16_LEN 24
extern const float16_t arm_neon_rfft_super_twiddles_neon_16_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_16_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_16_F16 12
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_16_F16 0

#define ARM_NEON_RFFT_TWIDDLES_32_F16_LEN 48
extern const float16_t arm_neon_rfft_twiddles_32_f16[ARM_NEON_RFFT_TWIDDLES_32_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_32_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_32_f16[ARM_NEON_RFFT_FACTORS_32_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_32_F16_LEN 0
extern const float16_t arm_neon_rfft_twiddles_neon_32_f16[ARM_NEON_RFFT_TWIDDLES_NEON_32_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_32_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_32_f16[ARM_NEON_RFFT_FACTORS_NEON_32_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_32_F16_LEN 24
extern const float16_t arm_neon_rfft_super_twiddles_neon_32_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_32_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_32_F16 24
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_32_F16 0

#define ARM_NEON_RFFT_TWIDDLES_64_F16_LEN 120
extern const float16_t arm_neon_rfft_twiddles_64_f16[ARM_NEON_RFFT_TWIDDLES_64_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_64_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_64_f16[ARM_NEON_RFFT_FACTORS_64_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_64_F16_LEN 24
extern const float16_t arm_neon_rfft_twiddles_neon_64_f16[ARM_NEON_RFFT_TWIDDLES_NEON_64_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_64_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_64_f16[ARM_NEON_RFFT_FACTORS_NEON_64_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_64_F16_LEN 48
extern const float16_t arm_neon_rfft_super_twiddles_neon_64_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_64_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_64_F16 60
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_64_F16 12

#define ARM_NEON_RFFT_TWIDDLES_128_F16_LEN 240
extern const float16_t arm_neon_rfft_twiddles_128_f16[ARM_NEON_RFFT_TWIDDLES_128_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_128_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_128_f16[ARM_NEON_RFFT_FACTORS_128_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_128_F16_LEN 48
extern const float16_t arm_neon_rfft_twiddles_neon_128_f16[ARM_NEON_RFFT_TWIDDLES_NEON_128_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_128_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_128_f16[ARM_NEON_RFFT_FACTORS_NEON_128_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_128_F16_LEN 96
extern const float16_t arm_neon_rfft_super_twiddles_neon_128_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_128_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_128_F16 120
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_128_F16 24

#define ARM_NEON_RFFT_TWIDDLES_256_F16_LEN 504
extern const float16_t arm_neon_rfft_twiddles_256_f16[ARM_NEON_RFFT_TWIDDLES_256_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_256_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_256_f16[ARM_NEON_RFFT_FACTORS_256_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_256_F16_LEN 120
extern const float16_t arm_neon_rfft_twiddles_neon_256_f16[ARM_NEON_RFFT_TWIDDLES_NEON_256_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_256_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_256_f16[ARM_NEON_RFFT_FACTORS_NEON_256_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_256_F16_LEN 192
extern const float16_t arm_neon_rfft_super_twiddles_neon_256_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_256_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_256_F16 252
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_256_F16 60

#define ARM_NEON_RFFT_TWIDDLES_512_F16_LEN 1008
extern const float16_t arm_neon_rfft_twiddles_512_f16[ARM_NEON_RFFT_TWIDDLES_512_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_512_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_512_f16[ARM_NEON_RFFT_FACTORS_512_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_512_F16_LEN 240
extern const float16_t arm_neon_rfft_twiddles_neon_512_f16[ARM_NEON_RFFT_TWIDDLES_NEON_512_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_512_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_512_f16[ARM_NEON_RFFT_FACTORS_NEON_512_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_512_F16_LEN 384
extern const float16_t arm_neon_rfft_super_twiddles_neon_512_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_512_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_512_F16 504
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_512_F16 120

#define ARM_NEON_RFFT_TWIDDLES_1024_F16_LEN 2040
extern const float16_t arm_neon_rfft_twiddles_1024_f16[ARM_NEON_RFFT_TWIDDLES_1024_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_1024_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_1024_f16[ARM_NEON_RFFT_FACTORS_1024_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_1024_F16_LEN 504
extern const float16_t arm_neon_rfft_twiddles_neon_1024_f16[ARM_NEON_RFFT_TWIDDLES_NEON_1024_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_1024_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_1024_f16[ARM_NEON_RFFT_FACTORS_NEON_1024_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_1024_F16_LEN 768
extern const float16_t arm_neon_rfft_super_twiddles_neon_1024_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_1024_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_1024_F16 1020
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_1024_F16 252

#define ARM_NEON_RFFT_TWIDDLES_2048_F16_LEN 4080
extern const float16_t arm_neon_rfft_twiddles_2048_f16[ARM_NEON_RFFT_TWIDDLES_2048_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_2048_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_2048_f16[ARM_NEON_RFFT_FACTORS_2048_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_2048_F16_LEN 1008
extern const float16_t arm_neon_rfft_twiddles_neon_2048_f16[ARM_NEON_RFFT_TWIDDLES_NEON_2048_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_2048_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_2048_f16[ARM_NEON_RFFT_FACTORS_NEON_2048_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_2048_F16_LEN 1536
extern const float16_t arm_neon_rfft_super_twiddles_neon_2048_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_2048_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_2048_F16 2040
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_2048_F16 504

#define ARM_NEON_RFFT_TWIDDLES_4096_F16_LEN 8184
extern const float16_t arm_neon_rfft_twiddles_4096_f16[ARM_NEON_RFFT_TWIDDLES_4096_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_4096_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_4096_f16[ARM_NEON_RFFT_FACTORS_4096_F16_LEN];
#define ARM_NEON_RFFT_TWIDDLES_NEON_4096_F16_LEN 2040
extern const float16_t arm_neon_rfft_twiddles_neon_4096_f16[ARM_NEON_RFFT_TWIDDLES_NEON_4096_F16_LEN];
#define ARM_NEON_RFFT_FACTORS_NEON_4096_F16_LEN 4
extern const uint32_t arm_neon_rfft_factors_neon_4096_f16[ARM_NEON_RFFT_FACTORS_NEON_4096_F16_LEN];
#define ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_4096_F16_LEN 3072
extern const float16_t arm_neon_rfft_super_twiddles_neon_4096_f16[ARM_NEON_RFFT_SUPER_TWIDDLES_NEON_4096_F16_LEN];
#define ARM_NE10_OFFSET_BACKWARD_TWID_RFFT_4096_F16 4092
#define ARM_NE10_OFFSET_BACKWARD_TWID_NEON_4096_F16 1020



#endif /* defined(ARM_MATH_NEON_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE) */



#ifdef   __cplusplus
}
#endif

#endif /*ARM_NEON_TABLES_F16_H*/

