#if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)
#include "TransformNeonCF16.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"           

#define SNR_THRESHOLD 56
#define REL_ERROR (5.0e-2)
#define ABS_ERROR (1.0e-1)

    void TransformNeonCF16::test_cfft_f16()
    {
       const float16_t *inp = input.ptr();

       float16_t *infftp = inputfft.ptr();

       float16_t *outfftp = outputfft.ptr();

        memcpy(infftp,inp,sizeof(float16_t)*input.nbSamples());

   
        float16_t *bufferp = bufferfft.ptr();
        arm_cfft_f16(
             this->varInstCfftF16Ptr,
             infftp,
             outfftp,
             bufferp,
             this->ifft);

          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        ASSERT_CLOSE_ERROR(outputfft,ref,ABS_ERROR,REL_ERROR);
        ASSERT_EMPTY_TAIL(outputfft);


        
    }

    


   void TransformNeonCF16::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
   {

       (void)paramsArgs;
       switch(id)
       {


         case TransformNeonCF16::TEST_CFFT_F16_1:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_20_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_20_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(20);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_2:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_24_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_24_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(24);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_3:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_32_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_32_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(32);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_4:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_36_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_36_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(36);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_5:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_40_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_40_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(40);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_6:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_48_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_48_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(48);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_7:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_60_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_60_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(60);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_8:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_64_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_64_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(64);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_9:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_72_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_72_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(72);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_10:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_80_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_80_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(80);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_11:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_96_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_96_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(96);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_12:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_100_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_100_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(100);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_13:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_108_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_108_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(108);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_14:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(120);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_15:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_128_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_128_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(128);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_16:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_144_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_144_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(144);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_17:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(160);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_18:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_180_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_180_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(180);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_19:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_192_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_192_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(192);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_20:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(200);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_21:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_216_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_216_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(216);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_22:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_240_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_240_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(240);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_23:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_256_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_256_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(256);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_24:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_288_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_288_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(288);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_25:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_300_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_300_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(300);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_26:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(320);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_27:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_360_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_360_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(360);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_28:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_384_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_384_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(384);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_29:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(400);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_30:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_432_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_432_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(432);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_31:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_480_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_480_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(480);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_32:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(500);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_33:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_512_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_512_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(512);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_34:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_540_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_540_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(540);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_35:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_576_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_576_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(576);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_36:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(600);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_37:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_640_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_640_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(640);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_38:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_720_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_720_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(720);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_39:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_768_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_768_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(768);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_40:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(800);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_41:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_864_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_864_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(864);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_42:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_900_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_900_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(900);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_43:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_960_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_960_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(960);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_44:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_45:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1024_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1024_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1024);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_46:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1080_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1080_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1080);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_47:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1152_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1152_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1152);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_48:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1200);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_49:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1280_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1280_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1280);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_50:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1440_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1440_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1440);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_51:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1500);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_52:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1536_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1536_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1536);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_53:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1600);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_54:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1728_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1728_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1728);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_55:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1800);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_56:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_1920_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_1920_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1920);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_57:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_58:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2048_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2048_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2048);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_59:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2160);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_60:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2304_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2304_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2304);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_61:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2400);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_62:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2560_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2560_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2560);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_63:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2700_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2700_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2700);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_64:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_2880_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_2880_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2880);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_65:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_3000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_3000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_66:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_3072_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_3072_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3072);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_67:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_3200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_3200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3200);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_68:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_3456_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_3456_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3456);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_69:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_3600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_3600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3600);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_70:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_3840_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_3840_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3840);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_71:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_4000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_4000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_72:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_4096_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_4096_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4096);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_73:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_4320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_4320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4320);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_74:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_4500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_4500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4500);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_75:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_4608_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_4608_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4608);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_76:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_4800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_4800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4800);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_77:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_5120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_5120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5120);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_78:

            input.reload(TransformNeonCF16::INPUTS_CFFT_NOISY_5400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_NOISY_5400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5400);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_79:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_20_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_20_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(20);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_80:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_24_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_24_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(24);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_81:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_32_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_32_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(32);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_82:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_36_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_36_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(36);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_83:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_40_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_40_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(40);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_84:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_48_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_48_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(48);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_85:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_60_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_60_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(60);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_86:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_64_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_64_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(64);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_87:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_72_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_72_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(72);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_88:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_80_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_80_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(80);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_89:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_96_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_96_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(96);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_90:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_100_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_100_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(100);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_91:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_108_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_108_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(108);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_92:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(120);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_93:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_128_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_128_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(128);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_94:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_144_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_144_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(144);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_95:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(160);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_96:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_180_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_180_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(180);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_97:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_192_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_192_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(192);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_98:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(200);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_99:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_216_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_216_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(216);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_100:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_240_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_240_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(240);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_101:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_256_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_256_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(256);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_102:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_288_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_288_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(288);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_103:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_300_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_300_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(300);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_104:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(320);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_105:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_360_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_360_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(360);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_106:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_384_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_384_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(384);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_107:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(400);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_108:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_432_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_432_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(432);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_109:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_480_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_480_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(480);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_110:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(500);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_111:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_512_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_512_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(512);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_112:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_540_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_540_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(540);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_113:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_576_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_576_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(576);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_114:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(600);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_115:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_640_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_640_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(640);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_116:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_720_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_720_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(720);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_117:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_768_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_768_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(768);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_118:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(800);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_119:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_864_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_864_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(864);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_120:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_900_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_900_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(900);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_121:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_960_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_960_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(960);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_122:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_123:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1024_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1024_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1024);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_124:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1080_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1080_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1080);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_125:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1152_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1152_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1152);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_126:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1200);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_127:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1280_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1280_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1280);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_128:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1440_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1440_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1440);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_129:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1500);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_130:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1536_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1536_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1536);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_131:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1600);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_132:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1728_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1728_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1728);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_133:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1800);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_134:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_1920_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_1920_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1920);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_135:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_136:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2048_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2048_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2048);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_137:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2160);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_138:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2304_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2304_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2304);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_139:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2400);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_140:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2560_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2560_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2560);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_141:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2700_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2700_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2700);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_142:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_2880_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_2880_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2880);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_143:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_3000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_3000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_144:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_3072_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_3072_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3072);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_145:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_3200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_3200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3200);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_146:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_3456_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_3456_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3456);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_147:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_3600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_3600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3600);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_148:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_3840_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_3840_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3840);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_149:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_4000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_4000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4000);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_150:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_4096_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_4096_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4096);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_151:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_4320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_4320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4320);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_152:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_4500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_4500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4500);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_153:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_4608_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_4608_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4608);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_154:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_4800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_4800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4800);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_155:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_5120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_5120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5120);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_156:

            input.reload(TransformNeonCF16::INPUTS_CFFT_STEP_5400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::REF_CFFT_STEP_5400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5400);

            this->ifft=0;

          break;


         case TransformNeonCF16::TEST_CFFT_F16_157:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_20_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_20_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(20);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_158:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_24_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_24_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(24);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_159:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_32_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_32_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(32);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_160:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_36_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_36_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(36);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_161:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_40_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_40_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(40);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_162:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_48_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_48_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(48);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_163:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_60_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_60_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(60);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_164:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_64_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_64_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(64);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_165:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_72_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_72_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(72);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_166:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_80_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_80_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(80);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_167:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_96_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_96_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(96);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_168:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_100_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_100_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(100);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_169:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_108_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_108_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(108);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_170:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(120);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_171:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_128_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_128_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(128);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_172:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_144_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_144_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(144);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_173:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(160);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_174:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_180_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_180_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(180);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_175:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_192_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_192_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(192);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_176:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(200);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_177:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_216_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_216_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(216);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_178:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_240_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_240_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(240);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_179:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_256_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_256_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(256);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_180:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_288_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_288_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(288);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_181:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_300_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_300_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(300);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_182:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(320);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_183:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_360_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_360_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(360);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_184:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_384_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_384_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(384);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_185:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(400);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_186:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_432_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_432_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(432);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_187:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_480_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_480_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(480);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_188:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(500);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_189:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_512_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_512_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(512);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_190:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_540_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_540_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(540);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_191:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_576_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_576_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(576);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_192:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(600);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_193:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_640_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_640_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(640);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_194:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_720_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_720_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(720);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_195:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_768_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_768_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(768);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_196:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(800);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_197:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_864_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_864_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(864);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_198:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_900_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_900_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(900);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_199:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_960_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_960_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(960);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_200:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_201:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1024_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1024_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1024);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_202:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1080_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1080_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1080);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_203:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1152_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1152_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1152);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_204:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1200);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_205:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1280_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1280_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1280);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_206:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1440_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1440_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1440);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_207:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1500);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_208:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1536_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1536_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1536);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_209:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1600);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_210:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1728_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1728_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1728);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_211:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1800);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_212:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_1920_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_1920_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1920);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_213:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_214:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2048_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2048_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2048);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_215:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2160);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_216:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2304_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2304_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2304);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_217:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2400);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_218:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2560_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2560_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2560);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_219:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2700_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2700_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2700);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_220:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_2880_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_2880_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2880);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_221:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_3000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_3000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_222:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_3072_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_3072_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3072);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_223:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_3200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_3200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3200);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_224:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_3456_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_3456_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3456);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_225:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_3600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_3600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3600);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_226:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_3840_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_3840_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3840);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_227:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_4000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_4000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_228:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_4096_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_4096_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4096);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_229:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_4320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_4320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4320);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_230:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_4500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_4500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4500);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_231:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_4608_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_4608_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4608);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_232:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_4800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_4800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4800);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_233:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_5120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_5120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5120);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_234:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_NOISY_5400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_NOISY_5400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5400);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_235:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_20_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_20_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(20);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_236:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_24_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_24_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(24);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_237:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_32_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_32_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(32);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_238:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_36_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_36_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(36);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_239:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_40_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_40_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(40);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_240:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_48_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_48_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(48);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_241:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_60_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_60_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(60);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_242:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_64_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_64_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(64);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_243:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_72_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_72_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(72);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_244:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_80_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_80_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(80);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_245:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_96_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_96_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(96);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_246:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_100_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_100_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(100);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_247:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_108_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_108_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(108);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_248:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(120);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_249:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_128_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_128_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(128);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_250:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_144_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_144_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(144);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_251:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(160);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_252:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_180_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_180_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(180);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_253:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_192_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_192_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(192);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_254:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(200);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_255:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_216_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_216_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(216);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_256:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_240_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_240_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(240);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_257:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_256_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_256_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(256);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_258:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_288_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_288_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(288);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_259:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_300_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_300_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(300);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_260:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(320);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_261:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_360_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_360_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(360);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_262:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_384_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_384_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(384);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_263:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(400);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_264:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_432_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_432_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(432);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_265:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_480_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_480_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(480);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_266:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(500);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_267:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_512_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_512_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(512);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_268:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_540_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_540_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(540);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_269:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_576_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_576_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(576);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_270:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(600);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_271:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_640_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_640_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(640);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_272:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_720_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_720_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(720);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_273:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_768_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_768_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(768);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_274:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(800);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_275:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_864_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_864_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(864);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_276:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_900_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_900_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(900);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_277:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_960_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_960_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(960);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_278:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_279:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1024_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1024_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1024);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_280:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1080_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1080_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1080);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_281:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1152_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1152_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1152);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_282:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1200);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_283:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1280_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1280_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1280);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_284:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1440_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1440_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1440);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_285:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1500);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_286:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1536_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1536_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1536);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_287:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1600);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_288:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1728_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1728_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1728);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_289:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1800);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_290:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_1920_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_1920_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(1920);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_291:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_292:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2048_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2048_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2048);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_293:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2160_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2160_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2160);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_294:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2304_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2304_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2304);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_295:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2400);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_296:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2560_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2560_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2560);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_297:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2700_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2700_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2700);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_298:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_2880_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_2880_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(2880);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_299:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_3000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_3000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_300:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_3072_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_3072_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3072);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_301:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_3200_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_3200_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3200);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_302:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_3456_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_3456_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3456);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_303:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_3600_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_3600_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3600);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_304:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_3840_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_3840_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(3840);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_305:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_4000_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_4000_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4000);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_306:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_4096_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_4096_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4096);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_307:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_4320_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_4320_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4320);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_308:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_4500_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_4500_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4500);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_309:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_4608_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_4608_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4608);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_310:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_4800_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_4800_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(4800);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_311:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_5120_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_5120_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5120);

            this->ifft=1;
            

          break;


         case TransformNeonCF16::TEST_CFFT_F16_312:

            input.reload(TransformNeonCF16::INPUTS_CIFFT_STEP_5400_F16_ID,mgr);
            ref.reload(  TransformNeonCF16::INPUTS_CFFT_STEP_5400_F16_ID,mgr);

            this->varInstCfftF16Ptr=arm_cfft_init_dynamic_f16(5400);

            this->ifft=1;
            

          break;


       }
       inputfft.create(ref.nbSamples(),TransformNeonCF16::OUTPUT_CFFT_F16_ID,mgr);

       outputfft.create(ref.nbSamples(),TransformNeonCF16::OUTPUT_CFFT_F16_ID,mgr);
       bufferfft.create(ref.nbSamples(),TransformNeonCF16::OUTPUT_CFFT_F16_ID,mgr);
       

    }

    void TransformNeonCF16::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
        if (varInstCfftF16Ptr)
        {
            free(varInstCfftF16Ptr);
        }
    }

#endif
