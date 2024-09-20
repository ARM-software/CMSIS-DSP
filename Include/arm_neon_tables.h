/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_neon_tables.h
 * Description:  common tables like fft twiddle factors, Bitreverse, reciprocal etc
 *               used for Neon implementation only
 *
 * @version  V1.10.0
 * @date     28 September 2024
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

 #ifndef _ARM_NEON_TABLES_H
 #define _ARM_NEON_TABLES_H

#include "arm_math_types.h"

#ifdef   __cplusplus
extern "C"
{
#endif


 

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)


extern const float32_t twiddle_neon_16_f32[8];
extern const float32_t twiddle_neon_32_f32[24];
extern const float32_t twiddle_neon_64_f32[98];
extern const float32_t twiddle_neon_128_f32[162];
extern const float32_t twiddle_neon_256_f32[290];
extern const float32_t twiddle_neon_512_f32[980];
extern const float32_t twiddle_neon_1024_f32[1492];
extern const float32_t twiddle_neon_2048_f32[2516];
extern const float32_t twiddle_neon_4096_f32[8134];


#endif /* defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE) */



#ifdef   __cplusplus
}
#endif

#endif /*_ARM_NEON_TABLES_H*/

