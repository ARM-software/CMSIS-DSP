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
struct vector_traits<float,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type >
{
  //! Scalar datatype
  typedef float type;
  //! Storage datatype
  typedef float storage_type;
  //! Vector datatype
  typedef float32x4_t vector;
  typedef float32x4_t real_vector;
  //! Temp accumulator datatype (must be reduced to final scalar datatype)
  typedef float32x4_t temp_accumulator;
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

  //! Number of lanes
  static constexpr int nb_lanes = 4;

  /**
   * @brief      Temp accumulator initialized to 0
   *
   * @return     Temp accumulator initialized to 0
   */
  static float32x4_t temp_acc_zero()
  {
     return(vdupq_n_f32(0.0f));
  }

  /**
   * @brief      Zero lane
   *
   * @return     Value to write 0 into a lane
   */
  static constexpr float zero_lane() {return 0.0f;};
  
  // Useful in fixed point since lane value is an int and not a Q something
  
  /**
   * @brief      Lane value
   *
   * @param[in]  x    Lane value
   *
   * @return     Value with scalar datatype
   */
  static constexpr float lane_value(const float x) {return x;};
  
};


/**
 * \ingroup HeliumFloatNumber
 * @{
 */
namespace inner {


  /**
   * @brief      vctpq for this datatype
   */
  template<>
  struct vctpq<float> {
   /**
    * @brief      Make a predicate
    *
    * @param[in]  v     Number of iterations
    *
    * @return     Predicate
    */
    static mve_pred16_t mk(uint32_t v)
    {
       return(vctp32q(v));
    };
  };
  
  /**
   * @brief      Vector constant
   *
   * @param[in]  v     Constant value
   *
   * @return     Vector initialized with constant in each lane
   */
  __STATIC_FORCEINLINE float32x4_t vconst(const float v)
  {
     return(vdupq_n_f32(v));
  }

  /**
   * @brief      Vector constant with tail
   *
   * @param[in]  v     Constant value
   * @param[in]  p0    Prddicate
   *
   * @return     Vector initialized with constant in some lanes
   *             dependign on the predicate
   */
  __STATIC_FORCEINLINE float32x4_t vconst_tail(const float v,
                                               const mve_pred16_t p0)
  {
     return(vdupq_x_n_f32(v,p0));
  }

  /**
   * @brief      Vector negate
   *
   * @param[in]  a     Vector value to negate
   *
   * @return     Negated value
   */
  __STATIC_FORCEINLINE float32x4_t vneg(const float32x4_t a)
  {
     return(vnegq(a));
  };

  /**
   * @brief      Vector negate with tail
   *
   * @param[in]  a     Value
   * @param[in]  p0    Predicate
   *
   * @return     Negated value
   */
  __STATIC_FORCEINLINE float32x4_t vneg(const float32x4_t a,
                                        const mve_pred16_t p0)
  {
     return(vnegq_x(a,p0));
  };

  /**
   * @brief      Vector conjugate
   *
   * @param[in]  a     Vector value to conjugate
   *
   * @return     Conjugated value
   */
  __STATIC_FORCEINLINE float32x4_t vconjugate(const float32x4_t a)
  {
     return(a);
  };

  /**
   * @brief      Vector conjugate with tail
   *
   * @param[in]  a     Value
   * @param[in]  p0    Predicate
   *
   * @return     Conjugated value
   */
  __STATIC_FORCEINLINE float32x4_t vconjugate(const float32x4_t a,
                                        const mve_pred16_t p0)
  {
     (void)p0;
     return(a);
  };
  
