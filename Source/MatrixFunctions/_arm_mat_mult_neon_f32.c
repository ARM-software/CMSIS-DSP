

#define L1_CACHE_SIZE (16*1024)
#define L2_CACHE_SIZE (128*1024)
#define L3_CACHE_SIZE (512*1024)


#define DR 4
#define DCS 4

#define LANE 4
#define DC (LANE*DCS) // Must be multiple of lane
#define vec float32x4_t
#define hvec float32x2_t

//#define INNER_BLOCK 200 
//#define ROWS_BLOCK 160
//#define COLS_BLOCK 64 

// A block : ROWS_BLOCK x COLS_BLOCK 
// B block : COLS_BLOCK x INNER_BLOCK
// C block ROWS_BLOCK x INNER_BLOCK
// 

// A55
//#define INNER_BLOCK 64  // inner
//#define ROWS_BLOCK 256 // rows
//#define COLS_BLOCK 64 // cols

// N1
#define INNER_BLOCK 64  // inner
#define ROWS_BLOCK 512 // rows
#define COLS_BLOCK 128 // cols


static float PACKEDB[INNER_BLOCK*COLS_BLOCK];
static float PACKEDA[ROWS_BLOCK*INNER_BLOCK];
static float PACKEDC[ROWS_BLOCK*COLS_BLOCK];

#if 0
// update 6x16 submatrix C[x:x+6][y:y+16]
// using A[x:x+6][l:r] and B[l:r][y:y+16]
__STATIC_INLINE void kernel(const float * __restrict__ a, 
                            float *  __restrict__ c, 
                            int x, int y, int yp,
                            int l, int r, 
                            int colsa,int colsb) {
    //printf("%d %d %d %d, yp=%d\n",x,y,l,r,yp);
    vec t[DR][DCS]={0}; // will be zero-filled and stored in ymm registers
    
    const float *pB = &PACKEDB[yp];


    for (int k=l; k < r ; k ++) 
    {
        const float *pAlpha0 = &a[(x + 0) * colsa+k];
        int i=0;
        for (; i < DR; i++) 
        {
            vec tmp;

            float alpha0 = *pAlpha0;
            pAlpha0 += colsa;

            tmp = vld1q_f32(pB);
            pB += LANE;

            // multiply b[k][y:y+16] by it and update t[i][0] and t[i][1]
            for (int j = 0; j < DCS; j++)
            {
                
                t[i][j] = vfmaq_n_f32(t[i][j],tmp,alpha0);
                
                tmp = vld1q_f32(pB);
                pB += LANE;
            }
            pB -= (DC+LANE);
        }
        pB += COLS_BLOCK;
    }

    // write the results back to C
    float *pC = &c[x * colsb + y];
    for (int i = 0; i < DR; i++)
    {
        for (int j = 0; j < DCS; j++)
        {
            vec tmp = vld1q_f32(pC);
            tmp = vaddq_f32(tmp,t[i][j]);
            vst1q_f32(pC,tmp);
            pC += LANE;
        }
        pC += colsb - DC;
    }
}
#else


