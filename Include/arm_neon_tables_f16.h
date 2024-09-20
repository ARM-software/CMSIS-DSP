/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_neon_tables_f16.h
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

 #ifndef _ARM_NEON_TABLES_F16_H
 #define _ARM_NEON_TABLES_F16_H

#include "arm_math_types_f16.h"

#ifdef   __cplusplus
extern "C"
{
#endif


 

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)


#if defined(ARM_FLOAT16_SUPPORTED)
#endif /* if defined(ARM_FLOAT16_SUPPORTED) */


#endif /* defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE) */



#ifdef   __cplusplus
}
#endif

#endif /*_ARM_NEON_TABLES_F16_H*/

