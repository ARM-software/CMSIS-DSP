#if defined(ARM_MATH_NEON)

#include "Test.h"
#include "Pattern.h"

#include "dsp/transform_functions.h"

class TransformNeonCF32:public Client::Suite
    {
        public:
            TransformNeonCF32(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "TransformNeonCF32_decl.h"
            
            Client::Pattern<float32_t> input;
            Client::LocalPattern<float32_t> outputfft;
            Client::LocalPattern<float32_t> bufferfft;

            Client::LocalPattern<float32_t> inputfft;

            Client::RefPattern<float32_t> ref;

            arm_cfft_instance_f32 *varInstCfftF32Ptr;

            int ifft;

            arm_status status;
            
    };
#endif