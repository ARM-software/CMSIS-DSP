#if defined(ARM_MATH_NEON)

#include "Test.h"
#include "Pattern.h"

#include "dsp/transform_functions.h"

class TransformNeonCQ31:public Client::Suite
    {
        public:
            TransformNeonCQ31(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "TransformNeonCQ31_decl.h"
            
            Client::Pattern<q31_t> input;
            Client::LocalPattern<q31_t> outputfft;
            Client::LocalPattern<q31_t> bufferfft;

            Client::LocalPattern<q31_t> inputfft;
            Client::LocalPattern<q31_t> tmp2;

            Client::RefPattern<q31_t> ref;

            arm_cfft_instance_q31 *varInstCfftQ31Ptr;

            uint8_t ifft;

            /*  

            ifft pattern is using the output of the fft and the input of the fft.
            Since output of the fft is scaled, the input is not recovered without an additional scaling.


            */
            int scaling;

            arm_status status;
            
    };
#endif