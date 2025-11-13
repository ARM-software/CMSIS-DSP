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

/*
 * NE10 Library : dsp/NE10_fft_float32.neon.c
 * Small modifications for inclusion in CMSIS-DSP
 */
#include "arm_compiler_specific.h"
#include <arm_neon.h>

#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_fft.h"

#include "dsp/transform_functions.h"

//#include <stdio.h>

#define CMPLX_VEC_F32 float32x4x2_t

#if 0 //defined(__ARM_FEATURE_COMPLEX)

#define CMPLX_LOAD_F32(DST,PTR)\
DST.val[0] = vld1q_f32 (PTR);  \
DST.val[1] = vld1q_f32 (PTR+4);

#define CMPLX_LOAD_INC_F32(DST,PTR)\
DST.val[0] = vld1q_f32 (PTR);      \
PTR += 4;                          \
DST.val[1] = vld1q_f32 (PTR);    \
PTR += 4;

#define CMPLX_STORE_F32(PTR,SRC)\
vst1q_f32 (PTR, SRC.val[0]);    \
vst1q_f32 (PTR+4, SRC.val[1]);

#define CMPLX_STORE_INC_F32(PTR,SRC)\
vst1q_f32 (PTR, SRC.val[0]);        \
PTR += 4;                           \
vst1q_f32 (PTR, SRC.val[1]);        \
PTR += 4;

#define CMPLX_ADD_F32(DST,SRCA,SRCB)              \
DST.val[0] = vaddq_f32 (SRCA.val[0], SRCB.val[0]);\
DST.val[1] = vaddq_f32 (SRCA.val[1], SRCB.val[1]);

#define CMPLX_SUB_F32(DST,SRCA,SRCB)              \
DST.val[0] = vsubq_f32 (SRCA.val[0], SRCB.val[0]);\
DST.val[1] = vsubq_f32 (SRCA.val[1], SRCB.val[1]);

// a + i b
#define CMPLX_ADD_ROT_F32(DST,SRCA,SRCB);         \
DST.val[0] = vcaddq_rot90_f32 (SRCA.val[0], SRCB.val[0]);\
DST.val[1] = vcaddq_rot90_f32 (SRCA.val[1], SRCB.val[1]);

// a - i b
#define CMPLX_SUB_ROT_F32(DST,SRCA,SRCB);         \
DST.val[0] = vcaddq_rot270_f32 (SRCA.val[0], SRCB.val[0]);\
DST.val[1] = vcaddq_rot270_f32 (SRCA.val[1], SRCB.val[1]);

#define CMPLX_MUL_F32(DST,SRCA,SRCB)                                 \
DST.val[0] = vdupq_n_f32 (0.0);                                      \
DST.val[1] = vdupq_n_f32 (0.0);                                      \
DST.val[0] = vcmlaq_f32 (DST.val[0], SRCA.val[0], SRCB.val[0]);      \
DST.val[1] = vcmlaq_f32 (DST.val[1], SRCA.val[1], SRCB.val[1]);      \
DST.val[0] = vcmlaq_rot90_f32 (DST.val[0], SRCA.val[0], SRCB.val[0]);\
DST.val[1] = vcmlaq_rot90_f32 (DST.val[1], SRCA.val[1], SRCB.val[1]);