#define UPDATE(i,j)            \
tmp = vld1q_f32(pC);         \
tmp = vaddq_f32(tmp,t##i##j);\
vst1q_f32(pC,tmp);           

// Only ok with DCS = 2
__STATIC_INLINE void kernel(const float * __restrict__ a, 
  float *  __restrict__ c, 
  int x, int y, int yp,
  int l, int r, 
  int colsa,int colsb) {

    //printf("row=%d col=%d l=%d r=%d\n",x,y,l,r);
  const float *pB0 = &PACKEDB[yp];
    //const float *pB1 = pB0 + COLS_BLOCK;
    //const float *pB2 = pB1 + COLS_BLOCK;
    //const float *pB3 = pB2 + COLS_BLOCK;

  vec tmp00,tmp01,tmp02,tmp03;
    //vec tmp10,tmp11,tmp12,tmp13;
    //vec tmp20,tmp21,tmp22,tmp23;
    //vec tmp30,tmp31,tmp32,tmp33;


  vec t00 = vdupq_n_f32(0);
  vec t01 = vdupq_n_f32(0);
  vec t02 = vdupq_n_f32(0);
  vec t03 = vdupq_n_f32(0);


  vec t10 = vdupq_n_f32(0);
  vec t11 = vdupq_n_f32(0);
  vec t12 = vdupq_n_f32(0);
  vec t13 = vdupq_n_f32(0);

  vec t20 = vdupq_n_f32(0);
  vec t21 = vdupq_n_f32(0);
  vec t22 = vdupq_n_f32(0);
  vec t23 = vdupq_n_f32(0);

  vec t30 = vdupq_n_f32(0);
  vec t31 = vdupq_n_f32(0);
  vec t32 = vdupq_n_f32(0);
  vec t33 = vdupq_n_f32(0);

  const float *pAlpha0 = &a[(x + 0) * colsa + l];
  const float *pAlpha1 = pAlpha0 + colsa;
  const float *pAlpha2 = pAlpha1 + colsa;
  const float *pAlpha3 = pAlpha2 + colsa;

  for (int k=l; k < r ; k ++) 
  {
            // broadcast a[x + i][k] into a register
    float alpha0 = *pAlpha0++;
    float alpha1 = *pAlpha1++;
    float alpha2 = *pAlpha2++;
    float alpha3 = *pAlpha3++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp03 = vld1q_f32(pB0);


            // multiply b[k][y:y+16] by it and update t[i][0] and t[i][1]


    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);
    t30 = vfmaq_n_f32(t30,tmp00,alpha3);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
    t21 = vfmaq_n_f32(t21,tmp01,alpha2);
    t31 = vfmaq_n_f32(t31,tmp01,alpha3);


    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t12 = vfmaq_n_f32(t12,tmp02,alpha1);
    t22 = vfmaq_n_f32(t22,tmp02,alpha2);
    t32 = vfmaq_n_f32(t32,tmp02,alpha3);


    t03 = vfmaq_n_f32(t03,tmp03,alpha0);
    t13 = vfmaq_n_f32(t13,tmp03,alpha1);
    t23 = vfmaq_n_f32(t23,tmp03,alpha2);
    t33 = vfmaq_n_f32(t33,tmp03,alpha3);

    pB0 += COLS_BLOCK - 3*LANE;

  }

    // write the results back to C
  float *pC = &c[x * colsb + y];
  vec tmp;

  UPDATE(0,0);
  pC += LANE;
  UPDATE(0,1);
  pC += LANE;
  UPDATE(0,2);
  pC += LANE;
  UPDATE(0,3);
  pC += (colsb - 3*LANE);

  UPDATE(1,0);
  pC += LANE;
  UPDATE(1,1);
  pC += LANE;
  UPDATE(1,2);
  pC += LANE;
  UPDATE(1,3);
  pC += (colsb - 3*LANE);

  UPDATE(2,0);
  pC += LANE;
  UPDATE(2,1);
  pC += LANE;
  UPDATE(2,2);
  pC += LANE;
  UPDATE(2,3);
  pC += (colsb - 3*LANE);

  UPDATE(3,0);
  pC += LANE;
  UPDATE(3,1);
  pC += LANE;
  UPDATE(3,2);
  pC += LANE;
  UPDATE(3,3);

}
#undef UPDATE
#endif


#define PACKB(WIDTH,COL,IN)                 \
{                                  \
\
const float *pB = &b[COL+IN*WIDTH];\
float *packed=PACKEDB;           \
int nb = MIN(WIDTH,COL+COLS_BLOCK)-COL; \
for(int i=0;i<INNER_BLOCK;i++)            \
  {                                \
for(int j=0;j<nb;j++)         \
 {                             \
*packed++ = *pB++;         \
}                             \
packed += COLS_BLOCK - nb; \
pB += WIDTH - nb;              \
}                                \
}

#define PACKA(WIDTH,HEIGHT,ROW,IN)                 \
{                                  \
                                   \
const float *pA = &a[ROW*WIDTH+IN];\
float *packed=PACKEDA;             \
int nb = MIN(HEIGHT,ROW+ROWS_BLOCK)-ROW;   \
for(int i=0;i<nb;i++)              \
{                                  \
for(int j=0;j<COLS_BLOCK;j++)              \
{                                  \
  packed[j*INNER_BLOCK+i] = *pA++;          \
}                                  \
pA += WIDTH - COLS_BLOCK;                  \
}                                  \
}

ARM_DSP_ATTRIBUTE arm_status arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
  const float32_t *a = pSrcA->pData;
  const float32_t *b = pSrcB->pData;
  float32_t *c = pDst->pData;

  int rows,cols,inner;
  rows=pSrcA->numRows;
  inner=pSrcA->numCols;
  cols=pSrcB->numCols;


  
