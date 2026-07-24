#include "DspOpTests.h"
#include "dsp/none.h"
#include <stdio.h>
#include "Error.h"

void DspOpTests::test_smlad_sat()
{
    const q31_t *x = inputAQ31.ptr();
    const q31_t *y = inputBQ31.ptr();
    const q63_t *acc = inputSMLALDAcc.ptr();
    q63_t *outp = outputQ63.ptr();

    for (unsigned long i = 0; i < refSMLALD.nbSamples(); i++)
    {
        outp[i] = (q63_t)__SMLALD((uint32_t)x[i],
                                  (uint32_t)y[i],
                                  (uint64_t)acc[i]);
    }

    ASSERT_EQ(outputQ63, refSMLALD);
}

void DspOpTests::test_smlaldx_sat()
{
    const q31_t *x = inputAQ31.ptr();
    const q31_t *y = inputBQ31.ptr();
    const q63_t *acc = inputSMLALDAcc.ptr();
    q63_t *outp = outputQ63.ptr();

    for (unsigned long i = 0; i < refSMLALDX.nbSamples(); i++)
    {
        outp[i] = (q63_t)__SMLALDX((uint32_t)x[i],
                                   (uint32_t)y[i],
                                   (uint64_t)acc[i]);
    }

    ASSERT_EQ(outputQ63, refSMLALDX);
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
    const q31_t *inp  = inputAQ31.ptr();
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
        case DspOpTests::TEST_SMLAD_SAT_1:
        {
               inputAQ31.reload(DspOpTests::INPUTA_Q31_ID,mgr);
               inputBQ31.reload(DspOpTests::INPUTB_Q31_ID,mgr);
               inputSMLALDAcc.reload(DspOpTests::INPUT_SMLALD_ACC_ID,mgr);
               refSMLALD.reload(DspOpTests::REF_SMLALD_ID,mgr);
               outputQ63.create(refSMLALD.nbSamples(),DspOpTests::OUT_Q63_ID,mgr);
        }
        break;


        case DspOpTests::TEST_SMLALDX_SAT_2:
        {
               inputAQ31.reload(DspOpTests::INPUTA_Q31_ID,mgr);
               inputBQ31.reload(DspOpTests::INPUTB_Q31_ID,mgr);
               inputSMLALDAcc.reload(DspOpTests::INPUT_SMLALD_ACC_ID,mgr);
               refSMLALDX.reload(DspOpTests::REF_SMLALDX_ID,mgr);
               outputQ63.create(refSMLALDX.nbSamples(),DspOpTests::OUT_Q63_ID,mgr);
        }
        break;


        case DspOpTests::TEST_CLIP_Q63_TO_Q31_3:
        {
               inputQ63.reload(DspOpTests::INPUT_CLIP_Q63_ID,mgr);
               refQ31.reload(DspOpTests::REF_CLIP_Q31_ID,mgr);
               outputQ31.create(refQ31.nbSamples(),DspOpTests::OUT_Q31_ID,mgr);
        }
        break;


        case DspOpTests::TEST_CLIP_Q31_TO_Q15_4:
        {
               inputAQ31.reload(DspOpTests::INPUTA_Q31_ID,mgr);
               refQ15.reload(DspOpTests::REF_CLIP_Q15_ID,mgr);
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
