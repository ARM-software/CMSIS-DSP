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
 *  \addtogroup HeliumQ15Number Q15
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
struct vector_traits<Q15,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type > 
{
  typedef Q15 type;
  typedef type::value_type storage_type;
  typedef int16x8_t vector;
  typedef Q<33,30> temp_accumulator;
  typedef mve_pred16_t predicate_t;

  static constexpr bool has_vector = true;
  static constexpr bool is_float = false;
  static constexpr bool is_fixed = true;
  static constexpr bool has_predicate = true;

  static constexpr int nb_lanes = 8;


  static Q<33,30> temp_acc_zero()
  {
       return(Q<33,30>());
  }

  static constexpr int16_t zero_lane() {return 0;};

  static constexpr int16_t lane_value(const Q15 x) {return x.v;};

};

namespace inner {


    template<>
    struct vctpq<Q15>{
       static mve_pred16_t mk(uint32_t v)
       {
            return(vctp16q(v));
       };
    };

     __STATIC_FORCEINLINE int16x8_t vconst(Q15 val)
    {
       return(vdupq_n_s16(val.v));
    }

      __STATIC_FORCEINLINE int16x8_t vconst_tail(Q15 val,
                                                 const mve_pred16_t p0)
    {
       return(vdupq_x_n_s16(val.v,p0));
    }

    
    __STATIC_FORCEINLINE int16x8_t vneg(const int16x8_t a)
    {
       return(vqnegq(a));
    };

    __STATIC_FORCEINLINE int16x8_t vneg(const int16x8_t a,
                                        const mve_pred16_t p0)
    {
       return(vqnegq_m(vuninitializedq_s16(),a,p0));
    };

    __STATIC_FORCEINLINE int16x8_t vadd(const int16x8_t a,const int16x8_t b)
    {
       return(vqaddq(a,b));
    };

     __STATIC_FORCEINLINE int16x8_t vadd(const int16x8_t a,const Q15 b)
    {
       return(vqaddq_n_s16(a,b.v));
    };

     __STATIC_FORCEINLINE int16x8_t vadd(const Q15 a,const int16x8_t b)
    {
       return(vqaddq_n_s16(b,a.v));
    };


