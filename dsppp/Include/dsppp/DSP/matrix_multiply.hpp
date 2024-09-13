// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_MATH_DSP
#undef ARM_MATH_MVEI
#undef ARM_MATH_MVEF
#undef ARM_MATH_NEON
#endif

/** \addtogroup DSPALG 
 *  @{
 */

#if defined(ARM_MATH_DSP)
#if !defined(ARM_MATH_MVEI) && !defined(ARM_MATH_MVEF) && !defined(ARM_MATH_NEON)

template<typename MA,
         typename MB,
         typename std::enable_if<
         !is_complex<MA>() &&
         !is_complex<MB>() &&
         std::is_same<typename traits<MA>::Scalar,Q15>::value &&
         number_traits<typename traits<MA>::Scalar>::is_fixed,bool>::type = true>
__STATIC_INLINE void _arm_mat_trans(
    const MA    &src,
    MB          &dst,
    const DSP* = nullptr)
{
    using T = typename traits<MA>::Scalar;
    using VEC = typename vector_traits<T>::vector;
    constexpr int nb_lanes = vector_traits<T>::nb_lanes;

    T *pIn = src.ptr();                      /* input data matrix pointer */
    T *pOut = dst.ptr();                     /* output data matrix pointer */
    uint16_t nRows = src.rows();                /* number of rows */
    uint16_t nCols = src.columns();                /* number of columns */
    uint32_t col, row = nRows, i = 0U;             /* Loop counters */

    VEC in;                                      /* variable to hold temporary output  */

    /* Matrix transpose by exchanging the rows with columns */
    /* row loop */
    do
    {
      /* Pointer pOut is set to starting address of column being processed */
      pOut = dst.ptr() + i;


      /* Loop unrolling: Compute 4 outputs at a time */
      col = nCols / (2*nb_lanes);

      while (col > 0U)        /* column loop */
      {
        /* Read two elements from row */
        in = inner::vload1<1>(pIn);
        pIn += nb_lanes;

        /* Unpack and store one element in  destination */
        *pOut = Q15(in.v);
        /* Update pointer pOut to point to next row of transposed matrix */
        pOut += dst.stride();

        /* Unpack and store second element in destination */
        *pOut = Q15((in.v & (q31_t) 0xffff0000) >> 16);
        /* Update  pointer pOut to point to next row of transposed matrix */
        pOut += dst.stride();

        /* Read two elements from row */
        in = inner::vload1<1>(pIn);
        pIn += nb_lanes;

        /* Unpack and store one element in destination */
        *pOut = Q15(in.v);
        /* Update pointer pOut to point to next row of transposed matrix */
        pOut += dst.stride();

        /* Unpack and store second element in destination */
        *pOut = Q15((in & (q31_t) 0xffff0000) >> 16);
        /* Update pointer pOut to point to next row of transposed matrix */
        pOut += dst.stride();

        /* Decrement column loop counter */
        col--;
      }

      /* Loop unrolling: Compute remaining outputs */
      col = nCols & (2*nb_lanes-1);
      while (col > 0U)
      {
        /* Read and store input element in destination */
        *pOut = *pIn++;

        /* Update pointer pOut to point to next row of transposed matrix */
        pOut += dst.stride();

        /* Decrement column loop counter */
        col--;
      }

      pIn += (src.stride()-nCols);

      i ++;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);          /* row loop end */


  
}


template<typename M,
         typename V,
         typename RES,
         typename std::enable_if<
         !is_complex<M>() &&
         !is_complex<V>() &&
         !is_complex<RES>() &&
         !std::is_same<typename traits<M>::Scalar,Q31>::value &&
         number_traits<typename traits<M>::Scalar>::is_fixed,bool>::type = true>
