extern "C" {
    extern void filter_test();
}

#include "allocator.h"

#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>
#include <dsppp/unroll.hpp>

#include <iostream>

#include <cmsis_tests.h>


#if defined(ARM_MATH_MVEI) || defined(ARM_MATH_MVEF)

#define MVE_ASRL_SAT16(acc, shift)          ((sqrshrl_sat48(acc, -(32-shift)) >> 32) & 0xffffffff)


#define FIR_Q15_CORE(pOutput, nbAcc, nbVecTaps, pSample, vecCoeffs)        \
        for (int j = 0; j < nbAcc; j++) {                                  \
            const q15_t    *pSmp = &pSample[j];                            \
            q63_t           acc[4];                                        \
                                                                           \
            acc[j] = 0;                                                    \
            for (int i = 0; i < nbVecTaps; i++) {                          \
                vecIn0 = vld1q(pSmp + 8 * i);                  \
                acc[j] = vmlaldavaq(acc[j], vecIn0, vecCoeffs[i]);         \
            }                                                              \
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc[j], 15);               \
        }

#define FIR_Q15_MAIN_CORE()                                                                  \
{                                                                                            \
    q15_t          *pState = S->pState;     /* State pointer */                              \
    const q15_t    *pCoeffs = S->pCoeffs;   /* Coefficient pointer */                        \
    q15_t          *pStateCur;              /* Points to the current sample of the state */  \
    const q15_t    *pSamples;               /* Temporary pointer to the sample buffer */     \
    q15_t          *pOutput;                /* Temporary pointer to the output buffer */     \
    const q15_t    *pTempSrc;               /* Temporary pointer to the source data */       \
    q15_t          *pTempDest;              /* Temporary pointer to the destination buffer */\
    uint32_t        numTaps = S->numTaps;   /* Number of filter coefficients in the filter */\
    int32_t         blkCnt;                                                                  \
    q15x8_t         vecIn0;                                                                  \
                                                                                             \
    /*                                                                                       \
     * load coefs                                                                            \
     */                                                                                      \
    q15x8_t         vecCoeffs[NBVECTAPS];                                                    \
                                                                                             \
    for (int i = 0; i < NBVECTAPS; i++)                                                      \
        vecCoeffs[i] = vldrhq_s16(pCoeffs + 8 * i);                                          \
                                                                                             \
    /*                                                                                       \
     * pState points to state array which contains previous frame (numTaps - 1) samples      \
     * pStateCur points to the location where the new input data should be written           \
     */                                                                                      \
    pStateCur = &(pState[(numTaps - 1u)]);                                                   \
    pTempSrc = pSrc;                                                                         \
    pSamples = pState;                                                                       \
    pOutput = pDst;                                                                          \
                                                                                             \
    blkCnt = blockSize >> 2;                                                                 \
    while (blkCnt > 0) {                                                                     \
        /*                                                                                   \
         * Save 4 input samples in the history buffer                                        \
         */                                                                                  \
        vstrhq_s32(pStateCur, vldrhq_s32(pTempSrc));                                         \
        pStateCur += 4;                                                                      \
        pTempSrc += 4;                                                                       \
                                                                                             \
        FIR_Q15_CORE(pOutput, 4, NBVECTAPS, pSamples, vecCoeffs);                            \
        pSamples += 4;                                                                       \
                                                                                             \
        blkCnt--;                                                                            \
    }                                                                                        \
                                                                                             \
    /* tail */                                                                               \
    int32_t        residual = blockSize & 3;                                                \
                                                                                             \
    for (int i = 0; i < residual; i++)                                                       \
        *pStateCur++ = *pTempSrc++;                                                          \
                                                                                             \
    FIR_Q15_CORE(pOutput, residual, NBVECTAPS, pSamples, vecCoeffs);                         \
                                                                                             \
    /*                                                                                       \
     * Copy the samples back into the history buffer start                                   \
     */                                                                                      \
    pTempSrc = &pState[blockSize];                                                           \
    pTempDest = pState;                                                                      \
                                                                                             \
    /* current compiler limitation */                                                        \
    blkCnt = (numTaps - 1) >> 3;                                                             \
    while (blkCnt > 0)                                                                       \
    {                                                                                        \
        vstrhq_s16(pTempDest, vldrhq_s16(pTempSrc));                                         \
        pTempSrc += 8;                                                                       \
        pTempDest += 8;                                                                      \
        blkCnt--;                                                                            \
    }                                                                                        \
    blkCnt = (numTaps - 1) & 7;                                                              \
    if (blkCnt > 0)                                                                          \
    {                                                                                        \
        mve_pred16_t p = vctp16q(blkCnt);                                                    \
        vstrhq_p_s16(pTempDest, vldrhq_z_s16(pTempSrc, p), p);                               \
    }                                                                                        \
}
    
