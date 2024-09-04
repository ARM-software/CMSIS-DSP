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

#if defined(ARM_MATH_MVEI) 



#define MVE_ASRL_SAT16(acc, shift)          ((sqrshrl_sat48(acc, -(32-shift)) >> 32) & 0xffffffff)

/*
 *
 * Complex
 * 
 */

template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         (is_complex<MA>()) &&
         (is_complex<MB>()) &&
         is_fixed<MA>(),bool>::type = true>
inline void arm_mat_mult_2x2_mve(
    const MA & pSrcA,
    const MB & pSrcB,
    RES && pDst)
{
    using EA = typename traits<remove_constref_t<MA>>::Scalar;
    using EB = typename traits<remove_constref_t<MB>>::Scalar;
    using EDST = typename traits<remove_constref_t<RES>>::Scalar;
    using RealDST = typename EDST::value_type;
    using RealDSTValueType = typename RealDST::value_type;

    using ACC = typename vector_traits<EDST>::temp_accumulator;
    using VEC = typename vector_traits<EDST>::half_vector;

    EB const     *pInB = pSrcB.const_ptr();   /* input data matrix pointer B */
    EA const     *pInA = pSrcA.const_ptr();   /* input data matrix pointer A */
    EDST           *pOut = pDst.ptr();   /* output data matrix pointer */
    EA const     *pInA0 = pInA;
    EA const     *pInA1 = pInA0 + pSrcA.stride();
    ACC            acc0, acc1;
    VEC          vecB, vecA;

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
    acc0 = inner::vmacc(ACC{},vecA , vecB);
    
    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA , vecB);

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});
    pOut++;

    /*
     * move to next B column
     */
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
    acc0 = inner::vmacc(ACC{},vecA , vecB);
    
    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA , vecB);

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});


}

template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         (is_complex<MA>()) &&
         (is_complex<MB>()) &&
         is_fixed<MA>(),bool>::type = true>
