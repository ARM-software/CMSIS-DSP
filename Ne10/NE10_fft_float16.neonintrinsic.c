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
 * CMSIS-DSP f16 implementation based upon the Ne10 f32 one
 */

#include "arm_compiler_specific.h"
#include <arm_neon.h>

#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_fft.h"

#include "dsp/transform_functions_f16.h"

#if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)


__STATIC_INLINE void ne10_radix8x4_neon (ne10_fft_cpx_float16_t *out,
                                       const ne10_fft_cpx_float16_t *in,
                                       ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1; // ne10_fft_cpx_float16_t -> float16_t offset
    const float16_t *p_src = (const float16_t *) in;
    float16_t *p_dst = (float16_t *) out;
    const ne10_float16_t TW_81 = 0.70710678f16;
    const ne10_float16_t TW_81N = -0.70710678f16;

    float16x4x2_t q2_in0, q2_in1, q2_in2, q2_in3, q2_in4, q2_in5, q2_in6, q2_in7;
    float16x4_t q_sin0_r, q_sin0_i, q_sin1_r, q_sin1_i, q_sin2_r, q_sin2_i, q_sin3_r, q_sin3_i;
    float16x4_t q_sin4_r, q_sin4_i, q_sin5_r, q_sin5_i, q_sin6_r, q_sin6_i, q_sin7_r, q_sin7_i;
    float16x4_t q_s3_r, q_s3_i, q_s5_r, q_s5_i, q_s7_r, q_s7_i;
    float16x4_t q_s8_r, q_s8_i, q_s9_r, q_s9_i, q_s10_r, q_s10_i, q_s11_r, q_s11_i;
    float16x4_t q_s12_r, q_s12_i, q_s13_r, q_s13_i, q_s14_r, q_s14_i, q_s15_r, q_s15_i;
    float16x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float16x4_t q_out4_r, q_out4_i, q_out5_r, q_out5_i, q_out6_r, q_out6_i, q_out7_r, q_out7_i;
    float16x8x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3; 
    float32x4x2_t q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7; 
    float16x4x2_t q2_out0, q2_out1, q2_out2, q2_out3, q2_out4, q2_out5, q2_out6, q2_out7;
    float16x4_t q_tw_81, q_tw_81n;

    // This loop is unrolled four times, taking 16 NEON quadword registers of input to
    // process four radix-8 butterflies per loop iteration (in contrast to the single
    // radix-8 butterfly per iteration in the pure C counterpart).
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values. Each q2_in* is a pair of two NEON quadword registers,
        // each member of which can hold two complex values (i.e. four float16 values).
        // Though we load four complex values at a time into the pairs here using VLD2Q,
        // the layout of the data ensures that we will not need to perform arithmetic
        // between values in the same vector.
        q2_in0 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in2 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in4 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in6 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in1 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in3 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in5 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in7 = vld2_f16 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[4N/8]>,
        // <X[N/8], X[5N/8]>, <X[2N/8], X[6N/8]>, and <X[3N/8], X[7N/8]> components.
        q_sin0_r = vadd_f16 (q2_in0.val[0], q2_in1.val[0]);
        q_sin0_i = vadd_f16 (q2_in0.val[1], q2_in1.val[1]);
        q_sin1_r = vsub_f16 (q2_in0.val[0], q2_in1.val[0]);
        q_sin1_i = vsub_f16 (q2_in0.val[1], q2_in1.val[1]);
        q_sin2_r = vadd_f16 (q2_in2.val[0], q2_in3.val[0]);
        q_sin2_i = vadd_f16 (q2_in2.val[1], q2_in3.val[1]);
        q_sin3_r = vsub_f16 (q2_in2.val[0], q2_in3.val[0]);
        q_sin3_i = vsub_f16 (q2_in2.val[1], q2_in3.val[1]);
        q_sin4_r = vadd_f16 (q2_in4.val[0], q2_in5.val[0]);
        q_sin4_i = vadd_f16 (q2_in4.val[1], q2_in5.val[1]);
        q_sin5_r = vsub_f16 (q2_in4.val[0], q2_in5.val[0]);
        q_sin5_i = vsub_f16 (q2_in4.val[1], q2_in5.val[1]);
        q_sin6_r = vadd_f16 (q2_in6.val[0], q2_in7.val[0]);
        q_sin6_i = vadd_f16 (q2_in6.val[1], q2_in7.val[1]);
        q_sin7_r = vsub_f16 (q2_in6.val[0], q2_in7.val[0]);
        q_sin7_i = vsub_f16 (q2_in6.val[1], q2_in7.val[1]);

        // Multiply some of these by hardcoded radix-8 twiddles
        q_tw_81 = vdup_n_f16 (TW_81);
        q_tw_81n = vdup_n_f16 (TW_81N);
        q_s5_r = q_sin5_i;
        q_s5_i = vneg_f16 (q_sin5_r);
        q_s3_r = vadd_f16 (q_sin3_r, q_sin3_i);
        q_s3_i = vsub_f16 (q_sin3_i, q_sin3_r);
        q_s7_r = vsub_f16 (q_sin7_r, q_sin7_i);
        q_s7_i = vadd_f16 (q_sin7_i, q_sin7_r);
        q_s3_r = vmul_f16 (q_s3_r, q_tw_81);
        q_s3_i = vmul_f16 (q_s3_i, q_tw_81);
        q_s7_r = vmul_f16 (q_s7_r, q_tw_81n);
        q_s7_i = vmul_f16 (q_s7_i, q_tw_81n);

        // Combine the first set of pairs of these sums
        q_s8_r = vadd_f16 (q_sin0_r, q_sin4_r);
        q_s8_i = vadd_f16 (q_sin0_i, q_sin4_i);
        q_s9_r = vadd_f16 (q_sin1_r, q_s5_r);
        q_s9_i = vadd_f16 (q_sin1_i, q_s5_i);
        q_s10_r = vsub_f16 (q_sin0_r, q_sin4_r);
        q_s10_i = vsub_f16 (q_sin0_i, q_sin4_i);
        q_s11_r = vsub_f16 (q_sin1_r, q_s5_r);
        q_s11_i = vsub_f16 (q_sin1_i, q_s5_i);

        // Combine the second set of pairs of these sums
        q_s12_r = vadd_f16 (q_sin2_r, q_sin6_r);
        q_s12_i = vadd_f16 (q_sin2_i, q_sin6_i);
        q_s13_r = vadd_f16 (q_s3_r, q_s7_r);
        q_s13_i = vadd_f16 (q_s3_i, q_s7_i);
        q_s14_r = vsub_f16 (q_sin2_r, q_sin6_r);
        q_s14_i = vsub_f16 (q_sin2_i, q_sin6_i);
        q_s15_r = vsub_f16 (q_s3_r, q_s7_r);
        q_s15_i = vsub_f16 (q_s3_i, q_s7_i);

        // Combine these combined components (for the full radix-8 butterfly)
        q_out4_r = vsub_f16 (q_s8_r, q_s12_r);
        q_out4_i = vsub_f16 (q_s8_i, q_s12_i);
        q_out5_r = vsub_f16 (q_s9_r, q_s13_r);
        q_out5_i = vsub_f16 (q_s9_i, q_s13_i);
        q_out0_r = vadd_f16 (q_s8_r, q_s12_r);
        q_out0_i = vadd_f16 (q_s8_i, q_s12_i);
        q_out1_r = vadd_f16 (q_s9_r, q_s13_r);
        q_out1_i = vadd_f16 (q_s9_i, q_s13_i);
        q_out2_r = vadd_f16 (q_s10_r, q_s14_i);
        q_out2_i = vsub_f16 (q_s10_i, q_s14_r);
        q_out3_r = vadd_f16 (q_s11_r, q_s15_i);
        q_out3_i = vsub_f16 (q_s11_i, q_s15_r);
        q_out6_r = vsub_f16 (q_s10_r, q_s14_i);
        q_out6_i = vadd_f16 (q_s10_i, q_s14_r);
        q_out7_r = vsub_f16 (q_s11_r, q_s15_i);
        q_out7_i = vadd_f16 (q_s11_i, q_s15_r);

        // Use VTRNQ and VCOMBINE to permute the vectors for contiguous stores. This is
        // necessary in this first stage as we wish to store the related calculated values
        // contiguously (as in the rolled pure C loop) even though they are not contiguous
        // in the vectors (instead, values that are from separate loop iterations in the C
        // version of the code are currently contiguous in vectors).
        q2_tmp0 = vtrnq_f16 (vcombine_f16(q_out0_r, q_out0_i), vcombine_f16(q_out1_r, q_out1_i)); 
        q2_tmp1 = vtrnq_f16 (vcombine_f16(q_out2_r, q_out2_i), vcombine_f16(q_out3_r, q_out3_i)); 
        q2_tmp2 = vtrnq_f16 (vcombine_f16(q_out4_r, q_out4_i), vcombine_f16(q_out5_r, q_out5_i)); 
        q2_tmp3 = vtrnq_f16 (vcombine_f16(q_out6_r, q_out6_i), vcombine_f16(q_out7_r, q_out7_i)); 
        q2_tmp4 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[0]), vreinterpretq_f32_f16(q2_tmp1.val[0])); 
        q2_tmp5 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[1]), vreinterpretq_f32_f16(q2_tmp1.val[1])); 
        q2_tmp6 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp2.val[0]), vreinterpretq_f32_f16(q2_tmp3.val[0])); 
        q2_tmp7 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp2.val[1]), vreinterpretq_f32_f16(q2_tmp3.val[1])); 
        q2_out0.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp4.val[0])); 
        q2_out0.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp4.val[0])); 
        q2_out1.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp6.val[0])); 
        q2_out1.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp6.val[0])); 
        q2_out2.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp5.val[0])); 
        q2_out2.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp5.val[0])); 
        q2_out3.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp7.val[0])); 
        q2_out3.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp7.val[0])); 
        q2_out4.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp4.val[1])); 
        q2_out4.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp4.val[1])); 
        q2_out5.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp6.val[1])); 
        q2_out5.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp6.val[1])); 
        q2_out6.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp5.val[1])); 
        q2_out6.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp5.val[1])); 
        q2_out7.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp7.val[1])); 
        q2_out7.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp7.val[1])); 

        // Store the results
        vst2_f16 (p_dst, q2_out0);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out1);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out2);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out3);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out4);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out5);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out6);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out7);
        p_dst += 8;

        // Undo the p_src arithmetic from earlier in the loop, and add eight to skip past
        // the float16 values that have been used within this loop iteration.
        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

                    
