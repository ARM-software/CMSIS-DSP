/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_init_f32.c
 * Description:  Initialization function for cfft f32 instance
 *
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2024 ARM Limited or its affiliates. All rights reserved.
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
 * @defgroup ComplexFFTF32 Complex FFT F32
 */

/**
  @ingroup groupTransforms
 */

/**
  @addtogroup ComplexFFT
  @{
 */

/**
  @addtogroup ComplexFFTF32
  @{
 */
#include "dsp/transform_functions.h"
#include "arm_common_tables.h"
#include "arm_const_structs.h"


#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_vec_fft.h"
#include "arm_mve_tables.h"


#define CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F32(LEN)                                               \
static arm_status arm_cfft_radix4by2_rearrange_twiddles_##LEN##_f32(arm_cfft_instance_f32 *S)\
{                                                                                            \
   S->rearranged_twiddle_tab_stride1_arr = rearranged_twiddle_tab_stride1_arr_##LEN##_f32;   \
   S->rearranged_twiddle_stride1  =  rearranged_twiddle_stride1_##LEN##_f32;                 \
                                                                                             \
   S->rearranged_twiddle_tab_stride2_arr = rearranged_twiddle_tab_stride2_arr_##LEN##_f32;   \
   S->rearranged_twiddle_stride2  =  rearranged_twiddle_stride2_##LEN##_f32;                 \
                                                                                             \
   S->rearranged_twiddle_tab_stride3_arr = rearranged_twiddle_tab_stride3_arr_##LEN##_f32;   \
   S->rearranged_twiddle_stride3  =  rearranged_twiddle_stride3_##LEN##_f32;                 \
   return(ARM_MATH_SUCCESS);                                                                 \
}

CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F32(4096);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F32(1024);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F32(256);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F32(64);
CFFT_RADIX4BY2_REARRANGE_TWIDDLES_F32(16);


#define CFFTINIT_F32(LEN,LENTWIDDLE)                                               \
ARM_DSP_ATTRIBUTE arm_status arm_cfft_init_##LEN##_f32(                                           \
  arm_cfft_instance_f32 * S)                                                    \
{                                                                               \
    /*  Initialise the default arm status */                                    \
    arm_status status = ARM_MATH_SUCCESS;                                       \
                                                                                \
    /*  Initialise the FFT length */                                            \
    S->fftLen = LEN;                                                            \
                                                                                \
    /*  Initialise the Twiddle coefficient pointer */                           \
    S->pTwiddle = NULL;                                                         \
                                                                                \
    /*  Initialise the bit reversal table modifier */                           \
    S->bitRevLength = ARMBITREVINDEXTABLE_FIXED_##LEN##_TABLE_LENGTH;           \
    S->pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_##LEN;              \
    S->pTwiddle = (float32_t *)twiddleCoef_##LEN;                               \
    status=arm_cfft_radix4by2_rearrange_twiddles_##LENTWIDDLE##_f32(S);         \
                                                                                \
    return (status);                                                            \
}

#elif defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_neon_tables.h"

#define CFFTINIT_F32(LEN,LENTWIDDLE)                                             \
ARM_DSP_ATTRIBUTE arm_status arm_cfft_init_##LEN##_f32(arm_cfft_instance_f32 * S)\
{                                                                                \
  /*  Initialise the default arm status */                                       \
  arm_status status = ARM_MATH_SUCCESS;                                          \
  S->pTwiddle = arm_neon_twiddles_##LEN##_f32;                                   \
  S->factors=arm_neon_factors_##LEN##_f32;                                       \
  S->fftLen = LEN;                                                               \
  S->algorithm_flag = 0;                                                         \
  S->last_twiddles = NULL;                                                       \
  return status;                                                                 \
}
#else 