#define CMPLX_MAT_TRANSPOSE_4x4_F32(DST,SRC,TMP)                                                      \
{                                                                                                     \
    float32x4x2_t tmpb0,tmpb1,tmpb2,tmpb3;                                                            \
    tmpb0 = vzipq_f32(SRC ## 0 .val[0],SRC ## 0 .val[1]);                                             \
    tmpb1 = vzipq_f32(SRC ## 1 .val[0],SRC ## 1 .val[1]);                                             \
    tmpb2 = vzipq_f32(SRC ## 2 .val[0],SRC ## 2 .val[1]);                                             \
    tmpb3 = vzipq_f32(SRC ## 3 .val[0],SRC ## 3 .val[1]);                                             \
                                                                                                      \
    TMP##0 = vtrnq_f32 (tmpb0 .val[0], tmpb1 .val[0]);                                                \
    TMP##1 = vtrnq_f32 (tmpb0 .val[1], tmpb1 .val[1]);                                                \
    TMP##2 = vtrnq_f32 (tmpb2 .val[0], tmpb3 .val[0]);                                                \
    TMP##3 = vtrnq_f32 (tmpb2 .val[1], tmpb3 .val[1]);                                                \
    DST ##0 .val[0] = vcombine_f32 (vget_low_f32 (TMP ##0 .val[0]), vget_low_f32 (TMP ##2 .val[0]));  \
    DST ##0 .val[1] = vcombine_f32 (vget_low_f32 (TMP ##1 .val[0]), vget_low_f32 (TMP ##3 .val[0]));  \
    DST ##1 .val[0] = vcombine_f32 (vget_low_f32 (TMP ##0 .val[1]), vget_low_f32 (TMP ##2 .val[1]));  \
    DST ##1 .val[1] = vcombine_f32 (vget_low_f32 (TMP ##1 .val[1]), vget_low_f32 (TMP ##3 .val[1]));  \
    DST ##2 .val[0] = vcombine_f32 (vget_high_f32 (TMP ##0 .val[0]), vget_high_f32 (TMP ##2 .val[0]));\
    DST ##2 .val[1] = vcombine_f32 (vget_high_f32 (TMP ##1 .val[0]), vget_high_f32 (TMP ##3 .val[0]));\
    DST ##3 .val[0] = vcombine_f32 (vget_high_f32 (TMP ##0 .val[1]), vget_high_f32 (TMP ##2 .val[1]));\
    DST ##3 .val[1] = vcombine_f32 (vget_high_f32 (TMP ##1 .val[1]), vget_high_f32 (TMP ##3 .val[1]));\
}

#else

#define CMPLX_LOAD_F32(DST,PTR)\
DST = vld2q_f32 (PTR);

#define CMPLX_LOAD_INC_F32(DST,PTR)\
DST = vld2q_f32 (PTR);             \
PTR+=8;

#define CMPLX_STORE_F32(PTR,SRC)\
vst2q_f32 (PTR, SRC);

#define CMPLX_STORE_INC_F32(PTR,SRC)\
vst2q_f32 (PTR, SRC);           \
PTR+=8;

#define CMPLX_ADD_F32(DST,SRCA,SRCB)              \
DST.val[0] = vaddq_f32 (SRCA.val[0], SRCB.val[0]);\
DST.val[1] = vaddq_f32 (SRCA.val[1], SRCB.val[1]);

#define CMPLX_SUB_F32(DST,SRCA,SRCB)              \
DST.val[0] = vsubq_f32 (SRCA.val[0], SRCB.val[0]);\
DST.val[1] = vsubq_f32 (SRCA.val[1], SRCB.val[1]);

// a + i b
#define CMPLX_ADD_ROT_F32(DST,SRCA,SRCB);         \
DST.val[0] = vsubq_f32 (SRCA.val[0], SRCB.val[1]);\
DST.val[1] = vaddq_f32 (SRCA.val[1], SRCB.val[0]);

// a - i b
#define CMPLX_SUB_ROT_F32(DST,SRCA,SRCB);     \
DST.val[0] = vaddq_f32 (SRCA.val[0], SRCB.val[1]);\
DST.val[1] = vsubq_f32 (SRCA.val[1], SRCB.val[0]);

#define CMPLX_MUL_F32(DST,SRCA,SRCB)                          \
DST.val[0] = vmulq_f32 (SRCA.val[0], SRCB.val[0]);            \
DST.val[1] = vmulq_f32 (SRCA.val[1], SRCB.val[0]);            \
DST.val[0] = vmlsq_f32 (DST.val[0], SRCA.val[1], SRCB.val[1]);\
DST.val[1] = vmlaq_f32 (DST.val[1], SRCA.val[0], SRCB.val[1]);

#define CMPLX_MAT_TRANSPOSE_4x4_F32(DST,SRC,TMP)                                                \
TMP##0 = vtrnq_f32 (SRC ## 0 .val[0], SRC ## 1 .val[0]);                                        \
TMP##1 = vtrnq_f32 (SRC ## 0 .val[1], SRC ## 1 .val[1]);                                        \
TMP##2 = vtrnq_f32 (SRC ## 2 .val[0], SRC ## 3 .val[0]);                                        \
TMP##3 = vtrnq_f32 (SRC ## 2 .val[1], SRC ## 3 .val[1]);                                        \
DST ##0 .val[0] = vcombine_f32 (vget_low_f32 (TMP ##0 .val[0]), vget_low_f32 (TMP ##2 .val[0]));  \
DST ##0 .val[1] = vcombine_f32 (vget_low_f32 (TMP ##1 .val[0]), vget_low_f32 (TMP ##3 .val[0]));  \
DST ##1 .val[0] = vcombine_f32 (vget_low_f32 (TMP ##0 .val[1]), vget_low_f32 (TMP ##2 .val[1]));  \
DST ##1 .val[1] = vcombine_f32 (vget_low_f32 (TMP ##1 .val[1]), vget_low_f32 (TMP ##3 .val[1]));  \
DST ##2 .val[0] = vcombine_f32 (vget_high_f32 (TMP ##0 .val[0]), vget_high_f32 (TMP ##2 .val[0]));\
DST ##2 .val[1] = vcombine_f32 (vget_high_f32 (TMP ##1 .val[0]), vget_high_f32 (TMP ##3 .val[0]));\
DST ##3 .val[0] = vcombine_f32 (vget_high_f32 (TMP ##0 .val[1]), vget_high_f32 (TMP ##2 .val[1]));\
DST ##3 .val[1] = vcombine_f32 (vget_high_f32 (TMP ##1 .val[1]), vget_high_f32 (TMP ##3 .val[1]));
#endif


void arm_ne10_fft16_forward_float32_neon (const arm_cfft_instance_f32 *S,
          const ne10_fft_cpx_float32_t *Fin,
          ne10_fft_cpx_float32_t *Fout)
{
    const ne10_fft_cpx_float32_t *tw1;
    const ne10_fft_cpx_float32_t *tw2;
    const ne10_fft_cpx_float32_t *tw3;

    // the first stage
    const float32_t *p_src0, *p_src4, *p_src8, *p_src12;
    CMPLX_VEC_F32 q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    float32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    float32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    float32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (const float32_t*) (& (Fin[0]));
    p_src4 = (const float32_t*) (& (Fin[4]));
    p_src8 = (const float32_t*) (& (Fin[8]));
    p_src12 = (const float32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_f32 (p_src0);
    q2_in_4567 = vld2q_f32 (p_src4);
    q2_in_89ab = vld2q_f32 (p_src8);
    q2_in_cdef = vld2q_f32 (p_src12);

    q_t2_r = vsubq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vsubq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vaddq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vaddq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vaddq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vaddq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vsubq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vsubq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vsubq_f32 (q_t3_r, q_t0_r);
    q_out_i26ae = vsubq_f32 (q_t3_i, q_t0_i);
    q_out_r048c = vaddq_f32 (q_t3_r, q_t0_r);
    q_out_i048c = vaddq_f32 (q_t3_i, q_t0_i);
    q_out_r159d = vaddq_f32 (q_t2_r, q_t1_i);
    q_out_i159d = vsubq_f32 (q_t2_i, q_t1_r);
    q_out_r37bf = vsubq_f32 (q_t2_r, q_t1_i);
    q_out_i37bf = vaddq_f32 (q_t2_i, q_t1_r);

    // second stages
    float32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    float32_t *p_tw1, *p_tw2, *p_tw3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    float32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    CMPLX_VEC_F32 q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    float32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    float32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    CMPLX_VEC_F32 q2_tw1, q2_tw2, q2_tw3;
    CMPLX_VEC_F32 q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    tw1 = (const ne10_fft_cpx_float32_t *)S->pTwiddle;
    tw2 = (const ne10_fft_cpx_float32_t *)S->pTwiddle + 4;
    tw3 = (const ne10_fft_cpx_float32_t *)S->pTwiddle + 8;
    p_dst0 = (float32_t*) (&Fout[0]);
    p_dst1 = (float32_t*) (&Fout[4]);
    p_dst2 = (float32_t*) (&Fout[8]);
    p_dst3 = (float32_t*) (&Fout[12]);
    p_tw1 = (float32_t*) tw1;
    p_tw2 = (float32_t*) tw2;
    p_tw3 = (float32_t*) tw3;
    q2_tmp_0 = vzipq_f32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_f32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_f32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_f32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[0]), vget_low_f32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[0]), vget_low_f32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[0]), vget_high_f32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[0]), vget_high_f32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[1]), vget_low_f32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[1]), vget_low_f32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[1]), vget_high_f32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[1]), vget_high_f32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_f32 (p_tw1);
    q2_tw2 = vld2q_f32 (p_tw2);
    q2_tw3 = vld2q_f32 (p_tw3);

    q_s0_r = vmulq_f32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vmulq_f32 (q_in_r4567, q2_tw1.val[1]);
    q_s1_r = vmulq_f32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vmulq_f32 (q_in_r89ab, q2_tw2.val[1]);
    q_s2_r = vmulq_f32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vmulq_f32 (q_in_rcdef, q2_tw3.val[1]);
    q_s0_r = vmlsq_f32 (q_s0_r, q_in_i4567, q2_tw1.val[1]);
    q_s0_i = vmlaq_f32 (q_s0_i, q_in_i4567, q2_tw1.val[0]);
    q_s1_r = vmlsq_f32 (q_s1_r, q_in_i89ab, q2_tw2.val[1]);
    q_s1_i = vmlaq_f32 (q_s1_i, q_in_i89ab, q2_tw2.val[0]);
    q_s2_r = vmlsq_f32 (q_s2_r, q_in_icdef, q2_tw3.val[1]);
    q_s2_i = vmlaq_f32 (q_s2_i, q_in_icdef, q2_tw3.val[0]);


    q_s5_r = vsubq_f32 (q_in_r0123, q_s1_r);
    q_s5_i = vsubq_f32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vaddq_f32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vaddq_f32 (q_in_i0123, q_s1_i);

    q_s3_r = vaddq_f32 (q_s0_r, q_s2_r);
    q_s3_i = vaddq_f32 (q_s0_i, q_s2_i);
    q_s4_r = vsubq_f32 (q_s0_r, q_s2_r);
    q_s4_i = vsubq_f32 (q_s0_i, q_s2_i);

    q2_out_89ab.val[0] = vsubq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vsubq_f32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vaddq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vaddq_f32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vaddq_f32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vsubq_f32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vsubq_f32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vaddq_f32 (q_s5_i, q_s4_r);

    vst2q_f32 (p_dst0, q2_out_0123);
    vst2q_f32 (p_dst1, q2_out_4567);
    vst2q_f32 (p_dst2, q2_out_89ab);
    vst2q_f32 (p_dst3, q2_out_cdef);
}

