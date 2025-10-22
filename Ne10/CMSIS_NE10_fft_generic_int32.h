/*
 *  Copyright 2015-16 ARM Limited and Contributors.
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
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* license of Kiss FFT */
/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * NE10 Library : dsp/NE10_fft_generic_int32.h
 */

#ifndef CMSIS_NE10_FFT_GENERIC_INT32_H
#define CMSIS_NE10_FFT_GENERIC_INT32_H

#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_macros.h"
#include "CMSIS_NE10_fft_cplx_ops.h"
#include "CMSIS_NE10_fft.h"
#include "CMSIS_NE10_fft_common_variables.h"

#define NE10_CPX_MUL_S32(Z,A,B) \
    do { \
        ne10_int32_t ARBR = ((NE10_F2I32_SAMPPROD) A.r * B.r) >> 31; \
        ne10_int32_t ARBI = ((NE10_F2I32_SAMPPROD) A.r * B.i) >> 31; \
        ne10_int32_t AIBR = ((NE10_F2I32_SAMPPROD) A.i * B.r) >> 31; \
        ne10_int32_t AIBI = ((NE10_F2I32_SAMPPROD) A.i * B.i) >> 31; \
        Z.r = ARBR - AIBI; \
        Z.i = ARBI + AIBR; \
    } while (0)

#define NE10_S_MUL_S32(A,S) (((NE10_F2I32_SAMPPROD) (A) * (S)) >> 31)

/**
 * @brief Multiply input with twiddles.
 * @tparam RADIX Length of given fix-point complex array
 * @param[out] out      Output array.
 * @param[in]  in       Input array.
 * @param[in]  tw       Twiddles array.
 */

static inline void FFT_MUL_TW_2 (ne10_fft_cpx_int32_t out[2],
        const ne10_fft_cpx_int32_t in[2],
        const ne10_fft_cpx_int32_t tw[1])
{
    out[0] = in[0];
    NE10_CPX_MUL_S32 (out[1], in[1], tw[0]);
}

#define FFT_MUL_TW(RADIX,RADIXM)                                \
static inline void FFT_MUL_TW_##RADIX (ne10_fft_cpx_int32_t out[RADIX],\
        const ne10_fft_cpx_int32_t in[RADIX],                   \
        const ne10_fft_cpx_int32_t tw[RADIXM])                  \
{                                                               \
    FFT_MUL_TW_##RADIXM (out, in, tw);                          \
    NE10_CPX_MUL_S32 (out[RADIXM], in[RADIXM], tw[RADIX - 2]);  \
}

FFT_MUL_TW(3,2)
FFT_MUL_TW(4,3)
FFT_MUL_TW(5,4)

///////////////////
// FFT Kernel
// F: Forward
// C: Complex
// U: Unscaled
//////////////////
/**
 * @brief Basic fixed-point butterfly used in each stage.
 * @tparam RADIX Radix of butterfly.
 * @param[out] scratch_out      Output array.
 * @param[in]  scratch_in       Input array.
 */


/**
 * @brief Basic fixed-point Radix-2 butterfly used in each stage.
 * @param[out] scratch_out      Output array.
 * @param[in]  scratch_in       Input array.
 */
static inline void FFT_FCU_2 (ne10_fft_cpx_int32_t scratch_out[2],
        const ne10_fft_cpx_int32_t scratch_in[2])
{
    NE10_CPX_ADD (scratch_out[0], scratch_in[0], scratch_in[1]);
    NE10_CPX_SUB (scratch_out[1], scratch_in[0], scratch_in[1]);
}

/**
 * @brief Basic fixed-point radix-3 butterfly used in each stage.
 * @param[out] scratch_out      Output array.
 * @param[in]  scratch_in       Input array.
 */
static inline void FFT_FCU_3 (ne10_fft_cpx_int32_t Fout[3],
        const ne10_fft_cpx_int32_t Fin[3])
{
    ne10_fft_cpx_int32_t scratch[4];
    ne10_fft_cpx_int32_t scratch_in[3];

    scratch_in[0] = Fin[0];
    scratch_in[1] = Fin[1];
    scratch_in[2] = Fin[2];

    scratch[1] = scratch_in[1];
    scratch[2] = scratch_in[2];

    NE10_CPX_ADD (scratch[3], scratch[1], scratch[2]);
    NE10_CPX_SUB (scratch[0], scratch[1], scratch[2]);

    scratch_in[1].r = scratch_in[0].r - (scratch[3].r >> 1);
    scratch_in[1].i = scratch_in[0].i - (scratch[3].i >> 1);

    scratch[0].r = NE10_S_MUL_S32 (scratch[0].r , -TW_3I_S32);
    scratch[0].i = NE10_S_MUL_S32 (scratch[0].i , -TW_3I_S32);

    scratch_in[0].r += scratch[3].r;
    scratch_in[0].i += scratch[3].i;

    scratch_in[2].r = scratch_in[1].r + scratch[0].i;
    scratch_in[2].i = scratch_in[1].i - scratch[0].r;

    scratch_in[1].r -= scratch[0].i;
    scratch_in[1].i += scratch[0].r;

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
}

