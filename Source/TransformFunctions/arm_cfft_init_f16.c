/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_init_f16.c
 * Description:  Initialization function for cfft f16 instance
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2023 ARM Limited or its affiliates. All rights reserved.
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


/**
 * @defgroup ComplexFFTF16 Complex FFT F16
 */

/**
  @ingroup groupTransforms
 */

/**
  @addtogroup ComplexFFT
  @{
 */

/**
  @addtogroup ComplexFFTF16
  @{
 */



#include "dsp/transform_functions_f16.h"
#include "arm_common_tables_f16.h"
#include "arm_const_structs_f16.h"


#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_vec_fft.h"
#include "arm_mve_tables_f16.h"

#define CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F16(LEN)                                           \
static arm_status arm_cfft_radix4by2_rearrange_twiddles_##LEN##_f16(arm_cfft_instance_f16 *S)\
{                                                                                            \
   S->rearranged_twiddle_tab_stride1_arr = rearranged_twiddle_tab_stride1_arr_##LEN##_f16;   \
   S->rearranged_twiddle_stride1  =  rearranged_twiddle_stride1_##LEN##_f16;                 \
                                                                                             \
   S->rearranged_twiddle_tab_stride2_arr = rearranged_twiddle_tab_stride2_arr_##LEN##_f16;   \
   S->rearranged_twiddle_stride2  =  rearranged_twiddle_stride2_##LEN##_f16;                 \
                                                                                             \
   S->rearranged_twiddle_tab_stride3_arr = rearranged_twiddle_tab_stride3_arr_##LEN##_f16;   \
   S->rearranged_twiddle_stride3  =  rearranged_twiddle_stride3_##LEN##_f16;                 \
   return(ARM_MATH_SUCCESS);                                                                 \
}

CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F16(4096);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F16(1024);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F16(256);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F16(64);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F16(16);



#define CFFTINIT_F16(LEN,LENTWIDDLE)                                   \
arm_status arm_cfft_init_##LEN##_f16(                                  \
  arm_cfft_instance_f16 * S)                                           \
{                                                                      \
    /*  Initialise the default arm status */                           \
    arm_status status = ARM_MATH_SUCCESS;                              \
                                                                       \
    /*  Initialise the FFT length */                                   \
    S->fftLen = LEN;                                                   \
                                                                       \
    /*  Initialise the Twiddle coefficient pointer */                  \
    S->pTwiddle = NULL;                                                \
                                                                       \
    /*  Initialise the bit reversal table modifier */                  \
    S->bitRevLength = ARMBITREVINDEXTABLE_FIXED_##LEN##_TABLE_LENGTH;  \
    S->pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_##LEN;     \
    S->pTwiddle = (float16_t *)twiddleCoefF16_##LEN;                   \
    status=arm_cfft_radix4by2_rearrange_twiddles_##LENTWIDDLE##_f16(S);\
                                                                       \
    return (status);                                                   \
}

#elif defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)
#include "arm_neon_tables_f16.h"

#define CFFTINIT_F16(LEN,LENTWIDDLE)                                             \
ARM_DSP_ATTRIBUTE arm_status arm_cfft_init_##LEN##_f16(arm_cfft_instance_f16 * S)\
{                                                                                \
  /*  Initialise the default arm status */                                       \
  arm_status status = ARM_MATH_SUCCESS;                                          \
  S->pTwiddle = arm_neon_twiddles_##LEN##_f16;                                   \
  S->factors=arm_neon_factors_##LEN##_f16;                                       \
  S->fftLen = LEN;                                                               \
  S->algorithm_flag = 0;                                                         \
  S->last_twiddles = NULL;                                                       \
  return status;                                                                 \
}
#else

#if defined(ARM_FLOAT16_SUPPORTED)

#define FFTINIT(EXT,SIZE)                                           \
  S->bitRevLength = arm_cfft_sR_##EXT##_len##SIZE.bitRevLength;        \
  S->pBitRevTable = arm_cfft_sR_##EXT##_len##SIZE.pBitRevTable;         \
  S->pTwiddle = arm_cfft_sR_##EXT##_len##SIZE.pTwiddle;

