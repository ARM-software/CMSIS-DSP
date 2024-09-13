// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_MATH_MVEI
#define ARM_MATH_MVEF
#define ARM_MATH_MVE_FLOAT16
#endif

/** \addtogroup HeliumNumber Helium specific number definitions
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup HeliumComplexHalfNumber Complex Half
 *  \ingroup HeliumNumber
 *  @{
 */

/******************
 *
 * Helium
 * 
 */

#if defined(ARM_MATH_MVE_FLOAT16)


/*


Arch is deriving from Helium

*/
/**
 * @brief      Vector datatype for Helium
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<std::complex<float16_t>,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type >
{
  //! Scalar datatype
  typedef std::complex<float16_t> type;
  //! Storage datatype
  typedef std::complex<float16_t> storage_type;
  //! Vector datatype
  typedef ComplexVector<float16x8_t> vector;
  //! Half vector datatype
  typedef HalfComplexVector<float16x8_t> half_vector;
  //! Real vector datatype
  typedef float16x8_t real_vector;
  //! Temp accumulator datatype (must be reduced to final scalar datatype)
  typedef ComplexVector<float16x8_t> temp_accumulator;
  //! Predicate datatype (not used for complex)
  typedef bool predicate_t;
  //! Has vector instruction
  static constexpr bool has_vector = true;
  //! Is float
  static constexpr bool is_float = true;
  //! Is not fixed point
  static constexpr bool is_fixed = false;
  //! Has predicated loops (not used for complex)
  static constexpr bool has_predicate = false;

  //! Number of lanes (each lane being two floats)
  static constexpr int nb_lanes = 8;

  //! If support mixed arithmetic
  static constexpr bool support_mixed = true;

  /**
   * @brief      Temp accumulator initialized to 0
   *
   * @return     Temp accumulator initialized to 0
   */
  static ComplexVector<float16x8_t> temp_acc_zero()
  {
     return(ComplexVector<float16x8_t>());
  }

  
  
};


/**
 * \ingroup HeliumComplexHalfNumber
 * @{
 */


namespace inner {


  /* 
   * For a complex (2 helium vector)
   * it generates the predicate to use on the last
   * or first vector.
   * 
   */
  template<>
  struct vctpq<std::complex<float16_t>> {
    static mve_pred16_t mk(uint32_t v)
    {
       if (v>4)
       {
         return(vctp16q(2*(v-4)));
       }
       else 
       {
          return(vctp16q(2*v));
       }
    };
  };
  
