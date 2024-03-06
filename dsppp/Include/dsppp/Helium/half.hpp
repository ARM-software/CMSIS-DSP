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
 *  \addtogroup HeliumHalfNumber Half
 *  \ingroup HeliumNumber
 *  @{
 */

#if defined(ARM_MATH_MVE_FLOAT16)

/**
 * @brief      Vector features for f16 on Helium
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<float16_t,arch,typename std::enable_if<std::is_base_of<Helium,arch>::value>::type> 
{
  //! Scalar datatype
  typedef float16_t type;
  //! Storage datatype
  typedef float16_t storage_type;
  //! Vector datatype
  typedef float16x8_t vector;
  //! Temp accumulator datatype
  typedef float16x8_t temp_accumulator;
  //! Predicate datatype
  typedef mve_pred16_t predicate_t;

  //! Has vector instructions
  static constexpr bool has_vector = true;
  //! Is float
  static constexpr bool is_float = true;
  //! Is not fixed point
  static constexpr bool is_fixed = false;
  //! Has predicated loop
  static constexpr bool has_predicate = true;

  //! Number of lanes
  static constexpr int nb_lanes = 8;

  /**
   * @brief      Vector of 0
   *
   * @return     Vector of 0
   */
  static float16x8_t temp_acc_zero()
  {
     return(vdupq_n_f16(0.0f));
  }

  /**
   * @brief      Value to write 0 in a lane
   *
   * @return     Value to write 0 in a lane
   * 
   * f16 suffix not supported in C++ 
   */
  static constexpr float16_t zero_lane() {return 0.0f;};
  // Useful in fixed point since lane value is an int and not a Q something

  /**
   * @brief      Convert from lane value
   *
   * @param[in]  x     Lane value
   *
   * @return     Lane value with current datatype
   */
  static constexpr float16_t lane_value(const float16_t x) {return x;};

};


/**
 * \ingroup HeliumNumber
 */
namespace inner {


   /**
    * @brief      vctpq for Helium and f16
    */
   template<>
   struct vctpq<float16_t>{
       /**
        * @brief      Make predicate
        *
        * @param[in]  v     Remaining iterations
        *
        * @return     Predicate
        */
       static mve_pred16_t mk(uint32_t v)
       
       {
            return(vctp16q(v));
       };
   };
   
   /**
    * @brief      Vector const
    *
    * @param[in]  v     Initialization value
    *
    * @return     Vector of const
    */
   __STATIC_FORCEINLINE float16x8_t vconst(float16_t v)
   {
      return(vdupq_n_f16(v));
   }

   /**
    * @brief      Vector of const with tail predicate
    *
    * @param[in]  v     The initialization parameter
    * @param[in]  p0    The predicate
    *
    * @return     The initialized vector with const and predicate
    */
   __STATIC_FORCEINLINE float16x8_t vconst_tail(const float16_t v,
                                                const mve_pred16_t p0)
  {
     return(vdupq_x_n_f16(v,p0));
  }

  /**
   * @brief      Vector negate
   *
   * @param[in]  a     Vector
   *
   * @return     Negate of vector
   */
   __STATIC_FORCEINLINE float16x8_t vneg(const float16x8_t a)
  {
     return(vnegq(a));
  };

  /**
   * @brief      Vector negate with tail predicate
   *
   * @param[in]  a     Vector
   * @param[in]  p0    Predicate
   *
   * @return     Negate of vector with tail predicate
   */
  __STATIC_FORCEINLINE float16x8_t vneg(const float16x8_t a,
                                        const mve_pred16_t p0)
  {
     return(vnegq_x(a,p0));
  };
   
   /*

   ADD

   */

