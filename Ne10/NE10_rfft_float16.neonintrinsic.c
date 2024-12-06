/*
 *  Copyright 2014-24 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_rfft_float16.neonintrinsic.c
 * Small modifications for inclusion in CMSIS-DSP
 */

#include <arm_neon.h>

#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_fft.h"
#include "CMSIS_NE10_fft.neonintrinsic_f16.h"
#include "CMSIS_NE10_fft_cplx_ops.h"
#include "CMSIS_NE10_fft_bfly_f16.h"

#include "dsp/transform_functions_f16.h"

#if defined(ARM_MATH_NEON_FLOAT16)

static void arm_ne10_radix8x4_r2c_neon (
    ne10_fft_cpx_float16_t *Fout,
    const ne10_fft_cpx_float16_t *Fin,
    const ne10_int32_t fstride,
    const ne10_int32_t mstride,
    const ne10_int32_t nfft)
{
    (void)mstride;
    (void)nfft;
    ne10_int32_t f_count;

    NE10_DECLARE_8(float16x4_t,q_in);
    NE10_DECLARE_8(float16x4_t,q_out);

    const float16x4_t *Fin_neon  = (float16x4_t*) Fin;  // 8 x fstride
          float16x4_t *Fout_neon = (float16x4_t*) Fout; // fstride x 8

    for (f_count = fstride; f_count > 0; f_count --)
    {
        // from Fin_neon load 8 float16x4_t into q_in0 ~ q_in7, by step = fstride
        NE10_RADIX8x4_R2C_NEON_LOAD(Fin_neon,q_in,fstride);

        // print q_in0 ~ q_in7
        // NE10_PRINT_Qx8_VECTOR(q_in);

        // do r2c fft, size = 8
        NE10_RADIX8x4_R2C_NEON_KERNEL(q_out,q_in);

        // print q_out0 ~ q_out7
        // NE10_PRINT_Qx8_VECTOR(q_out);

        // store q_out0 ~ q_out7 to Fout_neon, by step = 1
        NE10_RADIX8x4_R2C_NEON_STORE(Fout_neon,q_out,1);

        Fin_neon = Fin_neon - fstride * 8 + 1;
        Fout_neon += 8; // next column
    }
}

static void arm_ne10_radix8x4_c2r_neon (
    ne10_fft_cpx_float16_t *Fout,
    const ne10_fft_cpx_float16_t *Fin,
    const ne10_int32_t fstride,
    const ne10_int32_t mstride,
    const ne10_int32_t nfft)
{
    (void)mstride;
    (void)nfft;
    ne10_int32_t f_count;

    NE10_DECLARE_8(float16x4_t,q_in);
    NE10_DECLARE_8(float16x4_t,q_out);

#if defined(CMSIS_NE10_DSP_RIFFT_SCALING)
    const ne10_float16_t one_by_N = 0.25f16 / nfft;
    const float16x4_t one_by_N_neon = vdup_n_f16(one_by_N);
#endif 

    const float16x4_t *Fin_neon  = (float16x4_t*) Fin;
          float16x4_t *Fout_neon = (float16x4_t*) Fout;

    for (f_count = fstride; f_count > 0; f_count --)
    {
        // from Fin_neon load 8 float16x4_t into q_in0 ~ q_in7, by step = 1
        NE10_RADIX8x4_R2C_NEON_LOAD(Fin_neon,q_in,1);

        // NE10_PRINT_Qx8_VECTOR(q_in);

        NE10_RADIX8x4_C2R_NEON_KERNEL(q_out,q_in);

        // NE10_PRINT_Qx8_VECTOR(q_out);

#ifdef CMSIS_NE10_DSP_RIFFT_SCALING
        q_out0 = vmul_f16(q_out0,one_by_N_neon);
        q_out1 = vmul_f16(q_out1,one_by_N_neon);
        q_out2 = vmul_f16(q_out2,one_by_N_neon);
        q_out3 = vmul_f16(q_out3,one_by_N_neon);
        q_out4 = vmul_f16(q_out4,one_by_N_neon);
        q_out5 = vmul_f16(q_out5,one_by_N_neon);
        q_out6 = vmul_f16(q_out6,one_by_N_neon);
        q_out7 = vmul_f16(q_out7,one_by_N_neon);
#endif

        // store
        NE10_RADIX8x4_R2C_NEON_STORE(Fout_neon,q_out,fstride);

        Fout_neon ++;
    }
}


static void arm_ne10_radix4x4_r2c_neon (
    ne10_fft_cpx_float16_t *Fout,
    const ne10_fft_cpx_float16_t *Fin,
    const ne10_int32_t fstride,
    const ne10_int32_t mstride,
    const ne10_int32_t nfft)
{
    (void)mstride;
    (void)nfft;
    ne10_int32_t f_count;

    const float16x4_t *Fin_neon  = (float16x4_t*) Fin;
          float16x4_t *Fout_neon = (float16x4_t*) Fout;

    for (f_count = 0; f_count < fstride; f_count ++)
    {
        NE10_DECLARE_4(float16x4_t,q_in);
        NE10_DECLARE_4(float16x4_t,q_out);

        // load
        NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,fstride);

        NE10_RADIX4x4_R2C_NEON_KERNEL(q_out,q_in)

        // store
        NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,1);

        Fin_neon = Fin_neon - 4*fstride + 1;
        Fout_neon += 4;
    }
}

static void arm_ne10_radix4x4_c2r_neon (
    ne10_fft_cpx_float16_t *Fout,
    const ne10_fft_cpx_float16_t *Fin,
    const ne10_int32_t fstride,
    const ne10_int32_t mstride,
    const ne10_int32_t nfft)
{
    (void)mstride;
    (void)nfft;
    ne10_int32_t f_count;

    const float16x4_t *Fin_neon  = (float16x4_t*) Fin;
          float16x4_t *Fout_neon = (float16x4_t*) Fout;

#if defined(CMSIS_NE10_DSP_RIFFT_SCALING)
    const ne10_float16_t one_by_N = 0.25f16 / nfft;
    const float16x4_t one_by_N_neon = vdup_n_f16(one_by_N);
#endif
    for (f_count = 0; f_count < fstride; f_count ++)
    {
        NE10_DECLARE_4(float16x4_t,q_in);
        NE10_DECLARE_4(float16x4_t,q_out);

        // load
        NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,1);

        // NE10_PRINT_Qx4_VECTOR(q_in);

        NE10_RADIX4x4_C2R_NEON_KERNEL(q_out,q_in)

        // NE10_PRINT_Qx4_VECTOR(q_out);

#ifdef CMSIS_NE10_DSP_RIFFT_SCALING
        q_out0 = vmul_f16(q_out0,one_by_N_neon);
        q_out1 = vmul_f16(q_out1,one_by_N_neon);
        q_out2 = vmul_f16(q_out2,one_by_N_neon);
        q_out3 = vmul_f16(q_out3,one_by_N_neon);
#endif

        // store
        NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,fstride);

        Fout_neon ++;
    }
}

static void arm_ne10_radix4x4_r2c_with_twiddles_first_butterfly_neon (
    float16x4_t *Fout_neon,
    const float16x4_t *Fin_neon,
    const ne10_int32_t out_step,
    const ne10_int32_t in_step,
    const ne10_fft_cpx_float16_t *twiddles)
{
    (void)twiddles;
    NE10_DECLARE_4(float16x4_t,q_in);
    NE10_DECLARE_4(float16x4_t,q_out);

    // load
    NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,in_step);

    NE10_RADIX4x4_R2C_NEON_KERNEL(q_out,q_in);

    // store
    vst1_f16( (ne10_float16_t*) (Fout_neon                          ), q_out0);
    vst1_f16( (ne10_float16_t*) (Fout_neon +     (out_step << 1) - 1), q_out1);
    vst1_f16( (ne10_float16_t*) (Fout_neon +     (out_step << 1)    ), q_out2);
    vst1_f16( (ne10_float16_t*) (Fout_neon + 2 * (out_step << 1) - 1), q_out3);
}

