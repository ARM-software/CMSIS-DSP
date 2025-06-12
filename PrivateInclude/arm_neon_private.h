/******************************************************************************
 * @file     arm_neon_private.h
 * @brief    Private header file for CMSIS DSP Library
 ******************************************************************************/
/*
 * Copyright (c) 2025 Arm Limited or its affiliates. All rights reserved.
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

#ifndef ARM_NEON_PRIVATE_H_
#define ARM_NEON_PRIVATE_H_

#include "arm_math_types.h"
#include "arm_math_memory.h"

#ifdef   __cplusplus
extern "C"
{
#endif

#if defined(ARM_MATH_NEON) || defined(ARM_MATH_NEON_EXPERIMENTAL)

#define DCS 4 // Max number of vectors for columns (can't be changed)
#define MAXDC (8*DCS) // Must be multiple of lane

#define DR 4 // Max number rows in kernels (can't be changed)
#define HDR 2 // Max number rows by half

// INNER * DC < L1/2
// INNER * ROWS < L2 
// INNER * COLS < L3
#define INNER_BLOCK ((((ARM_MATH_L1_CACHE_SIZE>>2) >> 1)/MAXDC+MAXDC-1) & ~(MAXDC-1))
#define ROWS_BLOCK ((((ARM_MATH_L2_CACHE_SIZE>>2) >> 0) / INNER_BLOCK + DR - 1) & ~(DR-1) ) 
#define COLS_BLOCK (((ARM_MATH_L3_CACHE_SIZE>>2) / INNER_BLOCK + MAXDC - 1) & ~(MAXDC-1)) 


#endif 

#ifdef   __cplusplus
}
#endif


#endif /* _ARM_VEC_FILTERING_H_ */