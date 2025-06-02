#if defined(ARM_MATH_NEON)
#include "TransformNeonCQ31.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"           

#define SNR_THRESHOLD 100
#define ABS_ERROR ((q31_t)950)


    void TransformNeonCQ31::test_cfft_q31()
    {
       const q31_t *inp = input.ptr();

       q31_t *infftp = inputfft.ptr();

       q31_t *outfftp = outputfft.ptr();

        memcpy(infftp,inp,sizeof(q31_t)*input.nbSamples());

   
        q31_t *bufferp = bufferfft.ptr();
        arm_cfft_q31(
             this->varInstCfftQ31Ptr,
             infftp,
             outfftp,
             bufferp,
             this->ifft);

          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        ASSERT_NEAR_EQ(outputfft,ref,ABS_ERROR);
        ASSERT_EMPTY_TAIL(outputfft);


        
    }

        void TransformNeonCQ31::test_cifft_q31()
    {
       const q31_t *inp = input.ptr();

       q31_t *outfftp = outputfft.ptr();
       q31_t *refp = ref.ptr();

        memcpy(outfftp,inp,sizeof(q31_t)*input.nbSamples());
   
        q31_t *tmp2p = tmp2.ptr();
        arm_cfft_q31(
             this->varInstCfftQ31Ptr,
             inp,
             outfftp,
             tmp2p,
             this->ifft);


        for(unsigned long i=0; i < outputfft.nbSamples();i++)
        {
          refp[i] = (q31_t)(1.0*refp[i]/this->scaling);
        }
          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        ASSERT_NEAR_EQ(outputfft,ref,ABS_ERROR);
        ASSERT_EMPTY_TAIL(outputfft);

       
        
    } 



   void TransformNeonCQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
   {

       (void)paramsArgs;
       switch(id)
       {


         case TransformNeonCQ31::TEST_CFFT_Q31_1:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_20_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_20_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(20);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_2:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_24_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_24_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(24);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_3:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_32_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_32_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(32);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_4:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_36_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_36_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(36);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_5:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_40_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_40_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(40);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_6:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_48_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_48_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(48);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_7:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_60_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_60_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(60);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_8:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_64_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_64_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(64);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_9:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_72_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_72_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(72);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_10:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_80_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_80_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(80);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_11:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_96_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_96_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(96);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_12:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_100_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_100_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(100);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_13:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_108_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_108_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(108);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_14:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(120);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_15:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_128_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_128_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(128);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_16:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_144_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_144_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(144);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_17:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(160);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_18:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_180_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_180_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(180);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_19:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_192_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_192_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(192);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_20:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(200);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_21:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_216_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_216_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(216);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_22:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_240_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_240_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(240);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_23:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_256_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_256_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(256);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_24:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_288_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_288_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(288);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_25:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_300_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_300_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(300);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_26:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(320);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_27:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_360_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_360_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(360);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_28:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_384_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_384_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(384);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_29:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(400);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_30:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_432_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_432_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(432);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_31:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_480_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_480_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(480);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_32:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(500);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_33:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_512_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_512_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(512);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_34:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_540_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_540_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(540);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_35:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_576_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_576_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(576);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_36:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(600);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_37:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_640_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_640_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(640);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_38:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_720_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_720_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(720);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_39:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_768_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_768_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(768);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_40:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(800);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_41:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_864_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_864_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(864);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_42:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_900_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_900_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(900);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_43:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_960_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_960_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(960);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_44:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_45:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1024_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1024_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1024);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_46:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1080_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1080_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1080);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_47:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1152_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1152_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1152);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_48:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1200);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_49:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1280_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1280_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1280);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_50:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1440_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1440_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1440);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_51:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1500);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_52:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1536_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1536_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1536);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_53:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1600);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_54:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1728_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1728_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1728);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_55:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1800);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_56:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_1920_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_1920_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1920);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_57:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_58:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2048_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2048_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2048);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_59:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2160);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_60:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2304_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2304_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2304);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_61:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2400);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_62:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2560_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2560_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2560);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_63:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2700_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2700_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2700);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_64:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_2880_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_2880_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2880);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_65:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_3000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_3000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_66:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_3072_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_3072_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3072);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_67:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_3200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_3200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3200);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_68:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_3456_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_3456_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3456);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_69:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_3600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_3600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3600);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_70:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_3840_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_3840_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3840);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_71:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_4000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_4000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_72:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_4096_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_4096_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4096);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_73:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_4320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_4320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4320);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_74:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_4500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_4500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4500);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_75:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_4608_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_4608_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4608);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_76:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_4800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_4800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4800);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_77:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_5120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_5120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5120);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_78:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_NOISY_5400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_NOISY_5400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5400);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_79:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_20_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_20_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(20);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_80:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_24_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_24_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(24);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_81:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_32_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_32_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(32);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_82:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_36_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_36_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(36);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_83:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_40_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_40_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(40);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_84:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_48_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_48_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(48);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_85:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_60_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_60_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(60);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_86:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_64_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_64_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(64);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_87:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_72_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_72_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(72);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_88:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_80_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_80_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(80);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_89:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_96_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_96_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(96);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_90:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_100_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_100_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(100);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_91:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_108_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_108_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(108);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_92:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(120);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_93:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_128_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_128_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(128);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_94:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_144_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_144_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(144);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_95:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(160);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_96:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_180_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_180_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(180);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_97:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_192_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_192_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(192);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_98:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(200);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_99:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_216_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_216_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(216);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_100:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_240_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_240_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(240);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_101:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_256_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_256_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(256);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_102:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_288_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_288_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(288);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_103:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_300_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_300_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(300);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_104:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(320);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_105:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_360_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_360_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(360);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_106:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_384_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_384_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(384);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_107:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(400);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_108:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_432_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_432_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(432);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_109:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_480_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_480_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(480);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_110:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(500);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_111:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_512_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_512_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(512);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_112:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_540_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_540_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(540);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_113:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_576_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_576_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(576);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_114:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(600);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_115:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_640_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_640_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(640);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_116:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_720_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_720_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(720);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_117:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_768_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_768_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(768);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_118:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(800);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_119:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_864_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_864_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(864);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_120:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_900_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_900_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(900);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_121:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_960_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_960_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(960);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_122:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_123:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1024_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1024_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1024);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_124:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1080_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1080_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1080);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_125:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1152_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1152_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1152);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_126:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1200);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_127:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1280_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1280_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1280);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_128:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1440_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1440_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1440);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_129:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1500);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_130:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1536_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1536_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1536);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_131:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1600);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_132:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1728_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1728_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1728);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_133:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1800);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_134:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_1920_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_1920_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1920);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_135:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_136:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2048_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2048_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2048);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_137:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2160);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_138:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2304_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2304_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2304);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_139:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2400);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_140:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2560_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2560_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2560);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_141:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2700_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2700_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2700);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_142:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_2880_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_2880_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2880);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_143:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_3000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_3000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_144:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_3072_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_3072_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3072);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_145:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_3200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_3200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3200);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_146:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_3456_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_3456_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3456);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_147:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_3600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_3600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3600);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_148:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_3840_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_3840_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3840);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_149:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_4000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_4000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4000);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_150:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_4096_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_4096_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4096);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_151:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_4320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_4320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4320);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_152:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_4500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_4500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4500);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_153:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_4608_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_4608_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4608);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_154:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_4800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_4800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4800);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_155:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_5120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_5120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5120);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CFFT_Q31_156:

            input.reload(TransformNeonCQ31::INPUTS_CFFT_STEP_5400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::REF_CFFT_STEP_5400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5400);

            this->ifft=0;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_157:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_20_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_20_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(20);

            this->ifft=1;
            this->scaling = 20;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_158:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_24_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_24_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(24);

            this->ifft=1;
            this->scaling = 24;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_159:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_32_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_32_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(32);

            this->ifft=1;
            this->scaling = 32;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_160:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_36_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_36_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(36);

            this->ifft=1;
            this->scaling = 36;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_161:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_40_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_40_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(40);

            this->ifft=1;
            this->scaling = 40;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_162:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_48_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_48_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(48);

            this->ifft=1;
            this->scaling = 48;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_163:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_60_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_60_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(60);

            this->ifft=1;
            this->scaling = 60;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_164:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_64_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_64_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(64);

            this->ifft=1;
            this->scaling = 64;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_165:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_72_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_72_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(72);

            this->ifft=1;
            this->scaling = 72;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_166:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_80_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_80_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(80);

            this->ifft=1;
            this->scaling = 80;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_167:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_96_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_96_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(96);

            this->ifft=1;
            this->scaling = 96;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_168:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_100_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_100_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(100);

            this->ifft=1;
            this->scaling = 100;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_169:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_108_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_108_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(108);

            this->ifft=1;
            this->scaling = 108;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_170:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(120);

            this->ifft=1;
            this->scaling = 120;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_171:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_128_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_128_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(128);

            this->ifft=1;
            this->scaling = 128;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_172:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_144_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_144_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(144);

            this->ifft=1;
            this->scaling = 144;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_173:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(160);

            this->ifft=1;
            this->scaling = 160;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_174:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_180_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_180_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(180);

            this->ifft=1;
            this->scaling = 180;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_175:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_192_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_192_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(192);

            this->ifft=1;
            this->scaling = 192;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_176:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(200);

            this->ifft=1;
            this->scaling = 200;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_177:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_216_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_216_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(216);

            this->ifft=1;
            this->scaling = 216;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_178:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_240_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_240_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(240);

            this->ifft=1;
            this->scaling = 240;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_179:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_256_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_256_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(256);

            this->ifft=1;
            this->scaling = 256;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_180:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_288_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_288_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(288);

            this->ifft=1;
            this->scaling = 288;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_181:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_300_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_300_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(300);

            this->ifft=1;
            this->scaling = 300;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_182:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(320);

            this->ifft=1;
            this->scaling = 320;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_183:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_360_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_360_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(360);

            this->ifft=1;
            this->scaling = 360;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_184:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_384_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_384_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(384);

            this->ifft=1;
            this->scaling = 384;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_185:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(400);

            this->ifft=1;
            this->scaling = 400;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_186:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_432_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_432_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(432);

            this->ifft=1;
            this->scaling = 432;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_187:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_480_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_480_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(480);

            this->ifft=1;
            this->scaling = 480;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_188:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(500);

            this->ifft=1;
            this->scaling = 500;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_189:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_512_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_512_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(512);

            this->ifft=1;
            this->scaling = 512;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_190:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_540_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_540_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(540);

            this->ifft=1;
            this->scaling = 540;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_191:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_576_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_576_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(576);

            this->ifft=1;
            this->scaling = 576;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_192:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(600);

            this->ifft=1;
            this->scaling = 600;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_193:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_640_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_640_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(640);

            this->ifft=1;
            this->scaling = 640;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_194:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_720_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_720_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(720);

            this->ifft=1;
            this->scaling = 720;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_195:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_768_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_768_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(768);

            this->ifft=1;
            this->scaling = 768;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_196:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(800);

            this->ifft=1;
            this->scaling = 800;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_197:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_864_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_864_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(864);

            this->ifft=1;
            this->scaling = 864;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_198:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_900_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_900_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(900);

            this->ifft=1;
            this->scaling = 900;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_199:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_960_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_960_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(960);

            this->ifft=1;
            this->scaling = 960;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_200:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1000);

            this->ifft=1;
            this->scaling = 1000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_201:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1024_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1024_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1024);

            this->ifft=1;
            this->scaling = 1024;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_202:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1080_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1080_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1080);

            this->ifft=1;
            this->scaling = 1080;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_203:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1152_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1152_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1152);

            this->ifft=1;
            this->scaling = 1152;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_204:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1200);

            this->ifft=1;
            this->scaling = 1200;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_205:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1280_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1280_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1280);

            this->ifft=1;
            this->scaling = 1280;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_206:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1440_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1440_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1440);

            this->ifft=1;
            this->scaling = 1440;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_207:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1500);

            this->ifft=1;
            this->scaling = 1500;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_208:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1536_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1536_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1536);

            this->ifft=1;
            this->scaling = 1536;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_209:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1600);

            this->ifft=1;
            this->scaling = 1600;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_210:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1728_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1728_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1728);

            this->ifft=1;
            this->scaling = 1728;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_211:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1800);

            this->ifft=1;
            this->scaling = 1800;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_212:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_1920_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_1920_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1920);

            this->ifft=1;
            this->scaling = 1920;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_213:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2000);

            this->ifft=1;
            this->scaling = 2000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_214:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2048_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2048_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2048);

            this->ifft=1;
            this->scaling = 2048;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_215:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2160);

            this->ifft=1;
            this->scaling = 2160;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_216:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2304_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2304_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2304);

            this->ifft=1;
            this->scaling = 2304;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_217:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2400);

            this->ifft=1;
            this->scaling = 2400;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_218:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2560_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2560_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2560);

            this->ifft=1;
            this->scaling = 2560;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_219:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2700_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2700_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2700);

            this->ifft=1;
            this->scaling = 2700;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_220:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_2880_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_2880_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2880);

            this->ifft=1;
            this->scaling = 2880;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_221:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_3000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_3000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3000);

            this->ifft=1;
            this->scaling = 3000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_222:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_3072_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_3072_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3072);

            this->ifft=1;
            this->scaling = 3072;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_223:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_3200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_3200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3200);

            this->ifft=1;
            this->scaling = 3200;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_224:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_3456_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_3456_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3456);

            this->ifft=1;
            this->scaling = 3456;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_225:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_3600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_3600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3600);

            this->ifft=1;
            this->scaling = 3600;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_226:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_3840_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_3840_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3840);

            this->ifft=1;
            this->scaling = 3840;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_227:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_4000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_4000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4000);

            this->ifft=1;
            this->scaling = 4000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_228:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_4096_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_4096_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4096);

            this->ifft=1;
            this->scaling = 4096;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_229:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_4320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_4320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4320);

            this->ifft=1;
            this->scaling = 4320;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_230:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_4500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_4500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4500);

            this->ifft=1;
            this->scaling = 4500;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_231:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_4608_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_4608_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4608);

            this->ifft=1;
            this->scaling = 4608;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_232:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_4800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_4800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4800);

            this->ifft=1;
            this->scaling = 4800;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_233:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_5120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_5120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5120);

            this->ifft=1;
            this->scaling = 5120;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_234:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_NOISY_5400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_NOISY_5400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5400);

            this->ifft=1;
            this->scaling = 5400;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_235:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_20_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_20_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(20);

            this->ifft=1;
            this->scaling = 20;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_236:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_24_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_24_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(24);

            this->ifft=1;
            this->scaling = 24;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_237:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_32_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_32_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(32);

            this->ifft=1;
            this->scaling = 32;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_238:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_36_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_36_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(36);

            this->ifft=1;
            this->scaling = 36;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_239:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_40_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_40_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(40);

            this->ifft=1;
            this->scaling = 40;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_240:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_48_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_48_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(48);

            this->ifft=1;
            this->scaling = 48;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_241:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_60_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_60_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(60);

            this->ifft=1;
            this->scaling = 60;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_242:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_64_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_64_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(64);

            this->ifft=1;
            this->scaling = 64;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_243:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_72_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_72_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(72);

            this->ifft=1;
            this->scaling = 72;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_244:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_80_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_80_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(80);

            this->ifft=1;
            this->scaling = 80;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_245:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_96_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_96_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(96);

            this->ifft=1;
            this->scaling = 96;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_246:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_100_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_100_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(100);

            this->ifft=1;
            this->scaling = 100;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_247:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_108_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_108_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(108);

            this->ifft=1;
            this->scaling = 108;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_248:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(120);

            this->ifft=1;
            this->scaling = 120;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_249:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_128_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_128_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(128);

            this->ifft=1;
            this->scaling = 128;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_250:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_144_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_144_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(144);

            this->ifft=1;
            this->scaling = 144;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_251:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(160);

            this->ifft=1;
            this->scaling = 160;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_252:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_180_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_180_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(180);

            this->ifft=1;
            this->scaling = 180;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_253:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_192_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_192_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(192);

            this->ifft=1;
            this->scaling = 192;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_254:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(200);

            this->ifft=1;
            this->scaling = 200;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_255:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_216_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_216_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(216);

            this->ifft=1;
            this->scaling = 216;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_256:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_240_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_240_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(240);

            this->ifft=1;
            this->scaling = 240;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_257:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_256_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_256_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(256);

            this->ifft=1;
            this->scaling = 256;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_258:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_288_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_288_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(288);

            this->ifft=1;
            this->scaling = 288;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_259:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_300_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_300_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(300);

            this->ifft=1;
            this->scaling = 300;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_260:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(320);

            this->ifft=1;
            this->scaling = 320;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_261:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_360_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_360_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(360);

            this->ifft=1;
            this->scaling = 360;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_262:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_384_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_384_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(384);

            this->ifft=1;
            this->scaling = 384;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_263:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(400);

            this->ifft=1;
            this->scaling = 400;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_264:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_432_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_432_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(432);

            this->ifft=1;
            this->scaling = 432;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_265:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_480_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_480_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(480);

            this->ifft=1;
            this->scaling = 480;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_266:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(500);

            this->ifft=1;
            this->scaling = 500;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_267:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_512_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_512_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(512);

            this->ifft=1;
            this->scaling = 512;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_268:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_540_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_540_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(540);

            this->ifft=1;
            this->scaling = 540;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_269:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_576_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_576_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(576);

            this->ifft=1;
            this->scaling = 576;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_270:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(600);

            this->ifft=1;
            this->scaling = 600;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_271:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_640_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_640_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(640);

            this->ifft=1;
            this->scaling = 640;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_272:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_720_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_720_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(720);

            this->ifft=1;
            this->scaling = 720;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_273:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_768_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_768_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(768);

            this->ifft=1;
            this->scaling = 768;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_274:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(800);

            this->ifft=1;
            this->scaling = 800;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_275:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_864_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_864_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(864);

            this->ifft=1;
            this->scaling = 864;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_276:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_900_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_900_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(900);

            this->ifft=1;
            this->scaling = 900;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_277:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_960_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_960_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(960);

            this->ifft=1;
            this->scaling = 960;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_278:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1000);

            this->ifft=1;
            this->scaling = 1000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_279:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1024_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1024_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1024);

            this->ifft=1;
            this->scaling = 1024;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_280:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1080_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1080_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1080);

            this->ifft=1;
            this->scaling = 1080;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_281:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1152_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1152_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1152);

            this->ifft=1;
            this->scaling = 1152;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_282:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1200);

            this->ifft=1;
            this->scaling = 1200;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_283:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1280_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1280_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1280);

            this->ifft=1;
            this->scaling = 1280;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_284:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1440_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1440_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1440);

            this->ifft=1;
            this->scaling = 1440;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_285:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1500);

            this->ifft=1;
            this->scaling = 1500;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_286:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1536_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1536_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1536);

            this->ifft=1;
            this->scaling = 1536;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_287:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1600);

            this->ifft=1;
            this->scaling = 1600;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_288:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1728_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1728_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1728);

            this->ifft=1;
            this->scaling = 1728;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_289:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1800);

            this->ifft=1;
            this->scaling = 1800;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_290:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_1920_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_1920_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(1920);

            this->ifft=1;
            this->scaling = 1920;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_291:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2000);

            this->ifft=1;
            this->scaling = 2000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_292:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2048_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2048_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2048);

            this->ifft=1;
            this->scaling = 2048;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_293:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2160_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2160_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2160);

            this->ifft=1;
            this->scaling = 2160;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_294:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2304_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2304_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2304);

            this->ifft=1;
            this->scaling = 2304;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_295:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2400);

            this->ifft=1;
            this->scaling = 2400;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_296:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2560_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2560_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2560);

            this->ifft=1;
            this->scaling = 2560;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_297:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2700_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2700_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2700);

            this->ifft=1;
            this->scaling = 2700;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_298:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_2880_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_2880_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(2880);

            this->ifft=1;
            this->scaling = 2880;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_299:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_3000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_3000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3000);

            this->ifft=1;
            this->scaling = 3000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_300:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_3072_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_3072_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3072);

            this->ifft=1;
            this->scaling = 3072;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_301:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_3200_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_3200_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3200);

            this->ifft=1;
            this->scaling = 3200;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_302:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_3456_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_3456_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3456);

            this->ifft=1;
            this->scaling = 3456;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_303:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_3600_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_3600_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3600);

            this->ifft=1;
            this->scaling = 3600;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_304:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_3840_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_3840_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(3840);

            this->ifft=1;
            this->scaling = 3840;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_305:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_4000_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_4000_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4000);

            this->ifft=1;
            this->scaling = 4000;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_306:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_4096_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_4096_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4096);

            this->ifft=1;
            this->scaling = 4096;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_307:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_4320_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_4320_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4320);

            this->ifft=1;
            this->scaling = 4320;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_308:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_4500_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_4500_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4500);

            this->ifft=1;
            this->scaling = 4500;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_309:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_4608_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_4608_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4608);

            this->ifft=1;
            this->scaling = 4608;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_310:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_4800_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_4800_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(4800);

            this->ifft=1;
            this->scaling = 4800;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_311:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_5120_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_5120_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5120);

            this->ifft=1;
            this->scaling = 5120;

          break;


         case TransformNeonCQ31::TEST_CIFFT_Q31_312:

            input.reload(TransformNeonCQ31::INPUTS_CIFFT_STEP_5400_Q31_ID,mgr);
            ref.reload(  TransformNeonCQ31::INPUTS_CFFT_STEP_5400_Q31_ID,mgr);

            this->varInstCfftQ31Ptr=arm_cfft_init_dynamic_q31(5400);

            this->ifft=1;
            this->scaling = 5400;

          break;


       }
       inputfft.create(ref.nbSamples(),TransformNeonCQ31::OUTPUT_CFFT_Q31_ID,mgr);

       outputfft.create(ref.nbSamples(),TransformNeonCQ31::OUTPUT_CFFT_Q31_ID,mgr);
       bufferfft.create(ref.nbSamples(),TransformNeonCQ31::OUTPUT_CFFT_Q31_ID,mgr);
       tmp2.create(ref.nbSamples(),TransformNeonCQ31::OUTPUT_CFFT_Q31_ID,mgr);

    }

    void TransformNeonCQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
        if (varInstCfftQ31Ptr)
        {
            free(varInstCfftQ31Ptr);
        }
    }

#endif