static void arm_fir_q15_25_32_mve(const arm_fir_instance_q15 * S, 
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 32
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}

static void arm_fir_q15_17_24_mve(const arm_fir_instance_q15 * S, 
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 24
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


static void arm_fir_q15_9_16_mve(const arm_fir_instance_q15 * S, 
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 16
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}

static void arm_fir_q15_1_8_mve(const arm_fir_instance_q15 * S, 
  const q15_t * __restrict pSrc, 
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 8
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


void debug_arm_fir_q15(
  const arm_fir_instance_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
    q15_t    *pState = S->pState;   /* State pointer */
    const q15_t    *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q15_t    *pStateCur;        /* Points to the current sample of the state */
    const q15_t    *pSamples;         /* Temporary pointer to the sample buffer */
    q15_t    *pOutput;          /* Temporary pointer to the output buffer */
    const q15_t    *pTempSrc;         /* Temporary pointer to the source data */
    q15_t    *pTempDest;        /* Temporary pointer to the destination buffer */
    uint32_t  numTaps = S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t  blkCnt;
    q15x8_t vecIn0;
    uint32_t  tapsBlkCnt = (numTaps + 7) / 8;
    q63_t     acc0, acc1, acc2, acc3;


int32_t nbTaps = (numTaps + 7) >> 3;

switch(nbTaps) {

    case 1:
        arm_fir_q15_1_8_mve(S, pSrc, pDst, blockSize);
        return;
    case 2:
        arm_fir_q15_9_16_mve(S, pSrc, pDst, blockSize);
        return;
    case 3:
        arm_fir_q15_17_24_mve(S, pSrc, pDst, blockSize);
        return;
    case 4:
        arm_fir_q15_25_32_mve(S, pSrc, pDst, blockSize);
        return;
    }
    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur   = &(pState[(numTaps - 1u)]);
    pTempSrc    = pSrc;
    pSamples    = pState;
    pOutput     = pDst;
    blkCnt      = blockSize >> 2;

    while (blkCnt > 0U)
    {
        const q15_t    *pCoeffsTmp = pCoeffs;
        const q15_t    *pSamplesTmp = pSamples;

        acc0 = 0LL;
        acc1 = 0LL;
        acc2 = 0LL;
        acc3 = 0LL;

        /*
         * Save 8 input samples in the history buffer
         */
        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 8;
        pTempSrc += 8;

        //INIT_SYSTICK;
        //START_CYCLE_MEASUREMENT;
        int       i = tapsBlkCnt;
        //startSectionNB(3);
        while (i > 0)
        {
            /*
             * load 8 coefs
             */
            q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

            vecIn0 = vld1q(pSamplesTmp);
            acc0 =  vmlaldavaq(acc0, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[1]);
            acc1 = vmlaldavaq(acc1, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[2]);
            acc2 = vmlaldavaq(acc2, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[3]);
            acc3 = vmlaldavaq(acc3, vecIn0, vecCoeffs);

            pSamplesTmp += 8;
            pCoeffsTmp += 8;
            /*
             * Decrement the taps block loop counter
             */
            i--;
        }
        //stopSectionNB(3);
        //STOP_CYCLE_MEASUREMENT;


        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc1, 15);
        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc2, 15);
        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc3, 15);

        pSamples += 4;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

    uint32_t  residual = blockSize & 3;
    switch (residual)
    {
    case 3:
        {
            const q15_t    *pCoeffsTmp = pCoeffs;
            const q15_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;
            acc1 = 0LL;
            acc2 = 0LL;

            /*
             * Save 8 input samples in the history buffer
             */
            *(q15x8_t *) pStateCur = *(q15x8_t *) pTempSrc;
            pStateCur += 8;
            pTempSrc += 8;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                /*
                 * load 8 coefs
                 */
                q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vmlaldavaq(acc0, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[2]);
                acc1 = vmlaldavaq(acc1, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[4]);
                acc2 = vmlaldavaq(acc2, vecIn0, vecCoeffs);

                pSamplesTmp += 8;
                pCoeffsTmp += 8;
                /*
                 * Decrement the taps block loop counter
                 */
                i--;
            }

            acc0 = asrl(acc0, 15);
            acc1 = asrl(acc1, 15);
            acc2 = asrl(acc2, 15);

            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc1, 15);
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc2, 15);
        }
        break;

    case 2:
        {
            const q15_t    *pCoeffsTmp = pCoeffs;
            const q15_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;
            acc1 = 0LL;
            /*
             * Save 8 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 8;
            pTempSrc += 8;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                /*
                 * load 8 coefs
                 */
                q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vmlaldavaq(acc0, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[2]);
                acc1 = vmlaldavaq(acc1, vecIn0, vecCoeffs);

                pSamplesTmp += 8;
                pCoeffsTmp += 8;
                /*
                 * Decrement the taps block loop counter
                 */
                i--;
            }

            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc1, 15);
        }
        break;

    case 1:
        {
            const q15_t    *pCoeffsTmp = pCoeffs;
            const q15_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;

            /*
             * Save 8 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 8;
            pTempSrc += 8;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                /*
                 * load 8 coefs
                 */
                q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vmlaldavaq(acc0, vecIn0, vecCoeffs);

                pSamplesTmp += 8;
                pCoeffsTmp += 8;
                /*
                 * Decrement the taps block loop counter
                 */
                i--;
            }

            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
        }
        break;

    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = numTaps >> 3;
    while (blkCnt > 0U)
    {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 8;
        pTempDest += 8;
        blkCnt--;
    }
    blkCnt = numTaps & 7;
    if (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp16q(blkCnt);
        vstrhq_p_s16(pTempDest, vld1q(pTempSrc), p0);
    }
}
#endif 