void arm_ne10_fft16_backward_float32_neon (const arm_cfft_instance_f32 *S,
          const ne10_fft_cpx_float32_t *Fin,
          ne10_fft_cpx_float32_t *Fout)
{
    const ne10_fft_cpx_float32_t *tw1; 
    const ne10_fft_cpx_float32_t *tw2; 
    const ne10_fft_cpx_float32_t *tw3;

    // the first stage
    const float32_t *p_src0, *p_src4, *p_src8, *p_src12;
    CMPLX_VEC_F32 q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    float32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    float32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    float32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (const float32_t*) (& (Fin[0]));
    p_src4 = (const float32_t*) (& (Fin[4]));
    p_src8 = (const float32_t*) (& (Fin[8]));
    p_src12 = (const float32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_f32 (p_src0);
    q2_in_4567 = vld2q_f32 (p_src4);
    q2_in_89ab = vld2q_f32 (p_src8);
    q2_in_cdef = vld2q_f32 (p_src12);

    q_t2_r = vsubq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vsubq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vaddq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vaddq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vaddq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vaddq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vsubq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vsubq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vsubq_f32 (q_t3_r, q_t0_r);
    q_out_i26ae = vsubq_f32 (q_t3_i, q_t0_i);
    q_out_r048c = vaddq_f32 (q_t3_r, q_t0_r);
    q_out_i048c = vaddq_f32 (q_t3_i, q_t0_i);
    q_out_r159d = vsubq_f32 (q_t2_r, q_t1_i);
    q_out_i159d = vaddq_f32 (q_t2_i, q_t1_r);
    q_out_r37bf = vaddq_f32 (q_t2_r, q_t1_i);
    q_out_i37bf = vsubq_f32 (q_t2_i, q_t1_r);

    // second stages
    float32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    float32_t *p_tw1, *p_tw2, *p_tw3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    float32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    CMPLX_VEC_F32 q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    float32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    float32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    CMPLX_VEC_F32 q2_tw1, q2_tw2, q2_tw3;
    CMPLX_VEC_F32 q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    float32x4_t q_one_by_nfft;
    tw1 = (const ne10_fft_cpx_float32_t *)S->pTwiddle;
    tw2 = (const ne10_fft_cpx_float32_t *)S->pTwiddle + 4;
    tw3 = (const ne10_fft_cpx_float32_t *)S->pTwiddle + 8;
    p_dst0 = (float32_t*) (&Fout[0]);
    p_dst1 = (float32_t*) (&Fout[4]);
    p_dst2 = (float32_t*) (&Fout[8]);
    p_dst3 = (float32_t*) (&Fout[12]);
    p_tw1 = (float32_t*) tw1;
    p_tw2 = (float32_t*) tw2;
    p_tw3 = (float32_t*) tw3;
    q2_tmp_0 = vzipq_f32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_f32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_f32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_f32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[0]), vget_low_f32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[0]), vget_low_f32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[0]), vget_high_f32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[0]), vget_high_f32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[1]), vget_low_f32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[1]), vget_low_f32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[1]), vget_high_f32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[1]), vget_high_f32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_f32 (p_tw1);
    q2_tw2 = vld2q_f32 (p_tw2);
    q2_tw3 = vld2q_f32 (p_tw3);

    q_s0_r = vmulq_f32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vmulq_f32 (q_in_i4567, q2_tw1.val[0]);
    q_s1_r = vmulq_f32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vmulq_f32 (q_in_i89ab, q2_tw2.val[0]);
    q_s2_r = vmulq_f32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vmulq_f32 (q_in_icdef, q2_tw3.val[0]);
    q_s0_r = vmlaq_f32 (q_s0_r, q_in_i4567, q2_tw1.val[1]);
    q_s0_i = vmlsq_f32 (q_s0_i, q_in_r4567, q2_tw1.val[1]);
    q_s1_r = vmlaq_f32 (q_s1_r, q_in_i89ab, q2_tw2.val[1]);
    q_s1_i = vmlsq_f32 (q_s1_i, q_in_r89ab, q2_tw2.val[1]);
    q_s2_r = vmlaq_f32 (q_s2_r, q_in_icdef, q2_tw3.val[1]);
    q_s2_i = vmlsq_f32 (q_s2_i, q_in_rcdef, q2_tw3.val[1]);

    q_s5_r = vsubq_f32 (q_in_r0123, q_s1_r);
    q_s5_i = vsubq_f32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vaddq_f32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vaddq_f32 (q_in_i0123, q_s1_i);

    q_s3_r = vaddq_f32 (q_s0_r, q_s2_r);
    q_s3_i = vaddq_f32 (q_s0_i, q_s2_i);
    q_s4_r = vsubq_f32 (q_s0_r, q_s2_r);
    q_s4_i = vsubq_f32 (q_s0_i, q_s2_i);

    q_one_by_nfft = vdupq_n_f32 (0.0625f);
    q2_out_89ab.val[0] = vsubq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vsubq_f32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vaddq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vaddq_f32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vsubq_f32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vaddq_f32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vaddq_f32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vsubq_f32 (q_s5_i, q_s4_r);

    q2_out_89ab.val[0] = vmulq_f32 (q2_out_89ab.val[0], q_one_by_nfft);
    q2_out_89ab.val[1] = vmulq_f32 (q2_out_89ab.val[1], q_one_by_nfft);
    q2_out_0123.val[0] = vmulq_f32 (q2_out_0123.val[0], q_one_by_nfft);
    q2_out_0123.val[1] = vmulq_f32 (q2_out_0123.val[1], q_one_by_nfft);
    q2_out_4567.val[0] = vmulq_f32 (q2_out_4567.val[0], q_one_by_nfft);
    q2_out_4567.val[1] = vmulq_f32 (q2_out_4567.val[1], q_one_by_nfft);
    q2_out_cdef.val[0] = vmulq_f32 (q2_out_cdef.val[0], q_one_by_nfft);
    q2_out_cdef.val[1] = vmulq_f32 (q2_out_cdef.val[1], q_one_by_nfft);

    vst2q_f32 (p_dst0, q2_out_0123);
    vst2q_f32 (p_dst1, q2_out_4567);
    vst2q_f32 (p_dst2, q2_out_89ab);
    vst2q_f32 (p_dst3, q2_out_cdef);
}


