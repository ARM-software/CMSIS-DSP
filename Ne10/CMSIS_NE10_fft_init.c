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
 * NE10 Library : Functions coming from NE10_fft.c
 */

#include "arm_compiler_specific.h"
#include <arm_neon.h>

#include "CMSIS_NE10_types.h"
#include "CMSIS_NE10_fft.h"
#include "CMSIS_NE10_macros.h"

#include "dsp/transform_functions.h"
#include "dsp/transform_functions_f16.h"

#include <stdlib.h>
//#include <stdio.h>




/*
 * This function outputs a factor buffer ('facbuf') that decomposes an FFT of input size
 * n into a number of radix-r butterfly calculations (for r in some set of radix values).
 *
 * Factor buffer layout:
 *     index 0: stage count
 *     index 1: stride for the first stage
 *     index 2 to (2*stage_count + 1): pairs of factors (number of sections, section size)
 *     index (2*stage_count + 2): an flag specifying which algorithm to use
 *
 * e.g. 1024 samples might result in the following five stage radix-4 factors buffer:
 *          [5, 256, 4, 256, 4, 64, 4, 16, 4, 4, 4, 1]
 *          i.e. 1024 = 4x256, each of which is 4x64, each of which is 4x16, each of which
 *               is 4x4, each of which is 4x1. There are 5 stages, and the stride for the
 *               first stage is 256 (1024 / 4, for radix-4).
 *
 * Only the leading 42 int32 is used to store factors.
 * The left can be used as algorithm flags, or status flags.
 * Even the leading bits of stage number can be reused.
 */
static ne10_int32_t ne10_factor (ne10_int32_t n,
        ne10_uint32_t * facbuf,
        ne10_int32_t ne10_factor_flags)
{
    //printf("n=%d\n",n);
    // This is a workaround. We need to "return" some flags.
    // Otherwise, we need to modify signature of ne10_factor.
    assert (NE10_MAXFACTORS >= 32);

    if ((facbuf == NULL)
        || (n <= 0))
    {
        return NE10_ERR;
    }

    ne10_int32_t p;
    ne10_int32_t i = 1;
    ne10_int32_t stage_num = 0;
    ne10_int32_t stride_max = n;

    // Default algorithm flag is NE10_FFT_ALG_DEFAULT
    ne10_int32_t alg_flag = NE10_FFT_ALG_DEFAULT;

    // Factor out powers of 4, 2, 5, and 3. Additionally, factor out powers
    // of 8 if the right factor flags are passed. If none of these factors
    // can be applied at any stage, the remaining size is used as a factor.
    do
    {
        // If NE10_FACTOR_EIGHT_FIRST_STAGE is enabled, we can generate
        // a first stage of radix-8 (e.g. by combining one radix-4 and
        // one radix-2 stage into a single radix-8 stage).
        if ((ne10_factor_flags & NE10_FACTOR_EIGHT_FIRST_STAGE)
                && ((n==8) || (n==40) || (n==24)))
        {
            switch (n)
            {
            case 8:
                p = 8;
                break;
            case 24:
                p = 3;
                alg_flag = NE10_FFT_ALG_ANY;
                break;
            default: // n == 40
                p = 5;
                alg_flag = NE10_FFT_ALG_ANY;
                break;
            }
        }
        else if ((n % 4) == 0)
        {
            p = 4;
        }
        else if ((n % 2) == 0)
        {
            p = 2;
        }
        else if ((n % 5) == 0)
        {
            p = 5;
            alg_flag = NE10_FFT_ALG_ANY;
        }
        else if ((n % 3) == 0)
        {
            p = 3;
            alg_flag = NE10_FFT_ALG_ANY;
        }
        else // stop factoring
        {
            p = n;
            alg_flag = NE10_FFT_ALG_ANY;
        }

        //printf("p = %d\n",p);

        n /= p;
        facbuf[2 * i] = p;
        facbuf[2 * i + 1] = n;
        i++;
        stage_num++;
    }
    while (n > 1);
    facbuf[0] = stage_num;
    facbuf[1] = stride_max / p;

    if (stage_num > 21)
    {
        // Since nfft is ne10_int32_t, stage_num can never be greater than 21,
        // because 3^21 > 2^32
        return NE10_ERR;
    }

    facbuf[2 * i] = alg_flag;

    //for(int k=0;k<2*i;k++)
    //{
    //   printf("%d ",facbuf[k]);
    //}
    //printf("\n");
    
    return NE10_OK;
}

// Twiddles matrix [radix-1][mstride]
// First column (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
static void ne10_fft_generate_twiddles_line_float32 (ne10_fft_cpx_float32_t * twiddles,
        const ne10_int32_t mstride,
        const ne10_int32_t fstride,
        const ne10_int32_t radix,
        const ne10_int32_t nfft)
{
    ne10_int32_t j, k;
    ne10_float64_t phase;
    const ne10_float64_t pi = NE10_PI;
    //printf("%d %d %d %d\n",mstride,fstride,radix,nfft);

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;
            twiddles[mstride * (k - 1) + j].r = (ne10_float32_t) cos ((double)phase);
            twiddles[mstride * (k - 1) + j].i = (ne10_float32_t) sin ((double)phase);
        } // radix
    } // mstride
}


// Transposed twiddles matrix [mstride][radix-1]
// First row (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
// Transposed twiddle tables are used in RFFT to avoid memory access by a large
// stride.
static void ne10_fft_generate_twiddles_line_transposed_float32 (
    ne10_fft_cpx_float32_t* twiddles,
    const ne10_int32_t mstride,
    const ne10_int32_t fstride,
    const ne10_int32_t radix,
    const ne10_int32_t nfft)
{
    //printf("Transposed\n");

    ne10_int32_t j, k;
    ne10_float64_t phase;
    const ne10_float64_t pi = NE10_PI;

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;
            twiddles[(radix - 1) * j + k - 1].r = (ne10_float32_t) cos ((double)phase);
            twiddles[(radix - 1) * j + k - 1].i = (ne10_float32_t) sin ((double)phase);
        } // radix
    } // mstride
}



// Twiddles matrix [mstride][radix-1]
// First column (k == 0)is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
static void ne10_fft_generate_twiddles_line_int32 (ne10_fft_cpx_int32_t * twiddles,
        const ne10_int32_t mstride,
        const ne10_int32_t fstride,
        const ne10_int32_t radix,
        const ne10_int32_t nfft)
{
    ne10_int32_t j, k;
    ne10_float64_t phase;
    const ne10_float64_t pi = NE10_PI;

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;

            ne10_fft_cpx_int32_t *tw = &twiddles[mstride * (k - 1) + j];

            tw->r = (ne10_int32_t) floor (0.5 + NE10_F2I32_MAX * cos((double)phase));
            tw->i = (ne10_int32_t) floor (0.5 + NE10_F2I32_MAX * sin((double)phase));
        } // radix
    } // mstride
}



