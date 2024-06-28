/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_f64_to_q7.c
 * Description:  Converts the elements of the 64 bit floating-point vector to Q7 vector
 *
 * $Date:        18 August 2022
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/support_functions.h"

/**
  @ingroup groupSupport
 */

/**
  @addtogroup f64_to_x
  @{
 */

/**
 * @brief Converts the elements of the 64 bit floating-point vector to Q7 vector.
 * @param[in]       *pSrc points to the 64 bit floating-point input vector
 * @param[out]      *pDst points to the Q7 output vector
 * @param[in]       blockSize length of the input vector
 *
 *\par Description:
 * \par
 * The equation used for the conversion process is:
 * <pre>
 *   pDst[n] = (q7_t)(pSrc[n] * 128);   0 <= n < blockSize.
 * </pre>
 * \par Scaling and Overflow Behavior:
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the allowable Q7 range [0x80 0x7F] will be saturated.
 * \note
 * In order to apply rounding, the library should be rebuilt with the ROUNDING macro
 * defined in the preprocessor section of project options.
 */

ARM_DSP_ATTRIBUTE void arm_f64_to_q7(
  const float64_t * pSrc,
        q7_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
  const float64_t *pIn = pSrc;                         /* Source pointer */

#ifdef ARM_MATH_ROUNDING
        float64_t in;
#endif /* #ifdef ARM_MATH_ROUNDING */

#if defined (ARM_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A * 128 */

    /* Convert from float to q7 and store result in destination buffer */
#ifdef ARM_MATH_ROUNDING

    in = (*pIn++ * 128);
    in += in > 0.0 ? 0.5 : -0.5;
    *pDst++ = (q7_t) (__SSAT((q15_t) (in), 8));

    in = (*pIn++ * 128);
    in += in > 0.0 ? 0.5 : -0.5;
    *pDst++ = (q7_t) (__SSAT((q15_t) (in), 8));

    in = (*pIn++ * 128);
    in += in > 0.0 ? 0.5 : -0.5;
    *pDst++ = (q7_t) (__SSAT((q15_t) (in), 8));

    in = (*pIn++ * 128);
    in += in > 0.0 ? 0.5 : -0.5;
    *pDst++ = (q7_t) (__SSAT((q15_t) (in), 8));

#else

    *pDst++ = __SSAT((q31_t) (*pIn++ * 128.0), 8);
    *pDst++ = __SSAT((q31_t) (*pIn++ * 128.0), 8);
    *pDst++ = __SSAT((q31_t) (*pIn++ * 128.0), 8);
    *pDst++ = __SSAT((q31_t) (*pIn++ * 128.0), 8);

#endif /* #ifdef ARM_MATH_ROUNDING */

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize % 0x4U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = A * 128 */

    /* Convert from float to q7 and store result in destination buffer */
#ifdef ARM_MATH_ROUNDING

    in = (*pIn++ * 128);
    in += in > 0.0 ? 0.5 : -0.5;
    *pDst++ = (q7_t) (__SSAT((q15_t) (in), 8));

#else

    *pDst++ = (q7_t) __SSAT((q31_t) (*pIn++ * 128.0), 8);

#endif /* #ifdef ARM_MATH_ROUNDING */

    /* Decrement loop counter */
    blkCnt--;
  }

}


/**
  @} end of f64_to_x group
 */