__STATIC_INLINE void ne10_radix4x4_without_twiddles_neon (ne10_fft_cpx_float16_t *out,
        const ne10_fft_cpx_float16_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1; // ne10_fft_cpx_float16_t -> float16_t offset
    const float16_t *p_src = (const float16_t *) in;
    float16_t *p_dst = (float16_t *) out;

    float16x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float16x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float16x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float16x8x2_t q2_tmp0, q2_tmp1;
    float32x4x2_t q2_tmp2, q2_tmp3;
    float16x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in1 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in2 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in3 = vld2_f16 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s0_r = vadd_f16 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vadd_f16 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsub_f16 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsub_f16 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vadd_f16 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vadd_f16 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsub_f16 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsub_f16 (q2_in1.val[1], q2_in3.val[1]);

        // Combine these sums (for the full radix-4 butterfly)
        q_out2_r = vsub_f16 (q_s0_r, q_s2_r);
        q_out2_i = vsub_f16 (q_s0_i, q_s2_i);
        q_out0_r = vadd_f16 (q_s0_r, q_s2_r);
        q_out0_i = vadd_f16 (q_s0_i, q_s2_i);
        q_out1_r = vadd_f16 (q_s1_r, q_s3_i);
        q_out1_i = vsub_f16 (q_s1_i, q_s3_r);
        q_out3_r = vsub_f16 (q_s1_r, q_s3_i);
        q_out3_i = vadd_f16 (q_s1_i, q_s3_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f16 ((vcombine_f16(q_out0_r, q_out0_i)), (vcombine_f16(q_out1_r, q_out1_i))); 
        q2_tmp1 = vtrnq_f16 ((vcombine_f16(q_out2_r, q_out2_i)), (vcombine_f16(q_out3_r, q_out3_i))); 
        q2_tmp2 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[0]), vreinterpretq_f32_f16(q2_tmp1.val[0])); 
        q2_tmp3 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[1]), vreinterpretq_f32_f16(q2_tmp1.val[1])); 

        q2_out0.val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp2.val[0]))); 
        q2_out0.val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp2.val[0]))); 
        q2_out1.val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp3.val[0]))); 
        q2_out1.val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp3.val[0]))); 
        q2_out2.val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp2.val[1]))); 
        q2_out2.val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp2.val[1]))); 
        q2_out3.val[0] = (vget_low_f16  (vreinterpretq_f16_f32(q2_tmp3.val[1]))); 
        q2_out3.val[1] = (vget_high_f16 (vreinterpretq_f16_f32(q2_tmp3.val[1]))); 

    
        // Store the results
        vst2_f16 (p_dst, q2_out0);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out1);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out2);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out3);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
    } // f_count
}


