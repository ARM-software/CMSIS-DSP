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

template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,RES>() &&
         (is_complex<MA>()) &&
         (is_complex<MB>()) &&
         same_type_as<MA,float32_t>(),bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_2x2_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    using EA = typename traits<remove_constref_t<MA>>::Scalar;
    using EB = typename traits<remove_constref_t<MB>>::Scalar;
    using EDST = typename traits<remove_constref_t<RES>>::Scalar;

    //using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<EDST>::half_vector;

    const EA   *pInA0, *pInA1;
    const EB   *pInB;
    EDST *pOut;
    VEC  acc0,acc1,vecA,vecB;

    pOut = pDst.ptr();
    pInA0 = pSrcA.const_ptr();
    pInA1 = pInA0 + pSrcA.stride();
    pInB = pSrcB.const_ptr();

    /* col0 = B00 B10 */
    if constexpr (!HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    /* Row0 : A00 A01 */
    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    /* Row1 : A10 A11 */
    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);
       
   
    /* Output column 0 */
    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut++;

    /*
     * move to next B column
     */
    pInB++;

    /* col1 = B01 B11 */
    if constexpr (!HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    /* Row0 : A00 A01 */
    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    /* Row1 : A10 A11 */
    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    /* Output col 1 */
    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut++;
}

template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,RES>() &&
         (is_complex<MA>()) &&
         (is_complex<MB>()) &&
         same_type_as<MA,float32_t>(),bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_3x3_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    using EA = typename traits<MA>::Scalar;
    //using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<EA>::half_vector;

    const EA   *pInA0, *pInA1, *pInA2, *pInB;
    EA *pOut;
    VEC  acc0,acc1,acc2, vecA,vecB;

    mve_pred16_t p = inner::vctpq<EA>::mk(1);

    pOut = pDst.ptr();
    pInA0 = pSrcA.ptr();
    pInA1 = pInA0 + pSrcA.stride();
    pInA2 = pInA1 + pSrcA.stride();
    pInB = pSrcB.ptr();


    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }


    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2);
    acc2 = inner::vmul(vecA,vecB);

    vecB = inner::vload1_half_z<1>(pInB + 2*pSrcB.stride(),1,p);

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc2 = inner::vmacc(acc2,vecA,vecB);

    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut[2*pDst.stride()] = inner::vreduce(acc2);

    pOut++;
    pInB++;

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2);
    acc2 = inner::vmul(vecA,vecB);

    vecB = inner::vload1_half_z<1>(pInB + 2*pSrcB.stride(),1,p);

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc2 = inner::vmacc(acc2,vecA,vecB);

    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut[2*pDst.stride()] = inner::vreduce(acc2);

    pOut++;
    pInB++;

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2);
    acc2 = inner::vmul(vecA,vecB);

    vecB = inner::vload1_half_z<1>(pInB + 2*pSrcB.stride(),1,p);


    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc2 = inner::vmacc(acc2,vecA,vecB);

    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut[2*pDst.stride()] = inner::vreduce(acc2);
    pOut++;
}

