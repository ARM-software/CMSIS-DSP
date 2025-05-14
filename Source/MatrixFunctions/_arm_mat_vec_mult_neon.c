ARM_DSP_ATTRIBUTE void FUNCNAME(const MATRIX_TYPE *pSrcMat, 
                                const VEC_SCALAR_DT *pSrcVec, 
                                      VEC_SCALAR_DT *pDstVec)
{
    uint32_t         numRows = pSrcMat->numRows;
    uint32_t         numCols = pSrcMat->numCols;
    const MAT_SCALAR_DT *pSrcA = pSrcMat->pData;
    
    int32_t          row;
    uint32_t         blkCnt;           /* loop counters */
    TMP_DEFINE_AND_INIT(tmp);
    

    row = numRows;

    /*
     * compute 4 rows in parallel
     */
    while (row >= 4)
    {
        const MAT_SCALAR_DT     *pInA0,  *pInA1,*pInA2, *pInA3;
        const MAT_SCALAR_DT     *pSrcA0, *pSrcA1, *pSrcA2, *pSrcA3;
        const VEC_SCALAR_DT     *pInVec;
        VECTOR_DT               vecIn;
        VECTOR_ACC              acc0, acc1, acc2, acc3;
        SCALAR_ACC              tmpScalarAcc0,tmpScalarAcc1,tmpScalarAcc2,tmpScalarAcc3;


        /*
         * Initialize the pointers to 4 consecutive MatrixA rows
         */
        pInA0 = pSrcA;
        pInA1 = pInA0 + numCols;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        /*
         * Initialize the vector pointer
         */
        pInVec =  pSrcVec;
        /*
         * reset accumulators
         */
        VECTOR_ACC_INIT(acc0);
        VECTOR_ACC_INIT(acc1);
        VECTOR_ACC_INIT(acc2);
        VECTOR_ACC_INIT(acc3);
       
        SCALAR_ACC_INIT(tmpScalarAcc0);
        SCALAR_ACC_INIT(tmpScalarAcc1);
        SCALAR_ACC_INIT(tmpScalarAcc2);
        SCALAR_ACC_INIT(tmpScalarAcc3);
       

        pSrcA0 = pInA0;
        pSrcA1 = pInA1;
        pSrcA2 = pInA2;
        pSrcA3 = pInA3;

        blkCnt = numCols >> NBLANE_SHIFT;
        while (blkCnt > 0U)
        {
            VECTOR_DT vecA0, vecA1, vecA2, vecA3;

            VEC_LOAD(vecIn, pInVec);    
            pInVec += NBLANE;

            VEC_LOAD(vecA0, pSrcA0);
            pSrcA0 += NBLANE;
            VEC_LOAD(vecA1, pSrcA1);
            pSrcA1 += NBLANE;
            VEC_LOAD(vecA2, pSrcA2);
            pSrcA2 += NBLANE;
            VEC_LOAD(vecA3, pSrcA3);
            pSrcA3 += NBLANE;

            VMAC(acc0, vecIn, vecA0);
            VMAC(acc1, vecIn, vecA1);
            VMAC(acc2, vecIn, vecA2);
            VMAC(acc3, vecIn, vecA3);
           
            blkCnt--;
        }

        REDUCE(tmpScalarAcc0 , acc0);
        REDUCE(tmpScalarAcc1 , acc1);
        REDUCE(tmpScalarAcc2 , acc2);
        REDUCE(tmpScalarAcc3 , acc3);
        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & (NBLANE - 1);
        while (blkCnt > 0U)
        {
            SCALAR_MAC(tmpScalarAcc0,*pSrcA0++,*pInVec);
            SCALAR_MAC(tmpScalarAcc1,*pSrcA1++,*pInVec);
            SCALAR_MAC(tmpScalarAcc2,*pSrcA2++,*pInVec);
            SCALAR_MAC(tmpScalarAcc3,*pSrcA3++,*pInVec);
           
            pInVec++;

            blkCnt--;
        }
        /*
         * Sum the partial parts
         */
        STORE_SCALAR_ACC(*pDstVec++,tmpScalarAcc0);
        STORE_SCALAR_ACC(*pDstVec++,tmpScalarAcc1);
        STORE_SCALAR_ACC(*pDstVec++,tmpScalarAcc2);
        STORE_SCALAR_ACC(*pDstVec++,tmpScalarAcc3);
       

        pSrcA += numCols * 4;
        /*
         * Decrement the row loop counter
         */
        row -= 4;
    }

    /*
     * compute 2 rows in parallel
     */
    if (row >= 2)
    {
        const MAT_SCALAR_DT *pInA0,*pInA1;
        const MAT_SCALAR_DT *pSrcA0, *pSrcA1;
        const VEC_SCALAR_DT *pInVec;
        VECTOR_DT           vecIn;
        VECTOR_ACC          acc0, acc1;
        SCALAR_ACC          tmpScalarAcc0,tmpScalarAcc1;

        /*
         * Initialize the pointers to 2 consecutive MatrixA rows
         */
        pInA0 = pSrcA;
        pInA1 = pInA0 + numCols;
        /*
         * Initialize the vector pointer
         */
        pInVec = pSrcVec;
        /*
         * reset accumulators
         */
        VECTOR_ACC_INIT(acc0);
        VECTOR_ACC_INIT(acc1);

        SCALAR_ACC_INIT(tmpScalarAcc0);
        SCALAR_ACC_INIT(tmpScalarAcc1);

        pSrcA0 = pInA0;
        pSrcA1 = pInA1;

        blkCnt = numCols >> NBLANE_SHIFT;
        while (blkCnt > 0U)
        {
            VECTOR_DT vecA0, vecA1;

            VEC_LOAD(vecIn, pInVec);    
            pInVec += NBLANE;

            VEC_LOAD(vecA0, pSrcA0);
            pSrcA0 += NBLANE;
            VEC_LOAD(vecA1, pSrcA1);
            pSrcA1 += NBLANE;

            VMAC(acc0, vecIn, vecA0);
            VMAC(acc1, vecIn, vecA1);

            blkCnt--;
        }

        REDUCE(tmpScalarAcc0 , acc0);
        REDUCE(tmpScalarAcc1 , acc1);

        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & (NBLANE - 1);
        while (blkCnt > 0U)
        {
            SCALAR_MAC(tmpScalarAcc0,*pSrcA0++,*pInVec);
            SCALAR_MAC(tmpScalarAcc1,*pSrcA1++,*pInVec);
           
            pInVec++;

            blkCnt--;
        }
        /*
         * Sum the partial parts
         */
        STORE_SCALAR_ACC(*pDstVec++,tmpScalarAcc0);
        STORE_SCALAR_ACC(*pDstVec++,tmpScalarAcc1);

        pSrcA += numCols * 2;
        row -= 2;
    }

    if (row >= 1)
    {
        const MAT_SCALAR_DT   *pInA0;
        const MAT_SCALAR_DT   *pSrcA0;
        const VEC_SCALAR_DT   *pInVec;
        VECTOR_DT             vecIn;
        VECTOR_ACC            acc0;
        SCALAR_ACC            tmpScalarAcc0;

        /*
         * Initialize the pointers to last MatrixA row
         */
        pInA0 = pSrcA;
        /*
         * Initialize the vector pointer
         */
        pInVec = pSrcVec;
        /*
         * reset accumulators
         */
        VECTOR_ACC_INIT(acc0);
        SCALAR_ACC_INIT(tmpScalarAcc0);

        pSrcA0 = pInA0;

        blkCnt = numCols >> NBLANE_SHIFT;
        while (blkCnt > 0U)
        {
            VECTOR_DT vecA0;

            VEC_LOAD(vecIn, pInVec);    
            pInVec += NBLANE;

            VEC_LOAD(vecA0, pSrcA0);
            pSrcA0 += NBLANE;

            VMAC(acc0, vecIn, vecA0);

            blkCnt--;
        }
        REDUCE(tmpScalarAcc0 , acc0);

        /*
         * tail
         * (will be merged thru tail predication)
         */
        blkCnt = numCols & (NBLANE - 1);
        while (blkCnt > 0U)
        {
            SCALAR_MAC(tmpScalarAcc0,*pSrcA0++,*pInVec);
            pInVec++;
            
            blkCnt--;
        }
        /*
         * Sum the partial parts
         */
        STORE_SCALAR_ACC(*pDstVec++,tmpScalarAcc0);
    }
}

#undef MAT_SCALAR_DT
#undef VEC_SCALAR_DT
#undef VECTOR_ACC
#undef NBLANE 
#undef NBLANE_SHIFT
#undef REDUCE
#undef HALF_VECTOR_ACC
#undef SCALAR_ACC
#undef VECTOR_DT
#undef TMP_DEFINE_AND_INIT
#undef SCALAR_ACC_INIT 
#undef VEC_LOAD 
#undef VMAC
#undef SCALAR_MAC
#undef STORE_SCALAR_ACC
#undef MATRIX_TYPE 
#undef FUNCNAME
#undef VECTOR_ACC_INIT 
