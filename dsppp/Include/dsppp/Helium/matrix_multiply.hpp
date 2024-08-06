// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_MATH_MVEI
#define ARM_MATH_MVEF
#define ARM_MATH_MVE_FLOAT16
#endif

/** \addtogroup HELIUMALG
 *  @{
 */

template<typename M,
         typename V,
         typename RES,
         typename std::enable_if<
         has_vector_inst<M>() &&
         has_vector_inst<V>() && 
         //!is_mixed<V>() && 
         same_nb_lanes<M,V>() &&
         same_nb_lanes<M,RES>() ,bool>::type = true>
inline void _dot_m_v(RES &res,
                    const M&m,const V&v,
                    const Helium* = nullptr)
{

   const vector_length_t nb_rows=m.rows();
   constexpr int U = 4;
   
   index_t row=0;

   DISABLE_LOOP_UNROLL
   for(; row<=nb_rows-U; row += U)
   {
      results<U>([&res,&row](index_t k){return &res[row+k];}) =
           inner::from_accumulator(dot(unroll<U>(
                                 [&row,&m](index_t k){return m.row(row+k);}),
                                 replicate<U>(v)
              ));
   }

   switch (nb_rows-row)
   {
      case 3:
         results<3>([&res,row](index_t k){return &res[row+k];}) =
           inner::from_accumulator(dot(unroll<3>(
                                 [row,&m](index_t k){return m.row(row+k);}),
                                 replicate<3>(v)
                           ));
      break;
      case 2:
         results<2>([&res,row](index_t k){return &res[row+k];}) =
           inner::from_accumulator(dot(unroll<2>(
                                 [row,&m](index_t k){return m.row(row+k);}),
                                 replicate<2>(v)
                               ));
      break;
      case 1:
         res[row] = inner::from_accumulator(dot(m.row(row),v));
      break;
   }

}

#define MATRIX_DIM2 2
#define MATRIX_DIM3 3
#define MATRIX_DIM4 4

#if defined(ARM_MATH_MVEI) 

/* Fixed point specific cases*/
#include "matrix_multiply_fixed.hpp"

#endif

#if defined(ARM_MATH_MVEF)

/* Datatype specific cases*/
#include "matrix_multiply_f16.hpp"
#include "matrix_multiply_f32.hpp"

/* Generic float */
template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<
         has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         is_float<MA>(),bool>::type = true>
