/******************************************************************************
 * @file     transform_functions.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.10.0
 * @date     08 July 2021
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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

 
#ifndef TRANSFORM_FUNCTIONS_H_
#define TRANSFORM_FUNCTIONS_H_

#include "arm_math_types.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"

#include "dsp/basic_math_functions.h"
#include "dsp/complex_math_functions.h"

// Only available for Neon versions of the FFTs
// otherwise only powers of 2 are supported

#if !defined(ARM_MIXED_RADIX_FFT)
#define ARM_MIXED_RADIX_FFT 1
#endif

#if !defined(ARM_MATH_NEON) || defined(ARM_MATH_AUTOVECTORIZE)
#if defined(ARM_MFCC_CFFT_BASED)
#if !defined(ARM_MFCC_USE_CFFT)
#define ARM_MFCC_USE_CFFT
#endif
#endif
#endif

#ifdef   __cplusplus
extern "C"
{
#endif


/**
 * @defgroup groupTransforms Transform Functions
 * 
 * CMSIS-DSP provides CFFT and RFFT for different architectures.
 * The implementation of those transforms may be different for the
 * different architectures : different algorithms, different capabilities
 * of the instruction set.
 * 
 * All those variants are not giving exactly the same results but they 
 * are passing the same tests with same SNR checks and same threshold for 
 * the sample errors.
 * 
 * The APIs for the Neon variants are different : some additional
 * temporary buffers are required.
 * 
 * Float16 versions are provided but they are not very accurate and 
 * should only be used for small FFTs.
 * 
 * @par Transform initializations
 *  
 *  There are several ways to initialize the transform functions.
 *  Below explanations are using q15 as example but the same explanations
 *  apply to other datatypes.
 *  
 *  Before Helium and Neon, you can just use a pre-initialized 
 *  constant data structure and use it in the arm_cfft_... function.
 *  For instance, &arm_cfft_sR_q15_len256 for a 256 Q15 CFFT.
 *  
 *  On Helium and Neon, you *must* use an initialization function. If you know the size of your FFT (or other transform) you can use a specific initialization function for this size only : like arm_cfft_init_256_q15 for a 256 Q15 complex FFT.
 *  
 *  By using a specific initialization function, you give an hint to the linker and it will be able to remove all unused initialization tables (some compilation and link flags must be used for the linker to be able to do this optimization. It is compiler dependent).
 *  
 *  If you don't know the size you'll need at runtime, you need to use a function like arm_cfft_init_q15. If you use such a function, all the tables for all FFT sizes (up to the CMSIS-DSP maximum of 4096) will be included in the build !
 *  
 *  On Neon, there is another possibility. You can use arm_cfft_init_dynamic_q15. This function will allocate a buffer at runtime and compute at runtime all the tables that are required for a specific FFT size. This initialization is also supported by RFFT.
 *  The computation to initialize all the tables can take lot of cycles
 *  (since several cos and sin must be computed)
 *  
 *  With this new Neon specific initialization you can use longer lengths.
 *  With CFFT, you can also use lengths containing radix 3 and/or 5 (but
 *  the length must still be a multiple of 4).
 * 
 * @par Size of buffers according to the target architecture and datatype:
 *      They are described on the page \ref transformbuffers "transform buffers".
 */


  /**
   * @brief Instance structure for the Q15 CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                 /**< length of the FFT. */
          uint8_t ifftFlag;                /**< flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform. */
          uint8_t bitReverseFlag;          /**< flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output. */
    const q15_t *pTwiddle;                 /**< points to the Sin twiddle factor table. */
    const uint16_t *pBitRevTable;          /**< points to the bit reversal table. */
          uint16_t twidCoefModifier;       /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
          uint16_t bitRevFactor;           /**< bit reversal modifier that supports different size FFTs with the same bit reversal table. */
  } arm_cfft_radix2_instance_q15;

/* Deprecated */
  arm_status arm_cfft_radix2_init_q15(
        arm_cfft_radix2_instance_q15 * S,
        uint16_t fftLen,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

/* Deprecated */
  void arm_cfft_radix2_q15(
  const arm_cfft_radix2_instance_q15 * S,
        q15_t * pSrc);


  /**
   * @brief Instance structure for the Q15 CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                 /**< length of the FFT. */
          uint8_t ifftFlag;                /**< flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform. */
          uint8_t bitReverseFlag;          /**< flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output. */
    const q15_t *pTwiddle;                 /**< points to the twiddle factor table. */
    const uint16_t *pBitRevTable;          /**< points to the bit reversal table. */
          uint16_t twidCoefModifier;       /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
          uint16_t bitRevFactor;           /**< bit reversal modifier that supports different size FFTs with the same bit reversal table. */
  } arm_cfft_radix4_instance_q15;