__STATIC_INLINE void ne10_radix8x4_neon (ne10_fft_cpx_float32_t *out,
                                       const ne10_fft_cpx_float32_t *in,
                                       ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1; // ne10_fft_cpx_float32_t -> float32_t offset
    const float32_t *p_src = (const float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    const ne10_float32_t TW_81 = 0.70710678f;
    const ne10_float32_t TW_81N = -0.70710678f;

    CMPLX_VEC_F32 q2_in0, q2_in1, q2_in2, q2_in3, q2_in4, q2_in5, q2_in6, q2_in7;
    float32x4_t q_sin0_r, q_sin0_i, q_sin1_r, q_sin1_i, q_sin2_r, q_sin2_i, q_sin3_r, q_sin3_i;
    float32x4_t q_sin4_r, q_sin4_i, q_sin5_r, q_sin5_i, q_sin6_r, q_sin6_i, q_sin7_r, q_sin7_i;
    float32x4_t q_s3_r, q_s3_i, q_s5_r, q_s5_i, q_s7_r, q_s7_i;
    float32x4_t q_s8_r, q_s8_i, q_s9_r, q_s9_i, q_s10_r, q_s10_i, q_s11_r, q_s11_i;
    float32x4_t q_s12_r, q_s12_i, q_s13_r, q_s13_i, q_s14_r, q_s14_i, q_s15_r, q_s15_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4_t q_out4_r, q_out4_i, q_out5_r, q_out5_i, q_out6_r, q_out6_i, q_out7_r, q_out7_i;
    CMPLX_VEC_F32 q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3, q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7;
    CMPLX_VEC_F32 q2_out0, q2_out1, q2_out2, q2_out3, q2_out4, q2_out5, q2_out6, q2_out7;
    float32x4_t q_tw_81, q_tw_81n;

    // This loop is unrolled four times, taking 16 NEON quadword registers of input to
    // process four radix-8 butterflies per loop iteration (in contrast to the single
    // radix-8 butterfly per iteration in the pure C counterpart).
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values. Each q2_in* is a pair of two NEON quadword registers,
        // each member of which can hold two complex values (i.e. four float32 values).
        // Though we load four complex values at a time into the pairs here using VLD2Q,
        // the layout of the data ensures that we will not need to perform arithmetic
        // between values in the same vector.
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in4 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in6 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in5 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in7 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[4N/8]>,
        // <X[N/8], X[5N/8]>, <X[2N/8], X[6N/8]>, and <X[3N/8], X[7N/8]> components.
        q_sin0_r = vaddq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin0_i = vaddq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin1_r = vsubq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin1_i = vsubq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin2_r = vaddq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin2_i = vaddq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin3_r = vsubq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin3_i = vsubq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin4_r = vaddq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin4_i = vaddq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin5_r = vsubq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin5_i = vsubq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin6_r = vaddq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin6_i = vaddq_f32 (q2_in6.val[1], q2_in7.val[1]);
        q_sin7_r = vsubq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin7_i = vsubq_f32 (q2_in6.val[1], q2_in7.val[1]);

        // Multiply some of these by hardcoded radix-8 twiddles
        q_tw_81 = vdupq_n_f32 (TW_81);
        q_tw_81n = vdupq_n_f32 (TW_81N);
        q_s5_r = q_sin5_i;
        q_s5_i = vnegq_f32 (q_sin5_r);
        q_s3_r = vaddq_f32 (q_sin3_r, q_sin3_i);
        q_s3_i = vsubq_f32 (q_sin3_i, q_sin3_r);
        q_s7_r = vsubq_f32 (q_sin7_r, q_sin7_i);
        q_s7_i = vaddq_f32 (q_sin7_i, q_sin7_r);
        q_s3_r = vmulq_f32 (q_s3_r, q_tw_81);
        q_s3_i = vmulq_f32 (q_s3_i, q_tw_81);
        q_s7_r = vmulq_f32 (q_s7_r, q_tw_81n);
        q_s7_i = vmulq_f32 (q_s7_i, q_tw_81n);

        // Combine the first set of pairs of these sums
        q_s8_r = vaddq_f32 (q_sin0_r, q_sin4_r);
        q_s8_i = vaddq_f32 (q_sin0_i, q_sin4_i);
        q_s9_r = vaddq_f32 (q_sin1_r, q_s5_r);
        q_s9_i = vaddq_f32 (q_sin1_i, q_s5_i);
        q_s10_r = vsubq_f32 (q_sin0_r, q_sin4_r);
        q_s10_i = vsubq_f32 (q_sin0_i, q_sin4_i);
        q_s11_r = vsubq_f32 (q_sin1_r, q_s5_r);
        q_s11_i = vsubq_f32 (q_sin1_i, q_s5_i);

        // Combine the second set of pairs of these sums
        q_s12_r = vaddq_f32 (q_sin2_r, q_sin6_r);
        q_s12_i = vaddq_f32 (q_sin2_i, q_sin6_i);
        q_s13_r = vaddq_f32 (q_s3_r, q_s7_r);
        q_s13_i = vaddq_f32 (q_s3_i, q_s7_i);
        q_s14_r = vsubq_f32 (q_sin2_r, q_sin6_r);
        q_s14_i = vsubq_f32 (q_sin2_i, q_sin6_i);
        q_s15_r = vsubq_f32 (q_s3_r, q_s7_r);
        q_s15_i = vsubq_f32 (q_s3_i, q_s7_i);

        // Combine these combined components (for the full radix-8 butterfly)
        q_out4_r = vsubq_f32 (q_s8_r, q_s12_r);
        q_out4_i = vsubq_f32 (q_s8_i, q_s12_i);
        q_out5_r = vsubq_f32 (q_s9_r, q_s13_r);
        q_out5_i = vsubq_f32 (q_s9_i, q_s13_i);
        q_out0_r = vaddq_f32 (q_s8_r, q_s12_r);
        q_out0_i = vaddq_f32 (q_s8_i, q_s12_i);
        q_out1_r = vaddq_f32 (q_s9_r, q_s13_r);
        q_out1_i = vaddq_f32 (q_s9_i, q_s13_i);
        q_out2_r = vaddq_f32 (q_s10_r, q_s14_i);
        q_out2_i = vsubq_f32 (q_s10_i, q_s14_r);
        q_out3_r = vaddq_f32 (q_s11_r, q_s15_i);
        q_out3_i = vsubq_f32 (q_s11_i, q_s15_r);
        q_out6_r = vsubq_f32 (q_s10_r, q_s14_i);
        q_out6_i = vaddq_f32 (q_s10_i, q_s14_r);
        q_out7_r = vsubq_f32 (q_s11_r, q_s15_i);
        q_out7_i = vaddq_f32 (q_s11_i, q_s15_r);

        // Use VTRNQ and VCOMBINE to permute the vectors for contiguous stores. This is
        // necessary in this first stage as we wish to store the related calculated values
        // contiguously (as in the rolled pure C loop) even though they are not contiguous
        // in the vectors (instead, values that are from separate loop iterations in the C
        // version of the code are currently contiguous in vectors).
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_tmp4 = vtrnq_f32 (q_out4_r, q_out5_r);
        q2_tmp5 = vtrnq_f32 (q_out4_i, q_out5_i);
        q2_tmp6 = vtrnq_f32 (q_out6_r, q_out7_r);
        q2_tmp7 = vtrnq_f32 (q_out6_i, q_out7_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out2.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out2.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out4.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out4.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out6.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out6.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[0]), vget_low_f32 (q2_tmp6.val[0]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[0]), vget_low_f32 (q2_tmp7.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[1]), vget_low_f32 (q2_tmp6.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[1]), vget_low_f32 (q2_tmp7.val[1]));
        q2_out5.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[0]), vget_high_f32 (q2_tmp6.val[0]));
        q2_out5.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[0]), vget_high_f32 (q2_tmp7.val[0]));
        q2_out7.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[1]), vget_high_f32 (q2_tmp6.val[1]));
        q2_out7.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[1]), vget_high_f32 (q2_tmp7.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out4);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out5);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out6);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out7);
        p_dst += 8;

        // Undo the p_src arithmetic from earlier in the loop, and add eight to skip past
        // the float32 values that have been used within this loop iteration.
        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

