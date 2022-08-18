/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_q15_to_f64.c
 * Description:  Converts the elements of the Q15 vector to 64 bit floating-point vector
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
 * @defgroup q15_to_x  Convert 16-bit fixed point value
 */

/**
  @addtogroup q15_to_x
  @{
 */

/**
  @brief         Converts the elements of the Q15 vector to 64 bit floating-point vector.
  @param[in]     pSrc       points to the Q15 input vector
  @param[out]    pDst       points to the 64 bit floating-point output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Details
                   The equation used for the conversion process is:
  <pre>
      pDst[n] = (float64_t) pSrc[n] / 32768;   0 <= n < blockSize.
  </pre>
 */

void arm_q15_to_f64(
  const q15_t * pSrc,
        float64_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
  const q15_t *pIn = pSrc;                             /* Source pointer */

#if defined (ARM_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = (float64_t) A / 32768 */

    /* Convert from q15 to float and store result in destination buffer */
    *pDst++ = ((float64_t) * pIn++ / 32768.0);
    *pDst++ = ((float64_t) * pIn++ / 32768.0);
    *pDst++ = ((float64_t) * pIn++ / 32768.0);
    *pDst++ = ((float64_t) * pIn++ / 32768.0);

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
    /* C = (float64_t) A / 32768 */

    /* Convert from q15 to float and store result in destination buffer */
    *pDst++ = ((float64_t) *pIn++ / 32768.0);

    /* Decrement loop counter */
    blkCnt--;
  }

}


/**
  @} end of q15_to_x group
 */
