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
 *  \addtogroup HeliumComplexQ15Number Complex Q15
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
struct vector_traits<std::complex<Q15>,arch,
typename std::enable_if<std::is_base_of<Helium,arch>::value>::type > 
{
  typedef std::complex<Q15> type;
  typedef type::value_type storage_type;
  typedef ComplexVector<int16x8_t> vector;
  typedef HalfComplexVector<int16x8_t> half_vector;
  typedef int16x8_t real_vector;
  typedef std::complex<Q<33,30>> temp_accumulator;
  //! Predicate datatype (not used)
  typedef bool predicate_t;

  static constexpr bool has_vector = true;
  static constexpr bool is_float = false;
  static constexpr bool is_fixed = true;
  static constexpr bool has_predicate = false;

  static constexpr int nb_lanes = 8;

  //! If support mixed arithmetic
  static constexpr bool support_mixed = true;


  static std::complex<Q<33,30>> temp_acc_zero()
  {
       return(std::complex<Q<33,30>>());
  }

  static constexpr int16_t zero_lane() {return 0;};

  //static constexpr int16_t lane_value(const std::complex<Q15> x) {return x.v;};

};

/**
 * Inner implementation of Helium intrinsics
 * \ingroup HeliumComplexQ15Number
 */
namespace inner {

    template<>
    struct vctpq<std::complex<Q15>>{
       static mve_pred16_t mk(uint32_t v)
       {
            if (v>4)
            {
              return(vctp16q(2*(v-4)));
            }
            else 
            {
               return(vctp16q(2*v));
            }
       };
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vconst(std::complex<Q15> v)
    {
             int16x8_t bv;

        #if defined(SCATTER_CONST)
             static constexpr uint16x8_t rd_offset = {0,1,0,1,0,1,0,1};
             bv=vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(&v),rd_offset);
        #else
             bv = vdupq_n_s16(v.real());
             bv[1] = v.imag();
             bv[3] = v.imag();
             bv[5] = v.imag();
             bv[7] = v.imag();
        #endif

             return(ComplexVector<int16x8_t>(bv,bv));
    }

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vconst(const int16x8_t v)
    {
       int16x8_t ca;
       int16x8_t cb;

       ca=vdupq_n_s16(0);
       cb=vdupq_n_s16(0);

       ca[0] = v[0];
       ca[2] = v[1];
       ca[4] = v[2];
       ca[6] = v[3];

       cb[0] = v[4];
       cb[2] = v[5];
       cb[4] = v[6];
       cb[6] = v[7];

       return(ComplexVector<int16x8_t>(ca,cb));
    }

