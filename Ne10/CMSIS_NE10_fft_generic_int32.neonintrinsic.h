/*
 *  Copyright 2015-16 ARM Limited
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
 * NE10 Library : dsp/NE10_fft_generic_int32.neonintrisic.h
 *
 * This file must be compiled by C++ toolchain because some functions are
 * written as template functions to make it easier for compiler to
 * reduce branch jump.
 */

#ifndef CMSIS_NE10_FFT_GENERIC_INT32_NEONINTRINSIC_H
#define CMSIS_NE10_FFT_GENERIC_INT32_NEONINTRINSIC_H

#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_macros.h"


typedef int32x4x2_t CPLX;
typedef int32x4_t   REAL;

#include "CMSIS_NE10_fft.neonintrinsic.h"
#include "CMSIS_NE10_fft_generic_int32.h"


#define NE10_REAL_DUP_NEON_S32 vdupq_n_s32

#define NE10_CPLX_LOAD(PTR) vld2q_s32 ((ne10_int32_t*) (PTR))
#define NE10_CPLX_STORE(PTR,OUT) \
    do { \
        vst2q_s32 ((ne10_int32_t*) (PTR), OUT); \
    } while (0)


static inline CPLX NE10_CPX_LOAD_S(const CPLX *ptr)
{
    return NE10_CPLX_LOAD(ptr);
}

static inline void NE10_CPX_STORE_S (CPLX *ptr, const CPLX out)
{
    NE10_CPLX_STORE (ptr, out);
}

static inline void NE10_LOAD_BY_STEP_1 (CPLX out[1],
        const CPLX *Fin,
        const ne10_int32_t ignored)
{
    (void)ignored;
    out[0] = NE10_CPX_LOAD_S (Fin);
}


static inline void NE10_STORE_BY_STEP_1 (CPLX *Fout,
        const CPLX out[1],
        const ne10_int32_t ignored)
{
    (void)ignored;
    NE10_CPX_STORE_S (Fout, out[0]);
}

#define NE10_STORE_BY_STEP(RADIX,RADIXM)                            \
static inline void NE10_STORE_BY_STEP_##RADIX (CPLX *Fout,                         \
        const CPLX in[RADIX],                                       \
        const ne10_int32_t out_step)                                \
{                                                                   \
    NE10_CPX_STORE_S (Fout, in[0]);                                 \
    NE10_STORE_BY_STEP_##RADIXM (Fout + out_step, in + 1, out_step);\
}

NE10_STORE_BY_STEP(2,1)
NE10_STORE_BY_STEP(3,2)
NE10_STORE_BY_STEP(4,3)
NE10_STORE_BY_STEP(5,4)


#define NE10_LOAD_BY_STEP(RADIX,RADIXM)                          \
static inline void NE10_LOAD_BY_STEP_##RADIX (CPLX out[RADIX],          \
        const CPLX *Fin,                                         \
        const ne10_int32_t in_step)                              \
{                                                                \
    out[0] = NE10_CPX_LOAD_S (Fin);                              \
    NE10_LOAD_BY_STEP_##RADIXM (out + 1, Fin + in_step, in_step);\
}

NE10_LOAD_BY_STEP(2,1)
NE10_LOAD_BY_STEP(3,2)
NE10_LOAD_BY_STEP(4,3)
NE10_LOAD_BY_STEP(5,4)



static inline REAL NE10_S_MUL_NEON_S32 (const REAL vec,
        const ne10_int32_t scalar)
{
    REAL scalar_neon = NE10_REAL_DUP_NEON_S32 (scalar);
    REAL result = vqrdmulhq_s32 (scalar_neon, vec);
    return result;
}

static inline void NE10_CPX_MUL_NEON_S32 (CPLX *result, const CPLX A, const CPLX B)
{
        REAL ARBR = vqrdmulhq_s32 (A.val[0], B.val[0]);
        REAL ARBI = vqrdmulhq_s32 (A.val[0], B.val[1]);
        REAL AIBR = vqrdmulhq_s32 (A.val[1], B.val[0]);
        REAL AIBI = vqrdmulhq_s32 (A.val[1], B.val[1]);
        result->val[0] = vsubq_s32(ARBR , AIBI);
        result->val[1] = vaddq_s32(ARBI , AIBR);
}