template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,RES>() &&
         (is_complex<MA>()) &&
         (is_complex<MB>()) &&
         same_type_as<MA,float32_t>(),bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_4x4_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    using EA = typename traits<MA>::Scalar;
    //using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<EA>::half_vector;

    const EA   *pInA0, *pInA1, *pInA2, *pInA3, *pInB;
    EA *pOut;
    VEC  acc0,acc1,acc2, acc3, vecA,vecB;

    pOut = pDst.ptr();
    pInA0 = pSrcA.ptr();
    pInA1 = pInA0 + pSrcA.stride();
    pInA2 = pInA1 + pSrcA.stride();
    pInA3 = pInA2 + pSrcA.stride();
    pInB = pSrcB.ptr();

   
    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2);
    acc2 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc3 = inner::vmul(vecA,vecB);

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB + 2*pSrcB.stride(), pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0,s>::run(pInB + 2*pSrcB.stride());
    }

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc2 = inner::vmacc(acc2,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc3 = inner::vmacc(acc3,vecA,vecB);

    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut[2*pDst.stride()] = inner::vreduce(acc2);
    pOut[3*pDst.stride()] = inner::vreduce(acc3);

    pOut++;
    pInB++;

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2);
    acc2 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc3 = inner::vmul(vecA,vecB);

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB + 2*pSrcB.stride(), pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0,s>::run(pInB + 2*pSrcB.stride());
    }

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc2 = inner::vmacc(acc2,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc3 = inner::vmacc(acc3,vecA,vecB);

    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut[2*pDst.stride()] = inner::vreduce(acc2);
    pOut[3*pDst.stride()] = inner::vreduce(acc3);

    pOut++;
    pInB++;

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2);
    acc2 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc3 = inner::vmul(vecA,vecB);

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB + 2*pSrcB.stride(), pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0,s>::run(pInB + 2*pSrcB.stride());
    }

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc2 = inner::vmacc(acc2,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc3 = inner::vmacc(acc3,vecA,vecB);

    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut[2*pDst.stride()] = inner::vreduce(acc2);
    pOut[3*pDst.stride()] = inner::vreduce(acc3);

    pOut++;
    pInB++;

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB, pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0, s>::run(pInB);
    }

    vecA = inner::vload1_half<1>(pInA0);
    acc0 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2);
    acc2 = inner::vmul(vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc3 = inner::vmul(vecA,vecB);

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecB = inner::vload1_half(pInB + 2*pSrcB.stride(), pSrcB.stride());
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecB = inner::vload1_half_gen_stride<EA,0,s>::run(pInB + 2*pSrcB.stride());
    }

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc2 = inner::vmacc(acc2,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc3 = inner::vmacc(acc3,vecA,vecB);

    pOut[0] = inner::vreduce(acc0);
    pOut[pDst.stride()] = inner::vreduce(acc1);
    pOut[2*pDst.stride()] = inner::vreduce(acc2);
    pOut[3*pDst.stride()] = inner::vreduce(acc3);

    pOut++;
    pInB++;
}


template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,RES>() &&
         (!is_complex<MA>()) &&
         (!is_complex<MB>()) &&
         same_type_as<MA,float32_t>(),bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_2x2_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    using EA = typename traits<MA>::Scalar;
    using T = typename ComplexNumberType<EA>::type;
    //using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;

    /* {a00, a00, a10, a10} */
    const uint32_t  offsetA0[4] = { 0, 0, pSrcA.stride(), pSrcA.stride() };
    /* {b00, b01, b00, b01} */
    const uint32_t  offsetB0[4] = { 0, 1, 0, 1 };
    /* {a01, a01, a11, a11} */
    const uint32_t  offsetA1[4] = { 1, 1, pSrcA.stride() + 1, pSrcA.stride() + 1 };
    /* {b10, b11, b10, b11} */
    const uint32_t  offsetB1[4] = { pSrcB.stride(), pSrcB.stride()+1, pSrcB.stride(), pSrcB.stride()+1 };

    /* {d00, d01, d10, d11} */
    const uint32_t  offsetD[4] = { 0, 1, pDst.stride(), pDst.stride()+1 };
   
    uint32x4_t vecOffsA, vecOffsB,vecOffsC;
    VEC vecInA, vecInB, vecDst;

    if constexpr (!HasStaticStride<MA>::value)
    {
       vecOffsA = vldrwq_u32((uint32_t const *) offsetA0);
    }
    vecOffsB = vldrwq_u32((uint32_t const *) offsetB0);

    if constexpr (!HasStaticStride<MA>::value)
    {
       vecInA = vldrwq_gather_shifted_offset((const T*)pSrcA.const_ptr(), vecOffsA);
    }
    else
    {
        constexpr int s = StaticStride<MA>::value;
        vecInA = inner::vload1_gen_stride<T,0, 0, s, s>::run((const T*)pSrcA.const_ptr());
    }

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecInB = vldrwq_gather_shifted_offset((const T*)pSrcB.const_ptr(), vecOffsB);
    }
    else
    {
        vecInB = inner::vload1_gen_stride<T,0, 1, 0, 1>::run((const T*)pSrcB.const_ptr());
    }
    vecDst = inner::vmul(vecInA, vecInB);

    if constexpr (!HasStaticStride<MA>::value)
    {
        vecOffsA = vldrwq_u32((uint32_t const *) offsetA1);
    }

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecOffsB = vldrwq_u32((uint32_t const *) offsetB1);
    }

    if constexpr (!HasStaticStride<MA>::value)
    {
       vecInA = vldrwq_gather_shifted_offset((const T*)pSrcA.const_ptr(), vecOffsA);
    }
    else 
    {
        constexpr int s = StaticStride<MA>::value;
        vecInA = inner::vload1_gen_stride<T,1, 1, s+1, s+1>::run((const T*)pSrcA.const_ptr());

    }

    if constexpr (!HasStaticStride<MB>::value)
    {
        vecInB = vldrwq_gather_shifted_offset((const T*)pSrcB.const_ptr(), vecOffsB);
    }
    else 
    {
        constexpr int s = StaticStride<MB>::value;
        vecInB = inner::vload1_gen_stride<T,s, s+1, s, s+1>::run((const T*)pSrcB.const_ptr());
    }

    if constexpr (!HasStaticStride<RES>::value)
    {
        vecOffsC = vldrwq_u32((uint32_t const *) offsetD);
    }

    vecDst = inner::vmacc(vecDst, vecInA, vecInB);

    //inner::vstore1<1>(pDst.ptr(), vecDst);
    if constexpr (!HasStaticStride<RES>::value)
    {
       vstrwq_scatter_shifted_offset((T*)pDst.ptr(),vecOffsC,vecDst);
    }
    else 
    {        
        constexpr int s = StaticStride<RES>::value;
        inner::vstore1_gen_stride<T,0, 1, s, s+1>::run((T*)pDst.ptr(),vecDst);
    }

}