   /**
    * @brief      Vector + Vector
    *
    * @param[in]  a     Vector
    * @param[in]  b     Vector
    *
    * @return     a + b
    */
   __STATIC_FORCEINLINE float16x8_t vadd(const float16x8_t a,
                                         const float16x8_t b)
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
   __STATIC_FORCEINLINE float16x8_t vadd(const float16x8_t a,
                                         const float16_t b)
   {
      return(vaddq_n_f16(a,b));
   };

   /**
    * @brief      Scalar + Vector
    *
    * @param[in]  a     Scalar
    * @param[in]  b     Vector
    *
    * @return     a + b
    */
   __STATIC_FORCEINLINE float16x8_t vadd(const float16_t a,
                                         const float16x8_t b)
   {
      return(vaddq_n_f16(b,a));
   };

   /**
    * @brief      Vector + Vector with tail predicate 
    *
    * @param[in]  a     Vector
    * @param[in]  b     Vector
    * @param[in]  p0    predicate
    *
    * @return     a + b with tail predicate
    */
   __STATIC_FORCEINLINE float16x8_t vadd(const float16x8_t a,
                                         const float16x8_t b,
                                         const mve_pred16_t p0)
   {
     return(vaddq_x(a,b,p0));
   };

   /**
    * @brief      Vector + Scalar with tail predicate
    *
    * @param[in]  a     Vector
    * @param[in]  b     Scalar
    * @param[in]  p0    Predicate
    *
    * @return     a + b with tail predicate
    */
   __STATIC_FORCEINLINE float16x8_t vadd(const float16x8_t a,
                                         const float16_t b,
                                         const mve_pred16_t p0)
   {
     return(vaddq_x_n_f16(a,b,p0));
   };

   /**
    * @brief      Scalar + Vector with tail predicate
    *
    * @param[in]  a     Scalar
    * @param[in]  b     Vector
    * @param[in]  p0    Predicate
    *
    * @return     a + b with tail predicate
    */
   __STATIC_FORCEINLINE float16x8_t vadd(const float16_t a,
                                         const float16x8_t b,
                                         const mve_pred16_t p0)
   {
     return(vaddq_x_n_f16(b,a,p0));
   };

   /*

   SUB

   */

   __STATIC_FORCEINLINE float16x8_t vsub(const float16x8_t a,
                                         const float16x8_t b)
   {
      return(vsubq(a,b));
   };

   __STATIC_FORCEINLINE float16x8_t vsub(const float16x8_t a,
                                         const float16_t b)
   {
      return(vsubq_n_f16(a,b));
   };

   __STATIC_FORCEINLINE float16x8_t vsub(const float16_t a,
                                         const float16x8_t b)
   {
      return(vsubq_n_f16(b,a));
   };

   __STATIC_FORCEINLINE float16x8_t vsub(const float16x8_t a,
                                         const float16x8_t b,
                                         const mve_pred16_t p0)
   {
     return(vsubq_x(a,b,p0));
   };

   __STATIC_FORCEINLINE float16x8_t vsub(const float16x8_t a,
                                         const float16_t b,
                                         const mve_pred16_t p0)
   {
     return(vsubq_x_n_f16(a,b,p0));
   };

   __STATIC_FORCEINLINE float16x8_t vsub(const float16_t a,
                                         const float16x8_t b,
                                         const mve_pred16_t p0)
   {
     return(vsubq_x_n_f16(b,a,p0));
   };

   /*

   MUL

   */
   
   __STATIC_FORCEINLINE float16x8_t vmul(const float16x8_t a,
                                         const float16x8_t b)
   {
      return(vmulq(a,b));
   };

   __STATIC_FORCEINLINE float16x8_t vmul(const float16x8_t a,
                                         const float16_t b)
   {
      return(vmulq_n_f16(a,b));
   };

   __STATIC_FORCEINLINE float16x8_t vmul(const float16_t a,
                                         const float16x8_t b)
   {
      return(vmulq_n_f16(b,a));
   };

   __STATIC_FORCEINLINE float16x8_t vmul(const float16x8_t a,
                                         const float16x8_t b,
                                         const mve_pred16_t p0)
   {
     return(vmulq_x(a,b,p0));
   };

