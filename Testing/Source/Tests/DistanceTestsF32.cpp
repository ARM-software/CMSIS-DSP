#include "DistanceTestsF32.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"



    void DistanceTestsF32::test_dtw_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();
       arm_matrix_instance_f32 distances;
       arm_matrix_instance_f32 costs;
       arm_matrix_instance_q7 window;

       distances.numRows=this->queryLength;
       distances.numCols=this->templateLength;
       distances.pData = tmpA.ptr();

       costs.numRows=this->queryLength;
       costs.numCols=this->templateLength;
       costs.pData = tmpB.ptr();

       window.numRows=this->queryLength;
       window.numCols=this->templateLength;
       window.pData = tmpC.ptr();


       float32_t *outp = output.ptr();
       int16_t *outPathp = outPath.ptr();
       uint32_t pathLength;

       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          float32_t *c = distances.pData;
          for(int q=0; q < this->queryLength; q++)
          {
              for(int t=0; t < this->templateLength; t++)
              {
                  *c = fabs(inpA[q] - inpB[t]);
                  c++;
              }
          }

          
          arm_status status = arm_dtw_distance_f32(&distances, NULL,&costs,outp);
          outp++;
          ASSERT_TRUE(status == ARM_MATH_SUCCESS);

          arm_dtw_path_f32(&costs,outPathp,&pathLength);

          /* ARM_DTW_SAKOE_CHIBA_WINDOW 5*/
          status = arm_dtw_init_window_q7(ARM_DTW_SAKOE_CHIBA_WINDOW,5,&window);
          ASSERT_TRUE(status == ARM_MATH_SUCCESS);

          c = distances.pData;
          for(int q=0; q < this->queryLength; q++)
          {
              for(int t=0; t < this->templateLength; t++)
              {
                  /* Distance does not have
                     to be computed outside of
                     the window */
                  if (window.pData[q*this->templateLength+t])
                  {
                     *c = fabs(inpA[q] - inpB[t]);
                  }
                  c++;
              }
          }

          status = arm_dtw_distance_f32(&distances, &window,&costs,outp);
          ASSERT_TRUE(status == ARM_MATH_SUCCESS);
          outp++;

          /* ARM_DTW_SAKOE_CHIBA_WINDOW 3 */
          status = arm_dtw_init_window_q7(ARM_DTW_SAKOE_CHIBA_WINDOW,3,&window);
          ASSERT_TRUE(status == ARM_MATH_SUCCESS);
          status = arm_dtw_distance_f32(&distances, &window,&costs,outp);
          ASSERT_TRUE(status == ARM_MATH_ARGUMENT_ERROR);

          /* ARM_DTW_SLANTED_BAND_WINDOW 1*/
          status = arm_dtw_init_window_q7(ARM_DTW_SLANTED_BAND_WINDOW,1,&window);
          ASSERT_TRUE(status == ARM_MATH_SUCCESS);
          /* Here again we could compute the distance matrix
          only on a subset */

          status = arm_dtw_distance_f32(&distances, &window,&costs,outp);
          ASSERT_TRUE(status == ARM_MATH_SUCCESS);
          outp++;

         
          inpA += this->queryLength;
          inpB += this->templateLength;
          outPathp += 2*pathLength;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
        ASSERT_EQ_PARTIAL(2*pathLength,outPath,refPath);

    }

    void DistanceTestsF32::test_braycurtis_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_braycurtis_distance_f32(inpA, inpB, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 
 
    void DistanceTestsF32::test_canberra_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_canberra_distance_f32(inpA, inpB, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 

    void DistanceTestsF32::test_chebyshev_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_chebyshev_distance_f32(inpA, inpB, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 

    void DistanceTestsF32::test_cityblock_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_cityblock_distance_f32(inpA, inpB, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 

    void DistanceTestsF32::test_correlation_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *tmpap = tmpA.ptr();
       float32_t *tmpbp = tmpB.ptr();

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          memcpy(tmpap, inpA, sizeof(float32_t) * this->vecDim);
          memcpy(tmpbp, inpB, sizeof(float32_t) * this->vecDim);
          
          *outp = arm_correlation_distance_f32(tmpap, tmpbp, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 

    void DistanceTestsF32::test_cosine_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_cosine_distance_f32(inpA, inpB, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 

    void DistanceTestsF32::test_euclidean_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_euclidean_distance_f32(inpA, inpB, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 

    void DistanceTestsF32::test_jensenshannon_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();

       float32_t *outp = output.ptr();

      
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_jensenshannon_distance_f32(inpA, inpB, this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 

    void DistanceTestsF32::test_minkowski_distance_f32()
    {
       const float32_t *inpA = inputA.ptr();
       const float32_t *inpB = inputB.ptr();
       const int16_t   *dimsp= dims.ptr();
       dimsp += 2;

       float32_t *outp = output.ptr();
       
       for(int i=0; i < this->nbPatterns ; i ++)
       {
          *outp = arm_minkowski_distance_f32(inpA, inpB, *dimsp,this->vecDim);
         
          inpA += this->vecDim;
          inpB += this->vecDim;
          outp ++;
          dimsp ++;
       }

        ASSERT_NEAR_EQ(output,ref,(float32_t)1e-3);
    } 
  
  
    void DistanceTestsF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

        (void)paramsArgs;
        if ((id != DistanceTestsF32::TEST_MINKOWSKI_DISTANCE_F32_9) && (id != DistanceTestsF32::TEST_JENSENSHANNON_DISTANCE_F32_8))
        {
            inputA.reload(DistanceTestsF32::INPUTA_F32_ID,mgr);
            inputB.reload(DistanceTestsF32::INPUTB_F32_ID,mgr);
            dims.reload(DistanceTestsF32::DIMS_S16_ID,mgr);
            
            const int16_t   *dimsp = dims.ptr();
            
            this->nbPatterns=dimsp[0];
            this->vecDim=dimsp[1];
            output.create(this->nbPatterns,DistanceTestsF32::OUT_F32_ID,mgr);
        }

        switch(id)
        {
            case DistanceTestsF32::TEST_BRAYCURTIS_DISTANCE_F32_1:
            {
              ref.reload(DistanceTestsF32::REF1_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_CANBERRA_DISTANCE_F32_2:
            {
              ref.reload(DistanceTestsF32::REF2_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_CHEBYSHEV_DISTANCE_F32_3:
            {
              ref.reload(DistanceTestsF32::REF3_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_CITYBLOCK_DISTANCE_F32_4:
            {
              ref.reload(DistanceTestsF32::REF4_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_CORRELATION_DISTANCE_F32_5:
            {
              ref.reload(DistanceTestsF32::REF5_F32_ID,mgr);
              tmpA.create(this->vecDim,DistanceTestsF32::TMPA_F32_ID,mgr);
              tmpB.create(this->vecDim,DistanceTestsF32::TMPB_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_COSINE_DISTANCE_F32_6:
            {
              ref.reload(DistanceTestsF32::REF6_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_EUCLIDEAN_DISTANCE_F32_7:
            {
              ref.reload(DistanceTestsF32::REF7_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_JENSENSHANNON_DISTANCE_F32_8:
            {
              inputA.reload(DistanceTestsF32::INPUTA_JEN_F32_ID,mgr);
              inputB.reload(DistanceTestsF32::INPUTB_JEN_F32_ID,mgr);
              dims.reload(DistanceTestsF32::DIMS_S16_ID,mgr);
              
              const int16_t   *dimsp = dims.ptr();
              
              this->nbPatterns=dimsp[0];
              this->vecDim=dimsp[1];
              output.create(this->nbPatterns,DistanceTestsF32::OUT_F32_ID,mgr);

              ref.reload(DistanceTestsF32::REF8_F32_ID,mgr);
            }
            break;

            case DistanceTestsF32::TEST_MINKOWSKI_DISTANCE_F32_9:
            {
              inputA.reload(DistanceTestsF32::INPUTA_F32_ID,mgr);
              inputB.reload(DistanceTestsF32::INPUTB_F32_ID,mgr);
              dims.reload(DistanceTestsF32::DIMS_MINKOWSKI_S16_ID,mgr);
              
              const int16_t   *dimsp = dims.ptr();
              
              this->nbPatterns=dimsp[0];
              this->vecDim=dimsp[1];
              output.create(this->nbPatterns,DistanceTestsF32::OUT_F32_ID,mgr);

              ref.reload(DistanceTestsF32::REF9_F32_ID,mgr);
            }
            break;

         case DistanceTestsF32::TEST_DTW_DISTANCE_F32_10:
            {
              inputA.reload(DistanceTestsF32::INPUT_QUERY_F32_ID,mgr);
              inputB.reload(DistanceTestsF32::INPUT_TEMPLATE_F32_ID,mgr);
              
              
              this->nbPatterns=1;
              this->queryLength=inputA.nbSamples();
              this->templateLength=inputB.nbSamples();
              output.create(3*this->nbPatterns,DistanceTestsF32::OUT_F32_ID,mgr);
              tmpA.create(this->queryLength*this->templateLength,DistanceTestsF32::TMPA_F32_ID,mgr);
              tmpB.create(this->queryLength*this->templateLength,DistanceTestsF32::TMPB_F32_ID,mgr);
              tmpC.create(this->queryLength*this->templateLength,DistanceTestsF32::TMPC_Q7_ID,mgr);

              outPath.create(2*(this->queryLength+this->templateLength),DistanceTestsF32::OUTA_S16_ID,mgr);

              ref.reload(DistanceTestsF32::REF10_F32_ID,mgr);
              refPath.reload(DistanceTestsF32::REF10_S16_PATH_ID,mgr);

            }
            break;

        }

       

       

    }

    void DistanceTestsF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
       (void)id;
       output.dump(mgr);
    }