template<typename T>
struct FirType;

template<>
struct FirType<float32_t>
{
   typedef arm_fir_instance_f32 type;
   static void init_state(type * S,
                          uint16_t numTaps,
                          const float32_t * pCoeffs,
                                float32_t * pState,
                                uint32_t blockSize)
   {
       arm_fir_init_f32(S,numTaps,pCoeffs,pState,blockSize);
   };

   static void init_coef(float32_t *coefs,uint16_t numTaps)
   {
     for(int i=0;i<numTaps;i++)
     {
        coefs[i] = 1.0f / numTaps;
     }
   };
};

template<>
struct FirType<Q15>
{
   typedef arm_fir_instance_q15 type;
   static void init_state(type * S,
                          uint16_t numTaps,
                          const Q15 * pCoeffs,
                                Q15 * pState,
                                uint32_t blockSize)
   {
       arm_fir_init_q15(S,numTaps,
         reinterpret_cast<const q15_t*>(pCoeffs),
         reinterpret_cast<q15_t*>(pState),blockSize);
   };

   static void init_coef(Q15 *coefs,uint16_t numTaps)
   {
     for(int i=0;i<numTaps;i++)
     {
        coefs[i] = Q15::f(1.0f / numTaps);
     }
   };
};



template<typename T,int BLOCK,int TAPS>
struct FIR {

   FIR(const PVector<T,TAPS> &coefs):coef_(coefs),state_(T{})
   {};

   
   PVector<T,BLOCK> filter(const PVector<T,BLOCK> &signal)
   {
      constexpr int UNROLL_FACTOR = 4;
      PVector<T,BLOCK> res(T{});
      using acc_type = typename number_traits<T>::accumulator;
      std::array<acc_type,UNROLL_FACTOR> accu;
      index_t i=0;

#if defined(ARM_COMPUTE_DISABLE_UNROLL)
      #pragma clang loop unroll(disable)
#endif
      for(;i<=BLOCK-UNROLL_FACTOR;i+=UNROLL_FACTOR)
      {
         
         state_.sub(TAPS-1+i,TAPS-1+i+UNROLL_FACTOR) = copy(signal.sub(i,i+UNROLL_FACTOR));
        
         //INIT_SYSTICK;
         //START_CYCLE_MEASUREMENT;
         //startSectionNB(2);
         results(accu) =
           dot(unroll<UNROLL_FACTOR>(
                  [i,this](index_t k){return state_.sub(i+k,i+k+TAPS);}),
               replicate<UNROLL_FACTOR>(coef_)
              );
         //stopSectionNB(2);
         //STOP_CYCLE_MEASUREMENT;

         for(index_t k=0;k<UNROLL_FACTOR;k++)
         {
            res[i+k] = inner::from_accumulator(accu[k]);
         }
      }

#if defined(ARM_COMPUTE_DISABLE_UNROLL)
      #pragma clang loop unroll(disable)
#endif
      for(;i<BLOCK;i++)
      {
         state_[TAPS-1+i] = signal[i];
         acc_type acc = dot(state_.sub(i,i+TAPS),coef_);
         res[i] = inner::from_accumulator(acc);
      }

      state_.sub(0,TAPS) = copy(state_.sub(BLOCK,TAPS+BLOCK));
      return(res);
   }