/* Deprecated */
  arm_status arm_cfft_radix4_init_q15(
        arm_cfft_radix4_instance_q15 * S,
        uint16_t fftLen,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

/* Deprecated */
  void arm_cfft_radix4_q15(
  const arm_cfft_radix4_instance_q15 * S,
        q15_t * pSrc);

  /**
   * @brief Instance structure for the Radix-2 Q31 CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                 /**< length of the FFT. */
          uint8_t ifftFlag;                /**< flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform. */
          uint8_t bitReverseFlag;          /**< flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output. */
    const q31_t *pTwiddle;                 /**< points to the Twiddle factor table. */
    const uint16_t *pBitRevTable;          /**< points to the bit reversal table. */
          uint16_t twidCoefModifier;       /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
          uint16_t bitRevFactor;           /**< bit reversal modifier that supports different size FFTs with the same bit reversal table. */
  } arm_cfft_radix2_instance_q31;

/* Deprecated */
  arm_status arm_cfft_radix2_init_q31(
        arm_cfft_radix2_instance_q31 * S,
        uint16_t fftLen,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

/* Deprecated */
  void arm_cfft_radix2_q31(
  const arm_cfft_radix2_instance_q31 * S,
        q31_t * pSrc);

  /**
   * @brief Instance structure for the Q31 CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                 /**< length of the FFT. */
          uint8_t ifftFlag;                /**< flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform. */
          uint8_t bitReverseFlag;          /**< flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output. */
    const q31_t *pTwiddle;                 /**< points to the twiddle factor table. */
    const uint16_t *pBitRevTable;          /**< points to the bit reversal table. */
          uint16_t twidCoefModifier;       /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
          uint16_t bitRevFactor;           /**< bit reversal modifier that supports different size FFTs with the same bit reversal table. */
  } arm_cfft_radix4_instance_q31;

/* Deprecated */
  void arm_cfft_radix4_q31(
  const arm_cfft_radix4_instance_q31 * S,
        q31_t * pSrc);

/* Deprecated */
  arm_status arm_cfft_radix4_init_q31(
        arm_cfft_radix4_instance_q31 * S,
        uint16_t fftLen,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

  /**
   * @brief Instance structure for the floating-point CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                   /**< length of the FFT. */
          uint8_t ifftFlag;                  /**< flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform. */
          uint8_t bitReverseFlag;            /**< flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output. */
    const float32_t *pTwiddle;               /**< points to the Twiddle factor table. */
    const uint16_t *pBitRevTable;            /**< points to the bit reversal table. */
          uint16_t twidCoefModifier;         /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
          uint16_t bitRevFactor;             /**< bit reversal modifier that supports different size FFTs with the same bit reversal table. */
          float32_t onebyfftLen;             /**< value of 1/fftLen. */
  } arm_cfft_radix2_instance_f32;


/* Deprecated */
  arm_status arm_cfft_radix2_init_f32(
        arm_cfft_radix2_instance_f32 * S,
        uint16_t fftLen,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

/* Deprecated */
  void arm_cfft_radix2_f32(
  const arm_cfft_radix2_instance_f32 * S,
        float32_t * pSrc);

  /**
   * @brief Instance structure for the floating-point CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                   /**< length of the FFT. */
          uint8_t ifftFlag;                  /**< flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform. */
          uint8_t bitReverseFlag;            /**< flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output. */
    const float32_t *pTwiddle;               /**< points to the Twiddle factor table. */
    const uint16_t *pBitRevTable;            /**< points to the bit reversal table. */
          uint16_t twidCoefModifier;         /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
          uint16_t bitRevFactor;             /**< bit reversal modifier that supports different size FFTs with the same bit reversal table. */
          float32_t onebyfftLen;             /**< value of 1/fftLen. */
  } arm_cfft_radix4_instance_f32;