   __STATIC_FORCEINLINE float16x8_t vmul(const float16x8_t a,
                                         const float16_t b,
                                         const mve_pred16_t p0)
   {
     return(vmulq_x_n_f16(a,b,p0));
   };

   __STATIC_FORCEINLINE float16x8_t vmul(const float16_t a,
                                         const float16x8_t b,
                                         const mve_pred16_t p0)
   {
     return(vmulq_x_n_f16(b,a,p0));
   };

   /*

   vmacc

   */

   __STATIC_FORCEINLINE float16x8_t vmacc(const float16x8_t acc,
                                          const float16x8_t a,
                                          const float16x8_t b)
  {
     return(vfmaq(acc,a,b));
  };

  __STATIC_FORCEINLINE float16x8_t vmacc(const float16x8_t acc,
                                         const float16x8_t a,
                                         const float16_t b)
  {
     return(vfmaq(acc,a,b));
  };

  __STATIC_FORCEINLINE float16x8_t vmacc(const float16x8_t acc,
                                         const float16x8_t a,
                                         const float16x8_t b,
                                         const mve_pred16_t p0)
  {
     return(vfmaq_m(acc,a,b,p0));
  };



  __STATIC_FORCEINLINE float16_t vreduce(float16x8_t in)
  {
     float16x8_t tmpVec;
    _Float16 acc;

    tmpVec = (float16x8_t) vrev32q_s16((int16x8_t) in);
    in = vaddq_f16(tmpVec, in);
    tmpVec = (float16x8_t) vrev64q_s32((int32x4_t) in);
    in = vaddq_f16(tmpVec, in);
    acc = (_Float16)vgetq_lane_f16(in, 0) + (_Float16)vgetq_lane_f16(in, 4);

    return acc;
  };

  /*

  Load

  */

  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline float16x8_t vload1(const float16_t *p)
  {
     return(vld1q(p));
  };