__STATIC_INLINE void _dot_m_m(const MA&    pSrcA,
                              const MB&    pSrcB,
                                    RES && pDst,
                              const Helium* = nullptr)
   {
    using TA = typename traits<remove_constref_t<MA>>::Scalar;
    using TB = typename traits<remove_constref_t<MB>>::Scalar;
    using TDST = typename traits<remove_constref_t<RES>>::Scalar;
    

    using ACC = typename vector_traits<TDST>::temp_accumulator;
    using VECB = typename vector_traits<TB>::vector;
    using C = typename number_traits<TDST>::compute_type;

    constexpr int nb_lanes = vector_traits<TDST>::nb_lanes;
    constexpr int longunroll = IsComplexNumber<TDST>::value ? 0 : 1;
    constexpr int unrolltail = longunroll ? 3 : 1;

    TB  *pInB = pSrcB.ptr();        /* input data matrix pointer B */
    TA  *pInA = pSrcA.ptr();        /* input data matrix pointer A  */
    TDST  *pOut = pDst.ptr();         /* output data matrix pointer */
    int         numRowsA = pSrcA.rows();  /* number of rows of input matrix A */
    int         numColsB = pSrcB.columns();  /* number of columns of input matrix B */
    int         numColsA = pSrcA.columns();  /* number of columns of input matrix A */
    uint32_t    blkCnt;                     /* loop counters */
    uint32_t    i;

  {
      /* small squared matrix specialized routines 
       * but not for mixed complex / real product
      */
    if constexpr (std::is_same<TA,TB>::value)
    {
        if(numRowsA == numColsB && numColsB == numColsA) {
            if (numRowsA == 1)
            {
               pDst(0,0)= (C)pSrcA(0,0) * (C)pSrcB(0,0);
               return;
            }
            else if(numRowsA == 2)
            {
                return _arm_mat_mult_2x2_mve(pSrcA, pSrcB, std::forward<RES>(pDst));
            }
            else if(numRowsA == 3)
            {
                return _arm_mat_mult_3x3_mve(pSrcA, pSrcB, std::forward<RES>(pDst));
            }
            else if(numRowsA == 4)
            {
                return _arm_mat_mult_4x4_mve(pSrcA, pSrcB, std::forward<RES>(pDst));
            }
        }
    }

    /* main loop process 4 rows */
    if constexpr (longunroll)
    {
       i = numRowsA >> 2;
    }
    else 
    {
        i = numRowsA >> 1;
    }
    while (i > 0U)
    {
        TA *pInA0, *pInA1, *pInA2, *pInA3;
        TB *pInB0;
        TDST *pOut0, *pOut1, *pOut2, *pOut3;
        ACC vecMac0, vecMac1, vecMac2, vecMac3;
        VECB vecInB;

        /* pointers to 4 consecutive output rows */
        pOut0 = pOut;
        pOut1 = pOut0 + pDst.stride();
        if constexpr (longunroll)
        {
           pOut2 = pOut1 + pDst.stride();
           pOut3 = pOut2 + pDst.stride();
        }
        pInB0 = pInB;

        uint32_t  k = numColsB / nb_lanes;
        while (k > 0U)
        {
            /* pointers to 4 consecutive Matrix A rows */
            pInA0 = pInA;
            pInA1 = pInA0 + pSrcA.stride();
            if constexpr (longunroll)
            {
               pInA2 = pInA1 + pSrcA.stride();
               pInA3 = pInA2 + pSrcA.stride();
            }

            vecMac0 = vector_traits<TDST>::temp_acc_zero();
            vecMac1 = vector_traits<TDST>::temp_acc_zero();

            if constexpr (longunroll)
            {
               vecMac2 = vector_traits<TDST>::temp_acc_zero();
               vecMac3 = vector_traits<TDST>::temp_acc_zero();
            }

            blkCnt = numColsA;

            while (blkCnt > 0U)
            {
                /*
                 * load {bi,4n+0, bi,4n+1, bi,4n+2, bi,4n+3}
                 */
                vecInB = inner::vload1<1>(pInB0); /* vldrwq_f32(pInB0, 0); */

                vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);
                vecMac1 = inner::vmacc(vecMac1, vecInB, *pInA1++);
                if constexpr (longunroll)
                {
                   vecMac2 = inner::vmacc(vecMac2, vecInB, *pInA2++);
                   vecMac3 = inner::vmacc(vecMac3, vecInB, *pInA3++);
                }
                pInB0 = pInB0 + pSrcB.stride();
                /*
                 * Decrement the blockSize loop counter
                 */
                blkCnt--;
            }

            /* Store the results (4 x 4 block) in the destination buffer */
            inner::vstore1<1>(pOut0, vecMac0);  
            pOut0 += nb_lanes;
            inner::vstore1<1>(pOut1, vecMac1);  
            pOut1 += nb_lanes;

            if constexpr (longunroll)
            {
               inner::vstore1<1>(pOut2, vecMac2);  
               pOut2 += nb_lanes;
               inner::vstore1<1>(pOut3, vecMac3);  
               pOut3 += nb_lanes;
            }

            /*
             * rewind
             */
            pInB0 -= (pSrcB.stride() * numColsA) - nb_lanes;
            k--;
        }

        int       colBLeft = numColsB & (nb_lanes - 1);
        if (colBLeft)
        {
            pInA0 = pInA;
            pInA1 = pInA0 + pSrcA.stride();
            if constexpr (longunroll)
            {
               pInA2 = pInA1 + pSrcA.stride();
               pInA3 = pInA2 + pSrcA.stride();
            }
            
            mve_pred16_t    p0 = inner::vctpq<TDST>::mk(colBLeft);

            vecMac0 = vector_traits<TDST>::temp_acc_zero();
            vecMac1 = vector_traits<TDST>::temp_acc_zero();
            if constexpr (longunroll)
            {
               vecMac2 = vector_traits<TDST>::temp_acc_zero();
               vecMac3 = vector_traits<TDST>::temp_acc_zero();
            }

            blkCnt = numColsA;

            while (blkCnt > 0U)
            {
                /*
                 * load {bi,4n+0, bi,4n+1, bi,4n+2, bi,4n+3}
                 */
                vecInB = inner::vload1_z<1>(pInB0, colBLeft,p0);

                vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);
                vecMac1 = inner::vmacc(vecMac1, vecInB, *pInA1++);
                if constexpr (longunroll)
                {
                   vecMac2 = inner::vmacc(vecMac2, vecInB, *pInA2++);
                   vecMac3 = inner::vmacc(vecMac3, vecInB, *pInA3++);
                }

                pInB0 = pInB0 + pSrcB.stride();
                /*
                 * Decrement the blockSize loop counter
                 */
                blkCnt--;
            }

            /* Store the results (4 x colBLeft block) in the destination buffer */
            inner::vstore1_z<1>(pOut0, vecMac0, colBLeft,p0);
            inner::vstore1_z<1>(pOut1, vecMac1, colBLeft,p0);
            if constexpr (longunroll)
            {
               inner::vstore1_z<1>(pOut2, vecMac2, colBLeft,p0);
               inner::vstore1_z<1>(pOut3, vecMac3, colBLeft,p0);
            }
        }

        /* move to next rows */
        if constexpr (longunroll)
        {
           pInA += 4 * pSrcA.stride();
           pOut += 4 * pDst.stride();
        }
        else 
        {
           pInA += 2 * pSrcA.stride();
           pOut += 2 * pDst.stride();
        }
        i--;
    }

    /*
     * non multiple of 4 rows for Matrix A
     * process single row
     */
    if (numRowsA & unrolltail)
    {
        i = numRowsA & unrolltail;
        while (i > 0U)
        {
            TA   *pInA0;
            TB   *pInB0;
            TDST   *pOut0;
            VECB    vecInB;
            ACC    vecMac0;

            pOut0 = pOut;
            pInB0 = pInB;

            uint32_t       k = numColsB / nb_lanes;
            while (k > 0U)
            {
                pInA0 = pInA;

                vecMac0 = vector_traits<TDST>::temp_acc_zero();
                blkCnt = numColsA;
                while (blkCnt > 0U)
                {
                    /*
                     * load {bi,4n+0, bi,4n+1, bi,4n+2, bi,4n+3}
                     */
                    vecInB = inner::vload1<1>(pInB0); /* vldrwq_f32(pInB0, 0); */

                    vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);

                    pInB0 = pInB0 + pSrcB.stride();
                    /*
                     * Decrement the blockSize loop counter
                     */
                    blkCnt--;
                }

                /* Store the results (1 x 4 block) in the destination buffer */
                inner::vstore1<1>(pOut0, vecMac0);  
                pOut0 += nb_lanes;

                /*
                 * rewind
                 */
                pInB0 -= (pSrcB.stride() * numColsA) - nb_lanes;
                k--;
            }

            int       colBLeft = numColsB & (nb_lanes-1);
            if (colBLeft)
            {
                pInA0 = pInA;
                mve_pred16_t    p0 = inner::vctpq<TDST>::mk(colBLeft);

                vecMac0 = vector_traits<TDST>::temp_acc_zero();
                blkCnt = numColsA;
                while (blkCnt > 0U)
                {
                    /*
                     * load {bi,4n+0, bi,4n+1, bi,4n+2, bi,4n+3}
                     */
                    vecInB = inner::vload1_z<1>(pInB0, colBLeft,p0);

                    vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);

                    pInB0 = pInB0 + pSrcB.stride();
                    /*
                     * Decrement the blockSize loop counter
                     */
                    blkCnt--;
                }
                /* Store the results (1 x colBLeft block) in the destination buffer */
                inner::vstore1_z<1>(pOut0, vecMac0, colBLeft,p0);
            }

            /* move to next row */
            pInA += 1 * pSrcA.stride();
            pOut += 1 * pDst.stride();
            i--;
        }
        
      }
 
}

}


#undef MATRIX_DIM2
#undef MATRIX_DIM3
#undef MATRIX_DIM4

#endif

/*! @} */