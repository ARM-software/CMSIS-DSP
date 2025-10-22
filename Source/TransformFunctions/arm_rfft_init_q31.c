/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_init_q31.c
 * Description:  RFFT & RIFFT Q31 initialisation function
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
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
#include "arm_compiler_specific.h"


#include "dsp/transform_functions.h"
#include "arm_common_tables.h"
#include "arm_const_structs.h"

/**
  @ingroup RealFFT
*/

/**
  @defgroup RealFFTQ31 Real FFT Q31 Functions
*/

/**
  @addtogroup RealFFTQ31
  @{
 */


#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
#define RFFTINIT_Q31(LEN,CFFTLEN,TWIDMOD)                         \
ARM_DSP_ATTRIBUTE arm_status arm_rfft_init_##LEN##_q31( arm_rfft_instance_q31 * S,  \
    uint32_t ifftFlagR,                                           \
    uint32_t bitReverseFlag )                                     \
{                                                                 \
    /*  Initialise the default arm status */                      \
    arm_status status = ARM_MATH_ARGUMENT_ERROR;                  \
                                                                  \
    /*  Initialize the Real FFT length */                         \
    S->fftLenReal = (uint16_t) LEN;                               \
                                                                  \
    /*  Initialize the Twiddle coefficientA pointer */            \
    S->pTwiddleAReal = (q31_t *) realCoefAQ31;                    \
                                                                  \
    /*  Initialize the Twiddle coefficientB pointer */            \
    S->pTwiddleBReal = (q31_t *) realCoefBQ31;                    \
                                                                  \
    /*  Initialize the Flag for selection of RFFT or RIFFT */     \
    S->ifftFlagR = (uint8_t) ifftFlagR;                           \
                                                                  \
    /*  Initialize the Flag for calculation Bit reversal or not */\
    S->bitReverseFlagR = (uint8_t) bitReverseFlag;                \
                                                                  \
    S->twidCoefRModifier = TWIDMOD;                               \
                                                                  \
    status=arm_cfft_init_##CFFTLEN##_q31(&(S->cfftInst));         \
                                                                  \
    /* return the status of RFFT Init function */                 \
    return (status);                                              \
}
#elif defined(ARM_MATH_NEON) 
#define RFFTINIT_Q31(LEN,CFFTLEN,TWIDMOD)                                          \
ARM_DSP_ATTRIBUTE arm_status arm_rfft_init_##LEN##_q31( arm_rfft_instance_q31 * S )\
{                                                                                  \
    /*  Initialise the default arm status */                                       \
    arm_status status = ARM_MATH_SUCCESS;                                          \
    S->nfft = LEN;                                                                 \
    S->ncfft = LEN >> 1;                                                           \
                                                                                   \
    S->twiddles = arm_neon_rfft_twiddles_##LEN##_q31;                              \
    S->factors = arm_neon_rfft_factors_##LEN##_q31;                                \
                                                                                   \
    S->super_twiddles = arm_neon_rfft_super_twiddles_neon_##LEN##_q31;             \
                                                                                   \
    /* return the status of RFFT Init function */                                  \
    return (status);                                                               \
}
#include "arm_neon_tables.h"


#else
#define RFFTINIT_Q31(LEN,CFFTLEN,TWIDMOD)                         \
ARM_DSP_ATTRIBUTE arm_status arm_rfft_init_##LEN##_q31( arm_rfft_instance_q31 * S,  \
    uint32_t ifftFlagR,                                           \
    uint32_t bitReverseFlag )                                     \
{                                                                 \
    /*  Initialize the Real FFT length */                         \
    S->fftLenReal = (uint16_t) LEN;                               \
                                                                  \
    /*  Initialize the Twiddle coefficientA pointer */            \
    S->pTwiddleAReal = (q31_t *) realCoefAQ31;                    \
                                                                  \
    /*  Initialize the Twiddle coefficientB pointer */            \
    S->pTwiddleBReal = (q31_t *) realCoefBQ31;                    \
                                                                  \
    /*  Initialize the Flag for selection of RFFT or RIFFT */     \
    S->ifftFlagR = (uint8_t) ifftFlagR;                           \
                                                                  \
    /*  Initialize the Flag for calculation Bit reversal or not */\
    S->bitReverseFlagR = (uint8_t) bitReverseFlag;                \
                                                                  \
    S->twidCoefRModifier = TWIDMOD;                               \
                                                                  \
    S->pCfft = &arm_cfft_sR_q31_len##CFFTLEN;                     \
                                                                  \
    /* return the status of RFFT Init function */                 \
    return (ARM_MATH_SUCCESS);                                    \
}
#endif


