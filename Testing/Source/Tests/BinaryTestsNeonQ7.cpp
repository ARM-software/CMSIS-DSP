#include "BinaryTestsNeonQ7.h"
#include <stdio.h>
#include "Error.h"

#define SNR_THRESHOLD 20

/* 

Reference patterns are generated with
a double precision computation.

*/
#define ABS_ERROR_Q7 ((q7_t)5)

/* Upper bound of maximum matrix dimension used by Python */
#define MAXMATRIXDIM 737

static void checkInnerTail(q7_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
    ASSERT_TRUE(b[2] == 0);
    ASSERT_TRUE(b[3] == 0);
}

#define LOADDATA2()                          \
      const q7_t *inp1=input1.ptr();    \
      const q7_t *inp2=input2.ptr();    \
                                             \
      q7_t *ap=a.ptr();                 \
      q7_t *bp=b.ptr();                 \
                                             \
      q7_t *outp=output.ptr();          \
      q7_t *tmpPtr=tmp.ptr();              \
      int16_t *dimsp = dims.ptr();           \
      int nbMatrixes = dims.nbSamples() / 3;\
      int rows,internal,columns;                      \
      int i;





#define PREPAREDATA2R()                                                   \
      in1.numRows=rows;                                                  \
      in1.numCols=internal;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(q7_t)*rows*internal);\
      in1.pData = ap;                                                    \
                                                                         \
      in2.numRows=internal;                                                  \
      in2.numCols=columns;                                               \
      memcpy((void*)bp,(const void*)inp2,sizeof(q7_t)*internal*columns);\
      in2.pData = bp;                                                    \
                                                                         \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      out.pData = outp;

                                             

    void BinaryTestsNeonQ7::test_mat_mult_q7()
    {     
      LOADDATA2();
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          internal = *dimsp++;
          columns = *dimsp++;

          PREPAREDATA2R();

          status=arm_mat_mult_q7(&this->in1,&this->in2,&this->out,tmpPtr);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);
          checkInnerTail(outp);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(q7_t)SNR_THRESHOLD);

      ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q7);


    } 



    void BinaryTestsNeonQ7::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


      (void)params;
      switch(id)
      {
         case TEST_MAT_MULT_Q7_1:
            input1.reload(BinaryTestsNeonQ7::INPUTS1_Q7_ID,mgr);
            input2.reload(BinaryTestsNeonQ7::INPUTS2_Q7_ID,mgr);
            dims.reload(BinaryTestsNeonQ7::DIMSBINARY1_S16_ID,mgr);

            ref.reload(BinaryTestsNeonQ7::REFMUL1_Q7_ID,mgr);

            output.create(ref.nbSamples(),BinaryTestsNeonQ7::OUT_Q7_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ7::TMPA_Q7_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ7::TMPB_Q7_ID,mgr);
         
            tmp.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ7::TMP_Q7_ID,mgr);

         break;
      }
       

    
    }

    void BinaryTestsNeonQ7::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
       (void)id;
       output.dump(mgr);
    }