/* Deprecated */
  arm_status arm_cfft_radix4_init_f32(
        arm_cfft_radix4_instance_f32 * S,
        uint16_t fftLen,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

/* Deprecated */
  void arm_cfft_radix4_f32(
  const arm_cfft_radix4_instance_f32 * S,
        float32_t * pSrc);

  /**
   * @brief Instance structure for the fixed-point CFFT/CIFFT function.
   */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
typedef struct
{
          uint32_t fftLen;                   /**< length of the FFT. */
    const q15_t *pTwiddle;         /**< points to the Twiddle factor table. */
    const q15_t *last_twiddles; /**< last stage twiddle used for mixed radix */
    const uint32_t *factors;
    int32_t algorithm_flag;
} arm_cfft_instance_q15;
#else
  typedef struct
  {
          uint16_t fftLen;                   /**< length of the FFT. */
    const q15_t *pTwiddle;             /**< points to the Twiddle factor table. */
    const uint16_t *pBitRevTable;      /**< points to the bit reversal table. */
          uint16_t bitRevLength;             /**< bit reversal table length. */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
   const uint32_t *rearranged_twiddle_tab_stride1_arr;        /**< Per stage reordered twiddle pointer (offset 1) */                                                       \
   const uint32_t *rearranged_twiddle_tab_stride2_arr;        /**< Per stage reordered twiddle pointer (offset 2) */                                                       \
   const uint32_t *rearranged_twiddle_tab_stride3_arr;        /**< Per stage reordered twiddle pointer (offset 3) */                                                       \
   const q15_t *rearranged_twiddle_stride1; /**< reordered twiddle offset 1 storage */                                                                   \
   const q15_t *rearranged_twiddle_stride2; /**< reordered twiddle offset 2 storage */                                                                   \
   const q15_t *rearranged_twiddle_stride3;
#endif
  } arm_cfft_instance_q15;
#endif 

arm_status arm_cfft_init_4096_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_2048_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_1024_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_512_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_256_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_128_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_64_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_32_q15(arm_cfft_instance_q15 * S);
arm_status arm_cfft_init_16_q15(arm_cfft_instance_q15 * S);

arm_status arm_cfft_init_q15(
  arm_cfft_instance_q15 * S,
  uint16_t fftLen);



#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

extern arm_cfft_instance_q15 *arm_cfft_init_dynamic_q15(uint32_t fftLen);

void arm_cfft_q15(
    const arm_cfft_instance_q15 * S,
          const q15_t * src,
          q15_t * dst,
          q15_t * buffer,
          uint8_t ifftFlag
          );
#else
void arm_cfft_q15(
    const arm_cfft_instance_q15 * S,
          q15_t * p1,
          uint8_t ifftFlag,
          uint8_t bitReverseFlag);
#endif 

  /**
   * @brief Instance structure for the fixed-point CFFT/CIFFT function.
   */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
typedef struct
{
          uint32_t fftLen;                   /**< length of the FFT. */
    const q31_t *pTwiddle;         /**< points to the Twiddle factor table. */
    const q31_t *last_twiddles; /**< last stage twiddle used for mixed radix */
    const uint32_t *factors;
    int32_t algorithm_flag;
} arm_cfft_instance_q31;
#else
  typedef struct
  {
          uint16_t fftLen;                   /**< length of the FFT. */
    const q31_t *pTwiddle;             /**< points to the Twiddle factor table. */
    const uint16_t *pBitRevTable;      /**< points to the bit reversal table. */
          uint16_t bitRevLength;             /**< bit reversal table length. */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
   const uint32_t *rearranged_twiddle_tab_stride1_arr;        /**< Per stage reordered twiddle pointer (offset 1) */                                                       \
   const uint32_t *rearranged_twiddle_tab_stride2_arr;        /**< Per stage reordered twiddle pointer (offset 2) */                                                       \
   const uint32_t *rearranged_twiddle_tab_stride3_arr;        /**< Per stage reordered twiddle pointer (offset 3) */                                                       \
   const q31_t *rearranged_twiddle_stride1; /**< reordered twiddle offset 1 storage */                                                                   \
   const q31_t *rearranged_twiddle_stride2; /**< reordered twiddle offset 2 storage */                                                                   \
   const q31_t *rearranged_twiddle_stride3;
#endif
  } arm_cfft_instance_q31;
#endif 

arm_status arm_cfft_init_4096_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_2048_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_1024_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_512_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_256_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_128_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_64_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_32_q31(arm_cfft_instance_q31 * S);
arm_status arm_cfft_init_16_q31(arm_cfft_instance_q31 * S);

arm_status arm_cfft_init_q31(
  arm_cfft_instance_q31 * S,
  uint16_t fftLen);

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

extern arm_cfft_instance_q31 *arm_cfft_init_dynamic_q31(uint32_t fftLen);

void arm_cfft_q31(
    const arm_cfft_instance_q31 * S,
          const q31_t * src,
          q31_t * dst,
          q31_t * buffer,
          uint8_t ifftFlag
          );
#else
void arm_cfft_q31(
    const arm_cfft_instance_q31 * S,
          q31_t * p1,
          uint8_t ifftFlag,
          uint8_t bitReverseFlag);
#endif

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
typedef struct
{
          uint32_t fftLen;                   /**< length of the FFT. */
    const float32_t *pTwiddle;         /**< points to the Twiddle factor table. */
    const float32_t *last_twiddles; /**< last stage twiddle used for mixed radix */
    const uint32_t *factors;
    int32_t algorithm_flag;
} arm_cfft_instance_f32;
#else
  /**
   * @brief Instance structure for the floating-point CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                   /**< length of the FFT. */
    const float32_t *pTwiddle;         /**< points to the Twiddle factor table. */
    const uint16_t *pBitRevTable;      /**< points to the bit reversal table. */
          uint16_t bitRevLength;             /**< bit reversal table length. */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
   const uint32_t *rearranged_twiddle_tab_stride1_arr;        /**< Per stage reordered twiddle pointer (offset 1) */                                                       \
   const uint32_t *rearranged_twiddle_tab_stride2_arr;        /**< Per stage reordered twiddle pointer (offset 2) */                                                       \
   const uint32_t *rearranged_twiddle_tab_stride3_arr;        /**< Per stage reordered twiddle pointer (offset 3) */                                                       \
   const float32_t *rearranged_twiddle_stride1; /**< reordered twiddle offset 1 storage */                                                                   \
   const float32_t *rearranged_twiddle_stride2; /**< reordered twiddle offset 2 storage */                                                                   \
   const float32_t *rearranged_twiddle_stride3;
#endif
  } arm_cfft_instance_f32;