    __STATIC_FORCEINLINE int16x8_t half_vconst(const std::complex<Q15> v)
    {
       int16x8_t bv;
  #if defined(SCATTER_CONST)
       static constexpr uint16x8_t rd_offset = {0,1,0,1,0,1,0,1};
       bv=vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(&v),rd_offset);
  #else
       bv = vdupq_n_s16(v.real());
       bv[1] = v.imag();
       bv[3] = v.imag();
       bv[5] = v.imag();
       bv[7] = v.imag();
  #endif
       return(bv);
  
    }

    __STATIC_FORCEINLINE int16x8_t half_vconst(const Q15 v)
    {
       int16x8_t bv;
  #if defined(SCATTER_CONST)
       static int16_t tmp[2];
       tmp[0] = v.v;
       tmp[1] = 0;
       static constexpr uint16x8_t rd_offset = {0,1,0,1,0,1,0,1};
       bv=vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(tmp),rd_offset);
  #else  
       bv = vdupq_n_s16(v.v);
       bv[1] = 0.;
       bv[3] = 0;
       bv[5] = 0;
       bv[7] = 0;
  #endif
       return(bv);
    }


    
    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vneg(const ComplexVector<int16x8_t> a)
    {
       return(ComplexVector<int16x8_t>(vnegq(a.va),vnegq(a.vb)));
    };

     static constexpr int16x8_t cmplx_conj_sign_int16 = { 1, -1, 1, -1, 1, -1, 1, -1 };


    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vconjugate(const ComplexVector<int16x8_t> a)
    {
       return (ComplexVector<int16x8_t>(vmulq(a.va, cmplx_conj_sign_int16),vmulq(a.vb, cmplx_conj_sign_int16)));
    };


    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vadd(const ComplexVector<int16x8_t> a,
                                                       const ComplexVector<int16x8_t> b)
    {
     return(ComplexVector<int16x8_t>(vqaddq(a.va,b.va),vqaddq(a.vb,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vadd(const ComplexVector<int16x8_t> a,
                                                        const std::complex<Q15> b)
    {
       const int16x8_t c = half_vconst(b);
       return(ComplexVector<int16x8_t>(vqaddq(a.va,c),vqaddq(a.vb,c)));
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vadd(const ComplexVector<int16x8_t> a,
                                                        const Q15 b)
    {
       const int16x8_t c = half_vconst(b);
       return(ComplexVector<int16x8_t>(vqaddq(a.va,c),vqaddq(a.vb,c)));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vadd(const ComplexVector<int16x8_t> &a,
                                                       const int16x8_t b)
    {
       ComplexVector<int16x8_t> c = vconst(b);
       return(vadd(a,c));
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vadd(const std::complex<Q15> a,
                                                        const ComplexVector<int16x8_t> b)
    {
       const int16x8_t c = half_vconst(a);
       return(ComplexVector<int16x8_t>(vqaddq(c,b.va),vqaddq(c,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vadd(const Q15 a,
                                                        const ComplexVector<int16x8_t> &b)
    {
       const int16x8_t c = half_vconst(a);
       return(ComplexVector<int16x8_t>(vqaddq(c,b.va),vqaddq(c,b.vb)));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vadd(const int16x8_t a,
                                                       const ComplexVector<int16x8_t> &b)
    {
       ComplexVector<int16x8_t> c = vconst(a);
       return(vadd(c,b));
    };


   
    
   

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vsub(const ComplexVector<int16x8_t> a,
                                                       const ComplexVector<int16x8_t> b)
    {
       return(ComplexVector<int16x8_t>(vqsubq(a.va,b.va),vqsubq(a.vb,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vsub(const ComplexVector<int16x8_t> a,
                                                        const std::complex<Q15> b)
    {
       const int16x8_t c = half_vconst(b);
       return(ComplexVector<int16x8_t>(vqsubq(a.va,c),vqsubq(a.vb,c)));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vsub(const ComplexVector<int16x8_t> &a,
                                                       const Q15 b)
    {
       const int16x8_t c = half_vconst(b);
       return(ComplexVector<int16x8_t>(vqsubq(a.va,c),vqsubq(a.vb,c)));
    };
  
    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vsub(const ComplexVector<int16x8_t> &a,
                                                       const int16x8_t b)
    {
       ComplexVector<int16x8_t> c = vconst(b);
       return vsub(a,c);
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vsub(const std::complex<Q15> a,
                                                        const ComplexVector<int16x8_t> b)
    {
       const int16x8_t c = half_vconst(a);
       return(ComplexVector<int16x8_t>(vqsubq(c,b.va),vqsubq(c,b.vb)));
    };

     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vsub(const Q15 a,
                                                        const ComplexVector<int16x8_t> &b)
    {
       const int16x8_t c = half_vconst(a);
       return(ComplexVector<int16x8_t>(vqsubq(c,b.va),vqsubq(c,b.vb)));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const ComplexVector<int16x8_t> a,
                                                       const ComplexVector<int16x8_t> b)
    {
        int16x8_t vec_acc_a = vqdmlsdhq(vuninitializedq_s16(),a.va, b.va);
        vec_acc_a = vqdmladhxq(vec_acc_a, a.va, b.va);

        int16x8_t vec_acc_b = vqdmlsdhq(vuninitializedq_s16(),a.vb, b.vb);
        vec_acc_b = vqdmladhxq(vec_acc_b, a.vb, b.vb);

        return(ComplexVector<int16x8_t>(vec_acc_a,vec_acc_b));

    };
  
     __STATIC_FORCEINLINE ComplexVector<int16x8_t> vsub(const int16x8_t a,
                                                        const ComplexVector<int16x8_t> &b)
    {
       ComplexVector<int16x8_t> c = vconst(a);
       return vsub(c,b);
    };

   
  

    __STATIC_FORCEINLINE HalfComplexVector<int16x8_t> vmul(const HalfComplexVector<int16x8_t> a,
                                                           const HalfComplexVector<int16x8_t> b)
    {
        int16x8_t vec_acc_a = vqdmlsdhq(vuninitializedq_s16(),a.va, b.va);
        vec_acc_a = vqdmladhxq(vec_acc_a, a.va, b.va);

        return(HalfComplexVector<int16x8_t>(vec_acc_a));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const ComplexVector<int16x8_t> a,
                                                       const std::complex<Q15> b)
    {
        int16x8_t vecB = half_vconst(b);

        int16x8_t vec_acc_a = vqdmlsdhq(vuninitializedq_s16(),a.va, vecB);
        vec_acc_a = vqdmladhxq(vec_acc_a, a.va, vecB);

        int16x8_t vec_acc_b = vqdmlsdhq(vuninitializedq_s16(),a.vb, vecB);
        vec_acc_b = vqdmladhxq(vec_acc_b, a.vb, vecB);

        return(ComplexVector<int16x8_t>(vec_acc_a,vec_acc_b));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const int16x8_t &a,
                                                       const std::complex<Q15> &b)
    {
       static int16_t tmp[16];
       int16x8x2_t c;
       int16x8_t va,vb;
       c.val[0] = vqdmulhq(a,b.real().v);
       c.val[1] = vqdmulhq(a,b.imag().v);
       vst2q(tmp,c);
       va=vld1q(tmp);
       vb=vld1q(tmp+8);

       return(ComplexVector<int16x8_t>(va,vb));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const ComplexVector<int16x8_t> &a,
                                                       const Q15 b)
    {
  
       int16x8_t vec_acc_a = vqdmulhq(a.va, b.v);
       int16x8_t vec_acc_b = vqdmulhq(a.vb, b.v);
  
       return(ComplexVector<int16x8_t>(vec_acc_a,vec_acc_b));
    };
  
    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const ComplexVector<int16x8_t> &a,
                                                       const int16x8_t b)
    {
       ComplexVector<int16x8_t> c = vconst(b);
       return vmul(a,c);
    };


    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const std::complex<Q15> a,
                                                       const ComplexVector<int16x8_t> b)
    {
        int16x8_t vecA = half_vconst(a);

        int16x8_t vec_acc_a = vqdmlsdhq(vuninitializedq_s16(),vecA, b.va);
        vec_acc_a = vqdmladhxq(vec_acc_a, vecA, b.va);

        int16x8_t vec_acc_b = vqdmlsdhq(vuninitializedq_s16(),vecA, b.vb);
        vec_acc_b = vqdmladhxq(vec_acc_b, vecA, b.vb);

        return(ComplexVector<int16x8_t>(vec_acc_a,vec_acc_b));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const std::complex<Q15> &a,
                                                       const int16x8_t &b
                                                        )
    {
       static int16_t tmp[16];
       int16x8x2_t c;
       int16x8_t va,vb;
       c.val[0] = vqdmulhq(b,a.real().v);
       c.val[1] = vqdmulhq(b,a.imag().v);
       vst2q(tmp,c);
       va=vld1q(tmp);
       vb=vld1q(tmp+8);

       return(ComplexVector<int16x8_t>(va,vb));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const Q15 a,
                                                       const ComplexVector<int16x8_t> &b)
    {
       int16x8_t vec_acc_a = vqdmulhq(b.va, a.v);
       int16x8_t vec_acc_b = vqdmulhq(b.vb, a.v);

       return(ComplexVector<int16x8_t>(vec_acc_a,vec_acc_b));
    };

    __STATIC_FORCEINLINE ComplexVector<int16x8_t> vmul(const int16x8_t a,
                                                       const ComplexVector<int16x8_t> &b)
    {
       ComplexVector<int16x8_t> c = vconst(a);
       return vmul(c,b);
    }

    __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const std::complex<Q<33,30>> sum,
                                                     const ComplexVector<int16x8_t> vala,
                                                     const ComplexVector<int16x8_t> valb)
    {
       Q<33,30> re,im;

       re.v = vmlsldavaq(sum.real().v,vala.va,valb.va);
       im.v = vmlaldavaxq(sum.imag().v,vala.va,valb.va);

       re.v = vmlsldavaq(re.v,vala.vb,valb.vb);
       im.v = vmlaldavaxq(im.v,vala.vb,valb.vb);

       return(std::complex<Q<33,30>>(re,im));
    };

    __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const std::complex<Q<33,30>> sum,
                                                     const HalfComplexVector<int16x8_t> vala,
                                                     const HalfComplexVector<int16x8_t> valb)
    {
       Q<33,30> re,im;

       re.v = vmlsldavaq(sum.real().v,vala.va,valb.va);
       im.v = vmlaldavaxq(sum.imag().v,vala.va,valb.va);

       return(std::complex<Q<33,30>>(re,im));
    };

    __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const std::complex<Q<33,30>> &acc,
                                                     const int16x8_t a,
                                                     const ComplexVector<int16x8_t> &b)
    {
       ComplexVector<int16x8_t> c = vconst(a);
       return vmacc(acc,c,b);
    };
  
     __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const std::complex<Q<33,30>> &acc,
                                                      const ComplexVector<int16x8_t> &a,
                                                      const int16x8_t b)
     {
       ComplexVector<int16x8_t> c = vconst(b);
       return vmacc(acc,a,c);
     }

     __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const std::complex<Q<33,30>> sum,
                                                      const ComplexVector<int16x8_t> vala,
                                                      const std::complex<Q15> valb)
    {
       Q<33,30> re,im;
       int16x8_t vecB = half_vconst(valb);

       re.v = vmlsldavaq(sum.real().v,vala.va,vecB);
       im.v = vmlaldavaxq(sum.imag().v,vala.va,vecB);

       re.v = vmlsldavaq(re.v,vala.vb,vecB);
       im.v = vmlaldavaxq(im.v,vala.vb,vecB);

       return(std::complex<Q<33,30>>(re,im));
    };

     __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const std::complex<Q<33,30>> &sum,
                                                      const int16x8_t &vala,
                                                      const std::complex<Q15> &valb)
    {
       Q<33,30> re,im;
       int16x8_t vecB = half_vconst(valb);
       ComplexVector<int16x8_t> ca = vconst(vala);
  
       re.v = vmlsldavaq(sum.real().v, ca.va, vecB);
       im.v = vmlaldavaxq(sum.imag().v, ca.va, vecB);
  
       re.v = vmlsldavaq(re.v, ca.vb, vecB);
       im.v = vmlaldavaxq(im.v, ca.vb, vecB);
     

       return(std::complex<Q<33,30>>(re,im));
    };

     __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const std::complex<Q<33,30>> sum,
                                                      const ComplexVector<int16x8_t> vala,
                                                      const Q15 valb)
    {
       Q<33,30> re,im;
       int16x8_t vecB = half_vconst(valb);

       re.v = vmlsldavaq(sum.real().v,vala.va,vecB);
       im.v = vmlaldavaxq(sum.imag().v,vala.va,vecB);

       re.v = vmlsldavaq(re.v,vala.vb,vecB);
       im.v = vmlaldavaxq(im.v,vala.vb,vecB);

       return(std::complex<Q<33,30>>(re,im));
    };


     __STATIC_FORCEINLINE std::complex<Q<33,30>> vmacc(const ComplexVector<int16x8_t> vala,
                                                      const ComplexVector<int16x8_t> valb)
    {
       return(vmacc(std::complex<Q<33,30>>{},vala,valb));
    };


    __STATIC_FORCEINLINE std::complex<Q<33,30>> vreduce(const std::complex<Q<33,30>> &sum)
    {
       const Q<33,30> re = vreduce(sum.real());
       const Q<33,30> im = vreduce(sum.imag());

       return(std::complex<Q<33,30>>(re,im));
    };


    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline ComplexVector<int16x8_t> vload1(const std::complex<Q15> *p)
    {
       return(ComplexVector<int16x8_t>(
      vld1q(reinterpret_cast<const int16_t*>(p)),
      vld1q(reinterpret_cast<const int16_t*>(p)+8)
      ));
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline HalfComplexVector<int16x8_t> vload1_half(const std::complex<Q15> *p)
    {
       return(HalfComplexVector<int16x8_t>(
        vld1q(reinterpret_cast<const int16_t*>(p))
        ));
    };

    __STATIC_FORCEINLINE HalfComplexVector<int16x8_t> vload1_half(const std::complex<Q15> *p,
                                                                  const index_t stride)
    {
     
     constexpr uint16x8_t scale = {0,0,2,2,4,4,6,6};
     constexpr uint16x8_t cmplx = {0,1,0,1,0,1,0,1};
     uint16x8_t offset;
  
     offset = vmulq_n_u16(scale,stride);
     offset = vaddq(offset,cmplx);
  
     return(HalfComplexVector<int16x8_t>(
       vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(p),offset))
     );
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline ComplexVector<int16x8_t> vload1_z(const std::complex<Q15> *p,
                                             const std::size_t nb,
                                             const mve_pred16_t p0)
    {
       if (nb > 4)
       {
           return(ComplexVector<int16x8_t>(
                 vld1q(reinterpret_cast<const int16_t*>(p)),
                 vld1q_z(reinterpret_cast<const int16_t*>(p)+8,p0)
           ));
       }
       else 
       {
           return(ComplexVector<int16x8_t>(
                 vld1q_z(reinterpret_cast<const int16_t*>(p),p0),
                 vdupq_n_s16(0)
           ));
       }
    };
  

    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline ComplexVector<int16x8_t> vload1(const std::complex<Q15> *p)
    {
       constexpr uint16x8_t offset={0*S,0*S+1,2*S,2*S+1,4*S,4*S+1,6*S,6*S+1};
     return(ComplexVector<int16x8_t>(
      vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(p),offset),
      vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(p)+8*S,offset)
      ));
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline HalfComplexVector<int16x8_t> vload1_half_z(const std::complex<Q15> *p,
                                                      const std::size_t nb,
                                                      const mve_pred16_t p0)
    {
       (void)nb;
       return(HalfComplexVector<int16x8_t>(vld1q_z(reinterpret_cast<const int16_t*>(p),p0)));
    };


    // Dynamic stride
    inline ComplexVector<int16x8_t> vload1(const std::complex<Q15> *p,
                                                 index_t stride)
    {
        constexpr uint16x8_t scale = {0,0,2,2,4,4,6,6};
        constexpr uint16x8_t cmplx = {0,1,0,1,0,1,0,1};
        uint16x8_t offset;
   
        offset = vmulq_n_u16(scale,stride);
        offset = vqaddq(offset,cmplx);
        return(ComplexVector<int16x8_t>(
         vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(p),offset),
         vldrhq_gather_shifted_offset_s16(reinterpret_cast<const int16_t*>(p)+8*stride,offset)
         ));
        
    }

  template<int ...S>
  struct vload1_half_gen_stride<std::complex<Q15>,S...>
  {
     template<int ...I>
     static HalfComplexVector<int16x8_t> _run(const std::complex<Q15> *p,
      std::integer_sequence<int,I...>)
     {
        constexpr uint16x8_t offsetA={I...};
        return(HalfComplexVector<int16x8_t>(
         vldrhq_gather_shifted_offset_s16(
            reinterpret_cast<const int16_t*>(p),offsetA)
        ));
     };

     /**
      * @brief      Load with generalized stride
      *
      * @param[in]  p     Load address
      *
      * @return     Gather load
      */
     static HalfComplexVector<int16x8_t> run(const std::complex<Q15> *p)
     {
        return _run(p,typename ToComplexStride<S...>::type{});
     };
  };

   

    
    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(      std::complex<Q15> *p,
                        const ComplexVector<int16x8_t> val)
    {
       vst1q(reinterpret_cast<int16_t*>(p),val.va);
       vst1q(reinterpret_cast<int16_t*>(p)+8,val.vb);
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1_z(std::complex<Q15> *p,
                          ComplexVector<int16x8_t> &val,
                          const std::size_t nb,
                          const mve_pred16_t p0)
    {
       if (nb>4)
       {
         vst1q(reinterpret_cast<int16_t*>(p),val.va);
         vstrhq_p(reinterpret_cast<int16_t*>(p)+8,val.vb,p0);
       }
       else 
       {
          vstrhq_p(reinterpret_cast<int16_t*>(p),val.va,p0);
       }
    };

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(std::complex<Q15> *p,const int16x8_t val)
    {
       int16x8_t ca,cb;
       ca = vdupq_n_s16(0);
       cb = vdupq_n_s16(0);
       ca[0] = val[0];
       ca[2] = val[1];
       ca[4] = val[2];
       ca[6] = val[3];
  
       cb[0] = val[4];
       cb[2] = val[5];
       cb[4] = val[6];
       cb[6] = val[7];
  
       vst1q(reinterpret_cast<int16_t*>(p),ca);
       vst1q(reinterpret_cast<int16_t*>(p)+8,cb);
    };

    template<int S,
    typename std::enable_if<(S>1) ,bool>::type = true>
    inline void vstore1(      std::complex<Q15> *p,
                        const ComplexVector<int16x8_t> val)
    {

       constexpr uint16x8_t offset={0*S,0*S+1,2*S,2*S+1,4*S,4*S+1,6*S,6*S+1};
       vstrhq_scatter_shifted_offset_s16(reinterpret_cast<int16_t*>(p),offset,val.va);
       vstrhq_scatter_shifted_offset_s16(reinterpret_cast<int16_t*>(p)+8*S,offset,val.vb);
    };

   

    // dynamic stride 
    inline void vstore1(      std::complex<Q15> *p,
                        const index_t stride,
                        const ComplexVector<int16x8_t> val)
    {
        
        constexpr uint16x8_t scale = {0,0,2,2,4,4,6,6};
        constexpr uint16x8_t cmplx = {0,1,0,1,0,1,0,1};
        uint16x8_t offset;
     
        offset = vmulq_n_u16(scale,stride);
        offset = vqaddq(offset,cmplx);
        vstrhq_scatter_shifted_offset_s16(reinterpret_cast<int16_t*>(p),offset,val.va);
        vstrhq_scatter_shifted_offset_s16(reinterpret_cast<int16_t*>(p)+8*stride,offset,val.vb);
    }

   


};

#endif

/*! @} */
/*! @} */