static ne10_fft_cpx_int32_t* ne10_fft_generate_twiddles_int32 (ne10_fft_cpx_int32_t * twiddles,
        const ne10_uint32_t * factors,
        const ne10_int32_t nfft )
{
    ne10_uint32_t stage_count = factors[0];
    ne10_uint32_t fstride = factors[1];
    ne10_uint32_t mstride;
    ne10_uint32_t cur_radix; // current radix

    // for first stage
    cur_radix = factors[2 * stage_count];
    if (cur_radix % 2) // current radix is not 4 or 2
    {
        twiddles += 1;
        ne10_fft_generate_twiddles_line_int32 (twiddles, 1, fstride, cur_radix, nfft);
        twiddles += cur_radix - 1;
    }
    stage_count--;

    // for other stage
    for (; stage_count > 0; stage_count--)
    {
        cur_radix = factors[2 * stage_count];
        fstride /= cur_radix;
        mstride = factors[2 * stage_count + 1];
        ne10_fft_generate_twiddles_line_int32 (twiddles, mstride, fstride, cur_radix, nfft);
        twiddles += mstride * (cur_radix - 1);
    } // stage_count

    return twiddles;
}


typedef void (*line_generator_float32)(ne10_fft_cpx_float32_t*,
      const ne10_int32_t,
      const ne10_int32_t,
      const ne10_int32_t,
      const ne10_int32_t);

static ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_impl_float32 (
      line_generator_float32 generator,
      ne10_fft_cpx_float32_t * twiddles,
      const ne10_uint32_t * factors,
      const ne10_int32_t nfft)
{
    ne10_uint32_t stage_count = factors[0];
    ne10_uint32_t fstride = factors[1];
    ne10_uint32_t mstride;
    ne10_uint32_t cur_radix; // current radix

    //printf("stage count = %d, fstride = %d\n",stage_count,fstride);

    // for first stage
    cur_radix = factors[2 * stage_count];
    //printf("cur radix = %d\n",cur_radix);

    if (cur_radix % 2) // current radix is not 4 or 2
    {
        //printf("Gen cur radix\n");
        twiddles[0].r = 1.0;
        twiddles[0].i = 0.0;
        twiddles += 1;
        generator (twiddles, 1, fstride, cur_radix, nfft);
        twiddles += cur_radix - 1;
    }
    stage_count --;

    // for other stage
    for (; stage_count > 0; stage_count --)
    {
        cur_radix = factors[2 * stage_count];
        //printf("state=%d, cur radix = %d\n",stage_count,cur_radix);
        fstride /= cur_radix;
        mstride = factors[2 * stage_count + 1];
        generator (twiddles, mstride, fstride, cur_radix, nfft);
        twiddles += mstride * (cur_radix - 1);
    } // stage_count

    return twiddles;
}

static ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_float32 (ne10_fft_cpx_float32_t * twiddles,
        const ne10_uint32_t * factors,
        const ne10_int32_t nfft )
{
    line_generator_float32 generator = ne10_fft_generate_twiddles_line_float32;
    twiddles = ne10_fft_generate_twiddles_impl_float32(generator,
        twiddles, factors, nfft);
    return twiddles;
}


static ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_transposed_float32 (
      ne10_fft_cpx_float32_t * twiddles,
      const ne10_uint32_t * factors,
      const ne10_int32_t nfft)
{
    //printf("Gen Transposed\n");
    line_generator_float32 generator =
        ne10_fft_generate_twiddles_line_transposed_float32;
    twiddles = ne10_fft_generate_twiddles_impl_float32(generator,
        twiddles, factors, nfft);
    return twiddles;
}


/**
  @addtogroup ComplexFFTF32
  @{
 */

/**
 * @brief      Initialize data structure for a FFT
 *
 * @param[in]  fftLen  The fft length
 *
 * @return     Pointer to the new structure
 * 
 * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with FFT lengths
 *            longer than the ones supported on Cortex-M
 *            
 *            With this function, FFT lengths can also contain
 *            factor 3 and 5 (but must be multiple of 4)
 *            
 */