  /**
   * @brief      Vector constant
   *
   * @param[in]  v     Constant value
   *
   * @return     Vector initialized with constant in each lane
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vconst(const std::complex<float16_t> v)
  {
     float16x8_t bv;

#if defined(SCATTER_CONST)
     static constexpr uint16x8_t rd_offset = {0,1,0,1,0,1,0,1};
     bv=vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(&v),rd_offset);
#else
     bv = vdupq_n_f16(v.real());
     bv[1] = v.imag();
     bv[3] = v.imag();
     bv[5] = v.imag();
     bv[7] = v.imag();
#endif

     return(ComplexVector<float16x8_t>(bv,bv));
  }

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vconst(const float16x8_t v)
  {
     float16x8_t ca;
     float16x8_t cb;

     ca=vdupq_n_f16(0.0f);
     cb=vdupq_n_f16(0.0f);

     ca[0] = v[0];
     ca[2] = v[1];
     ca[4] = v[2];
     ca[6] = v[3];

     cb[0] = v[4];
     cb[2] = v[5];
     cb[4] = v[6];
     cb[6] = v[7];

     return(ComplexVector<float16x8_t>(ca,cb));
  }

  

  __STATIC_FORCEINLINE float16x8_t half_vconst(const std::complex<float16_t> v)
  {
     float16x8_t bv;
#if defined(SCATTER_CONST)
     static constexpr uint16x8_t rd_offset = {0,1,0,1,0,1,0,1};
     bv=vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(&v),rd_offset);
#else
     bv = vdupq_n_f16(v.real());
     bv[1] = v.imag();
     bv[3] = v.imag();
     bv[5] = v.imag();
     bv[7] = v.imag();
#endif
     return(bv);

  }

  __STATIC_FORCEINLINE float16x8_t half_vconst(const float16_t v)
  {
     float16x8_t bv;
#if defined(SCATTER_CONST)
     static float16_t tmp[2];
     tmp[0] = v;
     tmp[1] = 0.0f;
     static constexpr uint16x8_t rd_offset = {0,1,0,1,0,1,0,1};
     bv=vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(tmp),rd_offset);
#else  
     bv = vdupq_n_f16(v);
     bv[1] = 0.0f;
     bv[3] = 0.0f;
     bv[5] = 0.0f;
     bv[7] = 0.0f;
#endif
     return(bv);
  }

  /**
   * @brief      Vector negate
   *
   * @param[in]  a     Vector value to negate
   *
   * @return     Negated value
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vneg(const ComplexVector<float16x8_t> &a)
  {
     return(ComplexVector<float16x8_t>(vnegq(a.va),vnegq(a.vb)));
  };

  static constexpr float16x8_t cmplx_conj_sign_f16 = { 1.0f, -1.0f, 1.0f, -1.0f,1.0f, -1.0f, 1.0f, -1.0f };

  /**
   * @brief      Vector conjugate
   *
   * @param[in]  a     Vector value to conjugate
   *
   * @return     Conjugated value
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vconjugate(const ComplexVector<float16x8_t> &a)
  {
     return (ComplexVector<float16x8_t>(vmulq(a.va, cmplx_conj_sign_f16),vmulq(a.vb, cmplx_conj_sign_f16)));
  };

  
  /**
   * @brief      Vector + Vector
   *
   * @param[in]  a     First operand
   * @param[in]  b     Second operand
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vadd(const ComplexVector<float16x8_t> &a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     return(ComplexVector<float16x8_t>(vaddq(a.va,b.va),vaddq(a.vb,b.vb)));
  };

  /**
   * @brief      Vector + Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vadd(const ComplexVector<float16x8_t> &a,
                                                       const std::complex<float16_t> &b)
  {
     const float16x8_t c = half_vconst(b);
     return(ComplexVector<float16x8_t>(vaddq(a.va,c),vaddq(a.vb,c)));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vadd(const ComplexVector<float16x8_t> &a,
                                                       const float16_t b)
  {
     const float16x8_t c = half_vconst(b);
     return(ComplexVector<float16x8_t>(vaddq(a.va,c),vaddq(a.vb,c)));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vadd(const ComplexVector<float16x8_t> &a,
                                                       const float16x8_t b)
  {
     ComplexVector<float16x8_t> c = vconst(b);
     return(vadd(a,c));
  };

  /**
   * @brief      Scalar + Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a + b
   */
   __STATIC_FORCEINLINE ComplexVector<float16x8_t> vadd(const std::complex<float16_t> &a,
                                                        const ComplexVector<float16x8_t> &b)
  {
     const float16x8_t c = half_vconst(a);
     return(ComplexVector<float16x8_t>(vaddq(c,b.va),vaddq(c,b.vb)));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vadd(const float16_t a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     const float16x8_t c = half_vconst(a);
     return(ComplexVector<float16x8_t>(vaddq(c,b.va),vaddq(c,b.vb)));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vadd(const float16x8_t a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     ComplexVector<float16x8_t> c = vconst(a);
     return(vadd(c,b));
  };

 


   /**
   * @brief      Vector - Vector
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vsub(const ComplexVector<float16x8_t> &a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     return(ComplexVector<float16x8_t>(vsubq(a.va,b.va),vsubq(a.vb,b.vb)));
  };


  /**
   * @brief      Vector - Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vsub(const ComplexVector<float16x8_t> &a,
                                                       const std::complex<float16_t> &b)
  {
     const float16x8_t c = half_vconst(b);
     return(ComplexVector<float16x8_t>(vsubq(a.va,c),vsubq(a.vb,c)));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vsub(const ComplexVector<float16x8_t> &a,
                                                       const float16_t b)
  {
     const float16x8_t c = half_vconst(b);
     return(ComplexVector<float16x8_t>(vsubq(a.va,c),vsubq(a.vb,c)));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vsub(const ComplexVector<float16x8_t> &a,
                                                       const float16x8_t b)
  {
     ComplexVector<float16x8_t> c = vconst(b);
     return vsub(a,c);
  };

  /**
   * @brief      Scalar - Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a - b
   */
   __STATIC_FORCEINLINE ComplexVector<float16x8_t> vsub(const std::complex<float16_t> &a,
                                                        const ComplexVector<float16x8_t> &b)
  {
     const float16x8_t c = half_vconst(a);
     return(ComplexVector<float16x8_t>(vsubq(c,b.va),vsubq(c,b.vb)));
  };

   __STATIC_FORCEINLINE ComplexVector<float16x8_t> vsub(const float16_t a,
                                                        const ComplexVector<float16x8_t> &b)
  {
     const float16x8_t c = half_vconst(a);
     return(ComplexVector<float16x8_t>(vsubq(c,b.va),vsubq(c,b.vb)));
  };

   __STATIC_FORCEINLINE ComplexVector<float16x8_t> vsub(const float16x8_t a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     ComplexVector<float16x8_t> c = vconst(a);
     return vsub(c,b);
  };

  
  /**
   * @brief      Vector * Vector
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const ComplexVector<float16x8_t> &a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     float16x8_t vec_acc_a = vcmulq(a.va, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, b.va);

     float16x8_t vec_acc_b = vcmulq(a.vb, b.vb);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, b.vb);

     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE HalfComplexVector<float16x8_t> vmul(const HalfComplexVector<float16x8_t> &a,
                                                           const HalfComplexVector<float16x8_t> &b)
  {
     float16x8_t vec_acc_a = vcmulq(a.va, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, b.va);

     return(HalfComplexVector<float16x8_t>(vec_acc_a));
  };

 

  /**
   * @brief      Vector * Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const ComplexVector<float16x8_t> &a,
                                                       const std::complex<float16_t> &b)
  {
     float16x8_t vecB = half_vconst(b);

     float16x8_t vec_acc_a = vcmulq(a.va, vecB);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, vecB);

     float16x8_t vec_acc_b = vcmulq(a.vb, vecB);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, vecB);

     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const float16x8_t &a,
                                                       const std::complex<float16_t> &b)
  {
     static float16_t tmp[16];
     float16x8x2_t c;
     float16x8_t va,vb;
     c.val[0] = vmulq_n_f16(a,b.real());
     c.val[1] = vmulq_n_f16(a,b.imag());
     vst2q(tmp,c);
     va=vld1q(tmp);
     vb=vld1q(tmp+8);

     return(ComplexVector<float16x8_t>(va,vb));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const ComplexVector<float16x8_t> &a,
                                                       const float16_t b)
  {

     float16x8_t vec_acc_a = vmulq_n_f16(a.va, b);
     float16x8_t vec_acc_b = vmulq_n_f16(a.vb, b);

     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const ComplexVector<float16x8_t> &a,
                                                       const float16x8_t b)
  {
     ComplexVector<float16x8_t> c = vconst(b);
     return vmul(a,c);
  };

  /**
   * @brief      Scalar * Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const std::complex<float16_t> &a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     float16x8_t vecA = half_vconst(a);

     float16x8_t vec_acc_a = vcmulq(vecA, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, vecA, b.va);

     float16x8_t vec_acc_b = vcmulq(vecA, b.vb);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, vecA, b.vb);

     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

   __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const std::complex<float16_t> &a,
                                                        const float16x8_t &b
                                                       )
  {
     static float16_t tmp[16];
     float16x8x2_t c;
     float16x8_t va,vb;
     c.val[0] = vmulq_n_f16(b,a.real());
     c.val[1] = vmulq_n_f16(b,a.imag());
     vst2q(tmp,c);
     va=vld1q(tmp);
     vb=vld1q(tmp+8);

     return(ComplexVector<float16x8_t>(va,vb));
  };


  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const float16_t a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     float16x8_t vec_acc_a = vmulq_n_f16(b.va, a);
     float16x8_t vec_acc_b = vmulq_n_f16(b.vb, a);

     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmul(const float16x8_t a,
                                                       const ComplexVector<float16x8_t> &b)
  {
     ComplexVector<float16x8_t> c = vconst(a);
     return vmul(c,b);
  }

  /**
   * @brief      Multiply accumulate (Vector * Vector)
   *
   * @param[in]  acc   Accumulator
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     acc + a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmacc(const ComplexVector<float16x8_t> &acc,
                                                        const ComplexVector<float16x8_t> &a,
                                                        const ComplexVector<float16x8_t> &b)
  {
     float16x8_t vec_acc_a,vec_acc_b;

     vec_acc_a = vcmlaq(acc.va, a.va, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, b.va);

     vec_acc_b = vcmlaq(acc.vb, a.vb, b.vb);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, b.vb);
     
     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE HalfComplexVector<float16x8_t> vmacc(const HalfComplexVector<float16x8_t> &acc,
                                                            const HalfComplexVector<float16x8_t> &a,
                                                            const HalfComplexVector<float16x8_t> &b)
  {
     float16x8_t vec_acc_a = vcmlaq(acc.va, a.va, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, b.va);

    
     return(HalfComplexVector<float16x8_t>(vec_acc_a));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmacc(const ComplexVector<float16x8_t> &acc,
                                                        const float16x8_t a,
                                                        const ComplexVector<float16x8_t> &b)
  {
     ComplexVector<float16x8_t> c = vconst(a);
     return vmacc(acc,c,b);
  };

   __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmacc(const ComplexVector<float16x8_t> &acc,
                                                        const ComplexVector<float16x8_t> &a,
                                                        const float16x8_t b)
   {
     ComplexVector<float16x8_t> c = vconst(b);
     return vmacc(acc,a,c);
   }

/**
 * @brief      Multiply accumulate (Vector * Scalar)
 *
 * @param[in]  acc   Accumulator
 * @param[in]  a     Vector
 * @param[in]  b     Scalar
 *
 * @return     acc + a  * b
 */
  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmacc(const ComplexVector<float16x8_t> &acc,
                                                        const ComplexVector<float16x8_t> &a,
                                                        const std::complex<float16_t> &b)
  {
     float16x8_t vec_acc_a,vec_acc_b;
     float16x8_t vecB = half_vconst(b);

     vec_acc_a = vcmlaq(acc.va, a.va, vecB);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, vecB);

     vec_acc_b = vcmlaq(acc.vb, a.vb, vecB);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, vecB);
     
     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmacc(const ComplexVector<float16x8_t> &acc,
                                                        const float16x8_t &a,
                                                        const std::complex<float16_t> &b)
  {
     float16x8_t vec_acc_a,vec_acc_b;
     float16x8_t vecB = half_vconst(b);
     ComplexVector<float16x8_t> ca = vconst(a);

     vec_acc_a = vcmlaq(acc.va, ca.va, vecB);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, ca.va, vecB);

     vec_acc_b = vcmlaq(acc.vb, ca.vb, vecB);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, ca.vb, vecB);
     
     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float16x8_t> vmacc(const ComplexVector<float16x8_t> &acc,
                                                        const ComplexVector<float16x8_t> &a,
                                                        const float16_t b)
  {
     float16x8_t vec_acc_a,vec_acc_b;
     float16x8_t vecB = half_vconst(b);

     vec_acc_a = vcmlaq(acc.va, a.va, vecB);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, vecB);

