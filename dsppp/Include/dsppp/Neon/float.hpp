// -*- C++ -*-

#pragma once 

/******************
 *
 * Neon
 * 
 */
#if defined(ARM_MATH_NEON)

template<typename arch>
struct vector_traits<float,arch,
typename std::enable_if<std::is_base_of<Neon,arch>::value>::type> 
{
  typedef float type;
  typedef float storage_type;
  typedef float32x4_t vector;
  static constexpr bool has_vector = true;
  static constexpr bool is_float = true;
  static constexpr bool is_fixed = false;

  static constexpr int nb_lanes = 4;

   static constexpr float zero_lane() {return 0.0f;};


};

namespace inner {

  
   
   __STATIC_FORCEINLINE float32x4_t vadd(const float32x4_t a,const float32x4_t b)
   {
      return(vaddq_f32(a,b));
   };
   
   __STATIC_FORCEINLINE float32x4_t vmul(const float32x4_t a,const float32x4_t b)
   {
      return(vmulqq_f32(a,b));
   };

   __STATIC_FORCEINLINE float32x4_t vconst(const float v)
   {
      const float32x4_t t = vdupq_n_f32(v)
      return(t);
   }

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
       float32x4_t res;
       res[0] = *p;
       p += S;
    
       res[1] = *p;
       p += S;
    
       res[2] = *p;
       p += S;
    
       res[3] = *p;
       p += S;
    
       return(res);
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(float32_t *p,const float32x4_t val)
    {
       return(vst1q(p,val));
    };
    
    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline void vstore1(float32_t *p,const float32x4_t val)
    {
       *p = val[0];
       p += S;
    
       *p = val[1];
       p += S;
    
       *p = val[2];
       p += S;
    
       *p = val[3];
       p += S;
    };
  
  

};

#endif