#endif


arm_status arm_cfft_init_4096_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_2048_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_1024_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_512_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_256_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_128_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_64_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_32_f32(arm_cfft_instance_f32 * S);
arm_status arm_cfft_init_16_f32(arm_cfft_instance_f32 * S);

  arm_status arm_cfft_init_f32(
  arm_cfft_instance_f32 * S,
  uint16_t fftLen);

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

extern arm_cfft_instance_f32 *arm_cfft_init_dynamic_f32(uint32_t fftLen);

/* `pIn` content is modified by the function.
   `pIn` array must be different from `pOut` one
*/
void arm_cfft_f32(
  const arm_cfft_instance_f32 * S,
        const float32_t * pIn,
        float32_t * pOut,
        float32_t * pBuffer, /* When used, `in` is not modified */
        uint8_t ifftFlag);
#else
  void arm_cfft_f32(
  const arm_cfft_instance_f32 * S,
        float32_t * p1,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);
#endif


  /**
   * @brief Instance structure for the Double Precision Floating-point CFFT/CIFFT function.
   */
  typedef struct
  {
          uint16_t fftLen;                   /**< length of the FFT. */
    const float64_t *pTwiddle;         /**< points to the Twiddle factor table. */
    const uint16_t *pBitRevTable;      /**< points to the bit reversal table. */
          uint16_t bitRevLength;             /**< bit reversal table length. */
  } arm_cfft_instance_f64;

