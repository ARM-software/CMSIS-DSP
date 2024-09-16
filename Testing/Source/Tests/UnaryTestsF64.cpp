#include "UnaryTestsF64.h"
#include "Error.h"

#define SNR_THRESHOLD 250

/* 

Reference patterns are generated with
a double precision computation.

*/
#define REL_ERROR (1.0e-12)
#define ABS_ERROR (1.0e-12)

/*

Comparisons for householder

*/
#define SNR_HOUSEHOLDER_THRESHOLD 270
#define REL_HOUSEHOLDER_ERROR (1.0e-13)
#define ABS_HOUSEHOLDER_ERROR (1.0e-13)

/*

Comparison for QR decomposition

*/
#define SNR_QR_THRESHOLD 270
#define REL_QR_ERROR (1.0e-13)
#define ABS_QR_ERROR (1.0e-13)

/*

Comparison for Cholesky

*/
#define SNR_THRESHOLD_CHOL 269
#define REL_ERROR_CHOL (1.0e-9)
#define ABS_ERROR_CHOL (1.0e-9)

/* LDLT comparison */

#define REL_ERROR_LDLT (1e-5)
#define ABS_ERROR_LDLT (1e-5)

/* Upper bound of maximum matrix dimension used by Python */
#define MAXMATRIXDIM 40

#define LOADDATA2()                          \
      const float64_t *inp1=input1.ptr();    \
      const float64_t *inp2=input2.ptr();    \
                                             \
      float64_t *ap=a.ptr();                 \
      float64_t *bp=b.ptr();                 \
                                             \
      float64_t *outp=output.ptr();          \
      int16_t *dimsp = dims.ptr();           \
      int nbMatrixes = dims.nbSamples() >> 1;\
      int rows,columns;                      \
      int i;

#define LOADDATA1()                          \
      const float64_t *inp1=input1.ptr();    \
                                             \
      float64_t *ap=a.ptr();                 \
                                             \
      float64_t *outp=output.ptr();          \
      int16_t *dimsp = dims.ptr();           \
      int nbMatrixes = dims.nbSamples() >> 1;\
      int rows,columns;                      \
      int i;

#define PREPAREDATA2()                                                   \
      in1.numRows=rows;                                                  \
      in1.numCols=columns;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(float64_t)*rows*columns);\
      in1.pData = ap;                                                    \
                                                                         \
      in2.numRows=rows;                                                  \
      in2.numCols=columns;                                               \
      memcpy((void*)bp,(const void*)inp2,sizeof(float64_t)*rows*columns);\
      in2.pData = bp;                                                    \
                                                                         \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      out.pData = outp;

#define PREPAREDATALT()                                                  \
      in1.numRows=rows;                                                  \
      in1.numCols=rows;                                                  \
      memcpy((void*)ap,(const void*)inp1,sizeof(float64_t)*rows*rows);   \
      in1.pData = ap;                                                    \
                                                                         \
      in2.numRows=rows;                                                  \
      in2.numCols=columns;                                               \
      memcpy((void*)bp,(const void*)inp2,sizeof(float64_t)*rows*columns);\
      in2.pData = bp;                                                    \
                                                                         \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      out.pData = outp;

#define PREPAREDATA1(TRANSPOSED)                                         \
      in1.numRows=rows;                                                  \
      in1.numCols=columns;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(float64_t)*rows*columns);\
      in1.pData = ap;                                                    \
                                                                         \
      if (TRANSPOSED)                                                    \
      {                                                                  \
         out.numRows=columns;                                            \
         out.numCols=rows;                                               \
      }                                                                  \
      else                                                               \
      {                                                                  \
      out.numRows=rows;                                                  \
      out.numCols=columns;                                               \
      }                                                                  \
      out.pData = outp;

#define PREPAREDATALL1()                                                 \
      in1.numRows=rows;                                                  \
      in1.numCols=columns;                                               \
      memcpy((void*)ap,(const void*)inp1,sizeof(float64_t)*rows*columns);\
      in1.pData = ap;                                                    \
                                                                         \
      outll.numRows=rows;                                                \
      outll.numCols=columns;                                             \
                                                                         \
      outll.pData = outllp;

