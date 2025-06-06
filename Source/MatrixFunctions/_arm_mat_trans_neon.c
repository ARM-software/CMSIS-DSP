#define ARM_MAT_TRANS_NEON_INIT_U32                                                          \
  const uint32_t *pIn =  (const uint32_t *)pSrc->pData;                  /* input data matrix pointer */ \
  uint32_t *pOut = (uint32_t *)pDst->pData;                 /* output data matrix pointer */ \
  uint32_t *pCol;                                 /* Temporary output data matrix pointer */ \
  uint16_t nRows = pSrc->numRows;                /* number of rows */                        \
  uint16_t nColumns = pSrc->numCols;             /* number of columns */                     \
                                                                                             \
  uint16_t blkCnt, rowCnt, row = nRows;          /* loop counters */                         \
  arm_status status;                             /* status of matrix transpose  */           \
                                                                                             \
  const uint32_t *pIn2 = pIn + nColumns;                                                           \
  const uint32_t *pIn3 = pIn2 + nColumns;                                                          \
  const uint32_t *pIn4 = pIn3 + nColumns;

#define ARM_MAT_TRANS_NEON_INIT_U16                                                          \
  const uint16_t *pIn[8];                   /* input data matrix pointer */                  \
  uint16_t *pOut = (uint16_t*)pDst->pData;                 /* output data matrix pointer */  \
  uint16_t *pCol;                                 /* Temporary output data matrix pointer */ \
  uint16_t nRows = pSrc->numRows;                /* number of rows */                        \
  uint16_t nColumns = pSrc->numCols;             /* number of columns */                     \
                                                                                             \
  uint16_t blkCnt, rowCnt, row = nRows;          /* loop counters */                         \
  arm_status status;                             /* status of matrix transpose  */           \
                                                                                             \
  pIn[0] = (const uint16_t*)pSrc->pData;                  /* input data matrix pointer */    \
  for(int i=0;i<7;i++) {                                                                     \
    pIn[i+1] = pIn[i] + nColumns;                                                            \
  }                                                                                            \
 


#define ARM_MAT_TRANS_NEON_U32                                                        \
    /* Matrix transpose by exchanging the rows with columns */                        \
    /* Row loop */                                                                    \
    rowCnt = row >> BLOCK_ROWS_SHIFT;                                                 \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      uint32x4_t row0V,row1V,row2V,row3V;                                             \
      uint32x4x2_t ra0,ra1,rb0,rb1;                                                   \
                                                                                      \
      blkCnt = nColumns >> LANE_SHIFT;                                                \
                                                                                      \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      /* Compute 4 outputs at a time.  */                                             \
      /** a second loop below computes the remaining 1 to 3 samples. */               \
      while (blkCnt > 0U)        /* Column loop */                                    \
      {                                                                               \
        row0V = vld1q_u32(pIn);                                                       \
        pIn += LANE;                                                                  \
        row1V = vld1q_u32(pIn2);                                                      \
        pIn2 += LANE;                                                                 \
        row2V = vld1q_u32(pIn3);                                                      \
        pIn3 += LANE;                                                                 \
        row3V = vld1q_u32(pIn4);                                                      \
        pIn4 += LANE;                                                                 \
                                                                                      \
                                                                                      \
        ra0 = vzipq_u32(row0V,row2V);                                                 \
        ra1 = vzipq_u32(row1V,row3V);                                                 \
                                                                                      \
        rb0 = vzipq_u32(ra0.val[0],ra1.val[0]);                                       \
        rb1 = vzipq_u32(ra0.val[1],ra1.val[1]);                                       \
                                                                                      \
        vst1q_u32(pCol,rb0.val[0]);                                                   \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u32(pCol,rb0.val[1]);                                                   \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u32(pCol,rb1.val[0]);                                                   \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u32(pCol,rb1.val[1]);                                                   \
        pCol += nRows;                                                                \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      /* Perform matrix transpose for last <=3 samples here. */                       \
      blkCnt = nColumns & (LANE - 1);                                                 \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        *pCol++ = *pIn++;                                                             \
        *pCol++ = *pIn2++;                                                            \
        *pCol++ = *pIn3++;                                                            \
        *pCol++ = *pIn4++;                                                            \
                                                                                      \
        pCol += (nRows - BLOCK_ROWS);                                                 \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      pOut += BLOCK_ROWS;                                                             \
      pIn += (BLOCK_ROWS-1) * nColumns;                                               \
      pIn2 += (BLOCK_ROWS-1) * nColumns;                                              \
      pIn3 += (BLOCK_ROWS-1) * nColumns;                                              \
      pIn4 += (BLOCK_ROWS-1) * nColumns;                                              \
                                                                                      \
      /* Decrement the row loop counter */                                            \
      rowCnt--;                                                                       \
                                                                                      \
    }         /* Row loop end  */                                                     \
                                                                                      \
    rowCnt = row & (BLOCK_ROWS - 1);                                                  \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      blkCnt = nColumns ;                                                             \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        *pCol = *pIn++;                                                               \
                                                                                      \
        /* Update the pointer px to point to the next row of the transposed matrix */ \
        pCol += nRows;                                                                \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
      pOut++;                                                                         \
      rowCnt -- ;                                                                     \
    }