static void arm_ne10_radix4x4_c2r_with_twiddles_first_butterfly_neon (
    float16x4_t *Fout_neon,
    const float16x4_t *Fin_neon,
    const ne10_int32_t out_step,
    const ne10_int32_t in_step,
    const ne10_fft_cpx_float16_t *twiddles)
{
    (void)twiddles;
    NE10_DECLARE_4(float16x4_t,q_in);
    NE10_DECLARE_4(float16x4_t,q_out);

    // load
    q_in0 = vld1_f16( (ne10_float16_t*) (Fin_neon                          ) );
    q_in1 = vld1_f16( (ne10_float16_t*) (Fin_neon +     (out_step << 1) - 1) );
    q_in2 = vld1_f16( (ne10_float16_t*) (Fin_neon +     (out_step << 1)    ) );
    q_in3 = vld1_f16( (ne10_float16_t*) (Fin_neon + 2 * (out_step << 1) - 1) );

    // NE10_PRINT_Qx4_VECTOR(q_in);

    NE10_RADIX4x4_C2R_NEON_KERNEL(q_out,q_in);

    // NE10_PRINT_Qx4_VECTOR(q_out);

    // store
    NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,in_step);
}

static void arm_ne10_radix4x4_r2c_with_twiddles_other_butterfly_neon (
    float16x4_t *Fout_neon,
    const float16x4_t *Fin_neon,
    const ne10_int32_t out_step,
    const ne10_int32_t in_step,
    const ne10_fft_cpx_float16_t *twiddles)
{
    ne10_int32_t m_count;
    ne10_int32_t loop_count = (out_step>>1) -1;
    float16x4_t *Fout_b = Fout_neon + (((out_step<<1)-1)<<1) - 2; // reversed

    NE10_DECLARE_3(float16x4x2_t,q2_tw);
    NE10_DECLARE_4(float16x4x2_t,q2_in);
    NE10_DECLARE_4(float16x4x2_t,q2_out);

    for (m_count = loop_count; m_count > 0; m_count -- )
    {
        // load
        q2_in0.val[0] = vld1_f16( (ne10_float16_t*) (Fin_neon + 0*in_step    ) );
        q2_in0.val[1] = vld1_f16( (ne10_float16_t*) (Fin_neon + 0*in_step + 1) );

        q2_in1.val[0] = vld1_f16( (ne10_float16_t*) (Fin_neon + 1*in_step    ) );
        q2_in1.val[1] = vld1_f16( (ne10_float16_t*) (Fin_neon + 1*in_step + 1) );

        q2_in2.val[0] = vld1_f16( (ne10_float16_t*) (Fin_neon + 2*in_step    ) );
        q2_in2.val[1] = vld1_f16( (ne10_float16_t*) (Fin_neon + 2*in_step + 1) );

        q2_in3.val[0] = vld1_f16( (ne10_float16_t*) (Fin_neon + 3*in_step    ) );
        q2_in3.val[1] = vld1_f16( (ne10_float16_t*) (Fin_neon + 3*in_step + 1) );

        q2_tw0.val[0] = vdup_n_f16(twiddles[0].r);
        q2_tw0.val[1] = vdup_n_f16(twiddles[0].i);

        q2_tw1.val[0] = vdup_n_f16(twiddles[1].r);
        q2_tw1.val[1] = vdup_n_f16(twiddles[1].i);

        q2_tw2.val[0] = vdup_n_f16(twiddles[2].r);
        q2_tw2.val[1] = vdup_n_f16(twiddles[2].i);

        // R2C TW KERNEL
        NE10_RADIX4x4_R2C_TW_MUL_NEON (q2_out, q2_in, q2_tw);

        NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S1 (q2_in, q2_out);
        NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S2 (q2_out, q2_in);

        // store
        vst1_f16( (ne10_float16_t*) ( Fout_neon                      ), q2_out0.val[0] );
        vst1_f16( (ne10_float16_t*) ( Fout_neon                   + 1), q2_out0.val[1] );

        vst1_f16( (ne10_float16_t*) ( Fout_neon + (out_step << 1)    ), q2_out1.val[0] );
        vst1_f16( (ne10_float16_t*) ( Fout_neon + (out_step << 1) + 1), q2_out1.val[1] );

        vst1_f16( (ne10_float16_t*) ( Fout_b                         ), q2_out2.val[0] );
        vst1_f16( (ne10_float16_t*) ( Fout_b                      + 1), q2_out2.val[1] );

        vst1_f16( (ne10_float16_t*) ( Fout_b    - (out_step << 1)    ), q2_out3.val[0] );
        vst1_f16( (ne10_float16_t*) ( Fout_b    - (out_step << 1) + 1), q2_out3.val[1] );

        // update pointers
        Fin_neon  += 2;
        Fout_neon += 2;
        Fout_b    -= 2;
        twiddles += 3;
    }
}

static void arm_ne10_radix4x4_c2r_with_twiddles_other_butterfly_neon (
    float16x4_t *Fout_neon,
    const float16x4_t *Fin_neon,
    const ne10_int32_t out_step,
    const ne10_int32_t in_step,
    const ne10_fft_cpx_float16_t *twiddles)
{
    ne10_int32_t m_count;
    ne10_int32_t loop_count = (out_step>>1) -1;
    const float16x4_t *Fin_b = Fin_neon + (((out_step<<1)-1)<<1) - 2; // reversed

    NE10_DECLARE_3(float16x4x2_t,q2_tw);
    NE10_DECLARE_4(float16x4x2_t,q2_in);
    NE10_DECLARE_4(float16x4x2_t,q2_out);

    for (m_count = loop_count; m_count > 0; m_count -- )
    {
        // load
        q2_in0.val[0] = vld1_f16( (ne10_float16_t*) ( Fin_neon                      ) );
        q2_in0.val[1] = vld1_f16( (ne10_float16_t*) ( Fin_neon                   + 1) );

        q2_in1.val[0] = vld1_f16( (ne10_float16_t*) ( Fin_neon + (out_step << 1)    ) );
        q2_in1.val[1] = vld1_f16( (ne10_float16_t*) ( Fin_neon + (out_step << 1) + 1) );

        q2_in2.val[0] = vld1_f16( (ne10_float16_t*) ( Fin_b                         ) );
        q2_in2.val[1] = vld1_f16( (ne10_float16_t*) ( Fin_b                      + 1) );

        q2_in3.val[0] = vld1_f16( (ne10_float16_t*) ( Fin_b    - (out_step << 1)    ) );
        q2_in3.val[1] = vld1_f16( (ne10_float16_t*) ( Fin_b    - (out_step << 1) + 1) );

        q2_tw0.val[0] = vdup_n_f16(twiddles[0].r);
        q2_tw0.val[1] = vdup_n_f16(twiddles[0].i);

        q2_tw1.val[0] = vdup_n_f16(twiddles[1].r);
        q2_tw1.val[1] = vdup_n_f16(twiddles[1].i);

        q2_tw2.val[0] = vdup_n_f16(twiddles[2].r);
        q2_tw2.val[1] = vdup_n_f16(twiddles[2].i);

        // NE10_PRINT_Q2x4_VECTOR(q2_in);

        // R2C TW KERNEL
        NE10_RADIX4x4_C2R_TW_NEON_KERNEL(q2_out,q2_in,q2_tw);

        // NE10_PRINT_Q2x4_VECTOR(q2_out);

        // store
        vst1_f16( (ne10_float16_t*) (Fout_neon + 0*in_step    ), q2_out0.val[0] );
        vst1_f16( (ne10_float16_t*) (Fout_neon + 0*in_step + 1), q2_out0.val[1] );

        vst1_f16( (ne10_float16_t*) (Fout_neon + 1*in_step    ), q2_out1.val[0] );
        vst1_f16( (ne10_float16_t*) (Fout_neon + 1*in_step + 1), q2_out1.val[1] );

        vst1_f16( (ne10_float16_t*) (Fout_neon + 2*in_step    ), q2_out2.val[0] );
        vst1_f16( (ne10_float16_t*) (Fout_neon + 2*in_step + 1), q2_out2.val[1] );

        vst1_f16( (ne10_float16_t*) (Fout_neon + 3*in_step    ), q2_out3.val[0] );
        vst1_f16( (ne10_float16_t*) (Fout_neon + 3*in_step + 1), q2_out3.val[1] );

        // update pointers
        Fin_neon  += 2;
        Fout_neon += 2;
        Fin_b    -= 2;
        twiddles += 3;
    }
}