#define SWAP_ROWS(A,i,j)     \
  for(int w=0;w < n; w++)    \
  {                          \
     float64_t tmp;          \
     tmp = A[i*n + w];       \
     A[i*n + w] = A[j*n + w];\
     A[j*n + w] = tmp;       \
  }

static void checkInnerTailOverflow(float64_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
}


void UnaryTestsF64::test_mat_add_f64()
{

}

void UnaryTestsF64::test_householder_f64()
{
   int64_t vecDim;
   const int16_t *dimsp = dims.ptr();          
   const int nbVectors = dims.nbSamples();
   const float64_t *inp1=input1.ptr(); 

   float64_t *outp=output.ptr();   
   float64_t *outBetap=outputBeta.ptr();  


   for(int i=0; i < nbVectors ; i++)
   {
      vecDim = *dimsp++;

      float64_t beta = arm_householder_f64(inp1,DEFAULT_HOUSEHOLDER_THRESHOLD_F64,vecDim,outp);
      *outBetap = beta; 

      outp += vecDim;
      inp1 += vecDim;
      outBetap++;
      checkInnerTailOverflow(outp);
      checkInnerTailOverflow(outBetap);

   }

   ASSERT_EMPTY_TAIL(output);
   ASSERT_EMPTY_TAIL(outputBeta);

   ASSERT_SNR(output,ref,(float64_t)SNR_HOUSEHOLDER_THRESHOLD);
   ASSERT_SNR(outputBeta,refBeta,(float64_t)SNR_HOUSEHOLDER_THRESHOLD);

   ASSERT_CLOSE_ERROR(output,ref,ABS_HOUSEHOLDER_ERROR,REL_HOUSEHOLDER_ERROR);
   ASSERT_CLOSE_ERROR(outputBeta,refBeta,ABS_HOUSEHOLDER_ERROR,REL_HOUSEHOLDER_ERROR);

  
}

#include "dsp/debug.h"

void UnaryTestsF64::test_mat_qr_f64()
{
   int64_t rows, columns, rank;
   int nb;
   const int16_t *dimsp = dims.ptr();          
   const int nbMatrixes = dims.nbSamples() / 3;
   const float64_t *inp1=input1.ptr(); 

   float64_t *outTaup=outputTau.ptr();   
   float64_t *outRp=outputR.ptr();  
   float64_t *outQp=outputQ.ptr();  

   float64_t *pTmpA=a.ptr();  
   float64_t *pTmpB=b.ptr();  

   (void) outTaup;
   (void) outRp; 
   (void) outQp; 
   (void)nbMatrixes;
   (void)nb;
   (void)dimsp;
   (void)inp1;

   nb=0;
   for(int i=0; i < nbMatrixes ; i++)
   //for(int i=0; i < 1 ; i++)
   {
      rows = *dimsp++;
      columns = *dimsp++;
      rank = *dimsp++;
      (void)rank;

      //printf("--> %d %d : %lld %lld\n",nb,i,rows,columns);
      nb += rows * columns;

      in1.numRows=rows;
      in1.numCols=columns;
      in1.pData = (float64_t*)inp1;

      outR.numRows = rows;
      outR.numCols = columns;
      outR.pData = (float64_t*)outRp;

      outQ.numRows = rows;
      outQ.numCols = rows;
      outQ.pData = (float64_t*)outQp;

      
      arm_status status=arm_mat_qr_f64(&in1,DEFAULT_HOUSEHOLDER_THRESHOLD_F64,&outR,&outQ,outTaup,pTmpA,pTmpB);
      ASSERT_TRUE(status==ARM_MATH_SUCCESS);
      
      // Set Householder reflectors into R matrix to 0
      //float64_t *p = outRp ;
      //printf("%d %d %d\n",in1.numCols, outR.numRows,outR.numCols);
      #if 0
      for(int col=0 ; col < in1.numCols; col++)
      {
          float64_t *pa = p + outR.numCols;
          for(int k=0;k<outR.numRows-col-1; k++)
          {
             *pa = 0;
             pa += outR.numCols;
          }
          p += 1 + outR.numCols;
      }
      #endif
      //PM_f64("Corrected R",&outR);

      inp1 += rows * columns;
      outRp += rows * columns;
      outQp += rows * rows;
      outTaup += columns;

      checkInnerTailOverflow(outRp);
      checkInnerTailOverflow(outQp);
      checkInnerTailOverflow(outTaup);


   }


   ASSERT_EMPTY_TAIL(outputR);
   ASSERT_EMPTY_TAIL(outputQ);
   ASSERT_EMPTY_TAIL(outputTau);

   ASSERT_SNR(refQ,outputQ,(float64_t)SNR_QR_THRESHOLD);
   ASSERT_SNR(refR,outputR,(float64_t)SNR_QR_THRESHOLD);
   ASSERT_SNR(refTau,outputTau,(float64_t)SNR_QR_THRESHOLD);

   ASSERT_CLOSE_ERROR(refQ,outputQ,ABS_QR_ERROR,REL_QR_ERROR);
   ASSERT_CLOSE_ERROR(refR,outputR,ABS_QR_ERROR,REL_QR_ERROR);
   ASSERT_CLOSE_ERROR(refTau,outputTau,ABS_QR_ERROR,REL_QR_ERROR);
}