inline void arm_mat_mult_3x3_mve(
    const MA & pSrcA,
    const MB & pSrcB,
    RES && pDst)
{
    using EA = typename traits<remove_constref_t<MA>>::Scalar;
    using EB = typename traits<remove_constref_t<MB>>::Scalar;
    using EDST = typename traits<remove_constref_t<RES>>::Scalar;
    using RealDST = typename EDST::value_type;
    using RealDSTValueType = typename RealDST::value_type;

    using ACC = typename vector_traits<EDST>::temp_accumulator;
    using VEC = typename vector_traits<EDST>::half_vector;

    EB const     *pInB = pSrcB.const_ptr();   /* input data matrix pointer B */
    EA const     *pInA = pSrcA.const_ptr();   /* input data matrix pointer A */
    EDST           *pOut = pDst.ptr();   /* output data matrix pointer */

    EA const     *pInA0 = pInA;
    EA const     *pInA1 = pInA0 + pSrcA.stride();
    EA const     *pInA2 = pInA1 + pSrcA.stride();
    ACC            acc0, acc1;
    VEC          vecB, vecA;
    /*
     * enable predication to disable upper half complex vector element
     */
    mve_pred16_t p = inner::vctpq<EA>::mk(1);

    

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
    acc0 = inner::vmacc(ACC{},vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA,vecB);

    vecB = inner::vload1_half_z<1>(pInB + 2*pSrcB.stride(),1,p);

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

    vecA = inner::vload1_half<1>(pInA2);
    acc0 = inner::vmacc(ACC{},vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);


    pOut[2*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    pOut++;

    /*
     * move to next B column
     */
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
    acc0 = inner::vmacc(ACC{},vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA,vecB);

    vecB = inner::vload1_half_z<1>(pInB + 2*pSrcB.stride(),1,p);

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

    vecA = inner::vload1_half<1>(pInA2);
    acc0 = inner::vmacc(ACC{},vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);


    pOut[2*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut ++;

    /*
     * move to next B column
     */
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
    acc0 = inner::vmacc(ACC{},vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA,vecB);

    vecB = inner::vload1_half_z<1>(pInB + 2*pSrcB.stride(),1,p);

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA,vecB);

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

    vecA = inner::vload1_half<1>(pInA2);
    acc0 = inner::vmacc(ACC{},vecA,vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc0 = inner::vmacc(acc0,vecA,vecB);


    pOut[2*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
}

template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         (is_complex<MA>()) &&
         (is_complex<MB>()) &&
         is_fixed<MA>(),bool>::type = true>
inline void arm_mat_mult_4x4_mve(
    const MA & pSrcA,
    const MB & pSrcB,
    RES && pDst)
{
    using EA = typename traits<remove_constref_t<MA>>::Scalar;
    using EB = typename traits<remove_constref_t<MB>>::Scalar;
    using EDST = typename traits<remove_constref_t<RES>>::Scalar;
    using RealDST = typename EDST::value_type;
    using RealDSTValueType = typename RealDST::value_type;

    using ACC = typename vector_traits<EDST>::temp_accumulator;
    using VEC = typename vector_traits<EDST>::half_vector;

    EB const     *pInB = pSrcB.const_ptr();   /* input data matrix pointer B */
    EA const     *pInA = pSrcA.const_ptr();   /* input data matrix pointer A */
    EDST           *pOut = pDst.ptr();   /* output data matrix pointer */

    EA const     *pInA0 = pInA;
    EA const     *pInA1 = pInA0 + pSrcA.stride();
    EA const     *pInA2 = pInA1 + pSrcA.stride();
    EA const     *pInA3 = pInA2 + pSrcA.stride();
    ACC            acc0, acc1;
    VEC        vecB,vecB1,vecA;

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
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecB1 = inner::vload1_half<1>(pInB + 2*pSrcB.stride());

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

    vecA = inner::vload1_half<1>(pInA2);
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[2*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[3*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

  

    pOut ++;

    /*
     * move to next B column
     */
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
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecB1 = inner::vload1_half<1>(pInB + 2*pSrcB.stride());

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

    vecA = inner::vload1_half<1>(pInA2);
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[2*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[3*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

  

    pOut ++;

    /*
     * move to next B column
     */
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
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecB1 = inner::vload1_half<1>(pInB + 2*pSrcB.stride());

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

    vecA = inner::vload1_half<1>(pInA2);
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[2*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[3*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

  

    pOut ++;

    /*
     * move to next B column
     */
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
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA1);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecB1 = inner::vload1_half<1>(pInB + 2*pSrcB.stride());

    vecA = inner::vload1_half<1>(pInA0+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA1+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[0] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                   RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                               RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

    vecA = inner::vload1_half<1>(pInA2);
    acc0 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA3);
    acc1 = inner::vmacc(ACC{},vecA, vecB);

    vecA = inner::vload1_half<1>(pInA2+2);
    acc0 = inner::vmacc(acc0,vecA, vecB1);

    vecA = inner::vload1_half<1>(pInA3+2);
    acc1 = inner::vmacc(acc1,vecA, vecB1);

    

    pOut[2*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc0.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc0.imag().v,23)});
    
    pOut[3*pDst.stride()] = EDST(RealDST{(RealDSTValueType)asrl(acc1.real().v,23)},
                                 RealDST{(RealDSTValueType)asrl(acc1.imag().v,23)});

  

}

/*
 *
 * Real
 * 
 */

template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         (!is_complex<MA>()) &&
         (!is_complex<MB>()) &&
         is_fixed<MA>(),bool>::type = true>
inline void arm_mat_mult_2x2_mve(
    const MA & pSrcA,
    const MB & pSrcB,
    RES && pDst)
{
    using T = typename traits<MA>::Scalar;
    using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;

    const T     *pInB = pSrcB.const_ptr();  /* input data matrix pointer B */
    const T       *pInA = pSrcA.const_ptr();  /* input data matrix pointer A */
    T       *pOut = pDst.ptr();   /* output data matrix pointer */
    const T       *pInA0 = pInA;
    const T       *pInA1 = pInA0 + pSrcA.stride();
    ACC        acc0, acc1;
    VEC     vecB, vecA0, vecA1;
    mve_pred16_t p0 = inner::vctpq<T>::mk(MATRIX_DIM2);


   
    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM2,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM2,p0);
    }


    vecA0 = inner::vload1_z<1>(pInA0,MATRIX_DIM2,p0);
    vecA1 = inner::vload1_z<1>(pInA1,MATRIX_DIM2,p0);

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);

    pOut[0] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut++;

    /* move to next B column */
    pInB = pInB + 1;

    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM2,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM2,p0);
    }

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);

    pOut[0] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));

}