#define NE10_LOAD_TW_AND_MUL(RADIX,RADIXM)                                           \
static inline void NE10_LOAD_TW_AND_MUL_##RADIX (CPLX scratch_in[RADIX],                    \
        const ne10_fft_cpx_int32_t *ptr_in,                                          \
        const ne10_int32_t step)                                                     \
{                                                                                    \
    CPLX scratch_tw;                                                                 \
    int32x2_t d2_tmp = vld1_s32 ((ne10_int32_t *)(ptr_in + (RADIX - 2) * step));     \
                                                                                     \
    scratch_tw.val[0] = NE10_REAL_DUP_NEON_S32 (vget_lane_s32(d2_tmp,0));                          \
    scratch_tw.val[1] = NE10_REAL_DUP_NEON_S32 (vget_lane_s32(d2_tmp,1));                          \
    NE10_CPX_MUL_NEON_S32 (&scratch_in[RADIX - 1], scratch_in[RADIX - 1], scratch_tw);\
                                                                                     \
    NE10_LOAD_TW_AND_MUL_##RADIXM (scratch_in, ptr_in, step);                        \
}

static inline void NE10_LOAD_TW_AND_MUL_1 (CPLX ignoredc[1],
        const ne10_fft_cpx_int32_t *ignoreda,
        const ne10_int32_t ignoredb)
{
    (void)ignoreda;
    (void)ignoredb;
    (void)ignoredc;
}

NE10_LOAD_TW_AND_MUL(2,1)
NE10_LOAD_TW_AND_MUL(3,2)
NE10_LOAD_TW_AND_MUL(4,3)
NE10_LOAD_TW_AND_MUL(5,4)

////////////////
// Conj inplace.
////////////////
static inline void NE10_CONJ_S (CPLX *cplx)
{
    cplx->val[1] = vnegq_s32(cplx->val[1]);
}

static inline void NE10_CONJ_1 (CPLX in[1])
{
    NE10_CONJ_S (&in[0]);
}

/**
 * @brief Conjugate a fix-point complex array.
 * @tparam RADIX Length of given fix-point complex array
 * @param[out] in Given array
 */
#define NE10_CONJ(RADIX,RADIXM)               \
static inline void NE10_CONJ_##RADIX (CPLX in[RADIX])\
{                                             \
    NE10_CONJ_##RADIXM (in);                  \
    NE10_CONJ_S (&in[RADIX - 1]);              \
}

NE10_CONJ(2,1)
NE10_CONJ(3,2)
NE10_CONJ(4,3)
NE10_CONJ(5,4)

NE10_CONJ_SCALAR(2,1)
NE10_CONJ_SCALAR(3,2)
NE10_CONJ_SCALAR(4,3)
NE10_CONJ_SCALAR(5,4)

/////////////////////////////////////////////////////////////////////////////
// Scaling
// If Macro NE10_DSP_CFFT_SCALING is not defined, these functions do nothing.
/////////////////////////////////////////////////////////////////////////////
#define NE10_FFT_SCALING(RADIX,SIZE,SIZEM)                                                                    \
static void ne10_fft_scaling_##RADIX##_##SIZE(CPLX scratch_out[RADIX]) {                                      \
        const ne10_int32_t one_by_RADIX = (ne10_int32_t) floorf(1.0f / RADIX * (float)NE10_F2I32_MAX + 0.5f); \
        scratch_out[SIZEM].val[0] = vqrdmulhq_n_s32 (scratch_out[SIZEM].val[0], one_by_RADIX);                \
        scratch_out[SIZEM].val[1] = vqrdmulhq_n_s32 (scratch_out[SIZEM].val[1], one_by_RADIX);                \
        ne10_fft_scaling_##RADIX##_##SIZEM(scratch_out);                                                      \
};

#define NE10_FFT_SCALING_SIZE1(RADIX)                                                              \
static void ne10_fft_scaling_##RADIX##_1(CPLX scratch_out[1]) {                                    \
        const ne10_int32_t one_by_RADIX =  floorf(1.0f / RADIX * (float)NE10_F2I32_MAX + 0.5f) ;   \
        scratch_out[0].val[0] = vqrdmulhq_n_s32 (scratch_out[0].val[0], one_by_RADIX);             \
        scratch_out[0].val[1] = vqrdmulhq_n_s32 (scratch_out[0].val[1], one_by_RADIX);             \
};

NE10_FFT_SCALING_SIZE1(2)
NE10_FFT_SCALING_SIZE1(3)
NE10_FFT_SCALING_SIZE1(4)
NE10_FFT_SCALING_SIZE1(5)

NE10_FFT_SCALING(2,2,1)

NE10_FFT_SCALING(3,2,1)
NE10_FFT_SCALING(3,3,2)

NE10_FFT_SCALING(4,2,1)
NE10_FFT_SCALING(4,3,2)
NE10_FFT_SCALING(4,4,3)

NE10_FFT_SCALING(5,2,1)
NE10_FFT_SCALING(5,3,2)
NE10_FFT_SCALING(5,4,3)
NE10_FFT_SCALING(5,5,4)