arm_cfft_instance_f32 *arm_cfft_init_dynamic_f32(uint32_t fftLen)
{
    arm_cfft_instance_f32* st = NULL;
    ne10_uint32_t memneeded = sizeof (arm_cfft_instance_f32)
                              + sizeof (ne10_uint32_t) * (NE10_MAXFACTORS * 2) /* factors */
                              + sizeof (ne10_fft_cpx_float32_t) * fftLen       /* twiddles */
                              + NE10_FFT_BYTE_ALIGNMENT;             /* 64-bit alignment */

    st = (arm_cfft_instance_f32*) NE10_MALLOC (memneeded);

    // Bad allocation.
    if (st == NULL)
    {
        //printf("Bad alloc\n");
        return NULL;
    }

    // Only backward FFT is scaled by default.
    //st->is_forward_scaled = 0;
    //st->is_backward_scaled = 1;

    uintptr_t address = (uintptr_t) st + sizeof (arm_cfft_instance_f32);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
    
    ne10_uint32_t *factors = (ne10_uint32_t*) address;

    st->factors = factors;
    ne10_fft_cpx_float32_t *pTwiddle =  (ne10_fft_cpx_float32_t*)(st->factors + (NE10_MAXFACTORS * 2));
    st -> pTwiddle = (const float32_t*)pTwiddle;
    // st->last_twiddles is default NULL.
    // Calling fft_c or fft_neon is decided by this pointers.
    st->last_twiddles = NULL;

    //printf("fftLen = %d\n",fftLen);
    st->fftLen = fftLen;

    if ((fftLen % NE10_FFT_PARA_LEVEL) == 0)
    {
       st->fftLen /= NE10_FFT_PARA_LEVEL;
       st->last_twiddles = (const float32_t*)(pTwiddle + fftLen / NE10_FFT_PARA_LEVEL);
    }
        
    ne10_int32_t result = ne10_factor (st->fftLen, factors, NE10_FACTOR_EIGHT_FIRST_STAGE);

    // Cannot factor
    if (result == NE10_ERR)
    {
        //printf("Cannot factor\n");
        NE10_FREE (st);
        return NULL;
    }

    ne10_int32_t stage_count    = st->factors[0];
    st->algorithm_flag = st->factors[2 * (stage_count + 1)];

    if (st->algorithm_flag == NE10_FFT_ALG_ANY)
    {
        //printf("Any\n");
        if (fftLen % NE10_FFT_PARA_LEVEL)
        {
            //printf("Multiple paral level\n");
            NE10_FREE (st);
            return NULL;
        }

        //printf("Gen twiddle\n");
        ne10_fft_generate_twiddles_float32 (pTwiddle, factors, st->fftLen);

        // Generate super twiddles for the last stage.
        ne10_fft_generate_twiddles_line_float32 ((ne10_fft_cpx_float32_t *)st->last_twiddles,
                st->fftLen,
                1,
                NE10_FFT_PARA_LEVEL,
                fftLen);
        st->fftLen *= NE10_FFT_PARA_LEVEL;
    }
    else
    {
        //printf("Standard\n");
        if ((fftLen % NE10_FFT_PARA_LEVEL) == 0)
        {
            st->fftLen = fftLen;
    
            // Adjust the factoring for a size "nfft / 4" FFT to work for size "nfft"
            if (stage_count > NE10_MAXFACTORS - 4)
            {
                //printf("Too big stage count\n");
                NE10_FREE (st);
                return NULL;
            }
            factors[0]++;          // Bump the stage count
            factors[1] *= 4;       // Quadruple the first stage stride
            memmove(&factors[4], &factors[2], ((2 * (stage_count + 1)) - 1) * sizeof(factors[0]));
            factors[2] = 4;        // Add a new radix-4 stage
            factors[3] = fftLen / 4;
        }
        else
        {
            //printf("Not multiple para level\n");
            NE10_FREE (st);
            return NULL;
        }
        ne10_fft_generate_twiddles_float32 (pTwiddle, factors, st->fftLen);
    
        // CMSIS-DSP standard init (constant table) do not
        // need the full factor array
        factors[2] = st->factors[2 * (stage_count+1)];        // first radix
        factors[3] = st->factors[2 * (stage_count+1)]; // mstride

    }


    
    
    //printf("%d %d %d %d\n",factors[0],factors[1],factors[2],factors[3]);

    return st;
}

/**
  @} end of ComplexFFTF32 group
 */

/**
  @addtogroup ComplexFFTQ31
  @{
 */

/**
 * @brief      Initialize data structure for a FFT
 *
 * @param[in]  fftLen  The fft length
 *
 * @return     Pointer to the new structure
 * 
 * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with FFT lengths
 *            longer than the ones supported on Cortex-M
 *            
 *            With this function, FFT lengths can also contain
 *            factor 3 and 5 (but must be multiple of 4)
 */
arm_cfft_instance_q31 *arm_cfft_init_dynamic_q31(uint32_t fftLen)
{
    arm_cfft_instance_q31* st = NULL;
    ne10_uint32_t memneeded = sizeof (arm_cfft_instance_q31)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2) /* factors */
                              + sizeof (ne10_fft_cpx_int32_t) * fftLen         /* twiddles */
                              + NE10_FFT_BYTE_ALIGNMENT;             /* 64-bit alignment */

    st = (arm_cfft_instance_q31*) NE10_MALLOC (memneeded);
    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (arm_cfft_instance_q31);
        NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
        uint32_t *factors = (ne10_uint32_t*) address;
        st->factors = factors;

        ne10_fft_cpx_int32_t* twiddles = (ne10_fft_cpx_int32_t*) (st->factors + (NE10_MAXFACTORS * 2));
        st->pTwiddle = (q31_t*)twiddles;
        st->fftLen = fftLen;

        

        ne10_int32_t result = ne10_factor (fftLen, factors, NE10_FACTOR_EIGHT_FIRST_STAGE);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return NULL;
        }
        
        ne10_int32_t stage_count    = st->factors[0];
        st->algorithm_flag = st->factors[2 * (stage_count + 1)];
        
        if (st->algorithm_flag == NE10_FFT_ALG_ANY)
        {
            if (fftLen % NE10_FFT_PARA_LEVEL == 0)
            {
              // Size of FFT satisfies requirement of NEON optimization.
              st->fftLen /= NE10_FFT_PARA_LEVEL;
              st->last_twiddles = (const q31_t*)(twiddles + fftLen / NE10_FFT_PARA_LEVEL);
            }

            // Disable radix 8 for INT32 generic FFTs (it isn't supported)
            result = ne10_factor (st->fftLen, factors, NE10_FACTOR_DEFAULT);
            if ((result == NE10_ERR) || (fftLen % NE10_FFT_PARA_LEVEL))
            {
                NE10_FREE (st);
                return NULL;
            }
    
            ne10_fft_generate_twiddles_int32 (twiddles, st->factors, st->fftLen);
    
            // Generate super twiddles for the last stage.
            
            ne10_fft_generate_twiddles_line_int32 ((ne10_fft_cpx_int32_t*)st->last_twiddles,
                    st->fftLen,
                    1,
                    NE10_FFT_PARA_LEVEL,
                    fftLen);
            
            st->fftLen *= NE10_FFT_PARA_LEVEL;
        }
        else
        {
            ne10_fft_generate_twiddles_int32 (twiddles, factors, fftLen);
    
            factors[2] = st->factors[2 * stage_count];        // first radix
            factors[3] = st->factors[2 * stage_count]; // mstride
        }
        //printf("%d %d %d %d\n",factors[0],factors[1],factors[2],factors[3]);

    }

    return st;
}

/**
  @} end of ComplexFFTQ31 group
 */

/**
  @addtogroup ComplexFFTQ15
  @{
 */
/**
 * @brief      Initialize data structure for a FFT
 *
 * @param[in]  fftLen  The fft length
 *
 * @return     Pointer to the new structure
 * 
 * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with FFT lengths
 *            longer than the ones supported on Cortex-M
 *            
 *            With this function, FFT lengths can also contain
 *            factor 3 and 5 (but must be multiple of 4)
 */
