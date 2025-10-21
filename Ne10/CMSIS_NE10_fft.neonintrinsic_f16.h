/*
 *  Copyright 2014-16 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_fft.neonintrinsic.h
 */

#ifndef CMSIS_NE10_FFT_NEONINTRINSIC_F16_H
#define CMSIS_NE10_FFT_NEONINTRINSIC_F16_H

#include "CMSIS_NE10_fft.h"
#include <arm_neon.h>

#if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)

#define NE10_CPX_ADD_NEON_F16(Z,A,B) do {           \
    Z.val[0] = A.val[0] + B.val[0];    \
    Z.val[1] = A.val[1] + B.val[1];    \
} while (0);

#define NE10_CPX_SUB_NEON_F16(Z,A,B) do {           \
    Z.val[0] = A.val[0] - B.val[0];    \
    Z.val[1] = A.val[1] - B.val[1];    \
} while (0);

#define NE10_CPX_MUL_NEON_F16(Z,A,B) do {           \
    float16x4_t ARBR = vmul_f16( A.val[0], B.val[0] ); \
    float16x4_t ARBI = vmul_f16( A.val[0], B.val[1] ); \
    Z.val[0] = vfms_f16(ARBR, A.val[1], B.val[1]); \
    Z.val[1] = vfma_f16(ARBI, A.val[1], B.val[0]); \
} while (0);

#define NE10_CPX_MUL_INV_NEON_F16(Z,A,B) do {           \
    float16x4_t ARBR = vmul_f16( A.val[0], B.val[0] ); \
    float16x4_t AIBI = vmul_f16( A.val[1], B.val[1] ); \
    float16x4_t ARBI = vmul_f16( A.val[0], B.val[1] ); \
    float16x4_t AIBR = vmul_f16( A.val[1], B.val[0] ); \
    Z.val[0] = vadd_f16(ARBR,AIBI);                \
    Z.val[1] = vsub_f16(AIBR,ARBI);                \
} while (0);

#define NE10_BUTTERFLY_NEON_F16(O1,O2,I1,I2) do {   \
    NE10_CPX_ADD_NEON(O1,I1,I2);                    \
    NE10_CPX_SUB_NEON(O2,I1,I2);                    \
} while(0);

#define NE10_DECLARE_2(TYPE,NAME)   TYPE NAME ## 0; \
                                    TYPE NAME ## 1;

#define NE10_DECLARE_3(TYPE,NAME)   NE10_DECLARE_2(TYPE,NAME);  \
                                    TYPE NAME ## 2;

#define NE10_DECLARE_4(TYPE,NAME)   NE10_DECLARE_3(TYPE,NAME);  \
                                    TYPE NAME ## 3;

#define NE10_DECLARE_8(TYPE,NAME)   NE10_DECLARE_4(TYPE,NAME);  \
                                    TYPE NAME ## 4; \
                                    TYPE NAME ## 5; \
                                    TYPE NAME ## 6; \
                                    TYPE NAME ## 7;

#define NE10_REVERSE_FLOAT16X4(VECTOR4F) do {                                     \
    VECTOR4F = vreinterpret_f16_s16(vrev32_s16(vreinterpret_s16_f16(VECTOR4F)));\
    VECTOR4F = vreinterpret_f16_f32(vrev64_f32(vreinterpret_f32_f16(VECTOR4F)));\
} while (0);

#define NE10_REVERSE_OUT_FLOAT16X4(VECTOR4F_OUT,VECTOR4F) do {                             \
    float16x4_t Q_TMP = vreinterpret_f16_s16(vrev32_s16(vreinterpret_s16_f16(VECTOR4F)));\
    VECTOR4F_OUT = vreinterpret_f16_f32(vrev64_f32(vreinterpret_f32_f16(Q_TMP)));        \
} while (0);

