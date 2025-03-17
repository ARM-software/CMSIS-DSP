#include "BinaryTestsNeonF16.h"
#include <stdio.h>
#include "Error.h"

// We are testing with big matrixes that are too big for f16 datatype
// so accuracy is not so great (if the test was limited to smaller matrixes
// the test thresholds could be better)
#define SNR_THRESHOLD 45
/* 

Reference patterns are generated with
a double precision computation.

*/
#define REL_ERROR (3.0e-3)
#define ABS_ERROR (5.0e-2)

/* Upper bound of maximum matrix dimension used by Python */
#define MAXMATRIXDIM 1537

static void checkInnerTail(float16_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
    ASSERT_TRUE(b[2] == 0);
    ASSERT_TRUE(b[3] == 0);
}

#define LOADDATA2()                          \
      const float16_t *inp1=input1.ptr();    \
      const float16_t *inp2=input2.ptr();    \
                                             \
      float16_t *ap=a.ptr();                 \
      float16_t *bp=b.ptr();                 \
                                             \
      float16_t *outp=output.ptr();          \
      int16_t *dimsp = dims.ptr();           \
      int nbMatrixes = dims.nbSamples() / 3;\
      int rows,internal,columns;                      \
      int i;





#define PREPAREDATA2R()                                                   \
      in1.numRows=rows;                                                  \
      in1.numCols=internal;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(float16_t)*rows*internal);\
      in1.pData = ap;                                                    \
                                                                         \
      in2.numRows=internal;                                                  \
      in2.numCols=columns;                                               \
      memcpy((void*)bp,(const void*)inp2,sizeof(float16_t)*internal*columns);\
      in2.pData = bp;                                                    \
                                                                         \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      out.pData = outp;

                                             

    void BinaryTestsNeonF16::test_mat_mult_f16()
    {     
      LOADDATA2();
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          internal = *dimsp++;
          columns = *dimsp++;

          PREPAREDATA2R();

          status=arm_mat_mult_f16(&this->in1,&this->in2,&this->out);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);
          checkInnerTail(outp);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

      ASSERT_SNR(output,ref,(float16_t)SNR_THRESHOLD);


    } 



    void BinaryTestsNeonF16::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


      (void)params;
      switch(id)
      {
         case TEST_MAT_MULT_F16_1:
            input1.reload(BinaryTestsNeonF16::INPUTS1_F16_ID,mgr);
            input2.reload(BinaryTestsNeonF16::INPUTS2_F16_ID,mgr);
            dims.reload(BinaryTestsNeonF16::DIMSBINARY1_S16_ID,mgr);

            ref.reload(BinaryTestsNeonF16::REFMUL1_F16_ID,mgr);

            output.create(ref.nbSamples(),BinaryTestsNeonF16::OUT_F16_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonF16::TMPA_F16_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonF16::TMPB_F16_ID,mgr);
         break;
      }
       

    
    }

    void BinaryTestsNeonF16::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
       (void)id;
       output.dump(mgr);
    }