arm_cfft_instance_q15 *arm_cfft_init_dynamic_q15(uint32_t fftLen)
{
    arm_cfft_instance_q15* st = NULL;
    ne10_uint32_t memneeded = sizeof (arm_cfft_instance_q15)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2) /* factors */
                              + sizeof (ne10_fft_cpx_int16_t) * (fftLen)       /* twiddles */
                              + NE10_FFT_BYTE_ALIGNMENT;             /* 64-bit alignment */

    st = (arm_cfft_instance_q15*) NE10_MALLOC (memneeded);

    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (arm_cfft_instance_q15);
        NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
        ne10_uint32_t *factors = (ne10_uint32_t*) address;
        st->factors = factors;

        ne10_fft_cpx_int16_t *twiddles = (ne10_fft_cpx_int16_t*) (st->factors + (NE10_MAXFACTORS * 2));
        st->pTwiddle = (q15_t*)twiddles;
        st->fftLen = fftLen;

        ne10_int32_t result = ne10_factor (fftLen, factors, NE10_FACTOR_EIGHT_FIRST_STAGE);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return NULL;
        }

        ne10_int32_t j, k;
        ne10_int32_t stage_count = factors[0];
        ne10_int32_t fstride = factors[1];
        ne10_int32_t mstride;
        ne10_int32_t cur_radix;
        ne10_float64_t phase;
        const ne10_float64_t pi = NE10_PI;

        // Don't generate any twiddles for the first stage
        stage_count --;

        // Generate twiddles for the other stages
        for (; stage_count > 0; stage_count --)
        {
            cur_radix = factors[2 * stage_count];
            fstride /= cur_radix;
            mstride = factors[2 * stage_count + 1];
            for (j = 0; j < mstride; j++)
            {
                for (k = 1; k < cur_radix; k++) // phase = 1 when k = 0
                {
                    phase = -2 * pi * fstride * k * j / fftLen;
                    twiddles[mstride * (k - 1) + j].r = (ne10_int16_t) floor (0.5 + NE10_F2I16_MAX * cos (phase));
                    twiddles[mstride * (k - 1) + j].i = (ne10_int16_t) floor (0.5 + NE10_F2I16_MAX * sin (phase));
                }
            }
            twiddles += mstride * (cur_radix - 1);
        }

        stage_count = factors[0];
        factors[2] = st->factors[2 * stage_count];        // first radix
        factors[3] = st->factors[2 * stage_count]; // mstride

        //printf("%d %d %d %d\n",factors[0],factors[1],factors[2],factors[3]);
    }

    

    return st;
}
/**
  @} end of ComplexFFTQ15 group
 */


#if ((defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)) || defined(DOXYGEN))

typedef void (*line_generator_float16)(ne10_fft_cpx_float16_t*,
      const ne10_int32_t,
      const ne10_int32_t,
      const ne10_int32_t,
      const ne10_int32_t);

// First column (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
static void ne10_fft_generate_twiddles_line_float16 (ne10_fft_cpx_float16_t * twiddles,
        const ne10_int32_t mstride,
        const ne10_int32_t fstride,
        const ne10_int32_t radix,
        const ne10_int32_t nfft)
{
    ne10_int32_t j, k;
    ne10_float64_t phase;
    const ne10_float64_t pi = NE10_PI;
    //printf("%d %d %d %d\n",mstride,fstride,radix,nfft);

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;
            twiddles[mstride * (k - 1) + j].r = (ne10_float16_t) cos ((double)phase);
            twiddles[mstride * (k - 1) + j].i = (ne10_float16_t) sin ((double)phase);
        } // radix
    } // mstride
}



static ne10_fft_cpx_float16_t* ne10_fft_generate_twiddles_impl_float16 (
      line_generator_float16 generator,
      ne10_fft_cpx_float16_t * twiddles,
      const ne10_uint32_t * factors,
      const ne10_int32_t nfft)
{
    ne10_uint32_t stage_count = factors[0];
    ne10_uint32_t fstride = factors[1];
    ne10_uint32_t mstride;
    ne10_uint32_t cur_radix; // current radix

    //printf("stage count = %d, fstride = %d\n",stage_count,fstride);

    // for first stage
    cur_radix = factors[2 * stage_count];
    //printf("cur radix = %d\n",cur_radix);

    if (cur_radix % 2) // current radix is not 4 or 2
    {
        //printf("Gen cur radix\n");
        twiddles[0].r = 1.0;
        twiddles[0].i = 0.0;
        twiddles += 1;
        generator (twiddles, 1, fstride, cur_radix, nfft);
        twiddles += cur_radix - 1;
    }
    stage_count --;

    // for other stage
    for (; stage_count > 0; stage_count --)
    {
        cur_radix = factors[2 * stage_count];
        //printf("state=%d, cur radix = %d\n",stage_count,cur_radix);
        fstride /= cur_radix;
        mstride = factors[2 * stage_count + 1];
        generator (twiddles, mstride, fstride, cur_radix, nfft);
        twiddles += mstride * (cur_radix - 1);
    } // stage_count

    return twiddles;
}

static ne10_fft_cpx_float16_t* ne10_fft_generate_twiddles_float16 (ne10_fft_cpx_float16_t * twiddles,
        const ne10_uint32_t * factors,
        const ne10_int32_t nfft )
{
    line_generator_float16 generator = ne10_fft_generate_twiddles_line_float16;
    twiddles = ne10_fft_generate_twiddles_impl_float16(generator,
        twiddles, factors, nfft);
    return twiddles;
}


// Transposed twiddles matrix [mstride][radix-1]
// First row (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
// Transposed twiddle tables are used in RFFT to avoid memory access by a large
// stride.
static void ne10_fft_generate_twiddles_line_transposed_float16 (
    ne10_fft_cpx_float16_t* twiddles,
    const ne10_int32_t mstride,
    const ne10_int32_t fstride,
    const ne10_int32_t radix,
    const ne10_int32_t nfft)
{
    //printf("Transposed\n");

    ne10_int32_t j, k;
    ne10_float64_t phase;
    const ne10_float64_t pi = NE10_PI;

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;
            twiddles[(radix - 1) * j + k - 1].r = (ne10_float16_t) cos ((double)phase);
            twiddles[(radix - 1) * j + k - 1].i = (ne10_float16_t) sin ((double)phase);
        } // radix
    } // mstride
}

static ne10_fft_cpx_float16_t* ne10_fft_generate_twiddles_transposed_float16 (
      ne10_fft_cpx_float16_t * twiddles,
      const ne10_uint32_t * factors,
      const ne10_int32_t nfft)
{
    //printf("Gen Transposed\n");
    line_generator_float16 generator =
        ne10_fft_generate_twiddles_line_transposed_float16;
    twiddles = ne10_fft_generate_twiddles_impl_float16(generator,
        twiddles, factors, nfft);
    return twiddles;
}



