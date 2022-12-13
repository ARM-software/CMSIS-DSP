#include "WindowTestsF32.h"
#include <stdio.h>
#include "Error.h"

#define SNR_THRESHOLD 120

/* 

Reference patterns are generated with
a double precision computation.

*/
#define REL_ERROR (1.0e-6)
#define ABS_ERROR (2.0e-6)

void WindowTestsF32::test_welch_f32()
{
        float32_t *outp=output.ptr();

        arm_welch_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_bartlett_f32()
{
        float32_t *outp=output.ptr();

        arm_bartlett_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hamming_f32()
{
        float32_t *outp=output.ptr();

        arm_hamming_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hanning_f32()
{
        float32_t *outp=output.ptr();

        arm_hanning_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_nuttall3_f32()
{
        float32_t *outp=output.ptr();

        arm_nuttall3_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_nuttall4_f32()
{
        float32_t *outp=output.ptr();

        arm_nuttall4_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_nuttall3a_f32()
{
        float32_t *outp=output.ptr();

        arm_nuttall3a_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_nuttall3b_f32()
{
        float32_t *outp=output.ptr();

        arm_nuttall3b_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_nuttall4a_f32()
{
        float32_t *outp=output.ptr();

        arm_nuttall4a_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_blackman_harris_92db_f32()
{
        float32_t *outp=output.ptr();

        arm_blackman_harris_92db_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_nuttall4b_f32()
{
        float32_t *outp=output.ptr();

        arm_nuttall4b_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_nuttall4c_f32()
{
        float32_t *outp=output.ptr();

        arm_nuttall4c_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft90d_f32()
{
        float32_t *outp=output.ptr();

        arm_hft90d_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft95_f32()
{
        float32_t *outp=output.ptr();

        arm_hft95_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft116d_f32()
{
        float32_t *outp=output.ptr();

        arm_hft116d_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft144d_f32()
{
        float32_t *outp=output.ptr();

        arm_hft144d_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft169d_f32()
{
        float32_t *outp=output.ptr();

        arm_hft169d_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft196d_f32()
{
        float32_t *outp=output.ptr();

        arm_hft196d_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft223d_f32()
{
        float32_t *outp=output.ptr();

        arm_hft223d_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF32::test_hft248d_f32()
{
        float32_t *outp=output.ptr();

        arm_hft248d_f32(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 

    
    void WindowTestsF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
      
       (void)params;

       Testing::nbSamples_t nb=MAX_NB_SAMPLES; 

       
       switch(id)
       {

          case WindowTestsF32::TEST_WELCH_F32_1:
            ref.reload(WindowTestsF32::REF1_WELCH_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_BARTLETT_F32_2:
            ref.reload(WindowTestsF32::REF2_BARTLETT_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HAMMING_F32_3:
            ref.reload(WindowTestsF32::REF3_HAMMING_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HANNING_F32_4:
            ref.reload(WindowTestsF32::REF4_HANNING_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_NUTTALL3_F32_5:
            ref.reload(WindowTestsF32::REF5_NUTTALL3_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_NUTTALL4_F32_6:
            ref.reload(WindowTestsF32::REF6_NUTTALL4_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_NUTTALL3A_F32_7:
            ref.reload(WindowTestsF32::REF7_NUTTALL3A_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_NUTTALL3B_F32_8:
            ref.reload(WindowTestsF32::REF8_NUTTALL3B_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_NUTTALL4A_F32_9:
            ref.reload(WindowTestsF32::REF9_NUTTALL4A_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_BLACKMAN_HARRIS_92DB_F32_10:
            ref.reload(WindowTestsF32::REF10_BLACKMAN_HARRIS_92DB_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_NUTTALL4B_F32_11:
            ref.reload(WindowTestsF32::REF11_NUTTALL4B_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_NUTTALL4C_F32_12:
            ref.reload(WindowTestsF32::REF12_NUTTALL4C_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT90D_F32_13:
            ref.reload(WindowTestsF32::REF13_HFT90D_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT95_F32_14:
            ref.reload(WindowTestsF32::REF14_HFT95_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT116D_F32_15:
            ref.reload(WindowTestsF32::REF15_HFT116D_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT144D_F32_16:
            ref.reload(WindowTestsF32::REF16_HFT144D_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT169D_F32_17:
            ref.reload(WindowTestsF32::REF17_HFT169D_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT196D_F32_18:
            ref.reload(WindowTestsF32::REF18_HFT196D_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT223D_F32_19:
            ref.reload(WindowTestsF32::REF19_HFT223D_F32_ID,mgr,nb);
          break;

          case WindowTestsF32::TEST_HFT248D_F32_20:
            ref.reload(WindowTestsF32::REF20_HFT248D_F32_ID,mgr,nb);
          break;

       }
      

       

       output.create(ref.nbSamples(),WindowTestsF32::OUT_F32_ID,mgr);
    }

    void WindowTestsF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        output.dump(mgr);
    }
