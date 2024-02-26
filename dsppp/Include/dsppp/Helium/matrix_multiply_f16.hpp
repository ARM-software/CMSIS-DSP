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

#if defined(ARM_MATH_MVE_FLOAT16)

/*

This can't be used with stride bigger than 21845
which for embedded is acceptable.

No check is done at runtime or build time that the stride is not
too big.

*/

template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         SameElementType<MA,float16_t>::value,bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_2x2_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    using T = typename traits<MA>::Scalar;
    //using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;

    const uint16_t offsetA[8] = { 0, 0, (uint16_t)pSrcA.stride(), (uint16_t)pSrcA.stride(), 
                                  0, 0, (uint16_t)pSrcA.stride(), (uint16_t)pSrcA.stride() };
    /* offsetB allows to read and duplicate 1 row of B */
    const uint16_t offsetB[8] = { 0, 1, 0, 1, 0, 1, 0, 1 };

    /* {d00, d01, d10, d11} */
    const uint16_t  offsetD[8] = { 0, 1, (uint16_t)pDst.stride(), (uint16_t)(pDst.stride()+1),
                                   0,0,0,0 };

    uint16x8_t    vecOffsA, vecOffsB,vecOffsD;
    VEC       vecInA, vecInB, vecDst;
    T      *pOut = pDst.ptr();  /* output data matrix pointer */

    /*
     * load initial offsets
     */
    vecOffsA = vldrhq_u16((uint16_t const *) offsetA);
    vecOffsB = vldrhq_u16((uint16_t const *) offsetB);
    /*
     * load {a00 a00 a10 a10 x x x x }
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * load {b00 b01 b00 b01 x x x x }
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     *  { a00 b00       a00 b01
     *    a10 b00       a10 b01
     *       x             x
     *       x             x   }
     */
    vecDst = vmulq(vecInA, vecInB);
    /*
     * move to 2nd column of matrix A
     */
    vecOffsA = vaddq_n_u16(vecOffsA, (uint16_t) 1);
    /*
     * load {a01 a01 a11 a11 x x x x}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * move to next B row
     */
    vecOffsB = vaddq_n_u16(vecOffsB, (uint16_t) pSrcB.stride());
    /*
     * load {b10, b11, b10, b11, x x x x }
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     *  { a00 b00 + a01 b10   a00 b01 + a01 b11
     *    a10 b00 + a11 b10     a10 b01 + a11 b11
     *             x                    x
     *             x                    x       }
     */
    vecDst = vfmaq(vecDst, vecInA, vecInB);

    mve_pred16_t p0 = vctp16q(2*2);
    /*
     * Store the result in the destination buffer
     * (lower half of the vector)
     */

    vecOffsD = vldrhq_u16((uint16_t const *) offsetD);

    vstrhq_scatter_shifted_offset_p(pOut,vecOffsD,vecDst,p0);

}


