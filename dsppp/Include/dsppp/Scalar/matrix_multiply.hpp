// -*- C++ -*-
/** @file */ 
#pragma once 


/** \addtogroup SCALARALG
 *  @{
 */

/**
 * @brief      Transposition for scalar architecture
 *
 * @param[in]  src        The source
 * @param      dst        The destination
 *
 * @tparam     MA         Source datatype
 * @tparam     MB         Destination datatype
 */
template<typename MA,
         typename MB>
__STATIC_INLINE void _arm_mat_trans(
    const MA    &src,
    MB          &dst,
    const Scalar* = nullptr)
{
  DISABLE_LOOP_UNROLL
  for(index_t r=0;r < dst.rows() ; r++)
  {
        dst.row(r) = copy(src.col(r));
  }
}

/**
 * @brief      Matrix times vector for scalar architecture
 *
 * @param      res        Destination
 * @param[in]  m          Matrix
 * @param[in]  v          Vector (my be expression)
 *
 * @tparam     M          Matrix datatype
 * @tparam     V          Vector datatype
 * @tparam     RES        Result datatype
 */
template<typename M,
         typename V,
         typename RES>
inline void _dot_m_v(RES &res,
                     const M&m,const V&v,
                     const Scalar* = nullptr)
{
    using TM = typename traits<M>::Scalar;
    using TV = typename traits<V>::Scalar;
    using T = typename MixedRes<TM,TV>::type;

    using Acc = typename number_traits<T>::accumulator;
    uint32_t numRows = m.rows();
    uint32_t numCols = m.columns();
    const TM *pSrcA = m.ptr();
    const TM *pInA1;      /* input data matrix pointer A of Q31 type */
    const TM *pInA2;      /* input data matrix pointer A of Q31 type */
    const TM *pInA3;      /* input data matrix pointer A of Q31 type */
    const TM *pInA4;      /* input data matrix pointer A of Q31 type */
    T *px;               /* Temporary output data matrix pointer */
    uint32_t i;
    uint16_t row, colCnt; /* loop counters */
    TM matData, matData2;
    TV vecData, vecData2;


    /* Process 4 rows at a time */
    row = numRows >> 2;
    i = 0u;
    px = res.ptr();

    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    /* row loop */
    while (row > 0) {
        /* Initialize accumulators */
        Acc sum1 = Acc{};
        Acc sum2 = Acc{};
        Acc sum3 = Acc{};
        Acc sum4 = Acc{};

        
        /* Loop unrolling: process 2 columns per iteration */
        //colCnt = numCols;

        /* Initialize pointers to the starting address of the column being processed */
        pInA1 = pSrcA + i;
        pInA2 = pInA1 + m.stride();
        pInA3 = pInA2 + m.stride();
        pInA4 = pInA3 + m.stride();


        // Main loop: matrix-vector multiplication
        for(colCnt = 0 ; colCnt < numCols; colCnt ++)
        {
            // Read 2 values from vector
            vecData = v[colCnt];
            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData = *(pInA1)++;
            sum1 = inner::mac(sum1, matData, vecData);
            matData = *(pInA2)++;
            sum2 = inner::mac(sum2, matData, vecData);
            matData = *(pInA3)++;
            sum3 = inner::mac(sum3, matData, vecData);
            matData = *(pInA4)++;
            sum4 = inner::mac(sum4, matData, vecData);
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

        Acc sum = Acc{};
        pInA1 = pSrcA + i;
       
        int32_t k=0;
        for(k=0; k <= (int)numCols-2; k += 2)
        {
            vecData = v[k];
            vecData2 = v[k+1];
            matData = *(pInA1)++;
            matData2 = *(pInA1)++;
            sum = inner::mac(sum, matData, vecData);
            sum = inner::mac(sum, matData2, vecData2);
        }
        // process remainder of row


        for(; k < (int)numCols; k ++)
        {
            sum = inner::mac(sum ,*pInA1++, v[k]);
        }

        *px++ = inner::from_accumulator(sum);
        i = i + m.stride();
        row--;
    }
}

#include "matrix_multiply_fixed.hpp"
#include "matrix_multiply_float.hpp"

/*! @} */