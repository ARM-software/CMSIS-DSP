#include "BinaryTestsNeonQ31.h"
#include <stdio.h>
#include "Error.h"

#include "dsp/basic_math_functions.h"

#define SNR_THRESHOLD 75

/* 

Reference patterns are generated with
a double precision computation.

The high value is because we use the same threshold for all matrixes : small ones
and very big ones. The Q31 implementation requires a shift of input vector to avoid
saturations. When the matrixes are big, this shift is bigger and thus there is a loss of
accuracy for big matrixes.

So this threshold does not reflect the accuracy for more reasonable sizes.
*/
#define ABS_ERROR_Q31 ((q31_t)900000)

/* Upper bound of maximum matrix dimension used by Python */
#define MAXMATRIXDIM 1537

static void checkInnerTail(q31_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
    ASSERT_TRUE(b[2] == 0);
    ASSERT_TRUE(b[3] == 0);
}

#define LOADDATA2()                          \
      const q31_t *inp1=input1.ptr();    \
      const q31_t *inp2=input2.ptr();    \
                                             \
      q31_t *ap=a.ptr();                 \
      q31_t *bp=b.ptr();                 \
                                             \
      q31_t *outp=output.ptr();          \
      int16_t *dimsp = dims.ptr();           \
      int nbMatrixes = dims.nbSamples() / 4;\
      int rows,internal,columns,shift;                      \
      int i;





#define PREPAREDATA2R()                                                   \
      in1.numRows=rows;                                                  \
      in1.numCols=internal;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(q31_t)*rows*internal);\
      in1.pData = ap;                                                    \
                                                                         \
      in2.numRows=internal;                                                  \
      in2.numCols=columns;                                               \
      memcpy((void*)bp,(const void*)inp2,sizeof(q31_t)*internal*columns);\
      in2.pData = bp;                                                    \
                                                                         \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      out.pData = outp;

                                             

    void BinaryTestsNeonQ31::test_mat_mult_q31()
    {     
      LOADDATA2();
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          internal = *dimsp++;
          columns = *dimsp++;
          shift = *dimsp++;

          PREPAREDATA2R();

          if (shift!=0)
             arm_shift_q31(inp1,-shift,ap,rows*internal);
          status=arm_mat_mult_q31(&this->in1,&this->in2,&this->out);
          if (shift!=0)
             arm_shift_q31(outp,shift,outp,rows*columns);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);
          checkInnerTail(outp);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

      ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q31);


    } 



    void BinaryTestsNeonQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


      (void)params;
      switch(id)
      {
         case TEST_MAT_MULT_Q31_1:
            input1.reload(BinaryTestsNeonQ31::INPUTS1_Q31_ID,mgr);
            input2.reload(BinaryTestsNeonQ31::INPUTS2_Q31_ID,mgr);
            dims.reload(BinaryTestsNeonQ31::DIMSBINARY1_S16_ID,mgr);

            ref.reload(BinaryTestsNeonQ31::REFMUL1_Q31_ID,mgr);

            output.create(ref.nbSamples(),BinaryTestsNeonQ31::OUT_Q31_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ31::TMPA_Q31_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ31::TMPB_Q31_ID,mgr);
         break;
      }
       

    
    }

    void BinaryTestsNeonQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
       (void)id;
       output.dump(mgr);
    }
