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


void DspOpTests::setUp(Testing::testID_t id,
                       std::vector<Testing::param_t>& params,
                       Client::PatternMgr *mgr)
{
    (void)params;
    (void)mgr;

    switch (id)
    {
        case DspOpTests::TEST_SMLAD_SAT_1:
            break;
    }
}

void DspOpTests::tearDown(Testing::testID_t id, Client::PatternMgr *mgr)
{
    (void)id;
    (void)mgr;
}
