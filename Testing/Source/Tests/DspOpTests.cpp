#include "DspOpTests.h"
#include "dsp/none.h"
#include <stdio.h>
#include "Error.h"

void DspOpTests::test_smlad_sat()
{
    uint32_t x = 0x80008000u, y = 0x80008000u; 
    uint32_t res = __SMLALD(x, y, 0);
   
    ASSERT_TRUE(res == 2147483648);
}

void DspOpTests::test_clip_q63_to_q31()
{
    const q63_t *inp  = inputQ63.ptr();
    q31_t *outp  = outputQ31.ptr();
    unsigned long i;
    arm_status status=ARM_MATH_SUCCESS;

    for(i=0; i < refQ31.nbSamples(); i++)
    {
          outp[i] = clip_q63_to_q31(inp[i]);
    }

    ASSERT_EQ(outputQ31,refQ31);
}



void DspOpTests::test_clip_q31_to_q15()
{
    const q31_t *inp  = inputQ31.ptr();
    q15_t *outp  = outputQ15.ptr();
    unsigned long i;
    arm_status status=ARM_MATH_SUCCESS;

    for(i=0; i < refQ15.nbSamples(); i++)
    {
          outp[i] = clip_q31_to_q15(inp[i]);
    }

    ASSERT_EQ(outputQ15,refQ15);
}


void DspOpTests::setUp(Testing::testID_t id,
                       std::vector<Testing::param_t>& params,
                       Client::PatternMgr *mgr)
{
    (void)params;
    (void)mgr;

    switch (id)
    {
        case DspOpTests::TEST_CLIP_Q63_TO_Q31_2:
        {
               inputQ63.reload(DspOpTests::INPUT_Q63_ID,mgr);
               refQ31.reload(DspOpTests::REF_Q31_ID,mgr);
               outputQ31.create(refQ31.nbSamples(),DspOpTests::OUT_Q31_ID,mgr);
        }
        break;


        case DspOpTests::TEST_CLIP_Q31_TO_Q15_3:
        {
               inputQ31.reload(DspOpTests::INPUT_Q31_ID,mgr);
               refQ15.reload(DspOpTests::REF_Q15_ID,mgr);
               outputQ15.create(refQ15.nbSamples(),DspOpTests::OUT_Q15_ID,mgr);
        }
        break;


        default:
            break;
    }
}

void DspOpTests::tearDown(Testing::testID_t id, Client::PatternMgr *mgr)
{
    (void)id;
    (void)mgr;
}