static void arm_ne10_radix4x4_r2c_with_twiddles_last_butterfly_neon (
    float16x4_t *Fout_neon,
    const float16x4_t *Fin_neon,
    const ne10_int32_t out_step,
    const ne10_int32_t in_step,
    const ne10_fft_cpx_float16_t *twiddles)
{
    (void)twiddles;
    NE10_DECLARE_4(float16x4_t,q_in);
    NE10_DECLARE_4(float16x4_t,q_out);

    // load
    NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,in_step);

    NE10_RADIX4x4_R2C_TW_NEON_KERNEL_LAST(q_out,q_in);

    // store
    vst1_f16( (ne10_float16_t*) (Fout_neon    ), q_out0);
    vst1_f16( (ne10_float16_t*) (Fout_neon + 1), q_out1);
    vst1_f16( (ne10_float16_t*) (Fout_neon + (out_step << 1)    ), q_out2);
    vst1_f16( (ne10_float16_t*) (Fout_neon + (out_step << 1) + 1), q_out3);
}

static void arm_ne10_radix4x4_c2r_with_twiddles_last_butterfly_neon (
    float16x4_t *Fout_neon,
    const float16x4_t *Fin_neon,
    const ne10_int32_t out_step,
    const ne10_int32_t in_step,
    const ne10_fft_cpx_float16_t *twiddles)
{
    (void)twiddles;
    NE10_DECLARE_4(float16x4_t,q_in);
    NE10_DECLARE_4(float16x4_t,q_out);

    // load
    q_in0 = vld1_f16( (ne10_float16_t*) (Fin_neon    ) );
    q_in1 = vld1_f16( (ne10_float16_t*) (Fin_neon + 1) );
    q_in2 = vld1_f16( (ne10_float16_t*) (Fin_neon + (out_step << 1)    ) );
    q_in3 = vld1_f16( (ne10_float16_t*) (Fin_neon + (out_step << 1) + 1) );

    // NE10_PRINT_Qx4_VECTOR(q_in);

    NE10_RADIX4x4_C2R_TW_NEON_KERNEL_LAST(q_out,q_in);

    // NE10_PRINT_Qx4_VECTOR(q_out);

    // store
    NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,in_step);
}


static void arm_ne10_radix4x4_r2c_with_twiddles_neon (
    ne10_fft_cpx_float16_t *Fout,
    const ne10_fft_cpx_float16_t *Fin,
    const ne10_int32_t fstride,
    const ne10_int32_t mstride,
    const ne10_int32_t nfft,
    const ne10_fft_cpx_float16_t *twiddles)
{
    ne10_int32_t f_count;
    const ne10_int32_t in_step = nfft >> 2;
    const ne10_int32_t out_step = mstride;

    const float16x4_t *Fin_neon  = (float16x4_t*) Fin;
          float16x4_t *Fout_neon = (float16x4_t*) Fout;
    const ne10_fft_cpx_float16_t *tw;

    for (f_count = fstride; f_count; f_count --)
    {
        tw = twiddles + 3;

        // first butterfly
        arm_ne10_radix4x4_r2c_with_twiddles_first_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, NULL);

        Fin_neon ++;
        Fout_neon ++;

        // other butterfly
        // Twiddle tables are transposed to avoid memory access by a large stride.
        arm_ne10_radix4x4_r2c_with_twiddles_other_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, tw);

        // update Fin_r, Fout_r, twiddles
        Fin_neon  += 2 * ( (out_step >> 1) - 1);
        Fout_neon += 2 * ( (out_step >> 1) - 1);

        // last butterfly
        arm_ne10_radix4x4_r2c_with_twiddles_last_butterfly_neon (Fout_neon, Fin_neon, out_step, in_step, NULL);
        Fin_neon ++;
        Fout_neon ++;

        Fout_neon = Fout_neon + 3 * out_step;
    } // f_count
}

static void arm_ne10_radix4x4_c2r_with_twiddles_neon (
    ne10_fft_cpx_float16_t *Fout,
    const ne10_fft_cpx_float16_t *Fin,
    const ne10_int32_t fstride,
    const ne10_int32_t mstride,
    const ne10_int32_t nfft,
    const ne10_fft_cpx_float16_t *twiddles)
{
    ne10_int32_t f_count;
    const ne10_int32_t in_step = nfft >> 2;
    const ne10_int32_t out_step = mstride;

    const float16x4_t *Fin_neon  = (float16x4_t*) Fin;
          float16x4_t *Fout_neon = (float16x4_t*) Fout;
    const ne10_fft_cpx_float16_t *tw;

    for (f_count = fstride; f_count; f_count --)
    {
        tw = twiddles + 3;

        // first butterfly
        arm_ne10_radix4x4_c2r_with_twiddles_first_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, NULL);

        Fin_neon ++;
        Fout_neon ++;

        // other butterfly
        // Twiddle tables are transposed to avoid memory access by a large stride.
        arm_ne10_radix4x4_c2r_with_twiddles_other_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, tw);

        // update Fin_r, Fout_r, twiddles
        Fin_neon  += 2 * ( (out_step >> 1) - 1);
        Fout_neon += 2 * ( (out_step >> 1) - 1);

        // last butterfly
        arm_ne10_radix4x4_c2r_with_twiddles_last_butterfly_neon (Fout_neon, Fin_neon, out_step, in_step, NULL);
        Fin_neon ++;
        Fout_neon ++;

        Fin_neon = Fin_neon + 3 * out_step;
    } // f_count
}

static void arm_ne10_mixed_radix_r2c_butterfly_float16_neon (
    const arm_rfft_fast_instance_f16 *S,
    const ne10_fft_cpx_float16_t * Fin,
    ne10_fft_cpx_float16_t * Fout,
    ne10_fft_cpx_float16_t * buffer)
{
    ne10_int32_t fstride, mstride, nfft;
    ne10_int32_t radix;
    ne10_int32_t stage_count;

    const ne10_fft_cpx_float16_t *twiddles = (const ne10_fft_cpx_float16_t *)S->r_twiddles_neon;


    // PRINT_STAGE_INFO;

    // init fstride, mstride, radix, nfft

    stage_count = S->r_factors_neon[0];
    fstride = S->r_factors_neon[1];
    mstride = S->r_factors_neon[3];
    radix = S->r_factors_neon[2];
//
    nfft        = radix * fstride; // not the real nfft
    
    // PRINT_STAGE_INFO;

    if (stage_count % 2 == 1) // since there is another stage outside
    {
        arm_ne10_swap_ptr (buffer, Fout);
    }

    // the first stage
    if (radix == 8)   // length of FFT is 2^n (n is odd)
    {
        arm_ne10_radix8x4_r2c_neon (Fout, Fin, fstride, mstride, nfft);
    }
    else if (radix == 4)   // length of FFT is 2^n (n is even)
    {
        arm_ne10_radix4x4_r2c_neon (Fout, Fin, fstride, mstride, nfft);
    }
    // end of first stage

    // others
    for (; fstride > 1;)
    {
        fstride >>= 2;
        arm_ne10_swap_ptr (buffer, Fout);

        arm_ne10_radix4x4_r2c_with_twiddles_neon (Fout, buffer, fstride, mstride, nfft, twiddles);
        twiddles += 3 * mstride;
        mstride <<= 2;
    } // other stage
}

static void arm_ne10_mixed_radix_c2r_butterfly_float16_neon (
    ne10_fft_cpx_float16_t * Fout,
    const ne10_fft_cpx_float16_t * Fin,
    const ne10_uint32_t * factors,
    const ne10_fft_cpx_float16_t * twiddles,
    ne10_fft_cpx_float16_t * buffer)
{
    (void)Fin;
    ne10_int32_t fstride, mstride, nfft;
    ne10_int32_t radix;
    ne10_int32_t stage_count;

    // PRINT_STAGE_INFO;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride = factors[1];
    mstride = factors[3];
    radix = factors[2];

    nfft        = radix * fstride; // not the real nfft

    // fstride, mstride for last last stage
    fstride = 1;
    mstride = nfft >> 2;

    if (stage_count % 2 == 0)
    {
        arm_ne10_swap_ptr(Fout,buffer);
    }

    // others but the first stage
    for (; stage_count > 1;)
    {
        twiddles -= 3 * mstride;

        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        arm_ne10_radix4x4_c2r_with_twiddles_neon (Fout, buffer, fstride, mstride, nfft, twiddles);

        fstride <<= 2;
        mstride >>= 2;
        stage_count --;
        arm_ne10_swap_ptr (buffer, Fout);
    }

    // first stage -- inversed
    if (radix == 8)   // length of FFT is 2^n (n is odd)
    {
        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        arm_ne10_radix8x4_c2r_neon (Fout, buffer, fstride, mstride, nfft);
    }
    else if (radix == 4)   // length of FFT is 2^n (n is even)
    {
        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        arm_ne10_radix4x4_c2r_neon (Fout, buffer, fstride, mstride, nfft);
    }
}