__STATIC_INLINE void ne10_radix4x4_with_twiddles_neon (ne10_fft_cpx_float16_t *out,
        const ne10_fft_cpx_float16_t *in,
        const ne10_fft_cpx_float16_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1; // ne10_fft_cpx_float16_t -> float16_t offsets
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    const float16_t *p_src = (const float16_t *) in;
    float16_t *p_dst = (float16_t *) out;
    const float16_t *p_tw  = (const float16_t *) tw;

    float16x8x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float16x8x2_t q2_tw0, q2_tw1, q2_tw2;
    float16x8_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float16x8_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float16x8x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    float16x4x2_t q2_in0_s, q2_in1_s, q2_in2_s, q2_in3_s;
    float16x4x2_t q2_tw0_s, q2_tw1_s, q2_tw2_s;
    float16x4_t q_s1_r_s, q_s1_i_s, q_s2_r_s, q_s2_i_s, q_s3_r_s, q_s3_i_s;
    float16x4_t q_s4_r_s, q_s4_i_s, q_s5_r_s, q_s5_i_s, q_s6_r_s, q_s6_i_s, q_s7_r_s, q_s7_i_s;
    float16x4x2_t q2_out0_s, q2_out1_s, q2_out2_s, q2_out3_s;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    if (mstride<8)
    {
        for (m_count = 0; m_count < mstride; m_count += 4)
        {
            // Load the input values
            q2_in0_s = vld2_f16 (p_src);
            p_src += src_step;
            q2_in1_s = vld2_f16 (p_src);
            p_src += src_step;
            q2_in2_s = vld2_f16 (p_src);
            p_src += src_step;
            q2_in3_s = vld2_f16 (p_src);
            p_src += src_step;
    
            // Load the twiddles
            q2_tw0_s = vld2_f16 (p_tw);
            p_tw += tw_step;
            q2_tw1_s = vld2_f16 (p_tw);
            p_tw += tw_step;
            q2_tw2_s = vld2_f16 (p_tw);
    
            // Multiply input elements by their associated twiddles
            q_s1_r_s = vmul_f16 (q2_in1_s.val[0], q2_tw0_s.val[0]);
            q_s1_i_s = vmul_f16 (q2_in1_s.val[1], q2_tw0_s.val[0]);
            q_s2_r_s = vmul_f16 (q2_in2_s.val[0], q2_tw1_s.val[0]);
            q_s2_i_s = vmul_f16 (q2_in2_s.val[1], q2_tw1_s.val[0]);
            q_s3_r_s = vmul_f16 (q2_in3_s.val[0], q2_tw2_s.val[0]);
            q_s3_i_s = vmul_f16 (q2_in3_s.val[1], q2_tw2_s.val[0]);
            q_s1_r_s = vfms_f16 (q_s1_r_s, q2_in1_s.val[1], q2_tw0_s.val[1]);
            q_s1_i_s = vfma_f16 (q_s1_i_s, q2_in1_s.val[0], q2_tw0_s.val[1]);
            q_s2_r_s = vfms_f16 (q_s2_r_s, q2_in2_s.val[1], q2_tw1_s.val[1]);
            q_s2_i_s = vfma_f16 (q_s2_i_s, q2_in2_s.val[0], q2_tw1_s.val[1]);
            q_s3_r_s = vfms_f16 (q_s3_r_s, q2_in3_s.val[1], q2_tw2_s.val[1]);
            q_s3_i_s = vfma_f16 (q_s3_i_s, q2_in3_s.val[0], q2_tw2_s.val[1]);
    
            // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
            // <X[N/4], X[3N/4]> components.
            q_s4_r_s = vadd_f16 (q2_in0_s.val[0], q_s2_r_s);
            q_s4_i_s = vadd_f16 (q2_in0_s.val[1], q_s2_i_s);
            q_s5_r_s = vsub_f16 (q2_in0_s.val[0], q_s2_r_s);
            q_s5_i_s = vsub_f16 (q2_in0_s.val[1], q_s2_i_s);
            q_s6_r_s = vadd_f16 (q_s1_r_s, q_s3_r_s);
            q_s6_i_s = vadd_f16 (q_s1_i_s, q_s3_i_s);
            q_s7_r_s = vsub_f16 (q_s1_r_s, q_s3_r_s);
            q_s7_i_s = vsub_f16 (q_s1_i_s, q_s3_i_s);
    
            // Combine these sums (for the full radix-4 butterfly)
            q2_out2_s.val[0] = vsub_f16 (q_s4_r_s, q_s6_r_s);
            q2_out2_s.val[1] = vsub_f16 (q_s4_i_s, q_s6_i_s);
            q2_out0_s.val[0] = vadd_f16 (q_s4_r_s, q_s6_r_s);
            q2_out0_s.val[1] = vadd_f16 (q_s4_i_s, q_s6_i_s);
            q2_out1_s.val[0] = vadd_f16 (q_s5_r_s, q_s7_i_s);
            q2_out1_s.val[1] = vsub_f16 (q_s5_i_s, q_s7_r_s);
            q2_out3_s.val[0] = vsub_f16 (q_s5_r_s, q_s7_i_s);
            q2_out3_s.val[1] = vadd_f16 (q_s5_i_s, q_s7_r_s);
    
            // Store the results
            vst2_f16 (p_dst, q2_out0_s);
            p_dst += dst_step;
            vst2_f16 (p_dst, q2_out1_s);
            p_dst += dst_step;
            vst2_f16 (p_dst, q2_out2_s);
            p_dst += dst_step;
            vst2_f16 (p_dst, q2_out3_s);
            p_dst += dst_step;
    
            // Undo the arithmetic we did to these variables earlier in the loop, and add
            // eight to skip past the float16 values processed within this loop iteration.
            p_src = p_src - src_step * 4 + 8;
            p_dst = p_dst - dst_step * 4 + 8;
            p_tw  = p_tw - tw_step * 2 + 8;
        } // m_count
    }
    else
    {
        for (m_count = 0; m_count < mstride; m_count += 8)
        {
            // Load the input values
            q2_in0 = vld2q_f16 (p_src);
            p_src += src_step;
            q2_in1 = vld2q_f16 (p_src);
            p_src += src_step;
            q2_in2 = vld2q_f16 (p_src);
            p_src += src_step;
            q2_in3 = vld2q_f16 (p_src);
            p_src += src_step;
    
            // Load the twiddles
            q2_tw0 = vld2q_f16 (p_tw);
            p_tw += tw_step;
            q2_tw1 = vld2q_f16 (p_tw);
            p_tw += tw_step;
            q2_tw2 = vld2q_f16 (p_tw);
    
            // Multiply input elements by their associated twiddles
            q_s1_r = vmulq_f16 (q2_in1.val[0], q2_tw0.val[0]);
            q_s1_i = vmulq_f16 (q2_in1.val[1], q2_tw0.val[0]);
            q_s2_r = vmulq_f16 (q2_in2.val[0], q2_tw1.val[0]);
            q_s2_i = vmulq_f16 (q2_in2.val[1], q2_tw1.val[0]);
            q_s3_r = vmulq_f16 (q2_in3.val[0], q2_tw2.val[0]);
            q_s3_i = vmulq_f16 (q2_in3.val[1], q2_tw2.val[0]);
            q_s1_r = vfmsq_f16 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
            q_s1_i = vfmaq_f16 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
            q_s2_r = vfmsq_f16 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
            q_s2_i = vfmaq_f16 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
            q_s3_r = vfmsq_f16 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
            q_s3_i = vfmaq_f16 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);
    
            // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
            // <X[N/4], X[3N/4]> components.
            q_s4_r = vaddq_f16 (q2_in0.val[0], q_s2_r);
            q_s4_i = vaddq_f16 (q2_in0.val[1], q_s2_i);
            q_s5_r = vsubq_f16 (q2_in0.val[0], q_s2_r);
            q_s5_i = vsubq_f16 (q2_in0.val[1], q_s2_i);
            q_s6_r = vaddq_f16 (q_s1_r, q_s3_r);
            q_s6_i = vaddq_f16 (q_s1_i, q_s3_i);
            q_s7_r = vsubq_f16 (q_s1_r, q_s3_r);
            q_s7_i = vsubq_f16 (q_s1_i, q_s3_i);
    
            // Combine these sums (for the full radix-4 butterfly)
            q2_out2.val[0] = vsubq_f16 (q_s4_r, q_s6_r);
            q2_out2.val[1] = vsubq_f16 (q_s4_i, q_s6_i);
            q2_out0.val[0] = vaddq_f16 (q_s4_r, q_s6_r);
            q2_out0.val[1] = vaddq_f16 (q_s4_i, q_s6_i);
            q2_out1.val[0] = vaddq_f16 (q_s5_r, q_s7_i);
            q2_out1.val[1] = vsubq_f16 (q_s5_i, q_s7_r);
            q2_out3.val[0] = vsubq_f16 (q_s5_r, q_s7_i);
            q2_out3.val[1] = vaddq_f16 (q_s5_i, q_s7_r);
    
            // Store the results
            vst2q_f16 (p_dst, q2_out0);
            p_dst += dst_step;
            vst2q_f16 (p_dst, q2_out1);
            p_dst += dst_step;
            vst2q_f16 (p_dst, q2_out2);
            p_dst += dst_step;
            vst2q_f16 (p_dst, q2_out3);
            p_dst += dst_step;
    
            // Undo the arithmetic we did to these variables earlier in the loop, and add
            // eight to skip past the float16 values processed within this loop iteration.
            p_src = p_src - src_step * 4 + 16;
            p_dst = p_dst - dst_step * 4 + 16;
            p_tw  = p_tw - tw_step * 2 + 16;
        } // m_count
    }
}


