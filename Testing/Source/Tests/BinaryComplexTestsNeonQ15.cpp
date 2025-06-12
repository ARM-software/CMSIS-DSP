#include "BinaryComplexTestsNeonQ15.h"
#include <stdio.h>
#include "Error.h"

#include "dsp/basic_math_functions.h"

#define SNR_THRESHOLD 75

/* 

Reference patterns are generated with
a double precision computation.

The high value is because we use the same threshold for all matrixes : small ones
and very big ones. The Q15 implementation requires a shift of input vector to avoid
saturations. When the matrixes are big, this shift is bigger and thus there is a loss of
accuracy for big matrixes.

So this threshold does not reflect the accuracy for more reasonable sizes.
*/
#define ABS_ERROR_Q15 ((q15_t)12)

/* Upper bound of maximum matrix dimension used by Python */
#define MAXMATRIXDIM 1537

static void checkInnerTail(q15_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
    ASSERT_TRUE(b[2] == 0);
    ASSERT_TRUE(b[3] == 0);
    ASSERT_TRUE(b[4] == 0);
    ASSERT_TRUE(b[5] == 0);
    ASSERT_TRUE(b[6] == 0);
    ASSERT_TRUE(b[7] == 0);
}

#define LOADDATA2()                          \
      const q15_t *inp1=input1.ptr();    \
      const q15_t *inp2=input2.ptr();    \
                                             \
      q15_t *ap=a.ptr();                 \
      q15_t *bp=b.ptr();                 \
                                             \
      q15_t *outp=output.ptr();          \
      q15_t *tmpPtr=tmp.ptr();              \
      q15_t *refp=ref.ptr(); \
      int16_t *dimsp = dims.ptr();           \
      int nbMatrixes = dims.nbSamples() / 4;\
      int rows,internal,columns,shift;                      \
      int i;





#define PREPAREDATA2()                                                   \
      in1.numRows=rows;                                                  \
      in1.numCols=internal;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(q15_t)*2*rows*internal);\
      in1.pData = ap;                                                    \
                                                                         \
      in2.numRows=internal;                                                  \
      in2.numCols=columns;                                               \
      memcpy((void*)bp,(const void*)inp2,sizeof(q15_t)*2*internal*columns);\
      in2.pData = bp;                                                    \
                                                                         \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      out.pData = outp;

                                             

    void BinaryComplexTestsNeonQ15::test_mat_cmplx_mult_q15()
    {     
      LOADDATA2();
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          internal = *dimsp++;
          columns = *dimsp++;
          shift = *dimsp++;

          PREPAREDATA2();

          try {
             // Temporary buffer only used in scalar version
             memset(tmpPtr,0,sizeof(q15_t)*2*internal*columns);
             // Shift disabled because the loss of accuracy is too big
             // Saturation is avoided by limiting the test to smaller matrixes.
             shift = 0;
             if (shift!=0)
                arm_shift_q15(inp1,-shift,ap,2*rows*internal);
             status=arm_mat_cmplx_mult_q15(&this->in1,&this->in2,&this->out,tmpPtr);
             if (shift!=0)
                arm_shift_q15(outp,shift,outp,2*rows*columns);
             ASSERT_TRUE(status==ARM_MATH_SUCCESS);
             ASSERT_NEAR_EQ_NB(outp,refp,ABS_ERROR_Q15,rows*columns);
          }
          catch(Client::Error &err) {
            char tmp[256];
            snprintf(tmp,256," (%d x %d x %d)\n",rows,internal,columns);
            strcat(err.details,tmp);
            throw(err);
          }

          outp += (2*rows * columns);
          refp += (2*rows * columns);
          checkInnerTail(outp);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(float32_t)SNR_THRESHOLD);

      ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q15);


    } 



    void BinaryComplexTestsNeonQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


      (void)params;
      switch(id)
      {
         case TEST_MAT_CMPLX_MULT_Q15_1:
            input1.reload(BinaryComplexTestsNeonQ15::INPUTS1_Q15_ID,mgr);
            input2.reload(BinaryComplexTestsNeonQ15::INPUTS2_Q15_ID,mgr);
            dims.reload(BinaryComplexTestsNeonQ15::DIMSBINARY1_S16_ID,mgr);

            ref.reload(BinaryComplexTestsNeonQ15::REFMUL1_Q15_ID,mgr);

            output.create(ref.nbSamples(),BinaryComplexTestsNeonQ15::OUT_Q15_ID,mgr);
            a.create(2*MAXMATRIXDIM*MAXMATRIXDIM,BinaryComplexTestsNeonQ15::TMPA_Q15_ID,mgr);
            b.create(2*MAXMATRIXDIM*MAXMATRIXDIM,BinaryComplexTestsNeonQ15::TMPB_Q15_ID,mgr);
            // Only useful for scalar version
            tmp.create(2*MAXMATRIXDIM*MAXMATRIXDIM,BinaryComplexTestsNeonQ15::TMP_Q15_ID,mgr);

         break;
      }
       

    
    }

    void BinaryComplexTestsNeonQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
       (void)id;
       output.dump(mgr);
    }