#define FFTINIT(EXT,SIZE)                                           \
  S->bitRevLength = arm_cfft_sR_##EXT##_len##SIZE.bitRevLength;        \
  S->pBitRevTable = arm_cfft_sR_##EXT##_len##SIZE.pBitRevTable;         \
  S->pTwiddle = arm_cfft_sR_##EXT##_len##SIZE.pTwiddle;

#define CFFTINIT_F32(LEN,LENTWIDDLE)                                          \
ARM_DSP_ATTRIBUTE arm_status arm_cfft_init_##LEN##_f32(arm_cfft_instance_f32 * S)\
{                                                              \
    /*  Initialise the default arm status */                   \
        arm_status status = ARM_MATH_SUCCESS;                  \
                                                               \
        /*  Initialise the FFT length */                       \
        S->fftLen = LEN;                                       \
                                                               \
        /*  Initialise the Twiddle coefficient pointer */      \
        S->pTwiddle = NULL;                                    \
                                                               \
        FFTINIT(f32,LEN);                                      \
                                                               \
        return (status);                                       \
}

#endif /* if not neon and not mve*/


/**
  @brief         Initialization function for the cfft f32 function with 4096 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(4096,4096)


/**
  @brief         Initialization function for the cfft f32 function with 2048 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(2048,1024)


/**
  @brief         Initialization function for the cfft f32 function with 1024 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(1024,1024)


/**
  @brief         Initialization function for the cfft f32 function with 512 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(512,256)


/**
  @brief         Initialization function for the cfft f32 function with 256 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(256,256)


/**
  @brief         Initialization function for the cfft f32 function with 128 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(128,64)


/**
  @brief         Initialization function for the cfft f32 function with 64 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(64,64)
 

/**
  @brief         Initialization function for the cfft f32 function with 32 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(32,16)


/**
  @brief         Initialization function for the cfft f32 function with 16 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 */
CFFTINIT_F32(16,16)


/**
  @brief         Generic initialization function for the cfft f32 function
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @param[in]     fftLen         fft length (number of complex samples)
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par
                Use of this function is mandatory only for the Helium and Neon versions of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs.h
 
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
ARM_DSP_ATTRIBUTE arm_status arm_cfft_init_f32(
  arm_cfft_instance_f32 * S,
  uint16_t fftLen)
{

        /*  Initialise the default arm status */                                
        arm_status status = ARM_MATH_SUCCESS;                                   
                                                                                                                                                          
        /*  Initializations of Instance structure depending on the FFT length */
        switch (fftLen) {                                                    
            /*  Initializations of structure parameters for 4096 point FFT */   
        case 4096U:  
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_4096_f32(S);
            break;                                                              

            /*  Initializations of structure parameters for 2048 point FFT */   
        case 2048U:                                                             
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_2048_f32(S);
            break;     

            /*  Initializations of structure parameters for 1024 point FFT */   
        case 1024U:                                                             
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_1024_f32(S);
            break;                                                              

            /*  Initializations of structure parameters for 512 point FFT */    
        case 512U:                                                              
            /*  Initialise the bit reversal table modifier */                   
            status=arm_cfft_init_512_f32(S);
            break;                                                              

        case 256U:                                                              
            status=arm_cfft_init_256_f32(S);          
            break;  
                 
        case 128U:                                                              
            status=arm_cfft_init_128_f32(S);
            break;                                                              

        case 64U:                                                               
            status=arm_cfft_init_64_f32(S);           
            break;                                                              

        case 32U:                                                               
            status=arm_cfft_init_32_f32(S);
            break;                                                              

        case 16U:                                                               
            /*  Initializations of structure parameters for 16 point FFT */     
            status=arm_cfft_init_16_f32(S);
            break;                                                              
                                                                                
        default:                                                                
            /*  Reporting argument error if fftSize is not valid value */       
            status = ARM_MATH_ARGUMENT_ERROR;                                   
            break;                                                              
        }                                                                       
                                                                                
                                                                                
        return (status);     
}

/**
  @} end of ComplexFFTF32 group
 */

/**
  @} end of ComplexFFT group
 */