//const int cols_delta = COLS_BLOCK;  // how many columns of B to select
//const int rows_delta = ROWS_BLOCK; // how many rows of A to select 
//const int inner_delta = INNER_BLOCK; // how many rows of B to select
//
// A block : ROWS_BLOCK x INNER_BLOCK 
// B block : INNER_BLOCK x COLS_BLOCK
// C block ROWS_BLOCK x INNER_BLOCK
// COLS_BLOCKXINNER_BLOCK -> half of L1 
// ROWS_BLOCKxCOLS_BLOCK -> L2 
// 
// // 64 120 128
const int inner_delta = INNER_BLOCK; // inner
const int rows_delta = ROWS_BLOCK; // rows
const int cols_delta = COLS_BLOCK; // cols


memset(c,0,sizeof(float32_t)*rows*cols);

for (int block_col = 0; block_col < cols; block_col += cols_delta)
{ 
    // now we are working with b[:][block_col:block_col+cols_delta]
  for (int block_row = 0; block_row < rows; block_row += rows_delta) 
  {    
    
        // now we are working with a[block_row:block_row+rows_delta][:]
   for (int block_inner = 0; block_inner < inner; block_inner += inner_delta)
// 
   {
    PACKB(cols,block_col,block_inner);

    int row=block_row;

    for (; row <= (MIN(block_row + rows_delta, rows)-DR); row += DR)
    {
      int col=block_col;

      for (; col <= (MIN(block_col + cols_delta, cols)-DC); col += DC)
      {
        kernel(a, c, row,col, col-block_col,block_inner, MIN(block_inner + inner_delta, inner), inner, cols);
      }

      for(;col < MIN(block_col + cols_delta, cols); col ++)
      {
        for(int nr = 0; nr < DR ; nr++)
        {
          for(int k=block_inner;k<MIN(block_inner + inner_delta, inner);k++)
          {
            c[(row+nr) * cols + col] += a[(row+nr) * inner + k] * b[k * cols + col];
          }
        }
      }

    }

    for(;row < MIN(block_row + rows_delta, rows);row++)
    {
      for(int col=block_col ;col < MIN(block_col + cols_delta, cols); col ++)
      {
       for(int k=block_inner;k<MIN(block_inner + inner_delta, inner);k++)
       {
        c[row * cols + col] += a[row * inner + k] * b[k * cols + col];
      }
    }
  }

}
}

}

return (ARM_MATH_SUCCESS);
}

#undef DR 
#undef DC
#undef LANE
#undef UNROLL
#undef DCS



#if 0
ARM_DSP_ATTRIBUTE arm_status arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
  float32_t *pIn1 = pSrcA->pData;                /* input data matrix pointer A */
  float32_t *pIn2 = pSrcB->pData;                /* input data matrix pointer B */
  float32_t *pInA = pSrcA->pData;                /* input data matrix pointer A  */
  float32_t *pOut = pDst->pData;                 /* output data matrix pointer */
  float32_t *px;                                 /* Temporary output data matrix pointer */
  float32_t sum;                                 /* Accumulator */
  uint16_t numRowsA = pSrcA->numRows;            /* number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* number of columns of input matrix A */


  uint32_t col, i = 0U, j, row = numRowsA, rowCnt, colCnt;      /* loop counters */
  arm_status status;                             /* status of matrix multiplication */

  float32x4_t a0V, a1V, a2V, a3V, a4V, a5V, a6V, a7V;
  float32x4_t acc0,acc1,acc2,acc3,acc4,acc5,acc6,acc7,temp;
  float32x2_t accum = vdup_n_f32(0);
  float32_t *pIn1B = pSrcA->pData;    
  float32_t *pIn1C = pSrcA->pData;    
  float32_t *pIn1D = pSrcA->pData;  
  float32_t *pIn1E = pSrcA->pData; 
  float32_t *pIn1F = pSrcA->pData; 
  float32_t *pIn1G = pSrcA->pData; 
  float32_t *pIn1H = pSrcA->pData;   

  float32_t *pxB,*pxC, *pxD, *pxE, *pxF, *pxG, *pxH;                                 /* Temporary output data matrix pointer */
  float32_t sum0,sum1, sum2,sum3, sum4, sum5 , sum6, sum7;

