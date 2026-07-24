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

    Client::Pattern<q63_t> inputQ63;
    Client::Pattern<q31_t> inputQ31;
    Client::Pattern<q31_t> inputSMLALDX;
    Client::Pattern<q31_t> inputSMLALDY;
    Client::Pattern<q63_t> inputSMLALDAcc;

    Client::LocalPattern<q31_t> outputQ31;
    Client::LocalPattern<q15_t> outputQ15;
    Client::LocalPattern<q7_t> outputQ7;
    Client::LocalPattern<q63_t> outputQ63;

    Client::RefPattern<q31_t> refQ31;
    Client::RefPattern<q15_t> refQ15;
    Client::RefPattern<q7_t> refQ7;
    Client::RefPattern<q63_t> refSMLALD;
    Client::RefPattern<q63_t> refSMLALDX;

};

#endif
