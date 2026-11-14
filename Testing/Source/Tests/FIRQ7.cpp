#include "FIRQ7.h"
#include <stdio.h>
#include "Error.h"

#define SNR_THRESHOLD 10

#define ABS_ERROR_Q7 ((q7_t)2)

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
static __ALIGNED(8) q7_t coeffArray[32];
#endif 

static void checkInnerTail(q7_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
    ASSERT_TRUE(b[2] == 0);
    ASSERT_TRUE(b[3] == 0);
}

// Coef must be padded to a multiple of 16
#define FIRCOEFPADDING 4

    void FIRQ7::test_fir_q7()
    {
        

        const int16_t *configp = configs.ptr();
        q7_t *statep = state.ptr();
        const q7_t *orgcoefsp = coefs.ptr();

        const q7_t *coefsp;
        const q7_t *inputp = inputs.ptr();
        q7_t *outp = output.ptr();

        unsigned long i;
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
        int j;
        int round;
#endif
        int blockSize;
        int numTaps;
        

        /*

        Python script is generating different tests with
        different blockSize and numTaps.

        We loop on those configs.

        */
        for(i=0; i < configs.nbSamples() >> 1; i++)
        {
           blockSize = configp[0];
           numTaps = configp[1];

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
            /* Copy coefficients and pad to zero 
           */
           memset(coeffArray,127,32*sizeof(q7_t));
           round = numTaps >> FIRCOEFPADDING;
           if ((round << FIRCOEFPADDING) < numTaps)
           {
             round ++;
           }
           round = round<<FIRCOEFPADDING;
           memset(coeffArray,0,round*sizeof(q7_t));

           //printf("blockSize=%d, numTaps=%d, round=%d (%d)\n",blockSize,numTaps,round,round - numTaps);


           for(j=0;j < numTaps; j++)
           {
              coeffArray[j] = orgcoefsp[j];
           }
   
           coefsp = coeffArray;
#else
           coefsp = orgcoefsp;
#endif

           /*

           The filter is initialized with the coefs, blockSize and numTaps.

           */
           arm_fir_init_q7(&this->S,numTaps,coefsp,statep,blockSize);

           /*

           Input pointer is reset since the same input pattern is used

           */
           inputp = inputs.ptr();

           /*
           
           Python script is filtering a 2*blockSize number of samples.
           We do the same filtering in two pass to check (indirectly that
           the state management of the fir is working.)

           */
           arm_fir_q7(&this->S,inputp,outp,blockSize);
           outp += blockSize;
           checkInnerTail(outp);

           inputp += blockSize;
           arm_fir_q7(&this->S,inputp,outp,blockSize);
           outp += blockSize;
           checkInnerTail(outp);

           configp += 2;
           orgcoefsp += numTaps;

        }

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(q7_t)SNR_THRESHOLD);

        ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q7);

    } 

 
/*

Filters with a number of taps above the limit of the specialized Helium
kernels. The references are computed with an exact model of the fixed
point FIR so the comparison is bit exact.
Each configuration is processed with LONG_NBCALLS successive calls with
the same blockSize, and guard samples placed just after the documented
size of the state buffer check that it is not overrun.

*/
#define LONG_NBCALLS 3
#define LONG_MAX_TAPS 200
#define LONG_MAX_BLOCKSIZE 16
/* The guard must be larger than the overrun of the buggy Helium kernels
   (up to 51 samples for Q7 with blockSize 13) to stay inside the state buffer */
#define STATE_GUARD 64
#define STATE_GUARD_VALUE ((q7_t)0x5A)

// Coefficient array padded with zeros to a multiple of 16 (Helium)
static __ALIGNED(8) q7_t longCoeffArray[LONG_MAX_TAPS + 16];

    void FIRQ7::test_fir_long_q7()
    {
        const int16_t *configp = configs.ptr();
        q7_t *statep = state.ptr();
        const q7_t *orgcoefsp = coefs.ptr();
        const q7_t *inputp;
        q7_t *outp = output.ptr();
        unsigned long i;
        int k;
        unsigned long j;
        int blockSize;
        int numTaps;
        int stateSize;
        int guardErrors = 0;

        for(i=0; i < configs.nbSamples(); i += 2)
        {
           blockSize = configp[0];
           numTaps = configp[1];

           /* Documented size of the state buffer */
           stateSize = numTaps + blockSize - 1;

           memset(longCoeffArray,0,sizeof(longCoeffArray));
           memcpy(longCoeffArray,orgcoefsp,numTaps*sizeof(q7_t));

           for(j=0; j < STATE_GUARD; j++)
           {
              statep[stateSize + j] = STATE_GUARD_VALUE;
           }

           arm_fir_init_q7(&this->S,numTaps,longCoeffArray,statep,blockSize);

           inputp = inputs.ptr();

           for(k=0; k < LONG_NBCALLS; k++)
           {
              arm_fir_q7(&this->S,inputp,outp,blockSize);
              outp += blockSize;
              inputp += blockSize;
              checkInnerTail(outp);

              for(j=0; j < STATE_GUARD; j++)
              {
                 if (statep[stateSize + j] != STATE_GUARD_VALUE)
                 {
                    guardErrors++;
                    break;
                 }
              }
           }

           configp += 2;
           orgcoefsp += numTaps;
        }

        ASSERT_EMPTY_TAIL(output);

        ASSERT_EQ(output,ref);

        /* No write beyond the documented size of the state buffer */
        ASSERT_TRUE(guardErrors == 0);

    }

    void FIRQ7::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
      
       (void)params;

       switch(id)
       {
        case FIRQ7::TEST_FIR_LONG_Q7_2:
          inputs.reload(FIRQ7::FIRINPUTS2_Q7_ID,mgr);
          coefs.reload(FIRQ7::FIRCOEFS2_Q7_ID,mgr);
          configs.reload(FIRQ7::FIRCONFIGS2_S16_ID,mgr);
          ref.reload(FIRQ7::FIRREFS2_Q7_ID,mgr);

          output.create(ref.nbSamples(),FIRQ7::OUT_Q7_ID,mgr);
          /* Max numTaps + blockSize, plus the guard samples */
          state.create(LONG_MAX_TAPS + LONG_MAX_BLOCKSIZE + STATE_GUARD,FIRQ7::OUT_Q7_ID,mgr);
        break;

        default:
          inputs.reload(FIRQ7::FIRINPUTS_Q7_ID,mgr);
          coefs.reload(FIRQ7::FIRCOEFS_Q7_ID,mgr);
          configs.reload(FIRQ7::FIRCONFIGS_S16_ID,mgr);
          ref.reload(FIRQ7::FIRREFS_Q7_ID,mgr);

          output.create(ref.nbSamples(),FIRQ7::OUT_Q7_ID,mgr);
          /* Max blockSize + numTaps - 1 as generated by Python script */
          state.create(47,FIRQ7::OUT_Q7_ID,mgr);
        break;
       }
    }

    void FIRQ7::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        output.dump(mgr);
    }