  template<int S,
  typename std::enable_if<(S>1) && (S<=65535),bool>::type = true>
  inline float16x8_t vload1(const float16_t *p)
  {
     constexpr uint16x8_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S};
     return(vldrhq_gather_shifted_offset_f16(p,offset));
  };

  template<int S,
  typename std::enable_if<(S>65535),bool>::type = true>
  inline float16x8_t vload1(const float16_t *p)
    {
       float16x8_t res;
       for(std::size_t i=0;i<8;i++)
       {
         res[i] = *p;
         p += S;
       }
       
       return(res);
    };

  // With dynamic stride
  inline float16x8_t vload1(const float16_t *p,const index_t stride)
  {
     if (stride <= 65535)
     {
       uint16x8_t offset = vidupq_u16((uint32_t)0,1);
       offset = vmulq_n_u16(offset,stride);
       return(vldrhq_gather_shifted_offset_f16(p,offset));
     }
     else 
     {
        float16x8_t res;
        for(std::size_t i=0;i<8;i++)
        {
              res[i] = *p;
              p += stride;
        }
        return(res);
     }
  };

  
 
  template<int S,
  typename std::enable_if<S==1,bool>::type = true>
  inline float16x8_t vload1_z(const float16_t *p,
                              const std::size_t nb,
                              const mve_pred16_t p0)
  {
     (void)nb;
     return(vld1q_z(p,p0));
  };

  template<int S,
  typename std::enable_if<(S>1)&& (S<=65535),bool>::type = true>
  inline float16x8_t vload1_z(const float16_t *p,
                              const std::size_t nb,
                              const mve_pred16_t p0)
  {
     (void)nb;
     constexpr uint16x8_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S};
     return(vldrhq_gather_shifted_offset_z_f16(p,offset,p0));
  };

    template<int S,
    typename std::enable_if<(S>65535),bool>::type = true>
    inline float16x8_t vload1_z(const float16_t *p,std::size_t nb,mve_pred16_t p0)
    {
        (void)p0;
        float16x8_t res;
        std::size_t i=0;
        for(;i<std::min(nb,8U);i++)
        {
         res[i] = *p;
         p += S;
        }

        for(;i<8;i++)
        {
         res[i] = 0;
         p += S;
        }
       
        return(res);
     
    };

  // With dynamic stride
  inline float16x8_t vload1_z(const float16_t *p,
                              const index_t stride,
                              const std::size_t nb,
                              const mve_pred16_t p0)
  {
     (void)nb;
     if (stride <= 65535)
     {
       uint16x8_t offset = vidupq_u16((uint32_t)0,1);
       offset = vmulq_n_u16(offset,stride);
       return(vldrhq_gather_shifted_offset_z_f16(p,offset,p0));
     }
     else 
     {
        (void)p0;
        float16x8_t res;
        std::size_t i=0;
        for(;i<std::min(nb,8U);i++)
        {
         res[i] = *p;
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
   

   /*

   Store

   */

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(float16_t *p,const float16x8_t val)
    {
       vst1q(p,val);
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=65535),bool>::type = true>
    inline void vstore1(float16_t *p,const float16x8_t val)
    {
       constexpr uint16x8_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S};
       vstrhq_scatter_shifted_offset_f16(p,offset,val);
    };

    template<int S,
    typename std::enable_if<(S>65535),bool>::type = true>
    inline void vstore1(float16_t *p,const float16x8_t val)
    {
      for(std::size_t i=0;i<8;i++)
      {
        *p = val[i];
         p += S;
      }
      
    };

    // dynamic stride 
    inline void vstore1(float16_t *p,
                        const index_t stride,
                        const float16x8_t val)
    {
        if (stride <=65535)
        {
            uint16x8_t offset = vidupq_u16((uint32_t)0,1);
            offset = vmulq_n_u16(offset,stride);
            vstrhq_scatter_shifted_offset_f16(p,offset,val);
        }
        else
        {
            for(std::size_t i=0;i<8;i++)
            {
              *p = val[i];
               p += stride;
            }
        }
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1_z(float16_t *p,
                          const float16x8_t val,
                          std::size_t nb,
                          mve_pred16_t p0)
    {
       (void)nb;
       vstrhq_p(p,val,p0);
    };

    template<int S,
    typename std::enable_if<(S>1) && (S<=65535),bool>::type = true>
    inline void vstore1_z(float16_t *p,
                          const float16x8_t val,
                          std::size_t nb,
                          mve_pred16_t p0)
    {
       (void)nb;
       
       constexpr uint16x8_t offset={0*S,1*S,2*S,3*S,4*S,5*S,6*S,7*S};
       vstrhq_scatter_shifted_offset_p_f16(p,offset,val,p0);
    };
    
    
    template<int S,
    typename std::enable_if<(S>65535),bool>::type = true>
    inline void vstore1_z(float16_t *p,
                          const float16x8_t val,
                          std::size_t nb,
                          mve_pred16_t p0)
    {
      (void)p0;
       for(std::size_t i=0;i<std::min(nb,8U);i++)
       {
           *p = val[i];
           p += S;
       }
    };

    // dynamic stride
    inline void vstore1_z(float16_t *p,
                          const index_t stride,
                          const float16x8_t val,
                          std::size_t nb,
                          mve_pred16_t p0)
    {
       if (stride<=65535)
       {
         uint16x8_t offset = vidupq_u16((uint32_t)0,1);
         offset = vmulq_n_u16(offset,stride);
         vstrhq_scatter_shifted_offset_p_f16(p,offset,val,p0);
       }
       else 
       {
         for(std::size_t i=0;i<std::min(nb,8U);i++)
         {
             *p = val[i];
             p += stride;
         }
       }
    };


}


#endif

/*! @} */
/*! @} */
