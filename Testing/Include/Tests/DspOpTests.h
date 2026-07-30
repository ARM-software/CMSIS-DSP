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

    Client::Pattern<q63_t> inputAQ63;
    Client::Pattern<q31_t> inputAQ31;
    Client::Pattern<q31_t> inputBQ31;
    Client::Pattern<q31_t> inputCQ31;
    Client::Pattern<q15_t> inputAQ15;

    Client::LocalPattern<q31_t> outputAQ31;
    Client::LocalPattern<q31_t> outputBQ31;
    Client::LocalPattern<q15_t> outputAQ15;
    Client::LocalPattern<q15_t> outputBQ15;
    Client::LocalPattern<q63_t> outputQ63;

    Client::RefPattern<q31_t> refAQ31;
    Client::RefPattern<q31_t> refBQ31;
    Client::RefPattern<q15_t> refAQ15;
    Client::RefPattern<q15_t> refBQ15;
    Client::RefPattern<q63_t> refQ63;

};

#endif