__STATIC_INLINE void ne10_radix4x4_without_twiddles_neon (ne10_fft_cpx_float32_t *out,
        const ne10_fft_cpx_float32_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1; // ne10_fft_cpx_float32_t -> float32_t offset
    const float32_t *p_src = (const float32_t *) in;
    float32_t *p_dst = (float32_t *) out;

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s0_r = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vaddq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vaddq_f32 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsubq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsubq_f32 (q2_in1.val[1], q2_in3.val[1]);

        // Combine these sums (for the full radix-4 butterfly)
        q_out2_r = vsubq_f32 (q_s0_r, q_s2_r);
        q_out2_i = vsubq_f32 (q_s0_i, q_s2_i);
        q_out0_r = vaddq_f32 (q_s0_r, q_s2_r);
        q_out0_i = vaddq_f32 (q_s0_i, q_s2_i);
        q_out1_r = vaddq_f32 (q_s1_r, q_s3_i);
        q_out1_i = vsubq_f32 (q_s1_i, q_s3_r);
        q_out3_r = vsubq_f32 (q_s1_r, q_s3_i);
        q_out3_i = vaddq_f32 (q_s1_i, q_s3_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out2.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out2.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
    } // f_count
}

__STATIC_INLINE void ne10_radix4x4_with_twiddles_neon (ne10_fft_cpx_float32_t *out,
        const ne10_fft_cpx_float32_t *in,
        const ne10_fft_cpx_float32_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1; // ne10_fft_cpx_float32_t -> float32_t offsets
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    const float32_t *p_src = (const float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    const float32_t *p_tw  = (const float32_t *) tw;

    CMPLX_VEC_F32 q2_in0, q2_in1, q2_in2, q2_in3;
    CMPLX_VEC_F32 q2_tw0, q2_tw1, q2_tw2;
    CMPLX_VEC_F32 q_s1, q_s2, q_s3;
    CMPLX_VEC_F32 q_s4, q_s5, q_s6, q_s7;
    CMPLX_VEC_F32 q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // Load the input values
        CMPLX_LOAD_F32(q2_in0,p_src);
        p_src += src_step;
        CMPLX_LOAD_F32(q2_in1,p_src);
        p_src += src_step;
        CMPLX_LOAD_F32(q2_in2,p_src);
        p_src += src_step;
        CMPLX_LOAD_F32(q2_in3,p_src);
        p_src += src_step;

        // Load the twiddles
        CMPLX_LOAD_F32(q2_tw0,p_tw);
        p_tw += tw_step;
        CMPLX_LOAD_F32(q2_tw1,p_tw);
        p_tw += tw_step;
        CMPLX_LOAD_F32(q2_tw2,p_tw);

        // Multiply input elements by their associated twiddles
        CMPLX_MUL_F32(q_s1,q2_in1,q2_tw0);
        CMPLX_MUL_F32(q_s2,q2_in2,q2_tw1);
        CMPLX_MUL_F32(q_s3,q2_in3,q2_tw2);

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        CMPLX_ADD_F32(q_s4,q2_in0,q_s2);
        CMPLX_SUB_F32(q_s5,q2_in0,q_s2);

        CMPLX_ADD_F32(q_s6,q_s1,q_s3);
        CMPLX_SUB_F32(q_s7,q_s1,q_s3);

        // Combine these sums (for the full radix-4 butterfly)
        CMPLX_SUB_F32(q2_out2,q_s4,q_s6);
        
        CMPLX_ADD_F32(q2_out0,q_s4,q_s6);
        
        CMPLX_SUB_ROT_F32(q2_out1,q_s5,q_s7);
        
        CMPLX_ADD_ROT_F32(q2_out3,q_s5,q_s7);
        
        // Store the results
        CMPLX_STORE_F32(p_dst,q2_out0);
        p_dst += dst_step;
        CMPLX_STORE_F32(p_dst,q2_out1);
        p_dst += dst_step;
        CMPLX_STORE_F32(p_dst,q2_out2);
        p_dst += dst_step;
        CMPLX_STORE_F32(p_dst,q2_out3);
        p_dst += dst_step;

        // Undo the arithmetic we did to these variables earlier in the loop, and add
        // eight to skip past the float32 values processed within this loop iteration.
        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw  = p_tw - tw_step * 2 + 8;
    } // m_count
}