/**
  @addtogroup ComplexFFTF16
  @{
 */
/**
 * @brief      Initialize data structure for a FFT
 *
 * @param[in]  fftLen  The fft length
 *
 * @return     Pointer to the new structure
 * 
 * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with FFT lengths
 *            longer than the ones supported on Cortex-M
 *            
 *            With this function, FFT lengths can also contain
 *            factor 3 and 5 (but must be multiple of 4)
 */
arm_cfft_instance_f16 *arm_cfft_init_dynamic_f16(uint32_t fftLen)
{
    arm_cfft_instance_f16* st = NULL;
    ne10_uint32_t memneeded = sizeof (arm_cfft_instance_f16)
                              + sizeof (ne10_uint32_t) * (NE10_MAXFACTORS * 2) /* factors */
                              + sizeof (ne10_fft_cpx_float16_t) * fftLen       /* twiddles */
                              + NE10_FFT_BYTE_ALIGNMENT;             /* 64-bit alignment */

    st = (arm_cfft_instance_f16*) NE10_MALLOC (memneeded);

    // Bad allocation.
    if (st == NULL)
    {
        return NULL;
    }

    // Only backward FFT is scaled by default.
    //st->is_forward_scaled = 0;
    //st->is_backward_scaled = 1;

    uintptr_t address = (uintptr_t) st + sizeof (arm_cfft_instance_f16);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
    
    ne10_uint32_t *factors = (ne10_uint32_t*) address;

    st->factors = factors;
    ne10_fft_cpx_float16_t *pTwiddle =  (ne10_fft_cpx_float16_t*)(st->factors + (NE10_MAXFACTORS * 2));
    st -> pTwiddle = (const float16_t*)pTwiddle;
    st->last_twiddles = NULL;

    st->fftLen = fftLen;
    if (fftLen % NE10_FFT_PARA_LEVEL == 0)
    {
        //printf("Neon %d, st->nfft %d\n",NE10_FFT_PARA_LEVEL,st->nfft);
        // Size of FFT satisfies requirement of NEON optimization.
        st->fftLen /= NE10_FFT_PARA_LEVEL;
        st->last_twiddles = (const float16_t*)(pTwiddle + fftLen / NE10_FFT_PARA_LEVEL);
        //printf("st->nfft %d\n",st->nfft);

    }
        
    ne10_int16_t result = ne10_factor (st->fftLen, factors, NE10_FACTOR_EIGHT_FIRST_STAGE);

    // Cannot factor
    if (result == NE10_ERR)
    {
        NE10_FREE (st);
        return NULL;
    }

    ne10_int32_t stage_count    = st->factors[0];
    st->algorithm_flag = st->factors[2 * (stage_count + 1)];

    if (st->algorithm_flag == NE10_FFT_ALG_ANY)
    {
        if (fftLen % NE10_FFT_PARA_LEVEL)
        {
            NE10_FREE (st);
            return NULL;
        }

        ne10_fft_generate_twiddles_float16 (pTwiddle, st->factors, st->fftLen);

        // Generate super twiddles for the last stage.
        ne10_fft_generate_twiddles_line_float16 ((ne10_fft_cpx_float16_t *)st->last_twiddles,
                st->fftLen,
                1,
                NE10_FFT_PARA_LEVEL,
                fftLen);
        st->fftLen *= NE10_FFT_PARA_LEVEL;
    }
    else
    {
        if (fftLen % NE10_FFT_PARA_LEVEL == 0)
        {
            st->fftLen = fftLen;
    
            // Adjust the factoring for a size "nfft / 4" FFT to work for size "nfft"
            if (stage_count > NE10_MAXFACTORS - 4)
            {
                NE10_FREE (st);
                return NULL;
            }
            factors[0]++;          // Bump the stage count
            factors[1] *= 4;       // Quadruple the first stage stride
            memmove(&factors[4], &factors[2], ((2 * (stage_count + 1)) - 1) * sizeof(factors[0]));
            factors[2] = 4;        // Add a new radix-4 stage
            factors[3] = fftLen / 4;
        }
        else
        {
            NE10_FREE (st);
            return NULL;
        }
    
    
        ne10_fft_generate_twiddles_float16 (pTwiddle, factors, st->fftLen);
        
        factors[2] = st->factors[2 * (stage_count+1)];        // first radix
        factors[3] = st->factors[2 * (stage_count+1)]; // mstride
    }

    //printf("%d %d %d %d\n",factors[0],factors[1],factors[2],factors[3]);

    return st;
}
/**
  @} end of ComplexFFTF16 group
 */


/**
  @addtogroup RealFFTF16
  @{
 */
/**
 * @brief      Initialize data structure for a RFFT
 *
 * @param[in]  nfft  The rfft length
 *
 * @return     Pointer to the new structure
 * 
 * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with RFFT lengths
 *            longer than the ones supported on Cortex-M
 *            
 */
