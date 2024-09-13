// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup SCALARALG
 *  @{
 */

/**
 * @brief      Matrix times matrix for scalar architecture and fixed point
 *
 * @param[in]  pSrcA      The source a
 * @param[in]  pSrcB      The source b
 * @param      pDst       The destination
 * @param[in]  BT         Temporary matrix for transposition
 *
 * @tparam     MA         Left hand side datatype
 * @tparam     MB         Right hand side datatype
 * @tparam     RES        Destination datatype
 * @tparam     TMP        Temporary matrix datatype
 * @tparam     <unnamed>  Check fixed point arithmetic used
 */
template<typename MA,
         typename MB,
         typename RES,
         typename TMP,
         typename std::enable_if<
         number_traits<typename traits<MA>::Scalar>::is_fixed,bool>::type = true>
__STATIC_INLINE void _dot_m_m(const MA&pSrcA,const MB&pSrcB,
                     RES &&pDst,
                     const TMP &BT,
                     const Scalar* = nullptr)
{
  using TA = typename traits<MA>::Scalar;
  using TB = typename traits<MB>::Scalar;

  using T = typename MixedRes<TA,TB>::type;
  using Acc = typename number_traits<T>::accumulator;

  TA *pIn1 = pSrcA.ptr();                    /* Input data matrix pointer A */
  TB *pIn2 = pSrcB.ptr();                    /* Input data matrix pointer B */
  TA *pInA = pSrcA.ptr();                    /* Input data matrix pointer A */
  TB *pInB = pSrcB.ptr();                    /* Input data matrix pointer B */
  T *pOut = pDst.ptr();                     /* Output data matrix pointer */
  T *px;                                     /* Temporary output data matrix pointer */
  Acc sum;                                     /* Accumulator */
  uint16_t numRowsA = pSrcA.rows();            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB.columns();            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA.columns();            /* Number of columns of input matrix A */
  uint32_t col, i = 0U, row = numRowsA, colCnt;  /* Loop counters */

  (void)BT;
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
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* Perform the multiply-accumulates */
          sum = inner::mac(sum, *pIn1++, *pIn2);
          pIn2 += pSrcB.stride();

          sum = inner::mac(sum,*pIn1++, *pIn2);
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
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* Perform the multiply-accumulates */
          sum = inner::mac(sum ,*pIn1++, *pIn2);
          pIn2 += pSrcB.stride();

          /* Decrement loop counter */
          colCnt--;
        }

        /* Convert result from 2.62 to 1.31 format and store in destination buffer */
        *px++ = inner::from_accumulator(sum);

        /* Decrement column loop counter */
        col--;

        /* Update pointer pIn2 to point to starting address of next column */
        pIn2 = pInB + (numColsB - col) ;

      } while (col > 0U);

      /* Update pointer pInA to point to starting address of next row */
      i = i + pDst.stride();
      pInA = pInA + pSrcA.stride();

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);


}


/*! @} */