__STATIC_INLINE void ne10_radix8x4_inverse_neon (ne10_fft_cpx_float32_t *out,
        const ne10_fft_cpx_float32_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;
    const float32_t *p_src = (const float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    const ne10_float32_t TW_81 = 0.70710678f;
    const ne10_float32_t TW_81N = -0.70710678f;

    CMPLX_VEC_F32 q2_in0, q2_in1, q2_in2, q2_in3, q2_in4, q2_in5, q2_in6, q2_in7;
    float32x4_t q_sin0_r, q_sin0_i, q_sin1_r, q_sin1_i, q_sin2_r, q_sin2_i, q_sin3_r, q_sin3_i;
    float32x4_t q_sin4_r, q_sin4_i, q_sin5_r, q_sin5_i, q_sin6_r, q_sin6_i, q_sin7_r, q_sin7_i;
    float32x4_t q_s3_r, q_s3_i, q_s5_r, q_s5_i, q_s7_r, q_s7_i;
    float32x4_t q_s8_r, q_s8_i, q_s9_r, q_s9_i, q_s10_r, q_s10_i, q_s11_r, q_s11_i;
    float32x4_t q_s12_r, q_s12_i, q_s13_r, q_s13_i, q_s14_r, q_s14_i, q_s15_r, q_s15_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4_t q_out4_r, q_out4_i, q_out5_r, q_out5_i, q_out6_r, q_out6_i, q_out7_r, q_out7_i;
    CMPLX_VEC_F32 q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3, q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7;
    CMPLX_VEC_F32 q2_out0, q2_out1, q2_out2, q2_out3, q2_out4, q2_out5, q2_out6, q2_out7;
    float32x4_t q_tw_81, q_tw_81n;

    // This loop is unrolled four times, taking 16 NEON quadword registers of input to
    // process four radix-8 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in4 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in6 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in5 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in7 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[4N/8]>,
        // <X[N/8], X[5N/8]>, <X[2N/8], X[6N/8]>, and <X[3N/8], X[7N/8]> components.
        q_sin0_r = vaddq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin0_i = vaddq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin1_r = vsubq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin1_i = vsubq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin2_r = vaddq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin2_i = vaddq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin3_r = vsubq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin3_i = vsubq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin4_r = vaddq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin4_i = vaddq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin5_r = vsubq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin5_i = vsubq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin6_r = vaddq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin6_i = vaddq_f32 (q2_in6.val[1], q2_in7.val[1]);
        q_sin7_r = vsubq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin7_i = vsubq_f32 (q2_in6.val[1], q2_in7.val[1]);

        // Multiply some of these by hardcoded radix-8 twiddles
        q_tw_81 = vdupq_n_f32 (TW_81);
        q_tw_81n = vdupq_n_f32 (TW_81N);
        q_s5_r = vnegq_f32 (q_sin5_i);
        q_s5_i = q_sin5_r;
        q_s3_r = vsubq_f32 (q_sin3_r, q_sin3_i);
        q_s3_i = vaddq_f32 (q_sin3_i, q_sin3_r);
        q_s7_r = vaddq_f32 (q_sin7_r, q_sin7_i);
        q_s7_i = vsubq_f32 (q_sin7_i, q_sin7_r);
        q_s3_r = vmulq_f32 (q_s3_r, q_tw_81);
        q_s3_i = vmulq_f32 (q_s3_i, q_tw_81);
        q_s7_r = vmulq_f32 (q_s7_r, q_tw_81n);
        q_s7_i = vmulq_f32 (q_s7_i, q_tw_81n);

        // Combine the first set of pairs of these sums
        q_s8_r = vaddq_f32 (q_sin0_r, q_sin4_r);
        q_s8_i = vaddq_f32 (q_sin0_i, q_sin4_i);
        q_s9_r = vaddq_f32 (q_sin1_r, q_s5_r);
        q_s9_i = vaddq_f32 (q_sin1_i, q_s5_i);
        q_s10_r = vsubq_f32 (q_sin0_r, q_sin4_r);
        q_s10_i = vsubq_f32 (q_sin0_i, q_sin4_i);
        q_s11_r = vsubq_f32 (q_sin1_r, q_s5_r);
        q_s11_i = vsubq_f32 (q_sin1_i, q_s5_i);

        // Combine the second set of pairs of these sums
        q_s12_r = vaddq_f32 (q_sin2_r, q_sin6_r);
        q_s12_i = vaddq_f32 (q_sin2_i, q_sin6_i);
        q_s13_r = vaddq_f32 (q_s3_r, q_s7_r);
        q_s13_i = vaddq_f32 (q_s3_i, q_s7_i);
        q_s14_r = vsubq_f32 (q_sin2_r, q_sin6_r);
        q_s14_i = vsubq_f32 (q_sin2_i, q_sin6_i);
        q_s15_r = vsubq_f32 (q_s3_r, q_s7_r);
        q_s15_i = vsubq_f32 (q_s3_i, q_s7_i);

        // Combine these combined components (for the full radix-8 butterfly)
        q_out4_r = vsubq_f32 (q_s8_r, q_s12_r);
        q_out4_i = vsubq_f32 (q_s8_i, q_s12_i);
        q_out5_r = vsubq_f32 (q_s9_r, q_s13_r);
        q_out5_i = vsubq_f32 (q_s9_i, q_s13_i);
        q_out0_r = vaddq_f32 (q_s8_r, q_s12_r);
        q_out0_i = vaddq_f32 (q_s8_i, q_s12_i);
        q_out1_r = vaddq_f32 (q_s9_r, q_s13_r);
        q_out1_i = vaddq_f32 (q_s9_i, q_s13_i);
        q_out2_r = vsubq_f32 (q_s10_r, q_s14_i);
        q_out2_i = vaddq_f32 (q_s10_i, q_s14_r);
        q_out3_r = vsubq_f32 (q_s11_r, q_s15_i);
        q_out3_i = vaddq_f32 (q_s11_i, q_s15_r);
        q_out6_r = vaddq_f32 (q_s10_r, q_s14_i);
        q_out6_i = vsubq_f32 (q_s10_i, q_s14_r);
        q_out7_r = vaddq_f32 (q_s11_r, q_s15_i);
        q_out7_i = vsubq_f32 (q_s11_i, q_s15_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_tmp4 = vtrnq_f32 (q_out4_r, q_out5_r);
        q2_tmp5 = vtrnq_f32 (q_out4_i, q_out5_i);
        q2_tmp6 = vtrnq_f32 (q_out6_r, q_out7_r);
        q2_tmp7 = vtrnq_f32 (q_out6_i, q_out7_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out2.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out2.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out4.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out4.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out6.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out6.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[0]), vget_low_f32 (q2_tmp6.val[0]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[0]), vget_low_f32 (q2_tmp7.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[1]), vget_low_f32 (q2_tmp6.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[1]), vget_low_f32 (q2_tmp7.val[1]));
        q2_out5.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[0]), vget_high_f32 (q2_tmp6.val[0]));
        q2_out5.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[0]), vget_high_f32 (q2_tmp7.val[0]));
        q2_out7.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[1]), vget_high_f32 (q2_tmp6.val[1]));
        q2_out7.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[1]), vget_high_f32 (q2_tmp7.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out4);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out5);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out6);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out7);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

