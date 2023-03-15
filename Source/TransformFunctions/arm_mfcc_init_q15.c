/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mfcc_init_q15.c
 * Description:  MFCC initialization function for the q15 version
 *
 * $Date:        07 September 2021
 * $Revision:    V1.10.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @defgroup MFCCQ15 MFCC Q15
 */


/**
  @ingroup MFCC
 */

/**
  @addtogroup MFCCQ15
  @{
 */


#include "dsp/transform_functions.h"



/**
  @brief         Generic initialization of the MFCC Q15 instance structure
  @param[out]    S       points to the mfcc instance structure
  @param[in]     fftLen  fft length
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 
  @par          
                This function should be used only if you don't know the FFT sizes that 
                you'll need at build time. The use of this function will prevent the 
                linker from removing the FFT tables that are not needed and the library 
                code size will be bigger than needed.

  @par          
                If you use CMSIS-DSP as a static library, and if you know the MFCC sizes 
                that you need at build time, then it is better to use the initialization
                functions defined for each MFCC size.


 */
arm_status arm_mfcc_init_q15(
  arm_mfcc_instance_q15 * S,
  uint32_t fftLen,
  uint32_t nbMelFilters,
  uint32_t nbDctOutputs,
  const q15_t *dctCoefs,
  const uint32_t *filterPos,
  const uint32_t *filterLengths,
  const q15_t *filterCoefs,
  const q15_t *windowCoefs
  )
{
 arm_status status;

 S->fftLen=fftLen;
 S->nbMelFilters=nbMelFilters;
 S->nbDctOutputs=nbDctOutputs;
 S->dctCoefs=dctCoefs;
 S->filterPos=filterPos;
 S->filterLengths=filterLengths;
 S->filterCoefs=filterCoefs;
 S->windowCoefs=windowCoefs;

 #if defined(ARM_MFCC_CFFT_BASED)
 status=arm_cfft_init_q15(&(S->cfft),fftLen);
 #else
 status=arm_rfft_init_q15(&(S->rfft),fftLen,0,1);
 #endif
 
 return(status);
}

#if defined(ARM_MFCC_CFFT_BASED)
#define MFCC_INIT_Q15(LEN)                    \
arm_status arm_mfcc_init_##LEN##_q15(         \
  arm_mfcc_instance_q15 * S,                  \
  uint32_t nbMelFilters,                      \
  uint32_t nbDctOutputs,                      \
  const q15_t *dctCoefs,                      \
  const uint32_t *filterPos,                  \
  const uint32_t *filterLengths,              \
  const q15_t *filterCoefs,                   \
  const q15_t *windowCoefs                    \
  )                                           \
{                                             \
 arm_status status;                           \
                                              \
 S->fftLen=LEN;                               \
 S->nbMelFilters=nbMelFilters;                \
 S->nbDctOutputs=nbDctOutputs;                \
 S->dctCoefs=dctCoefs;                        \
 S->filterPos=filterPos;                      \
 S->filterLengths=filterLengths;              \
 S->filterCoefs=filterCoefs;                  \
 S->windowCoefs=windowCoefs;                  \
                                              \
 status=arm_cfft_init_##LEN##_q15(&(S->cfft));\
                                              \
 return(status);                              \
}
#else
#define MFCC_INIT_Q15(LEN)                        \
arm_status arm_mfcc_init_##LEN##_q15(             \
  arm_mfcc_instance_q15 * S,                      \
  uint32_t nbMelFilters,                          \
  uint32_t nbDctOutputs,                          \
  const q15_t *dctCoefs,                          \
  const uint32_t *filterPos,                      \
  const uint32_t *filterLengths,                  \
  const q15_t *filterCoefs,                       \
  const q15_t *windowCoefs                        \
  )                                               \
{                                                 \
 arm_status status;                               \
                                                  \
 S->fftLen=LEN;                                   \
 S->nbMelFilters=nbMelFilters;                    \
 S->nbDctOutputs=nbDctOutputs;                    \
 S->dctCoefs=dctCoefs;                            \
 S->filterPos=filterPos;                          \
 S->filterLengths=filterLengths;                  \
 S->filterCoefs=filterCoefs;                      \
 S->windowCoefs=windowCoefs;                      \
                                                  \
 status=arm_rfft_init_##LEN##_q15(&(S->rfft),0,1);\
                                                  \
 return(status);                                  \
}
#endif

/**
  @brief         Initialization of the MFCC Q15 instance structure for 32 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(32);

/**
  @brief         Initialization of the MFCC Q15 instance structure for 64 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(64);

/**
  @brief         Initialization of the MFCC Q15 instance structure for 128 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(128);

/**
  @brief         Initialization of the MFCC Q15 instance structure for 256 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(256);

/**
  @brief         Initialization of the MFCC Q15 instance structure for 512 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(512);

/**
  @brief         Initialization of the MFCC Q15 instance structure for 1024 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(1024);

/**
  @brief         Initialization of the MFCC Q15 instance structure for 2048 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(2048);

/**
  @brief         Initialization of the MFCC Q15 instance structure for 4096 samples MFCC
  @param[out]    S       points to the mfcc instance structure
  @param[in]     nbMelFilters  number of Mel filters
  @param[in]     nbDctOutputs  number of Dct outputs
  @param[in]     dctCoefs  points to an array of DCT coefficients
  @param[in]     filterPos  points of the array of filter positions
  @param[in]     filterLengths  points to the array of filter lengths
  @param[in]     filterCoefs  points to the array of filter coefficients
  @param[in]     windowCoefs  points to the array of window coefficients

  @return        error status

  @par           Description
                   The matrix of Mel filter coefficients is sparse.
                   Most of the coefficients are zero.
                   To avoid multiplying the spectrogram by those zeros, the
                   filter is applied only to a given position in the spectrogram
                   and on a given number of FFT bins (the filter length).
                   It is the reason for the arrays filterPos and filterLengths.

                   window coefficients can describe (for instance) a Hamming window.
                   The array has the same size as the FFT length.

                   The folder Scripts is containing a Python script which can be used
                   to generate the filter, dct and window arrays.
 */
MFCC_INIT_Q15(4096);

/**
  @} end of MFCCQ15 group
 */