#ifdef ARM_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrcA->numCols != pSrcB->numRows) ||
     (pSrcA->numRows != pDst->numRows) || (pSrcB->numCols != pDst->numCols))
  {
    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else
#endif /*      #ifdef ARM_MATH_MATRIX_CHECK    */
  {    
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* Row loop */
    rowCnt = row >> 3;

    while(rowCnt > 0)
    {
      /* Output pointer is set to starting address of the row being processed */
      px = pOut + GROUPOFROWS*i;
      pxB = px + numColsB;
      pxC = px + 2*numColsB;
      pxD = px + 3*numColsB;
      pxE = px + 4*numColsB;
      pxF = px + 5*numColsB;
      pxG = px + 6*numColsB;
      pxH = px + 7*numColsB;

      /* For every row wise process, the column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, the pIn2 pointer is set
       ** to the starting address of the pSrcB data */
      pIn2 = pSrcB->pData;

      j = 0U;

      /* Column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum0 = 0.0f;
        sum1 = 0.0f;
        sum2 = 0.0f;
        sum3 = 0.0f;
        sum4 = 0.0f;
        sum5 = 0.0f;
        sum6 = 0.0f;
        sum7 = 0.0f;

        /* Initiate the pointer pIn1 to point to the starting address of the column being processed */
        pIn1 = pInA;
        pIn1B = pIn1 + numColsA;
        pIn1C = pIn1 + 2*numColsA;
        pIn1D = pIn1 + 3*numColsA;
        pIn1E = pIn1 + 4*numColsA;
        pIn1F = pIn1 + 5*numColsA;
        pIn1G = pIn1 + 6*numColsA;
        pIn1H = pIn1 + 7*numColsA;

        acc0 = vdupq_n_f32(0.0);
        acc1 = vdupq_n_f32(0.0);
        acc2 = vdupq_n_f32(0.0);
        acc3 = vdupq_n_f32(0.0);
        acc4 = vdupq_n_f32(0.0);
        acc5 = vdupq_n_f32(0.0);
        acc6 = vdupq_n_f32(0.0);
        acc7 = vdupq_n_f32(0.0);

        /* Compute 4 MACs simultaneously. */
        colCnt = numColsA >> 2U;

        /* Matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1)*b(1,1) + a(1,2)*b(2,1) + ... + a(m,p)*b(p,n) */
          a0V = vld1q_f32(pIn1);  
          a1V = vld1q_f32(pIn1B);  
          a2V = vld1q_f32(pIn1C); 
          a3V = vld1q_f32(pIn1D); 
          a4V = vld1q_f32(pIn1E); 
          a5V = vld1q_f32(pIn1F); 
          a6V = vld1q_f32(pIn1G); 
          a7V = vld1q_f32(pIn1H); 

          pIn1 += 4;
          pIn1B += 4;
          pIn1C += 4;
          pIn1D += 4;
          pIn1E += 4;
          pIn1F += 4;
          pIn1G += 4;
          pIn1H += 4;
          
          temp = vsetq_lane_f32(*pIn2,temp,0);
          pIn2 += numColsB;
          temp = vsetq_lane_f32(*pIn2,temp,1);
          pIn2 += numColsB;
          temp = vsetq_lane_f32(*pIn2,temp,2);
          pIn2 += numColsB;
          temp = vsetq_lane_f32(*pIn2,temp,3);
          pIn2 += numColsB;

          acc0 = vmlaq_f32(acc0,a0V,temp);
          acc1 = vmlaq_f32(acc1,a1V,temp);
          acc2 = vmlaq_f32(acc2,a2V,temp);
          acc3 = vmlaq_f32(acc3,a3V,temp);
          acc4 = vmlaq_f32(acc4,a4V,temp);
          acc5 = vmlaq_f32(acc5,a5V,temp);
          acc6 = vmlaq_f32(acc6,a6V,temp);
          acc7 = vmlaq_f32(acc7,a7V,temp);

          /* Decrement the loop count */
          colCnt--;
        }

        accum = vpadd_f32(vget_low_f32(acc0), vget_high_f32(acc0));
        sum0 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        accum = vpadd_f32(vget_low_f32(acc1), vget_high_f32(acc1));
        sum1 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        accum = vpadd_f32(vget_low_f32(acc2), vget_high_f32(acc2));
        sum2 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        accum = vpadd_f32(vget_low_f32(acc3), vget_high_f32(acc3));
        sum3 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        accum = vpadd_f32(vget_low_f32(acc4), vget_high_f32(acc4));
        sum4 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        accum = vpadd_f32(vget_low_f32(acc5), vget_high_f32(acc5));
        sum5 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        accum = vpadd_f32(vget_low_f32(acc6), vget_high_f32(acc6));
        sum6 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        accum = vpadd_f32(vget_low_f32(acc7), vget_high_f32(acc7));
        sum7 += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        /* If the columns of pSrcA is not a multiple of 4, compute any remaining MACs here.
         ** No loop unrolling is used. */
        colCnt = numColsA & 3;

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1)*b(1,1) + a(1,2)*b(2,1) + ... + a(m,p)*b(p,n) */
          sum0 += *pIn1++ * (*pIn2);
          sum1 += *pIn1B++ * (*pIn2);
          sum2 += *pIn1C++ * (*pIn2);
          sum3 += *pIn1D++ * (*pIn2);
          sum4 += *pIn1E++ * (*pIn2);
          sum5 += *pIn1F++ * (*pIn2);
          sum6 += *pIn1G++ * (*pIn2);
          sum7 += *pIn1H++ * (*pIn2);
          pIn2 += numColsB;

          /* Decrement the loop counter */
          colCnt--;
        }

        /* Store the result in the destination buffer */
        *px++ = sum0;
        *pxB++ = sum1;
        *pxC++ = sum2;
        *pxD++ = sum3;
        *pxE++ = sum4;
        *pxF++ = sum5;
        *pxG++ = sum6;
        *pxH++ = sum7;

        /* Update the pointer pIn2 to point to the  starting address of the next column */
        j++;
        pIn2 = pSrcB->pData + j;

        /* Decrement the column loop counter */
        col--;

      } while (col > 0U);

      /* Update the pointer pInA to point to the  starting address of the next row */
      i = i + numColsB;
      pInA = pInA + GROUPOFROWS*numColsA;

      /* Decrement the row loop counter */
      rowCnt--;
    } 

    /*

    i was the index of a group of rows computed by previous loop.
    Now i is the index of a row since below code is computing row per row
    and no more group of row per group of rows.

    */

    i = GROUPOFROWS*i;
    rowCnt = row & 7;

    while(rowCnt > 0)
    {
      /* Output pointer is set to starting address of the row being processed */
      px = pOut + i;

      /* For every row wise process, the column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, the pIn2 pointer is set
       ** to the starting address of the pSrcB data */
      pIn2 = pSrcB->pData;

      j = 0U;

      /* Column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0.0f;

        /* Initiate the pointer pIn1 to point to the starting address of the column being processed */
        pIn1 = pInA;

        acc0 = vdupq_n_f32(0.0);

        /* Compute 4 MACs simultaneously. */
        colCnt = numColsA >> 2U;

        /* Matrix multiplication   */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1)*b(1,1) + a(1,2)*b(2,1) + ... + a(m,p)*b(p,n) */
          a0V = vld1q_f32(pIn1);  // load & separate real/imag pSrcA (de-interleave 2)
          pIn1 += 4;
          
          temp = vsetq_lane_f32(*pIn2,temp,0);
          pIn2 += numColsB;
          temp = vsetq_lane_f32(*pIn2,temp,1);
          pIn2 += numColsB;
          temp = vsetq_lane_f32(*pIn2,temp,2);
          pIn2 += numColsB;
          temp = vsetq_lane_f32(*pIn2,temp,3);
          pIn2 += numColsB;

          acc0 = vmlaq_f32(acc0,a0V,temp);

          /* Decrement the loop count */
          colCnt--;
        }

        accum = vpadd_f32(vget_low_f32(acc0), vget_high_f32(acc0));
        sum += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

        /* If the columns of pSrcA is not a multiple of 4, compute any remaining MACs here.
         ** No loop unrolling is used. */
        colCnt = numColsA % 0x4U;

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1)*b(1,1) + a(1,2)*b(2,1) + ... + a(m,p)*b(p,n) */
          sum += *pIn1++ * (*pIn2);
          pIn2 += numColsB;

          /* Decrement the loop counter */
          colCnt--;
        }

        /* Store the result in the destination buffer */
        *px++ = sum;

        /* Update the pointer pIn2 to point to the  starting address of the next column */
        j++;
        pIn2 = pSrcB->pData + j;

        /* Decrement the column loop counter */
        col--;

      } while (col > 0U);


      /* Update the pointer pInA to point to the  starting address of the next row */
      i = i + numColsB;
      pInA = pInA + numColsA;

      /* Decrement the row loop counter */
      rowCnt--;

    } 
    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

#endif