#define NE10_RADIX4X4C_TRANSPOSE_NEON(Q2_OUT,Q2_IN) do {                                                                                 \
    float16x8x2_t q2_tmp0,q2_tmp1;                                                                                                       \
    float32x4x2_t q2_tmp2, q2_tmp3;                                                                                                      \
    q2_tmp0 = vtrnq_f16 ((vcombine_f16(Q2_IN ## 0 .val[0], Q2_IN ## 0 .val[1])), (vcombine_f16(Q2_IN ## 1 .val[0], Q2_IN ## 1 .val[1]))); \
    q2_tmp1 = vtrnq_f16 ((vcombine_f16(Q2_IN ## 2 .val[0], Q2_IN ## 2 .val[1])), (vcombine_f16(Q2_IN ## 3 .val[0], Q2_IN ## 3 .val[1]))); \
    q2_tmp2 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[0]), vreinterpretq_f32_f16(q2_tmp1.val[0]));                                  \
    q2_tmp3 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[1]), vreinterpretq_f32_f16(q2_tmp1.val[1]));                                  \
                                                                                                                                         \
    Q2_OUT ## 0 .val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp2.val[0])));                                                       \
    Q2_OUT ## 0 .val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp2.val[0])));                                                       \
    Q2_OUT ## 1 .val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp3.val[0])));                                                       \
    Q2_OUT ## 1 .val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp3.val[0])));                                                       \
    Q2_OUT ## 2 .val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp2.val[1])));                                                       \
    Q2_OUT ## 2 .val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp2.val[1])));                                                       \
    Q2_OUT ## 3 .val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp3.val[1])));                                                       \
    Q2_OUT ## 3 .val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp3.val[1])));                                                       \
} while(0);




#define VDUPQ_N_F16(VAR) { VAR, VAR,VAR,VAR }

#define CONST_TW_81   0.70710678f16
#define CONST_TW_81N -0.70710678f16

static const float16x4_t Q_TW_81    = VDUPQ_N_F16(CONST_TW_81 );
static const float16x4_t Q_TW_81N   = VDUPQ_N_F16(CONST_TW_81N);

#define DIV_TW81   1.4142136f16
#define DIV_TW81N -1.4142136f16

static const float16x4_t DIV_TW81_NEON  = VDUPQ_N_F16(DIV_TW81);
static const float16x4_t DIV_TW81N_NEON = VDUPQ_N_F16(DIV_TW81N);

#define NE10_RADIX8x4_R2C_NEON_KERNEL_S1(Q_OUT,Q_IN) do {   \
        Q_OUT ## 0 = vadd_f16 (Q_IN ## 0, Q_IN ## 4);      \
        Q_OUT ## 1 = vsub_f16 (Q_IN ## 0, Q_IN ## 4);      \
        Q_OUT ## 2 = vadd_f16 (Q_IN ## 1, Q_IN ## 5);      \
        Q_OUT ## 3 = vsub_f16 (Q_IN ## 1, Q_IN ## 5);      \
        Q_OUT ## 4 = vadd_f16 (Q_IN ## 2, Q_IN ## 6);      \
        Q_OUT ## 5 = vsub_f16 (Q_IN ## 2, Q_IN ## 6);      \
        Q_OUT ## 6 = vadd_f16 (Q_IN ## 3, Q_IN ## 7);      \
        Q_OUT ## 7 = vsub_f16 (Q_IN ## 3, Q_IN ## 7);      \
        Q_OUT ## 3 = vmul_f16 (Q_OUT ## 3, Q_TW_81 );      \
        Q_OUT ## 7 = vmul_f16 (Q_OUT ## 7, Q_TW_81N);      \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_KERNEL_S2(Q_OUT,Q_IN) do {   \
        NE10_DECLARE_4(float16x4_t,Q_S);                    \
        Q_S0 =  vadd_f16 (Q_IN ## 0, Q_IN ## 4);           \
        Q_S1 =  vadd_f16 (Q_IN ## 2, Q_IN ## 6);           \
        Q_S2 =  vsub_f16 (Q_IN ## 7, Q_IN ## 3);           \
        Q_S3 =  vadd_f16 (Q_IN ## 3, Q_IN ## 7);           \
        Q_OUT ## 0 = vadd_f16 (      Q_S0,      Q_S1 );    \
        Q_OUT ## 1 = vadd_f16 ( Q_IN ## 1,      Q_S3 );    \
        Q_OUT ## 2 = vsub_f16 (      Q_S2, Q_IN ## 5 );    \
        Q_OUT ## 3 = vsub_f16 ( Q_IN ## 0, Q_IN ## 4 );    \
        Q_OUT ## 4 = vsub_f16 ( Q_IN ## 6, Q_IN ## 2 );    \
        Q_OUT ## 5 = vsub_f16 ( Q_IN ## 1,      Q_S3 );    \
        Q_OUT ## 6 = vadd_f16 ( Q_IN ## 5,      Q_S2 );    \
        Q_OUT ## 7 = vsub_f16 (      Q_S0,      Q_S1 );    \
} while(0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL_S1(Q_OUT,Q_IN) do {   \
        NE10_DECLARE_8(float16x4_t,Q_S_IN);                 \
        Q_S_IN0 = vadd_f16(Q_IN ## 0, Q_IN ## 7);          \
        Q_S_IN1 = vsub_f16(Q_IN ## 0, Q_IN ## 7);          \
        Q_S_IN2 = vadd_f16(Q_IN ## 1, Q_IN ## 5);          \
        Q_S_IN3 = vsub_f16(Q_IN ## 1, Q_IN ## 5);          \
        Q_S_IN4 = vadd_f16(Q_IN ## 6, Q_IN ## 2);          \
        Q_S_IN5 = vsub_f16(Q_IN ## 6, Q_IN ## 2);          \
        Q_S_IN6 = vadd_f16(Q_IN ## 3, Q_IN ## 3);          \
        Q_S_IN7 = vadd_f16(Q_IN ## 4, Q_IN ## 4);          \
        Q_OUT ## 0 = vadd_f16(Q_S_IN0, Q_S_IN6);           \
        Q_OUT ## 1 = vadd_f16(Q_S_IN2, Q_S_IN2);           \
        Q_OUT ## 2 = vsub_f16(Q_S_IN1, Q_S_IN7);           \
        Q_OUT ## 3 = vsub_f16(Q_S_IN3, Q_S_IN4);           \
        Q_OUT ## 4 = vsub_f16(Q_S_IN0, Q_S_IN6);           \
        Q_OUT ## 5 = vadd_f16(Q_S_IN5, Q_S_IN5);           \
        Q_OUT ## 6 = vadd_f16(Q_S_IN1, Q_S_IN7);           \
        Q_OUT ## 7 = vadd_f16(Q_S_IN4, Q_S_IN3);           \
} while (0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL_S2(Q_OUT,Q_IN) do {   \
        Q_IN ## 3 = vmul_f16(Q_IN ## 3,DIV_TW81_NEON);     \
        Q_IN ## 7 = vmul_f16(Q_IN ## 7,DIV_TW81N_NEON);    \
        Q_OUT ## 0 = vadd_f16(Q_IN ## 0, Q_IN ## 1);       \
        Q_OUT ## 4 = vsub_f16(Q_IN ## 0, Q_IN ## 1);       \
        Q_OUT ## 1 = vadd_f16(Q_IN ## 2, Q_IN ## 3);       \
        Q_OUT ## 5 = vsub_f16(Q_IN ## 2, Q_IN ## 3);       \
        Q_OUT ## 2 = vadd_f16(Q_IN ## 4, Q_IN ## 5);       \
        Q_OUT ## 6 = vsub_f16(Q_IN ## 4, Q_IN ## 5);       \
        Q_OUT ## 3 = vadd_f16(Q_IN ## 6, Q_IN ## 7);       \
        Q_OUT ## 7 = vsub_f16(Q_IN ## 6, Q_IN ## 7);       \
} while(0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL_SCALE(Q_OUT)  do {    \
    Q_OUT ## 0 = vmul_f16( Q_OUT ## 0, EIGH_NEON);         \
    Q_OUT ## 1 = vmul_f16( Q_OUT ## 1, EIGH_NEON);         \
    Q_OUT ## 2 = vmul_f16( Q_OUT ## 2, EIGH_NEON);         \
    Q_OUT ## 3 = vmul_f16( Q_OUT ## 3, EIGH_NEON);         \
    Q_OUT ## 4 = vmul_f16( Q_OUT ## 4, EIGH_NEON);         \
    Q_OUT ## 5 = vmul_f16( Q_OUT ## 5, EIGH_NEON);         \
    Q_OUT ## 6 = vmul_f16( Q_OUT ## 6, EIGH_NEON);         \
    Q_OUT ## 7 = vmul_f16( Q_OUT ## 7, EIGH_NEON);         \
} while(0);

#define NE10_RADIX4x4_C2R_NEON_KERNEL_SCALE(Q_OUT)  do {    \
    Q_OUT ## 0 = vmul_f16( Q_OUT ## 0, QUAD_NEON);         \
    Q_OUT ## 1 = vmul_f16( Q_OUT ## 1, QUAD_NEON);         \
    Q_OUT ## 2 = vmul_f16( Q_OUT ## 2, QUAD_NEON);         \
    Q_OUT ## 3 = vmul_f16( Q_OUT ## 3, QUAD_NEON);         \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_SCALE(Q2_OUT)  do {            \
    Q2_OUT ## 0 .val[0] = vmul_f16( Q2_OUT ## 0 .val[0], QUAD_NEON);   \
    Q2_OUT ## 1 .val[0] = vmul_f16( Q2_OUT ## 1 .val[0], QUAD_NEON);   \
    Q2_OUT ## 2 .val[0] = vmul_f16( Q2_OUT ## 2 .val[0], QUAD_NEON);   \
    Q2_OUT ## 3 .val[0] = vmul_f16( Q2_OUT ## 3 .val[0], QUAD_NEON);   \
    Q2_OUT ## 0 .val[1] = vmul_f16( Q2_OUT ## 0 .val[1], QUAD_NEON);   \
    Q2_OUT ## 1 .val[1] = vmul_f16( Q2_OUT ## 1 .val[1], QUAD_NEON);   \
    Q2_OUT ## 2 .val[1] = vmul_f16( Q2_OUT ## 2 .val[1], QUAD_NEON);   \
    Q2_OUT ## 3 .val[1] = vmul_f16( Q2_OUT ## 3 .val[1], QUAD_NEON);   \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_KERNEL(Q_OUT,Q_IN) do {  \
    NE10_DECLARE_8(float16x4_t,Q_S_IN);                 \
    NE10_RADIX8x4_R2C_NEON_KERNEL_S1(Q_S_IN,Q_IN);      \
    NE10_RADIX8x4_R2C_NEON_KERNEL_S2(Q_OUT,Q_S_IN);     \
} while(0);

#define NE10_RADIX4x4_R2C_NEON_KERNEL(Q_OUT,Q_IN) do {  \
    NE10_DECLARE_2(float16x4_t,Q_S_IN);                 \
    Q_S_IN0 = vadd_f16 (Q_IN ## 0, Q_IN ## 2);         \
    Q_S_IN1 = vadd_f16 (Q_IN ## 1, Q_IN ## 3);         \
    Q_OUT ## 0 = vadd_f16 (Q_S_IN0, Q_S_IN1);          \
    Q_OUT ## 1 = vsub_f16 (Q_IN##0, Q_IN##2);          \
    Q_OUT ## 2 = vsub_f16 (Q_IN##3, Q_IN##1);          \
    Q_OUT ## 3 = vsub_f16 (Q_S_IN0, Q_S_IN1);          \
} while(0);

#define NE10_RADIX4x4_C2R_NEON_KERNEL(Q_OUT,Q_IN) do {  \
    NE10_DECLARE_4(float16x4_t,Q_S_IN);                 \
    Q_S_IN0 = vadd_f16 (Q_IN##0, Q_IN##3);             \
    Q_S_IN1 = vsub_f16 (Q_IN##0, Q_IN##3);             \
    Q_S_IN2 = vadd_f16 (Q_IN##1, Q_IN##1);             \
    Q_S_IN3 = vadd_f16 (Q_IN##2, Q_IN##2);             \
    Q_OUT ## 0 = vadd_f16 (Q_S_IN0, Q_S_IN2);          \
    Q_OUT ## 1 = vsub_f16 (Q_S_IN1, Q_S_IN3);          \
    Q_OUT ## 2 = vsub_f16 (Q_S_IN0, Q_S_IN2);          \
    Q_OUT ## 3 = vadd_f16 (Q_S_IN1, Q_S_IN3);          \
} while(0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL(Q_OUT,Q_IN) do {          \
    NE10_DECLARE_8(float16x4_t,Q_S_IN_C2R_KERNEL);              \
    NE10_RADIX8x4_C2R_NEON_KERNEL_S1(Q_S_IN_C2R_KERNEL,Q_IN);   \
    NE10_RADIX8x4_C2R_NEON_KERNEL_S2(Q_OUT,Q_S_IN_C2R_KERNEL);  \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_LOAD(PTR_IN,Q_IN,IN_STEP) do { \
    Q_IN ## 0 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 1 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 2 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 3 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 4 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 5 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 6 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 7 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
} while(0);

#define NE10_RADIX4x4_R2C_NEON_LOAD(PTR_IN,Q_IN,IN_STEP) do {\
    Q_IN ## 0 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
    Q_IN ## 1 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
    Q_IN ## 2 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
    Q_IN ## 3 = vld1_f16( (ne10_float16_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_STORE(PTR_OUT,Q_OUT,OUT_STEP) do {           \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 0 * OUT_STEP ), Q_OUT ## 0);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 1 * OUT_STEP ), Q_OUT ## 1);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 2 * OUT_STEP ), Q_OUT ## 2);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 3 * OUT_STEP ), Q_OUT ## 3);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 4 * OUT_STEP ), Q_OUT ## 4);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 5 * OUT_STEP ), Q_OUT ## 5);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 6 * OUT_STEP ), Q_OUT ## 6);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 7 * OUT_STEP ), Q_OUT ## 7);  \
} while(0);

#define NE10_RADIX4x4_R2C_NEON_STORE(PTR_OUT,Q_OUT,OUT_STEP) do {           \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 0 * OUT_STEP ), Q_OUT ## 0);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 1 * OUT_STEP ), Q_OUT ## 1);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 2 * OUT_STEP ), Q_OUT ## 2);  \
     vst1_f16( (ne10_float16_t*) ( PTR_OUT + 3 * OUT_STEP ), Q_OUT ## 3);  \
} while(0);

#define NE10_RADIX4x4_R2C_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW) do {  \
    Q2_OUT ## 0 = Q2_IN ## 0;                                   \
    NE10_CPX_MUL_NEON_F16(Q2_OUT ## 1,Q2_IN ## 1,Q2_TW ## 0);   \
    NE10_CPX_MUL_NEON_F16(Q2_OUT ## 2,Q2_IN ## 2,Q2_TW ## 1);   \
    NE10_CPX_MUL_NEON_F16(Q2_OUT ## 3,Q2_IN ## 3,Q2_TW ## 2);   \
} while(0);

#define NE10_RADIX4x4_C2R_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW) do {      \
    Q2_OUT ## 0 = Q2_IN ## 0;                                       \
    NE10_CPX_MUL_INV_NEON_F16(Q2_OUT ## 1,Q2_IN ## 1,Q2_TW ## 0);   \
    NE10_CPX_MUL_INV_NEON_F16(Q2_OUT ## 2,Q2_IN ## 2,Q2_TW ## 1);   \
    NE10_CPX_MUL_INV_NEON_F16(Q2_OUT ## 3,Q2_IN ## 3,Q2_TW ## 2);   \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S1(Q2_OUT,Q2_IN) do {  \
    NE10_CPX_ADD_NEON_F16(Q2_OUT ## 0,Q2_IN ## 0,Q2_IN ## 2);   \
    NE10_CPX_SUB_NEON_F16(Q2_OUT ## 1,Q2_IN ## 0,Q2_IN ## 2);   \
    NE10_CPX_ADD_NEON_F16(Q2_OUT ## 2,Q2_IN ## 1,Q2_IN ## 3);   \
    NE10_CPX_SUB_NEON_F16(Q2_OUT ## 3,Q2_IN ## 1,Q2_IN ## 3);   \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S2(Q2_OUT,Q2_IN) do {  \
    Q2_OUT ## 0 .val[0] = vadd_f16(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 0 .val[1] = vadd_f16(Q2_IN ## 0 .val[1] , Q2_IN ## 2 .val[1]); \
    Q2_OUT ## 2 .val[0] = vsub_f16(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 2 .val[1] = vsub_f16(Q2_IN ## 2 .val[1] , Q2_IN ## 0 .val[1]); \
    Q2_OUT ## 1 .val[0] = vadd_f16(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 1 .val[1] = vsub_f16(Q2_IN ## 1 .val[1] , Q2_IN ## 3 .val[0]); \
    Q2_OUT ## 3 .val[0] = vsub_f16(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 3 .val[1] = vadd_f16(Q2_IN ## 3 .val[0] , Q2_IN ## 1 .val[1]); \
    Q2_OUT ## 3 .val[1] = vneg_f16(Q2_OUT ## 3 .val[1]); \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL_LAST(Q_OUT,Q_IN) do {  \
    float16x4_t Q_TMP;  \
    Q_IN ## 1 = vmul_f16(Q_IN ## 1, Q_TW_81);  \
    Q_IN ## 3 = vmul_f16(Q_IN ## 3, Q_TW_81);  \
    Q_TMP = vsub_f16(Q_IN ## 1, Q_IN ## 3);    \
    Q_IN ## 3 = vadd_f16(Q_IN ## 1, Q_IN ## 3);    \
    Q_IN ## 1 = Q_TMP;                      \
    Q_OUT ## 0 = vadd_f16(Q_IN ## 0, Q_IN ## 1);   \
    Q_OUT ## 1 = vadd_f16(Q_IN ## 2, Q_IN ## 3);   \
    Q_OUT ## 2 = vsub_f16(Q_IN ## 0, Q_IN ## 1);   \
    Q_OUT ## 3 = vsub_f16(Q_IN ## 2, Q_IN ## 3);   \
    Q_OUT ## 1 = vneg_f16(Q_OUT ## 1);         \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_LAST(Q_OUT,Q_IN) do {  \
    float16x4_t Q_TMP;  \
    Q_IN ## 1 = vneg_f16(Q_IN ## 1 );  \
    Q_OUT ## 0 = vadd_f16(Q_IN ## 0, Q_IN ## 2);   \
    Q_OUT ## 1 = vsub_f16(Q_IN ## 0, Q_IN ## 2);   \
    Q_OUT ## 2 = vadd_f16(Q_IN ## 1, Q_IN ## 3);   \
    Q_OUT ## 3 = vsub_f16(Q_IN ## 1, Q_IN ## 3);   \
    Q_TMP = vadd_f16(Q_OUT ## 1, Q_OUT ## 3);  \
    Q_OUT ## 3 = vsub_f16(Q_OUT ## 3, Q_OUT ## 1);  \
    Q_OUT ## 1 = Q_TMP; \
    Q_OUT ## 1 = vmul_f16( Q_OUT ## 1, DIV_TW81_NEON); \
    Q_OUT ## 3 = vmul_f16( Q_OUT ## 3, DIV_TW81_NEON); \
    Q_OUT ## 0 = vadd_f16( Q_OUT ## 0, Q_OUT ## 0 );   \
    Q_OUT ## 2 = vadd_f16( Q_OUT ## 2, Q_OUT ## 2 );   \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S1(Q2_OUT,Q2_IN) do {  \
    Q2_IN ## 3 .val[1] = vneg_f16(Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 0 .val[0] = vadd_f16(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 0 .val[1] = vsub_f16(Q2_IN ## 0 .val[1] , Q2_IN ## 2 .val[1]); \
    Q2_OUT ## 2 .val[0] = vsub_f16(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 2 .val[1] = vadd_f16(Q2_IN ## 2 .val[1] , Q2_IN ## 0 .val[1]); \
    Q2_OUT ## 1 .val[0] = vadd_f16(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[0]); \
    Q2_OUT ## 1 .val[1] = vadd_f16(Q2_IN ## 1 .val[1] , Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 3 .val[0] = vsub_f16(Q2_IN ## 3 .val[1] , Q2_IN ## 1 .val[1]); \
    Q2_OUT ## 3 .val[1] = vsub_f16(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[0]); \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S2(Q2_OUT,Q2_IN) do {  \
    NE10_CPX_ADD_NEON_F16(Q2_OUT ## 0,Q2_IN ## 0,Q2_IN ## 1);   \
    NE10_CPX_SUB_NEON_F16(Q2_OUT ## 2,Q2_IN ## 0,Q2_IN ## 1);   \
    NE10_CPX_ADD_NEON_F16(Q2_OUT ## 1,Q2_IN ## 2,Q2_IN ## 3);   \
    NE10_CPX_SUB_NEON_F16(Q2_OUT ## 3,Q2_IN ## 2,Q2_IN ## 3);   \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL(Q2_OUT,Q2_IN,Q2_TW) do {   \
    NE10_RADIX4x4_R2C_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW);              \
    NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S1(Q2_IN,Q2_OUT);              \
    NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S2(Q2_OUT,Q2_IN);              \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL(Q2_OUT,Q2_IN,Q2_TW) do {   \
    NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S1(Q2_OUT,Q2_IN);              \
    NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S2(Q2_IN,Q2_OUT);              \
    NE10_RADIX4x4_C2R_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW);              \
} while(0);


#define NE10_PRINT_Qx8_VECTOR(Q_VECTOR) ;
#define NE10_PRINT_Qx4_VECTOR(Q_VECTOR) ;
#define NE10_PRINT_Q2x4_VECTOR(Q2_VECTOR) ;
#endif // defined(ARM_MATH_NEON_FLOAT16)
#endif // header