__STATIC_INLINE void ne10_radix8x4_inverse_neon (ne10_fft_cpx_float16_t *out,
        const ne10_fft_cpx_float16_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;
    const float16_t *p_src = (const float16_t *) in;
    float16_t *p_dst = (float16_t *) out;
    const ne10_float16_t TW_81 = 0.70710678f16;
    const ne10_float16_t TW_81N = -0.70710678f16;

    float16x4x2_t q2_in0, q2_in1, q2_in2, q2_in3, q2_in4, q2_in5, q2_in6, q2_in7;
    float16x4_t q_sin0_r, q_sin0_i, q_sin1_r, q_sin1_i, q_sin2_r, q_sin2_i, q_sin3_r, q_sin3_i;
    float16x4_t q_sin4_r, q_sin4_i, q_sin5_r, q_sin5_i, q_sin6_r, q_sin6_i, q_sin7_r, q_sin7_i;
    float16x4_t q_s3_r, q_s3_i, q_s5_r, q_s5_i, q_s7_r, q_s7_i;
    float16x4_t q_s8_r, q_s8_i, q_s9_r, q_s9_i, q_s10_r, q_s10_i, q_s11_r, q_s11_i;
    float16x4_t q_s12_r, q_s12_i, q_s13_r, q_s13_i, q_s14_r, q_s14_i, q_s15_r, q_s15_i;
    float16x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float16x4_t q_out4_r, q_out4_i, q_out5_r, q_out5_i, q_out6_r, q_out6_i, q_out7_r, q_out7_i;
    float16x8x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3; 
    float32x4x2_t q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7; 
    float16x4x2_t q2_out0, q2_out1, q2_out2, q2_out3, q2_out4, q2_out5, q2_out6, q2_out7;
    float16x4_t q_tw_81, q_tw_81n;

    // This loop is unrolled four times, taking 16 NEON quadword registers of input to
    // process four radix-8 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in2 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in4 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in6 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in1 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in3 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in5 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in7 = vld2_f16 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[4N/8]>,
        // <X[N/8], X[5N/8]>, <X[2N/8], X[6N/8]>, and <X[3N/8], X[7N/8]> components.
        q_sin0_r = vadd_f16 (q2_in0.val[0], q2_in1.val[0]);
        q_sin0_i = vadd_f16 (q2_in0.val[1], q2_in1.val[1]);
        q_sin1_r = vsub_f16 (q2_in0.val[0], q2_in1.val[0]);
        q_sin1_i = vsub_f16 (q2_in0.val[1], q2_in1.val[1]);
        q_sin2_r = vadd_f16 (q2_in2.val[0], q2_in3.val[0]);
        q_sin2_i = vadd_f16 (q2_in2.val[1], q2_in3.val[1]);
        q_sin3_r = vsub_f16 (q2_in2.val[0], q2_in3.val[0]);
        q_sin3_i = vsub_f16 (q2_in2.val[1], q2_in3.val[1]);
        q_sin4_r = vadd_f16 (q2_in4.val[0], q2_in5.val[0]);
        q_sin4_i = vadd_f16 (q2_in4.val[1], q2_in5.val[1]);
        q_sin5_r = vsub_f16 (q2_in4.val[0], q2_in5.val[0]);
        q_sin5_i = vsub_f16 (q2_in4.val[1], q2_in5.val[1]);
        q_sin6_r = vadd_f16 (q2_in6.val[0], q2_in7.val[0]);
        q_sin6_i = vadd_f16 (q2_in6.val[1], q2_in7.val[1]);
        q_sin7_r = vsub_f16 (q2_in6.val[0], q2_in7.val[0]);
        q_sin7_i = vsub_f16 (q2_in6.val[1], q2_in7.val[1]);

        // Multiply some of these by hardcoded radix-8 twiddles
        q_tw_81 = vdup_n_f16 (TW_81);
        q_tw_81n = vdup_n_f16 (TW_81N);
        q_s5_r = vneg_f16 (q_sin5_i);
        q_s5_i = q_sin5_r;
        q_s3_r = vsub_f16 (q_sin3_r, q_sin3_i);
        q_s3_i = vadd_f16 (q_sin3_i, q_sin3_r);
        q_s7_r = vadd_f16 (q_sin7_r, q_sin7_i);
        q_s7_i = vsub_f16 (q_sin7_i, q_sin7_r);
        q_s3_r = vmul_f16 (q_s3_r, q_tw_81);
        q_s3_i = vmul_f16 (q_s3_i, q_tw_81);
        q_s7_r = vmul_f16 (q_s7_r, q_tw_81n);
        q_s7_i = vmul_f16 (q_s7_i, q_tw_81n);

        // Combine the first set of pairs of these sums
        q_s8_r = vadd_f16 (q_sin0_r, q_sin4_r);
        q_s8_i = vadd_f16 (q_sin0_i, q_sin4_i);
        q_s9_r = vadd_f16 (q_sin1_r, q_s5_r);
        q_s9_i = vadd_f16 (q_sin1_i, q_s5_i);
        q_s10_r = vsub_f16 (q_sin0_r, q_sin4_r);
        q_s10_i = vsub_f16 (q_sin0_i, q_sin4_i);
        q_s11_r = vsub_f16 (q_sin1_r, q_s5_r);
        q_s11_i = vsub_f16 (q_sin1_i, q_s5_i);

        // Combine the second set of pairs of these sums
        q_s12_r = vadd_f16 (q_sin2_r, q_sin6_r);
        q_s12_i = vadd_f16 (q_sin2_i, q_sin6_i);
        q_s13_r = vadd_f16 (q_s3_r, q_s7_r);
        q_s13_i = vadd_f16 (q_s3_i, q_s7_i);
        q_s14_r = vsub_f16 (q_sin2_r, q_sin6_r);
        q_s14_i = vsub_f16 (q_sin2_i, q_sin6_i);
        q_s15_r = vsub_f16 (q_s3_r, q_s7_r);
        q_s15_i = vsub_f16 (q_s3_i, q_s7_i);

        // Combine these combined components (for the full radix-8 butterfly)
        q_out4_r = vsub_f16 (q_s8_r, q_s12_r);
        q_out4_i = vsub_f16 (q_s8_i, q_s12_i);
        q_out5_r = vsub_f16 (q_s9_r, q_s13_r);
        q_out5_i = vsub_f16 (q_s9_i, q_s13_i);
        q_out0_r = vadd_f16 (q_s8_r, q_s12_r);
        q_out0_i = vadd_f16 (q_s8_i, q_s12_i);
        q_out1_r = vadd_f16 (q_s9_r, q_s13_r);
        q_out1_i = vadd_f16 (q_s9_i, q_s13_i);
        q_out2_r = vsub_f16 (q_s10_r, q_s14_i);
        q_out2_i = vadd_f16 (q_s10_i, q_s14_r);
        q_out3_r = vsub_f16 (q_s11_r, q_s15_i);
        q_out3_i = vadd_f16 (q_s11_i, q_s15_r);
        q_out6_r = vadd_f16 (q_s10_r, q_s14_i);
        q_out6_i = vsub_f16 (q_s10_i, q_s14_r);
        q_out7_r = vadd_f16 (q_s11_r, q_s15_i);
        q_out7_i = vsub_f16 (q_s11_i, q_s15_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f16 (vcombine_f16(q_out0_r, q_out0_i), vcombine_f16(q_out1_r, q_out1_i)); 
        q2_tmp1 = vtrnq_f16 (vcombine_f16(q_out2_r, q_out2_i), vcombine_f16(q_out3_r, q_out3_i)); 
        q2_tmp2 = vtrnq_f16 (vcombine_f16(q_out4_r, q_out4_i), vcombine_f16(q_out5_r, q_out5_i)); 
        q2_tmp3 = vtrnq_f16 (vcombine_f16(q_out6_r, q_out6_i), vcombine_f16(q_out7_r, q_out7_i)); 
        q2_tmp4 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[0]), vreinterpretq_f32_f16(q2_tmp1.val[0])); 
        q2_tmp5 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[1]), vreinterpretq_f32_f16(q2_tmp1.val[1])); 
        q2_tmp6 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp2.val[0]), vreinterpretq_f32_f16(q2_tmp3.val[0])); 
        q2_tmp7 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp2.val[1]), vreinterpretq_f32_f16(q2_tmp3.val[1])); 
        q2_out0.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp4.val[0])); 
        q2_out0.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp4.val[0])); 
        q2_out1.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp6.val[0])); 
        q2_out1.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp6.val[0])); 
        q2_out2.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp5.val[0])); 
        q2_out2.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp5.val[0])); 
        q2_out3.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp7.val[0])); 
        q2_out3.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp7.val[0])); 
        q2_out4.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp4.val[1])); 
        q2_out4.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp4.val[1])); 
        q2_out5.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp6.val[1])); 
        q2_out5.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp6.val[1])); 
        q2_out6.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp5.val[1])); 
        q2_out6.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp5.val[1])); 
        q2_out7.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp7.val[1])); 
        q2_out7.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp7.val[1])); 

        // Store the results
        vst2_f16 (p_dst, q2_out0);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out1);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out2);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out3);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out4);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out5);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out6);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out7);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