/**
  @brief         Initialization function for the 8192 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(8192,4096,1)

/**
  @brief         Initialization function for the 4096 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(4096,2048,2)

/**
  @brief         Initialization function for the 2048 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(2048,1024,4)

/**
  @brief         Initialization function for the 1024 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(1024,512,8)

/**
  @brief         Initialization function for the 512 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(512,256,16)

/**
  @brief         Initialization function for the 256 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(256,128,32)

/**
  @brief         Initialization function for the 128 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(128,64,64)

/**
  @brief         Initialization function for the 64 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(64,32,128)

/**
  @brief         Initialization function for the 32 pt Q31 real FFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */
RFFTINIT_Q31(32,16,256)


/**
  @brief         Generic initialization function for the Q31 RFFT/RIFFT.
  @param[in,out] S              points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     fftLenReal     length of the FFT
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par           Details
                   The parameter <code>fftLenReal</code> specifies length of RFFT/RIFFT Process.
                   Supported FFT Lengths are 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192.
  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
  @par
                   This function should be used only if you don't know the FFT sizes that 
                   you'll need at build time. The use of this function will prevent the 
                   linker from removing the FFT tables that are not needed and the library 
                   code size will be bigger than needed.
  @par
                   If you use CMSIS-DSP as a static library, and if you know the FFT sizes 
                   that you need at build time, then it is better to use the initialization
                   functions defined for each FFT size.

*/
#if defined(ARM_MATH_NEON) 
ARM_DSP_ATTRIBUTE arm_status arm_rfft_init_q31(
    arm_rfft_instance_q31 * S,
    uint32_t fftLenReal)
{
       /*  Initialise the default arm status */
    arm_status status = ARM_MATH_ARGUMENT_ERROR;
    /*  Initialization of coef modifier depending on the FFT length */
    switch (fftLenReal)
    {
    case 8192U:
        status = arm_rfft_init_8192_q31( S );
        break;
    case 4096U:
        status = arm_rfft_init_4096_q31( S );
        break;
    case 2048U:
        status = arm_rfft_init_2048_q31( S );
        break;
    case 1024U:
        status = arm_rfft_init_1024_q31( S );
        break;
    case 512U:
        status = arm_rfft_init_512_q31( S );
        break;
    case 256U:
        status = arm_rfft_init_256_q31( S );
        break;
    case 128U:
        status = arm_rfft_init_128_q31( S );
        break;
    case 64U:
        status = arm_rfft_init_64_q31( S );
        break;
    case 32U:
        status = arm_rfft_init_32_q31( S );
        break;
    default:
        /*  Reporting argument error if rfftSize is not valid value */
        status = ARM_MATH_ARGUMENT_ERROR;
        break;
    }

    /* return the status of RFFT Init function */
    return (status);
}
#else
ARM_DSP_ATTRIBUTE arm_status arm_rfft_init_q31(
    arm_rfft_instance_q31 * S,
    uint32_t fftLenReal,
    uint32_t ifftFlagR,
    uint32_t bitReverseFlag)
{
     /*  Initialise the default arm status */
    arm_status status = ARM_MATH_ARGUMENT_ERROR;
    /*  Initialization of coef modifier depending on the FFT length */
    switch (fftLenReal)
    {
    case 8192U:
        status = arm_rfft_init_8192_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 4096U:
        status = arm_rfft_init_4096_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 2048U:
        status = arm_rfft_init_2048_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 1024U:
        status = arm_rfft_init_1024_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 512U:
        status = arm_rfft_init_512_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 256U:
        status = arm_rfft_init_256_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 128U:
        status = arm_rfft_init_128_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 64U:
        status = arm_rfft_init_64_q31( S,ifftFlagR,bitReverseFlag );
        break;
    case 32U:
        status = arm_rfft_init_32_q31( S,ifftFlagR,bitReverseFlag );
        break;
    default:
        /*  Reporting argument error if rfftSize is not valid value */
        status = ARM_MATH_ARGUMENT_ERROR;
        break;
    }

    /* return the status of RFFT Init function */
    return (status);
}
#endif /* neon */
/**
  @} end of RealFFTQ31 group
 */