arm_rfft_fast_instance_f16 *arm_rfft_fast_init_dynamic_f16 (uint32_t nfft)
{
    arm_rfft_fast_instance_f16* st = NULL;
    ne10_int32_t result;

    ne10_uint32_t memneeded =   sizeof (arm_rfft_fast_instance_f16)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)       /* r_factors */
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)       /* r_factors_neon */
                              + sizeof (ne10_fft_cpx_float16_t) * nfft              /* r_twiddles */
                              + sizeof (ne10_fft_cpx_float16_t) * nfft/4            /* r_twiddles_neon */
                              + sizeof (ne10_fft_cpx_float16_t) * (12 + nfft/32*12) /* r_super_twiddles_neon */
                              + NE10_FFT_BYTE_ALIGNMENT;     /* 64-bit alignment*/

    st = (arm_rfft_fast_instance_f16*) NE10_MALLOC (memneeded);

    if (!st)
    {
        return st;
    }

    ne10_int32_t i,j;
    ne10_fft_cpx_float16_t *tw;
    const ne10_float64_t pi = NE10_PI;
    ne10_float64_t phase1;

    st->nfft = nfft;

    ne10_fft_cpx_float16_t *r_twiddles;
    uint32_t *r_factors;
    ne10_fft_cpx_float16_t *r_twiddles_backward;
    ne10_fft_cpx_float16_t *r_twiddles_neon;
    ne10_fft_cpx_float16_t *r_twiddles_neon_backward;
    uint32_t *r_factors_neon;
    ne10_fft_cpx_float16_t *r_super_twiddles_neon;

    
    uintptr_t address = (uintptr_t) st + sizeof (arm_rfft_fast_instance_f16);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);

    r_twiddles = (ne10_fft_cpx_float16_t*) address;
    r_factors = (ne10_uint32_t*) (r_twiddles + nfft);
    r_twiddles_neon = (ne10_fft_cpx_float16_t*) (r_factors + (NE10_MAXFACTORS * 2));
    r_factors_neon = (ne10_uint32_t*) (r_twiddles_neon + nfft/4);
    r_super_twiddles_neon = (ne10_fft_cpx_float16_t*) (r_factors_neon + (NE10_MAXFACTORS * 2));


    
    if (nfft<32)
    {
        return st;
    }

    // factors and twiddles for rfft C
    ne10_factor (nfft, r_factors, NE10_FACTOR_EIGHT_FIRST_STAGE);

    // backward twiddles pointers
    r_twiddles_backward = ne10_fft_generate_twiddles_float16 (r_twiddles, r_factors, nfft);

    //for(unsigned int i=0;i<nfft;i++)
    //{
    //    printf("%f %f\n",(double)r_twiddles[i].r,(double)r_twiddles[i].i);
    //}
    //printf("---\n");

    // factors and twiddles for rfft neon
    result = ne10_factor (nfft/4, r_factors_neon, NE10_FACTOR_EIGHT_FIRST_STAGE);
    if (result == NE10_ERR)
    {
        return st;
    }

    // Twiddle table is transposed here to improve cache access performance.
    r_twiddles_neon_backward = ne10_fft_generate_twiddles_transposed_float16 (
        r_twiddles_neon,
        r_factors_neon,
        nfft/4);

    //for(unsigned int i=0;i<nfft/4;i++)
    //{
    //    printf("%f %f\n",(double)r_twiddles_neon[i].r,(double)r_twiddles_neon[i].i);
    //}

    // nfft/4 x 4
    tw = r_super_twiddles_neon;
    for (i = 1; i < 4; i ++)
    {
        for (j = 0; j < 4; j++)
        {
            phase1 = - 2 * pi * ( (1.0*i * j) / nfft);
            tw[4*i-4+j].r = (ne10_float16_t) cos (phase1);
            tw[4*i-4+j].i = (ne10_float16_t) sin (phase1);
        }
    }

    ne10_uint32_t k,s;
    // [nfft/32] x [3] x [4]
    //     k        s     j
    for (k=1; k<nfft/32; k++)
    {
        // transposed
        for (s = 1; s < 4; s++)
        {
            for (j = 0; j < 4; j++)
            {
                phase1 = - 2 * pi * (  1.0*((k*4+j) * s) / nfft);
                //printf("%d %d %d %f\n",k,j,s,phase1);
                tw[12*k+j+4*(s-1)].r = (ne10_float16_t) cos (phase1);
                tw[12*k+j+4*(s-1)].i = (ne10_float16_t) sin (phase1);
            }
        }
    }

    //printf("---\n");
    //for(unsigned int i=0;i<nfft/32*12;i++)
    //{
    //    printf("%f %f\n",(double)r_super_twiddles_neon[i].r,(double)r_super_twiddles_neon[i].i);
    //}

    ne10_int32_t stage_count = r_factors_neon[0];
    //printf("stage %d\n",stage_count);
    r_factors_neon[2] = r_factors_neon[2 * (stage_count)];        // first radix
    if (stage_count > 1)
    {
      r_factors_neon[3] = r_factors_neon[2 * (stage_count-1)+1]; // mstride
    }
    else
    {
      r_factors_neon[3] = r_factors_neon[2 * (stage_count)+1]; // mstride
    }

    stage_count = r_factors[0];
    //printf("stage %d\n",stage_count);
    r_factors[2] = r_factors[2 * (stage_count)];        // first radix
    if (stage_count > 1)
    {
      r_factors[3] = r_factors[2 * (stage_count-1)+1]; // mstride
    }
    else
    {
      r_factors[3] = r_factors[2 * (stage_count)+1]; // mstride
    }
    
    st->r_twiddles=(float16_t*)r_twiddles;
    st->r_factors=r_factors;
    st->r_twiddles_backward=(float16_t*)r_twiddles_backward;
    st->r_twiddles_neon=(float16_t*)r_twiddles_neon;
    st->r_twiddles_neon_backward=(float16_t*)r_twiddles_neon_backward;
    st->r_factors_neon=r_factors_neon;
    st->r_super_twiddles_neon=(float16_t*)r_super_twiddles_neon;

    //printf("%d %d %d %d\n",r_factors[0],r_factors[1],r_factors[2],r_factors[3]);
    //printf("%d %d %d %d\n",r_factors_neon[0],r_factors_neon[1],r_factors_neon[2],r_factors_neon[3]);

    return st;
}
/**
  @} end of RealFFTF16 group
 */

#endif

/**
  @addtogroup RealFFTF32
  @{
 */
/**
 * @brief      Initialize data structure for a RFFT
 *
 * @param[in]  nfft  The rfft length
 *
 * @return     Pointer to the new structure
 * 
 * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with RFFT lengths
 *            longer than the ones supported on Cortex-M
 *            
 */