/**
 * @brief Basic fixed-point radix-4 butterfly used in each stage.
 * @param[out] scratch_out      Output array.
 * @param[in]  scratch_in       Input array.
 */
static inline void FFT_FCU_4 (ne10_fft_cpx_int32_t scratch_out[4],
        const ne10_fft_cpx_int32_t scratch_in[4])
{
    ne10_fft_cpx_int32_t scratch[4];

    NE10_CPX_ADD (scratch[0], scratch_in[0], scratch_in[2]);
    NE10_CPX_SUB (scratch[1], scratch_in[0], scratch_in[2]);
    NE10_CPX_ADD (scratch[2], scratch_in[1], scratch_in[3]);
    NE10_CPX_SUB (scratch[3], scratch_in[1], scratch_in[3]);

    NE10_CPX_SUB (scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD (scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].r = scratch[1].r + scratch[3].i;
    scratch_out[1].i = scratch[1].i - scratch[3].r;
    scratch_out[3].r = scratch[1].r - scratch[3].i;
    scratch_out[3].i = scratch[1].i + scratch[3].r;
}

/**
 * @brief Basic fixed-point radix-5 butterfly used in each stage.
 * @param[out] scratch_out      Output array.
 * @param[in]  scratch_in       Input array.
 */
static inline void FFT_FCU_5 (ne10_fft_cpx_int32_t Fout[5],
        const ne10_fft_cpx_int32_t Fin[5])
{
    ne10_fft_cpx_int32_t scratch[13], scratch_in[5];

    scratch_in[0] = Fin[0];
    scratch_in[1] = Fin[1];
    scratch_in[2] = Fin[2];
    scratch_in[3] = Fin[3];
    scratch_in[4] = Fin[4];

    scratch[0] = scratch_in[0];
    scratch[1] = scratch_in[1];
    scratch[2] = scratch_in[2];
    scratch[3] = scratch_in[3];
    scratch[4] = scratch_in[4];

    NE10_CPX_ADD (scratch[ 7], scratch[1], scratch[4]);
    NE10_CPX_SUB (scratch[10], scratch[1], scratch[4]);
    NE10_CPX_ADD (scratch[ 8], scratch[2], scratch[3]);
    NE10_CPX_SUB (scratch[ 9], scratch[2], scratch[3]);

    scratch_in[0].r += scratch[7].r + scratch[8].r;
    scratch_in[0].i += scratch[7].i + scratch[8].i;

    scratch[5].r = scratch[0].r
        + NE10_S_MUL_S32 (scratch[7].r, TW_5A_S32.r)
        + NE10_S_MUL_S32 (scratch[8].r, TW_5B_S32.r);
    scratch[5].i = scratch[0].i
        + NE10_S_MUL_S32 (scratch[7].i, TW_5A_S32.r)
        + NE10_S_MUL_S32 (scratch[8].i, TW_5B_S32.r);

    scratch[6].r = NE10_S_MUL_S32 (scratch[10].i, TW_5A_S32.i)
        + NE10_S_MUL_S32 (scratch[9].i, TW_5B_S32.i);
    scratch[6].i = -NE10_S_MUL_S32 (scratch[10].r, TW_5A_S32.i)
        - NE10_S_MUL_S32 (scratch[9].r, TW_5B_S32.i);

    NE10_CPX_SUB (scratch_in[1], scratch[5], scratch[6]);
    NE10_CPX_ADD (scratch_in[4], scratch[5], scratch[6]);

    scratch[11].r = scratch[0].r
        + NE10_S_MUL_S32 (scratch[7].r, TW_5B_S32.r)
        + NE10_S_MUL_S32 (scratch[8].r, TW_5A_S32.r);
    scratch[11].i = scratch[0].i
        + NE10_S_MUL_S32 (scratch[7].i, TW_5B_S32.r)
        + NE10_S_MUL_S32 (scratch[8].i, TW_5A_S32.r);

    scratch[12].r = -NE10_S_MUL_S32 (scratch[10].i, TW_5B_S32.i)
        + NE10_S_MUL_S32 (scratch[9].i, TW_5A_S32.i);
    scratch[12].i = NE10_S_MUL_S32 (scratch[10].r, TW_5B_S32.i)
        - NE10_S_MUL_S32 (scratch[9].r, TW_5A_S32.i);

    NE10_CPX_ADD (scratch_in[2], scratch[11], scratch[12]);
    NE10_CPX_SUB (scratch_in[3], scratch[11], scratch[12]);

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
    Fout[3] = scratch_in[3];
    Fout[4] = scratch_in[4];
}

/**
 * @brief Conjugate a fix-point complex scalar/NEON vector.
 */


static inline void NE10_CONJ_S_SCALAR(ne10_fft_cpx_int32_t *scalar)
{
    scalar->i = -scalar->i;
}



#define NE10_CONJ_SCALAR(RADIX,RADIXM)                               \
static inline void NE10_CONJ_SCALAR_##RADIX (ne10_fft_cpx_int32_t in[RADIX])\
{                                                                    \
    NE10_CONJ_SCALAR_##RADIXM (in);                                  \
    NE10_CONJ_S_SCALAR (&in[RADIX - 1]);                              \
}

static inline void NE10_CONJ_SCALAR_1(ne10_fft_cpx_int32_t in[1])
{
    NE10_CONJ_S_SCALAR (&in[0]);
}

static inline ne10_fft_cpx_int32_t NE10_CPX_LOAD_S_SCALAR (const ne10_fft_cpx_int32_t *ptr)
{
    return *ptr;
}

static inline void NE10_CPX_STORE_S_SCALAR (ne10_fft_cpx_int32_t *Fout, const ne10_fft_cpx_int32_t in)
{
    *Fout = in;
}

/**
 * @brief Load a fixed-size array from given buffer, by given step.
 * @tparam RADIX Length of array.
 * @param[out] out      Array to which data are loaded
 * @param[in]  Fin      Pointing to buffer from which data are loaded
 * @param[in]  in_step  Step between loaded data in Fin
 */

static inline void NE10_LOAD_BY_STEP_SCALAR_1 (
        ne10_fft_cpx_int32_t out[1],
        const ne10_fft_cpx_int32_t *Fin,
        const ne10_int32_t ignored)
{
    (void)ignored;
    out[0] = NE10_CPX_LOAD_S_SCALAR (Fin);
}

#define NE10_LOAD_BY_STEP_SCALAR(RADIX,RADIXM)                                \
static inline void NE10_LOAD_BY_STEP_SCALAR_##RADIX (ne10_fft_cpx_int32_t out[RADIX],\
        const ne10_fft_cpx_int32_t *Fin,                                      \
        const ne10_int32_t in_step)                                           \
{                                                                             \
    out[0] = NE10_CPX_LOAD_S_SCALAR (Fin);                                    \
    NE10_LOAD_BY_STEP_SCALAR_##RADIXM (out + 1, Fin + in_step, in_step);      \
}

