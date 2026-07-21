/*
 *  Copyright 2013-16 ARM Limited and Contributors.
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
 * NE10 Library : inc/NE10_macros.h
 */

/** NE10 defines a number of macros for use in its function signatures.
 *  The macros are defined within this header file.
 */

#ifndef CMSIS_NE10_MACROS_H
#define CMSIS_NE10_MACROS_H


#ifdef __cplusplus
#include <cstdlib>
extern "C" {
#else
#include <stdlib.h>
#endif

/////////////////////////////////////////////////////////
// constant values that are used across the library
/////////////////////////////////////////////////////////

#define NE10_PI (double)(3.1415926535897932384626433832795)

/////////////////////////////////////////////////////////
// some external macro definitions to be exposed to the users
/////////////////////////////////////////////////////////


#define NE10_MIN(a,b) ((a)>(b)?(b):(a))
#define NE10_MAX(a,b) ((a)<(b)?(b):(a))

#define NE10_BYTE_ALIGNMENT(address, alignment) \
    do { \
        (address) = (((address) + ((alignment) - 1)) & ~ ((alignment) - 1)); \
    }while (0)

/////////////////////////////////////////////////////////
// macro definitions for float to fixed point
/////////////////////////////////////////////////////////
#define NE10_F2I16_MAX         32767
#define NE10_F2I16_SHIFT       15
#define NE10_F2I16_SAMPPROD    ne10_int32_t
#define NE10_F2I16_OP(x)       (ne10_int16_t)((x)*NE10_F2I16_MAX + 0.5f)
#define NE10_F2I16_SROUND(x)   (ne10_int16_t)((((x)<<1)+(1<<NE10_F2I16_SHIFT))>>16)
#define NE10_F2I16_SMUL(a,b)   ((NE10_F2I16_SAMPPROD)(a)*(b))
#define NE10_F2I16_FIXDIV(c,div) \
    do {    ((c).r) = ( ( ((c).r)/div) );  \
        ((c).i) = ( ( ((c).i)/div) ); }while (0)

#define NE10_F2I32_MAX         2147483647
#define NE10_F2I32_SHIFT       31
#define NE10_F2I32_SAMPPROD    ne10_int64_t
#define NE10_F2I32_OP(x)       (ne10_int32_t)((x)*NE10_F2I32_MAX + 0.5f)
#define NE10_F2I32_SROUND(x)   (ne10_int32_t) ((x)>>NE10_F2I32_SHIFT)
#define NE10_F2I32_SMUL(a,b)    ((NE10_F2I32_SAMPPROD)(a)*(b))
#define NE10_F2I32_FIXDIV(c,div) \
    do {    ((c).r) = ( ( ((c).r)/div) );  \
        ((c).i) = ( ( ((c).i)/div) ); }while (0)

/*
 * FFT Algorithm Flags
 *
 * These are used within Ne10 to decide, after factoring an FFT into stages, what
 * FFT algorithm should be used.
 *
 * - NE10_FFT_ALG_DEFAULT is a mixed radix 2/4 algorithm.
 * Non power of 2 are not supported by CMSIS-DSP
 */
#define NE10_FFT_ALG_DEFAULT  0
#define NE10_FFT_ALG_ANY      1

/*
 * FFT Factor Flags
 *
 * These are used within Ne10 to decide how an input FFT size should be factored into
 * stages (i.e. what radices should be used).
 *
 * - NE10_FACTOR_DEFAULT factors into 2, 3, 4, 5.
 * - NE10_FACTOR_EIGHT_FIRST_STAGE is NE10_FACTOR_DEFAULT with the extended ability to
 *   have a radix-8 initial stage.
 * - NE10_FACTOR_EIGHT factors into 2, 3, 4, 5, 8.
 */
#define NE10_FACTOR_DEFAULT             0
#define NE10_FACTOR_EIGHT_FIRST_STAGE   1
#define NE10_FACTOR_EIGHT               2

#define NE10_MALLOC malloc
#define NE10_FREE(p) \
    do { \
        free(p); \
        p = 0; \
    }while(0)

#define NE10_FFT_PARA_LEVEL 4

#define NE10_FFT_BYTE_ALIGNMENT 8

#define NE10_BYTE_ALIGNMENT(address, alignment) \
    do { \
        (address) = (((address) + ((alignment) - 1)) & ~ ((alignment) - 1)); \
    }while (0)


#ifdef __cplusplus
}
#endif

#endif