__STATIC_INLINE void ne10_radix4x4_inverse_without_twiddles_neon (ne10_fft_cpx_float32_t *out,
        const ne10_fft_cpx_float32_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;
    const float32_t *p_src = (const float32_t *) in;
    float32_t *p_dst = (float32_t *) out;

    CMPLX_VEC_F32 q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    CMPLX_VEC_F32 q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3;
    CMPLX_VEC_F32 q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s0_r = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vaddq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vaddq_f32 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsubq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsubq_f32 (q2_in1.val[1], q2_in3.val[1]);

        // Combine these sums (for the full radix-4 butterfly)
        q_out2_r = vsubq_f32 (q_s0_r, q_s2_r);
        q_out2_i = vsubq_f32 (q_s0_i, q_s2_i);
        q_out0_r = vaddq_f32 (q_s0_r, q_s2_r);
        q_out0_i = vaddq_f32 (q_s0_i, q_s2_i);
        q_out1_r = vsubq_f32 (q_s1_r, q_s3_i);
        q_out1_i = vaddq_f32 (q_s1_i, q_s3_r);
        q_out3_r = vaddq_f32 (q_s1_r, q_s3_i);
        q_out3_i = vsubq_f32 (q_s1_i, q_s3_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out2.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out2.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
    } // f_count
}

__STATIC_INLINE void ne10_radix4x4_inverse_with_twiddles_neon (ne10_fft_cpx_float32_t *out,
        const ne10_fft_cpx_float32_t *in,
        const ne10_fft_cpx_float32_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    const float32_t *p_src = (const float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    const float32_t *p_tw  = (const float32_t *) tw;

    CMPLX_VEC_F32 q2_in0, q2_in1, q2_in2, q2_in3;
    CMPLX_VEC_F32 q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    CMPLX_VEC_F32 q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Load the twiddles
        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

        // Multiply input elements by their associated twiddles
        q_s1_r = vmulq_f32 (q2_in1.val[0], q2_tw0.val[0]);
        q_s1_i = vmulq_f32 (q2_in1.val[1], q2_tw0.val[0]);
        q_s2_r = vmulq_f32 (q2_in2.val[0], q2_tw1.val[0]);
        q_s2_i = vmulq_f32 (q2_in2.val[1], q2_tw1.val[0]);
        q_s3_r = vmulq_f32 (q2_in3.val[0], q2_tw2.val[0]);
        q_s3_i = vmulq_f32 (q2_in3.val[1], q2_tw2.val[0]);
        q_s1_r = vmlaq_f32 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
        q_s1_i = vmlsq_f32 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
        q_s2_r = vmlaq_f32 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
        q_s2_i = vmlsq_f32 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
        q_s3_r = vmlaq_f32 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
        q_s3_i = vmlsq_f32 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);
        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        // Combine these sums (for the full radix-4 butterfly)
        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);
        q2_out1.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_f32 (q_s5_i, q_s7_r);

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        // Adjust p_src, p_dst, p_tw for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    } // m_count
}

__STATIC_INLINE void ne10_radix4x4_inverse_with_twiddles_last_stage_neon (ne10_fft_cpx_float32_t *out,
        const ne10_fft_cpx_float32_t *in,
        const ne10_fft_cpx_float32_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride,
        ne10_int32_t nfft)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    const float32_t *p_src = (const float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    const float32_t *p_tw  = (const float32_t *) tw;
    ne10_float32_t one_by_nfft = (1.0f / (ne10_float32_t) nfft);

    CMPLX_VEC_F32 q2_in0, q2_in1, q2_in2, q2_in3;
    CMPLX_VEC_F32 q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    CMPLX_VEC_F32 q2_out0, q2_out1, q2_out2, q2_out3;
    float32x4_t q_one_by_nfft = vdupq_n_f32 (one_by_nfft);

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Load the twiddles
        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

        // Multiply input elements by their associated twiddles
        q_s1_r = vmulq_f32 (q2_in1.val[0], q2_tw0.val[0]);
        q_s1_i = vmulq_f32 (q2_in1.val[1], q2_tw0.val[0]);
        q_s2_r = vmulq_f32 (q2_in2.val[0], q2_tw1.val[0]);
        q_s2_i = vmulq_f32 (q2_in2.val[1], q2_tw1.val[0]);
        q_s3_r = vmulq_f32 (q2_in3.val[0], q2_tw2.val[0]);
        q_s3_i = vmulq_f32 (q2_in3.val[1], q2_tw2.val[0]);
        q_s1_r = vmlaq_f32 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
        q_s1_i = vmlsq_f32 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
        q_s2_r = vmlaq_f32 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
        q_s2_i = vmlsq_f32 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
        q_s3_r = vmlaq_f32 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
        q_s3_i = vmlsq_f32 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);
        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        // Combine these sums (for the full radix-4 butterfly)
        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);
        q2_out1.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_f32 (q_s5_i, q_s7_r);

        // Multiply by (1 / nfft)
        q2_out0.val[0] = vmulq_f32 (q2_out0.val[0], q_one_by_nfft);
        q2_out0.val[1] = vmulq_f32 (q2_out0.val[1], q_one_by_nfft);
        q2_out1.val[0] = vmulq_f32 (q2_out1.val[0], q_one_by_nfft);
        q2_out1.val[1] = vmulq_f32 (q2_out1.val[1], q_one_by_nfft);
        q2_out2.val[0] = vmulq_f32 (q2_out2.val[0], q_one_by_nfft);
        q2_out2.val[1] = vmulq_f32 (q2_out2.val[1], q_one_by_nfft);
        q2_out3.val[0] = vmulq_f32 (q2_out3.val[0], q_one_by_nfft);
        q2_out3.val[1] = vmulq_f32 (q2_out3.val[1], q_one_by_nfft);

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        // Adjust p_src, p_dst, p_tw for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    } // m_count
}

