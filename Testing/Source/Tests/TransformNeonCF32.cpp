#if defined(ARM_MATH_NEON)
#include "TransformNeonCF32.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"           

#define SNR_THRESHOLD 100
#define REL_ERROR (1.0e-4)
#define ABS_ERROR (2.0e-3)

    void TransformNeonCF32::test_cfft_f32()
    {
       const float32_t *inp = input.ptr();

       float32_t *infftp = inputfft.ptr();

       float32_t *outfftp = outputfft.ptr();

        memcpy(infftp,inp,sizeof(float32_t)*input.nbSamples());

   
        float32_t *bufferp = bufferfft.ptr();
        arm_cfft_f32(
             this->varInstCfftF32Ptr,
             infftp,
             outfftp,
             bufferp,
             this->ifft);

          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        ASSERT_CLOSE_ERROR(outputfft,ref,ABS_ERROR,REL_ERROR);
        ASSERT_EMPTY_TAIL(outputfft);


        
    }

    


   void TransformNeonCF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
   {

       (void)paramsArgs;
       switch(id)
       {


         case TransformNeonCF32::TEST_CFFT_F32_1:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_20_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_20_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(20);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_2:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_24_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_24_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(24);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_3:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_32_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_32_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_4:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_36_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_36_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(36);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_5:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_40_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_40_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(40);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_6:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_48_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_48_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(48);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_7:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_60_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_60_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(60);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_8:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_64_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_64_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(64);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_9:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_72_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_72_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(72);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_10:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_80_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_80_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(80);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_11:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_96_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_96_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(96);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_12:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_100_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_100_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(100);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_13:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_108_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_108_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(108);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_14:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(120);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_15:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_128_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_128_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(128);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_16:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_144_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_144_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(144);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_17:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(160);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_18:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_180_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_180_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(180);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_19:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(192);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_20:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(200);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_21:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_216_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_216_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(216);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_22:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_240_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_240_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(240);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_23:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_256_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_256_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(256);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_24:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_288_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_288_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(288);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_25:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_300_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_300_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(300);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_26:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(320);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_27:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_360_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_360_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(360);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_28:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(384);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_29:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(400);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_30:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_432_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_432_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(432);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_31:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_480_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_480_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(480);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_32:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(500);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_33:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_512_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_512_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(512);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_34:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_540_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_540_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(540);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_35:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_576_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_576_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(576);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_36:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(600);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_37:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_640_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_640_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(640);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_38:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_720_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_720_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(720);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_39:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(768);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_40:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(800);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_41:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_864_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_864_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(864);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_42:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_900_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_900_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(900);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_43:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_960_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_960_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(960);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_44:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_45:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1024_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1024_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1024);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_46:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1080_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1080_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1080);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_47:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1152_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1152_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1152);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_48:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1200);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_49:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1280_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1280_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1280);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_50:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1440_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1440_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1440);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_51:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1500);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_52:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1536_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1536_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1536);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_53:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1600);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_54:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1728_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1728_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1728);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_55:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1800);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_56:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_1920_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_1920_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1920);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_57:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_58:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2048_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2048_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2048);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_59:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2160);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_60:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2304_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2304_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2304);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_61:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2400);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_62:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2560_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2560_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2560);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_63:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2700_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2700_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2700);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_64:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_2880_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_2880_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2880);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_65:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_3000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_3000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_66:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_3072_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_3072_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3072);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_67:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_3200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_3200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3200);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_68:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_3456_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_3456_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3456);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_69:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_3600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_3600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3600);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_70:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_3840_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_3840_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3840);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_71:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_4000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_4000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_72:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_4096_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_4096_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4096);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_73:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_4320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_4320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4320);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_74:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_4500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_4500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4500);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_75:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_4608_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_4608_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4608);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_76:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_4800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_4800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4800);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_77:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_5120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_5120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5120);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_78:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_5400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_5400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5400);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_79:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_8192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_8192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(8192);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_80:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_16384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_16384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(16384);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_81:

            input.reload(TransformNeonCF32::INPUTS_CFFT_NOISY_32768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_NOISY_32768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32768);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_82:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_20_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_20_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(20);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_83:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_24_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_24_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(24);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_84:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_32_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_32_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_85:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_36_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_36_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(36);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_86:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_40_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_40_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(40);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_87:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_48_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_48_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(48);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_88:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_60_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_60_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(60);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_89:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_64_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_64_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(64);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_90:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_72_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_72_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(72);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_91:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_80_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_80_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(80);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_92:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_96_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_96_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(96);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_93:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_100_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_100_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(100);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_94:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_108_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_108_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(108);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_95:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(120);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_96:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_128_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_128_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(128);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_97:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_144_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_144_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(144);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_98:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(160);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_99:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_180_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_180_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(180);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_100:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(192);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_101:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(200);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_102:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_216_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_216_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(216);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_103:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_240_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_240_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(240);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_104:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_256_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_256_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(256);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_105:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_288_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_288_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(288);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_106:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_300_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_300_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(300);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_107:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(320);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_108:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_360_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_360_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(360);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_109:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(384);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_110:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(400);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_111:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_432_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_432_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(432);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_112:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_480_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_480_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(480);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_113:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(500);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_114:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_512_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_512_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(512);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_115:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_540_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_540_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(540);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_116:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_576_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_576_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(576);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_117:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(600);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_118:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_640_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_640_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(640);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_119:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_720_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_720_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(720);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_120:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(768);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_121:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(800);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_122:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_864_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_864_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(864);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_123:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_900_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_900_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(900);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_124:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_960_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_960_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(960);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_125:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_126:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1024_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1024_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1024);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_127:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1080_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1080_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1080);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_128:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1152_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1152_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1152);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_129:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1200);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_130:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1280_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1280_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1280);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_131:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1440_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1440_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1440);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_132:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1500);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_133:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1536_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1536_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1536);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_134:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1600);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_135:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1728_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1728_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1728);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_136:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1800);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_137:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_1920_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_1920_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1920);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_138:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_139:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2048_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2048_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2048);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_140:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2160);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_141:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2304_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2304_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2304);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_142:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2400);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_143:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2560_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2560_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2560);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_144:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2700_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2700_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2700);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_145:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_2880_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_2880_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2880);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_146:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_3000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_3000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_147:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_3072_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_3072_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3072);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_148:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_3200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_3200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3200);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_149:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_3456_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_3456_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3456);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_150:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_3600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_3600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3600);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_151:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_3840_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_3840_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3840);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_152:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_4000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_4000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4000);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_153:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_4096_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_4096_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4096);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_154:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_4320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_4320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4320);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_155:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_4500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_4500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4500);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_156:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_4608_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_4608_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4608);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_157:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_4800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_4800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4800);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_158:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_5120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_5120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5120);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_159:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_5400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_5400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5400);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_160:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_8192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_8192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(8192);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_161:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_16384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_16384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(16384);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_162:

            input.reload(TransformNeonCF32::INPUTS_CFFT_STEP_32768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::REF_CFFT_STEP_32768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32768);

            this->ifft=0;

          break;


         case TransformNeonCF32::TEST_CFFT_F32_163:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_20_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_20_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(20);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_164:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_24_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_24_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(24);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_165:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_32_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_32_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_166:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_36_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_36_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(36);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_167:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_40_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_40_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(40);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_168:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_48_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_48_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(48);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_169:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_60_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_60_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(60);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_170:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_64_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_64_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(64);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_171:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_72_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_72_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(72);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_172:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_80_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_80_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(80);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_173:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_96_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_96_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(96);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_174:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_100_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_100_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(100);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_175:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_108_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_108_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(108);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_176:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(120);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_177:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_128_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_128_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(128);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_178:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_144_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_144_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(144);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_179:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(160);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_180:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_180_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_180_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(180);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_181:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(192);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_182:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(200);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_183:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_216_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_216_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(216);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_184:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_240_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_240_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(240);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_185:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_256_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_256_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(256);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_186:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_288_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_288_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(288);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_187:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_300_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_300_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(300);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_188:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(320);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_189:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_360_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_360_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(360);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_190:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(384);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_191:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(400);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_192:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_432_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_432_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(432);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_193:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_480_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_480_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(480);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_194:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(500);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_195:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_512_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_512_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(512);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_196:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_540_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_540_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(540);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_197:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_576_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_576_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(576);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_198:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(600);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_199:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_640_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_640_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(640);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_200:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_720_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_720_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(720);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_201:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(768);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_202:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(800);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_203:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_864_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_864_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(864);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_204:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_900_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_900_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(900);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_205:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_960_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_960_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(960);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_206:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_207:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1024_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1024_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1024);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_208:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1080_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1080_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1080);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_209:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1152_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1152_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1152);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_210:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1200);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_211:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1280_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1280_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1280);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_212:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1440_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1440_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1440);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_213:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1500);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_214:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1536_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1536_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1536);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_215:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1600);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_216:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1728_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1728_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1728);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_217:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1800);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_218:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_1920_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_1920_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1920);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_219:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_220:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2048_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2048_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2048);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_221:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2160);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_222:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2304_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2304_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2304);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_223:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2400);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_224:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2560_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2560_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2560);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_225:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2700_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2700_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2700);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_226:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_2880_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_2880_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2880);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_227:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_3000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_3000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_228:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_3072_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_3072_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3072);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_229:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_3200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_3200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3200);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_230:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_3456_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_3456_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3456);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_231:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_3600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_3600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3600);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_232:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_3840_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_3840_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3840);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_233:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_4000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_4000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_234:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_4096_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_4096_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4096);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_235:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_4320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_4320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4320);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_236:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_4500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_4500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4500);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_237:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_4608_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_4608_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4608);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_238:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_4800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_4800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4800);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_239:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_5120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_5120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5120);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_240:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_5400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_5400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5400);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_241:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_8192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_8192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(8192);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_242:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_16384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_16384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(16384);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_243:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_NOISY_32768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_NOISY_32768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32768);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_244:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_20_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_20_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(20);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_245:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_24_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_24_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(24);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_246:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_32_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_32_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_247:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_36_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_36_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(36);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_248:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_40_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_40_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(40);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_249:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_48_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_48_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(48);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_250:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_60_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_60_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(60);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_251:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_64_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_64_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(64);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_252:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_72_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_72_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(72);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_253:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_80_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_80_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(80);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_254:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_96_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_96_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(96);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_255:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_100_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_100_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(100);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_256:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_108_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_108_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(108);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_257:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(120);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_258:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_128_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_128_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(128);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_259:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_144_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_144_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(144);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_260:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(160);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_261:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_180_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_180_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(180);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_262:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(192);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_263:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(200);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_264:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_216_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_216_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(216);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_265:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_240_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_240_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(240);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_266:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_256_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_256_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(256);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_267:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_288_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_288_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(288);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_268:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_300_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_300_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(300);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_269:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(320);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_270:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_360_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_360_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(360);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_271:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(384);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_272:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(400);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_273:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_432_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_432_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(432);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_274:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_480_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_480_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(480);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_275:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(500);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_276:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_512_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_512_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(512);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_277:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_540_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_540_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(540);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_278:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_576_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_576_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(576);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_279:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(600);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_280:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_640_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_640_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(640);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_281:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_720_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_720_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(720);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_282:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(768);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_283:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(800);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_284:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_864_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_864_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(864);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_285:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_900_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_900_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(900);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_286:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_960_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_960_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(960);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_287:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_288:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1024_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1024_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1024);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_289:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1080_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1080_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1080);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_290:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1152_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1152_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1152);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_291:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1200);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_292:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1280_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1280_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1280);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_293:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1440_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1440_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1440);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_294:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1500);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_295:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1536_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1536_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1536);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_296:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1600);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_297:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1728_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1728_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1728);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_298:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1800);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_299:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_1920_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_1920_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(1920);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_300:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_301:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2048_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2048_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2048);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_302:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2160_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2160_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2160);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_303:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2304_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2304_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2304);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_304:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2400);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_305:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2560_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2560_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2560);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_306:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2700_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2700_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2700);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_307:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_2880_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_2880_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(2880);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_308:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_3000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_3000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_309:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_3072_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_3072_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3072);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_310:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_3200_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_3200_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3200);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_311:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_3456_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_3456_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3456);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_312:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_3600_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_3600_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3600);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_313:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_3840_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_3840_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(3840);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_314:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_4000_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_4000_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4000);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_315:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_4096_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_4096_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4096);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_316:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_4320_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_4320_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4320);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_317:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_4500_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_4500_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4500);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_318:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_4608_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_4608_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4608);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_319:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_4800_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_4800_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(4800);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_320:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_5120_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_5120_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5120);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_321:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_5400_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_5400_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(5400);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_322:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_8192_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_8192_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(8192);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_323:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_16384_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_16384_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(16384);

            this->ifft=1;
            

          break;


         case TransformNeonCF32::TEST_CFFT_F32_324:

            input.reload(TransformNeonCF32::INPUTS_CIFFT_STEP_32768_F32_ID,mgr);
            ref.reload(  TransformNeonCF32::INPUTS_CFFT_STEP_32768_F32_ID,mgr);

            this->varInstCfftF32Ptr=arm_cfft_init_dynamic_f32(32768);

            this->ifft=1;
            

          break;


       }
       inputfft.create(ref.nbSamples(),TransformNeonCF32::OUTPUT_CFFT_F32_ID,mgr);

       outputfft.create(ref.nbSamples(),TransformNeonCF32::OUTPUT_CFFT_F32_ID,mgr);
       bufferfft.create(ref.nbSamples(),TransformNeonCF32::OUTPUT_CFFT_F32_ID,mgr);
       

    }

    void TransformNeonCF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
        if (varInstCfftF32Ptr)
        {
            free(varInstCfftF32Ptr);
        }
    }

#endif