static inline void NE10_CPX_ADD_NEON_S32 (CPLX *result, const CPLX a, const CPLX b)
{
    result->val[0] = vaddq_s32 (a.val[0], b.val[0]);
    result->val[1] = vaddq_s32 (a.val[1], b.val[1]);
}

static inline void NE10_CPX_SUB_NEON_S32 (CPLX *result, const CPLX a, const CPLX b)
{
    result->val[0] = vsubq_s32 (a.val[0], b.val[0]);
    result->val[1] = vsubq_s32 (a.val[1], b.val[1]);
}

static inline REAL NE10_HALF (REAL src)
{
    src = vshrq_n_s32 (src, 1);
    return src;
}

///////////////////
// FFT Kernel
// F: Forward
// C: Complex
// U: Unscaled
//////////////////

static inline void NE10_FFT_FCU_NEON_S32_2 (CPLX scratch_out[2],
        const CPLX scratch_in[2])
{
    NE10_CPX_ADD_NEON_S32 (&scratch_out[0], scratch_in[0], scratch_in[1]);
    NE10_CPX_SUB_NEON_S32 (&scratch_out[1], scratch_in[0], scratch_in[1]);
}

static inline void NE10_FFT_FCU_NEON_S32_3 (CPLX Fout[3],
        const CPLX Fin[3])
{
    CPLX scratch[4];

    Fout[0] = Fin[0];
    Fout[1] = Fin[1];
    Fout[2] = Fin[2];

    scratch[1] = Fout[1];
    scratch[2] = Fout[2];

    NE10_CPX_ADD_NEON_S32 (&scratch[3], scratch[1], scratch[2]);
    NE10_CPX_SUB_NEON_S32 (&scratch[0], scratch[1], scratch[2]);

    Fout[1].val[0] = vsubq_s32(Fout[0].val[0] , NE10_HALF (scratch[3].val[0]));
    Fout[1].val[1] = vsubq_s32(Fout[0].val[1] , NE10_HALF (scratch[3].val[1]));

    scratch[0].val[0] = NE10_S_MUL_NEON_S32 (scratch[0].val[0], TW_3IN_S32);
    scratch[0].val[1] = NE10_S_MUL_NEON_S32 (scratch[0].val[1], TW_3IN_S32);

    Fout[0].val[0] = vaddq_s32(Fout[0].val[0], scratch[3].val[0]);
    Fout[0].val[1] = vaddq_s32(Fout[0].val[1], scratch[3].val[1]);

    Fout[2].val[0] = vaddq_s32(Fout[1].val[0] , scratch[0].val[1]);
    Fout[2].val[1] = vsubq_s32(Fout[1].val[1] , scratch[0].val[0]);

    Fout[1].val[0] = vsubq_s32(Fout[1].val[0],scratch[0].val[1]);
    Fout[1].val[1] = vaddq_s32(Fout[1].val[1],scratch[0].val[0]);
}