#define ARM_MAT_TRANS_NEON_U16                                                        \
    /* Matrix transpose by exchanging the rows with columns */                        \
    /* Row loop */                                                                    \
    rowCnt = row >> BLOCK_ROWS_SHIFT;                                                 \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      uint16x8_t rowV[8];                                                             \
      uint16x8x2_t tmpa[4];                                                           \
      uint16x8x2_t tmpb[4];                                                           \
                                                                                      \
                                                                                      \
      blkCnt = nColumns >> LANE_SHIFT;                                                \
                                                                                      \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      /* Compute 4 outputs at a time.  */                                             \
       /* a second loop below computes the remaining 1 to 3 samples. */               \
      while (blkCnt > 0U)        /* Column loop */                                    \
      {                                                                               \
        for(int i=0;i<8;i++) {                                                        \
          rowV[i] = vld1q_u16(pIn[i]);                                                \
          pIn[i] += LANE;                                                             \
        }                                                                             \
                                                                                      \
        /* We must go from a00 a01 */                                                 \
        /* to a00 a08 so an offset of 8 */                                            \
        /* Each unzip, taking the even numbered */                                    \
        /* if multipliying the offset per 2. */                                       \
        /* We need to multiply per 8 hence three sequences of */                      \
        /* uzpq */                                                                    \
        /* First sequence gives a00 a02 */                                            \
        tmpa[0]=vuzpq_u16(rowV[0],rowV[1]);                                           \
        tmpa[1]=vuzpq_u16(rowV[2],rowV[3]);                                           \
        tmpa[2]=vuzpq_u16(rowV[4],rowV[5]);                                           \
        tmpa[3]=vuzpq_u16(rowV[6],rowV[7]);                                           \
                                                                                      \
        /* Second sequences gives a00 a04 */                                          \
        /*                     a02 06 */                                              \
        tmpb[0]=vuzpq_u16(tmpa[0].val[0],tmpa[1].val[0]);                             \
        tmpb[1]=vuzpq_u16(tmpa[2].val[0],tmpa[3].val[0]);                             \
        tmpb[2]=vuzpq_u16(tmpa[0].val[1],tmpa[1].val[1]);                             \
        tmpb[3]=vuzpq_u16(tmpa[2].val[1],tmpa[3].val[1]);                             \
                                                                                      \
        /* Third sequence gives a00 a08  */                                           \
        tmpa[0]=vuzpq_u16(tmpb[0].val[0],tmpb[1].val[0]);                             \
        tmpa[1]=vuzpq_u16(tmpb[2].val[0],tmpb[3].val[0]);                             \
        tmpa[2]=vuzpq_u16(tmpb[0].val[1],tmpb[1].val[1]);                             \
        tmpa[3]=vuzpq_u16(tmpb[2].val[1],tmpb[3].val[1]);                             \
                                                                                      \
                                                                                      \
        vst1q_u16(pCol,tmpa[0].val[0]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u16(pCol,tmpa[1].val[0]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u16(pCol,tmpa[2].val[0]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u16(pCol,tmpa[3].val[0]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u16(pCol,tmpa[0].val[1]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u16(pCol,tmpa[1].val[1]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u16(pCol,tmpa[2].val[1]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u16(pCol,tmpa[3].val[1]);                                               \
        pCol += nRows;                                                                \
                                                                                      \
                                                                                      \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      /* Perform matrix transpose for last 3 samples here. */                         \
      blkCnt = nColumns & (LANE - 1);                                                 \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        for(int i=0;i<8;i++) {                                                        \
          *pCol++ = *pIn[i]++;                                                        \
        }                                                                             \
                                                                                      \
        pCol += (nRows - BLOCK_ROWS);                                                 \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      pOut += BLOCK_ROWS;                                                             \
      for(int i=0;i<8;i++) {                                                          \
        pIn[i] += (BLOCK_ROWS-1) * nColumns;                                          \
      }                                                                               \
                                                                                      \
                                                                                      \
      /* Decrement the row loop counter */                                            \
      rowCnt--;                                                                       \
                                                                                      \
    }         /* Row loop end  */                                                     \
                                                                                      \
    rowCnt = row & (BLOCK_ROWS - 1);                                                  \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      blkCnt = nColumns ;                                                             \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        *pCol = *pIn[0]++;                                                            \
                                                                                      \
        /* Update the pointer px to point to the next row of the transposed matrix */ \
        pCol += nRows;                                                                \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
      pOut++;                                                                         \
      rowCnt -- ;                                                                     \
    }

#define ARM_MAT_TRANS_NEON_INIT_U8                                                          \
  const uint8_t *pIn[16];                   /* input data matrix pointer */                 \
  uint8_t *pOut = (uint8_t*)pDst->pData;                 /* output data matrix pointer */   \
  uint8_t *pCol;                                 /* Temporary output data matrix pointer */ \
  uint8_t nRows = pSrc->numRows;                /* number of rows */                        \
  uint8_t nColumns = pSrc->numCols;             /* number of columns */                     \
                                                                                            \
  uint8_t blkCnt, rowCnt, row = nRows;          /* loop counters */                         \
  arm_status status;                             /* status of matrix transpose  */          \
                                                                                            \
  pIn[0] = (const uint8_t*)pSrc->pData;                  /* input data matrix pointer */    \
  for(int i=0;i<15;i++) {                                                                   \
    pIn[i+1] = pIn[i] + nColumns;                                                           \
  }


#define ARM_MAT_TRANS_NEON_U8                                                         \
    /* Matrix transpose by exchanging the rows with columns */                        \
    /* Row loop */                                                                    \
    rowCnt = row >> BLOCK_ROWS_SHIFT;                                                 \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      uint8x16_t rowV[16];                                                            \
      uint8x16x2_t tmpa[8];                                                           \
      uint8x16x2_t tmpb[8];                                                           \
                                                                                      \
                                                                                      \
      blkCnt = nColumns >> LANE_SHIFT;                                                \
                                                                                      \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      /* Compute 4 outputs at a time.  */                                             \
       /* a second loop below computes the remaining 1 to 3 samples. */               \
      while (blkCnt > 0U)        /* Column loop */                                    \
      {                                                                               \
        for(int i=0;i<16;i++) {                                                       \
          rowV[i] = vld1q_u8(pIn[i]);                                                 \
          pIn[i] += LANE;                                                             \
        }                                                                             \
                                                                                      \
        /* We must go from a00 a01 */                                                 \
        /* to a00 a016 so an offset of 16 */                                          \
        /* Each unzip, taking the even numbered */                                    \
        /* is multipliying the offset per 2. */                                       \
        /* We need to multiply per 16 hence four  sequences of */                     \
        /* uzpq */                                                                    \
        /* First sequence gives a00 a02 */                                            \
        tmpa[0]=vuzpq_u8(rowV[0],rowV[1]);                                            \
        tmpa[1]=vuzpq_u8(rowV[2],rowV[3]);                                            \
        tmpa[2]=vuzpq_u8(rowV[4],rowV[5]);                                            \
        tmpa[3]=vuzpq_u8(rowV[6],rowV[7]);                                            \
        tmpa[4]=vuzpq_u8(rowV[8],rowV[9]);                                            \
        tmpa[5]=vuzpq_u8(rowV[10],rowV[11]);                                          \
        tmpa[6]=vuzpq_u8(rowV[12],rowV[13]);                                          \
        tmpa[7]=vuzpq_u8(rowV[14],rowV[15]);                                          \
                                                                                      \
        /* Second sequences gives a00 a04 */                                          \
        /*                     a02 06 */                                              \
        tmpb[0]=vuzpq_u8(tmpa[0].val[0],tmpa[1].val[0]);                              \
        tmpb[1]=vuzpq_u8(tmpa[2].val[0],tmpa[3].val[0]);                              \
        tmpb[2]=vuzpq_u8(tmpa[4].val[0],tmpa[5].val[0]);                              \
        tmpb[3]=vuzpq_u8(tmpa[6].val[0],tmpa[7].val[0]);                              \
                                                                                      \
        tmpb[4]=vuzpq_u8(tmpa[0].val[1],tmpa[1].val[1]);                              \
        tmpb[5]=vuzpq_u8(tmpa[2].val[1],tmpa[3].val[1]);                              \
        tmpb[6]=vuzpq_u8(tmpa[4].val[1],tmpa[5].val[1]);                              \
        tmpb[7]=vuzpq_u8(tmpa[6].val[1],tmpa[7].val[1]);                              \
                                                                                      \
        /* Third sequence gives a00 a08  */                                           \
        tmpa[0]=vuzpq_u8(tmpb[0].val[0],tmpb[1].val[0]);                              \
        tmpa[1]=vuzpq_u8(tmpb[2].val[0],tmpb[3].val[0]);                              \
        tmpa[2]=vuzpq_u8(tmpb[4].val[0],tmpb[5].val[0]);                              \
        tmpa[3]=vuzpq_u8(tmpb[6].val[0],tmpb[7].val[0]);                              \
                                                                                      \
        tmpa[4]=vuzpq_u8(tmpb[0].val[1],tmpb[1].val[1]);                              \
        tmpa[5]=vuzpq_u8(tmpb[2].val[1],tmpb[3].val[1]);                              \
        tmpa[6]=vuzpq_u8(tmpb[4].val[1],tmpb[5].val[1]);                              \
        tmpa[7]=vuzpq_u8(tmpb[6].val[1],tmpb[7].val[1]);                              \
                                                                                      \
        /* Fourth sequence */                                                         \
        tmpb[0]=vuzpq_u8(tmpa[0].val[0],tmpa[1].val[0]);                              \
        tmpb[1]=vuzpq_u8(tmpa[2].val[0],tmpa[3].val[0]);                              \
        tmpb[2]=vuzpq_u8(tmpa[4].val[0],tmpa[5].val[0]);                              \
        tmpb[3]=vuzpq_u8(tmpa[6].val[0],tmpa[7].val[0]);                              \
                                                                                      \
        tmpb[4]=vuzpq_u8(tmpa[0].val[1],tmpa[1].val[1]);                              \
        tmpb[5]=vuzpq_u8(tmpa[2].val[1],tmpa[3].val[1]);                              \
        tmpb[6]=vuzpq_u8(tmpa[4].val[1],tmpa[5].val[1]);                              \
        tmpb[7]=vuzpq_u8(tmpa[6].val[1],tmpa[7].val[1]);                              \
                                                                                      \
                                                                                      \
        vst1q_u8(pCol,tmpb[0].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[1].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[2].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[3].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[4].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[5].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[6].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[7].val[0]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[0].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[1].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[2].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[3].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[4].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[5].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[6].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
        vst1q_u8(pCol,tmpb[7].val[1]);                                                \
        pCol += nRows;                                                                \
                                                                                      \
                                                                                      \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      /* Perform matrix transpose for last 3 samples here. */                         \
      blkCnt = nColumns & (LANE - 1);                                                 \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        for(int i=0;i<16;i++) {                                                       \
          *pCol++ = *pIn[i]++;                                                        \
        }                                                                             \
                                                                                      \
        pCol += (nRows - BLOCK_ROWS);                                                 \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      pOut += BLOCK_ROWS;                                                             \
      for(int i=0;i<16;i++) {                                                         \
        pIn[i] += (BLOCK_ROWS-1) * nColumns;                                          \
      }                                                                               \
                                                                                      \
                                                                                      \
      /* Decrement the row loop counter */                                            \
      rowCnt--;                                                                       \
                                                                                      \
    }         /* Row loop end  */                                                     \
                                                                                      \
    rowCnt = row & (BLOCK_ROWS - 1);                                                  \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      blkCnt = nColumns ;                                                             \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        *pCol = *pIn[0]++;                                                            \
                                                                                      \
        /* Update the pointer px to point to the next row of the transposed matrix */ \
        pCol += nRows;                                                                \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
      pOut++;                                                                         \
      rowCnt -- ;                                                                     \
    }

#define ARM_MAT_TRANS_NEON_INIT_COMPLEX_U32                                                              \
  const uint32_t *pIn =  (const uint32_t *)pSrc->pData;                  /* input data matrix pointer */ \
  uint32_t *pOut = (uint32_t *)pDst->pData;                 /* output data matrix pointer */             \
  uint32_t *pCol;                                 /* Temporary output data matrix pointer */             \
  uint16_t nRows = pSrc->numRows;                /* number of rows */                                    \
  uint16_t nColumns = pSrc->numCols;             /* number of columns */                                 \
                                                                                                         \
  uint16_t blkCnt, rowCnt, row = nRows;          /* loop counters */                                     \
  arm_status status;                             /* status of matrix transpose  */                       \
                                                                                                         \
  const uint32_t *pIn2 = pIn + 2*nColumns;                                                                 \
  const uint32_t *pIn3 = pIn2 + 2*nColumns;                                                                \
  const uint32_t *pIn4 = pIn3 + 2*nColumns;

#define ARM_MAT_TRANS_NEON_COMPLEX_U32                                                \
    /* Matrix transpose by exchanging the rows with columns */                        \
    /* Row loop */                                                                    \
    rowCnt = row >> BLOCK_ROWS_SHIFT;                                                 \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      uint32x4x2_t row0V,row1V,row2V,row3V;                                             \
      uint32x4x2_t res[4] ;                                                          \
                                                                                      \
      uint32x4x2_t tmpa[2],tmpb[2];                                                  \
                                                                                      \
      blkCnt = nColumns >> LANE_SHIFT;                                                \
                                                                                      \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      /* Compute 4 outputs at a time.  */                                             \
      /** a second loop below computes the remaining 1 to 3 samples. */               \
      while (blkCnt > 0U)        /* Column loop */                                    \
      {                                                                               \
        row0V = vld2q_u32(pIn);                                                       \
        pIn  += 2*LANE;                                                               \
        row1V = vld2q_u32(pIn2);                                                      \
        pIn2 += 2*LANE;                                                               \
        row2V = vld2q_u32(pIn3);                                                      \
        pIn3 += 2*LANE;                                                               \
        row3V = vld2q_u32(pIn4);                                                      \
        pIn4 += 2*LANE;                                                               \
                                                                                      \
                                                                                      \
        tmpa[0]=vuzpq_u32(row0V.val[0],row1V.val[0]);                                 \
        tmpa[1]=vuzpq_u32(row2V.val[0],row3V.val[0]);                                 \
                                                                                      \
        tmpb[0]=vuzpq_u32(tmpa[0].val[0],tmpa[1].val[0]);                             \
        tmpb[1]=vuzpq_u32(tmpa[0].val[1],tmpa[1].val[1]);                             \
                                                                                      \
        res[0].val[0] = tmpb[0].val[0];                                               \
        res[1].val[0] = tmpb[1].val[0];                                               \
        res[2].val[0] = tmpb[0].val[1];                                               \
        res[3].val[0] = tmpb[1].val[1];                                               \
                                                                                      \
        tmpa[0]=vuzpq_u32(row0V.val[1],row1V.val[1]);                                 \
        tmpa[1]=vuzpq_u32(row2V.val[1],row3V.val[1]);                                 \
                                                                                      \
        tmpb[0]=vuzpq_u32(tmpa[0].val[0],tmpa[1].val[0]);                             \
        tmpb[1]=vuzpq_u32(tmpa[0].val[1],tmpa[1].val[1]);                             \
                                                                                      \
        res[0].val[1] = tmpb[0].val[0];                                               \
        res[1].val[1] = tmpb[1].val[0];                                               \
        res[2].val[1] = tmpb[0].val[1];                                               \
        res[3].val[1] = tmpb[1].val[1];                                               \
                                                                                      \
        vst2q_u32(pCol,res[0]);                                                       \
        pCol += 2*nRows;                                                              \
                                                                                      \
        vst2q_u32(pCol,res[1]);                                                       \
        pCol += 2*nRows;                                                              \
                                                                                      \
        vst2q_u32(pCol,res[2]);                                                       \
        pCol += 2*nRows;                                                              \
                                                                                      \
        vst2q_u32(pCol,res[3]);                                                       \
        pCol += 2*nRows;                                                              \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      /* Perform matrix transpose for last <=3 samples here. */                       \
      blkCnt = nColumns & (LANE - 1);                                                 \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        *pCol++ = *pIn++;                                                             \
        *pCol++ = *pIn++;                                                             \
        *pCol++ = *pIn2++;                                                            \
        *pCol++ = *pIn2++;                                                            \
        *pCol++ = *pIn3++;                                                            \
        *pCol++ = *pIn3++;                                                            \
        *pCol++ = *pIn4++;                                                            \
        *pCol++ = *pIn4++;                                                            \
                                                                                      \
        pCol += 2*(nRows - BLOCK_ROWS);                                               \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      pOut += 2*BLOCK_ROWS;                                                           \
      pIn  += 2*(BLOCK_ROWS-1) * nColumns;                                             \
      pIn2 += 2*(BLOCK_ROWS-1) * nColumns;                                            \
      pIn3 += 2*(BLOCK_ROWS-1) * nColumns;                                            \
      pIn4 += 2*(BLOCK_ROWS-1) * nColumns;                                            \
                                                                                      \
      /* Decrement the row loop counter */                                            \
      rowCnt--;                                                                       \
                                                                                      \
    }         /* Row loop end  */                                                     \
                                                                                      \
    rowCnt = row & (BLOCK_ROWS - 1);                                                  \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      blkCnt = nColumns ;                                                             \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        pCol[0] = *pIn++;                                                             \
        pCol[1] = *pIn++;                                                             \
                                                                                      \
        /* Update the pointer px to point to the next row of the transposed matrix */ \
        pCol += 2*nRows;                                                              \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
      pOut += 2;                                                                        \
      rowCnt -- ;                                                                     \
    }                                                                            \

#define ARM_MAT_TRANS_NEON_INIT_COMPLEX_U16                                                  \
  const uint16_t *pIn[8];                   /* input data matrix pointer */                  \
  uint16_t *pOut = (uint16_t*)pDst->pData;                 /* output data matrix pointer */  \
  uint16_t *pCol;                                 /* Temporary output data matrix pointer */ \
  uint16_t nRows = pSrc->numRows;                /* number of rows */                        \
  uint16_t nColumns = pSrc->numCols;             /* number of columns */                     \
                                                                                             \
  uint16_t blkCnt, rowCnt, row = nRows;          /* loop counters */                         \
  arm_status status;                             /* status of matrix transpose  */           \
                                                                                             \
  pIn[0] = (const uint16_t*)pSrc->pData;                  /* input data matrix pointer */    \
  for(int i=0;i<7;i++) {                                                                     \
    pIn[i+1] = pIn[i] + 2*nColumns;                                                          \
  }


#define ARM_MAT_TRANS_NEON_COMPLEX_U16                                                \
    /* Matrix transpose by exchanging the rows with columns */                        \
    /* Row loop */                                                                    \
    rowCnt = row >> BLOCK_ROWS_SHIFT;                                                 \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      uint16x8x2_t rowV[8];                                                           \
      uint16x8x2_t tmpa[4];                                                           \
      uint16x8x2_t tmpb[4];                                                           \
      uint16x8x2_t res[8];                                                            \
                                                                                      \
                                                                                      \
      blkCnt = nColumns >> LANE_SHIFT;                                                \
                                                                                      \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      /* Compute 4 outputs at a time.  */                                             \
       /* a second loop below computes the remaining 1 to 3 samples. */               \
      while (blkCnt > 0U)        /* Column loop */                                    \
      {                                                                               \
        for(int i=0;i<8;i++) {                                                        \
          rowV[i] = vld2q_u16(pIn[i]);                                                \
          pIn[i] += 2*LANE;                                                           \
        }                                                                             \
                                                                                      \
        /* We must go from a00 a01 */                                                 \
        /* to a00 a08 so an offset of 8 */                                            \
        /* Each unzip, taking the even numbered */                                    \
        /* if multipliying the offset per 2. */                                       \
        /* We need to multiply per 8 hence three sequences of */                      \
        /* uzpq */                                                                    \
        /* First sequence gives a00 a02 */                                            \
        tmpa[0]=vuzpq_u16(rowV[0].val[0],rowV[1].val[0]);                             \
        tmpa[1]=vuzpq_u16(rowV[2].val[0],rowV[3].val[0]);                             \
        tmpa[2]=vuzpq_u16(rowV[4].val[0],rowV[5].val[0]);                             \
        tmpa[3]=vuzpq_u16(rowV[6].val[0],rowV[7].val[0]);                             \
                                                                                      \
        /* Second sequences gives a00 a04 */                                          \
        /*                     a02 06 */                                              \
        tmpb[0]=vuzpq_u16(tmpa[0].val[0],tmpa[1].val[0]);                             \
        tmpb[1]=vuzpq_u16(tmpa[2].val[0],tmpa[3].val[0]);                             \
        tmpb[2]=vuzpq_u16(tmpa[0].val[1],tmpa[1].val[1]);                             \
        tmpb[3]=vuzpq_u16(tmpa[2].val[1],tmpa[3].val[1]);                             \
                                                                                      \
        /* Third sequence gives a00 a08  */                                           \
        tmpa[0]=vuzpq_u16(tmpb[0].val[0],tmpb[1].val[0]);                             \
        tmpa[1]=vuzpq_u16(tmpb[2].val[0],tmpb[3].val[0]);                             \
        tmpa[2]=vuzpq_u16(tmpb[0].val[1],tmpb[1].val[1]);                             \
        tmpa[3]=vuzpq_u16(tmpb[2].val[1],tmpb[3].val[1]);                             \
                                                                                      \
        res[0].val[0] = tmpa[0].val[0];                                               \
        res[1].val[0] = tmpa[1].val[0];                                               \
        res[2].val[0] = tmpa[2].val[0];                                               \
        res[3].val[0] = tmpa[3].val[0];                                               \
        res[4].val[0] = tmpa[0].val[1];                                               \
        res[5].val[0] = tmpa[1].val[1];                                               \
        res[6].val[0] = tmpa[2].val[1];                                               \
        res[7].val[0] = tmpa[3].val[1];                                               \
                                                                                      \
                                                                                      \
                                                                                      \
        tmpa[0]=vuzpq_u16(rowV[0].val[1],rowV[1].val[1]);                             \
        tmpa[1]=vuzpq_u16(rowV[2].val[1],rowV[3].val[1]);                             \
        tmpa[2]=vuzpq_u16(rowV[4].val[1],rowV[5].val[1]);                             \
        tmpa[3]=vuzpq_u16(rowV[6].val[1],rowV[7].val[1]);                             \
                                                                                      \
        /* Second sequences gives a00 a04 */                                          \
        /*                     a02 06 */                                              \
        tmpb[0]=vuzpq_u16(tmpa[0].val[0],tmpa[1].val[0]);                             \
        tmpb[1]=vuzpq_u16(tmpa[2].val[0],tmpa[3].val[0]);                             \
        tmpb[2]=vuzpq_u16(tmpa[0].val[1],tmpa[1].val[1]);                             \
        tmpb[3]=vuzpq_u16(tmpa[2].val[1],tmpa[3].val[1]);                             \
                                                                                      \
        /* Third sequence gives a00 a08  */                                           \
        tmpa[0]=vuzpq_u16(tmpb[0].val[0],tmpb[1].val[0]);                             \
        tmpa[1]=vuzpq_u16(tmpb[2].val[0],tmpb[3].val[0]);                             \
        tmpa[2]=vuzpq_u16(tmpb[0].val[1],tmpb[1].val[1]);                             \
        tmpa[3]=vuzpq_u16(tmpb[2].val[1],tmpb[3].val[1]);                             \
                                                                                      \
        res[0].val[1] = tmpa[0].val[0];                                               \
        res[1].val[1] = tmpa[1].val[0];                                               \
        res[2].val[1] = tmpa[2].val[0];                                               \
        res[3].val[1] = tmpa[3].val[0];                                               \
        res[4].val[1] = tmpa[0].val[1];                                               \
        res[5].val[1] = tmpa[1].val[1];                                               \
        res[6].val[1] = tmpa[2].val[1];                                               \
        res[7].val[1] = tmpa[3].val[1];                                               \
                                                                                      \
        for(int i=0;i<8;i++) {                                                        \
          vst2q_u16(pCol,res[i]);                                                     \
          pCol += 2*nRows;                                                            \
        }                                                                             \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      /* Perform matrix transpose for last 3 samples here. */                         \
      blkCnt = nColumns & (LANE - 1);                                                 \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        for(int i=0;i<8;i++) {                                                        \
          *pCol++ = *pIn[i]++;                                                        \
          *pCol++ = *pIn[i]++;                                                        \
        }                                                                             \
                                                                                      \
        pCol += 2*(nRows - BLOCK_ROWS);                                               \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
                                                                                      \
      pOut += 2*BLOCK_ROWS;                                                             \
      for(int i=0;i<8;i++) {                                                          \
        pIn[i] += 2*(BLOCK_ROWS-1) * nColumns;                                        \
      }                                                                               \
                                                                                      \
                                                                                      \
      /* Decrement the row loop counter */                                            \
      rowCnt--;                                                                       \
                                                                                      \
    }         /* Row loop end  */                                                     \
                                                                                      \
    rowCnt = row & (BLOCK_ROWS - 1);                                                  \
    while (rowCnt > 0U)                                                               \
    {                                                                                 \
      blkCnt = nColumns ;                                                             \
      /* The pointer px is set to starting address of the column being processed */   \
      pCol = pOut;                                                                    \
                                                                                      \
      while (blkCnt > 0U)                                                             \
      {                                                                               \
        /* Read and store the input element in the destination */                     \
        pCol[0] = *pIn[0]++;                                                          \
        pCol[1] = *pIn[0]++;                                                          \
                                                                                      \
        /* Update the pointer px to point to the next row of the transposed matrix */ \
        pCol += 2*nRows;                                                              \
                                                                                      \
        /* Decrement the column loop counter */                                       \
        blkCnt--;                                                                     \
      }                                                                               \
      pOut += 2;                                                                      \
      rowCnt -- ;                                                                     \
    }
