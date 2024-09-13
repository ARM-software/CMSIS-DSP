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
 *  \addtogroup HeliumComplexQ31Number Complex Q31
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
struct vector_traits<std::complex<Q31>,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type > 
{
  typedef std::complex<Q31> type;
  typedef type::value_type storage_type;
  typedef ComplexVector<int32x4_t> vector;
  typedef HalfComplexVector<int32x4_t> half_vector;
  typedef int32x4_t real_vector;
  typedef std::complex<Q<9,54>> temp_accumulator;
  //! Predicate datatype (not used)
  typedef bool predicate_t;

  static constexpr bool has_vector = true;
  static constexpr bool is_float = false;
  static constexpr bool is_fixed = true;
  static constexpr bool has_predicate = false;

  static constexpr int nb_lanes = 4;

  //! If support mixed arithmetic
  static constexpr bool support_mixed = true;


  static std::complex<Q<9,54>> temp_acc_zero()
  {
       return(std::complex<Q<9,54>>());
  }

  static constexpr int16_t zero_lane() {return 0;};

  //static constexpr int16_t lane_value(const std::complex<Q31> x) {return x.v;};

};

/**
 * Inner implementation of Helium intrinsics
 * \ingroup HeliumComplexQ31Number
 */
namespace inner {

    template<>
    struct vctpq<std::complex<Q31>>{
       static mve_pred16_t mk(uint32_t v)
       {
            if (v>2)
            {
              return(vctp32q(2*(v-2)));
            }
            else 
            {
               return(vctp32q(2*v));
            }
       };
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vconst(std::complex<Q31> v)
    {
             int32x4_t bv;

        #if defined(SCATTER_CONST)
             static constexpr uint32x4_t rd_offset = {0,1,0,1};
             bv=vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(&v),rd_offset);
        #else
             bv = vdupq_n_s32(v.real());
             bv[1] = v.imag();
             bv[3] = v.imag();
        #endif

             return(ComplexVector<int32x4_t>(bv,bv));
    }

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vconst(const int32x4_t v)
    {
       int32x4_t ca;
       int32x4_t cb;

       ca=vdupq_n_s32(0);
       cb=vdupq_n_s32(0);

       ca[0] = v[0];
       ca[2] = v[1];

       cb[0] = v[2];
       cb[2] = v[3];

       return(ComplexVector<int32x4_t>(ca,cb));
    }