void UnaryTestsF64::test_mat_sub_f64()
{
      LOADDATA2();
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          columns = *dimsp++;

          PREPAREDATA2();

          status=arm_mat_sub_f64(&this->in1,&this->in2,&this->out);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

      ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);
}

void UnaryTestsF64::test_mat_scale_f64()
{

}

void UnaryTestsF64::test_mat_trans_f64()
{
      LOADDATA1();
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          columns = *dimsp++;

          PREPAREDATA1(true);

          status=arm_mat_trans_f64(&this->in1,&this->out);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,SNR_THRESHOLD);

      ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

}

/*

Test framework is only adding 16 bytes of free memory after the end of a buffer.
So, we limit to 2 float64 for checking out of buffer write.
*/
static void refInnerTail(float64_t *b)
{
    b[0] = 1.0;
    b[1] = -2.0;
}

static void checkInnerTail(float64_t *b)
{
    ASSERT_TRUE(b[0] == 1.0);
    ASSERT_TRUE(b[1] == -2.0);
}


void UnaryTestsF64::test_mat_inverse_f64()
    {     
      const float64_t *inp1=input1.ptr();    
                                             
      float64_t *ap=a.ptr();                 
                                             
      float64_t *outp=output.ptr();          
      int16_t *dimsp = dims.ptr();           
      int nbMatrixes = dims.nbSamples();
      int rows,columns;                      
      int i;
      arm_status status;
      
      // Non singular matrixes
      // Last matrix is singular
      for(i=0;i < nbMatrixes - 1 ; i ++)
      {
          rows = *dimsp++;
          columns = rows;

          PREPAREDATA1(false);

          refInnerTail(outp+(rows * columns));

          status=arm_mat_inverse_f64(&this->in1,&this->out);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);
          inp1 += (rows * columns);

          checkInnerTail(outp);

      }
      /*** Singular matrix **/
      rows = *dimsp++;
      columns = rows;

      PREPAREDATA1(false);

      refInnerTail(outp+(rows * columns));

      status=arm_mat_inverse_f64(&this->in1,&this->out);
      ASSERT_TRUE(status==ARM_MATH_SINGULAR);

      outp += (rows * columns);
      inp1 += (rows * columns);

      checkInnerTail(outp);
      /**********************/


      ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

      ASSERT_CLOSE_ERROR(output,ref,ABS_ERROR,REL_ERROR);

    }

    void UnaryTestsF64::test_mat_cholesky_dpo_f64()
    {
      float64_t *ap=a.ptr();                 
      const float64_t *inp1=input1.ptr();    
                                             
                                             
      float64_t *outp=output.ptr();     
      int16_t *dimsp = dims.ptr();           
      int nbMatrixes = dims.nbSamples();

      int rows,columns;                      
      int i;
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          columns = rows;

          PREPAREDATA1(false);

          status=arm_mat_cholesky_f64(&this->in1,&this->out);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);
          inp1 += (rows * columns);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD_CHOL);

      ASSERT_CLOSE_ERROR(ref,output,ABS_ERROR_CHOL,REL_ERROR_CHOL);
    }

    void UnaryTestsF64::test_solve_upper_triangular_f64()
    {
      float64_t *ap=a.ptr();                 
      const float64_t *inp1=input1.ptr();    

      float64_t *bp=b.ptr();                 
      const float64_t *inp2=input2.ptr();    
                                             
                                             
      float64_t *outp=output.ptr();     
      int16_t *dimsp = dims.ptr();           
      int nbMatrixes = dims.nbSamples()>>1;

      int rows,columns;                      
      int i;
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          columns = *dimsp++;

          PREPAREDATALT();

          status=arm_mat_solve_upper_triangular_f64(&this->in1,&this->in2,&this->out);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);
          inp1 += (rows * rows);
          inp2 += (rows * columns);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

      ASSERT_CLOSE_ERROR(ref,output,ABS_ERROR,REL_ERROR);
    }

    void UnaryTestsF64::test_solve_lower_triangular_f64()
    {
      float64_t *ap=a.ptr();                 
      const float64_t *inp1=input1.ptr();    

      float64_t *bp=b.ptr();                 
      const float64_t *inp2=input2.ptr();    
                                             
                                             
      float64_t *outp=output.ptr();     
      int16_t *dimsp = dims.ptr();           
      int nbMatrixes = dims.nbSamples()>>1;

      int rows,columns;                      
      int i;
      arm_status status;

      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          columns = *dimsp++;

          PREPAREDATALT();

          status=arm_mat_solve_lower_triangular_f64(&this->in1,&this->in2,&this->out);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          outp += (rows * columns);
          inp1 += (rows * rows);
          inp2 += (rows * columns);

      }

      ASSERT_EMPTY_TAIL(output);

      ASSERT_SNR(output,ref,(float64_t)SNR_THRESHOLD);

      ASSERT_CLOSE_ERROR(ref,output,ABS_ERROR,REL_ERROR);
    }

    static void trans_f64(const float64_t *src, float64_t *dst, int n)
    {
        for(int r=0; r<n ; r++)
        {
          for(int c=0; c<n ; c++)
          {
              dst[c*n+r] = src[r*n+c];
          }
        }
    }

    static void mult_f64_f64(const float64_t *srcA, const float64_t *srcB, float64_t *dst,int n)
    {
        for(int r=0; r<n ; r++)
        {
          for(int c=0; c<n ; c++)
          {
             float64_t sum=0.0;
             for(int k=0; k < n ; k++)
             {
                sum += srcA[r*n+k] * srcB[k*n+c];
             }
             dst[r*n+c] = sum;
          }
        }
    }

    void UnaryTestsF64::compute_ldlt_error(const int n,const int16_t *outpp)
    {
           float64_t *tmpa  = tmpapat.ptr() ;
           float64_t *tmpb  = tmpbpat.ptr() ;
           float64_t *tmpc  = tmpcpat.ptr() ;
                                           
          
          /* Compute P A P^t */

          // Create identiy matrix
          for(int r=0; r < n; r++)
          {
            for(int c=0; c < n; c++)
            {
               if (r == c)
               {
                 tmpa[r*n+c] = 1.0;
               }
               else
               {
                 tmpa[r*n+c] = 0.0;
               }
            }
          }

          // Create permutation matrix

          for(int r=0;r < n; r++)
          {
            SWAP_ROWS(tmpa,r,outpp[r]);
          }

          

          trans_f64((const float64_t*)tmpa,tmpb,n);
          mult_f64_f64((const float64_t*)this->in1.pData,(const float64_t*)tmpb,tmpc,n);
          mult_f64_f64((const float64_t*)tmpa,(const float64_t*)tmpc,outa,n);


          /* Compute L D L^t */
          trans_f64((const float64_t*)this->outll.pData,tmpc,n);
          mult_f64_f64((const float64_t*)this->outd.pData,(const float64_t*)tmpc,tmpa,n);
          mult_f64_f64((const float64_t*)this->outll.pData,(const float64_t*)tmpa,outb,n);

          
    }

    void UnaryTestsF64::test_mat_ldl_f64()
    {
      float64_t *ap=a.ptr();                 
      const float64_t *inp1=input1.ptr();  

                                            
      float64_t *outllp=outputll.ptr();   
      float64_t *outdp=outputd.ptr();   
      int16_t *outpp=outputp.ptr();   


      outa=outputa.ptr();   
      outb=outputb.ptr();   

      int16_t *dimsp = dims.ptr();           
      int nbMatrixes = dims.nbSamples();

      int rows,columns;                      
      int i;
      arm_status status;


      for(i=0;i < nbMatrixes ; i ++)
      {
          rows = *dimsp++;
          columns = rows;

          PREPAREDATALL1();

          outd.numRows=rows;
          outd.numCols=columns;
          outd.pData=outdp;

          


          memset(outpp,0,rows*sizeof(uint16_t));
          memset(outdp,0,columns*rows*sizeof(float64_t));

          status=arm_mat_ldlt_f64(&this->in1,&this->outll,&this->outd,(uint16_t*)outpp);
          ASSERT_TRUE(status==ARM_MATH_SUCCESS);

          compute_ldlt_error(rows,outpp);

          outllp += (rows * columns);
          outdp += (rows * columns);
          outpp += rows;

          outa += (rows * columns);
          outb +=(rows * columns);

          inp1 += (rows * columns);

      }

      ASSERT_EMPTY_TAIL(outputll);
      ASSERT_EMPTY_TAIL(outputd);
      ASSERT_EMPTY_TAIL(outputp);
      ASSERT_EMPTY_TAIL(outputa);
      ASSERT_EMPTY_TAIL(outputb);


      ASSERT_CLOSE_ERROR(outputa,outputb,ABS_ERROR_LDLT,REL_ERROR_LDLT);


     
    

    }


    void UnaryTestsF64::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
    
      (void)params;
      switch(id)
      {
          case TEST_MAT_SUB_F64_2:
            input1.reload(UnaryTestsF64::INPUTS1_F64_ID,mgr);
            input2.reload(UnaryTestsF64::INPUTS2_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMSUNARY1_S16_ID,mgr);

            ref.reload(UnaryTestsF64::REFSUB1_F64_ID,mgr);

            output.create(ref.nbSamples(),UnaryTestsF64::OUT_F64_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPA_F64_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPB_F64_ID,mgr);
          break;
          case TEST_MAT_TRANS_F64_4:
            input1.reload(UnaryTestsF64::INPUTS1_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMSUNARY1_S16_ID,mgr);

            ref.reload(UnaryTestsF64::REFTRANS1_F64_ID,mgr);

            output.create(ref.nbSamples(),UnaryTestsF64::OUT_F64_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPA_F64_ID,mgr);
         break;
         
         case TEST_MAT_INVERSE_F64_5:
            input1.reload(UnaryTestsF64::INPUTSINV_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMSINVERT1_S16_ID,mgr);

            ref.reload(UnaryTestsF64::REFINV1_F64_ID,mgr);

            output.create(ref.nbSamples(),UnaryTestsF64::OUT_F64_ID,mgr);
            a.create(ref.nbSamples(),UnaryTestsF64::TMPA_F64_ID,mgr);
         break;

         case TEST_MAT_CHOLESKY_DPO_F64_6:
            input1.reload(UnaryTestsF64::INPUTSCHOLESKY1_DPO_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMSCHOLESKY1_DPO_S16_ID,mgr);

            ref.reload(UnaryTestsF64::REFCHOLESKY1_DPO_F64_ID,mgr);

            output.create(ref.nbSamples(),UnaryTestsF64::OUT_F64_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPA_F64_ID,mgr);
         break;

         case TEST_SOLVE_UPPER_TRIANGULAR_F64_7:
            input1.reload(UnaryTestsF64::INPUT_MAT_UTSOLVE_F64_ID,mgr);
            input2.reload(UnaryTestsF64::INPUT_VEC_LTSOLVE_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIM_LTSOLVE_F64_ID,mgr);

            ref.reload(UnaryTestsF64::REF_UT_SOLVE_F64_ID,mgr);

            output.create(ref.nbSamples(),UnaryTestsF64::OUT_F64_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPA_F64_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPB_F64_ID,mgr);
         break;

         case TEST_SOLVE_LOWER_TRIANGULAR_F64_8:
            input1.reload(UnaryTestsF64::INPUT_MAT_LTSOLVE_F64_ID,mgr);
            input2.reload(UnaryTestsF64::INPUT_VEC_LTSOLVE_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIM_LTSOLVE_F64_ID,mgr);

            ref.reload(UnaryTestsF64::REF_LT_SOLVE_F64_ID,mgr);

            output.create(ref.nbSamples(),UnaryTestsF64::OUT_F64_ID,mgr);
            a.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPA_F64_ID,mgr);
            b.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPB_F64_ID,mgr);
         break;

         case TEST_MAT_LDL_F64_9:
            // Definite positive test
            input1.reload(UnaryTestsF64::INPUTSCHOLESKY1_DPO_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMSCHOLESKY1_DPO_S16_ID,mgr);

            outputll.create(input1.nbSamples(),UnaryTestsF64::LL_F64_ID,mgr);
            outputd.create(input1.nbSamples(),UnaryTestsF64::D_F64_ID,mgr);
            outputp.create(input1.nbSamples(),UnaryTestsF64::PERM_S16_ID,mgr);

            outputa.create(input1.nbSamples(),UnaryTestsF64::OUTA_F64_ID,mgr);
            outputb.create(input1.nbSamples(),UnaryTestsF64::OUTA_F64_ID,mgr);

            a.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPA_F64_ID,mgr);
            
            tmpapat.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPDB_F64_ID,mgr);
            tmpbpat.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPDC_F64_ID,mgr);
            tmpcpat.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPDD_F64_ID,mgr);

         break;

         case TEST_MAT_LDL_F64_10:
            // Semi definite positive test
            input1.reload(UnaryTestsF64::INPUTSCHOLESKY1_SDPO_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMSCHOLESKY1_SDPO_S16_ID,mgr);
           
            outputll.create(input1.nbSamples(),UnaryTestsF64::LL_F64_ID,mgr);
            outputd.create(input1.nbSamples(),UnaryTestsF64::D_F64_ID,mgr);
            outputp.create(input1.nbSamples(),UnaryTestsF64::PERM_S16_ID,mgr);

            outputa.create(input1.nbSamples(),UnaryTestsF64::OUTA_F64_ID,mgr);
            outputb.create(input1.nbSamples(),UnaryTestsF64::OUTA_F64_ID,mgr);

            a.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPA_F64_ID,mgr);
            
            tmpapat.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPDB_F64_ID,mgr);
            tmpbpat.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPDC_F64_ID,mgr);
            tmpcpat.create(MAXMATRIXDIM*MAXMATRIXDIM,UnaryTestsF64::TMPDD_F64_ID,mgr);

         break;

         case TEST_HOUSEHOLDER_F64_11:
            input1.reload(UnaryTestsF64::INPUTS_HOUSEHOLDER_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMS_HOUSEHOLDER_S16_ID,mgr);
            ref.reload(UnaryTestsF64::REF_HOUSEHOLDER_V_F64_ID,mgr);
            refBeta.reload(UnaryTestsF64::REF_HOUSEHOLDER_BETA_F64_ID,mgr);


            output.create(ref.nbSamples(),UnaryTestsF64::TMPA_F64_ID,mgr);
            outputBeta.create(refBeta.nbSamples(),UnaryTestsF64::TMPB_F64_ID,mgr);
         break;


         case TEST_MAT_QR_F64_12:
            input1.reload(UnaryTestsF64::INPUTS_QR_F64_ID,mgr);
            dims.reload(UnaryTestsF64::DIMS_QR_S16_ID,mgr);
            refTau.reload(UnaryTestsF64::REF_QR_TAU_F64_ID,mgr);
            refR.reload(UnaryTestsF64::REF_QR_R_F64_ID,mgr);
            refQ.reload(UnaryTestsF64::REF_QR_Q_F64_ID,mgr);


            outputTau.create(refTau.nbSamples(),UnaryTestsF64::TMPA_F64_ID,mgr);
            outputR.create(refR.nbSamples(),UnaryTestsF64::TMPB_F64_ID,mgr);
            outputQ.create(refQ.nbSamples(),UnaryTestsF64::TMPC_F64_ID,mgr);

            a.create(47,UnaryTestsF64::TMPC_F64_ID,mgr);
            b.create(47,UnaryTestsF64::TMPD_F64_ID,mgr);
         break;

      }
       

    
    }

    void UnaryTestsF64::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
       (void)id;
       //output.dump(mgr);
       (void)mgr;
    }