arm_status arm_cfft_init_4096_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_2048_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_1024_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_512_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_256_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_128_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_64_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_32_f64(arm_cfft_instance_f64 * S);
arm_status arm_cfft_init_16_f64(arm_cfft_instance_f64 * S);

  arm_status arm_cfft_init_f64(
  arm_cfft_instance_f64 * S,
  uint16_t fftLen);
  
  void arm_cfft_f64(
  const arm_cfft_instance_f64 * S,
        float64_t * p1,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

  /**
   * @brief Instance structure for the Q15 RFFT/RIFFT function.
   */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
  typedef struct
  {
    uint32_t nfft;
    uint32_t ncfft;
    const uint32_t *factors;
    const q15_t *twiddles;
    const q15_t *super_twiddles;
  } arm_rfft_instance_q15;
#else
  typedef struct
  {
          uint32_t fftLenReal;                      /**< length of the real FFT. */
          uint8_t ifftFlagR;                        /**< flag that selects forward (ifftFlagR=0) or inverse (ifftFlagR=1) transform. */
          uint8_t bitReverseFlagR;                  /**< flag that enables (bitReverseFlagR=1) or disables (bitReverseFlagR=0) bit reversal of output. */
          uint32_t twidCoefRModifier;               /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
    const q15_t *pTwiddleAReal;                     /**< points to the real twiddle factor table. */
    const q15_t *pTwiddleBReal;                     /**< points to the imag twiddle factor table. */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
    arm_cfft_instance_q15 cfftInst;
#else
    const arm_cfft_instance_q15 *pCfft;       /**< points to the complex FFT instance. */
#endif
  } arm_rfft_instance_q15;
#endif 
  
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

extern arm_rfft_instance_q15 *arm_rfft_init_dynamic_q15(uint32_t fftLenReal);

arm_status arm_rfft_init_32_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_64_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_128_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_256_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_512_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_1024_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_2048_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_4096_q15(
        arm_rfft_instance_q15 * S);

arm_status arm_rfft_init_8192_q15(
        arm_rfft_instance_q15 * S);

  arm_status arm_rfft_init_q15(
        arm_rfft_instance_q15 * S,
        uint32_t fftLenReal);

void arm_rfft_q15(
  const arm_rfft_instance_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        q15_t * tmp,
        uint8_t ifftFlag);
#else
arm_status arm_rfft_init_32_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_64_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_128_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_256_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_512_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_1024_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_2048_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_4096_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

arm_status arm_rfft_init_8192_q15(
        arm_rfft_instance_q15 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_q15(
        arm_rfft_instance_q15 * S,
        uint32_t fftLenReal,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  void arm_rfft_q15(
  const arm_rfft_instance_q15 * S,
        q15_t * pSrc,
        q15_t * pDst);
#endif 

  /**
   * @brief Instance structure for the Q31 RFFT/RIFFT function.
   */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
  typedef struct
  {
    uint32_t nfft;
    uint32_t ncfft;
    const uint32_t *factors;
    const q31_t *twiddles;
    const q31_t *super_twiddles;
  } arm_rfft_instance_q31;
#else
  typedef struct
  {
          uint32_t fftLenReal;                        /**< length of the real FFT. */
          uint8_t ifftFlagR;                          /**< flag that selects forward (ifftFlagR=0) or inverse (ifftFlagR=1) transform. */
          uint8_t bitReverseFlagR;                    /**< flag that enables (bitReverseFlagR=1) or disables (bitReverseFlagR=0) bit reversal of output. */
          uint32_t twidCoefRModifier;                 /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
    const q31_t *pTwiddleAReal;                       /**< points to the real twiddle factor table. */
    const q31_t *pTwiddleBReal;                       /**< points to the imag twiddle factor table. */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
    arm_cfft_instance_q31 cfftInst;
#else
    const arm_cfft_instance_q31 *pCfft;         /**< points to the complex FFT instance. */
#endif
  } arm_rfft_instance_q31;
#endif 


#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

extern arm_rfft_instance_q31 *arm_rfft_init_dynamic_q31(uint32_t fftLenReal);

    arm_status arm_rfft_init_32_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_64_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_128_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_256_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_512_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_1024_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_2048_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_4096_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_8192_q31(
        arm_rfft_instance_q31 * S);

  arm_status arm_rfft_init_q31(
        arm_rfft_instance_q31 * S,
        uint32_t fftLenReal);

  void arm_rfft_q31(
  const arm_rfft_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        q31_t * tmp,
        uint8_t ifftFlag);
#else
    arm_status arm_rfft_init_32_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_64_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_128_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_256_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_512_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_1024_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_2048_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_4096_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_8192_q31(
        arm_rfft_instance_q31 * S,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  arm_status arm_rfft_init_q31(
        arm_rfft_instance_q31 * S,
        uint32_t fftLenReal,
        uint32_t ifftFlagR,
        uint32_t bitReverseFlag);

  void arm_rfft_q31(
  const arm_rfft_instance_q31 * S,
        q31_t * pSrc,
        q31_t * pDst);
#endif

  /**
   * @brief Instance structure for the floating-point RFFT/RIFFT function.
   */
  typedef struct
  {
          uint32_t fftLenReal;                        /**< length of the real FFT. */
          uint16_t fftLenBy2;                         /**< length of the complex FFT. */
          uint8_t ifftFlagR;                          /**< flag that selects forward (ifftFlagR=0) or inverse (ifftFlagR=1) transform. */
          uint8_t bitReverseFlagR;                    /**< flag that enables (bitReverseFlagR=1) or disables (bitReverseFlagR=0) bit reversal of output. */
          uint32_t twidCoefRModifier;                     /**< twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table. */
    const float32_t *pTwiddleAReal;                   /**< points to the real twiddle factor table. */
    const float32_t *pTwiddleBReal;                   /**< points to the imag twiddle factor table. */
          arm_cfft_radix4_instance_f32 *pCfft;        /**< points to the complex FFT instance. */
  } arm_rfft_instance_f32;

  /**
   * @brief Instance structure for the Double Precision Floating-point RFFT/RIFFT function.
   */
typedef struct
  {
          arm_cfft_instance_f64 Sint;      /**< Internal CFFT structure. */
          uint16_t fftLenRFFT;             /**< length of the real sequence */
    const float64_t * pTwiddleRFFT;        /**< Twiddle factors real stage  */
  } arm_rfft_fast_instance_f64 ;

arm_status arm_rfft_fast_init_32_f64( arm_rfft_fast_instance_f64 * S );
arm_status arm_rfft_fast_init_64_f64( arm_rfft_fast_instance_f64 * S );
arm_status arm_rfft_fast_init_128_f64( arm_rfft_fast_instance_f64 * S );
arm_status arm_rfft_fast_init_256_f64( arm_rfft_fast_instance_f64 * S );
arm_status arm_rfft_fast_init_512_f64( arm_rfft_fast_instance_f64 * S );
arm_status arm_rfft_fast_init_1024_f64( arm_rfft_fast_instance_f64 * S );
arm_status arm_rfft_fast_init_2048_f64( arm_rfft_fast_instance_f64 * S );
arm_status arm_rfft_fast_init_4096_f64( arm_rfft_fast_instance_f64 * S );

arm_status arm_rfft_fast_init_f64 (
         arm_rfft_fast_instance_f64 * S,
         uint16_t fftLen);


void arm_rfft_fast_f64(
    arm_rfft_fast_instance_f64 * S,
    float64_t * p, float64_t * pOut,
    uint8_t ifftFlag);


  /**
   * @brief Instance structure for the floating-point RFFT/RIFFT function.
   */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
  typedef struct
  {
    uint32_t nfft;
    const float32_t *r_twiddles;
    const uint32_t *r_factors;
    const float32_t *r_twiddles_backward;
    const float32_t *r_twiddles_neon;
    const float32_t *r_twiddles_neon_backward;
    const uint32_t *r_factors_neon;
    const float32_t *r_super_twiddles_neon;
  } arm_rfft_fast_instance_f32 ;
#else
typedef struct
  {
          arm_cfft_instance_f32 Sint;      /**< Internal CFFT structure. */
          uint16_t fftLenRFFT;             /**< length of the real sequence */
    const float32_t * pTwiddleRFFT;        /**< Twiddle factors real stage  */
  } arm_rfft_fast_instance_f32 ;
#endif 

arm_status arm_rfft_fast_init_32_f32( arm_rfft_fast_instance_f32 * S );
arm_status arm_rfft_fast_init_64_f32( arm_rfft_fast_instance_f32 * S );
arm_status arm_rfft_fast_init_128_f32( arm_rfft_fast_instance_f32 * S );
arm_status arm_rfft_fast_init_256_f32( arm_rfft_fast_instance_f32 * S );
arm_status arm_rfft_fast_init_512_f32( arm_rfft_fast_instance_f32 * S );
arm_status arm_rfft_fast_init_1024_f32( arm_rfft_fast_instance_f32 * S );
arm_status arm_rfft_fast_init_2048_f32( arm_rfft_fast_instance_f32 * S );
arm_status arm_rfft_fast_init_4096_f32( arm_rfft_fast_instance_f32 * S );

                       
arm_status arm_rfft_fast_init_f32 (
         arm_rfft_fast_instance_f32 * S,
         uint16_t fftLen);

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

extern arm_rfft_fast_instance_f32 *arm_rfft_fast_init_dynamic_f32 (uint32_t fftLen);

void arm_rfft_fast_f32(
        const arm_rfft_fast_instance_f32 * S,
        const float32_t * p, 
        float32_t * pOut,
        float32_t *tmpbuf,
        uint8_t ifftFlag);
#else
  void arm_rfft_fast_f32(
        const arm_rfft_fast_instance_f32 * S,
        float32_t * p, float32_t * pOut,
        uint8_t ifftFlag);
#endif


  /**
   * @brief Instance structure for the Floating-point MFCC function.
   */
typedef struct
  {
     const float32_t *dctCoefs; /**< Internal DCT coefficients */
     const float32_t *filterCoefs; /**< Internal Mel filter coefficients */ 
     const float32_t *windowCoefs; /**< Windowing coefficients */ 
     const uint32_t *filterPos; /**< Internal Mel filter positions in spectrum */ 
     const uint32_t *filterLengths; /**< Internal Mel filter  lengths */ 
     uint32_t fftLen; /**< FFT length */
     uint32_t nbMelFilters; /**< Number of Mel filters */
     uint32_t nbDctOutputs; /**< Number of DCT outputs */
#if defined(ARM_MFCC_USE_CFFT)
     /* Implementation of the MFCC is using a CFFT */
     arm_cfft_instance_f32 cfft; /**< Internal CFFT instance */
#else
     /* Implementation of the MFCC is using a RFFT (default) */
     arm_rfft_fast_instance_f32 rfft;
#endif
  } arm_mfcc_instance_f32 ;

arm_status arm_mfcc_init_32_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_64_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_128_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_256_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_512_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_1024_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_2048_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_4096_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );

arm_status arm_mfcc_init_f32(
  arm_mfcc_instance_f32 * S,
  uint32_t fftLen,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const float32_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const float32_t *filterCoefs,
  const float32_t *windowCoefs
  );


/**
  @brief         MFCC F32
  @param[in]    S       points to the mfcc instance structure
  @param[in]     pSrc points to the input samples
  @param[out]     pDst  points to the output MFCC values
  @param[inout]     pTmp  points to a temporary buffer of complex
 */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
void arm_mfcc_f32(
  const arm_mfcc_instance_f32 * S,
  float32_t *pSrc,
  float32_t *pDst,
  float32_t *pTmp,
  float32_t *pTmp2
  );
#else
  void arm_mfcc_f32(
  const arm_mfcc_instance_f32 * S,
  float32_t *pSrc,
  float32_t *pDst,
  float32_t *pTmp
  );
#endif

 /**
   * @brief Instance structure for the Q31 MFCC function.
   */
typedef struct
  {
     const q31_t *dctCoefs; /**< Internal DCT coefficients */
     const q31_t *filterCoefs; /**< Internal Mel filter coefficients */ 
     const q31_t *windowCoefs; /**< Windowing coefficients */ 
     const uint32_t *filterPos; /**< Internal Mel filter positions in spectrum */ 
     const uint32_t *filterLengths; /**< Internal Mel filter  lengths */ 
     uint32_t fftLen; /**< FFT length */
     uint32_t nbMelFilters; /**< Number of Mel filters */
     uint32_t nbDctOutputs; /**< Number of DCT outputs */
#if defined(ARM_MFCC_USE_CFFT)
     /* Implementation of the MFCC is using a CFFT */
     arm_cfft_instance_q31 cfft; /**< Internal CFFT instance */
#else
     /* Implementation of the MFCC is using a RFFT (default) */
     arm_rfft_instance_q31 rfft;
#endif
  } arm_mfcc_instance_q31 ;

arm_status arm_mfcc_init_32_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_64_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_128_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_256_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_512_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_1024_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_2048_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_4096_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );

arm_status arm_mfcc_init_q31(
  arm_mfcc_instance_q31 * S,
  uint32_t fftLen,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q31_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q31_t *filterCoefs,
  const q31_t *windowCoefs
  );


/**
  @brief         MFCC Q31
  @param[in]    S       points to the mfcc instance structure
  @param[in]     pSrc points to the input samples
  @param[out]     pDst  points to the output MFCC values
  @param[inout]     pTmp  points to a temporary buffer of complex
  @return        error status
 */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
  arm_status arm_mfcc_q31(
  const arm_mfcc_instance_q31 * S,
  q31_t *pSrc,
  q31_t *pDst,
  q31_t *pTmp,
  q31_t *pTmp2
  );
#else
  arm_status arm_mfcc_q31(
  const arm_mfcc_instance_q31 * S,
  q31_t *pSrc,
  q31_t *pDst,
  q31_t *pTmp
  );
#endif

 /**
   * @brief Instance structure for the Q15 MFCC function.
   */
typedef struct
  {
     const q15_t *dctCoefs; /**< Internal DCT coefficients */
     const q15_t *filterCoefs; /**< Internal Mel filter coefficients */ 
     const q15_t *windowCoefs; /**< Windowing coefficients */ 
     const uint32_t *filterPos; /**< Internal Mel filter positions in spectrum */ 
     const uint32_t *filterLengths; /**< Internal Mel filter  lengths */ 
     uint32_t fftLen; /**< FFT length */
     uint32_t nbMelFilters; /**< Number of Mel filters */
     uint32_t nbDctOutputs; /**< Number of DCT outputs */
#if defined(ARM_MFCC_USE_CFFT)
     /* Implementation of the MFCC is using a CFFT */
     arm_cfft_instance_q15 cfft; /**< Internal CFFT instance */
#else
     /* Implementation of the MFCC is using a RFFT (default) */
     arm_rfft_instance_q15 rfft;
#endif
  } arm_mfcc_instance_q15 ;

arm_status arm_mfcc_init_32_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_64_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_128_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_256_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_512_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_1024_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_2048_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_4096_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );

arm_status arm_mfcc_init_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t fftLen,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  );