    __STATIC_FORCEINLINE int32x4_t half_vconst(const std::complex<Q31> v)
    {
       int32x4_t bv;
  #if defined(SCATTER_CONST)
       static constexpr uint32x4_t rd_offset = {0,1,0,1};
       bv=vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(&v),rd_offset);
  #else
       bv = vdupq_n_s32(v.real());
       bv[1] = v.imag();
       bv[3] = v.imag();
  #endif
       return(bv);
  
    }

    __STATIC_FORCEINLINE int32x4_t half_vconst(const Q31 v)
    {
       int32x4_t bv;
  #if defined(SCATTER_CONST)
       static int32_t tmp[2];
       tmp[0] = v.v;
       tmp[1] = 0;
       static constexpr uint32x4_t rd_offset = {0,1,0,1};
       bv=vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(tmp),rd_offset);
  #else  
       bv = vdupq_n_s32(v.v);
       bv[1] = 0;
       bv[3] = 0;
  #endif
       return(bv);
    }


    
    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vneg(const ComplexVector<int32x4_t> a)
    {
       return(ComplexVector<int32x4_t>(vnegq(a.va),vnegq(a.vb)));
    };

     static constexpr int32x4_t cmplx_conj_sign_int32 = { 1, -1, 1, -1 };


    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vconjugate(const ComplexVector<int32x4_t> a)
    {
       return (ComplexVector<int32x4_t>(vmulq(a.va, cmplx_conj_sign_int32),vmulq(a.vb, cmplx_conj_sign_int32)));
    };


    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vadd(const ComplexVector<int32x4_t> a,
                                                       const ComplexVector<int32x4_t> b)
    {
     return(ComplexVector<int32x4_t>(vqaddq(a.va,b.va),vqaddq(a.vb,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vadd(const ComplexVector<int32x4_t> a,
                                                        const std::complex<Q31> b)
    {
       const int32x4_t c = half_vconst(b);
       return(ComplexVector<int32x4_t>(vqaddq(a.va,c),vqaddq(a.vb,c)));
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vadd(const ComplexVector<int32x4_t> a,
                                                        const Q31 b)
    {
       const int32x4_t c = half_vconst(b);
       return(ComplexVector<int32x4_t>(vqaddq(a.va,c),vqaddq(a.vb,c)));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vadd(const ComplexVector<int32x4_t> &a,
                                                       const int32x4_t b)
    {
       ComplexVector<int32x4_t> c = vconst(b);
       return(vadd(a,c));
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vadd(const std::complex<Q31> a,
                                                        const ComplexVector<int32x4_t> b)
    {
       const int32x4_t c = half_vconst(a);
       return(ComplexVector<int32x4_t>(vqaddq(c,b.va),vqaddq(c,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vadd(const Q31 a,
                                                        const ComplexVector<int32x4_t> &b)
    {
       const int32x4_t c = half_vconst(a);
       return(ComplexVector<int32x4_t>(vqaddq(c,b.va),vqaddq(c,b.vb)));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vadd(const int32x4_t a,
                                                       const ComplexVector<int32x4_t> &b)
    {
       ComplexVector<int32x4_t> c = vconst(a);
       return(vadd(c,b));
    };


   
    
   

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vsub(const ComplexVector<int32x4_t> a,
                                                       const ComplexVector<int32x4_t> b)
    {
       return(ComplexVector<int32x4_t>(vqsubq(a.va,b.va),vqsubq(a.vb,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vsub(const ComplexVector<int32x4_t> a,
                                                        const std::complex<Q31> b)
    {
       const int32x4_t c = half_vconst(b);
       return(ComplexVector<int32x4_t>(vqsubq(a.va,c),vqsubq(a.vb,c)));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vsub(const ComplexVector<int32x4_t> &a,
                                                       const Q31 b)
    {
       const int32x4_t c = half_vconst(b);
       return(ComplexVector<int32x4_t>(vqsubq(a.va,c),vqsubq(a.vb,c)));
    };
  
    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vsub(const ComplexVector<int32x4_t> &a,
                                                       const int32x4_t b)
    {
       ComplexVector<int32x4_t> c = vconst(b);
       return vsub(a,c);
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vsub(const std::complex<Q31> a,
                                                        const ComplexVector<int32x4_t> b)
    {
       const int32x4_t c = half_vconst(a);
       return(ComplexVector<int32x4_t>(vqsubq(c,b.va),vqsubq(c,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vsub(const Q31 a,
                                                        const ComplexVector<int32x4_t> &b)
    {
       const int32x4_t c = half_vconst(a);
       return(ComplexVector<int32x4_t>(vqsubq(c,b.va),vqsubq(c,b.vb)));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const ComplexVector<int32x4_t> a,
                                                       const ComplexVector<int32x4_t> b)
    {
        int32x4_t vec_acc_a = vqdmlsdhq(vuninitializedq_s32(),a.va, b.va);
        vec_acc_a = vqdmladhxq(vec_acc_a, a.va, b.va);

        int32x4_t vec_acc_b = vqdmlsdhq(vuninitializedq_s32(),a.vb, b.vb);
        vec_acc_b = vqdmladhxq(vec_acc_b, a.vb, b.vb);

        return(ComplexVector<int32x4_t>(vec_acc_a,vec_acc_b));
    };
  
     __STATIC_FORCEINLINE ComplexVector<int32x4_t> vsub(const int32x4_t a,
                                                        const ComplexVector<int32x4_t> &b)
    {
       ComplexVector<int32x4_t> c = vconst(a);
       return vsub(c,b);
    };

   
  

    __STATIC_FORCEINLINE HalfComplexVector<int32x4_t> vmul(const HalfComplexVector<int32x4_t> a,
                                                           const HalfComplexVector<int32x4_t> b)
    {
        int32x4_t vec_acc_a = vqdmlsdhq(vuninitializedq_s32(),a.va, b.va);
        vec_acc_a = vqdmladhxq(vec_acc_a, a.va, b.va);

        return(HalfComplexVector<int32x4_t>(vec_acc_a));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const ComplexVector<int32x4_t> a,
                                                       const std::complex<Q31> b)
    {
        int32x4_t vecB = half_vconst(b);

        int32x4_t vec_acc_a = vqdmlsdhq(vuninitializedq_s32(),a.va, vecB);
        vec_acc_a = vqdmladhxq(vec_acc_a, a.va, vecB);

        int32x4_t vec_acc_b = vqdmlsdhq(vuninitializedq_s32(),a.vb, vecB);
        vec_acc_b = vqdmladhxq(vec_acc_b, a.vb, vecB);

        return(ComplexVector<int32x4_t>(vec_acc_a,vec_acc_b));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const int32x4_t &a,
                                                       const std::complex<Q31> &b)
    {
       static int32_t tmp[8];
       int32x4x2_t c;
       int32x4_t va,vb;
       c.val[0] = vqdmulhq(a,b.real().v);
       c.val[1] = vqdmulhq(a,b.imag().v);
       vst2q(tmp,c);
       va=vld1q(tmp);
       vb=vld1q(tmp+4);

       return(ComplexVector<int32x4_t>(va,vb));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const ComplexVector<int32x4_t> &a,
                                                       const Q31 b)
    {
  
       int32x4_t vec_acc_a = vqdmulhq(a.va, b.v);
       int32x4_t vec_acc_b = vqdmulhq(a.vb, b.v);
  
       return(ComplexVector<int32x4_t>(vec_acc_a,vec_acc_b));
    };
  
    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const ComplexVector<int32x4_t> &a,
                                                       const int32x4_t b)
    {
       ComplexVector<int32x4_t> c = vconst(b);
       return vmul(a,c);
    };


    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const std::complex<Q31> a,
                                                       const ComplexVector<int32x4_t> b)
    {
        int32x4_t vecA = half_vconst(a);

        int32x4_t vec_acc_a = vqdmlsdhq(vuninitializedq_s32(),vecA, b.va);
        vec_acc_a = vqdmladhxq(vec_acc_a, vecA, b.va);

        int32x4_t vec_acc_b = vqdmlsdhq(vuninitializedq_s32(),vecA, b.vb);
        vec_acc_b = vqdmladhxq(vec_acc_b, vecA, b.vb);

        return(ComplexVector<int32x4_t>(vec_acc_a,vec_acc_b));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const std::complex<Q31> &a,
                                                       const int32x4_t &b
                                                        )
    {
       static int32_t tmp[8];
       int32x4x2_t c;
       int32x4_t va,vb;
       c.val[0] = vqdmulhq(b,a.real().v);
       c.val[1] = vqdmulhq(b,a.imag().v);
       vst2q(tmp,c);
       va=vld1q(tmp);
       vb=vld1q(tmp+4);

       return(ComplexVector<int32x4_t>(va,vb));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const Q31 a,
                                                       const ComplexVector<int32x4_t> &b)
    {
       int32x4_t vec_acc_a = vqdmulhq(b.va, a.v);
       int32x4_t vec_acc_b = vqdmulhq(b.vb, a.v);

       return(ComplexVector<int32x4_t>(vec_acc_a,vec_acc_b));
    };

    __STATIC_FORCEINLINE ComplexVector<int32x4_t> vmul(const int32x4_t a,
                                                       const ComplexVector<int32x4_t> &b)
    {
       ComplexVector<int32x4_t> c = vconst(a);
       return vmul(c,b);
    }

    __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const std::complex<Q<9,54>> sum,
                                                     const ComplexVector<int32x4_t> vala,
                                                     const ComplexVector<int32x4_t> valb)
    {
       Q<9,54> re,im;

       re.v = vrmlsldavhaq(sum.real().v,vala.va,valb.va);
       im.v = vrmlaldavhaxq(sum.imag().v,vala.va,valb.va);

       re.v = vrmlsldavhaq(re.v,vala.vb,valb.vb);
       im.v = vrmlaldavhaxq(im.v,vala.vb,valb.vb);

       return(std::complex<Q<9,54>>(re,im));
    };

    __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const std::complex<Q<9,54>> sum,
                                                     const HalfComplexVector<int32x4_t> vala,
                                                     const HalfComplexVector<int32x4_t> valb)
    {
       Q<9,54> re,im;

       re.v = vrmlsldavhaq(sum.real().v,vala.va,valb.va);
       im.v = vrmlaldavhaxq(sum.imag().v,vala.va,valb.va);

       return(std::complex<Q<9,54>>(re,im));
    };

    __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const std::complex<Q<9,54>> &acc,
                                                     const int32x4_t a,
                                                     const ComplexVector<int32x4_t> &b)
    {
       ComplexVector<int32x4_t> c = vconst(a);
       return vmacc(acc,c,b);
    };
  
     __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const std::complex<Q<9,54>> &acc,
                                                      const ComplexVector<int32x4_t> &a,
                                                      const int32x4_t b)
     {
       ComplexVector<int32x4_t> c = vconst(b);
       return vmacc(acc,a,c);
     }

     __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const std::complex<Q<9,54>> sum,
                                                      const ComplexVector<int32x4_t> vala,
                                                      const std::complex<Q31> valb)
    {
       Q<9,54> re,im;
       int32x4_t vecB = half_vconst(valb);

       re.v = vrmlsldavhaq(sum.real().v,vala.va,vecB);
       im.v = vrmlaldavhaxq(sum.imag().v,vala.va,vecB);

       re.v = vrmlsldavhaq(re.v,vala.vb,vecB);
       im.v = vrmlaldavhaxq(im.v,vala.vb,vecB);

       return(std::complex<Q<9,54>>(re,im));
    };

     __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const std::complex<Q<9,54>> &sum,
                                                      const int32x4_t &vala,
                                                      const std::complex<Q31> &valb)
    {
       Q<9,54> re,im;
       int32x4_t vecB = half_vconst(valb);
       ComplexVector<int32x4_t> ca = vconst(vala);
  
       re.v = vrmlsldavhaq(sum.real().v, ca.va, vecB);
       im.v = vrmlaldavhaxq(sum.imag().v, ca.va, vecB);
  
       re.v = vrmlsldavhaq(re.v, ca.vb, vecB);
       im.v = vrmlaldavhaxq(im.v, ca.vb, vecB);
     

       return(std::complex<Q<9,54>>(re,im));
    };

     __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const std::complex<Q<9,54>> sum,
                                                      const ComplexVector<int32x4_t> vala,
                                                      const Q31 valb)
    {
       Q<9,54> re,im;
       int32x4_t vecB = half_vconst(valb);

       re.v = vrmlsldavhaq(sum.real().v,vala.va,vecB);
       im.v = vrmlaldavhaxq(sum.imag().v,vala.va,vecB);

       re.v = vrmlsldavhaq(re.v,vala.vb,vecB);
       im.v = vrmlaldavhaxq(im.v,vala.vb,vecB);

       return(std::complex<Q<9,54>>(re,im));
    };


     __STATIC_FORCEINLINE std::complex<Q<9,54>> vmacc(const ComplexVector<int32x4_t> vala,
                                                      const ComplexVector<int32x4_t> valb)
    {
       return(vmacc(std::complex<Q<9,54>>{},vala,valb));
    };


    __STATIC_FORCEINLINE std::complex<Q<15,48>> vreduce(const std::complex<Q<9,54>> &sum)
    {
       const Q<15,48> re = vreduce(sum.real());
       const Q<15,48> im = vreduce(sum.imag());

       return(std::complex<Q<15,48>>(re,im));
    };


    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline ComplexVector<int32x4_t> vload1(const std::complex<Q31> *p)
    {
       return(ComplexVector<int32x4_t>(
      vld1q(reinterpret_cast<const int32_t*>(p)),
      vld1q(reinterpret_cast<const int32_t*>(p)+4)
      ));
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline HalfComplexVector<int32x4_t> vload1_half(const std::complex<Q31> *p)
    {
       return(HalfComplexVector<int32x4_t>(
        vld1q(reinterpret_cast<const int32_t*>(p))
        ));
    };

    __STATIC_FORCEINLINE HalfComplexVector<int32x4_t> vload1_half(const std::complex<Q31> *p,
                                                                  const index_t stride)
    {
     
     constexpr uint32x4_t scale = {0,0,2,2};
     constexpr uint32x4_t cmplx = {0,1,0,1};
     uint32x4_t offset;
  
     offset = vmulq_n_u32(scale,stride);
     offset = vaddq(offset,cmplx);
  
     return(HalfComplexVector<int32x4_t>(
       vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(p),offset))
     );
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline ComplexVector<int32x4_t> vload1_z(const std::complex<Q31> *p,
                                             const std::size_t nb,
                                             const mve_pred16_t p0)
    {
       if (nb > 2)
       {
           return(ComplexVector<int32x4_t>(
                 vld1q(reinterpret_cast<const int32_t*>(p)),
                 vld1q_z(reinterpret_cast<const int32_t*>(p)+4,p0)
           ));
       }
       else 
       {
           return(ComplexVector<int32x4_t>(
                 vld1q_z(reinterpret_cast<const int32_t*>(p),p0),
                 vdupq_n_s32(0)
           ));
       }
    };
  

    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline ComplexVector<int32x4_t> vload1(const std::complex<Q31> *p)
    {
       constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
     return(ComplexVector<int32x4_t>(
      vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(p),offset),
      vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(p)+4*S,offset)
      ));
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline HalfComplexVector<int32x4_t> vload1_half_z(const std::complex<Q31> *p,
                                                      const std::size_t nb,
                                                      const mve_pred16_t p0)
    {
       (void)nb;
       return(HalfComplexVector<int32x4_t>(vld1q_z(reinterpret_cast<const int32_t*>(p),p0)));
    };


    // Dynamic stride
    inline ComplexVector<int32x4_t> vload1(const std::complex<Q31> *p,
                                                 index_t stride)
    {
        constexpr uint32x4_t scale = {0,0,2,2};
        constexpr uint32x4_t cmplx = {0,1,0,1};
        uint32x4_t offset;
   
        offset = vmulq_n_u32(scale,stride);
        offset = vqaddq(offset,cmplx);
        return(ComplexVector<int32x4_t>(
         vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(p),offset),
         vldrwq_gather_shifted_offset_s32(reinterpret_cast<const int32_t*>(p)+4*stride,offset)
         ));
        
    }

  template<int ...S>
  struct vload1_half_gen_stride<std::complex<Q31>,S...>
  {
     template<int ...I>
     static HalfComplexVector<int32x4_t> _run(const std::complex<Q31> *p,
      std::integer_sequence<int,I...>)
     {
        constexpr uint32x4_t offsetA={I...};
        return(HalfComplexVector<int32x4_t>(
         vldrwq_gather_shifted_offset_s32(
            reinterpret_cast<const int32_t*>(p),offsetA)
        ));
     };

     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Gather load
      */
     static HalfComplexVector<int32x4_t> run(const std::complex<Q31> *p)
     {
        return _run(p,typename ToComplexStride<S...>::type{});
     };
  };

   

    
    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(      std::complex<Q31> *p,
                        const ComplexVector<int32x4_t> val)
    {
       vst1q(reinterpret_cast<int32_t*>(p),val.va);
       vst1q(reinterpret_cast<int32_t*>(p)+4,val.vb);
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1_z(std::complex<Q31> *p,
                          ComplexVector<int32x4_t> &val,
                          const std::size_t nb,
                          const mve_pred16_t p0)
    {
       if (nb>2)
       {
         vst1q(reinterpret_cast<int32_t*>(p),val.va);
         vstrwq_p(reinterpret_cast<int32_t*>(p)+4,val.vb,p0);
       }
       else 
       {
          vstrwq_p(reinterpret_cast<int32_t*>(p),val.va,p0);
       }
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(std::complex<Q31> *p,const int32x4_t val)
    {
       int32x4_t ca,cb;
       ca = vdupq_n_s32(0);
       cb = vdupq_n_s32(0);
       ca[0] = val[0];
       ca[2] = val[1];
       cb[0] = val[2];
       cb[2] = val[3];
  
       vst1q(reinterpret_cast<int32_t*>(p),ca);
       vst1q(reinterpret_cast<int32_t*>(p)+4,cb);
    };

    template<int S,
    typename std::enable_if<(S>1) ,bool>::type = true>
    inline void vstore1(      std::complex<Q31> *p,
                        const ComplexVector<int32x4_t> val)
    {

       constexpr uint32x4_t offset={0*S,0*S+1,2*S,2*S+1};
       vstrwq_scatter_shifted_offset_s32(reinterpret_cast<int32_t*>(p),offset,val.va);
       vstrwq_scatter_shifted_offset_s32(reinterpret_cast<int32_t*>(p)+4*S,offset,val.vb);
    };

   

    // dynamic stride 
    inline void vstore1(      std::complex<Q31> *p,
                        const index_t stride,
                        const ComplexVector<int32x4_t> val)
    {
        
        constexpr uint32x4_t scale = {0,0,2,2};
        constexpr uint32x4_t cmplx = {0,1,0,1};
        uint32x4_t offset;
     
        offset = vmulq_n_u32(scale,stride);
        offset = vqaddq(offset,cmplx);
        vstrwq_scatter_shifted_offset_s32(reinterpret_cast<int32_t*>(p),offset,val.va);
        vstrwq_scatter_shifted_offset_s32(reinterpret_cast<int32_t*>(p)+4*stride,offset,val.vb);
    }

   


};

#endif

/*! @} */
/*! @} */
