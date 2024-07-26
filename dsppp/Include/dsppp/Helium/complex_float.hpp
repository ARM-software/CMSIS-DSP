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
  //! Temp accumulator datatype (must be reduced to final scalar datatype)
  typedef ComplexVector<float32x4_t> temp_accumulator;
  //! Predicate datatype
  typedef mve_pred16_t predicate_t;
  //! Has vector instruction
  static constexpr bool has_vector = true;
  //! Is float
  static constexpr bool is_float = true;
  //! Is not fixed point
  static constexpr bool is_fixed = false;
  //! Has predicated loops
  static constexpr bool has_predicate = true;

  //! Number of lanes (each lane being two floats)
  static constexpr int nb_lanes = 2;

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


  /**
   * @brief      vctpq for this datatype
   */
  template<>
  struct vctpq<std::complex<float>> {
   /**
    * @brief      Make a predicate
    *
    * @param[in]  v     Number of iterations
    *
    * @return     Predicate
    */
    static mve_pred16_t mk(uint32_t v)
    {
       // An iteration is 2 complex
       // So remaining iterations can at most be 1
       // and it corresponds to 2 float
       return(vctp32q(2*v));
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
     float32x4_t bv = vdupq_n_f32(v.real());
     bv[1] = v.imag();
     bv[3] = v.imag();
     return(ComplexVector<float32x4_t>(bv));
  }

  /**
   * @brief      Vector constant with tail
   *
   * @param[in]  v     Constant value
   * @param[in]  p0    Prddicate
   *
   * @return     Vector initialized with constant in some lanes
   *             dependinn on the predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vconst_tail(const std::complex<float> v,
                                                              const mve_pred16_t p0)
  {
     (void)p0;
     float32x4_t bv = vdupq_n_f32(v.real());
     bv[1] = v.imag();
     bv[3] = v.imag();
     return(ComplexVector<float32x4_t>(bv));
  }

  /**
   * @brief      Vector negate
   *
   * @param[in]  a     Vector value to negate
   *
   * @return     Negated value
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vneg(const ComplexVector<float32x4_t> a)
  {
     return(ComplexVector<float32x4_t>(vnegq(a.v)));
  };

  /**
   * @brief      Vector negate with tail
   *
   * @param[in]  a     Value
   * @param[in]  p0    Predicate
   *
   * @return     Negated value
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vneg(const ComplexVector<float32x4_t> a,
                                                       const mve_pred16_t p0)
  {
     return(ComplexVector<float32x4_t>(vnegq_x(a.v,p0)));
  };

  static constexpr float32x4_t cmplx_conj_sign = { 1.0f, -1.0f, 1.0f, -1.0f };

  /**
   * @brief      Vector conjugate
   *
   * @param[in]  a     Vector value to conjugate
   *
   * @return     Conjugated value
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vconjugate(const ComplexVector<float32x4_t> a)
  {
     return (ComplexVector<float32x4_t>(vmulq(a.v, cmplx_conj_sign)));
  };

  /**
   * @brief      Vector conjugate with tail
   *
   * @param[in]  a     Value
   * @param[in]  p0    Predicate
   *
   * @return     Conjugated value
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vconjugate(const ComplexVector<float32x4_t> a,
                                                             const mve_pred16_t p0)
  {
     return (ComplexVector<float32x4_t>(vmulq_x(a.v, cmplx_conj_sign,p0)));
  };
  
  /**
   * @brief      Vector + Vector
   *
   * @param[in]  a     First operand
   * @param[in]  b     Second operand
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> a,
                                                       const ComplexVector<float32x4_t> b)
  {
     return(ComplexVector<float32x4_t>(vaddq(a.v,b.v)));
  };

  /**
   * @brief      Vector + Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> a,
                                                       const std::complex<float> b)
  {
     
     return(ComplexVector<float32x4_t>(vaddq(a.v,vconst(b).v)));
  };

  /**
   * @brief      Scalar + Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a + b
   */
   __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const std::complex<float> a,
                                                        const ComplexVector<float32x4_t> b)
  {

     return(ComplexVector<float32x4_t>(vaddq(vconst(a).v,b.v)));
  };

  /**
   * @brief      Vector + Vector with tail
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   * @param[in]  p0    Predicated
   *
   * @return     a + b with tail predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> a,
                                                       const ComplexVector<float32x4_t> b,
                                                       const mve_pred16_t p0)
  {
     return(ComplexVector<float32x4_t>(vaddq_x(a.v,b.v,p0)));
  };


  /**
   * @brief      Vector + scalar with tail
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   * @param[in]  p0    Predicate
   *
   * @return     a + b with tail predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const ComplexVector<float32x4_t> a,
                                                       const std::complex<float> b,
                                                       const mve_pred16_t p0)
  {
     return(ComplexVector<float32x4_t>(vaddq_x(a.v,vconst(b).v,p0)));
  };

  /**
   * @brief      Scalar + vector with tail predicate
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   * @param[in]  p0    Predicate
   *
   * @return     a + b with tail predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vadd(const std::complex<float> a,
                                                       const ComplexVector<float32x4_t> b,
                                                       const mve_pred16_t p0)
  {
     return(ComplexVector<float32x4_t>(vaddq_x(vconst(a).v,b.v,p0)));
  };

  /**
   * @brief      Vector - Vector
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> a,
                                                       const ComplexVector<float32x4_t> b)
  {
     return(ComplexVector<float32x4_t>(vsubq(a.v,b.v)));
  };

  /**
   * @brief      Vector - Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> a,
                                                       const std::complex<float> b)
  {
     return(ComplexVector<float32x4_t>(vsubq(a.v,vconst(b).v)));
  };

  /**
   * @brief      Scalar - Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a - b
   */
   __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const std::complex<float> a,
                                                        const ComplexVector<float32x4_t> b)
  {
     return(ComplexVector<float32x4_t>(vsubq(vconst(a).v,b.v)));
  };

  /**
   * @brief      Vector - Vector with predicate
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   * @param[in]  p0    Predicate
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> a,
                                                       const ComplexVector<float32x4_t> b,
                                                       const mve_pred16_t p0)
  {
     return(ComplexVector<float32x4_t>(vsubq_x(a.v,b.v,p0)));
  };

  /**
   * @brief      Vector - Scalar with predicate
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   * @param[in]  p0    predicate
   *
   * @return     a - b with predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const ComplexVector<float32x4_t> a,
                                                       const std::complex<float> b,
                                                       const mve_pred16_t p0)
  {
     return(ComplexVector<float32x4_t>(vsubq_x(a.v,vconst(b).v,p0)));
  };

  /**
   * @brief      Scalar - Vector with predicate
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   * @param[in]  p0    predicate
   *
   * @return     a - b with predicate
   */
   __STATIC_FORCEINLINE ComplexVector<float32x4_t> vsub(const std::complex<float> a,
                                                        const ComplexVector<float32x4_t> b,
                                                        const mve_pred16_t p0)
  {
     return(ComplexVector<float32x4_t>(vsubq_x(vconst(a).v,b.v,p0)));
  };
  
  /**
   * @brief      Vector * Vector
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> a,
                                                       const ComplexVector<float32x4_t> b)
  {
     float32x4_t vec_acc = vcmulq(a.v, b.v);
     vec_acc = vcmlaq_rot90(vec_acc, a.v, b.v);

     return(ComplexVector<float32x4_t>(vec_acc));
  };

  /**
   * @brief      Vector * Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> a,
                                                       const std::complex<float> b)
  {
     float32x4_t vecB = vconst(b).v;
     float32x4_t vec_acc = vcmulq(a.v, vecB);
     vec_acc = vcmlaq_rot90(vec_acc, a.v, vecB);

     return(ComplexVector<float32x4_t>(vec_acc));
  };

  /**
   * @brief      Scalar * Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const std::complex<float> a,
                                                       const ComplexVector<float32x4_t> b)
  {
     float32x4_t vecA = vconst(a).v;
     float32x4_t vec_acc = vcmulq(vecA, b.v);
     vec_acc = vcmlaq_rot90(vec_acc, vecA, b.v);

     return(ComplexVector<float32x4_t>(vec_acc));
  };

 /**
  * @brief      Vector * Vector with predicate
  *
  * @param[in]  a     Vector
  * @param[in]  b     Vector
  * @param[in]  p0    Predicate
  *
  * @return     a * b
  */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> a,
                                                       const ComplexVector<float32x4_t> b,
                                                       const mve_pred16_t p0)
  {
     float32x4_t vec_acc = vcmulq_x(a.v, b.v,p0);
     vec_acc = vcmlaq_rot90_m(vec_acc, a.v, b.v,p0);

     return(ComplexVector<float32x4_t>(vec_acc));
  };

  /**
   * @brief      Vector * Scalar with predicate
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   * @param[in]  p0    Predicate
   *
   * @return     a * b with predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const ComplexVector<float32x4_t> a,
                                                       const std::complex<float> b,
                                                       const mve_pred16_t p0)
  {
     float32x4_t vecB = vconst(b).v;
     float32x4_t vec_acc = vcmulq_x(a.v, vecB,p0);
     vec_acc = vcmlaq_rot90_m(vec_acc, a.v, vecB,p0);

     return(ComplexVector<float32x4_t>(vec_acc));
  };

  /**
   * @brief      Scalar * Vector with predicate
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   * @param[in]  p0    Predicate
   *
   * @return     a * b with predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmul(const std::complex<float> a,
                                                       const ComplexVector<float32x4_t> b,
                                                       const mve_pred16_t p0)
  {
     float32x4_t vecA = vconst(a).v;
     float32x4_t vec_acc = vcmulq_x(vecA, b.v,p0);
     vec_acc = vcmlaq_rot90_m(vec_acc, vecA, b.v,p0);

     return(ComplexVector<float32x4_t>(vec_acc));
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
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> acc,
                                                        const ComplexVector<float32x4_t> a,
                                                        const ComplexVector<float32x4_t> b)
  {
     float32x4_t vec_acc;

     vec_acc = vcmlaq(acc.v, a.v, b.v);
     vec_acc = vcmlaq_rot90(vec_acc, a.v, b.v);
     
     return(ComplexVector<float32x4_t>(vec_acc));
  };

/**
 * @brief      Multiply accumulate (Vector * Scalar)
 *
 * @param[in]  acc   Accumulator
 * @param[in]  a     Vector
 * @param[in]  b     Scalar
 *
 * @return     acc + a  * b
 */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> acc,
                                                        const ComplexVector<float32x4_t> a,
                                                        const std::complex<float> b)
  {
     float32x4_t vec_acc;
     float32x4_t vecB = vconst(b).v;

     vec_acc = vcmlaq(acc.v, a.v, vecB);
     vec_acc = vcmlaq_rot90(vec_acc, a.v, vecB);
     
     return(ComplexVector<float32x4_t>(vec_acc));
  };

  /**
   * @brief      Multiply accumulate with predicate (Vector * Vector)
   *
   * @param[in]  acc   Accumulator
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   * @param[in]  p0    Predicate
   *
   * @return     acc + a*b with predicate
   */
  __STATIC_FORCEINLINE ComplexVector<float32x4_t> vmacc(const ComplexVector<float32x4_t> acc,
                                                        const ComplexVector<float32x4_t> a,
                                                        const ComplexVector<float32x4_t> b,
                                                        const mve_pred16_t p0)
  {
     float32x4_t vec_acc;

     vec_acc = vcmlaq_m(acc.v, a.v, b.v,p0);
     vec_acc = vcmlaq_rot90_m(vec_acc, a.v, b.v,p0);
     
     return(ComplexVector<float32x4_t>(vec_acc));
  };



  /**
   * @brief      Vector reduce
   *
   * @param[in]  in    Vector
   *
   * @return     Reduced scalar value
   */
  __STATIC_FORCEINLINE std::complex<float> vreduce(const ComplexVector<float32x4_t> in)
  {
     float re,im;

     re = vgetq_lane(in.v, 0) + vgetq_lane(in.v, 2);
     im = vgetq_lane(in.v, 1) + vgetq_lane(in.v, 3);

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
     return(ComplexVector<float32x4_t>(vld1q(reinterpret_cast<const float32_t*>(p))));
  };

 
  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline ComplexVector<float32x4_t> vload1(const std::complex<float32_t> *p)
  {
     constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
     return(ComplexVector<float32x4_t>(vldrwq_gather_shifted_offset_f32(reinterpret_cast<const float32_t*>(p),offset)));
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
  inline ComplexVector<float32x4_t> vload1(const std::complex<float32_t> *p,const index_t stride)
  {
     constexpr uint32x4_t scale = {0,0,2,2};
     constexpr uint32x4_t cmplx = {0,1,0,1};
     uint32x4_t offset;

     offset = vmulq_n_u32(scale,stride);
     offset = vaddq(offset,cmplx);
     return(ComplexVector<float32x4_t>(vldrwq_gather_shifted_offset_f32(reinterpret_cast<const float32_t*>(p),offset)));
  };

  
  /**
   * @brief      Vector load with stride and predicate 
   *
   * @param[in]  p          Load address
   * @param[in]  nb         Number of remaining loop samples
   * @param[in]  p0         Predicate for remaining loop samples
   *
   * @tparam     S          Stride
   * @tparam     <unnamed>  Check stride value
   *
   * @return     Loaded vector with stride and loop predication
   */
  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline ComplexVector<float32x4_t> vload1_z(const std::complex<float32_t> *p,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     return(ComplexVector<float32x4_t>(vld1q_z(reinterpret_cast<const float32_t*>(p),p0)));
  };


  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline ComplexVector<float32x4_t> vload1_z(const std::complex<float32_t> *p,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
     return(ComplexVector<float32x4_t>(vldrwq_gather_shifted_offset_z_f32(reinterpret_cast<const float32_t*>(p),offset,p0)));
  };

  // With dynamic stride

  /**
   * @brief      Vector load with dynamic stride and loop predication
   *
   * @param[in]  p       Load address
   * @param[in]  stride  Stride value
   * @param[in]  nb      Number of remaining loop samples
   * @param[in]  p0      Predicate for remaining loop samples
   *
   * @return     Loaded vector with stride and loop predicate
   */
  inline ComplexVector<float32x4_t> vload1_z(const std::complex<float32_t> *p,const index_t stride,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     constexpr uint32x4_t scale = {0,0,2,2};
     constexpr uint32x4_t cmplx = {0,1,0,1};
     uint32x4_t offset;
     
     offset = vmulq_n_u32(scale,stride);
     offset = vaddq(offset,cmplx);
     return(ComplexVector<float32x4_t>(vldrwq_gather_shifted_offset_z_f32(reinterpret_cast<const float32_t*>(p),offset,p0)));
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
     template<int ...I>
     static ComplexVector<float32x4_t> _run(const std::complex<float32_t> *p,std::integer_sequence<int,I...>)
     {
        constexpr uint32x4_t offset={I...};
        return(ComplexVector<float32x4_t>(vldrwq_gather_shifted_offset_f32(reinterpret_cast<const float32_t*>(p),offset)));
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
  struct vload1_gen_stride<std::complex<float>,0,1>
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
        return(ComplexVector<float32x4_t>(vld1q(reinterpret_cast<const float32_t*>(p))));
     };
  };

  /* Generalized stride */

  /**
   * @brief      Load with generalized stride (gather load) and tail predicate
   *
   * @tparam     S     List of offsets known at built time
   */
  template<int ...S>
  struct vload1_gen_stride_z<std::complex<float32_t>,S...>
  {
     template<int...I>
     static ComplexVector<float32x4_t> run(const std::complex<float32_t> *p,
                                           const std::size_t nb,
                                           const mve_pred16_t p0,
                                           std::integer_sequence<int,I...>
                                           )
     {
        constexpr uint32x4_t offset={I...};
        (void)nb;
        return(ComplexVector<float32x4_t>(vldrwq_gather_shifted_offset_z_f32(reinterpret_cast<const float32_t*>(p),offset,p0)));
     };
  
     /**
      * @brief      Load
      *
      * @param[in]  p     Load address
      * @param[in]  nb    Number of remaining samples in loop
      * @param[in]  p0    Predicate for remaining samples
      *
      * @return     Gather load with predicate
      */
     static ComplexVector<float32x4_t> run(const std::complex<float32_t> *p,const std::size_t nb,const mve_pred16_t p0)
     {
        _run(p,nb,p0,typename ToComplexStride<S...>::type{});
     };
  };

  /**
   * @brief      Load with generalized stride (gather load) and tail predicate specialized for <0,1,2,3>
   *
   * @tparam     S     List of offsets known at built time
   */
  template<>
  struct vload1_gen_stride_z<std::complex<float32_t>,0,1>
  {
     /**
      * @brief      Gather load with predicated specialized for <0,1,2,3>
      *
      * @param[in]  p     Load address
      * @param[in]  nb    Number of remaining samples in the loop
      * @param[in]  p0    Predicate for samples in the loop
      *
      * @return     Gather load
      */
     static ComplexVector<float32x4_t> run(const std::complex<float32_t> *p,const std::size_t nb,const mve_pred16_t p0)
     {
        (void)nb;
        return(ComplexVector<float32x4_t>(vld1q_z(reinterpret_cast<const float32_t*>(p),p0)));
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
  inline void vstore1(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val)
  {
     vst1q(reinterpret_cast<float32_t*>(p),val.v);
  };

  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline void vstore1(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val)
  {
     //uint32x4_t offset={0,1,2,3};
     //uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     //offset = vmulq_n_u32(offset,S);
     constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
     vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p),offset,val.v);
  };

  // with dynamic stride

  /**
   * @brief      Store with dynamic stride
   *
   * @param      p       Store address
   * @param[in]  stride  Stride value
   * @param[in]  val     Value to store
   */
  inline void vstore1(std::complex<float32_t> *p,const index_t stride,const ComplexVector<float32x4_t> val)
  {
     constexpr uint32x4_t scale = {0,0,2,2};
     constexpr uint32x4_t cmplx = {0,1,0,1};
     uint32x4_t offset;
     
     offset = vmulq_n_u32(scale,stride);
     offset = vaddq(offset,cmplx);
     vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p),offset,val.v);
  };

  
  /**
   * @brief      Store with stride and tail predicate
   *
   * @param      p          Store address
   * @param[in]  val        Value to store
   * @param[in]  nb         Number of remaining loop iterations
   * @param[in]  p0         Predicate for loop
   *
   * @tparam     S          Stride
   * @tparam     <unnamed>  Check stride value
   */
  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline void vstore1_z(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     vstrwq_p(reinterpret_cast<float32_t*>(p),val.v,p0);
  };

  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline void vstore1_z(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     //uint32x4_t offset={0,1,2,3};
     //uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     //offset = vmulq_n_u32(offset,S);
     constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
     vstrwq_scatter_shifted_offset_p_f32(reinterpret_cast<float32_t*>(p),offset,val.v,p0);
  };

  // with dynamic stride

  /**
   * @brief      Store with dynamic stride
   *
   * @param      p       Store address
   * @param[in]  stride  Stride value
   * @param[in]  val     Value to store
   * @param[in]  nb      Number of remaining loops
   * @param[in]  p0      Predicate for loop
   */
  inline void vstore1_z(std::complex<float32_t> *p,const index_t stride,const ComplexVector<float32x4_t> val,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     constexpr uint32x4_t scale = {0,0,2,2};
     constexpr uint32x4_t cmplx = {0,1,0,1};
     uint32x4_t offset;
     
     offset = vmulq_n_u32(scale,stride);
     offset = vaddq(offset,cmplx);
     vstrwq_scatter_shifted_offset_p_f32(reinterpret_cast<float32_t*>(p),offset,val.v,p0);
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
     template <int... I>
     static void _run(std::complex<float32_t> *p,
                      const ComplexVector<float32x4_t> val,
                      std::integer_sequence<int,I...>)
     {
        constexpr uint32x4_t offset={I...};
        vstrwq_scatter_shifted_offset_f32(reinterpret_cast<float32_t*>(p),offset,val.v);
     };
     /**
      * @brief      Scatter store
      *
      * @param      p     Store address
      * @param[in]  val   VAlue to store
      */
     static void run(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val)
     {
        return _run(p,val.v,typename ToComplexStride<S...>::type{});
     };
  };

  /**
   * @brief      Generalized store with stride (Specialized for <0,1,2,3>)
   */
  template<>
  struct vstore1_gen_stride<std::complex<float32_t>,0,1>
  {
      /**
       * @brief      Scatter store
       *
       * @param      p     Store address
       * @param[in]  val   Value to store
       */
      static void run(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val)
     {
        vst1q(reinterpret_cast<float32_t*>(p),val.v);
     };
  };

  /**
   * @brief      Store with generalized strides and tail predicate
   *
   * @tparam     S     Strides values known at built time
   */
  template<int ...S>
  struct vstore1_gen_stride_z<std::complex<float32_t>,S...>
  {
      template <int... I>
      static void vstore1_z(std::complex<float32_t> *p,
                            const ComplexVector<float32x4_t> val,
                            const std::size_t nb,
                            const mve_pred16_t p0,
                            std::integer_sequence<int,I...>)
      {
        constexpr uint32x4_t offset={I...};
        (void)nb;
        vstrwq_scatter_shifted_offset_p_f32(reinterpret_cast<float32_t*>(p),offset,val.v,p0);
      }
      /**
       * @brief      Scatter store with tail predicate
       *
       * @param      p     Store address
       * @param[in]  val   Value to store
       * @param[in]  nb    Remaining number of loops
       * @param[in]  p0    Loop predicate
       */
      static void vstore1_z(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val,const std::size_t nb,const mve_pred16_t p0)
      {
        return _run(p,val.v,nb,p0,typename ToComplexStride<S...>::type{});
      }
  };

  /**
   * @brief      Scatter store with tail predicate (specialized for <0,1,2,3>)
   */
  template<>
  struct vstore1_gen_stride_z<std::complex<float32_t>,0,1>
  {
     /**
      * @brief      Scatter store with tail predicate
      *
      * @param      p     Store address
      * @param[in]  val   Value to store
      * @param[in]  nb    Number of remaining loops
      * @param[in]  p0    Loop predicate
      */
     static void vstore1_z(std::complex<float32_t> *p,const ComplexVector<float32x4_t> val,const std::size_t nb,const mve_pred16_t p0)
     {
        (void)nb;
        vstrwq_p(reinterpret_cast<float32_t*>(p),val.v,p0);
     }

  };



};
/*! @} */

#endif

/*! @} */
/*! @} */