  /**
   * @brief      Vector + Vector
   *
   * @param[in]  a     First operand
   * @param[in]  b     Second operand
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float32x4_t b)
  {
     return(vaddq(a,b));
  };

  /**
   * @brief      Vector + Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a + b
   */
  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float b)
  {
     return(vaddq_n_f32(a,b));
  };

  /**
   * @brief      Scalar + Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a + b
   */
   __STATIC_FORCEINLINE float32x4_t vadd(const float a,const float32x4_t b)
  {
     return(vaddq_n_f32(b,a));
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
  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float32x4_t b,
                                         const mve_pred16_t p0)
  {
     return(vaddq_x(a,b,p0));
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
  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float b,
                                        const mve_pred16_t p0)
  {
     return(vaddq_x_n_f32(a,b,p0));
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
  __STATIC_FORCEINLINE float32x4_t vadd(const float a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vaddq_x_n_f32(b,a,p0));
  };

  /**
   * @brief      Vector - Vector
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float32x4_t b)
  {
     return(vsubq(a,b));
  };

  /**
   * @brief      Vector - Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a - b
   */
  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float b)
  {
     return(vsubq_n_f32(a,b));
  };

  /**
   * @brief      Scalar - Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a - b
   */
   __STATIC_FORCEINLINE float32x4_t vsub(const float a,const float32x4_t b)
  {
     return(vsubq(vdupq_n_f32(a),b));
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
  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vsubq_x(a,b,p0));
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
  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float b,
                                        const mve_pred16_t p0)
  {
     return(vsubq_x_n_f32(a,b,p0));
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
   __STATIC_FORCEINLINE float32x4_t vsub(const float a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vsubq_x(vdupq_n_f32(a),b,p0));
  };
  
  /**
   * @brief      Vector * Vector
   *
   * @param[in]  a     Vector
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float32x4_t b)
  {
     return(vmulq(a,b));
  };

  /**
   * @brief      Vector * Scalar
   *
   * @param[in]  a     Vector
   * @param[in]  b     Scalar
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float b)
  {
     return(vmulq_n_f32(a,b));
  };

  /**
   * @brief      Scalar * Vector
   *
   * @param[in]  a     Scalar
   * @param[in]  b     Vector
   *
   * @return     a * b
   */
  __STATIC_FORCEINLINE float32x4_t vmul(const float a,const float32x4_t b)
  {
     return(vmulq_n_f32(b,a));
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
  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vmulq_x(a,b,p0));
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
  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float b,
                                        const mve_pred16_t p0)
  {
     return(vmulq_x_n_f32(a,b,p0));
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
  __STATIC_FORCEINLINE float32x4_t vmul(const float a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vmulq_x_n_f32(b,a,p0));
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
  __STATIC_FORCEINLINE float32x4_t vmacc(const float32x4_t acc,const float32x4_t a,const float32x4_t b)
  {
     return(vfmaq(acc,a,b));
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
  __STATIC_FORCEINLINE float32x4_t vmacc(const float32x4_t acc,const float32x4_t a,const float_t b)
  {
     return(vfmaq(acc,a,b));
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
  __STATIC_FORCEINLINE float32x4_t vmacc(const float32x4_t acc,const float32x4_t a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vfmaq_m(acc,a,b,p0));
  };



  /**
   * @brief      Vector reduce
   *
   * @param[in]  in    Vector
   *
   * @return     Reduced scalar value
   */
  __STATIC_FORCEINLINE float vreduce(const float32x4_t in)
  {
     float acc = vgetq_lane(in, 0) + vgetq_lane(in, 1) +
                 vgetq_lane(in, 2) + vgetq_lane(in, 3);
     return(acc);
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
  inline float32x4_t vload1(const float32_t *p)
  {
     return(vld1q(p));
  };

 
  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline float32x4_t vload1(const float32_t *p)
  {
     constexpr uint32x4_t offset={0*S,1*S,2*S,3*S};
     return(vldrwq_gather_shifted_offset_f32(p,offset));
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
  inline float32x4_t vload1(const float32_t *p,const index_t stride)
  {
     uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     offset = vmulq_n_u32(offset,stride);
     return(vldrwq_gather_shifted_offset_f32(p,offset));
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
  inline float32x4_t vload1_z(const float32_t *p,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     return(vld1q_z(p,p0));
  };


  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline float32x4_t vload1_z(const float32_t *p,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     //uint32x4_t offset={0,1,2,3};
     //uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     //offset = vmulq_n_u32(offset,S);
     constexpr uint32x4_t offset={0*S,1*S,2*S,3*S};
     return(vldrwq_gather_shifted_offset_z_f32(p,offset,p0));
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
  inline float32x4_t vload1_z(const float32_t *p,const index_t stride,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     //uint32x4_t offset={0,1,2,3};
     //uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     offset = vmulq_n_u32(offset,stride);
     return(vldrwq_gather_shifted_offset_z_f32(p,offset,p0));
  };
  
  /* Generalized stride */

  /**
   * @brief      Load with generalized stride (gather load)
   *
   * @tparam     S     List of offsets known at built time
   */
  template<int ...S>
  struct vload1_gen_stride<float32_t,S...>
  {
     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Gather load
      */
     static float32x4_t run(const float32_t *p)
     {
        constexpr uint32x4_t offset={S...};
        return(vldrwq_gather_shifted_offset_f32(p,offset));
     };
  };

  /**
   * @brief      Load with generalized stride specialized for <0,1,2,3>
   */
  template<>
  struct vload1_gen_stride<float32_t,0,1,2,3>
  {
     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Loaded vector
      */
     static float32x4_t run(const float32_t *p)
     {
        return(vld1q(p));
     };
  };

  /* Generalized stride */

  /**
   * @brief      Load with generalized stride (gather load) and tail predicate
   *
   * @tparam     S     List of offsets known at built time
   */
  template<int ...S>
  struct vload1_gen_stride_z<float32_t,S...>
  {
     /**
      * @brief      Load
      *
      * @param[in]  p     Load address
      * @param[in]  nb    Number of remaining samples in loop
      * @param[in]  p0    Predicate for remaining samples
      *
      * @return     Gather load with predicate
      */
     static float32x4_t run(const float32_t *p,const std::size_t nb,const mve_pred16_t p0)
     {
        constexpr uint32x4_t offset={S...};
        (void)nb;
        return(vldrwq_gather_shifted_offset_z_f32(p,offset,p0));
     };
  };

  /**
   * @brief      Load with generalized stride (gather load) and tail predicate specialized for <0,1,2,3>
   *
   * @tparam     S     List of offsets known at built time
   */
  template<>
  struct vload1_gen_stride_z<float32_t,0,1,2,3>
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
     static float32x4_t run(const float32_t *p,const std::size_t nb,const mve_pred16_t p0)
     {
        (void)nb;
        return(vld1q_z(p,p0));
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
  inline void vstore1(float32_t *p,const float32x4_t val)
  {
     vst1q(p,val);
  };

  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline void vstore1(float32_t *p,const float32x4_t val)
  {
     //uint32x4_t offset={0,1,2,3};
     //uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     //offset = vmulq_n_u32(offset,S);
     constexpr uint32x4_t offset={0*S,1*S,2*S,3*S};
     vstrwq_scatter_shifted_offset_f32(p,offset,val);
  };

  // with dynamic stride

  /**
   * @brief      Store with dynamic stride
   *
   * @param      p       Store address
   * @param[in]  stride  Stride value
   * @param[in]  val     Value to store
   */
  inline void vstore1(float32_t *p,const index_t stride,const float32x4_t val)
  {
     uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     offset = vmulq_n_u32(offset,stride);
     vstrwq_scatter_shifted_offset_f32(p,offset,val);
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
  inline void vstore1_z(float32_t *p,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     vstrwq_p(p,val,p0);
  };

  template<int S,
  typename std::enable_if<(S>1),bool>::type = true>
  inline void vstore1_z(float32_t *p,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     //uint32x4_t offset={0,1,2,3};
     //uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     //offset = vmulq_n_u32(offset,S);
     constexpr uint32x4_t offset={0*S,1*S,2*S,3*S};
     vstrwq_scatter_shifted_offset_p_f32(p,offset,val,p0);
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
  inline void vstore1_z(float32_t *p,const index_t stride,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     offset = vmulq_n_u32(offset,stride);
     vstrwq_scatter_shifted_offset_p_f32(p,offset,val,p0);
  };

  // Generalized stride

  /**
   * @brief      Generalized store with strides
   *
   * @tparam     S     Stride values known at built time
   */
  template<int ...S>
  struct vstore1_gen_stride<float32_t,S...>
  {
     /**
      * @brief      Scatter store
      *
      * @param      p     Store address
      * @param[in]  val   VAlue to store
      */
     static void run(float32_t *p,const float32x4_t val)
     {
        constexpr uint32x4_t offset={S...};
        vstrwq_scatter_shifted_offset_f32(p,offset,val);
     };
  };

  /**
   * @brief      Generalized store with stride (Specialized for <0,1,2,3>)
   */
  template<>
  struct vstore1_gen_stride<float32_t,0,1,2,3>
  {
      /**
       * @brief      Scatter store
       *
       * @param      p     Store address
       * @param[in]  val   Value to store
       */
      static void run(float32_t *p,const float32x4_t val)
     {
        vst1q(p,val);
     };
  };

  /**
   * @brief      Store with generalized strides and tail predicate
   *
   * @tparam     S     Strides values known at built time
   */
  template<int ...S>
  struct vstore1_gen_stride_z<float32_t,S...>
  {
      /**
       * @brief      Scatter store with tail predicate
       *
       * @param      p     Store address
       * @param[in]  val   Value to store
       * @param[in]  nb    Remaining number of loops
       * @param[in]  p0    Loop predicate
       */
      static void vstore1_z(float32_t *p,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
      {
        constexpr uint32x4_t offset={S...};
        (void)nb;
        vstrwq_scatter_shifted_offset_p_f32(p,offset,val,p0);
      }
  };

  /**
   * @brief      Scatter store with tail predicate (specialized for <0,1,2,3>)
   */
  template<>
  struct vstore1_gen_stride_z<float32_t,0,1,2,3>
  {
     /**
      * @brief      Scatter store with tail predicate
      *
      * @param      p     Store address
      * @param[in]  val   Value to store
      * @param[in]  nb    Number of remaining loops
      * @param[in]  p0    Loop predicate
      */
     static void vstore1_z(float32_t *p,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
     {
        (void)nb;
        vstrwq_p(p,val,p0);
     }

  };



};
/*! @} */

#endif

/*! @} */
/*! @} */