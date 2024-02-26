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
template<typename arch>
struct vector_traits<float,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type >
{
  typedef float type;
  typedef float storage_type;
  typedef float32x4_t vector;
  typedef float32x4_t temp_accumulator;
  typedef mve_pred16_t predicate_t;
  static constexpr bool has_vector = true;
  static constexpr bool is_float = true;
  static constexpr bool is_fixed = false;
  static constexpr bool has_predicate = true;

  static constexpr int nb_lanes = 4;

  static float32x4_t temp_acc_zero()
  {
     return(vdupq_n_f32(0.0f));
  }

  static constexpr float zero_lane() {return 0.0f;};
  // Useful in fixed point since lane value is an int and not a Q something
  static constexpr float lane_value(const float x) {return x;};
  
};



namespace inner {

  template<>
  struct vctpq<float> {
    static mve_pred16_t mk(uint32_t v)
    {
       return(vctp32q(v));
    };
  };
  
  __STATIC_FORCEINLINE float32x4_t vconst(const float v)
  {
     return(vdupq_n_f32(v));
  }

  __STATIC_FORCEINLINE float32x4_t vconst_tail(const float v,
                                               const mve_pred16_t p0)
  {
     return(vdupq_x_n_f32(v,p0));
  }

  __STATIC_FORCEINLINE float32x4_t vneg(const float32x4_t a)
  {
     return(vnegq(a));
  };

  __STATIC_FORCEINLINE float32x4_t vneg(const float32x4_t a,
                                        const mve_pred16_t p0)
  {
     return(vnegq_x(a,p0));
  };
  
  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float32x4_t b)
  {
     return(vaddq(a,b));
  };

 
  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float b)
  {
     return(vaddq_n_f32(a,b));
  };

   __STATIC_FORCEINLINE float32x4_t vadd(const float a,const float32x4_t b)
  {
     return(vaddq_n_f32(b,a));
  };

  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float32x4_t b,
                                         const mve_pred16_t p0)
  {
     return(vaddq_x(a,b,p0));
  };


  __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float b,
                                        const mve_pred16_t p0)
  {
     return(vaddq_x_n_f32(a,b,p0));
  };

  __STATIC_FORCEINLINE float32x4_t vadd(const float a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vaddq_x_n_f32(b,a,p0));
  };

  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float32x4_t b)
  {
     return(vsubq(a,b));
  };

  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float b)
  {
     return(vsubq_n_f32(a,b));
  };

   __STATIC_FORCEINLINE float32x4_t vsub(const float a,const float32x4_t b)
  {
     return(vsubq_n_f32(b,a));
  };

  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vsubq_x(a,b,p0));
  };

  __STATIC_FORCEINLINE float32x4_t vsub(const float32x4_t a,const float b,
                                        const mve_pred16_t p0)
  {
     return(vsubq_x_n_f32(a,b,p0));
  };

   __STATIC_FORCEINLINE float32x4_t vsub(const float a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vsubq_x_n_f32(b,a,p0));
  };
  
  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float32x4_t b)
  {
     return(vmulq(a,b));
  };

  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float b)
  {
     return(vmulq_n_f32(a,b));
  };

  __STATIC_FORCEINLINE float32x4_t vmul(const float a,const float32x4_t b)
  {
     return(vmulq_n_f32(b,a));
  };

  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vmulq_x(a,b,p0));
  };

  __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float b,
                                        const mve_pred16_t p0)
  {
     return(vmulq_x_n_f32(a,b,p0));
  };

  __STATIC_FORCEINLINE float32x4_t vmul(const float a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vmulq_x_n_f32(b,a,p0));
  };

  __STATIC_FORCEINLINE float32x4_t vmacc(const float32x4_t acc,const float32x4_t a,const float32x4_t b)
  {
     return(vfmaq(acc,a,b));
  };

  __STATIC_FORCEINLINE float32x4_t vmacc(const float32x4_t acc,const float32x4_t a,const float_t b)
  {
     return(vfmaq(acc,a,b));
  };

  __STATIC_FORCEINLINE float32x4_t vmacc(const float32x4_t acc,const float32x4_t a,const float32x4_t b,
                                        const mve_pred16_t p0)
  {
     return(vfmaq_m(acc,a,b,p0));
  };



  __STATIC_FORCEINLINE float vreduce(const float32x4_t in)
  {
     float acc = vgetq_lane(in, 0) + vgetq_lane(in, 1) +
                 vgetq_lane(in, 2) + vgetq_lane(in, 3);
     return(acc);
  };




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
  inline float32x4_t vload1(const float32_t *p,const index_t stride)
  {
     uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     offset = vmulq_n_u32(offset,stride);
     return(vldrwq_gather_shifted_offset_f32(p,offset));
  };

  
 
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
  template<int ...S>
  struct vload1_gen_stride
  {
     static float32x4_t run(const float32_t *p)
     {
        constexpr uint32x4_t offset={S...};
        return(vldrwq_gather_shifted_offset_f32(p,offset));
     };
  };

  template<>
  struct vload1_gen_stride<0,1,2,3>
  {
     inline float32x4_t run(const float32_t *p)
     {
        return(vld1q(p));
     };
  };

  /* Generalized stride */
  template<int ...S>
  struct vload1_gen_stride_z 
  {
     inline float32x4_t run(const float32_t *p,const std::size_t nb,const mve_pred16_t p0)
     {
        constexpr uint32x4_t offset={S...};
        (void)nb;
        return(vldrwq_gather_shifted_offset_z_f32(p,offset,p0));
     };
  };

  template<>
  struct vload1_gen_stride_z<0,1,2,3>
  {
     inline float32x4_t run(const float32_t *p,const std::size_t nb,const mve_pred16_t p0)
     {
        (void)nb;
        return(vld1q_z(p,p0));
     };
  };
  
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
  inline void vstore1(float32_t *p,const index_t stride,const float32x4_t val)
  {
     uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     offset = vmulq_n_u32(offset,stride);
     vstrwq_scatter_shifted_offset_f32(p,offset,val);
  };

  
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
  inline void vstore1_z(float32_t *p,const index_t stride,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
  {
     (void)nb;
     uint32x4_t offset = vidupq_u32((uint32_t)0,1);
     offset = vmulq_n_u32(offset,stride);
     vstrwq_scatter_shifted_offset_p_f32(p,offset,val,p0);
  };

  // Generalized stride
  template<int ...S>
  struct vstore1_gen_stride
  {
     static void run(float32_t *p,const float32x4_t val)
     {
        constexpr uint32x4_t offset={S...};
        vstrwq_scatter_shifted_offset_f32(p,offset,val);
     };
  };

  template<>
  struct vstore1_gen_stride<0,1,2,3>
  {
      static void run(float32_t *p,const float32x4_t val)
     {
        vst1q(p,val);
     };
  };

  template<int ...S>
  struct vstore1_gen_stride_z
  {
      static void vstore1_z(float32_t *p,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
      {
        constexpr uint32x4_t offset={S...};
        (void)nb;
        vstrwq_scatter_shifted_offset_p_f32(p,offset,val,p0);
      }
  };

  template<>
  struct vstore1_gen_stride_z<0,1,2,3>
  {
     static void vstore1_z(float32_t *p,const float32x4_t val,const std::size_t nb,const mve_pred16_t p0)
     {
        (void)nb;
        vstrwq_p(p,val,p0);
     }

  };

  

};

#endif

/*! @} */
/*! @} */