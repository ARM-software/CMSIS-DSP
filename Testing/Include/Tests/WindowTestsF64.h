#include "Test.h"
#include "Pattern.h"

#include "dsp/window_functions.h"

class WindowTestsF64:public Client::Suite
    {
        public:
            WindowTestsF64(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "WindowTestsF64_decl.h"
            
            Client::LocalPattern<float64_t> output;
            // Reference patterns are not loaded when we are in dump mode
            Client::RefPattern<float64_t> ref;

    };