static void arm_ne10_radix4_r2c_with_twiddles_last_stage_first_butterfly (
    ne10_fft_cpx_float16_t *dst,
    const ne10_fft_cpx_float16_t *src,             
    const ne10_fft_cpx_float16_t *twiddles,
    const ne10_int32_t nfft)
{
    // b0
    {
        ne10_float16_t q_4r_out[4];
        const ne10_float16_t *p_src_r = (const ne10_float16_t*) src;

        NE10_FFT_R2C_4R_RCR(q_4r_out,p_src_r);

        dst[0].r = q_4r_out[0];
        dst[0].i = q_4r_out[3];
        dst += (nfft>>2);
        dst[0].r = q_4r_out[1];
        dst[0].i = q_4r_out[2];
        dst -= (nfft>>2);
    }

    // b2
    {
        const ne10_float16_t *p_src_r = (const ne10_float16_t*) (src);
        p_src_r  += nfft;
        p_src_r  -= 4;

        ne10_float16_t q_4r_out[4];

        NE10_FFT_R2C_4R_CC(q_4r_out,p_src_r);

        dst += (nfft>>3);
        dst[0].r = q_4r_out[0];
        dst[0].i = q_4r_out[1];
        dst += (nfft>>2);
        dst[0].r = q_4r_out[2];
        dst[0].i = q_4r_out[3];
        dst -= (nfft>>3);
        dst -= (nfft>>2);
    }

    // b1
    ne10_fft_cpx_float16_t cc_out[4];
    ne10_fft_cpx_float16_t cc_in [4];
    const ne10_float16_t *p_src_r = (const ne10_float16_t*) src;
    p_src_r += 4;

    cc_out[0].r = *(p_src_r ++);
    cc_out[1].r = *(p_src_r ++);
    cc_out[2].r = *(p_src_r ++);
    cc_out[3].r = *(p_src_r ++);

    cc_out[0].i = *(p_src_r ++);
    cc_out[1].i = *(p_src_r ++);
    cc_out[2].i = *(p_src_r ++);
    cc_out[3].i = *(p_src_r ++);

    //NE10_PRINT_Q2_VECTOR(cc_out);

    // twiddles[0] = ( 1.0, 0.0);
    // NE10_CPX_MUL_F16(cc_in[0],cc_out[0],twiddles[0]);
    cc_in[0] = cc_out[0];
    twiddles ++;

    NE10_CPX_MUL_F16(cc_in[1],cc_out[1],twiddles[0]);
    twiddles ++;

    NE10_CPX_MUL_F16(cc_in[2],cc_out[2],twiddles[0]);
    twiddles ++;

    NE10_CPX_MUL_F16(cc_in[3],cc_out[3],twiddles[0]);

    // NE10_PRINT_Q2_VECTOR(cc_in);

    NE10_FFT_R2C_CC_CC(cc_out,cc_in);

    // NE10_PRINT_Q2_VECTOR(cc_out);

    dst[1] = cc_out[0];
    dst += (nfft>>2);
    dst[ 1] = cc_out[1];
    dst[-1] = cc_out[3];
    dst += (nfft>>2);
    dst[-1] = cc_out[2];
}


static void arm_ne10_radix4_c2r_with_twiddles_first_stage_first_butterfly (
    ne10_fft_cpx_float16_t *dst,
    const ne10_fft_cpx_float16_t *src,
    const ne10_fft_cpx_float16_t *twiddles,
    const ne10_int32_t nfft)
{
    // b0
    {
        ne10_float16_t q_4r_in[4];
        ne10_float16_t *p_dst_r = (ne10_float16_t*) dst;

        q_4r_in[0] = src[0].r;
        q_4r_in[3] = src[0].i;
        src += (nfft>>2);
        q_4r_in[1] = src[0].r;
        q_4r_in[2] = src[0].i;
        src -= (nfft>>2);

        NE10_FFT_C2R_RCR_4R(p_dst_r,q_4r_in);
    }

    // b2
    {
        // float16x4_t q_in;
        ne10_float16_t *p_dst_r = (ne10_float16_t*) (dst);
        p_dst_r  += nfft;
        p_dst_r  -= 4;

        ne10_float16_t q_4r_in[4];
        src += (nfft>>3);
        q_4r_in[0] = src[0].r;
        q_4r_in[1] = src[0].i;
        src += (nfft>>2);
        q_4r_in[2] = src[0].r;
        q_4r_in[3] = src[0].i;
        src -= (nfft>>3);
        src -= (nfft>>2);

        NE10_FFT_C2R_CC_4R(p_dst_r,q_4r_in);
    }

    // b1
    ne10_fft_cpx_float16_t cc_out[4];
    ne10_fft_cpx_float16_t cc_in [4];
    ne10_float16_t *p_dst_r = (ne10_float16_t*) dst;
    p_dst_r += 4;

    // load
    cc_out[0] = src[1];
    src += (nfft>>2);
    cc_out[2] = src[ 1];
    cc_out[3] = src[-1];
    src += (nfft>>2);
    cc_out[1] = src[-1];

    // NE10_PRINT_Q2_VECTOR(cc_out);

    NE10_FFT_C2R_CC_CC(cc_in,cc_out);

    // NE10_PRINT_Q2_VECTOR(cc_in);

    // twiddles[0] = ( 1.0, 0.0);
    // NE10_CPX_MUL_F16(cc_in[0],cc_out[0],twiddles[0]);
    cc_out[0] = cc_in[0];
    twiddles ++;

    NE10_CPX_CONJ_MUL_F16(cc_out[1],cc_in[1],twiddles[0]);
    twiddles ++;

    NE10_CPX_CONJ_MUL_F16(cc_out[2],cc_in[2],twiddles[0]);
    twiddles ++;

    NE10_CPX_CONJ_MUL_F16(cc_out[3],cc_in[3],twiddles[0]);

    // NE10_PRINT_Q2_VECTOR(cc_out);

    *(p_dst_r ++) = cc_out[0].r;
    *(p_dst_r ++) = cc_out[1].r;
    *(p_dst_r ++) = cc_out[2].r;
    *(p_dst_r ++) = cc_out[3].r;

    *(p_dst_r ++) = cc_out[0].i;
    *(p_dst_r ++) = cc_out[1].i;
    *(p_dst_r ++) = cc_out[2].i;
    *(p_dst_r ++) = cc_out[3].i;
}

