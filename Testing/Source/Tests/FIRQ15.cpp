#include "FIRQ15.h"
#include <stdio.h>
#include "Error.h"

#define SNR_THRESHOLD 59

#define ABS_ERROR_Q15 ((q15_t)2)

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
static __ALIGNED(8) q15_t coeffArray[32];
#endif 

static void checkInnerTail(q15_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
    ASSERT_TRUE(b[2] == 0);
    ASSERT_TRUE(b[3] == 0);
}

// Coef must be padded to a multiple of 8
#define FIRCOEFPADDING 3

    void FIRQ15::test_fir_q15()
    {
        

        const int16_t *configp = configs.ptr();
        q15_t *statep = state.ptr();
        const q15_t *orgcoefsp = coefs.ptr();

        const q15_t *coefsp;
        const q15_t *inputp = inputs.ptr();
        q15_t *outp = output.ptr();

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
        for(i=0; i < configs.nbSamples(); i += 2)
        {
           blockSize = configp[0];
           numTaps = configp[1];

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
            /* Copy coefficients and pad to zero 
           */
           memset(coeffArray,127,32*sizeof(q15_t));
           round = numTaps >> FIRCOEFPADDING;
           if ((round << FIRCOEFPADDING) < numTaps)
           {
             round ++;
           }
           round = round<<FIRCOEFPADDING;
           memset(coeffArray,0,round*sizeof(q15_t));

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
           arm_fir_init_q15(&this->S,numTaps,coefsp,statep,blockSize);

           /*

           Input pointer is reset since the same input pattern is used

           */
           inputp = inputs.ptr();

       
           /*
           
           Python script is filtering a 2*blockSize number of samples.
           We do the same filtering in two pass to check (indirectly that
           the state management of the fir is working.)

           */
           arm_fir_q15(&this->S,inputp,outp,blockSize);
           outp += blockSize;
           checkInnerTail(outp);

           inputp += blockSize;
           arm_fir_q15(&this->S,inputp,outp,blockSize);
           outp += blockSize;
           checkInnerTail(outp);

           configp += 2;
           orgcoefsp += numTaps;


        }

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(q15_t)SNR_THRESHOLD);

        ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q15);

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
#define STATE_GUARD_VALUE ((q15_t)0x5A)

// Coefficient array padded with zeros to a multiple of 8 (Helium)
static __ALIGNED(8) q15_t longCoeffArray[LONG_MAX_TAPS + 8];

    void FIRQ15::test_fir_long_q15()
    {
        const int16_t *configp = configs.ptr();
        q15_t *statep = state.ptr();
        const q15_t *orgcoefsp = coefs.ptr();
        const q15_t *inputp;
        q15_t *outp = output.ptr();
        arm_status status;
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
#if defined(ARM_MATH_LOOPUNROLL)
           stateSize = numTaps + blockSize;
#else
           stateSize = numTaps + blockSize - 1;
#endif

           memset(longCoeffArray,0,sizeof(longCoeffArray));
           memcpy(longCoeffArray,orgcoefsp,numTaps*sizeof(q15_t));

           for(j=0; j < STATE_GUARD; j++)
           {
              statep[stateSize + j] = STATE_GUARD_VALUE;
           }

           status = arm_fir_init_q15(&this->S,numTaps,longCoeffArray,statep,blockSize);
           ASSERT_TRUE(status == ARM_MATH_SUCCESS);

           inputp = inputs.ptr();

           for(k=0; k < LONG_NBCALLS; k++)
           {
              arm_fir_q15(&this->S,inputp,outp,blockSize);
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

    void FIRQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
      
       (void)params;

       switch(id)
       {
        case FIRQ15::TEST_FIR_LONG_Q15_2:
          inputs.reload(FIRQ15::FIRINPUTS2_Q15_ID,mgr);
          coefs.reload(FIRQ15::FIRCOEFS2_Q15_ID,mgr);
          configs.reload(FIRQ15::FIRCONFIGS2_S16_ID,mgr);
          ref.reload(FIRQ15::FIRREFS2_Q15_ID,mgr);

          output.create(ref.nbSamples(),FIRQ15::OUT_Q15_ID,mgr);
          /* Max numTaps + blockSize, plus the guard samples */
          state.create(LONG_MAX_TAPS + LONG_MAX_BLOCKSIZE + STATE_GUARD,FIRQ15::OUT_Q15_ID,mgr);
        break;

        default:
          inputs.reload(FIRQ15::FIRINPUTS_Q15_ID,mgr);
          coefs.reload(FIRQ15::FIRCOEFS_Q15_ID,mgr);
          configs.reload(FIRQ15::FIRCONFIGS_S16_ID,mgr);
          ref.reload(FIRQ15::FIRREFS_Q15_ID,mgr);

          output.create(ref.nbSamples(),FIRQ15::OUT_Q15_ID,mgr);
          /* > Max blockSize + numTaps as generated by Python script
             numTaps may be increased by 1 by Python script to force it to even values
             */
          state.create(3 * 41,FIRQ15::OUT_Q15_ID,mgr);
        break;
       }
    }

    void FIRQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        output.dump(mgr);
    }
