#ifndef DSP_OP_TESTS_H
#define DSP_OP_TESTS_H

#include "Test.h"
#include "Pattern.h"

class DspOpTests : public Client::Suite
{
public:
    DspOpTests(Testing::testID_t id);
    virtual void setUp(Testing::testID_t id,
                       std::vector<Testing::param_t>& params,
                       Client::PatternMgr *mgr);
    virtual void tearDown(Testing::testID_t id, Client::PatternMgr *mgr);

private:
    #include "DspOpTests_decl.h"

};

#endif