/**
  @brief         MFCC Q15
  @param[in]    S       points to the mfcc instance structure
  @param[in]     pSrc points to the input samples
  @param[out]     pDst  points to the output MFCC values in q8.7 format
  @param[inout]     pTmp  points to a temporary buffer of complex
  @return        error status
 */
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
  arm_status arm_mfcc_q15(
  const arm_mfcc_instance_q15 * S,
  q15_t *pSrc,
  q15_t *pDst,
  q31_t *pTmp,
  q15_t *pTmp2
  );
#else
  arm_status arm_mfcc_q15(
  const arm_mfcc_instance_q15 * S,
  q15_t *pSrc,
  q15_t *pDst,
  q31_t *pTmp
  );
#endif

/**
  @brief Calculate required length for the temporary buffer
  @param[in] arch Target architecture identification
  @param[in] dt Data type of the input data
  @param[in] nb_samples Number of samples in the input data
  @param[in] buf_id Identification for the temporary buffer
  @return Length in datatype elements (real numbers) for the temporary buffer

  @note 0 means not applicable (temporary buffer not needed)
  @note -1 means error : configuration not supported
*/
extern int32_t arm_cfft_tmp_buffer_size(arm_math_target_arch arch,
                                         arm_math_datatype dt,
                                         uint32_t nb_samples,
                                         uint32_t buf_id);