arm_rfft_fast_instance_f32 *arm_rfft_fast_init_dynamic_f32 (uint32_t nfft)
{
     //printf("Alloc r2c\n");
    arm_rfft_fast_instance_f32* st = NULL;
    ne10_int32_t result;

    ne10_uint32_t memneeded =   sizeof (arm_rfft_fast_instance_f32)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)       /* r_factors */
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)       /* r_factors_neon */
                              + sizeof (ne10_fft_cpx_float32_t) * nfft              /* r_twiddles */
                              + sizeof (ne10_fft_cpx_float32_t) * nfft/4            /* r_twiddles_neon */
                              + sizeof (ne10_fft_cpx_float32_t) * (12 + nfft/32*12) /* r_super_twiddles_neon */
                              + NE10_FFT_BYTE_ALIGNMENT;     /* 64-bit alignment*/

    st = (arm_rfft_fast_instance_f32*) NE10_MALLOC (memneeded);

    if (!st)
    {
        return st;
    }

    ne10_int32_t i,j;
    ne10_fft_cpx_float32_t *tw;
    const ne10_float64_t pi = NE10_PI;
    ne10_float64_t phase1;

    st->nfft = nfft;

    ne10_fft_cpx_float32_t *r_twiddles;
    uint32_t *r_factors;
    ne10_fft_cpx_float32_t *r_twiddles_backward;
    ne10_fft_cpx_float32_t *r_twiddles_neon;
    ne10_fft_cpx_float32_t *r_twiddles_neon_backward;
    uint32_t *r_factors_neon;
    ne10_fft_cpx_float32_t *r_super_twiddles_neon;

    
    uintptr_t address = (uintptr_t) st + sizeof (arm_rfft_fast_instance_f32);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);

    r_twiddles = (ne10_fft_cpx_float32_t*) address;
    r_factors = (ne10_uint32_t*) (r_twiddles + nfft);
    r_twiddles_neon = (ne10_fft_cpx_float32_t*) (r_factors + (NE10_MAXFACTORS * 2));
    r_factors_neon = (ne10_uint32_t*) (r_twiddles_neon + nfft/4);
    r_super_twiddles_neon = (ne10_fft_cpx_float32_t*) (r_factors_neon + (NE10_MAXFACTORS * 2));


    
    if (nfft<32)
    {
        return st;
    }

    // factors and twiddles for rfft C
    ne10_factor (nfft, r_factors, NE10_FACTOR_EIGHT_FIRST_STAGE);

    // backward twiddles pointers
    r_twiddles_backward = ne10_fft_generate_twiddles_float32 (r_twiddles, r_factors, nfft);

    //for(unsigned int i=0;i<nfft;i++)
    //{
    //    printf("%f %f\n",(double)r_twiddles[i].r,(double)r_twiddles[i].i);
    //}
    //printf("---\n");

    // factors and twiddles for rfft neon
    result = ne10_factor (nfft/4, r_factors_neon, NE10_FACTOR_EIGHT_FIRST_STAGE);
    if (result == NE10_ERR)
    {
        return st;
    }

    // Twiddle table is transposed here to improve cache access performance.
    r_twiddles_neon_backward = ne10_fft_generate_twiddles_transposed_float32 (
        r_twiddles_neon,
        r_factors_neon,
        nfft/4);

    //for(unsigned int i=0;i<nfft/4;i++)
    //{
    //    printf("%f %f\n",(double)r_twiddles_neon[i].r,(double)r_twiddles_neon[i].i);
    //}

    // nfft/4 x 4
    tw = r_super_twiddles_neon;
    for (i = 1; i < 4; i ++)
    {
        for (j = 0; j < 4; j++)
        {
            phase1 = - 2 * pi * ( (1.0*i * j) / nfft);
            tw[4*i-4+j].r = (ne10_float32_t) cos (phase1);
            tw[4*i-4+j].i = (ne10_float32_t) sin (phase1);
        }
    }

    ne10_uint32_t k,s;
    // [nfft/32] x [3] x [4]
    //     k        s     j
    for (k=1; k<nfft/32; k++)
    {
        // transposed
        for (s = 1; s < 4; s++)
        {
            for (j = 0; j < 4; j++)
            {
                phase1 = - 2 * pi * (  1.0*((k*4+j) * s) / nfft);
                //printf("%d %d %d %f\n",k,j,s,phase1);
                tw[12*k+j+4*(s-1)].r = (ne10_float32_t) cos (phase1);
                tw[12*k+j+4*(s-1)].i = (ne10_float32_t) sin (phase1);
            }
        }
    }

    //printf("---\n");
    //for(unsigned int i=0;i<nfft/32*12;i++)
    //{
    //    printf("%f %f\n",(double)r_super_twiddles_neon[i].r,(double)r_super_twiddles_neon[i].i);
    //}

    ne10_int32_t stage_count = r_factors_neon[0];
    //printf("stage %d\n",stage_count);
    r_factors_neon[2] = r_factors_neon[2 * (stage_count)];        // first radix
    if (stage_count > 1)
    {
      r_factors_neon[3] = r_factors_neon[2 * (stage_count-1)+1]; // mstride
    }
    else
    {
      r_factors_neon[3] = r_factors_neon[2 * (stage_count)+1]; // mstride
    }

    stage_count = r_factors[0];
    //printf("stage %d\n",stage_count);
    r_factors[2] = r_factors[2 * (stage_count)];        // first radix
    if (stage_count > 1)
    {
      r_factors[3] = r_factors[2 * (stage_count-1)+1]; // mstride
    }
    else
    {
      r_factors[3] = r_factors[2 * (stage_count)+1]; // mstride
    }
    
    st->r_twiddles=(float32_t*)r_twiddles;
    st->r_factors=r_factors;
    st->r_twiddles_backward=(float32_t*)r_twiddles_backward;
    st->r_twiddles_neon=(float32_t*)r_twiddles_neon;
    st->r_twiddles_neon_backward=(float32_t*)r_twiddles_neon_backward;
    st->r_factors_neon=r_factors_neon;
    st->r_super_twiddles_neon=(float32_t*)r_super_twiddles_neon;

    //printf("%d %d %d %d\n",r_factors[0],r_factors[1],r_factors[2],r_factors[3]);
    //printf("%d %d %d %d\n",r_factors_neon[0],r_factors_neon[1],r_factors_neon[2],r_factors_neon[3]);

    return st;
}
/**
  @} end of RealFFTF32 group
 */

/**
  @addtogroup RealFFTQ31
  @{
 */