static void arm_ne10_radix4_r2c_with_twiddles_last_stage_second_butterfly (
    ne10_fft_cpx_float16_t *dst,
    const ne10_fft_cpx_float16_t *src,
    const ne10_fft_cpx_float16_t *twiddles,
    const ne10_int32_t nfft)
{
    // assert ( nfft % 4 == 0 );
    const ne10_float16_t *fin_r  = (const ne10_float16_t*) src + 12;
          ne10_float16_t *fout_r =       (ne10_float16_t*) dst;
    const ne10_float16_t *tw     = (const ne10_float16_t*) twiddles + 8;

    ne10_float16_t q_in0[4],    q_out0[4],
                   q_in1[4],    q_out1[4],
                   q_in2[4],    q_out2[4],
                   q_in3[4],    q_out3[4];

    ne10_float16_t q2_tw0[2][4],
                   q2_tw1[2][4];

    /*  INPUT & OUTPUT
     *  0R  1R  2R  3R      Q0
     *  0I  1I  2I  3I      Q1
     *  4R  5R  6R  7R      Q2
     *  4I  5I  6I  7I      Q3
     */

    q_in0[0] = *(fin_r++);
    q_in0[1] = *(fin_r++);
    q_in0[2] = *(fin_r++);
    q_in0[3] = *(fin_r++);
    q_in1[0] = *(fin_r++);
    q_in1[1] = *(fin_r++);
    q_in1[2] = *(fin_r++);
    q_in1[3] = *(fin_r++);
    q_in2[0] = *(fin_r++);
    q_in2[1] = *(fin_r++);
    q_in2[2] = *(fin_r++);
    q_in2[3] = *(fin_r++);
    q_in3[0] = *(fin_r++);
    q_in3[1] = *(fin_r++);
    q_in3[2] = *(fin_r++);
    q_in3[3] = *(fin_r++);

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);

    q2_tw0[0][0] = tw[0];
    q2_tw0[0][1] = tw[2];
    q2_tw0[0][2] = tw[4];
    q2_tw0[0][3] = tw[6];
    q2_tw0[1][0] = tw[1];
    q2_tw0[1][1] = tw[3];
    q2_tw0[1][2] = tw[5];
    q2_tw0[1][3] = tw[7];

    q2_tw1[0][0] = tw[0+8];
    q2_tw1[0][1] = tw[2+8];
    q2_tw1[0][2] = tw[4+8];
    q2_tw1[0][3] = tw[6+8];
    q2_tw1[1][0] = tw[1+8];
    q2_tw1[1][1] = tw[3+8];
    q2_tw1[1][2] = tw[5+8];
    q2_tw1[1][3] = tw[7+8];

    // TW: in->out
    q_out0[0] = q_in0[0];
    q_out1[0] = q_in1[0];
    q_out2[0] = q_in2[0];
    q_out3[0] = q_in3[0];

    //----------------------------------------------------------//
    // first 2 lines
    //   R          R             R           I             I
    q_out0[1] = (_Float16)q_in0[1] * (_Float16)q2_tw0[0][1] - (_Float16)q_in1[1] * (_Float16)q2_tw0[1][1];
    //   I          R             I           I             R
    q_out1[1] = (_Float16)q_in0[1] * (_Float16)q2_tw0[1][1] + (_Float16)q_in1[1] * (_Float16)q2_tw0[0][1];

    //   R          R             R           I             I
    q_out0[2] = (_Float16)q_in0[2] * (_Float16)q2_tw0[0][2] - (_Float16)q_in1[2] * (_Float16)q2_tw0[1][2];
    //   I          R             I           I             R
    q_out1[2] = (_Float16)q_in0[2] * (_Float16)q2_tw0[1][2] + (_Float16)q_in1[2] * (_Float16)q2_tw0[0][2];

    //   R          R             R           I             I
    q_out0[3] = (_Float16)q_in0[3] * (_Float16)q2_tw0[0][3] - (_Float16)q_in1[3] * (_Float16)q2_tw0[1][3];
    //   I          R             I           I             R
    q_out1[3] = (_Float16)q_in0[3] * (_Float16)q2_tw0[1][3] + (_Float16)q_in1[3] * (_Float16)q2_tw0[0][3];

    //---------------------------------------------------------//
    // second 2 lines
    //   R          R             R           I             I
    q_out2[1] = (_Float16)q_in2[1] * (_Float16)q2_tw1[0][1] - (_Float16)q_in3[1] * (_Float16)q2_tw1[1][1];
    //   I          R             I           I             R
    q_out3[1] = (_Float16)q_in2[1] * (_Float16)q2_tw1[1][1] + (_Float16)q_in3[1] * (_Float16)q2_tw1[0][1];

    //   R          R             R           I             I
    q_out2[2] = (_Float16)q_in2[2] * (_Float16)q2_tw1[0][2] - (_Float16)q_in3[2] * (_Float16)q2_tw1[1][2];
    //   I          R             I           I             R
    q_out3[2] = (_Float16)q_in2[2] * (_Float16)q2_tw1[1][2] + (_Float16)q_in3[2] * (_Float16)q2_tw1[0][2];

    //   R          R             R           I             I
    q_out2[3] = (_Float16)q_in2[3] * (_Float16)q2_tw1[0][3] - (_Float16)q_in3[3] * (_Float16)q2_tw1[1][3];
    //   I          R             I           I             R
    q_out3[3] = (_Float16)q_in2[3] * (_Float16)q2_tw1[1][3] + (_Float16)q_in3[3] * (_Float16)q2_tw1[0][3];

    // NE10_PRINT_Q_VECTOR(q_out0);
    // NE10_PRINT_Q_VECTOR(q_out1);
    // NE10_PRINT_Q_VECTOR(q_out2);
    // NE10_PRINT_Q_VECTOR(q_out3);

    // BUTTERFLY - radix 4x2
    // STAGE
    // q_out -> q_in
    //  R i         R j         R k
    q_in0[0] = (_Float16)q_out0[0] + (_Float16)q_out0[2];
    q_in1[0] = (_Float16)q_out1[0] + (_Float16)q_out1[2];

    q_in0[1] = (_Float16)q_out0[0] - (_Float16)q_out0[2];
    q_in1[1] = (_Float16)q_out1[0] - (_Float16)q_out1[2];

    //  R i         R j         R k
    q_in0[2] = (_Float16)q_out0[1] + (_Float16)q_out0[3];
    q_in1[2] = (_Float16)q_out1[1] + (_Float16)q_out1[3];

    q_in0[3] = (_Float16)q_out0[1] - (_Float16)q_out0[3];
    q_in1[3] = (_Float16)q_out1[1] - (_Float16)q_out1[3];

    //  R i         R j         R k
    q_in2[0] = (_Float16)q_out2[0] + (_Float16)q_out2[2];
    q_in3[0] = (_Float16)q_out3[0] + (_Float16)q_out3[2];

    q_in2[1] = (_Float16)q_out2[0] - (_Float16)q_out2[2];
    q_in3[1] = (_Float16)q_out3[0] - (_Float16)q_out3[2];

    //  R i         R j         R k
    q_in2[2] = (_Float16)q_out2[1] + (_Float16)q_out2[3];
    q_in3[2] = (_Float16)q_out3[1] + (_Float16)q_out3[3];

    q_in2[3] = (_Float16)q_out2[1] - (_Float16)q_out2[3];
    q_in3[3] = (_Float16)q_out3[1] - (_Float16)q_out3[3];

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);

    // STAGE
    // q_in -> q_out
    // and transpose
    //   R i          R j        R k
    q_out0[0] =   (_Float16)q_in0[0] + (_Float16)q_in0[2];
    q_out0[1] =   (_Float16)q_in1[0] + (_Float16)q_in1[2];

    q_out2[2] =   (_Float16)q_in0[0] - (_Float16)q_in0[2];
    q_out2[3] = - (_Float16)q_in1[0] + (_Float16)q_in1[2];// CONJ

    //   R i          R j        R k
    q_out3[2] =   (_Float16)q_in0[1] - (_Float16)q_in1[3];
    q_out3[3] = - (_Float16)q_in1[1] - (_Float16)q_in0[3];// CONJ

    q_out1[0] =   (_Float16)q_in0[1] + (_Float16)q_in1[3];
    q_out1[1] =   (_Float16)q_in1[1] - (_Float16)q_in0[3];

    //   R i          R j        R k
    q_out0[2] =   (_Float16)q_in2[0] + (_Float16)q_in2[2];
    q_out0[3] =   (_Float16)q_in3[0] + (_Float16)q_in3[2];

    q_out2[0] =   (_Float16)q_in2[0] - (_Float16)q_in2[2];
    q_out2[1] = - (_Float16)q_in3[0] + (_Float16)q_in3[2];// CONJ

    //   R i          R j        R k
    q_out3[0] =   (_Float16)q_in2[1] - (_Float16)q_in3[3];
    q_out3[1] = - (_Float16)q_in3[1] - (_Float16)q_in2[3]; // CONJ

    q_out1[2] =   (_Float16)q_in2[1] + (_Float16)q_in3[3];
    q_out1[3] =   (_Float16)q_in3[1] - (_Float16)q_in2[3];

    // NE10_PRINT_Q_VECTOR(q_out0);
    // NE10_PRINT_Q_VECTOR(q_out1);
    // NE10_PRINT_Q_VECTOR(q_out2);
    // NE10_PRINT_Q_VECTOR(q_out3);

    // STORE
    fout_r += 4;
    fout_r[0] = q_out0[0];
    fout_r[1] = q_out0[1];
    fout_r[2] = q_out0[2];
    fout_r[3] = q_out0[3];

    fout_r += (nfft>>1);
    fout_r[0] = q_out1[0];
    fout_r[1] = q_out1[1];
    fout_r[2] = q_out1[2];
    fout_r[3] = q_out1[3];

    fout_r -= 10;
    fout_r[0] = q_out3[0];
    fout_r[1] = q_out3[1];
    fout_r[2] = q_out3[2];
    fout_r[3] = q_out3[3];

    fout_r += (nfft>>1);
    fout_r[0] = q_out2[0];
    fout_r[1] = q_out2[1];
    fout_r[2] = q_out2[2];
    fout_r[3] = q_out2[3];
}