/**
 * @brief Store a fixed-size array to given buffer, by given step.
 * @tparam RADIX Length of array.
 * @param[out] Fout         Pointing to buffer to which data are stored
 * @param[in]  out          Array to from data are stored
 * @param[in]  out_step     Step between stored data in Fout
 */
#define NE10_STORE_BY_STEP_SCALAR(RADIX,RADIXM)                           \
static inline void NE10_STORE_BY_STEP_SCALAR_##RADIX (ne10_fft_cpx_int32_t *Fout, \
        const ne10_fft_cpx_int32_t in[RADIX],                              \
        const ne10_int32_t out_step)                                       \
{                                                                          \
    NE10_CPX_STORE_S_SCALAR (Fout, in[0]);                                 \
    NE10_STORE_BY_STEP_SCALAR_##RADIXM (Fout + out_step, in + 1, out_step);\
}

static inline void NE10_STORE_BY_STEP_SCALAR_1 (
        ne10_fft_cpx_int32_t *Fout,
        const ne10_fft_cpx_int32_t in[1],
        const ne10_int32_t ignored)
{
    (void)ignored;
    Fout[0] = in[0];
}

NE10_STORE_BY_STEP_SCALAR(2,1)
NE10_STORE_BY_STEP_SCALAR(3,2)
NE10_STORE_BY_STEP_SCALAR(4,3)
NE10_STORE_BY_STEP_SCALAR(5,4)
/**
 * @brief Scale a fixed-size array by given divider.
 * @tparam          RADIX        Length of array.
 * @param[out]      out          Array whose elements are scaled
 * @param[in]       scaling      Divider by which array is divided
 */
#define NE10_SCALED(RADIX,RADIXM)                        \
static inline void NE10_SCALED_##RADIX (ne10_fft_cpx_int32_t out[RADIX],\
        const ne10_int32_t scaling)                      \
{                                                        \
    NE10_F2I32_FIXDIV (out[0], scaling);                 \
    NE10_SCALED_##RADIXM (out + 1, scaling);             \
}

static inline void NE10_SCALED_1 (ne10_fft_cpx_int32_t out[1],
        const ne10_int32_t scaling)
{
    NE10_F2I32_FIXDIV (out[0], scaling);
}

NE10_SCALED(2,1)
NE10_SCALED(3,2)
NE10_SCALED(4,3)
NE10_SCALED(5,4)

#endif // NE10_FFT_GENERIC_INT32_H
