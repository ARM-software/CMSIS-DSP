#include "Test.h"
#include "Pattern.h"

#include "dsp/bayes_functions.h"

class BayesF32:public Client::Suite
    {
        public:
            BayesF32(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "BayesF32_decl.h"
            
            Client::Pattern<float32_t> input;
            Client::Pattern<float32_t> params;
            Client::Pattern<int16_t> dims;
            Client::Pattern<uint32_t> labels;

            Client::LocalPattern<float32_t> outputProbas;
            Client::LocalPattern<float32_t> temp;
            Client::LocalPattern<int16_t> outputPredicts;
            Client::LocalPattern<float32_t> outputWeights;
            Client::LocalPattern<float32_t> outputBias;
            Client::LocalPattern<float32_t> trainTemp;
            Client::LocalPattern<float32_t> ldaTemp;
            Client::LocalPattern<uint32_t> invalidLabels;

            // Reference patterns are not loaded when we are in dump mode
            Client::RefPattern<float32_t> probas;
            Client::RefPattern<int16_t> predicts;
            Client::RefPattern<float32_t> weights;
            Client::RefPattern<float32_t> bias;

            int nbPatterns,classNb,vecDim;
            const float32_t *theta;
            const float32_t *sigma;
            const float32_t *classPrior;
            float32_t epsilon;

            arm_gaussian_naive_bayes_instance_f32 bayes;

            int nbTrain,nbTest;
            arm_sefr_instance_f32 sefr;

            void train_sefr(bool lda);
            void load_sefr(Testing::testID_t dimsId,Testing::testID_t inputsId,Testing::testID_t labelsId,
               Testing::testID_t weightsId,Testing::testID_t biasId,Testing::testID_t predictsId,Client::PatternMgr *mgr);

    };
