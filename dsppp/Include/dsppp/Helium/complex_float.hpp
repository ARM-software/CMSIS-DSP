// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_MATH_MVEI
#define ARM_MATH_MVEF
#endif

/** \addtogroup HeliumNumber Helium specific number definitions
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup HeliumFloatNumber Float
 *  \ingroup HeliumNumber
 *  @{
 */

/******************
 *
 * Helium
 * 
 */

#if defined(ARM_MATH_MVEF)

// Create complex constant from float using scatter gather
#define SCATTER_CONST

/*


Arch is deriving from Helium

*/
/**
 * @brief      Vector datatype for Helium
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<std::complex<float>,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type >
{
  //! Scalar datatype
  typedef std::complex<float> type;
  //! Storage datatype
  typedef std::complex<float> storage_type;
  //! Vector datatype
  typedef ComplexVector<float32x4_t> vector;
  //! Real vector datatype
  typedef float32x4_t real_vector;
  //! Temp accumulator datatype (must be reduced to final scalar datatype)
  typedef ComplexVector<float32x4_t> temp_accumulator;
  //! Predicate datatype (not used)
  typedef bool predicate_t;
  //! Has vector instruction
  static constexpr bool has_vector = true;
  //! Is float
  static constexpr bool is_float = true;
  //! Is not fixed point
  static constexpr bool is_fixed = false;
  //! Has predicated loops
  static constexpr bool has_predicate = false;

  //! Number of lanes (each lane being two floats)
  static constexpr int nb_lanes = 4;

  //! If support mixed arithmetic
  static constexpr bool support_mixed = true;

  /**
   * @brief      Temp accumulator initialized to 0
   *
   * @return     Temp accumulator initialized to 0
   */
  static ComplexVector<float32x4_t> temp_acc_zero()
  {
     return(ComplexVector<float32x4_t>());
  }

  
  
};


/**
 * \ingroup HeliumNumber
 * @{
 */


namespace inner {


  /* Needed to build but not used : no predication for this
   * datatype
   */
  template<>
  struct vctpq<std::complex<float>> {
    static uint32_t mk(uint32_t v)
    {
       return(v);
    };
  };
  
