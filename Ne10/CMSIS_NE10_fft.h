/*
 *  Copyright 2012-16 ARM Limited and Contributors.
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

/* Modified for import into CMSIS-DSP */

#ifndef CMSIS_NE10_DSP_H 
#define CMSIS_NE10_DSP_H

#include "CMSIS_NE10_types.h"
#include "dsp/transform_functions.h"

#if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)
#include "dsp/transform_functions_f16.h"
#endif

extern void arm_ne10_fft16_forward_float32_neon (const arm_cfft_instance_f32 *S,
          const ne10_fft_cpx_float32_t *Fin,
          ne10_fft_cpx_float32_t *Fout);

extern void arm_ne10_fft16_backward_float32_neon (const arm_cfft_instance_f32 *S,
          const ne10_fft_cpx_float32_t *Fin,
          ne10_fft_cpx_float32_t *Fout);

extern void arm_ne10_mixed_radix_fft_forward_float32_neon (const arm_cfft_instance_f32 *S,
        const ne10_fft_cpx_float32_t *in,
        ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *buffer);

extern void arm_ne10_mixed_radix_fft_backward_float32_neon (const arm_cfft_instance_f32 *S,
        const ne10_fft_cpx_float32_t *in,
        ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *buffer);

extern void arm_ne10_fft_r2c_1d_float32_neon (const arm_rfft_fast_instance_f32 * S,
                                              const float32_t *in,
                                              float32_t *out,
                                              float32_t *buffer);


extern void arm_ne10_fft_c2r_1d_float32_neon (const arm_rfft_fast_instance_f32 * S,
                                              const float32_t *in,
                                              float32_t *out,
                                              float32_t *buffer);

#if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)

extern void arm_ne10_mixed_radix_fft_forward_float16_neon (const arm_cfft_instance_f16 *S,
        const ne10_fft_cpx_float16_t *in,
        ne10_fft_cpx_float16_t *out,
        ne10_fft_cpx_float16_t *buffer);

extern void arm_ne10_mixed_radix_fft_backward_float16_neon (const arm_cfft_instance_f16 *S,
        const ne10_fft_cpx_float16_t *in,
        ne10_fft_cpx_float16_t *out,
        ne10_fft_cpx_float16_t *buffer);

extern void arm_ne10_fft_r2c_1d_float16_neon (const arm_rfft_fast_instance_f16 * S,
                                              const float16_t *in,
                                              float16_t *out,
                                              float16_t *buffer);


extern void arm_ne10_fft_c2r_1d_float16_neon (const arm_rfft_fast_instance_f16 * S,
                                              const float16_t *in,
                                              float16_t *out,
                                              float16_t *buffer);
#endif 

extern void arm_ne10_fft_c2c_1d_int32_neon (q31_t *fout,
                                 const q31_t *fin,
                                 const arm_cfft_instance_q31* cfg,
                                 ne10_int32_t inverse_fft,
                                 ne10_int32_t scaled_flag,
                                 q31_t *buffer);

extern void arm_ne10_fft_r2c_1d_int32_neon (q31_t *fout,
                                 const q31_t *fin,
                                 const arm_rfft_instance_q31* S,
                                 int32_t scaled_flag,
                                 q31_t *buffer);
extern void arm_ne10_fft_c2r_1d_int32_neon (q31_t *fout,
                                 const q31_t *fin,
                                 const arm_rfft_instance_q31* S,
                                 int32_t scaled_flag,
                                 q31_t *buffer);

extern void arm_ne10_fft_c2c_1d_int16_neon (q15_t *fout,
                                 const q15_t *fin,
                                 const arm_cfft_instance_q15* cfg,
                                 ne10_int32_t inverse_fft,
                                 ne10_int32_t scaled_flag,
                                 q15_t *buffer);

extern void arm_ne10_fft_r2c_1d_int16_neon (q15_t *fout,
                                 const q15_t *fin,
                                 const arm_rfft_instance_q15* cfg,
                                 ne10_int32_t scaled_flag,
                                 q15_t *buffer);

extern void arm_ne10_fft_c2r_1d_int16_neon (q15_t *fout,
                                 const q15_t *fin,
                                 const arm_rfft_instance_q15* cfg,
                                 ne10_int32_t scaled_flag,
                                 q15_t *buffer);

extern void arm_ne10_mixed_radix_generic_butterfly_inverse_float32_neon (
        const arm_cfft_instance_f32 *S,
        const ne10_fft_cpx_float32_t *in,
        ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *buffer);

extern void arm_ne10_mixed_radix_generic_butterfly_float32_neon (
        const arm_cfft_instance_f32 *S,
        const ne10_fft_cpx_float32_t *in,
        ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *buffer);

#if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)

extern void arm_ne10_mixed_radix_generic_butterfly_inverse_float16_neon (
        const arm_cfft_instance_f16 *S,
        const ne10_fft_cpx_float16_t *in,
        ne10_fft_cpx_float16_t *out,
        ne10_fft_cpx_float16_t *buffer);

extern void arm_ne10_mixed_radix_generic_butterfly_float16_neon (
        const arm_cfft_instance_f16 *S,
        const ne10_fft_cpx_float16_t *in,
        ne10_fft_cpx_float16_t *out,
        ne10_fft_cpx_float16_t *buffer);

#endif

extern void arm_ne10_mixed_radix_generic_butterfly_inverse_int32_neon (
        const arm_cfft_instance_q31 *S,
        const ne10_fft_cpx_int32_t *in,
        ne10_fft_cpx_int32_t *out,
        ne10_fft_cpx_int32_t *buffer,
        ne10_int32_t scaled_flag);

extern void arm_ne10_mixed_radix_generic_butterfly_int32_neon (
        const arm_cfft_instance_q31 *S,
        const ne10_fft_cpx_int32_t *in,
        ne10_fft_cpx_int32_t *out,
        ne10_fft_cpx_int32_t *buffer,
        ne10_int32_t scaled_flag);

#endif