template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,RES>() &&
         (!is_complex<MA>()) &&
         (!is_complex<MB>()) &&
         same_type_as<MA,float32_t>(),bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_3x3_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    using T = typename traits<MA>::Scalar;
    using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;
    T   *pInB = pSrcB.ptr(); /* input data matrix pointer B */
    T   *pInA = pSrcA.ptr(); /* input data matrix pointer A  */
    T   *pOut = pDst.ptr();  /* output data matrix pointer */
    T   *pInA0, *pInA1, *pInA2;
    ACC    vecMac0, vecMac1, vecMac2;
    VEC    vecInB;
    T const *pSrBVec;

    pSrBVec = (T const *) pInB;

    pInA0 = pInA;
    pInA1 = pInA0 + pSrcA.stride();
    pInA2 = pInA1 + pSrcA.stride();
    /* enable predication to disable last (4th) vector element */
    mve_pred16_t p0 = inner::vctpq<T>::mk(MATRIX_DIM3);

    /*
     * load {b0,0, b0,1, b0,2, 0}
     */
    vecInB = inner::vload1_z<1>(pSrBVec, MATRIX_DIM3,p0);  
    pSrBVec += pSrcB.stride();

    vecMac0 = inner::vmul(vecInB, *pInA0++);
    vecMac1 = inner::vmul(vecInB, *pInA1++);
    vecMac2 = inner::vmul(vecInB, *pInA2++);
    /*
     * load {b1,0, b1,1, b1,2, 0}
     */
    vecInB = inner::vload1_z<1>(pSrBVec, MATRIX_DIM3,p0);  
    pSrBVec += pSrcB.stride();

    vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);
    vecMac1 = inner::vmacc(vecMac1, vecInB, *pInA1++);
    vecMac2 = inner::vmacc(vecMac2, vecInB, *pInA2++);
    /*
     * load {b2,0, b2,1 , b2,2, 0}
     */
    vecInB = inner::vload1_z<1>(pSrBVec, MATRIX_DIM3,p0);  
    pSrBVec += pSrcB.stride();

    vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);
    vecMac1 = inner::vmacc(vecMac1, vecInB, *pInA1++);
    vecMac2 = inner::vmacc(vecMac2, vecInB, *pInA2++);

    /* partial vector stores */
    inner::vstore1_z<1>(pOut, vecMac0, MATRIX_DIM3,p0); 
    pOut += pDst.stride();
    inner::vstore1_z<1>(pOut, vecMac1, MATRIX_DIM3,p0); 
    pOut += pDst.stride();
    inner::vstore1_z<1>(pOut, vecMac2, MATRIX_DIM3,p0);
    /*
     * Return to application
     */
}

