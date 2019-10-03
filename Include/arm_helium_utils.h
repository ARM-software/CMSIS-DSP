/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_helium_utils.h
 * Description:  Utility functions for Helium development
 *
 * $Date:        09. September 2019
 * $Revision:    V.1.5.1
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2019 ARM Limited or its affiliates. All rights reserved.
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

#ifndef _ARM_UTILS_HELIUM_H_
#define _ARM_UTILS_HELIUM_H_

/***************************************

Definitions available for MVEF and MVEI

***************************************/
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)

#endif /* defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI) */

/***************************************

Definitions available for MVEF only

***************************************/
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF)
__STATIC_FORCEINLINE float32_t vecAddAcrossF32Mve(float32x4_t in)
{
    float32_t acc;

    acc = vgetq_lane(in, 0) + vgetq_lane(in, 1) +
          vgetq_lane(in, 2) + vgetq_lane(in, 3);

    return acc;
}
#endif /* defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) */

/***************************************

Definitions available for MVEI only

***************************************/
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEI)
#endif /* defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEI) */

#endif