template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         SameElementType<MA,float16_t>::value,bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_3x3_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    const uint16_t offsetA[8] = { 0, 0, 0, 
                                         (uint16_t)pSrcA.stride(), (uint16_t)pSrcA.stride(), (uint16_t)pSrcA.stride(), 
                                         (uint16_t)(2U*pSrcA.stride()), (uint16_t)(2U*pSrcA.stride()) };
    /* offsetB allows to read and duplicate 1 row of B */
    const uint16_t offsetB[8] = { 0, 1, 2, 0, 1, 2, 0, 1 };
    const uint16_t offsetD[8] = { 0, 1, 2, 
                                (uint16_t)(0+pDst.stride()), (uint16_t)(1+pDst.stride()), 
                                (uint16_t)(2+pDst.stride()), 
                                (uint16_t)(0+2*pDst.stride()),
                                (uint16_t)(1+2*pDst.stride()) };

    uint16x8_t    vecOffsA, vecOffsB,vecOffsD;
    float16x8_t       vecInA, vecInB, vecDst;
    float16_t      *pOut = pDst.ptr();  /* output data matrix pointer */

    /*
     * load initial offsets
     */
    vecOffsA = vldrhq_u16((uint16_t const *) offsetA);
    vecOffsB = vldrhq_u16((uint16_t const *) offsetB);

    /*
     * load {a00 a00 a00 a10 a10 a10 a20 a20}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * load {b00 b01 b02 b00 b01 b02 b00 b01}
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     *  { a00 b00       a00 b01     a00 b02
     *    a10 b00       a10 b01     a10 b02
     *    a20 b00       a20 b01}
     */
    vecDst = vmulq(vecInA, vecInB);

    /*
     * move to 2nd column of matrix A
     */
    vecOffsA = vaddq_n_u16(vecOffsA, (uint16_t) 1);
    /*
     * load {a01 a01 a01 a11 a11 a11 a21 a21}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * move to next B row
     */
    vecOffsB = vaddq_n_u16(vecOffsB, (uint16_t) pSrcB.stride());
    /*
     * load {b10, b11, b12, b10, b11, b12, b10, b11}
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     *  { a00 b00 + a01 b10   a00 b01 + a01 b11     a00 b02 + a01 b12
     *    a10 b00 + a11 b10     a10 b01 + a11 b11     a10 b02 + a11 b12
     *    a20 b00 + a21 b10     a20 b01 + a21 b11   }
     */
    vecDst = vfmaq(vecDst, vecInA, vecInB);
    /*
     * move to 3rd column of matrix A
     */
    vecOffsA = vaddq_n_u16(vecOffsA, (uint16_t) 1);
    /*
     * load {a02 a02 a02 a12 a12 a12 a22 a22}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * move to next B row
     */
    vecOffsB = vaddq_n_u16(vecOffsB, (uint16_t)  pSrcB.stride());
    /*
     * load {b20, b21, b22, b20, b21, b22, b20, b21}
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     *  {a00 b00 + a01 b10 + a02 b20  a00 b01 + a01 b11 + a02 b21     a00 b02 + a01 b12 + a02 b22},
     *   a10 b00 + a11 b10 + a12 b20    a10 b01 + a11 b11 + a12 b21     a10 b02 + a11 b12 + a12 b22},
     *   a20 b00 + a21 b10 + a22 b20    a20 b01 + a21 b11 + a22 b21   }
     */
    vecDst = vfmaq(vecDst, vecInA, vecInB);

    /*
     * Store the result in the destination buffer
     */
    vecOffsD = vldrhq_u16((uint16_t const *) offsetD);

    vstrhq_scatter_shifted_offset(pOut,vecOffsD,vecDst);

    pOut += 2*pDst.stride()+2;

    /* last element computed in scalar mode
     * a20 b02 + a21 b12 + a22 b22
     */
  
    const _Float16 * pA = (const _Float16 *)pSrcA.const_ptr();
    const _Float16 * pB = (const _Float16 *)pSrcB.const_ptr();
    const index_t sa =pSrcA.stride();
    const index_t sb =pSrcB.stride();
    *pOut = pA[2*sa] * pB[2] + pA[1+2*sa] * pB[2+sb] + pA[2+2*sa] * pB[2+2*sb];

}