/**
 * @brief      Initialize RFFT Q31 with memory allocation
 *
 * @param[in]  nfft  The nfft
 *
 * @return     Pointer to RFFT instance Q31
 * 
 * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with RFFT lengths
 *            longer than the ones supported on Cortex-M
 *            
 */
  arm_rfft_instance_q31 *arm_rfft_init_dynamic_q31(uint32_t nfft)
  {
    arm_rfft_instance_q31 *st = NULL;
    ne10_int32_t ncfft = nfft >> 1;

    ne10_uint32_t memneeded = sizeof (arm_rfft_instance_q31)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)    /* factors */
                              + sizeof (ne10_fft_cpx_int32_t) * ncfft           /* twiddles */
                              + sizeof (ne10_fft_cpx_int32_t) * ncfft / 2 /* super twiddles */
                              + NE10_FFT_BYTE_ALIGNMENT;                /* 64-bit alignment */

    st = (arm_rfft_instance_q31*) NE10_MALLOC (memneeded);

    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (arm_rfft_instance_q31);
        NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
        ne10_uint32_t* factors = (ne10_uint32_t*) address;
        ne10_fft_cpx_int32_t *twiddles = (ne10_fft_cpx_int32_t*) (factors + (NE10_MAXFACTORS * 2));
        ne10_fft_cpx_int32_t* super_twiddles = twiddles + ncfft;

        st->nfft = nfft;
        st->ncfft = ncfft;
        st->factors= factors;
        st->twiddles = (q31_t*)twiddles;
        st->super_twiddles=(q31_t*)super_twiddles;

        ne10_int32_t result = ne10_factor (ncfft, factors, NE10_FACTOR_EIGHT_FIRST_STAGE);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return NULL;
        }

        ne10_int32_t j, k;
        ne10_int32_t stage_count = factors[0];
        ne10_int32_t fstride = factors[1];
        ne10_int32_t mstride;
        ne10_int32_t cur_radix;
        ne10_float64_t phase;
        const ne10_float64_t pi = NE10_PI;

        // Don't generate any twiddles for the first stage
        stage_count --;

        // Generate twiddles for the other stages
        for (; stage_count > 0; stage_count --)
        {
            cur_radix = factors[2 * stage_count];
            fstride /= cur_radix;
            mstride = factors[2 * stage_count + 1];
            for (j = 0; j < mstride; j++)
            {
                for (k = 1; k < cur_radix; k++) // phase = 1 when k = 0
                {
                    phase = -2 * pi * fstride * k * j / ncfft;
                    twiddles[mstride * (k - 1) + j].r = (ne10_int32_t) floor (0.5 + NE10_F2I32_MAX * cos (phase));
                    twiddles[mstride * (k - 1) + j].i = (ne10_int32_t) floor (0.5 + NE10_F2I32_MAX * sin (phase));
                }
            }
            twiddles += mstride * (cur_radix - 1);
        }

        twiddles = (ne10_fft_cpx_int32_t*)st->super_twiddles;
        for (j = 0; j < ncfft / 2; j++)
        {
            phase = -pi * ( (ne10_float64_t) (j + 1) / ncfft + 0.5);
            twiddles->r = (ne10_int32_t) floor (0.5 + NE10_F2I32_MAX * cos (phase));
            twiddles->i = (ne10_int32_t) floor (0.5 + NE10_F2I32_MAX * sin (phase));
            twiddles++;
        }

        stage_count = factors[0];
        factors[2] = factors[2 * (stage_count)]; // mstride
        if (stage_count > 1)
        {
          factors[3] = factors[2 * (stage_count-1)+1]; // mstride
        }
        else
        {
          factors[3] = factors[2 * (stage_count)+1]; // mstride
        }
       
        //printf("%d: %d %d %d %d\n",stage_count,factors[0],factors[1],factors[2],factors[3]);

    }

    return st;
  }
  /**
  @} end of RealFFTQ31 group
 */

  /**
  @addtogroup RealFFTQ15
  @{
 */

  /**
   * @brief      Initialize RFFT Q15 with memory allocation
   *
   * @param[in]  nfft  The nfft
   *
   * @return     Pointer to an RFFT Instance Q15
   * 
   * @par       This function is only available for Neon
 *            This function is allocating memory. The
 *            memory must be released when no more used.
 *            This function can be used with RFFT lengths
 *            longer than the ones supported on Cortex-M
   *            
   */
  arm_rfft_instance_q15 *arm_rfft_init_dynamic_q15(uint32_t nfft)
  {
    arm_rfft_instance_q15* st = NULL;
    ne10_int32_t ncfft = nfft >> 1;

    ne10_uint32_t memneeded = sizeof (arm_rfft_instance_q15)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)    /* factors */
                              + sizeof (ne10_fft_cpx_int16_t) * ncfft           /* twiddles */
                              + sizeof (ne10_fft_cpx_int16_t) * ncfft / 2 /* super twiddles */
                              + NE10_FFT_BYTE_ALIGNMENT;                /* 64-bit alignment */

    st = (arm_rfft_instance_q15*) NE10_MALLOC (memneeded);

    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (arm_rfft_instance_q15);
        NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
        ne10_uint32_t* factors = (ne10_uint32_t*) address;
        ne10_fft_cpx_int16_t* twiddles = (ne10_fft_cpx_int16_t*) (factors + (NE10_MAXFACTORS * 2));
        ne10_fft_cpx_int16_t* super_twiddles = twiddles + ncfft;
        
        st->nfft = nfft;
        st->ncfft = ncfft;
        st->factors= factors;
        st->twiddles = (q15_t*)twiddles;
        st->super_twiddles=(q15_t*)super_twiddles;

        ne10_int32_t result = ne10_factor (ncfft, factors, NE10_FACTOR_EIGHT_FIRST_STAGE);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return NULL;
        }

        ne10_int32_t j, k;
        ne10_int32_t stage_count = factors[0];
        ne10_int32_t fstride = factors[1];
        ne10_int32_t mstride;
        ne10_int32_t cur_radix;
        ne10_float64_t phase;
        const ne10_float64_t pi = NE10_PI;

        // Don't generate any twiddles for the first stage
        stage_count --;

        // Generate twiddles for the other stages
        for (; stage_count > 0; stage_count --)
        {
            cur_radix = factors[2 * stage_count];
            fstride /= cur_radix;
            mstride = factors[2 * stage_count + 1];
            for (j = 0; j < mstride; j++)
            {
                for (k = 1; k < cur_radix; k++) // phase = 1 when k = 0
                {
                    phase = -2 * pi * fstride * k * j / ncfft;
                    twiddles[mstride * (k - 1) + j].r = (ne10_int16_t) floor (0.5 + NE10_F2I16_MAX * cos (phase));
                    twiddles[mstride * (k - 1) + j].i = (ne10_int16_t) floor (0.5 + NE10_F2I16_MAX * sin (phase));
                }
            }
            twiddles += mstride * (cur_radix - 1);
        }

        twiddles = (ne10_fft_cpx_int16_t*)st->super_twiddles;
        for (j = 0; j < ncfft / 2; j++)
        {
            phase = -pi * ( (ne10_float64_t) (j + 1) / ncfft + 0.5);
            twiddles->r = (ne10_int16_t) floor (0.5 + NE10_F2I16_MAX * cos (phase));
            twiddles->i = (ne10_int16_t) floor (0.5 + NE10_F2I16_MAX * sin (phase));
            twiddles++;
        }

        stage_count = factors[0];
        factors[2] = factors[2 * (stage_count)]; // mstride
        if (stage_count > 1)
        {
          factors[3] = factors[2 * (stage_count-1)+1]; // mstride
        }
        else
        {
          factors[3] = factors[2 * (stage_count)+1]; // mstride
        }
    }

    return st;
  }
  /**
  @} end of RealFFTQ15 group
 */