   void purec(const T *signal, T *dst)
   {
      const T* coef=coef_.const_ptr();
      T *state = state_.ptr();

#if defined(ARM_COMPUTE_DISABLE_UNROLL)
      #pragma clang loop unroll(disable)
#endif
      for(index_t i=0;i<BLOCK;i++)
      {
         T acc=T{};
         state[TAPS-1+i] = signal[i];
         for(index_t k=0;k<TAPS;k++)
         {
            acc += state[i+k]*coef[k];
         }
         dst[i] = acc;
      }
      for(index_t i=0;i<TAPS-1;i++)
      {
         state[i] = state[BLOCK+i];
      }
   }

   const PVector<T,TAPS> coef_;
   PVector<T,TAPS+BLOCK-1> state_;
};

template<typename T,int BLOCK,int TAPS>
static void test()
{
   constexpr int NB = BLOCK;
   std::cout << "----\r\n(" << BLOCK << "," << TAPS << ")\r\n";

   typename FirType<T>::type S;
   PVector<T,NB> signal;
   PVector<T,TAPS> coefs;
   
   FirType<T>::init_coef(coefs.ptr(),TAPS);

   init_array(signal,NB);
   FIR<T,BLOCK,TAPS> fir(coefs);


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   PVector<T,BLOCK> res = fir.filter(signal);
   //PVector<T,BLOCK> res;
   //fir.purec(signal.const_ptr(),res.ptr());
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;


   T* state;
   T* coefsb;
   state=(T*)malloc(sizeof(T)*(TAPS+BLOCK+BLOCK));
   coefsb=(T*)malloc(sizeof(T)*(TAPS+32));
   memset(coefsb,0,sizeof(T)*(TAPS+32));
   for(int i =0;i<TAPS;i++)
   {
      coefsb[i] = coefs[i];
   }
   FirType<T>::init_state(&S,TAPS,coefsb,state,BLOCK);
   PVector<T,NB> ref;
   //std::cout << "---\r\n";


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   arm_fir_q15(&S,
                    reinterpret_cast<const q15_t*>(signal.const_ptr()),
                    reinterpret_cast<q15_t*>(ref.ptr()),BLOCK);
   STOP_CYCLE_MEASUREMENT;


   if (!validate(res.const_ptr(),ref.const_ptr(),BLOCK))
   {
      printf("fir failed \r\n");
   }

   free(state);
   free(coefsb);

   
}




template<typename T>
void all_filter_test()
{

    title<T>("FIR test");
   
   
    test<T,NBVEC_4,8>();
    test<T,NBVEC_4,16>();
    test<T,NBVEC_4,24>();
    test<T,NBVEC_4,32>();
    test<T,NBVEC_4,64>();

    test<T,NBVEC_32,8>();
    test<T,NBVEC_32,16>();
    test<T,NBVEC_32,24>();
    test<T,NBVEC_32,32>();
    test<T,NBVEC_32,64>();

    test<T,NBVEC_256,8>();
    test<T,NBVEC_256,16>();
    test<T,NBVEC_256,24>();
    test<T,NBVEC_256,32>();
    test<T,NBVEC_256,64>();
    

    test<T,NBVEC_512,8>();
    test<T,NBVEC_512,16>();
    test<T,NBVEC_512,24>();
    test<T,NBVEC_512,32>();
    test<T,NBVEC_512,64>();

   
}

void filter_test()
{
    //all_filter_test<Q15>();
}