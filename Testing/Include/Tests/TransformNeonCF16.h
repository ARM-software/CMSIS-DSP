#if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)

#include "Test.h"
#include "Pattern.h"

#include "dsp/transform_functions_f16.h"

class TransformNeonCF16:public Client::Suite
    {
        public:
            TransformNeonCF16(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "TransformNeonCF16_decl.h"
            
            Client::Pattern<float16_t> input;
            Client::LocalPattern<float16_t> outputfft;
            Client::LocalPattern<float16_t> bufferfft;

            Client::LocalPattern<float16_t> inputfft;

            Client::RefPattern<float16_t> ref;

            arm_cfft_instance_f16 *varInstCfftF16Ptr;

            int ifft;

            arm_status status;
            
    };
#endif