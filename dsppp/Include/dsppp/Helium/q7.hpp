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
 *  \addtogroup HeliumQ7Number Q7
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
struct vector_traits<Q7,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type > 
{
  typedef Q7 type;
  typedef type::value_type storage_type;
  typedef int8x16_t vector;
  typedef Q<17,14> temp_accumulator;
  typedef mve_pred16_t predicate_t;

  static constexpr bool has_vector = true;
  static constexpr bool is_float = false;
  static constexpr bool is_fixed = true;
  static constexpr bool has_predicate = true;

  static constexpr int nb_lanes = 16;


  static Q<17,14> temp_acc_zero()
  {
       return(Q<17,14>());
  }

  static constexpr int8_t zero_lane() {return 0;};

  static constexpr int8_t lane_value(const Q7 x) {return x.v;};

};

/**
 * Inner implementation of Helium intrinsics
 * \ingroup HeliumNumber
 */
namespace inner {


    template<>
    struct vctpq<Q7>{
       static mve_pred16_t mk(uint32_t v)
       {
            return(vctp8q(v));
       };
    };

     __STATIC_FORCEINLINE int8x16_t vconst(Q7 val)
    {
       return(vdupq_n_s8(val.v));
    }

      __STATIC_FORCEINLINE int8x16_t vconst_tail(Q7 val,
                                                 const mve_pred16_t p0)
    {
       return(vdupq_x_n_s8(val.v,p0));
    }

    
    __STATIC_FORCEINLINE int8x16_t vneg(const int8x16_t a)
    {
       return(vqnegq(a));
    };

    __STATIC_FORCEINLINE int8x16_t vneg(const int8x16_t a,
                                        const mve_pred16_t p0)
    {
       return(vqnegq_m(vuninitializedq_s8(),a,p0));
    };

    __STATIC_FORCEINLINE int8x16_t vadd(const int8x16_t a,const int8x16_t b)
    {
       return(vqaddq(a,b));
    };

     __STATIC_FORCEINLINE int8x16_t vadd(const int8x16_t a,const Q7 b)
    {
       return(vqaddq_n_s8(a,b.v));
    };

     __STATIC_FORCEINLINE int8x16_t vadd(const Q7 a,const int8x16_t b)
    {
       return(vqaddq_n_s8(b,a.v));
    };