     vec_acc_b = vcmlaq(acc.vb, a.vb, vecB);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, vecB);
     
     return(ComplexVector<float16x8_t>(vec_acc_a,vec_acc_b));
  };


  
  /**
   * @brief      Vector reduce
   *
   * @param[in]  in    Vector
   *
   * @return     Reduced scalar value
   */
  __STATIC_FORCEINLINE std::complex<float16_t> vreduce(const ComplexVector<float16x8_t> &in)
  {
     float16_t re,im;

     re = (_Float16)vgetq_lane(in.va, 0) + (_Float16)vgetq_lane(in.va, 2) + 
          (_Float16)vgetq_lane(in.va, 4) + (_Float16)vgetq_lane(in.va, 6) + 
          (_Float16)vgetq_lane(in.vb, 0) + (_Float16)vgetq_lane(in.vb, 2) +
          (_Float16)vgetq_lane(in.vb, 4) + (_Float16)vgetq_lane(in.vb, 6)
          ;
     im = (_Float16)vgetq_lane(in.va, 1) + (_Float16)vgetq_lane(in.va, 3) + 
          (_Float16)vgetq_lane(in.va, 5) + (_Float16)vgetq_lane(in.va, 7) + 
          (_Float16)vgetq_lane(in.vb, 1) + (_Float16)vgetq_lane(in.vb, 3) +
          (_Float16)vgetq_lane(in.vb, 5) + (_Float16)vgetq_lane(in.vb, 7);

     return(std::complex<float16_t>(re,im));
  };

  __STATIC_FORCEINLINE std::complex<float16_t> vreduce(const HalfComplexVector<float16x8_t> &in)
  {
     float16_t re,im;

     re = (_Float16)vgetq_lane(in.va, 0) + (_Float16)vgetq_lane(in.va, 2) +
          (_Float16)vgetq_lane(in.va, 4) + (_Float16)vgetq_lane(in.va, 6);
     
     im = (_Float16)vgetq_lane(in.va, 1) + (_Float16)vgetq_lane(in.va, 3) +
          (_Float16)vgetq_lane(in.va, 5) + (_Float16)vgetq_lane(in.va, 7);

     return(std::complex<float16_t>(re,im));
  };



  /**
   * @brief      Vector load with stride 
   *
   * @param[in]  p          Load address
   *
   * @tparam     S          Stride
   * @tparam     <unnamed>  Check stride value
   *
   * @return     Loaded vector with stride
   */
  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline ComplexVector<float16x8_t> vload1(const std::complex<float16_t> *p)
  {
     return(ComplexVector<float16x8_t>(
      vld1q(reinterpret_cast<const float16_t*>(p)),
      vld1q(reinterpret_cast<const float16_t*>(p)+8)
      ));
  };

  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline HalfComplexVector<float16x8_t> vload1_half(const std::complex<float16_t> *p)
  {
     return(HalfComplexVector<float16x8_t>(
      vld1q(reinterpret_cast<const float16_t*>(p))
      ));
  };

  inline HalfComplexVector<float16x8_t> vload1_half(const std::complex<float16_t> *p,
                                                    const index_t stride)
  {
   
   constexpr uint16x8_t scale = {0,0,2,2,4,4,6,6};
   constexpr uint16x8_t cmplx = {0,1,0,1,0,1,0,1};
   uint16x8_t offset;

   offset = vmulq_n_u16(scale,stride);
   offset = vaddq(offset,cmplx);

   return(HalfComplexVector<float16x8_t>(
     vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(p),offset))
   );
  }

  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline ComplexVector<float16x8_t> vload1_z(const std::complex<float16_t> *p,
                                             const std::size_t nb,
                                             const mve_pred16_t p0)
  {
     if (nb > 4)
     {
         return(ComplexVector<float16x8_t>(
               vld1q(reinterpret_cast<const float16_t*>(p)),
               vld1q_z(reinterpret_cast<const float16_t*>(p)+8,p0)
         ));
     }
     else 
     {
         return(ComplexVector<float16x8_t>(
               vld1q_z(reinterpret_cast<const float16_t*>(p),p0),
               vdupq_n_f16(0.0f)
         ));
     }
  };

 
  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline ComplexVector<float16x8_t> vload1(const std::complex<float16_t> *p)
  {
     constexpr uint16x8_t offset={0*S,0*S+1,2*S,2*S+1,4*S,4*S+1,6*S,6*S+1};
     return(ComplexVector<float16x8_t>(
      vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(p),offset),
      vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(p)+8*S,offset)
      ));
  };

  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline HalfComplexVector<float16x8_t> vload1_half_z(const std::complex<float16_t> *p,
                                                      const std::size_t nb,
                                                      const mve_pred16_t p0)
  {
     (void)nb;
     return(HalfComplexVector<float16x8_t>(vld1q_z(reinterpret_cast<const float16_t*>(p),p0)));
  };


  // With dynamic stride
  
  /**
   * @brief      Vector load with dynamic stride
   *
   * @param[in]  p       Load address
   * @param[in]  stride  Stride value
   *
   * @return     Loaded vector with stride
   */
  inline ComplexVector<float16x8_t> vload1(const std::complex<float16_t> *p,
                                           const index_t stride)
  {
     constexpr uint16x8_t scale = {0,0,2,2,4,4,6,6};
     constexpr uint16x8_t cmplx = {0,1,0,1,0,1,0,1};
     uint16x8_t offset;

     offset = vmulq_n_u16(scale,stride);
     offset = vaddq(offset,cmplx);
     return(ComplexVector<float16x8_t>(
      vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(p),offset),
      vldrhq_gather_shifted_offset_f16(reinterpret_cast<const float16_t*>(p)+8*stride,offset)
      ));
  };

  
  /* Generalized stride */

  /**
   * @brief      Load with generalized stride (gather load)
   *
   * @tparam     S     List of offsets known at built time
   */
  template<int ...S>
  struct vload1_gen_stride<std::complex<float16_t>,S...>
  {
     template<int ...IA,int ...IB>
     static ComplexVector<float16x8_t> __run(const std::complex<float16_t> *p,
      std::integer_sequence<int,IA...>,std::integer_sequence<int,IB...>)
     {
        constexpr uint16x8_t offsetA={IA...};
        constexpr uint16x8_t offsetB={IB...};
        return(ComplexVector<float16x8_t>(
         vldrhq_gather_shifted_offset_f16(
            reinterpret_cast<const float16_t*>(p),offsetA),
         vldrhq_gather_shifted_offset_f16(
            reinterpret_cast<const float16_t*>(p),offsetB)
        ));
     };

     template<int ...I>
     static ComplexVector<float16x8_t> _run(const std::complex<float16_t> *p,
      std::integer_sequence<int,I...>)
     {
        constexpr int nb_lanes = vector_traits<std::complex<float16_t>>::nb_lanes;
        using l = typename TakeDrop<(nb_lanes>>1),S...>::type;

        return __run(p,typename l::la{},typename l::lb{});
     };

     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Gather load
      */
     static ComplexVector<float16x8_t> run(const std::complex<float16_t> *p)
     {
        return _run(p,typename ToComplexStride<S...>::type{});
     };
  };

  /**
   * @brief      Load with generalized stride specialized for <0,1,2,3,4,5,6,7>
   */
  template<>
  struct vload1_gen_stride<std::complex<float16_t>,0,1,2,3,4,5,6,7>
  {
     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Loaded vector
      */
     static ComplexVector<float16x8_t> run(const std::complex<float16_t> *p)
     {
        return(ComplexVector<float16x8_t>(
         vld1q(reinterpret_cast<const float16_t*>(p)),
         vld1q(reinterpret_cast<const float16_t*>(p)+8)
         ));
     };
  };

  template<int ...S>
  struct vload1_half_gen_stride<std::complex<float16_t>,S...>
  {
     template<int ...I>
     static HalfComplexVector<float16x8_t> _run(const std::complex<float16_t> *p,
      std::integer_sequence<int,I...>)
     {
        constexpr uint16x8_t offsetA={I...};
        return(HalfComplexVector<float16x8_t>(
         vldrhq_gather_shifted_offset_f16(
            reinterpret_cast<const float16_t*>(p),offsetA)
        ));
     };

     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Gather load
      */
     static HalfComplexVector<float16x8_t> run(const std::complex<float16_t> *p)
     {
        return _run(p,typename ToComplexStride<S...>::type{});
     };
  };

  
  
  
  /**
   * @brief      Store with stride
   *
   * @param      p          Store address
   * @param[in]  val        Value to store
   *
   * @tparam     S          Stride
   * @tparam     <unnamed>  Check stride value
   */
  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline void vstore1(std::complex<float16_t> *p,const ComplexVector<float16x8_t> &val)
  {
     vst1q(reinterpret_cast<float16_t*>(p),val.va);
     vst1q(reinterpret_cast<float16_t*>(p)+8,val.vb);
  };

  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline void vstore1_z(std::complex<float16_t> *p,
                        ComplexVector<float16x8_t> &val,
                        const std::size_t nb,
                        const mve_pred16_t p0)
  {
     if (nb>4)
     {
       vst1q(reinterpret_cast<float16_t*>(p),val.va);
       vstrhq_p(reinterpret_cast<float16_t*>(p)+8,val.vb,p0);
     }
     else 
     {
        vstrhq_p(reinterpret_cast<float16_t*>(p),val.va,p0);
     }
  };


  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline void vstore1(std::complex<float16_t> *p,const float16x8_t val)
  {
     float16x8_t ca,cb;
     ca = vdupq_n_f16(0.0f);
     cb = vdupq_n_f16(0.0f);
     ca[0] = val[0];
     ca[2] = val[1];
     ca[4] = val[2];
     ca[6] = val[3];

     cb[0] = val[4];
     cb[2] = val[5];
     cb[4] = val[6];
     cb[6] = val[7];

     vst1q(reinterpret_cast<float16_t*>(p),ca);
     vst1q(reinterpret_cast<float16_t*>(p)+8,cb);
  };

  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline void vstore1(std::complex<float16_t> *p,const ComplexVector<float16x8_t> &val)
  {
     constexpr uint16x8_t offset={0*S,0*S+1,2*S,2*S+1,4*S,4*S+1,6*S,6*S+1};
     vstrhq_scatter_shifted_offset_f16(reinterpret_cast<float16_t*>(p),offset,val.va);
     vstrhq_scatter_shifted_offset_f16(reinterpret_cast<float16_t*>(p)+8*S,offset,val.vb);
  };

  // with dynamic stride

  /**
   * @brief      Store with dynamic stride
   *
   * @param      p       Store address
   * @param[in]  stride  Stride value
   * @param[in]  val     Value to store
   */
  inline void vstore1(std::complex<float16_t> *p,const index_t stride,
                      const ComplexVector<float16x8_t> &val)
  {
     constexpr uint16x8_t scale = {0,0,2,2,4,4,6,6};
     constexpr uint16x8_t cmplx = {0,1,0,1,0,1,0,1};
     uint16x8_t offset;
     
     offset = vmulq_n_u16(scale,stride);
     offset = vaddq(offset,cmplx);
     vstrhq_scatter_shifted_offset_f16(reinterpret_cast<float16_t*>(p),offset,val.va);
     vstrhq_scatter_shifted_offset_f16(reinterpret_cast<float16_t*>(p)+8*stride,offset,val.vb);
  };

  
 
 


  // Generalized stride

  /**
   * @brief      Generalized store with strides
   *
   * @tparam     S     Stride values known at built time
   */
  template<int ...S>
  struct vstore1_gen_stride<std::complex<float16_t>,S...>
  {
     template <int... IA,int ... IB>
     static void __run(std::complex<float16_t> *p,
                      const ComplexVector<float16x8_t> &val,
                      std::integer_sequence<int,IA...>,
                      std::integer_sequence<int,IB...>)
     {
        constexpr uint16x8_t offsetA={IA...};
        constexpr uint16x8_t offsetB={IB...};
        vstrhq_scatter_shifted_offset_f16(reinterpret_cast<float16_t*>(p),offsetA,val.va);
        vstrhq_scatter_shifted_offset_f16(reinterpret_cast<float16_t*>(p),offsetB,val.vb);
     };

     template <int... I>
     static void _run(std::complex<float16_t> *p,
                      const ComplexVector<float16x8_t> &val,
                      std::integer_sequence<int,I...>)
     {
        constexpr int nb_lanes = vector_traits<std::complex<float16_t>>::nb_lanes;
        using l = typename TakeDrop<(nb_lanes>>1),S...>::type;

        __run(p,val,typename l::la{},typename l::lb{});
      }

     /**
      * @brief      Scatter store
      *
      * @param      p     Store address
      * @param[in]  val   VAlue to store
      */
     static void run(std::complex<float16_t> *p,const ComplexVector<float16x8_t> &val)
     {
        return _run(p,val,typename ToComplexStride<S...>::type{});
     };
  };

  /**
   * @brief      Generalized store with stride (Specialized for <0,1,2,3>)
   */
  template<>
  struct vstore1_gen_stride<std::complex<float16_t>,0,1,2,3,4,5,6,7>
  {
      /**
       * @brief      Scatter store
       *
       * @param      p     Store address
       * @param[in]  val   Value to store
       */
      static void run(std::complex<float16_t> *p,const ComplexVector<float16x8_t> &val)
     {
        vst1q(reinterpret_cast<float16_t*>(p),val.va);
        vst1q(reinterpret_cast<float16_t*>(p)+8,val.vb);
     };
  };

  


};

/*! @} */

#endif

/*! @} */
/*! @} */