static void arm_ne10_radix4_c2r_with_twiddles_first_stage_second_butterfly (
    ne10_fft_cpx_float16_t *dst,
    const ne10_fft_cpx_float16_t *src,
    const ne10_fft_cpx_float16_t *twiddles,
    const ne10_int32_t nfft)
{
    const ne10_float16_t *fin_r  = (const ne10_float16_t*) src;
          ne10_float16_t *fout_r =       (ne10_float16_t*) dst + 12;
    const ne10_float16_t *tw     = (const ne10_float16_t*) twiddles + 8;

    ne10_float16_t q_in0[4],    q_out0[4],
                   q_in1[4],    q_out1[4],
                   q_in2[4],    q_out2[4],
                   q_in3[4],    q_out3[4];

    ne10_float16_t q2_tw0[2][4],
                   q2_tw1[2][4];

    /*  INPUT & OUTPUT
     *  0R  1R  2R  3R      Q0
     *  0I  1I  2I  3I      Q1
     *  4R  5R  6R  7R      Q2
     *  4I  5I  6I  7I      Q3
     */

    // load
    fin_r += 4;
    q_in0[0] = fin_r[0];
    q_in0[1] = fin_r[1];
    q_in0[2] = fin_r[2];
    q_in0[3] = fin_r[3];

    fin_r += (nfft>>1);
    q_in1[0] = fin_r[0];
    q_in1[1] = fin_r[1];
    q_in1[2] = fin_r[2];
    q_in1[3] = fin_r[3];

    fin_r -= 10;
    q_in3[0] = fin_r[0];
    q_in3[1] = fin_r[1];
    q_in3[2] = fin_r[2];
    q_in3[3] = fin_r[3];

    fin_r += (nfft>>1);
    q_in2[0] = fin_r[0];
    q_in2[1] = fin_r[1];
    q_in2[2] = fin_r[2];
    q_in2[3] = fin_r[3];

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);

    // OUTPUT
    // INPUT
#define NE10_INV_BUTTERFLY_TMP(I1,I2,J1,J2,K1,K2,S1,S2) do {    \
    q_out ## I1 [I2] = ( (_Float16)q_in ## K1 [K2] + (_Float16)q_in ## S1 [S2] ); \
    q_out ## J1 [J2] = ( (_Float16)q_in ## K1 [K2] - (_Float16)q_in ## S1 [S2] ); \
} while(0);

    // STAGE
    // q_in -> q_out
    // and transpose
    NE10_INV_BUTTERFLY_TMP( 0,0, 0,2,
                            0,0, 2,2);

    NE10_INV_BUTTERFLY_TMP( 1,2, 1,0,
                            0,1, 2,3);

    NE10_INV_BUTTERFLY_TMP( 0,1, 1,3,
                            1,0, 3,2);

    q_in3[3] *= - 1.0f;
    NE10_INV_BUTTERFLY_TMP( 1,1, 0,3,
                            3,3, 1,1);

    NE10_INV_BUTTERFLY_TMP( 2,0, 2,2,
                            0,2, 2,0);

    NE10_INV_BUTTERFLY_TMP( 3,2, 3,0,
                            0,3, 2,1);

    NE10_INV_BUTTERFLY_TMP( 2,1, 3,3,
                            1,2, 3,0);

    q_in3[1] *= - 1.0f;
    NE10_INV_BUTTERFLY_TMP( 3,1, 2,3,
                            3,1, 1,3);
#undef NE10_INV_BUTTERFLY_TMP

    // NE10_PRINT_Q_VECTOR(q_out0);
    // NE10_PRINT_Q_VECTOR(q_out1);
    // NE10_PRINT_Q_VECTOR(q_out2);
    // NE10_PRINT_Q_VECTOR(q_out3);

    // BUTTERFLY - radix 4x2
    // STAGE
    // q_out -> q_in

    // OUTPUT
    // INPUT
#define NE10_INV_BUTTERFLY_TMP(I1,I2,J1,J2,K1,K2,S1,S2) do {    \
    q_in ## I1 [I2] = ( (_Float16)q_out ## K1 [K2] + (_Float16)q_out ## S1 [S2] ); \
    q_in ## J1 [J2] = ( (_Float16)q_out ## K1 [K2] - (_Float16)q_out ## S1 [S2] ); \
} while(0);

    NE10_INV_BUTTERFLY_TMP(0,0, 0,2,
                           0,0, 0,1);

    NE10_INV_BUTTERFLY_TMP(1,0, 1,2,
                           1,0, 1,1);

    NE10_INV_BUTTERFLY_TMP(0,1, 0,3,
                           0,2, 0,3);

    NE10_INV_BUTTERFLY_TMP(1,1, 1,3,
                           1,2, 1,3);

    NE10_INV_BUTTERFLY_TMP(2,0, 2,2,
                           2,0, 2,1);

    NE10_INV_BUTTERFLY_TMP(3,0, 3,2,
                           3,0, 3,1);


    NE10_INV_BUTTERFLY_TMP(2,1, 2,3,
                           2,2, 2,3);

    NE10_INV_BUTTERFLY_TMP(3,1, 3,3,
                           3,2, 3,3);

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);
#undef NE10_INV_BUTTERFLY_TMP

    // load tw
    q2_tw0[0][0] = tw[0];
    q2_tw0[0][1] = tw[2];
    q2_tw0[0][2] = tw[4];
    q2_tw0[0][3] = tw[6];
    q2_tw0[1][0] = tw[1];
    q2_tw0[1][1] = tw[3];
    q2_tw0[1][2] = tw[5];
    q2_tw0[1][3] = tw[7];

    q2_tw1[0][0] = tw[0+8];
    q2_tw1[0][1] = tw[2+8];
    q2_tw1[0][2] = tw[4+8];
    q2_tw1[0][3] = tw[6+8];
    q2_tw1[1][0] = tw[1+8];
    q2_tw1[1][1] = tw[3+8];
    q2_tw1[1][2] = tw[5+8];
    q2_tw1[1][3] = tw[7+8];

    // TW: in->out
    q_out0[0] = q_in0[0];
    q_out1[0] = q_in1[0];
    q_out2[0] = q_in2[0];
    q_out3[0] = q_in3[0];

    //----------------------------------------------------------//
    // first 2 lines
    //   R          R             R           I             I
    q_out0[1] = (_Float16)q_in0[1] * (_Float16)q2_tw0[0][1] + (_Float16)q_in1[1] * (_Float16)q2_tw0[1][1];
    //   I          R             I           I             R
    q_out1[1] = (_Float16)q_in0[1] * (_Float16)q2_tw0[1][1] - (_Float16)q_in1[1] * (_Float16)q2_tw0[0][1];

    //   R          R             R           I             I
    q_out0[2] = (_Float16)q_in0[2] * (_Float16)q2_tw0[0][2] + (_Float16)q_in1[2] * (_Float16)q2_tw0[1][2];
    //   I          R             I           I             R
    q_out1[2] = (_Float16)q_in0[2] * (_Float16)q2_tw0[1][2] - (_Float16)q_in1[2] * (_Float16)q2_tw0[0][2];

    //   R          R             R           I             I
    q_out0[3] = (_Float16)q_in0[3] * (_Float16)q2_tw0[0][3] + (_Float16)q_in1[3] * (_Float16)q2_tw0[1][3];
    //   I          R             I           I             R
    q_out1[3] = (_Float16)q_in0[3] * (_Float16)q2_tw0[1][3] - (_Float16)q_in1[3] * (_Float16)q2_tw0[0][3];

    //----------------------------------------------------------//
    // second 2 lines
    //   R          R             R           I             I
    q_out2[1] = (_Float16)q_in2[1] * (_Float16)q2_tw1[0][1] + (_Float16)q_in3[1] * (_Float16)q2_tw1[1][1];
    //   I          R             I           I             R
    q_out3[1] = (_Float16)q_in2[1] * (_Float16)q2_tw1[1][1] - (_Float16)q_in3[1] * (_Float16)q2_tw1[0][1];

    //   R          R             R           I             I
    q_out2[2] = (_Float16)q_in2[2] * (_Float16)q2_tw1[0][2] + (_Float16)q_in3[2] * (_Float16)q2_tw1[1][2];
    //   I          R             I           I             R
    q_out3[2] = (_Float16)q_in2[2] * (_Float16)q2_tw1[1][2] - (_Float16)q_in3[2] * (_Float16)q2_tw1[0][2];

    //   R          R             R           I             I
    q_out2[3] = (_Float16)q_in2[3] * (_Float16)q2_tw1[0][3] + (_Float16)q_in3[3] * (_Float16)q2_tw1[1][3];
    //   I          R             I           I             R
    q_out3[3] = (_Float16)q_in2[3] * (_Float16)q2_tw1[1][3] - (_Float16)q_in3[3] * (_Float16)q2_tw1[0][3];

    // STORE
    *(fout_r++) =   q_out0[0];
    *(fout_r++) =   q_out0[1];
    *(fout_r++) =   q_out0[2];
    *(fout_r++) =   q_out0[3];
    *(fout_r++) =   q_out1[0];
    *(fout_r++) = - (_Float16)q_out1[1];
    *(fout_r++) = - (_Float16)q_out1[2];
    *(fout_r++) = - (_Float16)q_out1[3];
    *(fout_r++) =   q_out2[0];
    *(fout_r++) =   q_out2[1];
    *(fout_r++) =   q_out2[2];
    *(fout_r++) =   q_out2[3];
    *(fout_r++) =   q_out3[0];
    *(fout_r++) = - (_Float16)q_out3[1];
    *(fout_r++) = - (_Float16)q_out3[2];
    *(fout_r++) = - (_Float16)q_out3[3];
}

