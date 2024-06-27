/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mve_tables.h
 * Description:  common tables like fft twiddle factors, Bitreverse, reciprocal etc
 *               used for MVE implementation only
 *
 * @version  V1.10.0
 * @date     04 October 2021
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

 #ifndef ARM_MVE_TABLES_H
 #define ARM_MVE_TABLES_H

#include "arm_math_types.h"

#ifdef   __cplusplus
extern "C"
{
#endif


 

#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)



extern const uint32_t rearranged_twiddle_tab_stride1_arr_16_f32[2];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_16_f32[2];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_16_f32[2];
extern const float32_t rearranged_twiddle_stride1_16_f32[8];
extern const float32_t rearranged_twiddle_stride2_16_f32[8];
extern const float32_t rearranged_twiddle_stride3_16_f32[8];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_64_f32[3];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_64_f32[3];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_64_f32[3];
extern const float32_t rearranged_twiddle_stride1_64_f32[40];
extern const float32_t rearranged_twiddle_stride2_64_f32[40];
extern const float32_t rearranged_twiddle_stride3_64_f32[40];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_256_f32[4];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_256_f32[4];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_256_f32[4];
extern const float32_t rearranged_twiddle_stride1_256_f32[168];
extern const float32_t rearranged_twiddle_stride2_256_f32[168];
extern const float32_t rearranged_twiddle_stride3_256_f32[168];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_1024_f32[5];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_1024_f32[5];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_1024_f32[5];
extern const float32_t rearranged_twiddle_stride1_1024_f32[680];
extern const float32_t rearranged_twiddle_stride2_1024_f32[680];
extern const float32_t rearranged_twiddle_stride3_1024_f32[680];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_4096_f32[6];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_4096_f32[6];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_4096_f32[6];
extern const float32_t rearranged_twiddle_stride1_4096_f32[2728];
extern const float32_t rearranged_twiddle_stride2_4096_f32[2728];
extern const float32_t rearranged_twiddle_stride3_4096_f32[2728];


#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */



#if defined(ARM_MATH_MVEI)  && !defined(ARM_MATH_AUTOVECTORIZE)



extern const uint32_t rearranged_twiddle_tab_stride1_arr_16_q31[2];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_16_q31[2];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_16_q31[2];
extern const q31_t rearranged_twiddle_stride1_16_q31[8];
extern const q31_t rearranged_twiddle_stride2_16_q31[8];
extern const q31_t rearranged_twiddle_stride3_16_q31[8];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_64_q31[3];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_64_q31[3];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_64_q31[3];
extern const q31_t rearranged_twiddle_stride1_64_q31[40];
extern const q31_t rearranged_twiddle_stride2_64_q31[40];
extern const q31_t rearranged_twiddle_stride3_64_q31[40];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_256_q31[4];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_256_q31[4];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_256_q31[4];
extern const q31_t rearranged_twiddle_stride1_256_q31[168];
extern const q31_t rearranged_twiddle_stride2_256_q31[168];
extern const q31_t rearranged_twiddle_stride3_256_q31[168];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_1024_q31[5];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_1024_q31[5];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_1024_q31[5];
extern const q31_t rearranged_twiddle_stride1_1024_q31[680];
extern const q31_t rearranged_twiddle_stride2_1024_q31[680];
extern const q31_t rearranged_twiddle_stride3_1024_q31[680];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_4096_q31[6];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_4096_q31[6];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_4096_q31[6];
extern const q31_t rearranged_twiddle_stride1_4096_q31[2728];
extern const q31_t rearranged_twiddle_stride2_4096_q31[2728];
extern const q31_t rearranged_twiddle_stride3_4096_q31[2728];



#endif /* defined(ARM_MATH_MVEI) */



#if defined(ARM_MATH_MVEI)  && !defined(ARM_MATH_AUTOVECTORIZE)



extern const uint32_t rearranged_twiddle_tab_stride1_arr_16_q15[2];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_16_q15[2];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_16_q15[2];
extern const q15_t rearranged_twiddle_stride1_16_q15[8];
extern const q15_t rearranged_twiddle_stride2_16_q15[8];
extern const q15_t rearranged_twiddle_stride3_16_q15[8];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_64_q15[3];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_64_q15[3];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_64_q15[3];
extern const q15_t rearranged_twiddle_stride1_64_q15[40];
extern const q15_t rearranged_twiddle_stride2_64_q15[40];
extern const q15_t rearranged_twiddle_stride3_64_q15[40];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_256_q15[4];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_256_q15[4];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_256_q15[4];
extern const q15_t rearranged_twiddle_stride1_256_q15[168];
extern const q15_t rearranged_twiddle_stride2_256_q15[168];
extern const q15_t rearranged_twiddle_stride3_256_q15[168];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_1024_q15[5];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_1024_q15[5];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_1024_q15[5];
extern const q15_t rearranged_twiddle_stride1_1024_q15[680];
extern const q15_t rearranged_twiddle_stride2_1024_q15[680];
extern const q15_t rearranged_twiddle_stride3_1024_q15[680];


extern const uint32_t rearranged_twiddle_tab_stride1_arr_4096_q15[6];
extern const uint32_t rearranged_twiddle_tab_stride2_arr_4096_q15[6];
extern const uint32_t rearranged_twiddle_tab_stride3_arr_4096_q15[6];
extern const q15_t rearranged_twiddle_stride1_4096_q15[2728];
extern const q15_t rearranged_twiddle_stride2_4096_q15[2728];
extern const q15_t rearranged_twiddle_stride3_4096_q15[2728];


#endif /* defined(ARM_MATH_MVEI) */



#ifdef   __cplusplus
}
#endif

#endif /*_ARM_MVE_TABLES_H*/