__STATIC_INLINE void ne10_radix4x4_inverse_without_twiddles_neon (ne10_fft_cpx_float16_t *out,
        const ne10_fft_cpx_float16_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;
    const float16_t *p_src = (const float16_t *) in;
    float16_t *p_dst = (float16_t *) out;

    float16x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float16x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float16x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float16x8x2_t q2_tmp0, q2_tmp1;
    float32x4x2_t q2_tmp2, q2_tmp3;
    float16x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in1 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in2 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in3 = vld2_f16 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s0_r = vadd_f16 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vadd_f16 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsub_f16 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsub_f16 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vadd_f16 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vadd_f16 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsub_f16 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsub_f16 (q2_in1.val[1], q2_in3.val[1]);

        // Combine these sums (for the full radix-4 butterfly)
        q_out2_r = vsub_f16 (q_s0_r, q_s2_r);
        q_out2_i = vsub_f16 (q_s0_i, q_s2_i);
        q_out0_r = vadd_f16 (q_s0_r, q_s2_r);
        q_out0_i = vadd_f16 (q_s0_i, q_s2_i);
        q_out1_r = vsub_f16 (q_s1_r, q_s3_i);
        q_out1_i = vadd_f16 (q_s1_i, q_s3_r);
        q_out3_r = vadd_f16 (q_s1_r, q_s3_i);
        q_out3_i = vsub_f16 (q_s1_i, q_s3_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f16 (vcombine_f16(q_out0_r, q_out0_i), vcombine_f16(q_out1_r, q_out1_i)); 
        q2_tmp1 = vtrnq_f16 (vcombine_f16(q_out2_r, q_out2_i), vcombine_f16(q_out3_r, q_out3_i)); 
        q2_tmp2 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[0]), vreinterpretq_f32_f16(q2_tmp1.val[0])); 
        q2_tmp3 = vtrnq_f32 (vreinterpretq_f32_f16(q2_tmp0.val[1]), vreinterpretq_f32_f16(q2_tmp1.val[1])); 
        q2_out0.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp2.val[0])); 
        q2_out0.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp2.val[0])); 
        q2_out1.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp3.val[0])); 
        q2_out1.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp3.val[0])); 
        q2_out2.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp2.val[1])); 
        q2_out2.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp2.val[1])); 
        q2_out3.val[0] = vget_low_f16 (vreinterpretq_f16_f32(q2_tmp3.val[1])); 
        q2_out3.val[1] = vget_high_f16 (vreinterpretq_f16_f32(q2_tmp3.val[1])); 

        // Store the results
        vst2_f16 (p_dst, q2_out0);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out1);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out2);
        p_dst += 8;
        vst2_f16 (p_dst, q2_out3);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
    } // f_count
}