static void arm_ne10_radix4_r2c_with_twiddles_last_stage_other_butterfly (
    ne10_fft_cpx_float16_t *dst,
    const ne10_fft_cpx_float16_t *src,
    const ne10_fft_cpx_float16_t *twiddles,
    const ne10_int32_t nfft)
{
    const ne10_float16_t *fin_r = ((const ne10_float16_t*) src) + 12 + 16;
    ne10_float16_t *fout_r = (ne10_float16_t*) dst + 8;
    ne10_float16_t *fout_b = (ne10_float16_t*) dst - 14;
    const ne10_float16_t *tw = ((const ne10_float16_t*) twiddles) + 8  + 16;

    // Take 4 elements as a set.
    // The leading 8 sets are already transformed in first and seconds butterflies.
    // This function transforms 8 sets in each loop.
    ne10_int32_t loop_count = ((nfft >> 2) - 8) >> 3;

    for (; loop_count > 0; loop_count--)
    {
        NE10_DECLARE_4 (float16x4x2_t, q2_in);    // 8Q
        NE10_DECLARE_3 (float16x4x2_t, q2_tw);    // 6Q
        NE10_DECLARE_4 (float16x4x2_t, q2_out);   // 8Q

        /*  INPUT
         *  0R  1R  2R  3R      Q0
         *  0I  1I  2I  3I      Q1
         *  4R  5R  6R  7R      Q2
         *  4I  5I  6I  7I      Q3
         *  8R  9R  aR  bR      Q4
         *  8I  9I  aI  bI      Q5
         *  cR  dR  eR  fR      Q6
         *  cI  dI  eI  fI      Q7
         */

        // transpose
        // q2_out -> q2_in
        /*
         *      val[0]
         *  0R  4R  8R  cR      Q0
         *  1R  5R  9R  dR      Q2
         *  2R  6R  aR  eR      Q4
         *  3R  7R  bR  fR      Q6
         *
         *      val[1]
         *  0I  4I  8I  cI      Q1
         *  1I  5I  9I  dI      Q3
         *  2I  6I  aI  eI      Q5
         *  3I  7I  bI  fI      Q7
         */

        q2_out0.val[0] = vld1_f16 (fin_r);
        fin_r += 4;
        q2_out0.val[1] = vld1_f16 (fin_r);
        fin_r += 4;
        q2_out1.val[0] = vld1_f16 (fin_r);
        fin_r += 4;
        q2_out1.val[1] = vld1_f16 (fin_r);
        fin_r += 4;
        q2_out2.val[0] = vld1_f16 (fin_r);
        fin_r += 4;
        q2_out2.val[1] = vld1_f16 (fin_r);
        fin_r += 4;
        q2_out3.val[0] = vld1_f16 (fin_r);
        fin_r += 4;
        q2_out3.val[1] = vld1_f16 (fin_r);
        fin_r += 4;

        NE10_RADIX4X4C_TRANSPOSE_NEON (q2_in, q2_out);


        // Load twiddles
        q2_tw0 = vld2_f16 (tw);
        tw += 8;
        q2_tw1 = vld2_f16 (tw);
        tw += 8;
        q2_tw2 = vld2_f16 (tw);
        tw += 8;

        // tw
        // q2_in -> q2_out
        q2_out0 = q2_in0;
        NE10_CPX_MUL_NEON_F16 (q2_out1, q2_in1, q2_tw0);
        NE10_CPX_MUL_NEON_F16 (q2_out2, q2_in2, q2_tw1);
        NE10_CPX_MUL_NEON_F16 (q2_out3, q2_in3, q2_tw2);

        // butterfly
        // out -> in
        q2_in0.val[0] = vadd_f16 (q2_out0.val[0], q2_out2.val[0]);
        q2_in0.val[1] = vadd_f16 (q2_out0.val[1], q2_out2.val[1]);
        q2_in1.val[0] = vsub_f16 (q2_out0.val[0], q2_out2.val[0]);
        q2_in1.val[1] = vsub_f16 (q2_out0.val[1], q2_out2.val[1]);
        q2_in2.val[0] = vadd_f16 (q2_out1.val[0], q2_out3.val[0]);
        q2_in2.val[1] = vadd_f16 (q2_out1.val[1], q2_out3.val[1]);
        q2_in3.val[0] = vsub_f16 (q2_out1.val[0], q2_out3.val[0]);
        q2_in3.val[1] = vsub_f16 (q2_out1.val[1], q2_out3.val[1]);

        // in -> out
        q2_out2.val[0] = vsub_f16 (q2_in0.val[0], q2_in2.val[0]);
        q2_out2.val[1] = vsub_f16 (q2_in0.val[1], q2_in2.val[1]);
        q2_out3.val[0] = vsub_f16 (q2_in1.val[0], q2_in3.val[1]);
        q2_out3.val[1] = vadd_f16 (q2_in1.val[1], q2_in3.val[0]);

        q2_out3.val[1] = vneg_f16 (q2_out3.val[1]);
        q2_out2.val[1] = vneg_f16 (q2_out2.val[1]);

        q2_out0.val[0] = vadd_f16 (q2_in0.val[0], q2_in2.val[0]);
        q2_out0.val[1] = vadd_f16 (q2_in0.val[1], q2_in2.val[1]);

        q2_out1.val[0] = vadd_f16 (q2_in1.val[0], q2_in3.val[1]);
        q2_out1.val[1] = vsub_f16 (q2_in1.val[1], q2_in3.val[0]);

        // reverse -- CONJ
        NE10_REVERSE_FLOAT16X4 (q2_out2.val[0]);
        NE10_REVERSE_FLOAT16X4 (q2_out2.val[1]);
        NE10_REVERSE_FLOAT16X4 (q2_out3.val[0]);
        NE10_REVERSE_FLOAT16X4 (q2_out3.val[1]);

        // store
        vst2_f16 (fout_r, q2_out0);
        vst2_f16 (fout_r + (nfft >> 1), q2_out1);
        vst2_f16 (fout_b + (nfft >> 1), q2_out3);
        vst2_f16 (fout_b + nfft, q2_out2);

        fout_r += 8;
        fout_b -= 8;
    }
}