/**                                      
  @brief Calculate required length for the output buffer
  @param[in] arch Target architecture identification
  @param[in] dt Data type of the input data
  @param[in] nb_samples Number of samples in the input data
  @return Length in datatype elements (real numbers) for the temporary buffer

  @note 0 means not applicable (temporary buffer not needed)
  @note -1 means error : configuration not supported
*/
extern int32_t arm_cfft_output_buffer_size(arm_math_target_arch arch,
                                            arm_math_datatype dt,
                                            uint32_t nb_samples);

/**
  @brief Calculate required length for the output buffer
  @param[in] arch Target architecture identification
  @param[in] dt Data type of the input data
  @param[in] nb_samples Number of samples in the input data
  @return Length in datatype elements (real numbers) for the temporary buffer

  @note 0 means not applicable (temporary buffer not needed)
  @note -1 means error : configuration not supported
*/
extern int32_t arm_cifft_output_buffer_size(arm_math_target_arch arch,
                                             arm_math_datatype dt,
                                             uint32_t nb_samples);

/**
   @brief Calculate required length for the temporary buffer for both RFFT and RIFFT
   @param[in] arch Target architecture identification
   @param[in] dt Data type of the input data
   @param[in] nb_samples Number of samples in the input data
   @param[in] buf_id Identification for the temporary buffer
   @return Length in datatype elements (real numbers) for the temporary buffer

   @note 0 means not applicable (temporary buffer not needed)
   @note -1 means error : configuration not supported
*/
extern int32_t arm_rfft_tmp_buffer_size(arm_math_target_arch arch,
                                         arm_math_datatype dt,
                                         uint32_t nb_samples,
                                         uint32_t buf_id);

