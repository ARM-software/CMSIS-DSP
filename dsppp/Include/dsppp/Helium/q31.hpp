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
 *  \addtogroup HeliumQ31Number Q31
 *  \ingroup HeliumNumber
 *  @{
 */


/******************
 *
 * Helium
 * 
 */
#if defined(ARM_MATH_MVEI)



template<typename arch>
struct vector_traits<Q31,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type > 
{
  typedef Q31 type;
  typedef type::value_type storage_type;
  typedef int32x4_t vector;
  typedef Q<9,54> temp_accumulator;
  typedef mve_pred16_t predicate_t;

  static constexpr bool has_vector = true;
  static constexpr bool is_float = false;
  static constexpr bool is_fixed = true;
  static constexpr bool has_predicate = true;

  static constexpr int nb_lanes = 4;


  static Q<9,54> temp_acc_zero()
  {
       return(Q<9,54>());
  }

  static constexpr int16_t zero_lane() {return 0;};

  static constexpr int16_t lane_value(const Q31 x) {return x.v;};

};

/**
 * Inner implementation of Helium intrinsics
 * \ingroup HeliumNumber
 */
namespace inner {

    template<>
    struct vctpq<Q31>{
       static mve_pred16_t mk(uint32_t v)
       {
            return(vctp32q(v));
       };
    };

     __STATIC_FORCEINLINE int32x4_t vconst(Q31 val)
    {
       return(vdupq_n_s32(val.v));
    }

      __STATIC_FORCEINLINE int32x4_t vconst_tail(Q31 val,
                                                 const mve_pred16_t p0)
    {
       return(vdupq_x_n_s32(val.v,p0));
    }

    
    __STATIC_FORCEINLINE int32x4_t vneg(const int32x4_t a)
    {
       return(vqnegq(a));
    };

    __STATIC_FORCEINLINE int32x4_t vneg(const int32x4_t a,
                                        const mve_pred16_t p0)
    {
       return(vqnegq_m(vuninitializedq_s32(),a,p0));
    };

    __STATIC_FORCEINLINE int32x4_t vadd(const int32x4_t a,const int32x4_t b)
    {
       return(vqaddq(a,b));
    };

     __STATIC_FORCEINLINE int32x4_t vadd(const int32x4_t a,const Q31 b)
    {
       return(vqaddq_n_s32(a,b.v));
    };

     __STATIC_FORCEINLINE int32x4_t vadd(const Q31 a,const int32x4_t b)
    {
       return(vqaddq_n_s32(b,a.v));
    };


    __STATIC_FORCEINLINE int32x4_t vadd(const int32x4_t a,const int32x4_t b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m(vuninitializedq_s32(),a,b,p0));
    };

     __STATIC_FORCEINLINE int32x4_t vadd(const int32x4_t a,const Q31 b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m_n_s32(vuninitializedq_s32(),a,b.v,p0));
    };

     __STATIC_FORCEINLINE int32x4_t vadd(const Q31 a,const int32x4_t b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m_n_s32(vuninitializedq_s32(),b,a.v,p0));
    };

    __STATIC_FORCEINLINE int32x4_t vsub(const int32x4_t a,const int32x4_t b)
    {
       return(vqsubq(a,b));
    };

     __STATIC_FORCEINLINE int32x4_t vsub(const int32x4_t a,const Q31 b)
    {
       return(vqsubq_n_s32(a,b.v));
    };

     __STATIC_FORCEINLINE int32x4_t vsub(const Q31 a,const int32x4_t b)
    {
       return(vqsubq_n_s32(b,a.v));
    };

    __STATIC_FORCEINLINE int32x4_t vsub(const int32x4_t a,const int32x4_t b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m(vuninitializedq_s32(),a,b,p0));
    };

     __STATIC_FORCEINLINE int32x4_t vsub(const int32x4_t a,const Q31 b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m_n_s32(vuninitializedq_s32(),a,b.v,p0));
    };

     __STATIC_FORCEINLINE int32x4_t vsub(const Q31 a,const int32x4_t b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m_n_s32(vuninitializedq_s32(),b,a.v,p0));
    };
  
    __STATIC_FORCEINLINE int32x4_t vmul(const int32x4_t a,const int32x4_t b)
    {
        return(vqdmulhq(a,b));
    };


    __STATIC_FORCEINLINE int32x4_t vmul(const int32x4_t a,const Q31 b)
    {
        return(vqdmulhq_n_s32(a,b.v));
    };

    __STATIC_FORCEINLINE int32x4_t vmul(const Q31 a,const int32x4_t b)
    {
        return(vqdmulhq_n_s32(b,a.v));
    };

    __STATIC_FORCEINLINE int32x4_t vmul(const int32x4_t a,const int32x4_t b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m(vuninitializedq_s32(),a,b,p0));
    };


    __STATIC_FORCEINLINE int32x4_t vmul(const int32x4_t a,const Q31 b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m_n_s32(vuninitializedq_s32(),a,b.v,p0));
    };

    __STATIC_FORCEINLINE int32x4_t vmul(const Q31 a,const int32x4_t b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m_n_s32(vuninitializedq_s32(),b,a.v,p0));
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline int32x4_t vload1(const Q31 *p)
    {
       return(vld1q(reinterpret_cast<const int32_t*>(p)));
    };

    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline int32x4_t vload1(const Q31 *p)
    {
       constexpr uint32x4_t offset={0*S,1*S,2*S,3*S};
       return(vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(p),offset));
    };


    // Dynamic stride
    inline int32x4_t vload1(const Q31 *p,index_t stride)
    {
        uint32x4_t offset = vidupq_u32((uint32_t)0,1);
        offset = vmulq_n_u32(offset,stride);
        return(vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(p),offset));
        
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline int32x4_t vload1_z(const Q31 *p,std::size_t nb,mve_pred16_t p0)
    {
        (void)nb;
        return(vld1q_z(reinterpret_cast<const int32_t*>(p),p0));
     
    };

    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline int32x4_t vload1_z(const Q31 *p,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       uint32x4_t offset = vidupq_u32((uint32_t)0,1);
       offset = vmulq_n_u32(offset,S);
       return(vldrwq_gather_shifted_offset_z_s32(reinterpret_cast<const int32_t*>(p),offset,p0));
     };

    

    // Dynamic stride
    inline int32x4_t vload1_z(const Q31 *p,index_t stride,std::size_t nb,mve_pred16_t p0)
    {
        (void)nb;
        uint32x4_t offset = vidupq_u32((uint32_t)0,1);
        offset = vmulq_n_u32(offset,stride);
        return(vldrwq_gather_shifted_offset_z_s32(reinterpret_cast<const int32_t*>(p),offset,p0));

     };

    
    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(Q31 *p,const int32x4_t val)
    {
       vst1q(reinterpret_cast<int32_t*>(p),val);
    };

    template<int S,
    typename std::enable_if<(S>1) ,bool>::type = true>
    inline void vstore1(Q31 *p,const int32x4_t val)
    {

       constexpr uint32x4_t offset={0*S,1*S,2*S,3*S};
       return(vstrwq_scatter_shifted_offset_s32(reinterpret_cast<int32_t*>(p),offset,val));
    };

   

    // dynamic stride 
    inline void vstore1(Q31 *p,const index_t stride,const int32x4_t val)
    {
        
        uint32x4_t offset = vidupq_u32((uint32_t)0,1);
        offset = vmulq_n_u32(offset,stride);
        return(vstrwq_scatter_shifted_offset_s32(reinterpret_cast<int32_t*>(p),offset,val));
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1_z(Q31 *p,const int32x4_t val,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       return(vstrwq_p(reinterpret_cast<int32_t*>(p),val,p0));
    };

    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline void vstore1_z(Q31 *p,const int32x4_t val,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;

       constexpr uint32x4_t offset={0*S,1*S,2*S,3*S};
       vstrwq_scatter_shifted_offset_p_s32(reinterpret_cast<int32_t*>(p),offset,val,p0);
    };
    
   

    // dynamic stride
    inline void vstore1_z(Q31 *p,const index_t stride,const int32x4_t val,std::size_t nb,mve_pred16_t p0)
    {
         (void)nb;
         uint32x4_t offset = vidupq_u32((uint32_t)0,1);
         offset = vmulq_n_u32(offset,stride);
         vstrwq_scatter_shifted_offset_p_s32(reinterpret_cast<int32_t*>(p),offset,val,p0);

    };
   
    __STATIC_FORCEINLINE Q<9,54> vmacc(const Q<9,54> sum,
                                        const int32x4_t vala,
                                        const int32x4_t valb)
    {
       return(Q<9,54>(vrmlaldavhaq(sum.v,vala,valb)));
    };

    __STATIC_FORCEINLINE Q<9,54> vmacc(const Q<9,54> sum,
                                        const int32x4_t vala,
                                        const int32x4_t valb,
                                        const mve_pred16_t p0)
    {
       return(Q<9,54>(vrmlaldavhaq_p(sum.v,vala,valb,p0)));
    };

     __STATIC_FORCEINLINE Q<9,54> vmacc(const int32x4_t vala,
                                         const int32x4_t valb)
    {
       return(Q<9,54>(vrmlaldavhq(vala,valb)));
    };

    __STATIC_FORCEINLINE Q<9,54> vmacc(const int32x4_t vala,
                                        const int32x4_t valb,
                                        const mve_pred16_t p0)
    {
       return(Q<9,54>(vrmlaldavhq_p(vala,valb,p0)));
    };

    __STATIC_FORCEINLINE Q<15,48> vreduce(const Q<9,54> sum)
    {
       return(Q<15,48>(asrl(sum.v, 6)));
    };

};

#endif

/*! @} */
/*! @} */
