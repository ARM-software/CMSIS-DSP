// -*- C++ -*-

#pragma once 


#ifdef   __cplusplus
extern "C"
{
#endif

/* Compiler specific diagnostic adjustment */
#if   defined ( __CC_ARM )

#elif defined ( __ARMCC_VERSION ) && ( __ARMCC_VERSION >= 6010050 )

#elif defined ( __APPLE_CC__ )
  #pragma GCC diagnostic ignored "-Wold-style-cast"

#elif defined(__clang__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wsign-conversion"
  #pragma GCC diagnostic ignored "-Wconversion"
  #pragma GCC diagnostic ignored "-Wunused-parameter"

#elif defined ( __GNUC__ )
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wsign-conversion"
  #pragma GCC diagnostic ignored "-Wconversion"
  #pragma GCC diagnostic ignored "-Wunused-parameter"
  #define GCC_COMPILER

#elif defined ( __ICCARM__ )

#elif defined ( __TI_ARM__ )

#elif defined ( __CSMC__ )

#elif defined ( __TASKING__ )

#elif defined ( _MSC_VER )

#else
  #error Unknown compiler
#endif


/* Included for intrinsics definitions */
#if defined (_MSC_VER ) 
#include <stdint.h>
#define __STATIC_FORCEINLINE static __forceinline
#define __STATIC_INLINE static __inline
#define __ALIGNED(x) __declspec(align(x))
#define __WEAK
#elif defined ( __APPLE_CC__ )
#include <stdint.h>
#define  __ALIGNED(x) __attribute__((aligned(x)))
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline)) 
#define __STATIC_INLINE static inline
#define __WEAK
#elif defined (__GNUC_PYTHON__)
#include <stdint.h>
#define  __ALIGNED(x) __attribute__((aligned(x)))
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline)) 
#define __STATIC_INLINE static inline
#define __WEAK
#else
#include "cmsis_compiler.h"
#endif



#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

/* evaluate ARM DSP feature */
#if (defined (__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1))
  #define ARM_MATH_DSP                   1
#endif

#if defined(ARM_MATH_NEON)
  #if defined(_MSC_VER) && defined(_M_ARM64EC)
    #include <arm64_neon.h>
  #else
    #include <arm_neon.h>
  #endif
  #if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC) && __ARM_FEATURE_FP16_VECTOR_ARITHMETIC
    #if !defined(ARM_MATH_NEON_FLOAT16)
      #define ARM_MATH_NEON_FLOAT16
    #endif
  #endif
#endif

#if !defined(ARM_MATH_AUTOVECTORIZE)


#if defined(__ARM_FEATURE_MVE)
#if __ARM_FEATURE_MVE
  #if !defined(ARM_MATH_MVEI)
    #define ARM_MATH_MVEI
  #endif
#endif

#if (__ARM_FEATURE_MVE & 2)
  #if !defined(ARM_MATH_MVEF)
    #define ARM_MATH_MVEF
  #endif
  #if !defined(ARM_MATH_MVE_FLOAT16)
       #define ARM_MATH_MVE_FLOAT16
  #endif
#endif

#endif /*defined(__ARM_FEATURE_MVE)*/
#endif /*!defined(ARM_MATH_AUTOVECTORIZE)*/


#if defined (ARM_MATH_HELIUM)
  #if !defined(ARM_MATH_MVEF)
    #define ARM_MATH_MVEF
  #endif

  #if !defined(ARM_MATH_MVEI)
    #define ARM_MATH_MVEI
  #endif

  #if !defined(ARM_MATH_MVE_FLOAT16)
       #define ARM_MATH_MVE_FLOAT16
  #endif
#endif



#if   defined ( __CC_ARM )
  /* Enter low optimization region - place directly above function definition */
  #if defined( __ARM_ARCH_7EM__ )
    #define LOW_OPTIMIZATION_ENTER \
       _Pragma ("push")         \
       _Pragma ("O1")
  #else
    #define LOW_OPTIMIZATION_ENTER
  #endif

  /* Exit low optimization region - place directly after end of function definition */
  #if defined ( __ARM_ARCH_7EM__ )
    #define LOW_OPTIMIZATION_EXIT \
       _Pragma ("pop")
  #else
    #define LOW_OPTIMIZATION_EXIT
  #endif

  /* Enter low optimization region - place directly above function definition */
  #define IAR_ONLY_LOW_OPTIMIZATION_ENTER

  /* Exit low optimization region - place directly after end of function definition */
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT

#elif defined (__ARMCC_VERSION ) && ( __ARMCC_VERSION >= 6010050 )
  #define LOW_OPTIMIZATION_ENTER
  #define LOW_OPTIMIZATION_EXIT
  #define IAR_ONLY_LOW_OPTIMIZATION_ENTER
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT
  
#elif defined ( __APPLE_CC__ )
  #define LOW_OPTIMIZATION_ENTER
  #define LOW_OPTIMIZATION_EXIT
  #define IAR_ONLY_LOW_OPTIMIZATION_ENTER
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT

#elif defined ( __GNUC__ )
  #define LOW_OPTIMIZATION_ENTER \
       __attribute__(( optimize("-O1") ))
  #define LOW_OPTIMIZATION_EXIT
  #define IAR_ONLY_LOW_OPTIMIZATION_ENTER
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT

#elif defined ( __ICCARM__ )
  /* Enter low optimization region - place directly above function definition */
  #if defined ( __ARM_ARCH_7EM__ )
    #define LOW_OPTIMIZATION_ENTER \
       _Pragma ("optimize=low")
  #else
    #define LOW_OPTIMIZATION_ENTER
  #endif

  /* Exit low optimization region - place directly after end of function definition */
  #define LOW_OPTIMIZATION_EXIT

  /* Enter low optimization region - place directly above function definition */
  #if defined ( __ARM_ARCH_7EM__ )
    #define IAR_ONLY_LOW_OPTIMIZATION_ENTER \
       _Pragma ("optimize=low")
  #else
    #define IAR_ONLY_LOW_OPTIMIZATION_ENTER
  #endif

  /* Exit low optimization region - place directly after end of function definition */
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT

#elif defined ( __TI_ARM__ )
  #define LOW_OPTIMIZATION_ENTER
  #define LOW_OPTIMIZATION_EXIT
  #define IAR_ONLY_LOW_OPTIMIZATION_ENTER
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT

#elif defined ( __CSMC__ )
  #define LOW_OPTIMIZATION_ENTER
  #define LOW_OPTIMIZATION_EXIT
  #define IAR_ONLY_LOW_OPTIMIZATION_ENTER
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT

#elif defined ( __TASKING__ )
  #define LOW_OPTIMIZATION_ENTER
  #define LOW_OPTIMIZATION_EXIT
  #define IAR_ONLY_LOW_OPTIMIZATION_ENTER
  #define IAR_ONLY_LOW_OPTIMIZATION_EXIT
       
#elif defined ( _MSC_VER ) || defined(__GNUC_PYTHON__)
      #define LOW_OPTIMIZATION_ENTER
      #define LOW_OPTIMIZATION_EXIT
      #define IAR_ONLY_LOW_OPTIMIZATION_ENTER 
      #define IAR_ONLY_LOW_OPTIMIZATION_EXIT
#endif



/* Compiler specific diagnostic adjustment */
#if   defined ( __CC_ARM )

#elif defined ( __ARMCC_VERSION ) && ( __ARMCC_VERSION >= 6010050 )

#elif defined ( __APPLE_CC__ )

#elif defined ( __GNUC__ )
#pragma GCC diagnostic pop

#elif defined ( __ICCARM__ )

#elif defined ( __TI_ARM__ )

#elif defined ( __CSMC__ )

#elif defined ( __TASKING__ )

#elif defined ( _MSC_VER )

#else
  #error Unknown compiler
#endif

#ifdef   __cplusplus
}
#endif

#if defined(__ARM_FEATURE_MVE) && __ARM_FEATURE_MVE
#include <arm_mve.h>
#endif

#if !(__ARM_FEATURE_MVE & 2)
  #if !defined(DISABLEFLOAT16)
    #if defined(__ARM_FP16_FORMAT_IEEE) || defined(__ARM_FP16_FORMAT_ALTERNATIVE)
      typedef __fp16 float16_t;
      #define ARM_FLOAT16_SUPPORTED
    #endif
  #endif
#else
  /* When Vector float16, this flag is always defined and can't be disabled */
  #define ARM_FLOAT16_SUPPORTED
#endif

#if defined(ARM_FLOAT16_SUPPORTED)

#if defined(__ICCARM__)

#define F16INFINITY   ((float16_t) INFINITY)

#else

#define F16INFINITY ((float16_t)__builtin_inf())

#endif

#endif