__STATIC_INLINE void ne10_radix4x4_inverse_with_twiddles_neon (ne10_fft_cpx_float16_t *out,
        const ne10_fft_cpx_float16_t *in,
        const ne10_fft_cpx_float16_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    const float16_t *p_src = (const float16_t *) in;
    float16_t *p_dst = (float16_t *) out;
    const float16_t *p_tw  = (const float16_t *) tw;

    float16x8x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float16x8x2_t q2_tw0, q2_tw1, q2_tw2;
    float16x8_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float16x8_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float16x8x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    float16x4x2_t q2_in0_s, q2_in1_s, q2_in2_s, q2_in3_s;
    float16x4x2_t q2_tw0_s, q2_tw1_s, q2_tw2_s;
    float16x4_t q_s1_r_s, q_s1_i_s, q_s2_r_s, q_s2_i_s, q_s3_r_s, q_s3_i_s;
    float16x4_t q_s4_r_s, q_s4_i_s, q_s5_r_s, q_s5_i_s, q_s6_r_s, q_s6_i_s, q_s7_r_s, q_s7_i_s;
    float16x4x2_t q2_out0_s, q2_out1_s, q2_out2_s, q2_out3_s;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    if (mstride < 8)
    {
        for (m_count = 0; m_count < mstride; m_count += 4)
        {
            // Load the input values
            q2_in0_s = vld2_f16 (p_src);
            p_src += src_step;
            q2_in1_s = vld2_f16 (p_src);
            p_src += src_step;
            q2_in2_s = vld2_f16 (p_src);
            p_src += src_step;
            q2_in3_s = vld2_f16 (p_src);
            p_src += src_step;
    
            // Load the twiddles
            q2_tw0_s = vld2_f16 (p_tw);
            p_tw += tw_step;
            q2_tw1_s = vld2_f16 (p_tw);
            p_tw += tw_step;
            q2_tw2_s = vld2_f16 (p_tw);
    
            // Multiply input elements by their associated twiddles
            q_s1_r_s = vmul_f16 (q2_in1_s.val[0], q2_tw0_s.val[0]);
            q_s1_i_s = vmul_f16 (q2_in1_s.val[1], q2_tw0_s.val[0]);
            q_s2_r_s = vmul_f16 (q2_in2_s.val[0], q2_tw1_s.val[0]);
            q_s2_i_s = vmul_f16 (q2_in2_s.val[1], q2_tw1_s.val[0]);
            q_s3_r_s = vmul_f16 (q2_in3_s.val[0], q2_tw2_s.val[0]);
            q_s3_i_s = vmul_f16 (q2_in3_s.val[1], q2_tw2_s.val[0]);
            q_s1_r_s = vfma_f16 (q_s1_r_s, q2_in1_s.val[1], q2_tw0_s.val[1]);
            q_s1_i_s = vfms_f16 (q_s1_i_s, q2_in1_s.val[0], q2_tw0_s.val[1]);
            q_s2_r_s = vfma_f16 (q_s2_r_s, q2_in2_s.val[1], q2_tw1_s.val[1]);
            q_s2_i_s = vfms_f16 (q_s2_i_s, q2_in2_s.val[0], q2_tw1_s.val[1]);
            q_s3_r_s = vfma_f16 (q_s3_r_s, q2_in3_s.val[1], q2_tw2_s.val[1]);
            q_s3_i_s = vfms_f16 (q_s3_i_s, q2_in3_s.val[0], q2_tw2_s.val[1]);
    
            // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
            // <X[N/4], X[3N/4]> components.
            q_s4_r_s = vadd_f16 (q2_in0_s.val[0], q_s2_r_s);
            q_s4_i_s = vadd_f16 (q2_in0_s.val[1], q_s2_i_s);
            q_s5_r_s = vsub_f16 (q2_in0_s.val[0], q_s2_r_s);
            q_s5_i_s = vsub_f16 (q2_in0_s.val[1], q_s2_i_s);
            q_s6_r_s = vadd_f16 (q_s1_r_s, q_s3_r_s);
            q_s6_i_s = vadd_f16 (q_s1_i_s, q_s3_i_s);
            q_s7_r_s = vsub_f16 (q_s1_r_s, q_s3_r_s);
            q_s7_i_s = vsub_f16 (q_s1_i_s, q_s3_i_s);
    
            // Combine these sums (for the full radix-4 butterfly)
            q2_out2_s.val[0] = vsub_f16 (q_s4_r_s, q_s6_r_s);
            q2_out2_s.val[1] = vsub_f16 (q_s4_i_s, q_s6_i_s);
            q2_out0_s.val[0] = vadd_f16 (q_s4_r_s, q_s6_r_s);
            q2_out0_s.val[1] = vadd_f16 (q_s4_i_s, q_s6_i_s);
            q2_out1_s.val[0] = vsub_f16 (q_s5_r_s, q_s7_i_s);
            q2_out1_s.val[1] = vadd_f16 (q_s5_i_s, q_s7_r_s);
            q2_out3_s.val[0] = vadd_f16 (q_s5_r_s, q_s7_i_s);
            q2_out3_s.val[1] = vsub_f16 (q_s5_i_s, q_s7_r_s);
    
            // Store the results
            vst2_f16 (p_dst, q2_out0_s);
            p_dst += dst_step;
            vst2_f16 (p_dst, q2_out1_s);
            p_dst += dst_step;
            vst2_f16 (p_dst, q2_out2_s);
            p_dst += dst_step;
            vst2_f16 (p_dst, q2_out3_s);
            p_dst += dst_step;
    
            // Adjust p_src, p_dst, p_tw for the next loop iteration
            p_src = p_src - src_step * 4 + 8;
            p_dst = p_dst - dst_step * 4 + 8;
            p_tw = p_tw - tw_step * 2 + 8;
        } // m_count
    }
    else
    {
        for (m_count = 0; m_count < mstride; m_count += 8)
        {
            // Load the input values
            q2_in0 = vld2q_f16 (p_src);
            p_src += src_step;
            q2_in1 = vld2q_f16 (p_src);
            p_src += src_step;
            q2_in2 = vld2q_f16 (p_src);
            p_src += src_step;
            q2_in3 = vld2q_f16 (p_src);
            p_src += src_step;
    
            // Load the twiddles
            q2_tw0 = vld2q_f16 (p_tw);
            p_tw += tw_step;
            q2_tw1 = vld2q_f16 (p_tw);
            p_tw += tw_step;
            q2_tw2 = vld2q_f16 (p_tw);
    
            // Multiply input elements by their associated twiddles
            q_s1_r = vmulq_f16 (q2_in1.val[0], q2_tw0.val[0]);
            q_s1_i = vmulq_f16 (q2_in1.val[1], q2_tw0.val[0]);
            q_s2_r = vmulq_f16 (q2_in2.val[0], q2_tw1.val[0]);
            q_s2_i = vmulq_f16 (q2_in2.val[1], q2_tw1.val[0]);
            q_s3_r = vmulq_f16 (q2_in3.val[0], q2_tw2.val[0]);
            q_s3_i = vmulq_f16 (q2_in3.val[1], q2_tw2.val[0]);
            q_s1_r = vfmaq_f16 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
            q_s1_i = vfmsq_f16 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
            q_s2_r = vfmaq_f16 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
            q_s2_i = vfmsq_f16 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
            q_s3_r = vfmaq_f16 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
            q_s3_i = vfmsq_f16 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);
    
            // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
            // <X[N/4], X[3N/4]> components.
            q_s4_r = vaddq_f16 (q2_in0.val[0], q_s2_r);
            q_s4_i = vaddq_f16 (q2_in0.val[1], q_s2_i);
            q_s5_r = vsubq_f16 (q2_in0.val[0], q_s2_r);
            q_s5_i = vsubq_f16 (q2_in0.val[1], q_s2_i);
            q_s6_r = vaddq_f16 (q_s1_r, q_s3_r);
            q_s6_i = vaddq_f16 (q_s1_i, q_s3_i);
            q_s7_r = vsubq_f16 (q_s1_r, q_s3_r);
            q_s7_i = vsubq_f16 (q_s1_i, q_s3_i);
    
            // Combine these sums (for the full radix-4 butterfly)
            q2_out2.val[0] = vsubq_f16 (q_s4_r, q_s6_r);
            q2_out2.val[1] = vsubq_f16 (q_s4_i, q_s6_i);
            q2_out0.val[0] = vaddq_f16 (q_s4_r, q_s6_r);
            q2_out0.val[1] = vaddq_f16 (q_s4_i, q_s6_i);
            q2_out1.val[0] = vsubq_f16 (q_s5_r, q_s7_i);
            q2_out1.val[1] = vaddq_f16 (q_s5_i, q_s7_r);
            q2_out3.val[0] = vaddq_f16 (q_s5_r, q_s7_i);
            q2_out3.val[1] = vsubq_f16 (q_s5_i, q_s7_r);
    
            // Store the results
            vst2q_f16 (p_dst, q2_out0);
            p_dst += dst_step;
            vst2q_f16 (p_dst, q2_out1);
            p_dst += dst_step;
            vst2q_f16 (p_dst, q2_out2);
            p_dst += dst_step;
            vst2q_f16 (p_dst, q2_out3);
            p_dst += dst_step;
    
            // Adjust p_src, p_dst, p_tw for the next loop iteration
            p_src = p_src - src_step * 4 + 16;
            p_dst = p_dst - dst_step * 4 + 16;
            p_tw = p_tw - tw_step * 2 + 16;
        } // m_count
    }
}