/*
 * This function calculates the FFT for power-of-two input sizes using an ordered, mixed
 * radix-4/8 DIT algorithm. It is very similar to its C counterpart in NE10_fft_float32.c,
 * "ne10_mixed_radix_butterfly_float32_c".
 */

// Use arm_ to be able to use Ne10 with this library
void arm_ne10_mixed_radix_fft_forward_float32_neon (
    const arm_cfft_instance_f32 *S,
    const ne10_fft_cpx_float32_t *input,
          ne10_fft_cpx_float32_t *out,
          ne10_fft_cpx_float32_t *buffer)

{
    ne10_int32_t stage_count = S->factors[0];
    ne10_int32_t fstride = S->factors[1];
    ne10_int32_t mstride = S->factors[3];
    ne10_int32_t first_radix = S->factors[2];
    ne10_int32_t step, f_count;
    const ne10_fft_cpx_float32_t *src = input;
    ne10_fft_cpx_float32_t *in;
    ne10_fft_cpx_float32_t *dst = out;
    ne10_fft_cpx_float32_t *out_final = out;
    const ne10_fft_cpx_float32_t *tw; 
    ne10_fft_cpx_float32_t *tmp;

    const ne10_fft_cpx_float32_t *twiddles = (const ne10_fft_cpx_float32_t *)S->pTwiddle;

    //printf("CMSIS : stage_count %d\n",stage_count);
    //printf("CMSIS : fstride %d\n",fstride);
    //printf("CMSIS : mstride %d\n",mstride);
    //printf("CMSIS : first_radix %d\n",first_radix);

    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // For our factoring, this means nfft is of form 2^{odd}
    {
        ne10_radix8x4_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride << 1; // For C2C, 1/4 of input size (fstride is nfft/8)
        stage_count--;
        fstride /= 4;
    }
    else if (first_radix == 4) // For our factoring, this means nfft is of form 2^{even}
    {
        ne10_radix4x4_without_twiddles_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride; // For C2C, 1/4 of input size (fstride is nfft/4)
        stage_count--;
        fstride /= 4;
    }

    // The next stage should read the output of the first stage as input
    in = out;
    out = buffer;
    
    // Middle stages (after the first, excluding the last)
    for (; stage_count > 1 ; stage_count--)
    {
        src = in;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            dst = &out[f_count * (mstride * 4)];
            tw = twiddles; // Reset the twiddle pointer for the next section
            ne10_radix4x4_with_twiddles_neon (dst, src, tw, step, mstride, mstride);
            src += mstride;
        } // f_count

        // Update variables for the next stages
        twiddles += mstride * 3;
        mstride *= 4;
        fstride /= 4;

        // Swap the input and output buffers for the next stage
        tmp = in;
        in = out;
        out = tmp;
    } // stage_count

    // The last stage
    if (stage_count)
    {
        src = in;

        // Always write to the final output buffer (if necessary, we can calculate this
        // in-place as the final stage reads and writes at the same offsets)
        dst = out_final;

        for (f_count = 0; f_count < fstride; f_count++)
        {
            tw = twiddles; // Reset the twiddle pointer for the next section
            ne10_radix4x4_with_twiddles_neon (dst, src, tw, step, step, mstride);
            src += mstride;
            dst += mstride;
        } // f_count
    } // last stage

}


/*
 * This function calculates the inverse FFT, and is very similar in structure to its
 * complement "ne10_mixed_radix_fft_forward_float32_neon".
 */

void arm_ne10_mixed_radix_fft_backward_float32_neon (
    const arm_cfft_instance_f32 *S,
    const ne10_fft_cpx_float32_t *input,
          ne10_fft_cpx_float32_t *out,
          ne10_fft_cpx_float32_t *buffer)
{
    ne10_int32_t stage_count = S->factors[0];
    ne10_int32_t fstride = S->factors[1];
    ne10_int32_t mstride = S->factors[3];
    ne10_int32_t first_radix = S->factors[2];
    ne10_int32_t nfft = fstride * first_radix;
    ne10_int32_t step, f_count;
    const ne10_fft_cpx_float32_t *src = input;
    ne10_fft_cpx_float32_t *in;
    ne10_fft_cpx_float32_t *dst = out;
    ne10_fft_cpx_float32_t *out_final = out;
    const ne10_fft_cpx_float32_t *tw;

    ne10_fft_cpx_float32_t *tmp;

    const ne10_fft_cpx_float32_t *twiddles = (const ne10_fft_cpx_float32_t *)S->pTwiddle;


    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // nfft is of form 2^{odd}
    {
        ne10_radix8x4_inverse_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride << 1;
        stage_count--;
        fstride /= 4;
    }
    else if (first_radix == 4) // nfft is of form 2^{even}
    {
        ne10_radix4x4_inverse_without_twiddles_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride;
        stage_count--;
        fstride /= 4;
    }

    // The next stage should read the output of the first stage as input
    in = out;
    out = buffer;
    
    // Middle stages (after the first, excluding the last)
    for (; stage_count > 1; stage_count--)
    {
        src = in;
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * (mstride * 4)];
            tw = twiddles;
            ne10_radix4x4_inverse_with_twiddles_neon (dst, src, tw, step, mstride, mstride);
            src += mstride;
        } // f_count

        // Update variables for the next stages
        twiddles += mstride * 3;
        mstride *= 4;
        fstride /= 4;

        // Swap the input and output buffers for the next stage
        tmp = in;
        in = out;
        out = tmp;
    } // stage_count

    // The last stage
    if (stage_count)
    {
        src = in;

        // Always write to the final output buffer (if necessary, we can calculate this
        // in-place as the final stage reads and writes at the same offsets)
        dst = out_final;

        for (f_count = 0; f_count < fstride; f_count++)
        {
            tw = twiddles;
            ne10_radix4x4_inverse_with_twiddles_last_stage_neon (dst, src, tw, step, step, mstride, nfft);
            src += mstride;
            dst += mstride;
        } // f_count
    } // last stage
}
