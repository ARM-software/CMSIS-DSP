#include "Test.h"
#include "Pattern.h"
class BasicMathsBenchmarksQ15:public Client::Suite
    {
        public:
            BasicMathsBenchmarksQ15(Testing::testID_t id);
            void setUp(Testing::testID_t,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr);
            void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "BasicMathsBenchmarksQ15_decl.h"
            Client::Pattern<q15_t> input1;
            Client::Pattern<q15_t> input2;
            Client::LocalPattern<q15_t> output;

            int nb;
            
    };