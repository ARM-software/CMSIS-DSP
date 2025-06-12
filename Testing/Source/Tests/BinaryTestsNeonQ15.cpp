#include "BinaryTestsNeonQ15.h"
#include <stdio.h>
#include "Error.h"

#include "dsp/basic_math_functions.h"

extern int cov_mat_mul_q15[20];
extern int cov_mat_mul_fast_q15[20];

#define SNR_THRESHOLD 75
#define SNR_LOW_THRESHOLD 75
/* 

Reference patterns are generated with
a double precision computation.

*/
#define ABS_ERROR_Q15 ((q15_t)12)
#define ABS_LOW_ERROR_Q15 ((q15_t)3)

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





#define PREPAREDATA2R()                                                   \
      in1.numRows=rows;                                                  \
      in1.numCols=internal;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(q15_t)*rows*internal);\
      in1.pData = ap;                                                    \
                                                                         \
      in2.numRows=internal;                                                  \
      in2.numCols=columns;                                               \
      memcpy((void*)bp,(const void*)inp2,sizeof(q15_t)*internal*columns);\
      in2.pData = bp;                                                    \
                                                                         \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      out.pData = outp;

                                             

    void BinaryTestsNeonQ15::test_mat_mult_q15()
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

          try
          {
            memset(tmpPtr,0,sizeof(q15_t)*MAXMATRIXDIM*MAXMATRIXDIM);
            // Shift disabled because the loss of accuracy is too big
            // Saturation is avoided by limiting the test to smaller matrixes.
            shift = 0;
            if (shift!=0)
               arm_shift_q15(inp1,-shift,ap,rows*internal);
            status=arm_mat_mult_q15(&this->in1,&this->in2,&this->out,tmpPtr);
            if (shift!=0)
               arm_shift_q15(outp,shift,outp,rows*columns);
            ASSERT_TRUE(status==ARM_MATH_SUCCESS);
            ASSERT_NEAR_EQ_NB(outp,refp,ABS_ERROR_Q15,rows*columns);
          }
          catch(Client::Error &err)
          {
            char tmp[256];
            snprintf(tmp,256," mat id=%d (%d x %d x %d)\n",i,rows,internal,columns);
            strcat(err.details,tmp);
            throw(err);
          }

          outp += (rows * columns);
          refp += (rows * columns);
          checkInnerTail(outp);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(q15_t)SNR_THRESHOLD);

      ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q15);


    } 

    void BinaryTestsNeonQ15::test_mat_mult_fast_q15()
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

          try
          {
            memset(tmpPtr,0,sizeof(q15_t)*MAXMATRIXDIM*MAXMATRIXDIM);
            // Shift disabled because the loss of accuracy is too big
            // Saturation is avoided by limiting the test to smaller matrixes.
            shift = 0;
            if (shift!=0)
               arm_shift_q15(inp1,-shift,ap,rows*internal);
            status=arm_mat_mult_fast_q15(&this->in1,&this->in2,&this->out,tmpPtr);
            if (shift!=0)
               arm_shift_q15(outp,shift,outp,rows*columns);
            ASSERT_TRUE(status==ARM_MATH_SUCCESS);
            ASSERT_NEAR_EQ_NB(outp,refp,ABS_ERROR_Q15,rows*columns);
          }
          catch(Client::Error &err)
          {
            char tmp[256];
            snprintf(tmp,256," mat id=%d (%d x %d x %d)\n",i,rows,internal,columns);
            strcat(err.details,tmp);
            throw(err);
          }

          outp += (rows * columns);
          refp += (rows * columns);
          checkInnerTail(outp);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(q15_t)SNR_THRESHOLD);

      ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q15);


    }

    void BinaryTestsNeonQ15::beforeSuite()  
    {
       for(int i=0;i<NBKERNELS;i++)
       {
         cov_mat_mul_q15[i]=0;
         cov_mat_mul_fast_q15[i]=0;
       }
    };

    void BinaryTestsNeonQ15::afterSuite() 
    {
      int noerr = 1;
      Client::Error err(COVERAGE_ERROR,__LINE__);
      
      for(int i=0;i<NBKERNELS;i++)
      {
         
         if (cov_mat_mul_q15[i]==0)
         {
           noerr=0;
           char tmp[20];
           snprintf(tmp,20,"i=%d ",i);
           err.add_message(tmp);
         }
      }

      err.add_message(" F ");
      for(int i=0;i<NBKERNELS;i++)
      {
         
         if (cov_mat_mul_fast_q15[i]==0)
         {
           noerr=0;
           char tmp[20];
           snprintf(tmp,20,"i=%d ",i);
           err.add_message(tmp);
         }
      }
      if (noerr==0)
      {
        throw(err);
      }
    };



    void BinaryTestsNeonQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


      (void)params;
      switch(id)
      {
         case TEST_MAT_MULT_Q15_1:
            input1.reload(BinaryTestsNeonQ15::INPUTS1_Q15_ID,mgr);
            input2.reload(BinaryTestsNeonQ15::INPUTS2_Q15_ID,mgr);
            dims.reload(BinaryTestsNeonQ15::DIMSBINARY1_S16_ID,mgr);

            ref.reload(BinaryTestsNeonQ15::REFMUL1_Q15_ID,mgr);

            output.create(ref.nbSamples(),BinaryTestsNeonQ15::OUT_Q15_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ15::TMPA_Q15_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ15::TMPB_Q15_ID,mgr);
            tmp.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ15::TMP_Q15_ID,mgr);

         break;
      

         case TEST_MAT_MULT_FAST_Q15_2:
            input1.reload(BinaryTestsNeonQ15::INPUTS1_Q15_ID,mgr);
            input2.reload(BinaryTestsNeonQ15::INPUTS2_Q15_ID,mgr);
            dims.reload(BinaryTestsNeonQ15::DIMSLIMITEDBINARY1_S16_ID,mgr);

            ref.reload(BinaryTestsNeonQ15::REFLIMITEDMUL1_Q15_ID,mgr);

            output.create(ref.nbSamples(),BinaryTestsNeonQ15::OUT_Q15_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ15::TMPA_Q15_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ15::TMPB_Q15_ID,mgr);
            tmp.create(MAXMATRIXDIM*MAXMATRIXDIM,BinaryTestsNeonQ15::TMP_Q15_ID,mgr);
         break;
       
      }
    
    }

    void BinaryTestsNeonQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
       (void)id;
       output.dump(mgr);
    }