    __STATIC_FORCEINLINE int8x16_t vadd(const int8x16_t a,const int8x16_t b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m(vuninitializedq_s8(),a,b,p0));
    };

     __STATIC_FORCEINLINE int8x16_t vadd(const int8x16_t a,const Q7 b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m_n_s8(vuninitializedq_s8(),a,b.v,p0));
    };

     __STATIC_FORCEINLINE int8x16_t vadd(const Q7 a,const int8x16_t b,
                                        const mve_pred16_t p0)
    {
       return(vqaddq_m_n_s8(vuninitializedq_s8(),b,a.v,p0));
    };

    __STATIC_FORCEINLINE int8x16_t vsub(const int8x16_t a,const int8x16_t b)
    {
       return(vqsubq(a,b));
    };

     __STATIC_FORCEINLINE int8x16_t vsub(const int8x16_t a,const Q7 b)
    {
       return(vqsubq_n_s8(a,b.v));
    };

     __STATIC_FORCEINLINE int8x16_t vsub(const Q7 a,const int8x16_t b)
    {
       return(vqsubq_n_s8(b,a.v));
    };

    __STATIC_FORCEINLINE int8x16_t vsub(const int8x16_t a,const int8x16_t b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m(vuninitializedq_s8(),a,b,p0));
    };

     __STATIC_FORCEINLINE int8x16_t vsub(const int8x16_t a,const Q7 b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m_n_s8(vuninitializedq_s8(),a,b.v,p0));
    };

     __STATIC_FORCEINLINE int8x16_t vsub(const Q7 a,const int8x16_t b,
                                        const mve_pred16_t p0)
    {
       return(vqsubq_m_n_s8(vuninitializedq_s8(),b,a.v,p0));
    };
  
    __STATIC_FORCEINLINE int8x16_t vmul(const int8x16_t a,const int8x16_t b)
    {
        return(vqdmulhq(a,b));
    };


    __STATIC_FORCEINLINE int8x16_t vmul(const int8x16_t a,const Q7 b)
    {
        return(vqdmulhq_n_s8(a,b.v));
    };

    __STATIC_FORCEINLINE int8x16_t vmul(const Q7 a,const int8x16_t b)
    {
        return(vqdmulhq_n_s8(b,a.v));
    };

    __STATIC_FORCEINLINE int8x16_t vmul(const int8x16_t a,const int8x16_t b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m(vuninitializedq_s8(),a,b,p0));
    };


    __STATIC_FORCEINLINE int8x16_t vmul(const int8x16_t a,const Q7 b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m_n_s8(vuninitializedq_s8(),a,b.v,p0));
    };

    __STATIC_FORCEINLINE int8x16_t vmul(const Q7 a,const int8x16_t b,
                                        const mve_pred16_t p0)
    {
        return(vqdmulhq_m_n_s8(vuninitializedq_s8(),b,a.v,p0));
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline int8x16_t vload1(const Q7 *p)
    {
       return(vld1q(reinterpret_cast<const int8_t*>(p)));
    };

    /*

    15*S <= 255 => S <= 17

    */
    template<int S,
    typename std::enable_if<(S>1) && (S<=17),bool>::type = true>
    inline int8x16_t vload1(const Q7 *p)
    {
       constexpr uint8x16_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S,
                                    8*S,9*S,10*S,11*S,12*S,13*S,14*S,15*S};
       //uint8x16_t offset = vidupq_u8((uint16_t)0,1);
       //offset = vmulq_n_u8(offset,S);
       return(vldrbq_gather_offset_s8(reinterpret_cast<const int8_t*>(p),offset));
    };

    template<int S,
    typename std::enable_if<(S>17),bool>::type = true>
    inline int8x16_t vload1(const Q7 *p)
    {
       int8x16_t res;
       for(std::size_t i=0;i<16;i++)
       {
         res[i] = p->v;
         p += S;
       }
       
       return(res);
    };

    // Dynamic stride
    inline int8x16_t vload1(const Q7 *p,index_t stride)
    {
        if (stride <= 17)
        {
            uint8x16_t offset = vidupq_u8((uint32_t)0,1);
            offset = vmulq_n_u8(offset,stride);
            return(vldrbq_gather_offset_s8(reinterpret_cast<const int8_t*>(p),offset));
        }
        else
        {
            int8x16_t res;
            for(std::size_t i=0;i<16;i++)
            {
              res[i] = p->v;
              p += stride;
            }
            return(res);
        }
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline int8x16_t vload1_z(const Q7 *p,std::size_t nb,mve_pred16_t p0)
    {
        (void)nb;
        return(vld1q_z(reinterpret_cast<const int8_t*>(p),p0));
     
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=17),bool>::type = true>
    inline int8x16_t vload1_z(const Q7 *p,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       uint8x16_t offset = vidupq_u8((uint32_t)0,1);
       offset = vmulq_n_u8(offset,S);
       return(vldrbq_gather_offset_z_s8(reinterpret_cast<const int8_t*>(p),offset,p0));
     };

    template<int S,
    typename std::enable_if<(S>17),bool>::type = true>
    inline int8x16_t vload1_z(const Q7 *p,std::size_t nb,mve_pred16_t p0)
    {
        (void)p0;
        int8x16_t res;
        std::size_t i=0;
        for(;i<std::min(nb,16U);i++)
        {
         res[i] = p->v;
         p += S;
        }

        for(;i<16;i++)
        {
         res[i] = 0;
         p += S;
        }
       
        return(res);
     
    };

    // Dynamic stride
    inline int8x16_t vload1_z(const Q7 *p,index_t stride,std::size_t nb,mve_pred16_t p0)
    {

       if (stride <= 17)
       {
         uint8x16_t offset = vidupq_u8((uint32_t)0,1);
         offset = vmulq_n_u8(offset,stride);
         return(vldrbq_gather_offset_z_s8(reinterpret_cast<const int8_t*>(p),offset,p0));
       }
       else 
       {
          int8x16_t res;
          std::size_t i=0;
          for(;i<std::min(nb,16U);i++)
          {
           res[i] = p->v;
           p += stride;
          }

          for(;i<16;i++)
          {
           res[i] = 0;
           p += stride;
          }
          return(res);
       }
     };

    
    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(Q7 *p,const int8x16_t val)
    {
       vst1q(reinterpret_cast<int8_t*>(p),val);
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=17),bool>::type = true>
    inline void vstore1(Q7 *p,const int8x16_t val)
    {
       //uint8x16_t offset={0,1,2,3,4,5,6,7};
       //uint8x16_t offset = vidupq_u8((uint16_t)0,1);
       //offset = vmulq_n_u8(offset,S);
       constexpr uint8x16_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S,
                                    8*S,9*S,10*S,11*S,12*S,13*S,14*S,15*S};
       return(vstrbq_scatter_offset_s8(reinterpret_cast<int8_t*>(p),offset,val));
    };

    template<int S,
    typename std::enable_if<(S>17),bool>::type = true>
    inline void vstore1(Q7 *p,const int8x16_t val)
    {
      for(std::size_t i=0;i<16;i++)
      {
        *p = Q7(val[i]);
         p += S;
      }
      
    };

    // dynamic stride 
    inline void vstore1(Q7 *p,const index_t stride,const int8x16_t val)
    {
        if (stride <=17)
        {
            uint8x16_t offset = vidupq_u8((uint32_t)0,1);
            offset = vmulq_n_u8(offset,stride);
            return(vstrbq_scatter_offset_s8(reinterpret_cast<int8_t*>(p),offset,val));
        }
        else
        {
            for(std::size_t i=0;i<16;i++)
            {
              *p = Q7(val[i]);
               p += stride;
            }
        }
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1_z(Q7 *p,const int8x16_t val,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       return(vstrbq_p(reinterpret_cast<int8_t*>(p),val,p0));
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=17),bool>::type = true>
    inline void vstore1_z(Q7 *p,const int8x16_t val,std::size_t nb,mve_pred16_t p0)
    {
       (void)nb;
       //uint8x16_t offset={0,1,2,3,4,5,6,7};
       //uint8x16_t offset = vidupq_u8((uint16_t)0,1);
       //offset = vmulq_n_u8(offset,S);
       constexpr uint8x16_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S,
                                    8*S,9*S,10*S,11*S,12*S,13*S,14*S,15*S};
       return(vstrbq_scatter_offset_p_s8(reinterpret_cast<int8_t*>(p),offset,val,p0));
    };
    
    
    template<int S,
    typename std::enable_if<(S>17),bool>::type = true>
    inline void vstore1_z(Q7 *p,const int8x16_t val,std::size_t nb,mve_pred16_t p0)
    {
      (void)p0;
       for(std::size_t i=0;i<std::min(nb,16U);i++)
       {
           *p = Q7(val[i]);
           p += S;
       }
    };

    // dynamic stride
    inline void vstore1_z(Q7 *p,const index_t stride,const int8x16_t val,std::size_t nb,mve_pred16_t p0)
    {
       if (stride<=17)
       {
         uint8x16_t offset = vidupq_u8((uint32_t)0,1);
         offset = vmulq_n_u8(offset,stride);
         return(vstrbq_scatter_offset_p_s8(reinterpret_cast<int8_t*>(p),offset,val,p0));
       }
       else 
       {
         for(std::size_t i=0;i<std::min(nb,16U);i++)
         {
             *p = Q7(val[i]);
             p += stride;
         }
       }
    };
   
    __STATIC_FORCEINLINE Q<17,14> vmacc(const Q<17,14> sum,
                                        const int8x16_t vala,
                                        const int8x16_t valb)
    {
       return(Q<17,14>(vmladavaq(sum.v,vala,valb)));
    };

    __STATIC_FORCEINLINE Q<17,14> vmacc(const Q<17,14> sum,
                                        const int8x16_t vala,
                                        const int8x16_t valb,
                                        const mve_pred16_t p0)
    {
       return(Q<17,14>(vmladavaq_p(sum.v,vala,valb,p0)));
    };

     __STATIC_FORCEINLINE Q<17,14> vmacc(const int8x16_t vala,
                                         const int8x16_t valb)
    {
       return(Q<17,14>(vmladavq(vala,valb)));
    };

    __STATIC_FORCEINLINE Q<17,14> vmacc(const int8x16_t vala,
                                        const int8x16_t valb,
                                        const mve_pred16_t p0)
    {
       return(Q<17,14>(vmladavq_p(vala,valb,p0)));
    };

    __STATIC_FORCEINLINE Q<17,14> vreduce(const Q<17,14> sum)
    {
       return(sum);
    };

};
#endif

/*! @} */
/*! @} */