static inline void NE10_FFT_FCU_NEON_S32_4 (CPLX scratch_out[4],
        const CPLX scratch_in[4])
{
    CPLX scratch[4];

    NE10_CPX_ADD_NEON_S32 (&scratch[0], scratch_in[0], scratch_in[2]);
    NE10_CPX_SUB_NEON_S32 (&scratch[1], scratch_in[0], scratch_in[2]);
    NE10_CPX_ADD_NEON_S32 (&scratch[2], scratch_in[1], scratch_in[3]);
    NE10_CPX_SUB_NEON_S32 (&scratch[3], scratch_in[1], scratch_in[3]);

    NE10_CPX_SUB_NEON_S32 (&scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD_NEON_S32 (&scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].val[0] = vaddq_s32(scratch[1].val[0] , scratch[3].val[1]);
    scratch_out[1].val[1] = vsubq_s32(scratch[1].val[1] , scratch[3].val[0]);
    scratch_out[3].val[0] = vsubq_s32(scratch[1].val[0] , scratch[3].val[1]);
    scratch_out[3].val[1] = vaddq_s32(scratch[1].val[1] , scratch[3].val[0]);
}

static inline void NE10_FFT_FCU_NEON_S32_5 (CPLX Fout[5],
        const CPLX Fin[5])
{
    CPLX scratch[13], scratch_in[5];

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

    NE10_CPX_ADD_NEON_S32 (&scratch[ 7], scratch[1], scratch[4]);
    NE10_CPX_SUB_NEON_S32 (&scratch[10], scratch[1], scratch[4]);
    NE10_CPX_ADD_NEON_S32 (&scratch[ 8], scratch[2], scratch[3]);
    NE10_CPX_SUB_NEON_S32 (&scratch[ 9], scratch[2], scratch[3]);

    scratch_in[0].val[0] = vaddq_s32(scratch_in[0].val[0],vaddq_s32(scratch[7].val[0] , scratch[8].val[0]));
    scratch_in[0].val[1] = vaddq_s32(scratch_in[0].val[1],vaddq_s32(scratch[7].val[1] , scratch[8].val[1]));

    scratch[5].val[0] = vaddq_s32(scratch[0].val[0] , vaddq_s32(NE10_S_MUL_NEON_S32 (scratch[7].val[0], TW_5A_S32.r) , NE10_S_MUL_NEON_S32 (scratch[8].val[0], TW_5B_S32.r)));
    scratch[5].val[1] = vaddq_s32(scratch[0].val[1] , vaddq_s32(NE10_S_MUL_NEON_S32 (scratch[7].val[1], TW_5A_S32.r) , NE10_S_MUL_NEON_S32 (scratch[8].val[1], TW_5B_S32.r)));

    scratch[6].val[0] =           vaddq_s32(NE10_S_MUL_NEON_S32 (scratch[10].val[1], TW_5A_S32.i) , NE10_S_MUL_NEON_S32 (scratch[9].val[1], TW_5B_S32.i));
    scratch[6].val[1] = vnegq_s32(vsubq_s32(NE10_S_MUL_NEON_S32 (scratch[10].val[0], TW_5A_S32.i) , NE10_S_MUL_NEON_S32 (scratch[9].val[0], TW_5B_S32.i)));

    NE10_CPX_SUB_NEON_S32 (&scratch_in[1], scratch[5], scratch[6]);
    NE10_CPX_ADD_NEON_S32 (&scratch_in[4], scratch[5], scratch[6]);

    scratch[11].val[0] = vaddq_s32(scratch[0].val[0] , vaddq_s32(NE10_S_MUL_NEON_S32 (scratch[7].val[0], TW_5B_S32.r) , NE10_S_MUL_NEON_S32 (scratch[8].val[0], TW_5A_S32.r)));
    scratch[11].val[1] = vaddq_s32(scratch[0].val[1] , vaddq_s32(NE10_S_MUL_NEON_S32 (scratch[7].val[1], TW_5B_S32.r) , NE10_S_MUL_NEON_S32 (scratch[8].val[1], TW_5A_S32.r)));

    scratch[12].val[0] = vnegq_s32(vaddq_s32(NE10_S_MUL_NEON_S32 (scratch[10].val[1], TW_5B_S32.i) , NE10_S_MUL_NEON_S32 (scratch[9].val[1], TW_5A_S32.i)));
    scratch[12].val[1] =           vsubq_s32(NE10_S_MUL_NEON_S32 (scratch[10].val[0], TW_5B_S32.i) , NE10_S_MUL_NEON_S32 (scratch[9].val[0], TW_5A_S32.i));

    NE10_CPX_ADD_NEON_S32 (&scratch_in[2], scratch[11], scratch[12]);
    NE10_CPX_SUB_NEON_S32 (&scratch_in[3], scratch[11], scratch[12]);

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
    Fout[3] = scratch_in[3];
    Fout[4] = scratch_in[4];
}

////////////////////////////////////
// Following are butterfly functions
////////////////////////////////////
#define NE10_RADIX_BUTTERFLY_INT32_NEON(RADIX,ISFIRSTSTAGE,ISINVERSE,ISSCALED)                                                 \
static NO_INLINE void ne10_radix_butterfly_int32_neon_##RADIX##_##ISFIRSTSTAGE##_##ISINVERSE##_##ISSCALED   ( \
        CPLX *Fout,                                                                                                            \
        const CPLX *Fin,                                                                                                       \
        const ne10_fft_cpx_int32_t *twiddles,                                                                                  \
        const ne10_int32_t fstride,                                                                                            \
        const ne10_int32_t out_step,                                                                                           \
        const ne10_int32_t nfft)                                                                                               \
{                                                                                                                              \
    const ne10_int32_t in_step = nfft / RADIX;                                                                                 \
    ne10_int32_t f_count;                                                                                                      \
    ne10_int32_t m_count;                                                                                                      \
                                                                                                                               \
    for (f_count = fstride; f_count > 0; f_count--)                                                                            \
    {                                                                                                                          \
        for (m_count = out_step; m_count > 0; m_count--)                                                                       \
        {                                                                                                                      \
            CPLX in[RADIX];                                                                                                    \
            CPLX out[RADIX];                                                                                                   \
                                                                                                                               \
            NE10_LOAD_BY_STEP_##RADIX (in, Fin, in_step);                                                                      \
                                                                                                                               \
            if (ISINVERSE)                                                                                                     \
            {                                                                                                                  \
                NE10_CONJ_##RADIX (in);                                                                                        \
            }                                                                                                                  \
                                                                                                                               \
            if (ISSCALED)                                                                                                      \
            {                                                                                                                  \
                ne10_fft_scaling_##RADIX##_##RADIX (in);                                                                       \
            }                                                                                                                  \
                                                                                                                               \
            if (!ISFIRSTSTAGE)                                                                                                 \
            {                                                                                                                  \
                NE10_LOAD_TW_AND_MUL_##RADIX (in, twiddles, out_step);                                                         \
            }                                                                                                                  \
                                                                                                                               \
            NE10_FFT_FCU_NEON_S32_##RADIX (out, in);                                                                           \
                                                                                                                               \
            if (ISINVERSE)                                                                                                     \
            {                                                                                                                  \
                NE10_CONJ_##RADIX (out);                                                                                       \
            }                                                                                                                  \
                                                                                                                               \
            NE10_STORE_BY_STEP_##RADIX (Fout, out, out_step);                                                                  \
                                                                                                                               \
            Fin++;                                                                                                             \
                                                                                                                               \
            if (!ISFIRSTSTAGE)                                                                                                 \
            {                                                                                                                  \
                Fout++;                                                                                                        \
                twiddles++;                                                                                                    \
            }                                                                                                                  \
            else                                                                                                               \
            {                                                                                                                  \
                Fout += RADIX;                                                                                                 \
            }                                                                                                                  \
        }                                                                                                                      \
        if (!ISFIRSTSTAGE)                                                                                                     \
        {                                                                                                                      \
            twiddles -= out_step;                                                                                              \
            Fout += (RADIX - 1) * out_step;                                                                                    \
        }                                                                                                                      \
    }                                                                                                                          \
}

