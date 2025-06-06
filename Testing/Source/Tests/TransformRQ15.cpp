#include "TransformRQ15.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"


#define SNR_THRESHOLD 40
#define RIFFT_SNR_THRESHOLD 25

#define ABS_FFT_ERROR_Q15 ((q15_t)14)
#define ABS_IFFT_ERROR_Q15 ((q15_t)1250)

#if defined(ARM_MATH_NEON)
#define RFFT_INIT(L) \
  arm_rfft_init_q15(&this->instRfftQ15 ,L);
#define RIFFT_INIT(L) \
  arm_rfft_init_q15(&this->instRfftQ15 ,L);

#else 
#define RFFT_INIT(L) \
  arm_rfft_init_q15(&this->instRfftQ15 ,L,0,1);

#define RIFFT_INIT(L) \
  arm_rfft_init_q15(&this->instRfftQ15 ,L,1,1);
#endif

    void TransformRQ15::test_rfft_q15()
    {
       q15_t *inp = input.ptr();

       q15_t *tmp = inputchanged.ptr();

       q15_t *outp = outputfft.ptr();
       q15_t *overoutp = overheadoutputfft.ptr();
       
       memcpy(tmp,inp,sizeof(q15_t)*input.nbSamples());

#if defined(ARM_MATH_NEON)
      q15_t *tmp2p = tmp2.ptr();
      arm_rfft_q15(
             &this->instRfftQ15,
             tmp,
             overoutp,tmp2p,this->ifft);
#else
       arm_rfft_q15(
             &this->instRfftQ15,
             tmp,
             overoutp);
#endif 

       if (this->ifft)
       {
          for(unsigned long i = 0;i < overheadoutputfft.nbSamples(); i++)
          {
              overoutp[i] = overoutp[i] << this->scaling;
          }
       }


       memcpy(outp,overoutp,sizeof(q15_t)*outputfft.nbSamples());

       if (this->ifft)
       {
          ASSERT_SNR(outputfft,ref,(q15_t)RIFFT_SNR_THRESHOLD);
          ASSERT_NEAR_EQ(outputfft,ref,ABS_IFFT_ERROR_Q15);
       }
       else
       {
         ASSERT_SNR(outputfft,ref,(q15_t)SNR_THRESHOLD);
         ASSERT_NEAR_EQ(outputfft,ref,ABS_FFT_ERROR_Q15);
       }

       ASSERT_EMPTY_TAIL(outputfft);

        
    } 

  
    void TransformRQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {


       (void)paramsArgs;

       switch(id)
       {

          case TransformRQ15::TEST_RFFT_Q15_1:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_32_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_32_Q15_ID,mgr);

            RFFT_INIT(32);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_17:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_32_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_32_Q15_ID,mgr);

            RIFFT_INIT(32);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=5;

          break;

          case TransformRQ15::TEST_RFFT_Q15_2:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_64_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_64_Q15_ID,mgr);

            RFFT_INIT(64);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_18:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_64_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_64_Q15_ID,mgr);

            RIFFT_INIT(64);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=6;

          break;

          case TransformRQ15::TEST_RFFT_Q15_3:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_128_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_128_Q15_ID,mgr);

            RFFT_INIT(128);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_19:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_128_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_128_Q15_ID,mgr);

            RIFFT_INIT(128);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=7;

          break;

          case TransformRQ15::TEST_RFFT_Q15_4:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_256_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_256_Q15_ID,mgr);

            RFFT_INIT(256);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_20:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_256_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_256_Q15_ID,mgr);

            RIFFT_INIT(256);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=8;

          break;

          case TransformRQ15::TEST_RFFT_Q15_5:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_512_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_512_Q15_ID,mgr);

            RFFT_INIT(512);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_21:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_512_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_512_Q15_ID,mgr);

            RIFFT_INIT(512);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=9;

          break;

          case TransformRQ15::TEST_RFFT_Q15_6:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_1024_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_1024_Q15_ID,mgr);

            RFFT_INIT(1024);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_22:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_1024_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_1024_Q15_ID,mgr);

            RIFFT_INIT(1024);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=10;

          break;

          case TransformRQ15::TEST_RFFT_Q15_7:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_2048_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_2048_Q15_ID,mgr);

            RFFT_INIT(2048);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_23:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_2048_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_2048_Q15_ID,mgr);

            RIFFT_INIT(2048);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=11;

          break;

          case TransformRQ15::TEST_RFFT_Q15_8:

            input.reload(TransformRQ15::INPUTS_RFFT_NOISY_4096_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_NOISY_4096_Q15_ID,mgr);

            RFFT_INIT(4096);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_24:

            input.reload(TransformRQ15::INPUTS_RIFFT_NOISY_4096_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_NOISY_4096_Q15_ID,mgr);

            RIFFT_INIT(4096);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=12;

          break;

          /* STEP FUNCTIONS */

          case TransformRQ15::TEST_RFFT_Q15_9:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_32_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_32_Q15_ID,mgr);

            RFFT_INIT(32);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_25:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_32_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_32_Q15_ID,mgr);

            RIFFT_INIT(32);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=5;

          break;

          case TransformRQ15::TEST_RFFT_Q15_10:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_64_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_64_Q15_ID,mgr);

            RFFT_INIT(64);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_26:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_64_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_64_Q15_ID,mgr);

            RIFFT_INIT(64);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=6;

          break;

          case TransformRQ15::TEST_RFFT_Q15_11:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_128_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_128_Q15_ID,mgr);

            RFFT_INIT(128);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);
            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_27:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_128_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_128_Q15_ID,mgr);

            RIFFT_INIT(128);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=7;

          break;

          case TransformRQ15::TEST_RFFT_Q15_12:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_256_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_256_Q15_ID,mgr);

            RFFT_INIT(256);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_28:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_256_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_256_Q15_ID,mgr);

            RIFFT_INIT(256);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=8;

          break;

          case TransformRQ15::TEST_RFFT_Q15_13:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_512_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_512_Q15_ID,mgr);

            RFFT_INIT(512);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_29:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_512_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_512_Q15_ID,mgr);

            RIFFT_INIT(512);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=9;

          break;

          case TransformRQ15::TEST_RFFT_Q15_14:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_1024_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_1024_Q15_ID,mgr);

            RFFT_INIT(1024);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_30:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_1024_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_1024_Q15_ID,mgr);

            RIFFT_INIT(1024);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=10;

          break;

          case TransformRQ15::TEST_RFFT_Q15_15:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_2048_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_2048_Q15_ID,mgr);

            RFFT_INIT(2048);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_31:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_2048_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_2048_Q15_ID,mgr);

            RIFFT_INIT(2048);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=11;

          break;

          case TransformRQ15::TEST_RFFT_Q15_16:

            input.reload(TransformRQ15::INPUTS_RFFT_STEP_4096_Q15_ID,mgr);
            ref.reload(  TransformRQ15::REF_RFFT_STEP_4096_Q15_ID,mgr);

            RFFT_INIT(4096);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=0;

          break;

          case TransformRQ15::TEST_RFFT_Q15_32:

            input.reload(TransformRQ15::INPUTS_RIFFT_STEP_4096_Q15_ID,mgr);
            ref.reload(  TransformRQ15::INPUTS_RFFT_STEP_4096_Q15_ID,mgr);

            RIFFT_INIT(4096);

            inputchanged.create(input.nbSamples(),TransformRQ15::TEMP_Q15_ID,mgr);

            this->ifft=1;
            this->scaling=12;

          break;



       }

       
      outputfft.create(ref.nbSamples(),TransformRQ15::OUTPUT_RFFT_Q15_ID,mgr);
      /*

      RFFT is writing more samples than it should.
      This is a temporary buffer allowing the test to pass.

      */
#if defined(ARM_MATH_NEON)
      overheadoutputfft.create(ref.nbSamples(),TransformRQ15::FULLOUTPUT_Q15_ID,mgr);
#else 
      overheadoutputfft.create(2*ref.nbSamples(),TransformRQ15::FULLOUTPUT_Q15_ID,mgr);
#endif

      tmp2.create(ref.nbSamples(),TransformRQ15::FULLOUTPUT_Q15_ID,mgr);
    }

    void TransformRQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
        overheadoutputfft.dump(mgr);
    }
