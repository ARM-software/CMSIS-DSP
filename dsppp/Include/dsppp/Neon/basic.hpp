// -*- C++ -*-

#pragma once 

#include <dsppp/arch.hpp>
#include <type_traits>
#include <dsppp/number.hpp>
#if 0

template<typename T>
void _Add(const T* pSrcA,
           const T* pSrcB,
           T* pDst, 
           const std::size_t l,
           const Neon* = nullptr,
           typename std::enable_if<number_traits<T>::is_float && 
                                vector_traits<T,Neon>::has_vector,T>::type* = nullptr)
{
    using num = vector_traits<T,Neon>;
    using VecType = typename num::vector;
    constexpr int nb_lanes = num::nb_lanes;
    constexpr int lanes_shift = shiftFromValue(nb_lanes);
    constexpr int lanes_mask = maskFromShift(lanes_shift);

    //std::cout << "Neon float\r\n" ;

    uint32_t blkCnt;                               /* Loop counter */

    VecType vec1;
    VecType vec2;
    VecType res;

    /* Compute several lanes at a time */
    blkCnt = l >> lanes_shift;

    while (blkCnt > 0U)
    {
        /* C = A + B */

        /* Add and then store the results in the destination buffer. */
        vec1 = vld1q(pSrcA);
        vec2 = vld1q(pSrcB);
        res = vaddq(vec1, vec2);
        vst1q(pDst, res);

        /* Increment pointers */
        pSrcA += nb_lanes;
        pSrcB += nb_lanes; 
        pDst += nb_lanes;
        
        /* Decrement the loop counter */
        blkCnt--;
    }

    /* Tail */
    blkCnt = l & lanes_mask;

    while (blkCnt > 0U)
    {
      /* C = A + B */
  
      /* Add and store result in destination buffer. */
      *pDst++ = (*pSrcA++) + (*pSrcB++);
  
      /* Decrement loop counter */
      blkCnt--;
    }


};




template<typename T>
void _Add(const T* pSrcA_Q,
           const T* pSrcB_Q,
           T* pDst_Q, 
           const std::size_t l,
           const Neon* = nullptr,
           typename std::enable_if<number_traits<T>::is_fixed && 
                                   vector_traits<T,Neon>::has_vector,T>::type* = nullptr)
{
    using num = vector_traits<T,Neon>;
    using VecType = typename num::vector;
    using value_type = typename T::value_type;
    constexpr int nb_lanes = num::nb_lanes;
    constexpr int lanes_shift = shiftFromValue(nb_lanes);
    constexpr int lanes_mask = maskFromShift(lanes_shift);
    const value_type *pSrcA = reinterpret_cast<const value_type*>(pSrcA_Q);
    const value_type *pSrcB = reinterpret_cast<const value_type*>(pSrcB_Q);
          value_type *pDst = reinterpret_cast<value_type*>(pDst_Q);

    uint32_t  blkCnt;           /* loop counters */
    VecType vecA;
    VecType vecB;

    /* Compute 8 outputs at a time */
    blkCnt = l >> lanes_shift;
    while (blkCnt > 0U)
    {
        /*
         * C = A + B
         * Add and then store the results in the destination buffer.
         */
        vecA = vld1q(pSrcA);
        vecB = vld1q(pSrcB);
        vst1q(pDst, vqaddq(vecA, vecB));
        /*
         * Decrement the blockSize loop counter
         */
        blkCnt--;
        /*
         * advance vector source and destination pointers
         */
        pSrcA  += nb_lanes;
        pSrcB  += nb_lanes;
        pDst   += nb_lanes;
    }
    /*
     * tail
     */
    blkCnt = l & lanes_mask;
    if (blkCnt > 0U)
    {
        mve_pred16_t p0 = num::vctpq(blkCnt);
        vecA = vld1q(pSrcA);
        vecB = vld1q(pSrcB);
        vstrq_p(pDst, vqaddq(vecA, vecB), p0);
    }
}

#endif