#define CFFTINIT_F16(LEN,LENTWIDDLE)                                                       \
arm_status arm_cfft_init_##LEN##_f16(arm_cfft_instance_f16 * S)                 \
{                                                                               \
        /*  Initialise the default arm status */                                \
        arm_status status = ARM_MATH_SUCCESS;                                   \
                                                                                \
        /*  Initialise the FFT length */                                        \
        S->fftLen = LEN;                                                        \
                                                                                \
        /*  Initialise the Twiddle coefficient pointer */                       \
        S->pTwiddle = NULL;                                                     \
                                                                                \
        /*  Initializations of Instance structure depending on the FFT length */\
        FFTINIT(f16,LEN);                                                       \
                                                                                \
        return (status);                                                        \
}


#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

#if defined(ARM_MATH_MVE_FLOAT16) || defined(ARM_FLOAT16_SUPPORTED)
/**
  @brief         Initialization function for the cfft f16 function with 4096 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(4096,4096)

/**
  @brief         Initialization function for the cfft f16 function with 2048 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(2048,1024)


/**
  @brief         Initialization function for the cfft f16 function with 1024 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(1024,1024)


/**
  @brief         Initialization function for the cfft f16 function with 512 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(512,256)


/**
  @brief         Initialization function for the cfft f16 function with 256 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(256,256)


/**
  @brief         Initialization function for the cfft f16 function with 128 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(128,64)


/**
  @brief         Initialization function for the cfft f16 function with 64 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(64,64)
 

/**
  @brief         Initialization function for the cfft f16 function with 32 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(32,16)
 

/**
  @brief         Initialization function for the cfft f16 function with 16 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F16(16,16)


/**
  @brief         Generic initialization function for the cfft f16 function
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @param[in]     fftLen         fft length (number of complex samples)
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
  
  @par          This function should be used only if you don't know the FFT sizes that 
                you'll need at build time. The use of this function will prevent the 
                linker from removing the FFT tables that are not needed and the library 
                code size will be bigger than needed.

  @par          If you use CMSIS-DSP as a static library, and if you know the FFT sizes 
                that you need at build time, then it is better to use the initialization
                functions defined for each FFT size.
 */
ARM_DSP_ATTRIBUTE arm_status arm_cfft_init_f16(
  arm_cfft_instance_f16 * S,
  uint16_t fftLen)
{

        /*  Initialise the default arm status */                                
        arm_status status = ARM_MATH_SUCCESS;                                   
                                                                             
        /*  Initializations of Instance structure depending on the FFT length */
        switch (fftLen) {                                                    
            /*  Initializations of structure parameters for 4096 point FFT */   
        case 4096U:  
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_4096_f16(S);
            break;                                                              

            /*  Initializations of structure parameters for 2048 point FFT */   
        case 2048U:                                                             
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_2048_f16(S);
            break;     

            /*  Initializations of structure parameters for 1024 point FFT */   
        case 1024U:                                                             
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_1024_f16(S);
            break;                                                              

            /*  Initializations of structure parameters for 512 point FFT */    
        case 512U:                                                              
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_512_f16(S);
            break;                                                              

        case 256U:                                                              
            status=arm_cfft_init_256_f16(S);
            break;  
                 
        case 128U:                                                              
            status=arm_cfft_init_128_f16(S);
            break;                                                              

        case 64U:                                                               
            status=arm_cfft_init_64_f16(S);
            break;                                                              

        case 32U:                                                               
            status=arm_cfft_init_32_f16(S);
            break;                                                              

        case 16U:                                                               
            /*  Initializations of structure parameters for 16 point FFT */     
            status=arm_cfft_init_16_f16(S);
            break;                                                              
                                                                                
        default:                                                                
            /*  Reporting argument error if fftSize is not valid value */       
            status = ARM_MATH_ARGUMENT_ERROR;                                   
            break;                                                              
        }                                                                       
                                                                                
                                                                                
        return (status);     
}
#endif /* Float 16 used*/

/**
  @} end of ComplexFFTF16 group
 */

/**
  @} end of ComplexFFT group
 */