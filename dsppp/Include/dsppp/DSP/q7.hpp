// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_MATH_DSP
#undef ARM_MATH_MVEI
#undef ARM_MATH_MVEF
#undef ARM_MATH_NEON
#endif

/** \addtogroup DSPNumber DSP extension specific number definitions
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup DSPQ7Number Q7
 *  \ingroup DSPNumber
 *  @{
 */

#if defined(ARM_MATH_DSP)
#if !defined(ARM_MATH_MVEI) && !defined(ARM_MATH_MVEF) && !defined(ARM_MATH_NEON)


struct Q7DSPVector {
    Q7DSPVector():v(0){};
    explicit Q7DSPVector(int32_t val):v(val){};
    operator int32_t(){return v;};
int32_t v;
};

template<>
struct vector_traits<Q7,DSP,typename std::enable_if<true>::type> 
{
  typedef Q7 type;
  typedef type::value_type storage_type;
  typedef Q7DSPVector vector;
  typedef Q<17,14> temp_accumulator;

  /*

  The evaluators are not using any predication and instead 
  use additional code after the loop to manage the tail.

  So, no inner function with predicate_t is required.

  Fusion operators still have call to inner operator with
  predicate but they are not called in this context.

  */
  typedef uint32_t predicate_t;


  static constexpr bool has_vector = true;
  static constexpr bool is_float = false;
  static constexpr bool is_fixed = true;
  static constexpr bool has_predicate = false;

  static constexpr int nb_lanes = 4;

  static Q<17,14> temp_acc_zero()
  {
       return(Q<17,14>());
  }

  static constexpr int8_t zero_lane() {return 0;};

  static constexpr int8_t lane_value(const Q7 x) {return x.v;};


};



namespace inner {

    /* Needed to build but not used */
    template<>
    struct vctpq<Q7>{
       static uint32_t mk(uint32_t v)
       {
            return(v);
       };
    };

    __STATIC_FORCEINLINE Q7DSPVector vconst(Q7 val)
    {
       return(Q7DSPVector(__PACKq7(val.v, val.v, val.v, val.v)));
    }


    __STATIC_FORCEINLINE Q7DSPVector vneg(const Q7DSPVector a)
    {
       return(Q7DSPVector(__QSUB8(0, a.v)));
    };

    __STATIC_FORCEINLINE Q7DSPVector vadd(const Q7DSPVector a,
                                           const Q7DSPVector b)
    {
       return(Q7DSPVector(__QADD8(a.v,b.v)));
    };

    __STATIC_FORCEINLINE Q7DSPVector vadd(const Q7DSPVector a,
                                            const Q7 b)
    {
       return(Q7DSPVector(__QADD8(a.v,vconst(b).v)));
    };

     __STATIC_FORCEINLINE Q7DSPVector vadd(const Q7 a,
                                            const Q7DSPVector b)
    {
       return(Q7DSPVector(__QADD8(vconst(a).v,b.v)));
    };

     __STATIC_FORCEINLINE Q7DSPVector vsub(const Q7DSPVector a,
                                            const Q7DSPVector b)
    {
       return(Q7DSPVector(__QSUB8(a.v,b.v)));
    };

    __STATIC_FORCEINLINE Q7DSPVector vsub(const Q7DSPVector a,
                                            const Q7 b)
    {
       return(Q7DSPVector(__QSUB8(a.v,vconst(b).v)));
    };

     __STATIC_FORCEINLINE Q7DSPVector vsub(const Q7 a,
                                            const Q7DSPVector b)
    {
       return(Q7DSPVector(__QSUB8(vconst(a).v,b.v)));
    };