static void arm_ne10_radix4_c2r_with_twiddles_first_stage_other_butterfly (
    ne10_fft_cpx_float16_t *dst,
    const ne10_fft_cpx_float16_t *src,
    const ne10_fft_cpx_float16_t *twiddles,
    const ne10_int32_t nfft)
{
          ne10_float16_t *fout_r  =       ((ne10_float16_t*) dst ) + 12 + 16 ;
    const ne10_float16_t *fin_r =    (const ne10_float16_t*) src + 8;
    const ne10_float16_t *fin_b =    (const ne10_float16_t*) src - 14;
    const ne10_float16_t *tw     = ((const ne10_float16_t*) twiddles) + 8  + 16;
    ne10_int32_t loop_count = ((nfft>>2)-8)>>3;

    for ( ; loop_count>0; loop_count -- )
    {
        NE10_DECLARE_4(float16x4x2_t,q2_in);    // 8Q
        NE10_DECLARE_3(float16x4x2_t,q2_tw);    // 6Q
        NE10_DECLARE_4(float16x4x2_t,q2_out);   // 8Q

        /*  INPUT
         *  0R  1R  2R  3R      Q0
         *  0I  1I  2I  3I      Q1
         *  4R  5R  6R  7R      Q2
         *  4I  5I  6I  7I      Q3
         *  8R  9R  aR  bR      Q4
         *  8I  9I  aI  bI      Q5
         *  cR  dR  eR  fR      Q6
         *  cI  dI  eI  fI      Q7
         */

        q2_in0 = vld2_f16(fin_r            );
        q2_in1 = vld2_f16(fin_r + (nfft>>1));
        fin_r += 8;

        q2_in3 = vld2_f16(fin_b + (nfft>>1));
        q2_in2 = vld2_f16(fin_b + nfft     );
        fin_b -= 8;

        q2_tw0 = vld2_f16(tw);
        tw += 8;
        q2_tw1 = vld2_f16(tw);
        tw += 8;
        q2_tw2 = vld2_f16(tw);
        tw += 8;

        // reverse -- CONJ
        NE10_REVERSE_FLOAT16X4( q2_in3.val[0] );
        NE10_REVERSE_FLOAT16X4( q2_in3.val[1] );
        NE10_REVERSE_FLOAT16X4( q2_in2.val[0] );
        NE10_REVERSE_FLOAT16X4( q2_in2.val[1] );

        q2_in2.val[1] = vneg_f16( q2_in2.val[1] );
        q2_in3.val[1] = vneg_f16( q2_in3.val[1] );

        // in -> out
        q2_out0.val[0] = vadd_f16 (q2_in0.val[0], q2_in2.val[0]);
        q2_out2.val[0] = vsub_f16 (q2_in0.val[0], q2_in2.val[0]);

        q2_out0.val[1] = vadd_f16 (q2_in0.val[1], q2_in2.val[1]);
        q2_out2.val[1] = vsub_f16 (q2_in0.val[1], q2_in2.val[1]);

        q2_out1.val[0] = vadd_f16 (q2_in1.val[0], q2_in3.val[0]);
        q2_out3.val[1] = vsub_f16 (q2_in1.val[0], q2_in3.val[0]);

        q2_out1.val[1] = vadd_f16 (q2_in3.val[1], q2_in1.val[1]);
        q2_out3.val[0] = vsub_f16 (q2_in3.val[1], q2_in1.val[1]);

        // out -> in
        q2_in0.val[0] = vadd_f16 (q2_out0.val[0], q2_out1.val[0]);
        q2_in2.val[0] = vsub_f16 (q2_out0.val[0], q2_out1.val[0]);

        q2_in0.val[1] = vadd_f16 (q2_out0.val[1], q2_out1.val[1]);
        q2_in2.val[1] = vsub_f16 (q2_out0.val[1], q2_out1.val[1]);

        q2_in1.val[0] = vadd_f16 (q2_out2.val[0], q2_out3.val[0]);
        q2_in3.val[0] = vsub_f16 (q2_out2.val[0], q2_out3.val[0]);

        q2_in1.val[1] = vadd_f16 (q2_out2.val[1], q2_out3.val[1]);
        q2_in3.val[1] = vsub_f16 (q2_out2.val[1], q2_out3.val[1]);

        // tw
        // q2_in -> q2_out
        q2_out0 = q2_in0;
        NE10_CPX_MUL_INV_NEON_F16(q2_out1,q2_in1,q2_tw0);
        NE10_CPX_MUL_INV_NEON_F16(q2_out2,q2_in2,q2_tw1);
        NE10_CPX_MUL_INV_NEON_F16(q2_out3,q2_in3,q2_tw2);

        // transpose
        // q2_out -> q2_in
        NE10_RADIX4X4C_TRANSPOSE_NEON (q2_in,q2_out);

        // store
        vst1_f16(fout_r, q2_in0.val[0]);
        fout_r += 4;
        vst1_f16(fout_r, q2_in0.val[1]);
        fout_r += 4;
        vst1_f16(fout_r, q2_in1.val[0]);
        fout_r += 4;
        vst1_f16(fout_r, q2_in1.val[1]);
        fout_r += 4;
        vst1_f16(fout_r, q2_in2.val[0]);
        fout_r += 4;
        vst1_f16(fout_r, q2_in2.val[1]);
        fout_r += 4;
        vst1_f16(fout_r, q2_in3.val[0]);
        fout_r += 4;
        vst1_f16(fout_r, q2_in3.val[1]);
        fout_r += 4;
    }
}


static void arm_ne10_radix4_r2c_with_twiddles_last_stage( 
    const arm_rfft_fast_instance_f16 * S,
    const ne10_fft_cpx_float16_t *src,
    ne10_fft_cpx_float16_t *dst
                                            )
{
    const ne10_fft_cpx_float16_t *twiddles = (const ne10_fft_cpx_float16_t *)S->r_super_twiddles_neon;
    const int32_t nfft = S->nfft;

    arm_ne10_radix4_r2c_with_twiddles_last_stage_first_butterfly(dst,src,twiddles,nfft);

    if (nfft==16)
    {
        return;
    }

    arm_ne10_radix4_r2c_with_twiddles_last_stage_second_butterfly(dst,src,twiddles,nfft);

    if (nfft==32)
    {
        return;
    }

    arm_ne10_radix4_r2c_with_twiddles_last_stage_other_butterfly(dst,src,twiddles,nfft);
}

static void arm_ne10_radix4_c2r_with_twiddles_first_stage( 
    ne10_fft_cpx_float16_t *dst,
    const ne10_fft_cpx_float16_t *src,
    const ne10_fft_cpx_float16_t *twiddles,
    const ne10_int32_t nfft)
{
    arm_ne10_radix4_c2r_with_twiddles_first_stage_first_butterfly(dst,src,twiddles,nfft);

    if (nfft==16)
    {
        return;
    }

    arm_ne10_radix4_c2r_with_twiddles_first_stage_second_butterfly(dst,src,twiddles,nfft);

    if (nfft==32)
    {
        return;
    }

    arm_ne10_radix4_c2r_with_twiddles_first_stage_other_butterfly(dst,src,twiddles,nfft);
}

/**
 * @ingroup R2C_FFT_IFFT
 * Specific implementation of @ref ne10_fft_r2c_1d_float16 using NEON SIMD capabilities.
 */
void arm_ne10_fft_r2c_1d_float16_neon (
    const arm_rfft_fast_instance_f16 * S,
    const float16_t *fin,
    float16_t *out,
    float16_t *tmpbuf)
{
    ne10_fft_cpx_float16_t *fout=(ne10_fft_cpx_float16_t*)out;

    typedef ne10_fft_cpx_float16_t CPLX;

    memset(out,0,sizeof(float16_t)*S->nfft);

    arm_ne10_mixed_radix_r2c_butterfly_float16_neon (S,(CPLX*) fin, (CPLX*)fout,(CPLX*)tmpbuf);
    arm_ne10_radix4_r2c_with_twiddles_last_stage(S,(const CPLX*)tmpbuf,(CPLX*)out);
    
    //fout[S->nfft / 2].r = fout[0].i;

    //fout[0].i = fout[S->nfft / 2].i = 0.0f;

}

/**
 * @ingroup R2C_FFT_IFFT
 * Specific implementation of @ref ne10_fft_c2r_1d_float16 using NEON SIMD capabilities.
 */
void arm_ne10_fft_c2r_1d_float16_neon (
    const arm_rfft_fast_instance_f16 * S,
    const float16_t *in,
    float16_t *fout,
    float16_t *buffer)
{
    //typedef         ne10_float16_t REAL;
    typedef ne10_fft_cpx_float16_t CPLX;

    ne10_fft_cpx_float16_t * fin = (ne10_fft_cpx_float16_t *)in;
    ne10_fft_cpx_float16_t * tmpbuf = (ne10_fft_cpx_float16_t *)buffer;
    ne10_fft_cpx_float16_t * fout_c;
    ne10_int32_t stage_count;
    ne10_int32_t radix;


    stage_count = S->r_factors_neon[0];
    radix       = S->r_factors_neon[  stage_count << 1 ];
    if (radix==2)
    {
        stage_count --;
    }
    //fin[0].i = fin[S->nfft>>1].r;
    fout_c = (stage_count % 2==1) ? tmpbuf : (CPLX*)fout;
    arm_ne10_radix4_c2r_with_twiddles_first_stage( (CPLX*) fout_c, fin, (const CPLX*)S->r_super_twiddles_neon, S->nfft);
    arm_ne10_mixed_radix_c2r_butterfly_float16_neon ((CPLX*) fout,(CPLX*) NULL, S->r_factors_neon, (const CPLX*)S->r_twiddles_neon_backward,(CPLX*)tmpbuf);

    //fin[0].i = 0.0f;
}

#endif