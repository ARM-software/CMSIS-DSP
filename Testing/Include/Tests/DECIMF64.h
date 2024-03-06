#include "Test.h"
#include "Pattern.h"

#include "dsp/filtering_functions.h"

class DECIMF64:public Client::Suite
    {
        public:
            DECIMF64(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "DECIMF64_decl.h"
            
            Client::Pattern<float64_t> input;
            Client::Pattern<float64_t> coefs;
            Client::Pattern<uint32_t> config;

            Client::LocalPattern<float64_t> output;
            Client::LocalPattern<float64_t> state;
            // Reference patterns are not loaded when we are in dump mode
            Client::RefPattern<float64_t> ref;


            arm_fir_decimate_instance_f64 S;
            //arm_fir_interpolate_instance_f64 SI;

            int q;
            int numTaps;
            int blocksize;
            int refsize;

            arm_status status;
    };