     __STATIC_FORCEINLINE Q7DSPVector vmul(const Q7DSPVector a,
                                            const Q7DSPVector b)
    {
        q7_t out1, out2, out3, out4;
        q15_t mul1,mul2,mul3,mul4;

        mul1 = (q15_t) ((q7_t) (a.v     ) * (q7_t) (b.v      ));
        mul2 = (q15_t) ((q7_t) (a.v >> 8) * (q7_t) (b.v >> 8));
        mul3 = (q15_t) ((q7_t) (a.v >> 16) * (q7_t) (b.v >> 16));
        mul4 = (q15_t) ((q7_t) (a.v >> 24) * (q7_t) (b.v >> 24));

        out1 = (q7_t) __SSAT(mul1 >> 7, 8);
        out2 = (q7_t) __SSAT(mul2 >> 7, 8);
        out3 = (q7_t) __SSAT(mul3 >> 7, 8);
        out4 = (q7_t) __SSAT(mul4 >> 7, 8);
        return(Q7DSPVector(__PACKq7(out1,out2,out3,out4)));
    };


    __STATIC_FORCEINLINE Q7DSPVector vmul(const Q7DSPVector a,
                                           const Q7 b)
    {
        return(vmul(a,vconst(b)));
    };

    __STATIC_FORCEINLINE Q7DSPVector vmul(const Q7 a,
                                           const Q7DSPVector b)
    {
        return(vmul(vconst(a),b));
    };


    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline Q7DSPVector vload1(const Q7 *p)
    {
       return(Q7DSPVector(read_q7x4(p)));
    };

   
    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline Q7DSPVector vload1(const Q7 *p)
    {
       Q7 a = p[0];
       Q7 b = p[S];
       Q7 c = p[2*S];
       Q7 d = p[3*S];

       return(Q7DSPVector(__PACKq7(a.v, b.v, c.v,d.v)));
    };


    // Dynamic stride
    inline Q7DSPVector vload1(const Q7 *p,index_t stride)
    {
       Q7 a = p[0];
       Q7 b = *(p+stride);
       Q7 c = *(p+2*stride);
       Q7 d = *(p+3*stride);

       return(Q7DSPVector(__PACKq7(a.v, b.v, c.v,d.v)));
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(Q7 *p,const Q7DSPVector val)
    {
       write_q7x4 (p, val.v);
    };

    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline void vstore1(Q7 *p,const Q7DSPVector val)
    {
       p[0] = Q7(val.v & 0x0FF);
       p[S] = Q7(val.v >> 8);
       p[2*S] = Q7(val.v >> 16);
       p[3*S] = Q7(val.v >> 24);
    };

    // dynamic stride 
    inline void vstore1(Q7 *p,const index_t stride,
                        const Q7DSPVector val)
    {
        p[0] = Q7(val.v & 0x0FF);
        *(p+stride) = Q7(val.v >> 8);
        *(p+2*stride) = Q7(val.v >> 16);
        *(p+3*stride) = Q7(val.v >> 24);
    }

    __STATIC_FORCEINLINE Q<17,14> vmacc(const Q<17,14> sum,
                                        const Q7DSPVector vala,
                                        const Q7DSPVector valb)
    {
      q31_t inA1, inA2, inB1, inB2; 
      q31_t s;
      inA1 = __SXTB16(__ROR(vala.v, 8));
      /* extract reminaing two samples */
      inA2 = __SXTB16(vala.v);
      /* extract two q7_t samples to q15_t samples */
      inB1 = __SXTB16(__ROR(valb.v, 8));
      /* extract reminaing two samples */
      inB2 = __SXTB16(valb.v);

      /* multiply and accumulate two samples at a time */
      s = __SMLAD(inA1, inB1, sum.v);
      s = __SMLAD(inA2, inB2, s);

      return(Q<17,14>(s));
    };

     __STATIC_FORCEINLINE Q<17,14> vmacc(const Q7DSPVector vala,
                                         const Q7DSPVector valb)
    {
       return(vmacc(Q<17,14>(0),vala,valb));
    };

    __STATIC_FORCEINLINE Q<17,14> vreduce(const Q<17,14> sum)
    {
       return(sum);
    };

  
};


#endif
#endif


/*! @} */
/*! @} */