inline void _dot_m_v(RES &res,
                     const M&m,const V&v,
                     const DSP* = nullptr)
{
    using T = typename traits<M>::Scalar;
    using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;
    constexpr int nb_lanes = vector_traits<T>::nb_lanes;

    uint32_t numRows = m.rows();
    uint32_t numCols = m.columns();
    const T *pSrcA = m.ptr();
    const T *pInA1;      /* input data matrix pointer A of Q15 type */
    const T *pInA2;      /* input data matrix pointer A of Q15 type */
    const T *pInA3;      /* input data matrix pointer A of Q15 type */
    const T *pInA4;      /* input data matrix pointer A of Q15 type */
    T *px;               /* Temporary output data matrix pointer */
    uint16_t i, row; /* loop counters */
    int16_t colCnt;
    VEC matData, matData2, vecData, vecData2;
    T tmpData;


    /* Process 4 rows at a time */
    row = numRows >> 2;
    i = 0u;
    px = res.ptr();

    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    /* row loop */
    while (row > 0) {
        /* Initialize accumulators */
        ACC sum1 = ACC{};
        ACC sum2 = ACC{};
        ACC sum3 = ACC{};
        ACC sum4 = ACC{};

        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */

        /* Loop unrolling: process 2 columns per iteration */

        /* Initialize pointers to the starting address of the column being processed */
        pInA1 = pSrcA + i;
        pInA2 = pInA1 + m.stride();
        pInA3 = pInA2 + m.stride();
        pInA4 = pInA3 + m.stride();

        // Main loop: matrix-vector multiplication
        for(colCnt = 0 ; colCnt <= (int16_t)numCols - nb_lanes; colCnt += nb_lanes)
        {
            // Read 2 values from vector
            vecData = v.vector_op(colCnt);

            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData =  inner::vload1<1>  (pInA1);
            pInA1 += nb_lanes;
            sum1 = inner::vmacc(sum1, matData, vecData);

            matData = inner::vload1<1>  (pInA2);
            pInA2 += nb_lanes;
            sum2 = inner::vmacc(sum2, matData, vecData);

            matData = inner::vload1<1>  (pInA3);
            pInA3 += nb_lanes;
            sum3 = inner::vmacc(sum3, matData, vecData);

            matData = inner::vload1<1>  (pInA4);
            pInA4 += nb_lanes;
            sum4 = inner::vmacc(sum4, matData, vecData);

            // Decrement the loop counter
        }

        /* process any remaining columns */

        for(; colCnt < (int16_t)numCols; colCnt ++)
        {
            tmpData = v[colCnt];
            sum1 = inner::mac(sum1,*pInA1++ , tmpData);
            sum2 = inner::mac(sum2,*pInA2++ , tmpData);
            sum3 = inner::mac(sum3,*pInA3++ , tmpData);
            sum4 = inner::mac(sum4,*pInA4++ , tmpData);
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = inner::from_accumulator(sum1);
        *px++ = inner::from_accumulator(sum2);
        *px++ = inner::from_accumulator(sum3);
        *px++ = inner::from_accumulator(sum4);

        i = i + m.stride() * 4;

        /* Decrement the row loop counter */
        row--;
    }

    /* process any remaining rows */
    row = numRows & 3u;
    while (row > 0) {

        ACC sum = ACC{};
        pInA1 = pSrcA + i;

        // loop unrolling - process 4 elements at a time

        for(colCnt = 0 ; colCnt <= (int16_t)numCols - 2*nb_lanes; colCnt += 2*nb_lanes)
        {
            vecData = v.vector_op(colCnt);
            vecData2 = v.vector_op(colCnt+nb_lanes);

            matData = inner::vload1<1>(pInA1);
            pInA1 += nb_lanes;
            matData2 = inner::vload1<1>(pInA1);
            pInA1 += nb_lanes;
            sum = inner::vmacc(sum, matData, vecData);
            sum = inner::vmacc(sum, matData2, vecData2);
        }

        // process remainder of row
        for(; colCnt < (int16_t)numCols; colCnt ++)
        {

            sum = inner::mac(sum, *pInA1++ , v[colCnt]);
        }
        *px++ = inner::from_accumulator(sum);
        i = i + m.stride();
        row--;
    }
}

template<typename MA,
         typename MB,
         typename RES,
         typename TMP,
         typename std::enable_if<
         !is_complex<MA>() &&
         !is_complex<MB>() &&
         !is_complex<RES>() &&
         !is_complex<TMP>() &&
         !std::is_same<typename traits<MA>::Scalar,Q31>::value &&
         number_traits<typename traits<MA>::Scalar>::is_fixed,bool>::type = true>
__STATIC_INLINE void _dot_m_m(const MA&pSrcA,const MB&pSrcB,
                     RES &&pDst,
                     const TMP &BT,
                     const DSP* = nullptr)
{
  using T = typename traits<MA>::Scalar;
  using ACC = typename vector_traits<T>::temp_accumulator;
  using VEC = typename vector_traits<T>::vector;
  constexpr int nb_lanes = vector_traits<T>::nb_lanes;

          ACC sum;                                     /* Accumulator */


        T *pSrcBT = BT.ptr();                        /* Input data matrix pointer for transpose */
        T *pInA = pSrcA.ptr();                    /* Input data matrix pointer A of Q15 type */
        T *pInB = pSrcB.ptr();                    /* Input data matrix pointer B of Q15 type */
        T *px;                                     /* Temporary output data matrix pointer */
        uint16_t numRowsA = pSrcA.rows();            /* Number of rows of input matrix A */
        uint16_t numColsB = pSrcB.columns();            /* Number of columns of input matrix B */
        uint16_t numColsA = pSrcA.columns();            /* Number of columns of input matrix A */
        uint16_t numRowsB = pSrcB.rows();            /* Number of rows of input matrix B */
        uint32_t col, i = 0U, row = numRowsB, colCnt;  /* Loop counters */

        VEC inA1, inB1, inA2, inB2;

    
    /* Reset variables for usage in following multiplication process */
    row = numRowsA;
    i = 0U;
    px = pDst.ptr();

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of transposed pSrcB data */
      pInB = pSrcBT;

      /* column loop */
      do
      {
        /* Set variable sum, that acts as accumulator, to zero */
        sum = ACC{};

        /* Initiate pointer pInA to point to starting address of column being processed */
        pInA = pSrcA.ptr() + i;

        /* Apply loop unrolling and compute 2 MACs simultaneously. */
        colCnt = numColsA / (2*nb_lanes);

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* read real and imag values from pSrcA and pSrcB buffer */
          inA1 = inner::vload1<1> (pInA);
          pInA += nb_lanes;
          inB1 = inner::vload1<1> (pInB);
          pInB += nb_lanes;

          inA2 = inner::vload1<1> (pInA);
          pInA += nb_lanes;
          inB2 = inner::vload1<1> (pInB);
          pInB += nb_lanes;

          /* Multiply and Accumulates */
          sum = inner::vmacc(sum, inA1, inB1);
          sum = inner::vmacc(sum, inA2, inB2);

          /* Decrement loop counter */
          colCnt--;
        }

        /* process remaining column samples */
        colCnt = numColsA & (2*nb_lanes-1);

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
          sum = inner::mac(sum ,*pInA++ , *pInB++);

          /* Decrement loop counter */
          colCnt--;
        }

        /* Saturate and store result in destination buffer */
        *px = inner::from_accumulator(sum);
        px++;

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      i = i + pSrcA.stride();

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

}
#endif
#endif

/*! @} */