template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         (!is_complex<MA>()) &&
         (!is_complex<MB>()) &&
         is_fixed<MA>(),bool>::type = true>
inline void arm_mat_mult_3x3_mve(
    const MA & pSrcA,
    const MB & pSrcB,
    RES && pDst)
{

    using T = typename traits<MA>::Scalar;
    using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;

    const T       *pInB = pSrcB.const_ptr();  /* input data matrix pointer B */
    const T       *pInA = pSrcA.const_ptr();  /* input data matrix pointer A */
    T       *pOut = pDst.ptr();   /* output data matrix pointer */
    const T       *pInA0 = pInA;
    const T       *pInA1 = pInA0 + pSrcA.stride();
    const T       *pInA2 = pInA1 + pSrcA.stride();
    ACC        acc0, acc1, acc2;
    VEC    vecB, vecA0, vecA1, vecA2;
    mve_pred16_t p0 = inner::vctpq<T>::mk(MATRIX_DIM3);

   
    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM3,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM3,p0);
    }

    vecA0 = inner::vload1_z<1>(pInA0,MATRIX_DIM3,p0);
    vecA1 = inner::vload1_z<1>(pInA1,MATRIX_DIM3,p0);
    vecA2 = inner::vload1_z<1>(pInA2,MATRIX_DIM3,p0);

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);
    acc2 = inner::vmacc(vecA2, vecB,p0);

    pOut[0 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[1 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut[2 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc2));
    pOut++;

    /* move to next B column */
    pInB = pInB + 1;

    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM3,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM3,p0);
    }

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);
    acc2 = inner::vmacc(vecA2, vecB,p0);


    pOut[0 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[1 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut[2 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc2));
    pOut++;

    /* move to next B column */
    pInB = pInB + 1;

    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM3,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM3,p0);
    }

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);
    acc2 = inner::vmacc(vecA2, vecB,p0);


    pOut[0 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[1 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut[2 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc2));
   
}


template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         (!is_complex<MA>()) &&
         (!is_complex<MB>()) &&
         is_fixed<MA>(),bool>::type = true>
inline void arm_mat_mult_4x4_mve(
    const MA & pSrcA,
    const MB & pSrcB,
    RES && pDst)
{
    using T = typename traits<MA>::Scalar;
    using ACC = typename vector_traits<T>::temp_accumulator;
    using VEC = typename vector_traits<T>::vector;

    const T       *pInB = pSrcB.const_ptr();  /* input data matrix pointer B */
    const T       *pInA = pSrcA.const_ptr();  /* input data matrix pointer A */
    T       *pOut = pDst.ptr();   /* output data matrix pointer */
    const T       *pInA0 = pInA;
    const T       *pInA1 = pInA0 + pSrcA.stride();
    const T       *pInA2 = pInA1 + pSrcA.stride();
    const T       *pInA3 = pInA2 + pSrcA.stride();
    ACC        acc0, acc1, acc2, acc3;
    VEC     vecB, vecA0, vecA1, vecA2, vecA3;
    mve_pred16_t p0 = inner::vctpq<T>::mk(MATRIX_DIM4);


   
    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM4,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM4,p0);
    }

    vecA0 = inner::vload1_z<1>(pInA0,MATRIX_DIM4,p0);
    vecA1 = inner::vload1_z<1>(pInA1,MATRIX_DIM4,p0);
    vecA2 = inner::vload1_z<1>(pInA2,MATRIX_DIM4,p0);
    vecA3 = inner::vload1_z<1>(pInA3,MATRIX_DIM4,p0);

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);
    acc2 = inner::vmacc(vecA2, vecB,p0);
    acc3 = inner::vmacc(vecA3, vecB,p0);


    pOut[0 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[1 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut[2 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc2));
    pOut[3 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc3));
    pOut++;

    /* move to next B column */
    pInB = pInB + 1;

    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM4,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM4,p0);
    }


    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);
    acc2 = inner::vmacc(vecA2, vecB,p0);
    acc3 = inner::vmacc(vecA3, vecB,p0);

    pOut[0 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[1 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut[2 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc2));
    pOut[3 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc3));

    pOut++;

    /* move to next B column */
    pInB = pInB + 1;

    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM4,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM4,p0);
    }

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);
    acc2 = inner::vmacc(vecA2, vecB,p0);
    acc3 = inner::vmacc(vecA3, vecB,p0);
   
    pOut[0 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[1 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut[2 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc2));
    pOut[3 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc3));

    pOut++;

    /* move to next B column */
    pInB = pInB + 1;

    if constexpr (HasStaticStride<MB>::value)
    {
       vecB = inner::vload1_z<StaticStride<MB>::value>(pInB,MATRIX_DIM4,p0);
    }
    else
    {
       vecB = inner::vload1_z(pInB,pSrcB.stride(),MATRIX_DIM4,p0);
    }

    acc0 = inner::vmacc(vecA0, vecB,p0);
    acc1 = inner::vmacc(vecA1, vecB,p0);
    acc2 = inner::vmacc(vecA2, vecB,p0);
    acc3 = inner::vmacc(vecA3, vecB,p0);


    pOut[0 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc0));
    pOut[1 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc1));
    pOut[2 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc2));
    pOut[3 * pDst.stride()] = inner::from_accumulator(inner::vreduce(acc3));
   
}



