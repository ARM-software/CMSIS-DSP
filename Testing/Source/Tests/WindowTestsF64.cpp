#include "WindowTestsF64.h"
#include <stdio.h>
#include "Error.h"

#define SNR_THRESHOLD 250

/* 

Reference patterns are generated with
a double precision computation.

*/
#define REL_ERROR (3.0e-15)
#define ABS_ERROR (3.0e-15)

void WindowTestsF64::test_welch_f64()
{
        float64_t *outp=output.ptr();

        arm_welch_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_bartlett_f64()
{
        float64_t *outp=output.ptr();

        arm_bartlett_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hamming_f64()
{
        float64_t *outp=output.ptr();

        arm_hamming_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hanning_f64()
{
        float64_t *outp=output.ptr();

        arm_hanning_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_nuttall3_f64()
{
        float64_t *outp=output.ptr();

        arm_nuttall3_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_nuttall4_f64()
{
        float64_t *outp=output.ptr();

        arm_nuttall4_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_nuttall3a_f64()
{
        float64_t *outp=output.ptr();

        arm_nuttall3a_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_nuttall3b_f64()
{
        float64_t *outp=output.ptr();

        arm_nuttall3b_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_nuttall4a_f64()
{
        float64_t *outp=output.ptr();

        arm_nuttall4a_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_blackman_harris_92db_f64()
{
        float64_t *outp=output.ptr();

        arm_blackman_harris_92db_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_nuttall4b_f64()
{
        float64_t *outp=output.ptr();

        arm_nuttall4b_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_nuttall4c_f64()
{
        float64_t *outp=output.ptr();

        arm_nuttall4c_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft90d_f64()
{
        float64_t *outp=output.ptr();

        arm_hft90d_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft95_f64()
{
        float64_t *outp=output.ptr();

        arm_hft95_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft116d_f64()
{
        float64_t *outp=output.ptr();

        arm_hft116d_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft144d_f64()
{
        float64_t *outp=output.ptr();

        arm_hft144d_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft169d_f64()
{
        float64_t *outp=output.ptr();

        arm_hft169d_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft196d_f64()
{
        float64_t *outp=output.ptr();

        arm_hft196d_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft223d_f64()
{
        float64_t *outp=output.ptr();

        arm_hft223d_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 
void WindowTestsF64::test_hft248d_f64()
{
        float64_t *outp=output.ptr();

        arm_hft248d_f64(outp,output.nbSamples());

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

        ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

} 

    
    void WindowTestsF64::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
      
       (void)params;

       Testing::nbSamples_t nb=MAX_NB_SAMPLES; 

       
       switch(id)
       {

          case WindowTestsF64::TEST_WELCH_F64_1:
            ref.reload(WindowTestsF64::REF1_WELCH_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_BARTLETT_F64_2:
            ref.reload(WindowTestsF64::REF2_BARTLETT_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HAMMING_F64_3:
            ref.reload(WindowTestsF64::REF3_HAMMING_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HANNING_F64_4:
            ref.reload(WindowTestsF64::REF4_HANNING_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_NUTTALL3_F64_5:
            ref.reload(WindowTestsF64::REF5_NUTTALL3_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_NUTTALL4_F64_6:
            ref.reload(WindowTestsF64::REF6_NUTTALL4_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_NUTTALL3A_F64_7:
            ref.reload(WindowTestsF64::REF7_NUTTALL3A_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_NUTTALL3B_F64_8:
            ref.reload(WindowTestsF64::REF8_NUTTALL3B_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_NUTTALL4A_F64_9:
            ref.reload(WindowTestsF64::REF9_NUTTALL4A_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_BLACKMAN_HARRIS_92DB_F64_10:
            ref.reload(WindowTestsF64::REF10_BLACKMAN_HARRIS_92DB_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_NUTTALL4B_F64_11:
            ref.reload(WindowTestsF64::REF11_NUTTALL4B_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_NUTTALL4C_F64_12:
            ref.reload(WindowTestsF64::REF12_NUTTALL4C_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT90D_F64_13:
            ref.reload(WindowTestsF64::REF13_HFT90D_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT95_F64_14:
            ref.reload(WindowTestsF64::REF14_HFT95_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT116D_F64_15:
            ref.reload(WindowTestsF64::REF15_HFT116D_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT144D_F64_16:
            ref.reload(WindowTestsF64::REF16_HFT144D_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT169D_F64_17:
            ref.reload(WindowTestsF64::REF17_HFT169D_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT196D_F64_18:
            ref.reload(WindowTestsF64::REF18_HFT196D_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT223D_F64_19:
            ref.reload(WindowTestsF64::REF19_HFT223D_F64_ID,mgr,nb);
          break;

          case WindowTestsF64::TEST_HFT248D_F64_20:
            ref.reload(WindowTestsF64::REF20_HFT248D_F64_ID,mgr,nb);
          break;

       }
      

       

       output.create(ref.nbSamples(),WindowTestsF64::OUT_F64_ID,mgr);
    }

    void WindowTestsF64::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        output.dump(mgr);
    }