/**
   @brief Calculate required length for the output buffer
   @param[in] arch Target architecture identification
   @param[in] dt Data type of the input data
   @param[in] nb_samples Number of samples in the input data
   @return Length in datatype elements (real numbers) for the output buffer

   @note 0 means not applicable (temporary buffer not needed)
   @note -1 means error : configuration not supported
*/
extern int32_t arm_rfft_output_buffer_size(arm_math_target_arch arch,
                                            arm_math_datatype dt,
                                            uint32_t nb_samples);


/** 
 * @brief Calculate required length for the input buffer
 * @param[in] arch Target architecture identification
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples RFFT length in samples
 * @param[in] buf_id Identification for the temporary buffer
 * @return Length in datatype elements (real numbers) for the input buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
extern int32_t arm_rifft_input_buffer_size(arm_math_target_arch arch,
                                            arm_math_datatype dt,
                                            uint32_t nb_samples);

/**
   @brief Calculate required length for the temporary buffer
   @param[in] arch Target architecture identification
   @param[in] dt Data type of the input data
   @param[in] nb_samples Number of samples in the input data
   @param[in] buf_id Identification for the temporary buffer
   @param[in] use_cfft 1 if implementastion uses CFFT, 0 if RFFT
   @return Length in datatype elements (real numbers) for the temporary buffer

   @note 0 means not applicable (temporary buffer not needed)
   @note -1 means error : configuration not supported
*/
extern int32_t arm_mfcc_tmp_buffer_size(arm_math_target_arch arch,
                                         arm_math_datatype dt,
                                         uint32_t nb_samples,
                                         uint32_t buf_id,
                                         uint32_t use_cfft);

#ifdef   __cplusplus
}
#endif

#endif /* ifndef _TRANSFORM_FUNCTIONS_H_ */