  /**
   * @brief      Vector constant
   *
   * @param[in]  v     Constant value
   *
   * @return     Vector initialized with constant in each lane
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vconst(const std::complex<float> v)
  {
     float32x4_t bv;
     bv = vdupq_n_f32(v.real());
     bv[1] = v.imag();
     bv[3] = v.imag();

     return(ComplexVector<float32x4_t>(bv,bv));
  }

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vconst(const float32x4_t v)
  {
     float32x4_t ca=vdupq_n_f32(0.0f);
     float32x4_t cb=vdupq_n_f32(0.0f);

     ca[0] = v[0];
     ca[2] = v[1];

     cb[0] = v[2];
     cb[2] = v[3];
     return(ComplexVector<float32x4_t>(ca,cb));
  }

  

  __STATIC_FORCEINLINE float32x4_t half_vconst(const std::complex<float> v)
  {
     float32x4_t bv = vdupq_n_f32(v.real());
     bv[1] = v.imag();
     bv[3] = v.imag();
     return(bv);
  }

  __STATIC_FORCEINLINE float32x4_t half_vconst(const float v)
  {
     float32x4_t bv = vdupq_n_f32(v);
     bv[1] = 0.0f;
     bv[3] = 0.0f;
     return(bv);
  }

  /**
   * @brief      Vector negate
   *
   * @param[in]  a     Vector value to negate
   *
   * @return     Negated value
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vneg(const ComplexVector<float32x4_t> &a)
  {
     return(ComplexVector<float32x4_t>(vnegq(a.va),vnegq(a.vb)));
  };

  static constexpr float32x4_t cmplx_conj_sign = { 1.0f, -1.0f, 1.0f, -1.0f };

  /**
   * @brief      Vector conjugate
   *
   * @param[in]  a     Vector value to conjugate
   *
   * @return     Conjugated value
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vconjugate(const ComplexVector<float32x4_t> &a)
  {
     return (ComplexVector<float32x4_t>(vmulq(a.va, cmplx_conj_sign),vmulq(a.vb, cmplx_conj_sign)));
  };

  
  /**
   * @brief      Vector + Vector
   *
   * @param[in]  a     First operand
   * @param[in]  b     Second operand
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> &a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     return(ComplexVector<float32x4_t>(vaddq(a.va,b.va),vaddq(a.vb,b.vb)));
  };

  /**
   * @brief      Vector + Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> &a,
                                                       const std::complex<float> &b)
  {
     const float32x4_t c = half_vconst(b);
     return(ComplexVector<float32x4_t>(vaddq(a.va,c),vaddq(a.vb,c)));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> &a,
                                                       const float b)
  {
     const float32x4_t c = half_vconst(b);
     return(ComplexVector<float32x4_t>(vaddq(a.va,c),vaddq(a.vb,c)));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> &a,
                                                       const float32x4_t b)
  {
     ComplexVector<float32x4_t> c = vconst(b);
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
   __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const std::complex<float> &a,
                                                        const ComplexVector<float32x4_t> &b)
  {
     const float32x4_t c = half_vconst(a);
     return(ComplexVector<float32x4_t>(vaddq(c,b.va),vaddq(c,b.vb)));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const float a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     const float32x4_t c = half_vconst(a);
     return(ComplexVector<float32x4_t>(vaddq(c,b.va),vaddq(c,b.vb)));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const float32x4_t a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     ComplexVector<float32x4_t> c = vconst(a);
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
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> &a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     return(ComplexVector<float32x4_t>(vsubq(a.va,b.va),vsubq(a.vb,b.vb)));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const float32x4_t a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     ComplexVector<float32x4_t> c = vconst(a);
     return vsub(c,b);
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> &a,
                                                       const float32x4_t b)
  {
     ComplexVector<float32x4_t> c = vconst(b);
     return vsub(a,c);
  };

  /**
   * @brief      Vector - Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> &a,
                                                       const std::complex<float> &b)
  {
     const float32x4_t c = half_vconst(b);
     return(ComplexVector<float32x4_t>(vsubq(a.va,c),vsubq(a.vb,c)));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> &a,
                                                       const float b)
  {
     const float32x4_t c = half_vconst(b);
     return(ComplexVector<float32x4_t>(vsubq(a.va,c),vsubq(a.vb,c)));
  };

  /**
   * @brief      Scalar - Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a - b
   */
   __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const std::complex<float> &a,
                                                        const ComplexVector<float32x4_t> &b)
  {
     const float32x4_t c = half_vconst(a);
     return(ComplexVector<float32x4_t>(vsubq(c,b.va),vsubq(c,b.vb)));
  };

   __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const float a,
                                                        const ComplexVector<float32x4_t> &b)
  {
     const float32x4_t c = half_vconst(a);
     return(ComplexVector<float32x4_t>(vsubq(c,b.va),vsubq(c,b.vb)));
  };

  
  /**
   * @brief      Vector * Vector
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> &a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     float32x4_t vec_acc_a = vcmulq(a.va, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, b.va);

     float32x4_t vec_acc_b = vcmulq(a.vb, b.vb);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, b.vb);

     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const float32x4_t a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     const ComplexVector<float32x4_t> c = vconst(a);

     return(vmul(c,b));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> &a,
                                                       const float32x4_t b)
  {
     const ComplexVector<float32x4_t> c = vconst(b);

     return(vmul(a,c));
  };



  /**
   * @brief      Vector * Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> &a,
                                                       const std::complex<float> &b)
  {
     float32x4_t vecB = half_vconst(b);

     float32x4_t vec_acc_a = vcmulq(a.va, vecB);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, vecB);

     float32x4_t vec_acc_b = vcmulq(a.vb, vecB);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, vecB);

     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> &a,
                                                       const float b)
  {

     float32x4_t vec_acc_a = vmulq_n_f32(a.va, b);
     float32x4_t vec_acc_b = vmulq_n_f32(a.vb, b);

     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };

  /**
   * @brief      Scalar * Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const std::complex<float> &a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     float32x4_t vecA = half_vconst(a);

     float32x4_t vec_acc_a = vcmulq(vecA, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, vecA, b.va);

     float32x4_t vec_acc_b = vcmulq(vecA, b.vb);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, vecA, b.vb);

     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };


  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const float a,
                                                       const ComplexVector<float32x4_t> &b)
  {
     float32x4_t vec_acc_a = vmulq_n_f32(b.va, a);
     float32x4_t vec_acc_b = vmulq_n_f32(b.vb, a);

     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };

  /**
   * @brief      Multiply accumulate (Vector * Vector)
   *
   * @param[in]  acc   Accumulator
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     acc + a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> &acc,
                                                        const ComplexVector<float32x4_t> &a,
                                                        const ComplexVector<float32x4_t> &b)
  {
     float32x4_t vec_acc_a,vec_acc_b;

     vec_acc_a = vcmlaq(acc.va, a.va, b.va);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, b.va);

     vec_acc_b = vcmlaq(acc.vb, a.vb, b.vb);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, b.vb);
     
     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> &acc,
                                                        const float32x4_t a,
                                                        const ComplexVector<float32x4_t> &b)
  {
     ComplexVector<float32x4_t> c = vconst(a);
     return vmacc(acc,c,b);
  };

   __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> &acc,
                                                        const ComplexVector<float32x4_t> &a,
                                                        const float32x4_t b)
   {
     ComplexVector<float32x4_t> c = vconst(b);
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
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> &acc,
                                                        const ComplexVector<float32x4_t> &a,
                                                        const std::complex<float> &b)
  {
     float32x4_t vec_acc_a,vec_acc_b;
     float32x4_t vecB = half_vconst(b);

     vec_acc_a = vcmlaq(acc.va, a.va, vecB);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, vecB);

     vec_acc_b = vcmlaq(acc.vb, a.vb, vecB);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, vecB);
     
     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };

  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> &acc,
                                                        const ComplexVector<float32x4_t> &a,
                                                        const float b)
  {
     float32x4_t vec_acc_a,vec_acc_b;
     float32x4_t vecB = half_vconst(b);

     vec_acc_a = vcmlaq(acc.va, a.va, vecB);
     vec_acc_a = vcmlaq_rot90(vec_acc_a, a.va, vecB);

     vec_acc_b = vcmlaq(acc.vb, a.vb, vecB);
     vec_acc_b = vcmlaq_rot90(vec_acc_b, a.vb, vecB);
     
     return(ComplexVector<float32x4_t>(vec_acc_a,vec_acc_b));
  };


  
  /**
   * @brief      Vector reduce
   *
   * @param[in]  in    Vector
   *
   * @return     Reduced scalar value
   */
  __STATIC_FORCEINLINE std::complex<float> vreduce(const ComplexVector<float32x4_t> &in)
  {
     float re,im;

     re = vgetq_lane(in.va, 0) + vgetq_lane(in.va, 2) + vgetq_lane(in.vb, 0) + vgetq_lane(in.vb, 2);
     im = vgetq_lane(in.va, 1) + vgetq_lane(in.va, 3) + vgetq_lane(in.vb, 1) + vgetq_lane(in.vb, 3);

     return(std::complex<float>(re,im));
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
  inline ComplexVector<float32x4_t> vload1(const std::complex<float32_t> *p)
  {
     return(ComplexVector<float32x4_t>(
      vld1q(reinterpret_cast<const float32_t*>(p)),
      vld1q(reinterpret_cast<const float32_t*>(p)+4)
      ));
  };

 
  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline ComplexVector<float32x4_t> vload1(const std::complex<float32_t> *p)
  {
     constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
     return(ComplexVector<float32x4_t>(
      vldrwq_gather_shifted_offset_f32(reinterpret_cast<const float32_t*>(p),offset),
      vldrwq_gather_shifted_offset_f32(reinterpret_cast<const float32_t*>(p)+4*S,offset)
      ));
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
  inline ComplexVector<float32x4_t> vload1(const std::complex<float32_t> *p,
                                           const index_t stride)
  {
     constexpr uint32x4_t scale = {0,0,2,2};
     constexpr uint32x4_t cmplx = {0,1,0,1};
     uint32x4_t offset;

     offset = vmulq_n_u32(scale,stride);
     offset = vaddq(offset,cmplx);
     return(ComplexVector<float32x4_t>(
      vldrwq_gather_shifted_offset_f32(reinterpret_cast<const float32_t*>(p),offset),
      vldrwq_gather_shifted_offset_f32(reinterpret_cast<const float32_t*>(p)+4*stride,offset)
      ));
  };

  
  /* Generalized stride */

  /**
   * @brief      Load with generalized stride (gather load)
   *
   * @tparam     S     List of offsets known at built time
   */
  template<int ...S>
  struct vload1_gen_stride<std::complex<float32_t>,S...>
  {
     template<int ...IA,int ...IB>
     static ComplexVector<float32x4_t> __run(const std::complex<float32_t> *p,
      std::integer_sequence<int,IA...>,std::integer_sequence<int,IB...>)
     {
        constexpr uint32x4_t offsetA={IA...};
        constexpr uint32x4_t offsetB={IB...};
        return(ComplexVector<float32x4_t>(
         vldrwq_gather_shifted_offset_f32(
            reinterpret_cast<const float32_t*>(p),offsetA),
         vldrwq_gather_shifted_offset_f32(
            reinterpret_cast<const float32_t*>(p),offsetB)
        ));
     };

     template<int ...I>
     static ComplexVector<float32x4_t> _run(const std::complex<float32_t> *p,
      std::integer_sequence<int,I...>)
     {
        constexpr int nb_lanes = vector_traits<std::complex<float32_t>>::nb_lanes;
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
     static ComplexVector<float32x4_t> run(const std::complex<float32_t> *p)
     {
        return _run(p,typename ToComplexStride<S...>::type{});
     };
  };

  /**
   * @brief      Load with generalized stride specialized for <0,1,2,3>
   */
  template<>
  struct vload1_gen_stride<std::complex<float>,0,1,2,3>
  {
     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Loaded vector
      */
     static ComplexVector<float32x4_t> run(const std::complex<float32_t> *p)
     {
        return(ComplexVector<float32x4_t>(
         vld1q(reinterpret_cast<const float32_t*>(p)),
         vld1q(reinterpret_cast<const float32_t*>(p)+4)
         ));
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
  inline void vstore1(std::complex<float32_t> *p,const ComplexVector<float32x4_t> &val)
  {
     vst1q(reinterpret_cast<float32_t*>(p),val.va);
     vst1q(reinterpret_cast<float32_t*>(p)+4,val.vb);
  };

  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline void vstore1(std::complex<float32_t> *p,const float32x4_t val)
  {
     float32x4_t ca,cb;
     ca = vdupq_n_f32(0.0f);
     cb = vdupq_n_f32(0.0f);
     ca[0] = val[0];
     ca[2] = val[1];
     cb[0] = val[2];
     cb[2] = val[3];

     vst1q(reinterpret_cast<float32_t*>(p),ca);
     vst1q(reinterpret_cast<float32_t*>(p)+4,cb);
  };

  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline void vstore1(std::complex<float32_t> *p,const ComplexVector<float32x4_t> &val)
  {
     //uint32x4_t offset={0,1,2,3};
     //uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     //offset = vmulq_n_u32(offset,S);
     constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
     vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p),offset,val.va);
     vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p)+4*S,offset,val.vb);
  };

  // with dynamic stride

  /**
   * @brief      Store with dynamic stride
   *
   * @param      p       Store address
   * @param[in]  stride  Stride value
   * @param[in]  val     Value to store
   */
  inline void vstore1(std::complex<float32_t> *p,const index_t stride,
                      const ComplexVector<float32x4_t> &val)
  {
     constexpr uint32x4_t scale = {0,0,2,2};
     constexpr uint32x4_t cmplx = {0,1,0,1};
     uint32x4_t offset;
     
     offset = vmulq_n_u32(scale,stride);
     offset = vaddq(offset,cmplx);
     vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p),offset,val.va);
     vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p)+4*stride,offset,val.vb);
  };

  
 
 


  // Generalized stride

  /**
   * @brief      Generalized store with strides
   *
   * @tparam     S     Stride values known at built time
   */
  template<int ...S>
  struct vstore1_gen_stride<std::complex<float32_t>,S...>
  {
     template <int... IA,int ... IB>
     static void __run(std::complex<float32_t> *p,
                      const ComplexVector<float32x4_t> &val,
                      std::integer_sequence<int,IA...>,
                      std::integer_sequence<int,IB...>)
     {
        constexpr uint32x4_t offsetA={IA...};
        constexpr uint32x4_t offsetB={IB...};
        vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p),offsetA,val.va);
        vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p),offsetB,val.vb);
     };

     template <int... I>
     static void _run(std::complex<float32_t> *p,
                      const ComplexVector<float32x4_t> &val,
                      std::integer_sequence<int,I...>)
     {
        constexpr int nb_lanes = vector_traits<std::complex<float32_t>>::nb_lanes;
        using l = typename TakeDrop<(nb_lanes>>1),S...>::type;

        __run(p,val,typename l::la{},typename l::lb{});
      }

     /**
      * @brief      Scatter store
      *
      * @param      p     Store address
      * @param[in]  val   VAlue to store
      */
     static void run(std::complex<float32_t> *p,const ComplexVector<float32x4_t> &val)
     {
        return _run(p,val,typename ToComplexStride<S...>::type{});
     };
  };

  /**
   * @brief      Generalized store with stride (Specialized for <0,1,2,3>)
   */
  template<>
  struct vstore1_gen_stride<std::complex<float32_t>,0,1,2,3>
  {
      /**
       * @brief      Scatter store
       *
       * @param      p     Store address
       * @param[in]  val   Value to store
       */
      static void run(std::complex<float32_t> *p,const ComplexVector<float32x4_t> &val)
     {
        vst1q(reinterpret_cast<float32_t*>(p),val.va);
        vst1q(reinterpret_cast<float32_t*>(p)+4,val.vb);
     };
  };

  


};

#if defined(SCATTER_CONST)
#undef SCATTER_CONST
#endif
/*! @} */

#endif

/*! @} */
/*! @} */