NE10_RADIX_BUTTERFLY_INT32_NEON(2,1,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(2,1,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(2,1,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(2,1,1,1)

NE10_RADIX_BUTTERFLY_INT32_NEON(3,1,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(3,1,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(3,1,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(3,1,1,1)

NE10_RADIX_BUTTERFLY_INT32_NEON(4,1,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(4,1,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(4,1,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(4,1,1,1)

NE10_RADIX_BUTTERFLY_INT32_NEON(5,1,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(5,1,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(5,1,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(5,1,1,1)

NE10_RADIX_BUTTERFLY_INT32_NEON(2,0,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(2,0,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(2,0,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(2,0,1,1)

NE10_RADIX_BUTTERFLY_INT32_NEON(3,0,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(3,0,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(3,0,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(3,0,1,1)

NE10_RADIX_BUTTERFLY_INT32_NEON(4,0,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(4,0,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(4,0,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(4,0,1,1)

NE10_RADIX_BUTTERFLY_INT32_NEON(5,0,0,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(5,0,0,1)
NE10_RADIX_BUTTERFLY_INT32_NEON(5,0,1,0)
NE10_RADIX_BUTTERFLY_INT32_NEON(5,0,1,1)


#define NE10_MIXED_RADIX_GENERIC_BUTTERFLY_INT32_NEON_IMPL(ISINVERSE,ISSCALED)                       \
static void ne10_mixed_radix_generic_butterfly_int32_neon_impl_##ISINVERSE##_##ISSCALED (CPLX *Fout, \
        const CPLX *Fin,                                                                             \
        const ne10_uint32_t *factors,                                                                \
        const ne10_fft_cpx_int32_t *twiddles,                                                        \
        CPLX *buffer)                                                                                \
{                                                                                                    \
    ne10_int32_t fstride, mstride, radix;                                                            \
    ne10_int32_t stage_count;                                                                        \
    ne10_int32_t nfft;                                                                               \
                                                                                                     \
    /* init fstride, mstride, radix, nfft */                                                         \
    stage_count = factors[0];                                                                        \
    fstride = factors[1];                                                                            \
    mstride = 1;                                                                                     \
    radix = factors[ stage_count << 1 ]; /* radix of first stage */                                  \
    nfft = fstride * radix;                                                                          \
                                                                                                     \
    /* swap to make sure output to Fout  */                                                          \
    if (stage_count % 2 == 0)                                                                        \
    {                                                                                                \
        arm_ne10_swap_ptr (buffer, Fout);                                                            \
    }                                                                                                \
                                                                                                     \
    /* first stage  */                                                                               \
    switch (radix)                                                                                   \
    {                                                                                                \
    case 2:                                                                                          \
        ne10_radix_butterfly_int32_neon_2_1_##ISINVERSE##_##ISSCALED(Fout, Fin,                      \
                NULL,                                                                                \
                fstride, 1, nfft);                                                                   \
        break;                                                                                       \
    case 4:                                                                                          \
        ne10_radix_butterfly_int32_neon_4_1_##ISINVERSE##_##ISSCALED (Fout, Fin,                     \
                NULL,                                                                                \
                fstride, 1, nfft);                                                                   \
        break;                                                                                       \
    case 3:                                                                                          \
        ne10_radix_butterfly_int32_neon_3_1_##ISINVERSE##_##ISSCALED (Fout, Fin,                     \
                NULL,                                                                                \
                fstride, 1, nfft);                                                                   \
        break;                                                                                       \
    case 5:                                                                                          \
        ne10_radix_butterfly_int32_neon_5_1_##ISINVERSE##_##ISSCALED (Fout, Fin,                     \
                NULL,                                                                                \
                fstride, 1, nfft);                                                                   \
        break;                                                                                       \
    }                                                                                                \
                                                                                                     \
    stage_count--;                                                                                   \
    if (!stage_count) /* finish  */                                                                  \
    {                                                                                                \
        return;                                                                                      \
    }                                                                                                \
                                                                                                     \
    mstride *= radix;                                                                                \
                                                                                                     \
    /* update radix  */                                                                              \
    if (radix % 2)                                                                                   \
    {                                                                                                \
        twiddles += radix;                                                                           \
    }                                                                                                \
    radix = factors[ stage_count << 1 ];                                                             \
                                                                                                     \
    /* other stages */                                                                               \
    while (stage_count > 0)                                                                          \
    {                                                                                                \
        /* radix of first stage, should be one of {2,3,5,4}  */                                      \
        /* printf("Radix %d, index %d\n",radix,stage_count << 1); */                                 \
        assert ((radix > 1) && (radix < 6));                                                         \
                                                                                                     \
        arm_ne10_swap_ptr (buffer, Fout);                                                            \
                                                                                                     \
        fstride /= radix;                                                                            \
        switch (radix)                                                                               \
        {                                                                                            \
        case 2:                                                                                      \
            ne10_radix_butterfly_int32_neon_2_0_##ISINVERSE##_##ISSCALED (Fout, buffer,              \
                    twiddles,                                                                        \
                    fstride, mstride, nfft);                                                         \
            break;                                                                                   \
        case 3:                                                                                      \
            ne10_radix_butterfly_int32_neon_3_0_##ISINVERSE##_##ISSCALED (Fout, buffer,              \
                    twiddles,                                                                        \
                    fstride, mstride, nfft);                                                         \
            break;                                                                                   \
        case 4:                                                                                      \
            ne10_radix_butterfly_int32_neon_4_0_##ISINVERSE##_##ISSCALED (Fout, buffer,              \
                    twiddles,                                                                        \
                    fstride, mstride, nfft);                                                         \
            break;                                                                                   \
        case 5:                                                                                      \
            ne10_radix_butterfly_int32_neon_5_0_##ISINVERSE##_##ISSCALED (Fout, buffer,              \
                    twiddles, fstride, mstride, nfft);                                               \
            break;                                                                                   \
        } /* switch (radix)  */                                                                      \
                                                                                                     \
        twiddles += mstride * (radix - 1);                                                           \
        mstride *= radix;                                                                            \
                                                                                                     \
        stage_count--;                                                                               \
        radix = factors[ stage_count << 1 ];                                                         \
    } /* while (stage_count)  */                                                                     \
}

NE10_MIXED_RADIX_GENERIC_BUTTERFLY_INT32_NEON_IMPL(0,0)
NE10_MIXED_RADIX_GENERIC_BUTTERFLY_INT32_NEON_IMPL(0,1)
NE10_MIXED_RADIX_GENERIC_BUTTERFLY_INT32_NEON_IMPL(1,0)
NE10_MIXED_RADIX_GENERIC_BUTTERFLY_INT32_NEON_IMPL(1,1)

#define NE10_C2C_1D_LAST_STAGE_NEON(ISINVERSE,ISSCALED)                              \
static void ne10_c2c_1d_last_stage_neon_##ISINVERSE##_##ISSCALED (CPLX *Fout,        \
        const CPLX *Fin,                                                             \
        const ne10_fft_cpx_int32_t *twiddles,                                        \
        const ne10_int32_t fstride,                                                  \
        const ne10_int32_t out_step,                                                 \
        const ne10_int32_t ignored)                                                  \
{                                                                                    \
    (void)ignored;                                                                   \
    ne10_int32_t f_count;                                                            \
    ne10_int32_t m_count;                                                            \
                                                                                     \
    for (f_count = fstride; f_count > 0; f_count--)                                  \
    {                                                                                \
        CPLX scratch_in[4];                                                          \
        CPLX scratch_out[4];                                                         \
                                                                                     \
        for (m_count = out_step / NE10_FFT_PARA_LEVEL; m_count > 0; m_count--)       \
        {                                                                            \
            scratch_in[0] = NE10_CPLX_LOAD (Fin + 0);                                \
            scratch_in[1] = NE10_CPLX_LOAD (Fin + 1);                                \
            scratch_in[2] = NE10_CPLX_LOAD (Fin + 2);                                \
            scratch_in[3] = NE10_CPLX_LOAD (Fin + 3);                                \
                                                                                     \
            if (ISSCALED)                                                            \
            {                                                                        \
                ne10_fft_scaling_4_4 (scratch_in);                                   \
            }                                                                        \
                                                                                     \
            /* Transpose   */                                                        \
            {                                                                        \
                float32x4x2_t scratch0, scratch_in0;                                 \
                float32x4x2_t scratch1, scratch_in1;                                 \
                float32x4x2_t scratch2, scratch_in2;                                 \
                float32x4x2_t scratch3, scratch_in3;                                 \
                                                                                     \
                scratch_in0.val[0] = vreinterpretq_f32_s32 (scratch_in[0].val[0]);   \
                scratch_in1.val[0] = vreinterpretq_f32_s32 (scratch_in[1].val[0]);   \
                scratch_in2.val[0] = vreinterpretq_f32_s32 (scratch_in[2].val[0]);   \
                scratch_in3.val[0] = vreinterpretq_f32_s32 (scratch_in[3].val[0]);   \
                scratch_in0.val[1] = vreinterpretq_f32_s32 (scratch_in[0].val[1]);   \
                scratch_in1.val[1] = vreinterpretq_f32_s32 (scratch_in[1].val[1]);   \
                scratch_in2.val[1] = vreinterpretq_f32_s32 (scratch_in[2].val[1]);   \
                scratch_in3.val[1] = vreinterpretq_f32_s32 (scratch_in[3].val[1]);   \
                                                                                     \
                NE10_RADIX4X4C_TRANSPOSE_NEON (scratch, scratch_in);                 \
                                                                                     \
                scratch_in[0].val[0] = vreinterpretq_s32_f32 (scratch0.val[0]);      \
                scratch_in[1].val[0] = vreinterpretq_s32_f32 (scratch1.val[0]);      \
                scratch_in[2].val[0] = vreinterpretq_s32_f32 (scratch2.val[0]);      \
                scratch_in[3].val[0] = vreinterpretq_s32_f32 (scratch3.val[0]);      \
                scratch_in[0].val[1] = vreinterpretq_s32_f32 (scratch0.val[1]);      \
                scratch_in[1].val[1] = vreinterpretq_s32_f32 (scratch1.val[1]);      \
                scratch_in[2].val[1] = vreinterpretq_s32_f32 (scratch2.val[1]);      \
                scratch_in[3].val[1] = vreinterpretq_s32_f32 (scratch3.val[1]);      \
            }                                                                        \
                                                                                     \
            if (ISINVERSE)                                                           \
            {                                                                        \
                NE10_CONJ_4(scratch_in);                                             \
            }                                                                        \
                                                                                     \
            /* Not first stage  */                                                   \
            {                                                                        \
                CPLX scratch_tw[3];                                                  \
                                                                                     \
                scratch_tw[0] = NE10_CPLX_LOAD (twiddles + 0 * out_step);            \
                scratch_tw[1] = NE10_CPLX_LOAD (twiddles + 1 * out_step);            \
                scratch_tw[2] = NE10_CPLX_LOAD (twiddles + 2 * out_step);            \
                                                                                     \
                NE10_CPX_MUL_NEON_S32 (&scratch_in[1], scratch_in[1], scratch_tw[0]);\
                NE10_CPX_MUL_NEON_S32 (&scratch_in[2], scratch_in[2], scratch_tw[1]);\
                NE10_CPX_MUL_NEON_S32 (&scratch_in[3], scratch_in[3], scratch_tw[2]);\
            }                                                                        \
                                                                                     \
            NE10_FFT_FCU_NEON_S32_4 (scratch_out, scratch_in);                       \
                                                                                     \
            if (ISINVERSE)                                                           \
            {                                                                        \
                NE10_CONJ_4 (scratch_out);                                           \
            }                                                                        \
                                                                                     \
            /* Store.  */                                                            \
            {                                                                        \
                ne10_fft_cpx_int32_t *Fout_cpx;                                      \
                Fout_cpx = (ne10_fft_cpx_int32_t *) Fout;                            \
                                                                                     \
                NE10_CPLX_STORE (Fout_cpx + 0 * out_step, scratch_out[0]);           \
                NE10_CPLX_STORE (Fout_cpx + 1 * out_step, scratch_out[1]);           \
                NE10_CPLX_STORE (Fout_cpx + 2 * out_step, scratch_out[2]);           \
                NE10_CPLX_STORE (Fout_cpx + 3 * out_step, scratch_out[3]);           \
            }                                                                        \
                                                                                     \
            Fin += 4;                                                                \
            Fout += 1;                                                               \
            twiddles += 4;                                                           \
        }                                                                            \
    }                                                                                \
                                                                                     \
    ne10_int32_t left_over = out_step % 4;                                           \
    if (left_over == 0)                                                              \
    {                                                                                \
        return;                                                                      \
    }                                                                                \
                                                                                     \
    /* Left over.   */                                                               \
    const ne10_fft_cpx_int32_t *Fin_s  = (ne10_fft_cpx_int32_t *) Fin;               \
    ne10_fft_cpx_int32_t *Fout_s = (ne10_fft_cpx_int32_t *) Fout;                    \
    for (m_count = out_step % 4; m_count > 0; m_count--)                             \
    {                                                                                \
        ne10_fft_cpx_int32_t scratch_in[4];                                          \
        ne10_fft_cpx_int32_t scratch_tw[4];                                          \
                                                                                     \
        scratch_in[0] = Fin_s[0];                                                    \
        scratch_in[1] = Fin_s[1];                                                    \
        scratch_in[2] = Fin_s[2];                                                    \
        scratch_in[3] = Fin_s[3];                                                    \
                                                                                     \
        if (ISSCALED)                                                                \
        {                                                                            \
            scratch_in[0].r = scratch_in[0].r >> 2;                                  \
            scratch_in[1].r = scratch_in[1].r >> 2;                                  \
            scratch_in[2].r = scratch_in[2].r >> 2;                                  \
            scratch_in[3].r = scratch_in[3].r >> 2;                                  \
                                                                                     \
            scratch_in[0].i = scratch_in[0].i >> 2;                                  \
            scratch_in[1].i = scratch_in[1].i >> 2;                                  \
            scratch_in[2].i = scratch_in[2].i >> 2;                                  \
            scratch_in[3].i = scratch_in[3].i >> 2;                                  \
        }                                                                            \
                                                                                     \
                                                                                     \
        if (ISINVERSE)                                                               \
        {                                                                            \
            scratch_in[0].i = -scratch_in[0].i;                                      \
            scratch_in[1].i = -scratch_in[1].i;                                      \
            scratch_in[2].i = -scratch_in[2].i;                                      \
            scratch_in[3].i = -scratch_in[3].i;                                      \
        }                                                                            \
                                                                                     \
        scratch_tw[0] = twiddles[0 * out_step];                                      \
        scratch_tw[1] = twiddles[1 * out_step];                                      \
        scratch_tw[2] = twiddles[2 * out_step];                                      \
                                                                                     \
        NE10_CPX_MUL_S32 (scratch_in[1], scratch_in[1], scratch_tw[0]);              \
        NE10_CPX_MUL_S32 (scratch_in[2], scratch_in[2], scratch_tw[1]);              \
        NE10_CPX_MUL_S32 (scratch_in[3], scratch_in[3], scratch_tw[2]);              \
                                                                                     \
        FFT_FCU_4 (scratch_in, scratch_in);                                          \
                                                                                     \
        if (ISINVERSE)                                                               \
        {                                                                            \
            scratch_in[0].i = -scratch_in[0].i;                                      \
            scratch_in[1].i = -scratch_in[1].i;                                      \
            scratch_in[2].i = -scratch_in[2].i;                                      \
            scratch_in[3].i = -scratch_in[3].i;                                      \
        }                                                                            \
                                                                                     \
        Fout_s[0 * out_step] = scratch_in[0];                                        \
        Fout_s[1 * out_step] = scratch_in[1];                                        \
        Fout_s[2 * out_step] = scratch_in[2];                                        \
        Fout_s[3 * out_step] = scratch_in[3];                                        \
                                                                                     \
        Fin_s += 4;                                                                  \
        Fout_s += 1;                                                                 \
        twiddles += 1;                                                               \
    }                                                                                \
}

NE10_C2C_1D_LAST_STAGE_NEON(0,0)
NE10_C2C_1D_LAST_STAGE_NEON(0,1)
NE10_C2C_1D_LAST_STAGE_NEON(1,0)
NE10_C2C_1D_LAST_STAGE_NEON(1,1)

#endif
