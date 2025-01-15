#include <stdio.h>

#define DR 4
#define DCS 4

#define LANE 4
#define DC (LANE*DCS) // Must be multiple of lane
#define vec float32x4_t
#define hvec float32x2_t


// INNER * DC < L1/2
// INNER * ROWS < L2 
// INNER * COLS < L3
#define INNER_BLOCK ((((ARM_MATH_L1_CACHE_SIZE>>2) >> 1)/DC+DC-1) & ~(DC-1))
#define ROWS_BLOCK ((((ARM_MATH_L2_CACHE_SIZE>>2) >> 1) / INNER_BLOCK + DR - 1) & ~(DR-1) ) //256 // rows
#define COLS_BLOCK (((ARM_MATH_L3_CACHE_SIZE>>2) / INNER_BLOCK + DC - 1) & ~(DC-1)) //1024 // cols


static __ALIGNED(16) float PACKEDB[INNER_BLOCK*DC];
static __ALIGNED(16) float PACKEDA[ROWS_BLOCK*INNER_BLOCK];


__STATIC_INLINE void kernel(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02,tmp03;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += LANE;
  vec t03 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += LANE;
  vec t12 = vld1q_f32(p);
  p += LANE;
  vec t13 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t20 = vld1q_f32(p);
  p += LANE;
  vec t21 = vld1q_f32(p);
  p += LANE;
  vec t22 = vld1q_f32(p);
  p += LANE;
  vec t23 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t30 = vld1q_f32(p);
  p += LANE;
  vec t31 = vld1q_f32(p);
  p += LANE;
  vec t32 = vld1q_f32(p);
  p += LANE;
  vec t33 = vld1q_f32(p);
  p += cols - 3*LANE;

  const float *pAlpha0 = &PACKEDA[xp * INNER_BLOCK];
  const float *pAlpha1 = pAlpha0 + INNER_BLOCK;
  const float *pAlpha2 = pAlpha1 + INNER_BLOCK;
  const float *pAlpha3 = pAlpha2 + INNER_BLOCK;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;
    const float alpha3 = *pAlpha3++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp03 = vld1q_f32(pB0);
    pB0 += LANE;
    
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


  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += LANE;
  vst1q_f32(pC,t03);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += LANE;
  vst1q_f32(pC,t12);
  pC += LANE;
  vst1q_f32(pC,t13);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t20);
  pC += LANE;
  vst1q_f32(pC,t21);
  pC += LANE;
  vst1q_f32(pC,t22);
  pC += LANE;
  vst1q_f32(pC,t23);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t30);
  pC += LANE;
  vst1q_f32(pC,t31);
  pC += LANE;
  vst1q_f32(pC,t32);
  pC += LANE;
  vst1q_f32(pC,t33);
  pC += cols - 3*LANE;

}

#define PACK(BUF,PACKED,WIDTH,HEIGHT,RB,CB,ROW,COL)     \
{                                                       \
    const float *p = &(BUF)[(COL)+(ROW)*(WIDTH)];       \
    float *packed=(PACKED);                             \
    const int nb_rows = MIN((HEIGHT),(ROW)+(RB))-(ROW); \
    const int nb_cols = MIN((WIDTH),(COL)+(CB)) - (COL);\
    for(int i=0;i<nb_rows;i++)                          \
    {                                                   \
        int j=0;                                        \
        for(;j<=(nb_cols-LANE);j+=LANE)                 \
        {                                               \
            vec tmp = vld1q_f32(p);                     \
            p += LANE;                                  \
            vst1q_f32(packed,tmp);                      \
            packed += LANE;                             \
        }                                               \
        for(;j<nb_cols;j++)                             \
        {                                               \
            *packed++ = *p++;                           \
        }                                               \
        packed += (CB) - nb_cols;                       \
        p += WIDTH - nb_cols;                           \
    }                                                   \
}


ARM_DSP_ATTRIBUTE arm_status arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
  const float32_t *a = pSrcA->pData;
  const float32_t *b = pSrcB->pData;
  float32_t *c = pDst->pData;

  int rows,cols,inners;
  rows=pSrcA->numRows;
  inners=pSrcA->numCols;
  cols=pSrcB->numCols;


    for (int block_col = 0; block_col < cols; block_col += COLS_BLOCK)
    { 
      for (int block_row = 0; block_row < rows; block_row += ROWS_BLOCK) 
      {    
        for (int block_inner = 0; block_inner < inners; block_inner += INNER_BLOCK)
        {
          PACK(a,PACKEDA,inners,rows,ROWS_BLOCK,INNER_BLOCK,block_row,block_inner);

          int col=0;

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-DC); col += DC)
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,INNER_BLOCK,DC,block_inner,block_col+col);

            for (; row <= (MIN(0 + ROWS_BLOCK, rows - block_row)-DR); row += DR)
            {
              kernel(cols,c+(block_row+row)*cols + block_col+col,row,MIN(INNER_BLOCK, inners-block_inner));
            }

            for(; row < MIN(0 + ROWS_BLOCK, rows - block_row); row ++)
            {
              const float *pa = &PACKEDA[(row-0) * INNER_BLOCK ];
              const float *pb = PACKEDB;
              float *pc = &c[(row+block_row)*cols + col+block_col];


              for(int k=0;k<MIN(0 + INNER_BLOCK, inners - block_inner);k++)
              {
                float alpha = *pa++;
                for(int nc=0;nc < DCS; nc++)
                {
                  vec tmp0,tmp1;
                  tmp0 = vld1q_f32(pc);
                  tmp1 = vld1q_f32(pb);
                  pb += LANE;

                  tmp0 = vfmaq_n_f32(tmp0,tmp1,alpha);
                  //c[(row+block_row)*cols + (nc + col+block_col)] += alpha * PACKEDB[k*DC+nc];
                  vst1q_f32(pc,tmp0);
                  pc += LANE;
                }
                pc -= DC;
              }
            }
          }


          int maxnc=MIN(0 + COLS_BLOCK, cols- block_col) - col;

          if (maxnc > 0)
          {
             PACK(b,PACKEDB,cols,inners,INNER_BLOCK,DC,block_inner,block_col+col);
          
             for(int row=0;row < MIN(0 + ROWS_BLOCK, rows - block_row);row++)
             {
               float *pa = &PACKEDA[(row-0) * INNER_BLOCK];
               const float *pb = PACKEDB;
               float *pc = &c[(row+block_row)*cols + col+block_col];
   
               for(int k=0;k<MIN(0 + INNER_BLOCK, inners - block_inner);k++)
               {
                 float alpha = *pa++;
                 int nc = 0;
                 for(;nc <= (maxnc - LANE); nc += LANE)
                 {
                   vec tmp0,tmp1;
                   tmp0 = vld1q_f32(pc);
                   tmp1 = vld1q_f32(pb);
                   pb += LANE;
   
                   tmp0 = vfmaq_n_f32(tmp0,tmp1,alpha);
                   //c[(row+block_row)*cols + (nc + col+block_col)] += alpha * PACKEDB[k*DC+nc];
                   vst1q_f32(pc,tmp0);
                   pc += LANE;
                 }
                 for(;nc < maxnc; nc ++)
                 {
                   *pc += alpha * *pb++;
                   pc++;
                 }
                 pb += DC - maxnc;
                 pc -= maxnc;
   
               }
             }
          }
       } // end inner loop
     } // end row loop
   } // end col loop

   return (ARM_MATH_SUCCESS);
}

#undef DR 
#undef DC
#undef LANE
#undef UNROLL
#undef DCS

