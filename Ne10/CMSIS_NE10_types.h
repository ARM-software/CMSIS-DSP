/*
 *  Copyright 2011-16 ARM Limited and Contributors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ARM Limited nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * NE10 Library : CMSIS_NE10_types.h
 */

/*
 * Renamed to CMSIS_NE10_types.h to avoid any conflict
 * with original Ne10 library that could be needed for other
 * features not provided by CMSIS-DSP.
 * 
 * CMSIS-DSP is only using a subset of NE10_types.h
 *
 */

/** NE10 defines a number of types for use in its function signatures.
 *  The types are defined within this header file.
 */

#ifndef CMSIS_NE10_TYPES_H
#define CMSIS_NE10_TYPES_H

#ifdef __cplusplus
#include <cassert>
#include <cstdint>
#else
#include <assert.h>
#include <stdint.h>
#endif

#include "arm_math_types_f16.h"

/**
 * @TODO Move the definition of NE10_UNROLL_LEVEL to cmake configuration files.
 * Macro NE10_UNROLL_LEVEL controls algorithm of FFT functions.
 * When NE10_UNROLL_LEVEL == 0, complex FFT performs radix-4 x2 per loop.
 * When NE10_UNROLL_LEVEL == 1, complex FFT performs radix-4 x4 per loop.
 */
//#if !defined(NE10_UNROLL_LEVEL)
//#if defined(__arm__)
//#define NE10_UNROLL_LEVEL 0
//#elif defined(__aarch64__)
//#define NE10_UNROLL_LEVEL 1
//#else
//#define NE10_UNROLL_LEVEL 0
//#endif
//#endif

/////////////////////////////////////////////////////////
// constant values that are used across the library
/////////////////////////////////////////////////////////
#define NE10_OK 0
#define NE10_ERR -1

/////////////////////////////////////////////////////////
// some external definitions to be exposed to the users
/////////////////////////////////////////////////////////

typedef int8_t   ne10_int8_t;
typedef uint8_t  ne10_uint8_t;
typedef int16_t  ne10_int16_t;
typedef uint16_t ne10_uint16_t;
typedef int32_t  ne10_int32_t;
typedef uint32_t ne10_uint32_t;
typedef int64_t  ne10_int64_t;
typedef uint64_t ne10_uint64_t;
#if defined(ARM_FLOAT16_SUPPORTED)
typedef float16_t    ne10_float16_t;
#endif
typedef float    ne10_float32_t;
typedef double   ne10_float64_t;
typedef int      ne10_result_t;     // resulting [error-]code



/////////////////////////////////////////////////////////
// definitions for fft
/////////////////////////////////////////////////////////

/**
 * @brief Structure for the floating point FFT function.
 */
#define NE10_MAXFACTORS             32
typedef struct
{
    ne10_float32_t r;
    ne10_float32_t i;
} ne10_fft_cpx_float32_t;

#if defined(ARM_FLOAT16_SUPPORTED)
typedef struct
{
    ne10_float16_t r;
    ne10_float16_t i;
} ne10_fft_cpx_float16_t;
#endif 

 

/**
 * @brief Structure for the 16-bit fixed point FFT function.
 */
typedef struct
{
    ne10_int16_t r;
    ne10_int16_t i;
} ne10_fft_cpx_int16_t;



/**
 * @brief Structure for the 32-bit fixed point FFT function.
 */
typedef struct
{
    ne10_int32_t r;
    ne10_int32_t i;
} ne10_fft_cpx_int32_t;

#endif
