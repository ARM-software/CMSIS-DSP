#include "BayesF32.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
#include <cstring>

#define REL_ERROR_SEFR 1.0e-5
#define ABS_ERROR_SEFR 1.0e-5
#define REL_ERROR_LDA 5.0e-3
#define ABS_ERROR_LDA 5.0e-3

    void BayesF32::test_gaussian_naive_bayes_predict_f32()
    {
       const float32_t *inp = input.ptr();

       float32_t *bufp = outputProbas.ptr();
       float32_t *tempp = temp.ptr();
       int16_t *p = outputPredicts.ptr();

       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *p = arm_gaussian_naive_bayes_predict_f32(&bayes, 
                inp, 
                bufp,tempp);

          inp += this->vecDim;
          bufp += this->classNb;
          p++;
       }

        ASSERT_REL_ERROR(outputProbas,probas,5e-6);
        ASSERT_EQ(outputPredicts,predicts);
    } 

    /* Train on the first nbTrain samples, predict the rest, and compare the
       model and the predictions with the float64 reference */
    void BayesF32::train_sefr(bool lda)
    {
       const float32_t *inp = input.ptr();
       const uint32_t *lab = labels.ptr();
       float32_t *scores = outputProbas.ptr();
       int16_t *p = outputPredicts.ptr();
       arm_status status;

       arm_sefr_init_f32(&sefr,this->vecDim,this->classNb,outputWeights.ptr(),outputBias.ptr());

       status = arm_sefr_train_f32(&sefr,inp,lab,this->nbTrain,trainTemp.ptr());
       ASSERT_TRUE(status == ARM_MATH_SUCCESS);

       if (lda)
       {
          status = arm_sefr_lda_f32(&sefr,inp,lab,this->nbTrain,ldaTemp.ptr());
          ASSERT_TRUE(status == ARM_MATH_SUCCESS);
       }

       inp += this->nbTrain * this->vecDim;
       for(int i=0; i < this->nbTest ; i ++)
       {
          *p = (int16_t)arm_sefr_predict_f32(&sefr,inp,scores);
          inp += this->vecDim;
          p++;
       }

       ASSERT_EMPTY_TAIL(trainTemp);
       ASSERT_EMPTY_TAIL(ldaTemp);
       ASSERT_EMPTY_TAIL(outputWeights);
       ASSERT_EMPTY_TAIL(outputBias);
       ASSERT_EMPTY_TAIL(outputProbas);
       ASSERT_EQ(outputPredicts,predicts);
    }

    void BayesF32::test_sefr_train_f32()
    {
       train_sefr(false);

       ASSERT_CLOSE_ERROR(outputWeights,weights,ABS_ERROR_SEFR,REL_ERROR_SEFR);
       ASSERT_CLOSE_ERROR(outputBias,bias,ABS_ERROR_SEFR,REL_ERROR_SEFR);
    }

    void BayesF32::test_sefr_lda_f32()
    {
       train_sefr(true);

       ASSERT_CLOSE_ERROR(outputWeights,weights,ABS_ERROR_LDA,REL_ERROR_LDA);
       ASSERT_CLOSE_ERROR(outputBias,bias,ABS_ERROR_LDA,REL_ERROR_LDA);
    }

    void BayesF32::test_sefr_invalid_f32()
    {
       const float32_t *inp = input.ptr();
       uint32_t *bad = invalidLabels.ptr();
       float32_t *trainp = trainTemp.ptr();
       float32_t *ldap = ldaTemp.ptr();

       arm_sefr_init_f32(&sefr,this->vecDim,this->classNb,outputWeights.ptr(),outputBias.ptr());

       /* No samples */
       ASSERT_TRUE(arm_sefr_train_f32(&sefr,inp,labels.ptr(),0,trainp) == ARM_MATH_ARGUMENT_ERROR);
       ASSERT_TRUE(arm_sefr_lda_f32(&sefr,inp,labels.ptr(),0,ldap) == ARM_MATH_ARGUMENT_ERROR);

       /* All the samples in one class */
       memset(bad,0,this->nbTrain*sizeof(uint32_t));
       ASSERT_TRUE(arm_sefr_train_f32(&sefr,inp,bad,this->nbTrain,trainp) == ARM_MATH_ARGUMENT_ERROR);
       ASSERT_TRUE(arm_sefr_lda_f32(&sefr,inp,bad,this->nbTrain,ldap) == ARM_MATH_ARGUMENT_ERROR);

       /* Label out of range */
       memcpy(bad,labels.ptr(),this->nbTrain*sizeof(uint32_t));
       bad[this->nbTrain-1] = this->classNb;
       ASSERT_TRUE(arm_sefr_train_f32(&sefr,inp,bad,this->nbTrain,trainp) == ARM_MATH_ARGUMENT_ERROR);
       ASSERT_TRUE(arm_sefr_lda_f32(&sefr,inp,bad,this->nbTrain,ldap) == ARM_MATH_ARGUMENT_ERROR);

       ASSERT_EMPTY_TAIL(trainTemp);
       ASSERT_EMPTY_TAIL(ldaTemp);
    }

    void BayesF32::load_sefr(Testing::testID_t dimsId,Testing::testID_t inputsId,Testing::testID_t labelsId,
       Testing::testID_t weightsId,Testing::testID_t biasId,Testing::testID_t predictsId,Client::PatternMgr *mgr)
    {
       dims.reload(dimsId,mgr);
       input.reload(inputsId,mgr);
       labels.reload(labelsId,mgr);

       const int16_t *dimsp=dims.ptr();
       this->nbTrain=dimsp[0];
       this->nbTest=dimsp[1];
       this->classNb=dimsp[2];
       this->vecDim=dimsp[3];

       // Reference patterns are not loaded when we are in dump mode
       weights.reload(weightsId,mgr);
       bias.reload(biasId,mgr);
       predicts.reload(predictsId,mgr);

       outputWeights.create(this->classNb*this->vecDim,BayesF32::OUT_WEIGHTS_F32_ID,mgr);
       outputBias.create(this->classNb,BayesF32::OUT_BIAS_F32_ID,mgr);
       outputPredicts.create(this->nbTest,BayesF32::OUT_PREDICT_S16_ID,mgr);
       outputProbas.create(this->classNb,BayesF32::OUT_PROBA_F32_ID,mgr);
       trainTemp.create(arm_sefr_train_tmp_buffer_size(this->vecDim,this->classNb),BayesF32::OUT_TMP_F32_ID,mgr);
       ldaTemp.create(arm_sefr_lda_tmp_buffer_size(this->classNb),BayesF32::OUT_TMP_F32_ID,mgr);
       invalidLabels.create(this->nbTrain,BayesF32::OUT_LABELS_U32_ID,mgr);
    }

  
    void BayesF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {


       (void)paramsArgs;

       switch(id)
       {
          case BayesF32::TEST_GAUSSIAN_NAIVE_BAYES_PREDICT_F32_1:
          {

            input.reload(BayesF32::INPUTS1_F32_ID,mgr);
            params.reload(BayesF32::PARAMS1_F32_ID,mgr);
            dims.reload(BayesF32::DIMS1_S16_ID,mgr);

            const int16_t *dimsp=dims.ptr();
            const float32_t *paramsp = params.ptr();

            this->nbPatterns=dimsp[0];
            this->classNb=dimsp[1];
            this->vecDim=dimsp[2];

            this->theta=paramsp;
            this->sigma=paramsp + (this->classNb * this->vecDim);
            this->classPrior=paramsp + 2*(this->classNb * this->vecDim);
            this->epsilon=paramsp[this->classNb + 2*(this->classNb * this->vecDim)];
            //printf("%f %f %f\n",this->theta[0],this->sigma[0],this->classPrior[0]);

            // Reference patterns are not loaded when we are in dump mode
            probas.reload(BayesF32::PROBAS1_F32_ID,mgr);
            predicts.reload(BayesF32::PREDICTS1_S16_ID,mgr);

            outputProbas.create(this->nbPatterns*this->classNb,BayesF32::OUT_PROBA_F32_ID,mgr);
            temp.create(this->nbPatterns*this->classNb,BayesF32::OUT_PROBA_F32_ID,mgr);

            outputPredicts.create(this->nbPatterns,BayesF32::OUT_PREDICT_S16_ID,mgr);

            bayes.vectorDimension=this->vecDim;
            bayes.numberOfClasses=this->classNb;
            bayes.theta=this->theta;
            bayes.sigma=this->sigma;
            bayes.classPriors=this->classPrior;
            bayes.epsilon=this->epsilon; 
          }
          break;

          case BayesF32::TEST_SEFR_TRAIN_F32_2:
          case BayesF32::TEST_SEFR_INVALID_F32_6:
            load_sefr(BayesF32::SEFRDIMS3_S16_ID,BayesF32::SEFRINPUTS3_F32_ID,BayesF32::SEFRLABELS3_U32_ID,
                      BayesF32::SEFRWEIGHTS3_F32_ID,BayesF32::SEFRBIAS3_F32_ID,BayesF32::SEFRPREDICTS3_S16_ID,mgr);
          break;

          case BayesF32::TEST_SEFR_TRAIN_F32_3:
            load_sefr(BayesF32::SEFRDIMS4_S16_ID,BayesF32::SEFRINPUTS4_F32_ID,BayesF32::SEFRLABELS4_U32_ID,
                      BayesF32::SEFRWEIGHTS4_F32_ID,BayesF32::SEFRBIAS4_F32_ID,BayesF32::SEFRPREDICTS4_S16_ID,mgr);
          break;

          case BayesF32::TEST_SEFR_LDA_F32_4:
            load_sefr(BayesF32::SEFRDIMS3_S16_ID,BayesF32::SEFRINPUTS3_F32_ID,BayesF32::SEFRLABELS3_U32_ID,
                      BayesF32::SEFRLDAWEIGHTS3_F32_ID,BayesF32::SEFRLDABIAS3_F32_ID,BayesF32::SEFRLDAPREDICTS3_S16_ID,mgr);
          break;

          case BayesF32::TEST_SEFR_LDA_F32_5:
            load_sefr(BayesF32::SEFRDIMS4_S16_ID,BayesF32::SEFRINPUTS4_F32_ID,BayesF32::SEFRLABELS4_U32_ID,
                      BayesF32::SEFRLDAWEIGHTS4_F32_ID,BayesF32::SEFRLDABIAS4_F32_ID,BayesF32::SEFRLDAPREDICTS4_S16_ID,mgr);
          break;

       }
       


    }

    void BayesF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputProbas.dump(mgr);
        outputPredicts.dump(mgr);
    }