    __STATIC_FORCEINLINE int16x8_t vadd(const int16x8_t a,const int16x8_t b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m(vuninitializedq_s16(),a,b,p0));
    };

     __STATIC_FORCEINLINE int16x8_t vadd(const int16x8_t a,const Q15 b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m_n_s16(vuninitializedq_s16(),a,b.v,p0));
    };

     __STATIC_FORCEINLINE int16x8_t vadd(const Q15 a,const int16x8_t b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m_n_s16(vuninitializedq_s16(),b,a.v,p0));
    };

    __STATIC_FORCEINLINE int16x8_t vsub(const int16x8_t a,const int16x8_t b)
    {
       return(vqsubq(a,b));
    };

     __STATIC_FORCEINLINE int16x8_t vsub(const int16x8_t a,const Q15 b)
    {
       return(vqsubq_n_s16(a,b.v));
    };

     __STATIC_FORCEINLINE int16x8_t vsub(const Q15 a,const int16x8_t b)
    {
       return(vqsubq_n_s16(b,a.v));
    };

    __STATIC_FORCEINLINE int16x8_t vsub(const int16x8_t a,const int16x8_t b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m(vuninitializedq_s16(),a,b,p0));
    };

     __STATIC_FORCEINLINE int16x8_t vsub(const int16x8_t a,const Q15 b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m_n_s16(vuninitializedq_s16(),a,b.v,p0));
    };

     __STATIC_FORCEINLINE int16x8_t vsub(const Q15 a,const int16x8_t b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m_n_s16(vuninitializedq_s16(),b,a.v,p0));
    };
  
    __STATIC_FORCEINLINE int16x8_t vmul(const int16x8_t a,const int16x8_t b)
    {
        return(vqdmulhq(a,b));
    };


    __STATIC_FORCEINLINE int16x8_t vmul(const int16x8_t a,const Q15 b)
    {
        return(vqdmulhq_n_s16(a,b.v));
    };

    __STATIC_FORCEINLINE int16x8_t vmul(const Q15 a,const int16x8_t b)
    {
        return(vqdmulhq_n_s16(b,a.v));
    };

    __STATIC_FORCEINLINE int16x8_t vmul(const int16x8_t a,const int16x8_t b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m(vuninitializedq_s16(),a,b,p0));
    };


    __STATIC_FORCEINLINE int16x8_t vmul(const int16x8_t a,const Q15 b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m_n_s16(vuninitializedq_s16(),a,b.v,p0));
    };

    __STATIC_FORCEINLINE int16x8_t vmul(const Q15 a,const int16x8_t b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m_n_s16(vuninitializedq_s16(),b,a.v,p0));
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline int16x8_t vload1(const Q15 *p)
    {
       return(vld1q(reinterpret_cast<const int16_t*>(p)));
    };

    /*

    7*S must be <= 65535 so
    S <= 9362

    */
    template<int S,
    typename std::enable_if<(S>1) && (S<=9362),bool>::type = true>
    inline int16x8_t vload1(const Q15 *p)
    {
       constexpr uint16x8_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S};
       //uint16x8_t offset = vidupq_u16((uint16_t)0,1);
       //offset = vmulq_n_u16(offset,S);
       return(vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(p),offset));
    };

    template<int S,
    typename std::enable_if<(S>9362),bool>::type = true>
    inline int16x8_t vload1(const Q15 *p)
    {
       int16x8_t res;
       for(std::size_t i=0;i<8;i++)
       {
         res[i] = p->v;
         p += S;
       }
       
       return(res);
    };

    // Dynamic stride
    inline int16x8_t vload1(const Q15 *p,index_t stride)
    {
        if (stride <= 9362)
        {
            uint16x8_t offset = vidupq_u16((uint32_t)0,1);
            offset = vmulq_n_u16(offset,stride);
            return(vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(p),offset));
        }
        else
        {
            int16x8_t res;
            for(std::size_t i=0;i<8;i++)
            {
              res[i] = p->v;
              p += stride;
            }
            return(res);
        }
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline int16x8_t vload1_z(const Q15 *p,std::size_t nb,mve_pred16_t p0)
    {
        (void)nb;
        return(vld1q_z(reinterpret_cast<const int16_t*>(p),p0));
     
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=9362),bool>::type = true>
    inline int16x8_t vload1_z(const Q15 *p,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       uint16x8_t offset = vidupq_u16((uint32_t)0,1);
       offset = vmulq_n_u16(offset,S);
       return(vldrhq_gather_shifted_offset_z_s16(reinterpret_cast<const int16_t*>(p),offset,p0));
     };

    template<int S,
    typename std::enable_if<(S>9362),bool>::type = true>
    inline int16x8_t vload1_z(const Q15 *p,std::size_t nb,mve_pred16_t p0)
    {
        (void)p0;
        int16x8_t res;
        std::size_t i=0;
        for(;i<std::min(nb,8U);i++)
        {
         res[i] = p->v;
         p += S;
        }

        for(;i<8;i++)
        {
         res[i] = 0;
         p += S;
        }
       
        return(res);
     
    };

    // Dynamic stride
    inline int16x8_t vload1_z(const Q15 *p,index_t stride,std::size_t nb,mve_pred16_t p0)
    {

       if (stride <= 9362)
       {
         uint16x8_t offset = vidupq_u16((uint32_t)0,1);
         offset = vmulq_n_u16(offset,stride);
         return(vldrhq_gather_shifted_offset_z_s16(reinterpret_cast<const int16_t*>(p),offset,p0));
       }
       else 
       {
          int16x8_t res;
          std::size_t i=0;
          for(;i<std::min(nb,8U);i++)
          {
           res[i] = p->v;
           p += stride;
          }

          for(;i<8;i++)
          {
           res[i] = 0;
           p += stride;
          }
          return(res);
       }
     };

    
    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(Q15 *p,const int16x8_t val)
    {
       vst1q(reinterpret_cast<int16_t*>(p),val);
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=9362),bool>::type = true>
    inline void vstore1(Q15 *p,const int16x8_t val)
    {
       //uint16x8_t offset={0,1,2,3,4,5,6,7};
       //uint16x8_t offset = vidupq_u16((uint16_t)0,1);
       //offset = vmulq_n_u16(offset,S);
       constexpr uint16x8_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S};
       return(vstrhq_scatter_shifted_offset_s16(reinterpret_cast<int16_t*>(p),offset,val));
    };

    template<int S,
    typename std::enable_if<(S>9362),bool>::type = true>
    inline void vstore1(Q15 *p,const int16x8_t val)
    {
      for(std::size_t i=0;i<8;i++)
      {
        *p = Q15(val[i]);
         p += S;
      }
      
    };

    // dynamic stride 
    inline void vstore1(Q15 *p,const index_t stride,const int16x8_t val)
    {
        if (stride <=9362)
        {
            uint16x8_t offset = vidupq_u16((uint32_t)0,1);
            offset = vmulq_n_u16(offset,stride);
            return(vstrhq_scatter_shifted_offset_s16(reinterpret_cast<int16_t*>(p),offset,val));
        }
        else
        {
            for(std::size_t i=0;i<8;i++)
            {
              *p = Q15(val[i]);
               p += stride;
            }
        }
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1_z(Q15 *p,const int16x8_t val,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       return(vstrhq_p(reinterpret_cast<int16_t*>(p),val,p0));
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=9362),bool>::type = true>
    inline void vstore1_z(Q15 *p,const int16x8_t val,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       //uint16x8_t offset={0,1,2,3,4,5,6,7};
       //uint16x8_t offset = vidupq_u16((uint16_t)0,1);
       //offset = vmulq_n_u16(offset,S);
       constexpr uint16x8_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S};
       return(vstrhq_scatter_shifted_offset_p_s16(reinterpret_cast<int16_t*>(p),offset,val,p0));
    };
    
    
    template<int S,
    typename std::enable_if<(S>9362),bool>::type = true>
    inline void vstore1_z(Q15 *p,const int16x8_t val,std::size_t nb,mve_pred16_t p0)
    {
      (void)p0;
       for(std::size_t i=0;i<std::min(nb,8U);i++)
       {
           *p = Q15(val[i]);
           p += S;
       }
    };

    // dynamic stride
    inline void vstore1_z(Q15 *p,const index_t stride,const int16x8_t val,std::size_t nb,mve_pred16_t p0)
    {
       if (stride<=9362)
       {
         uint16x8_t offset = vidupq_u16((uint32_t)0,1);
         offset = vmulq_n_u16(offset,stride);
         return(vstrhq_scatter_shifted_offset_p_s16(reinterpret_cast<int16_t*>(p),offset,val,p0));
       }
       else 
       {
         for(std::size_t i=0;i<std::min(nb,8U);i++)
         {
             *p = Q15(val[i]);
             p += stride;
         }
       }
    };
   
    __STATIC_FORCEINLINE Q<33,30> vmacc(const Q<33,30> sum,
                                        const int16x8_t vala,
                                        const int16x8_t valb)
    {
       return(Q<33,30>(vmlaldavaq(sum.v,vala,valb)));
    };

    __STATIC_FORCEINLINE Q<33,30> vmacc(const Q<33,30> sum,
                                        const int16x8_t vala,
                                        const int16x8_t valb,
                                        const mve_pred16_t p0)
    {
       return(Q<33,30>(vmlaldavaq_p(sum.v,vala,valb,p0)));
    };

     __STATIC_FORCEINLINE Q<33,30> vmacc(const int16x8_t vala,
                                         const int16x8_t valb)
    {
       return(Q<33,30>(vmlaldavq(vala,valb)));
    };

    __STATIC_FORCEINLINE Q<33,30> vmacc(const int16x8_t vala,
                                        const int16x8_t valb,
                                        const mve_pred16_t p0)
    {
       return(Q<33,30>(vmlaldavq_p(vala,valb,p0)));
    };

    __STATIC_FORCEINLINE Q<33,30> vreduce(const Q<33,30> sum)
    {
       return(sum);
    };

};
#endif

/*! @} */
/*! @} */
