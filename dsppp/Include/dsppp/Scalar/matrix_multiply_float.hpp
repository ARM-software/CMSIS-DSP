// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup SCALARALG 
 *  @{
 */

/**
 * @brief      Matrix times matrix for scalar architecture and float
 *
 * @param[in]  pSrcA      The source a
 * @param[in]  pSrcB      The source b
 * @param      pDst       The destination
 *
 * @tparam     MA         Left hand side datatype
 * @tparam     MB         Right hand side datatype
 * @tparam     RES        Result datatype
 * @tparam     <unnamed>  Check if float
 */
template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<is_float<MA>(),bool>::type = true>
__STATIC_INLINE void _dot_m_m(const MA&    pSrcA,
                              const MB&    pSrcB,
                                    RES && pDst,
                              const Scalar* = nullptr)
{
  std::cout << "Mat mul scalar\n";
  using TA = typename traits<MA>::Scalar;
  using TB = typename traits<MB>::Scalar;
  using T = typename MixedRes<TA,TB>::type;

  using Acc = typename number_traits<T>::accumulator;
  //using Comp = typename number_traits<T>::compute_type;
  TA *pIn1 = pSrcA.ptr();                /* Input data matrix pointer A */
  TB *pIn2 = pSrcB.ptr();                /* Input data matrix pointer B */
  TA *pInA = pSrcA.ptr();                /* Input data matrix pointer A */
  TB *pInB = pSrcB.ptr();                /* Input data matrix pointer B */
  T *pOut = pDst.ptr();                 /* Output data matrix pointer */
  T *px;                                 /* Temporary output data matrix pointer */
  Acc sum;                                 /* Accumulator */
  uint16_t numRowsA = pSrcA.rows();            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB.columns();            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA.columns();            /* Number of columns of input matrix A */
  uint32_t col, i = 0U, row = numRowsA, colCnt;  /* Loop counters */


    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of row being processed */
      px = pOut + i;

      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of pSrcB data */
      pIn2 = pSrcB.ptr();

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = Acc{};

        /* Initialize pointer pIn1 to point to starting address of column being processed */
        pIn1 = pInA;


        /* Loop unrolling: Compute 4 MACs at a time. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,p) = a(m,1) * b(1,p) + a(m,2) * b(2,p) + .... + a(m,n) * b(n,p) */

          /* Perform the multiply-accumulates */
          sum = inner::mac(sum, *pIn1++, *pIn2);
          pIn2 += pSrcB.stride();

          sum = inner::mac(sum, *pIn1++, *pIn2);
          pIn2 += pSrcB.stride();

          sum = inner::mac(sum, *pIn1++, *pIn2);
          pIn2 += pSrcB.stride();

          sum = inner::mac(sum, *pIn1++, *pIn2);
          pIn2 += pSrcB.stride();

          /* Decrement loop counter */
          colCnt--;
        }

        /* Loop unrolling: Compute remaining MACs */
        colCnt = numColsA % 0x4U;

        while (colCnt > 0U)
        {
          /* c(m,p) = a(m,1) * b(1,p) + a(m,2) * b(2,p) + .... + a(m,n) * b(n,p) */

          /* Perform the multiply-accumulates */
          sum = inner::mac(sum, *pIn1++, *pIn2);
          pIn2 += pSrcB.stride();

          /* Decrement loop counter */
          colCnt--;
        }

        /* Store result in destination buffer */
        *px++ = inner::from_accumulator(sum);

        /* Decrement column loop counter */
        col--;

        /* Update pointer pIn2 to point to starting address of next column */
        pIn2 = pInB + (numColsB - col);

      } while (col > 0U);

      /* Update pointer pInA to point to starting address of next row */
      i = i + pDst.stride();
      pInA = pInA + pSrcA.stride();

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

  
}

/*! @} */