/*
 *
 * All cases
 * 
 */

template<typename MA,
         typename MB,
         typename RES,
         typename TMP,
         typename std::enable_if<has_vector_inst<MA>() &&
         has_vector_inst<MB>() &&
         same_nb_lanes<MA,MB>() &&
         same_nb_lanes<MA,RES>() &&
         is_fixed<MA>(),bool>::type = true>
   __STATIC_INLINE void _dot_m_m(const MA&pSrcA,const MB&pSrcB,
                        RES &&pDst,
                        const TMP &BT,
                        const Helium* = nullptr)
   {
    using TA = typename traits<remove_constref_t<MA>>::Scalar;
    using TB = typename traits<remove_constref_t<MB>>::Scalar;
    using TDST = typename traits<remove_constref_t<RES>>::Scalar;
    

    using ACC = typename vector_traits<TDST>::temp_accumulator;
    using VECA = typename vector_traits<TA>::vector;
    using VECB = typename vector_traits<TB>::vector;

    constexpr int nb_lanes = vector_traits<TDST>::nb_lanes;
    //constexpr int longunroll = IsComplexNumber<TDST>::value ? 0 : 1;
    //constexpr int unrolltail = longunroll ? 3 : 1;

    const TA          *pInA = pSrcA.const_ptr();        /* input data matrix pointer A */
    const TB          *pInB = pSrcB.const_ptr();
    TDST          *pOut = pDst.ptr();        /* input data matrix pointer B */
    TDST          *px;         /* Temporary output data matrix pointer */
    TDST          *px2;        /* Temporary output data matrix pointer */
    uint32_t        numRowsA = pSrcA.rows();  /* number of rows of input matrix A    */
    
    uint32_t        numColsB = pSrcB.columns();  /* number of columns of input matrix B */
    uint32_t        numColsA = pSrcA.columns();  /* number of columns of input matrix A */
    
    uint32_t        strideA = pSrcA.stride();  /* number of columns of input matrix A */
    
    uint32_t        numRowsB = pSrcB.rows();  /* number of rows of input matrix A    */
    uint32_t        col, i = 0u, j, row = numRowsB;     /* loop counters */
   
    const TA          *pInA2;
    const TB          *pInB2;
    uint32_t        blkCnt;     /* loop counters */
    


    {
        if constexpr (std::is_same<TA,TB>::value)
        {
            /* small squared matrix specialized routines */
            if (numRowsA == numColsB && numColsB == numColsA) {
    
                if (numRowsA == 1) {
                    pDst(0,0) = pSrcA(0,0) * pSrcB(0,0);
                    return;
                } else if (numRowsA == 2)
                    return arm_mat_mult_2x2_mve(pSrcA, pSrcB, std::forward<RES>(pDst));
                else if (numRowsA == 3)
                    return arm_mat_mult_3x3_mve(pSrcA, pSrcB, std::forward<RES>(pDst));
                else if (numRowsA == 4)
                    return arm_mat_mult_4x4_mve(pSrcA, pSrcB, std::forward<RES>(pDst));
            }
        }

        /*
         * Matrix transpose
         */

        const TB *pSrcBT = BT.const_ptr();


        /*
         * Reset the variables for the usage in the following multiplication process
         */
        i = 0;
        row = numRowsA >> 1;
        px = pOut;
        px2 = px + pDst.stride();

        /*
         * The following loop performs the dot-product of each row in pSrcA with each column in pSrcB
         */

        /*
         * row loop
         */
        while (row > 0u) {
            /*
             * For every row wise process, the column loop counter is to be initiated
             */
            col = numColsB >> 1;
            /*
             * For every row wise process, the pIn2 pointer is set
             * to the starting address of the transposed pSrcB data
             */
            pInB = pSrcBT;
            pInB2 = pInB + numRowsB;
            j = 0;

            /*
             * column loop
             */
            while (col > 0u) {
                TA const    *pSrcAVec, *pSrcA2Vec;
                TB const    *pSrcBVec, *pSrcB2Vec;
                VECA         vecA, vecA2;
                VECB         vecB, vecB2;
                ACC           acc0, acc1, acc2, acc3;

                /*
                 * Initiate the pointer pIn1 to point to the starting address of the column being processed
                 */
                pInA = pSrcA.const_ptr() + i;
                pInA2 = pInA + strideA;
                pInB = pSrcBT + j;
                pInB2 = pInB + numRowsB;


                pSrcAVec = (TA const *) pInA;
                pSrcA2Vec = (TA const *) pInA2;
                pSrcBVec = (TB const *) pInB;
                pSrcB2Vec = (TB const *) pInB2;

                acc0 = vector_traits<TDST>::temp_acc_zero();
                acc1 = vector_traits<TDST>::temp_acc_zero();
                acc2 = vector_traits<TDST>::temp_acc_zero();
                acc3 = vector_traits<TDST>::temp_acc_zero();

                vecA = inner::vload1<1>(pSrcAVec);
                pSrcAVec += nb_lanes;

                blkCnt = numColsA / nb_lanes;
                while (blkCnt > 0U) {
                    vecB = inner::vload1<1>(pSrcBVec);
                    pSrcBVec += nb_lanes;
                    acc0 = inner::vmacc(acc0, vecA, vecB);
                    vecA2 = inner::vload1<1>(pSrcA2Vec);
                    pSrcA2Vec += nb_lanes;
                    acc1 = inner::vmacc(acc1, vecA2, vecB);
                    vecB2 = inner::vload1<1>(pSrcB2Vec);
                    pSrcB2Vec += nb_lanes;
                    acc2 = inner::vmacc(acc2, vecA, vecB2);
                    vecA = inner::vload1<1>(pSrcAVec);
                    pSrcAVec += nb_lanes;
                    acc3 = inner::vmacc(acc3, vecA2, vecB2);

                    blkCnt--;
                }
                /*
                 * tail
                 */
                blkCnt = numColsA & (nb_lanes-1);
                if (blkCnt > 0U) {
                    mve_pred16_t    p0 = inner::vctpq<TDST>::mk(blkCnt);
                    vecB = inner::vload1_z<1>(pSrcBVec,blkCnt,p0);
                    acc0 = inner::vmacc(acc0, vecA, vecB);
                    vecA2 = inner::vload1_z<1>(pSrcA2Vec,blkCnt,p0);
                    acc1 = inner::vmacc(acc1, vecA2, vecB);
                    vecB2 = inner::vload1_z<1>(pSrcB2Vec,blkCnt,p0);
                    acc2 = inner::vmacc(acc2, vecA, vecB2);
                    vecA = inner::vload1_z<1>(pSrcAVec,blkCnt,p0);
                    acc3 = inner::vmacc(acc3, vecA2, vecB2);
                }

                *px++  = inner::from_accumulator(inner::vreduce(acc0));
                *px++  = inner::from_accumulator(inner::vreduce(acc2));
                *px2++ = inner::from_accumulator(inner::vreduce(acc1));
                *px2++ = inner::from_accumulator(inner::vreduce(acc3));

                j += numRowsB * 2;
                /*
                 * Decrement the column loop counter
                 */
                col--;

            }

            i = i + strideA * 2;
            px = px2 + (numColsB & 1u);
            px2 = px + pDst.stride();
            /*
             * Decrement the row loop counter
             */
            row--;
        }

        /*
         * Compute remaining row and/or column below
         */

        if (numColsB & 1u) {
            row = numRowsA & (~0x1);    //avoid redundant computation
            px = pOut + + pDst.stride() - 1;
            i = 0;

            /*
             * row loop
             */
            while (row > 0) {


                TA const    *pSrcAVec;
                TB const    *pSrcBVec;
                VECA         vecA;
                VECB         vecB;
                ACC           acc0;

                /*
                 * point to last column in matrix B
                 */
                pInB = pSrcBT + numRowsB * (numColsB - 1);
                pInA = pSrcA.const_ptr() + i;

                pSrcAVec = (TA const *) pInA;
                pSrcBVec = (TB const *) pInB;

                acc0 = vector_traits<TDST>::temp_acc_zero();
                blkCnt = (numColsA) / nb_lanes;
                while (blkCnt > 0U) {
                    vecA = inner::vload1<1>(pSrcAVec);
                    pSrcAVec += nb_lanes;
                    vecB = inner::vload1<1>(pSrcBVec);
                    pSrcBVec += nb_lanes;
                    acc0 = inner::vmacc(acc0, vecA, vecB);

                    blkCnt--;
                }
                /*
                 * tail
                 */
                blkCnt = (numColsA & (nb_lanes-1));
                if (blkCnt > 0U) {
                    mve_pred16_t    p0 = inner::vctpq<TDST>::mk(blkCnt);
                    vecA = inner::vload1_z<1>(pSrcAVec,blkCnt,p0);
                    vecB = inner::vload1_z<1>(pSrcBVec,blkCnt,p0);
                    acc0 = inner::vmacc(acc0, vecA, vecB);
                }

                *px = inner::from_accumulator(inner::vreduce(acc0));

                px += pDst.stride();

                i += strideA;
                /*
                 * Decrement the row loop counter
                 */
                row--;
            }
        }

        if (numRowsA & 1u) {
            col = numColsB;
            i = 0u;
            /*
             * point to last row in output matrix
             */
            px = pOut + pDst.stride() * (numRowsA - 1);
            /*
             * col loop
             */
            while (col > 0) {

                TA const    *pSrcAVec;
                TB const    *pSrcBVec;
                VECA         vecA;
                VECB         vecB;
                ACC           acc0;

                /*
                 * point to last row in matrix A
                 */
                pInA = pSrcA.const_ptr() + (numRowsA - 1) * strideA;
                pInB = pSrcBT + i;

                /*
                 * Set the variable sum, that acts as accumulator, to zero
                 */
                pSrcAVec = (TA const *) pInA;
                pSrcBVec = (TB const *) pInB;
                acc0 = vector_traits<TDST>::temp_acc_zero();

                blkCnt = ((numColsA) / nb_lanes);
                while (blkCnt > 0U) {
                    vecA = inner::vload1<1>(pSrcAVec);
                    pSrcAVec += nb_lanes;
                    vecB = inner::vload1<1>(pSrcBVec);
                    pSrcBVec += nb_lanes;
                    acc0 = inner::vmacc(acc0, vecA, vecB);

                    blkCnt--;
                }
                /*
                 * tail
                 */
                blkCnt = (numColsA & 7);
                if (blkCnt > 0U) {
                    mve_pred16_t    p0 = inner::vctpq<TDST>::mk(blkCnt);
                    vecA = inner::vload1_z<1>(pSrcAVec,blkCnt,p0);
                    vecB = inner::vload1_z<1>(pSrcBVec,blkCnt,p0);
                    acc0 = inner::vmacc(acc0, vecA, vecB);
                }

                *px++ = inner::from_accumulator(inner::vreduce(acc0));

                i += numColsA;

                /*
                 * Decrement the col loop counter
                 */
                col--;
            }
        }

    }
   
}

#endif

/*! @} */