template<typename MA,
         typename MB,
         typename RES,
typename std::enable_if<
         has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,RES>() &&
         (!is_complex<MA>()) &&
         (!is_complex<MB>()) &&
         same_type_as<MA,float32_t>(),bool>::type = true>
__STATIC_INLINE  void _arm_mat_mult_4x4_mve(
    const MA &pSrcA,
    const MB &pSrcB,
    RES &&pDst)
{
    using T = typename traits<MA>::Scalar;
    using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;
    T const *pSrBVec;
    T *pInB = pSrcB.ptr(); /* input data matrix pointer B */
    T *pInA = pSrcA.ptr(); /* input data matrix pointer A  */
    T *pOut = pDst.ptr();  /* output data matrix pointer */
    T *pInA0, *pInA1, *pInA2, *pInA3;
    ACC vecMac0, vecMac1, vecMac2, vecMac3;
    VEC vecInB;

    pSrBVec = (T const *) pInB;

    pInA0 = pInA;
    pInA1 = pInA0 + pSrcA.stride();
    pInA2 = pInA1 + pSrcA.stride();
    pInA3 = pInA2 + pSrcA.stride();
    /*
     * load {b0,0, b0,1, b0,2, b0,3}
     */
    vecInB = inner::vload1<1>(pSrBVec);  
    pSrBVec += pSrcB.stride();

    vecMac0 = inner::vmul(vecInB, *pInA0++);
    vecMac1 = inner::vmul(vecInB, *pInA1++);
    vecMac2 = inner::vmul(vecInB, *pInA2++);
    vecMac3 = inner::vmul(vecInB, *pInA3++);
    /*
     * load {b1,0, b1,1, b1,2, b1,3}
     */
    vecInB = inner::vload1<1>(pSrBVec);  
    pSrBVec += pSrcB.stride();

    vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);
    vecMac1 = inner::vmacc(vecMac1, vecInB, *pInA1++);
    vecMac2 = inner::vmacc(vecMac2, vecInB, *pInA2++);
    vecMac3 = inner::vmacc(vecMac3, vecInB, *pInA3++);
    /*
     * load {b2,0, b2,1, b2,2, b2,3}
     */
    vecInB = inner::vload1<1>(pSrBVec);  
    pSrBVec += pSrcB.stride();

    vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);
    vecMac1 = inner::vmacc(vecMac1, vecInB, *pInA1++);
    vecMac2 = inner::vmacc(vecMac2, vecInB, *pInA2++);
    vecMac3 = inner::vmacc(vecMac3, vecInB, *pInA3++);
    /*
     * load {b3,0, b3,1, b3,2, b3,3}
     */
    vecInB = inner::vload1<1>(pSrBVec);  
    pSrBVec += pSrcB.stride();

    vecMac0 = inner::vmacc(vecMac0, vecInB, *pInA0++);
    vecMac1 = inner::vmacc(vecMac1, vecInB, *pInA1++);
    vecMac2 = inner::vmacc(vecMac2, vecInB, *pInA2++);
    vecMac3 = inner::vmacc(vecMac3, vecInB, *pInA3++);

    inner::vstore1<1>(pOut, vecMac0);  
    pOut += pDst.stride();
    inner::vstore1<1>(pOut, vecMac1);  
    pOut += pDst.stride();
    inner::vstore1<1>(pOut, vecMac2);  
    pOut += pDst.stride();
    inner::vstore1<1>(pOut, vecMac3);
    
}

/*! @} */