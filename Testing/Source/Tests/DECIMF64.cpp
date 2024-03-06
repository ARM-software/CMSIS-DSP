#include "DECIMF64.h"
#include <stdio.h>
#include "Error.h"

#define SNR_THRESHOLD 120

/* 

Reference patterns are generated with
a double precision computation.

*/
#define REL_ERROR (8.0e-4)


    void DECIMF64::test_fir_decimate_f64()
    {
        int nbTests;
        int nb;
        uint32_t *pConfig = config.ptr();

        const float64_t * pSrc = input.ptr();
        float64_t * pDst = output.ptr();
        float64_t * pCoefs = coefs.ptr();

        nbTests=config.nbSamples() / 4;

        for(nb=0;nb < nbTests; nb++)
        {

            this->q = pConfig[0];
            this->numTaps = pConfig[1];
            this->blocksize = pConfig[2];
            this->refsize = pConfig[3];


            pConfig += 4;

            this->status=arm_fir_decimate_init_f64(&(this->S),
               this->numTaps,
               this->q,
               pCoefs,
               state.ptr(),
               this->blocksize);



            ASSERT_TRUE(this->status == ARM_MATH_SUCCESS);

            arm_fir_decimate_f64(
                 &(this->S),
                 pSrc,
                 pDst,
                 this->blocksize);

            pSrc += this->blocksize;
            pDst += this->refsize;

            pCoefs += this->numTaps;
        }


        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_REL_ERROR(output,ref,REL_ERROR);

    } 

#if 0
    void DECIMF64::test_fir_interpolate_f64()
    {
        int nbTests;
        int nb;
        uint64_t *pConfig = config.ptr();

        const float64_t * pSrc = input.ptr();
        float64_t * pDst = output.ptr();
        float64_t * pCoefs = coefs.ptr();

        nbTests=config.nbSamples() / 4;

        for(nb=0;nb < nbTests; nb++)
        {

            this->q = pConfig[0];
            this->numTaps = pConfig[1];
            this->blocksize = pConfig[2];
            this->refsize = pConfig[3];



            pConfig += 4;

            this->status=arm_fir_interpolate_init_f64(&(this->SI),
               this->q,
               this->numTaps,
               pCoefs,
               state.ptr(),
               this->blocksize);



            ASSERT_TRUE(this->status == ARM_MATH_SUCCESS);

            arm_fir_interpolate_f64(
                 &(this->SI),
                 pSrc,
                 pDst,
                 this->blocksize);

            pSrc += this->blocksize;
            pDst += this->refsize;

            pCoefs += this->numTaps;
        }


        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_REL_ERROR(output,ref,REL_ERROR);

    } 

#endif
    void DECIMF64::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
      
       (void)params;
       
       switch(id)
       {
        case DECIMF64::TEST_FIR_DECIMATE_F64_1:
          config.reload(DECIMF64::CONFIGSDECIMF64_ID,mgr);
         
          input.reload(DECIMF64::INPUT1_F64_ID,mgr);
          coefs.reload(DECIMF64::COEFS1_F64_ID,mgr);

          ref.reload(DECIMF64::REF1_DECIM_F64_ID,mgr);
          state.create(16 + 768 - 1,DECIMF64::STATE_F64_ID,mgr);

          break;

#if 0
        case DECIMF64::TEST_FIR_INTERPOLATE_F64_2:
          config.reload(DECIMF64::CONFIGSINTERPF64_ID,mgr);
         
          input.reload(DECIMF64::INPUT2_F64_ID,mgr);
          coefs.reload(DECIMF64::COEFS2_F64_ID,mgr);

          ref.reload(DECIMF64::REF2_INTERP_F64_ID,mgr);
          state.create(16 + 768 - 1,DECIMF64::STATE_F64_ID,mgr);

          break;
#endif

       }
      

       

       output.create(ref.nbSamples(),DECIMF64::OUT_F64_ID,mgr);
    }

    void DECIMF64::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        output.dump(mgr);
    }