template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         SameElementType<MA,float16_t>::value,bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_4x4_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
     /* offsetA allows to read and duplicate 2 successive column elements of A */
    const uint16_t offsetA[8] = { 0, 0, 0, 0, 
                                         (uint16_t)pSrcA.stride(), (uint16_t)pSrcA.stride(), (uint16_t)pSrcA.stride(), (uint16_t)pSrcA.stride() };
    /* offsetB allows to read and duplicate 1 row of B */
    const uint16_t offsetB[8] = { 0, 1, 2, 3, 0, 1, 2, 3 };

    const uint16_t offsetD[8] = { 0, 1, 2, 3, 
                                  (uint16_t)(0+pDst.stride()), (uint16_t)(1+pDst.stride()), 
                                  (uint16_t)(2+pDst.stride()), (uint16_t)(3+pDst.stride()) };

    uint16x8_t    vecOffsA, vecOffsB,vecOffsD;
    float16x8_t       vecInA, vecInB, vecDst0, vecDst1;
    float16_t      *pOut = pDst.ptr();  /* output data matrix pointer */

    /*
     * load initial offsets
     */
    vecOffsA = vldrhq_u16((uint16_t const *) offsetA);
    vecOffsB = vldrhq_u16((uint16_t const *) offsetB);

    /*
     * load {a00 a00 a00 a00 a10 a10 a10 a10}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * load {b00 b01 b02 b03 b00 b01 b02 b03}
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);

    /*
     *  { a00 b00       a00 b01     a00 b02     a00 b03
     *    a10 b00       a10 b01     a10 b02     a10 b03 }
     */
    vecDst0 = vmulq(vecInA, vecInB);
    /*
     * jump 2 x A rows (2nd half of matrix)
     */
    vecOffsA = vaddq_n_u16(vecOffsA, (uint16_t) pSrcA.stride()*2);
    /*
     * load {a20 a20 a20 a20 a30 a30 a30 a30}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     *  { a20 b00       a20 b01     a20 b02     a20 b03
     *    a30 b00       a30 b01     a30 b02 +   a31 b12 }
     */
    vecDst1 = vmulq(vecInA, vecInB);
    /*
     * rewind back to top half of the A matrix (2nd column)
     */
    vecOffsA = vsubq(vecOffsA, (uint16_t) (2*pSrcA.stride()-1));
    /*
     * load {a01 a01 a01 a01 a11 a11 a11 a11}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);

    /*
     * move to next B row
     */
    vecOffsB = vaddq_n_u16(vecOffsB, (uint16_t) pSrcB.stride());
    /*
     * load {b10, b11, b12, b13, b10, b11, b12, b13}
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     *  { a00 b00 + a01 b10         a00 b01 + a01 b11       a00 b02 + a01 b12       a00 b03 + a01 b13
     *    a10 b00 + a11 b10         a10 b01 + a11 b11       a10 b02 + a11 b12       a10 b03 + a11 b13 }
     */
    vecDst0 = vfmaq(vecDst0, vecInA, vecInB);
    /*
     * jump 2 x A rows (2nd half of matrix)
     */
    vecOffsA = vaddq_n_u16(vecOffsA, (uint16_t) pSrcA.stride()*2);
    /*
     * load {a21 a21 a21 a21 a31 a31 a31 a31}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     *  {a20 b00 + a21 b10      a20 b01 + a21 b11       a20 b02 + a21 b12       a20 b03 + a21 b13
     *   a30 b00 + a31 b10      a30 b01 + a31 b11       a30 b02 + a31 b12       a30 b03 + a31 b13 }
     */
    vecDst1 = vfmaq(vecDst1, vecInA, vecInB);

    /*
     * rewind back to top half of the A matrix (3rd column)
     */
    vecOffsA = vsubq(vecOffsA, (uint16_t) (2*pSrcA.stride()-1));
    /*
     * load {a02 a02 a02 a02 a12 a12 a12 a12}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * move to next B row
     */
    vecOffsB = vaddq_n_u16(vecOffsB, (uint16_t) pSrcB.stride());
    /*
     * load {b20, b21, b22, b23, b20, b21, b22, b23}
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     *  { a00 b00 + a01 b10 + a02 b20    a00 b01 + a01 b11 + a02 b21    a00 b02 + a01 b12 + a02 b22   a00 b03 + a01 b13 + a02 b23
     *    a10 b00 + a11 b10 + a12 b20    a10 b01 + a11 b11 + a12 b21    a10 b02 + a11 b12 + a12 b22   a10 b03 + a11 b13 + a12 b23 }
     */
    vecDst0 = vfmaq(vecDst0, vecInA, vecInB);
    /*
     * jump 2 x A rows
     */
    vecOffsA = vaddq_n_u16(vecOffsA, (uint16_t) 2*pSrcA.stride());

    /*
     * load {a22 a22 a22 a22 a32 a32 a32 a32}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     *  {a20 b00 + a21 b10 + a22 b20   a20 b01 + a21 b11 + a22 b21  a20 b02 + a21 b12 + a22 b22    a20 b03 + a21 b13 + a22 b23
     *   a30 b00 + a31 b10 + a32 b20   a30 b01 + a31 b11 + a32 b21  a30 b02 + a31 b12 + a32 b22    a30 b03 + a31 b13 + a32 b23 }
     */
    vecDst1 = vfmaq(vecDst1, vecInA, vecInB);

    /*
     * rewind back to top half of the A matrix (4th column)
     */
    vecOffsA = vsubq(vecOffsA, (uint16_t) (2*pSrcA.stride()-1));
    /*
     * load {a03 a03 a03 a03 a13 a13 a13 a13}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     * move to next B row
     */
    vecOffsB = vaddq_n_u16(vecOffsB, (uint16_t) pSrcB.stride());
    /*
     * load {b30, b31, b32, b33, b30, b31, b32, b33}
     */
    vecInB = vldrhq_gather_shifted_offset(pSrcB.const_ptr(), vecOffsB);
    /*
     * { a00 b00 +...+ a03 b30,    a00 b01 +...+ a03 b31,   a00 b02 +...+ a03 b32,   a00 b03 +...+ a03 b33
     *   a10 b00 +...+ a13 b30,    a10 b01 +...+ a13 b31,   a10 b02 +...+ a13 b32,   a10 b03 +...+ a13 b33 }
     */
    vecDst0 = vfmaq(vecDst0, vecInA, vecInB);
    /*
     * jump 2 x A rows
     */
    vecOffsA = vaddq_n_u16(vecOffsA, (uint16_t) pSrcA.stride()*2);
    /*
     * load {a23 a23 a23 a23 a33 a33 a33 a33}
     */
    vecInA = vldrhq_gather_shifted_offset(pSrcA.const_ptr(), vecOffsA);
    /*
     *  {a20 b00 +...+ a23 b30,   a20 b01 +...+ a23 b31,   a20 b02 +...+ a23 b32,   a20 b03 +...+ a23 b33
     *   a30 b00 +...+ a33 b30,   a30 b01 +...+ a33 b31,   a30 b02 +...+ a33 b32,   a30 b03 +...+ a33 b33 }
     */
    vecDst1 = vfmaq(vecDst1, vecInA, vecInB);

    /*
     * Store the result in the destination buffer
     */
    vecOffsD = vldrhq_u16((uint16_t const *) offsetD);
    vstrhq_scatter_shifted_offset(pOut,vecOffsD,vecDst0);
    pOut += 2*pDst.stride();
    vstrhq_scatter_shifted_offset(pOut,vecOffsD,vecDst1);

}

#endif

/*! @} */