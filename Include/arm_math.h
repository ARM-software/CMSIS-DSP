/******************************************************************************
 * @file     arm_math.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.10.0
 * @date     08 July 2021
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2021 Arm Limited or its affiliates. All rights reserved.
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


#ifndef ARM_MATH_H
#define ARM_MATH_H


#include "arm_math_types.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"

#include "dsp/basic_math_functions.h"  
#include "dsp/interpolation_functions.h"
#include "dsp/bayes_functions.h"
#include "dsp/matrix_functions.h"
#include "dsp/complex_math_functions.h"
#include "dsp/statistics_functions.h"
#include "dsp/controller_functions.h"
#include "dsp/support_functions.h"
#include "dsp/distance_functions.h"
#include "dsp/svm_functions.h"
#include "dsp/fast_math_functions.h"
#include "dsp/transform_functions.h"
#include "dsp/filtering_functions.h"
#include "dsp/quaternion_math_functions.h"
#include "dsp/window_functions.h"



#ifdef   __cplusplus
extern "C"
{
#endif




//#define TABLE_SPACING_Q31     0x400000
//#define TABLE_SPACING_Q15     0x80





#ifdef   __cplusplus
}
#endif


#endif /* _ARM_MATH_H */

/**
 *
 * End of file.
 */