__STATIC_INLINE void ne10_radix4x4_inverse_with_twiddles_last_stage_neon (ne10_fft_cpx_float16_t *out,
        const ne10_fft_cpx_float16_t *in,
        const ne10_fft_cpx_float16_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride,
        ne10_int32_t nfft)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    const float16_t *p_src = (const float16_t *) in;
    float16_t *p_dst = (float16_t *) out;
    const float16_t *p_tw  = (const float16_t *) tw;
    ne10_float16_t one_by_nfft = (1.0f16 / (_Float16) nfft);

    float16x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float16x4x2_t q2_tw0, q2_tw1, q2_tw2;
    float16x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float16x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float16x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;
    float16x4_t q_one_by_nfft = vdup_n_f16 (one_by_nfft);

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // Load the input values
        q2_in0 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in1 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in2 = vld2_f16 (p_src);
        p_src += src_step;
        q2_in3 = vld2_f16 (p_src);
        p_src += src_step;

        // Load the twiddles
        q2_tw0 = vld2_f16 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2_f16 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2_f16 (p_tw);

        // Multiply input elements by their associated twiddles
        q_s1_r = vmul_f16 (q2_in1.val[0], q2_tw0.val[0]);
        q_s1_i = vmul_f16 (q2_in1.val[1], q2_tw0.val[0]);
        q_s2_r = vmul_f16 (q2_in2.val[0], q2_tw1.val[0]);
        q_s2_i = vmul_f16 (q2_in2.val[1], q2_tw1.val[0]);
        q_s3_r = vmul_f16 (q2_in3.val[0], q2_tw2.val[0]);
        q_s3_i = vmul_f16 (q2_in3.val[1], q2_tw2.val[0]);
        q_s1_r = vfma_f16 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
        q_s1_i = vfms_f16 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
        q_s2_r = vfma_f16 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
        q_s2_i = vfms_f16 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
        q_s3_r = vfma_f16 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
        q_s3_i = vfms_f16 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s4_r = vadd_f16 (q2_in0.val[0], q_s2_r);
        q_s4_i = vadd_f16 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsub_f16 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsub_f16 (q2_in0.val[1], q_s2_i);
        q_s6_r = vadd_f16 (q_s1_r, q_s3_r);
        q_s6_i = vadd_f16 (q_s1_i, q_s3_i);
        q_s7_r = vsub_f16 (q_s1_r, q_s3_r);
        q_s7_i = vsub_f16 (q_s1_i, q_s3_i);

        // Combine these sums (for the full radix-4 butterfly)
        q2_out2.val[0] = vsub_f16 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsub_f16 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vadd_f16 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vadd_f16 (q_s4_i, q_s6_i);
        q2_out1.val[0] = vsub_f16 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vadd_f16 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vadd_f16 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsub_f16 (q_s5_i, q_s7_r);

        // Multiply by (1 / nfft)
        q2_out0.val[0] = vmul_f16 (q2_out0.val[0], q_one_by_nfft);
        q2_out0.val[1] = vmul_f16 (q2_out0.val[1], q_one_by_nfft);
        q2_out1.val[0] = vmul_f16 (q2_out1.val[0], q_one_by_nfft);
        q2_out1.val[1] = vmul_f16 (q2_out1.val[1], q_one_by_nfft);
        q2_out2.val[0] = vmul_f16 (q2_out2.val[0], q_one_by_nfft);
        q2_out2.val[1] = vmul_f16 (q2_out2.val[1], q_one_by_nfft);
        q2_out3.val[0] = vmul_f16 (q2_out3.val[0], q_one_by_nfft);
        q2_out3.val[1] = vmul_f16 (q2_out3.val[1], q_one_by_nfft);

        // Store the results
        vst2_f16 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2_f16 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2_f16 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2_f16 (p_dst, q2_out3);
        p_dst += dst_step;

        // Adjust p_src, p_dst, p_tw for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    } // m_count
}

/*
 * This function calculates the FFT for power-of-two input sizes using an ordered, mixed
 * radix-4/8 DIT algorithm. It is very similar to its C counterpart in NE10_fft_float16.c,
 * "ne10_mixed_radix_butterfly_float16_c".
 */


// Use arm_ to be able to use Ne10 with this library
// since Ne10 cover radix-3 and 5 that are not provided by CMSIS-DSP.
void arm_ne10_mixed_radix_fft_forward_float16_neon (
    const arm_cfft_instance_f16 *S,
    const ne10_fft_cpx_float16_t *input,
          ne10_fft_cpx_float16_t *out,
          ne10_fft_cpx_float16_t *buffer)

{
    ne10_int32_t stage_count = S->factors[0];
    ne10_int32_t fstride = S->factors[1];
    ne10_int32_t mstride = S->factors[3];
    ne10_int32_t first_radix = S->factors[2];
    ne10_int32_t step, f_count;
    const ne10_fft_cpx_float16_t *src = input;
    ne10_fft_cpx_float16_t *in;
    ne10_fft_cpx_float16_t *dst = out;
    ne10_fft_cpx_float16_t *out_final = out;
    const ne10_fft_cpx_float16_t *tw; 
    ne10_fft_cpx_float16_t *tmp;

    const ne10_fft_cpx_float16_t *twiddles = (const ne10_fft_cpx_float16_t *)S->pTwiddle;

    //printf("CMSIS : stage_count %d\n",stage_count);
    //printf("CMSIS : fstride %d\n",fstride);
    //printf("CMSIS : mstride %d\n",mstride);
    //printf("CMSIS : first_radix %d\n",first_radix);

    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // For our factoring, this means nfft is of form 2^{odd}
    {
        //printf("ne10_radix8x4_neon\n");
        ne10_radix8x4_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride << 1; // For C2C, 1/4 of input size (fstride is nfft/8)
        stage_count--;
        fstride /= 4;
    }
    else if (first_radix == 4) // For our factoring, this means nfft is of form 2^{even}
    {
        //printf("ne10_radix4x4_without_twiddles_neon\n");
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
            //printf("ne10_radix4x4_with_twiddles_neon\n");
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
            //printf("ne10_radix4x4_with_twiddles_neon\n");
            ne10_radix4x4_with_twiddles_neon (dst, src, tw, step, step, mstride);
            src += mstride;
            dst += mstride;
        } // f_count
    } // last stage

}


/*
 * This function calculates the inverse FFT, and is very similar in structure to its
 * complement "ne10_mixed_radix_fft_forward_float16_neon".
 */


void arm_ne10_mixed_radix_fft_backward_float16_neon (
    const arm_cfft_instance_f16 *S,
    const ne10_fft_cpx_float16_t *input,
          ne10_fft_cpx_float16_t *out,
          ne10_fft_cpx_float16_t *buffer)
{
    ne10_int32_t stage_count = S->factors[0];
    ne10_int32_t fstride = S->factors[1];
    ne10_int32_t mstride = S->factors[3];
    ne10_int32_t first_radix = S->factors[2];
    ne10_int32_t nfft = fstride * first_radix;
    ne10_int32_t step, f_count;
    const ne10_fft_cpx_float16_t *src = input;
    ne10_fft_cpx_float16_t *in;
    ne10_fft_cpx_float16_t *dst = out;
    ne10_fft_cpx_float16_t *out_final = out;
    const ne10_fft_cpx_float16_t *tw;

    ne10_fft_cpx_float16_t *tmp;

    const ne10_fft_cpx_float16_t *twiddles = (const ne10_fft_cpx_float16_t *)S->pTwiddle;


    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // nfft is of form 2^{odd}
    {
        //printf("ne10_radix8x4_inverse_neon\n");
        ne10_radix8x4_inverse_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride << 1;
        stage_count--;
        fstride /= 4;
    }
    else if (first_radix == 4) // nfft is of form 2^{even}
    {
        //printf("ne10_radix4x4_inverse_without_twiddles_neon\n");
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
            //printf("ne10_radix4x4_inverse_with_twiddles_neon\n");
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
            //printf("ne10_radix4x4_inverse_with_twiddles_last_stage_neon\n");
            ne10_radix4x4_inverse_with_twiddles_last_stage_neon (dst, src, tw, step, step, mstride, nfft);
            src += mstride;
            dst += mstride;
        } // f_count
    } // last stage
}

#endif