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
 *  \addtogroup DSPQ15Number Q15
 *  \ingroup DSPNumber
 *  @{
 */

#if defined(ARM_MATH_DSP)
#if !defined(ARM_MATH_MVEI) && !defined(ARM_MATH_MVEF) && !defined(ARM_MATH_NEON)


struct Q15DSPVector {
    Q15DSPVector():v(0){};
    explicit Q15DSPVector(int32_t val):v(val){};
    operator int32_t(){return v;};

int32_t v;
};

template<>
struct vector_traits<Q15,DSP,typename std::enable_if<true>::type> 
{
  typedef Q15 type;
  typedef type::value_type storage_type;
  typedef Q15DSPVector vector;
  typedef Q<33,30> temp_accumulator;

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

  static constexpr int nb_lanes = 2;

  static Q<33,30> temp_acc_zero()
  {
       return(Q<33,30>());
  }

  static constexpr int16_t zero_lane() {return 0;};

  static constexpr int16_t lane_value(const Q15 x) {return x.v;};


};



namespace inner {

    /* Needed to build but not used */
    template<>
    struct vctpq<Q15>{
       static uint32_t mk(uint32_t v)
       {
            return(v);
       };
    };

    __STATIC_FORCEINLINE Q15DSPVector vconst(Q15 val)
    {
       return(Q15DSPVector(__PKHBT(val.v, val.v, 16)));
    }


    __STATIC_FORCEINLINE Q15DSPVector vneg(const Q15DSPVector a)
    {
       return(Q15DSPVector(__QSUB16(0, a.v)));
    };

    __STATIC_FORCEINLINE Q15DSPVector vadd(const Q15DSPVector a,
                                           const Q15DSPVector b)
    {
       return(Q15DSPVector(__QADD16(a.v,b.v)));
    };

    __STATIC_FORCEINLINE Q15DSPVector vadd(const Q15DSPVector a,
                                            const Q15 b)
    {
       return(Q15DSPVector(__QADD16(a.v,vconst(b).v)));
    };

     __STATIC_FORCEINLINE Q15DSPVector vadd(const Q15 a,
                                            const Q15DSPVector b)
    {
       return(Q15DSPVector(__QADD16(vconst(a).v,b.v)));
    };

     __STATIC_FORCEINLINE Q15DSPVector vsub(const Q15DSPVector a,
                                            const Q15DSPVector b)
    {
       return(Q15DSPVector(__QSUB16(a.v,b.v)));
    };

    __STATIC_FORCEINLINE Q15DSPVector vsub(const Q15DSPVector a,
                                            const Q15 b)
    {
       return(Q15DSPVector(__QSUB16(a.v,vconst(b).v)));
    };

     __STATIC_FORCEINLINE Q15DSPVector vsub(const Q15 a,
                                            const Q15DSPVector b)
    {
       return(Q15DSPVector(__QSUB16(vconst(a).v,b.v)));
    };

     __STATIC_FORCEINLINE Q15DSPVector vmul(const Q15DSPVector a,
                                            const Q15DSPVector b)
    {
        q31_t mul1,mul2;
        q15_t out1,out2;

        mul1 = (q31_t) ((q15_t) (a.v      ) * (q15_t) (b.v      ));
        mul2 = (q31_t) ((q15_t) (a.v >> 16) * (q15_t) (b.v >> 16));

        out1 = (q15_t) __SSAT(mul1 >> 15, 16);
        out2 = (q15_t) __SSAT(mul2 >> 15, 16);
        return(Q15DSPVector(__PKHBT(out1, out2, 16)));
    };


    __STATIC_FORCEINLINE Q15DSPVector vmul(const Q15DSPVector a,
                                           const Q15 b)
    {
        return(vmul(a,vconst(b)));
    };

    __STATIC_FORCEINLINE Q15DSPVector vmul(const Q15 a,
                                           const Q15DSPVector b)
    {
        return(vmul(vconst(a),b));
    };


    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline Q15DSPVector vload1(const Q15 *p)
    {
       return(Q15DSPVector(read_q15x2(p)));
    };

   
    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline Q15DSPVector vload1(const Q15 *p)
    {
       Q15 a = p[0];
       Q15 b = p[S];

       return(Q15DSPVector(__PKHBT(a.v, b.v, 16)));
    };


    // Dynamic stride
    inline Q15DSPVector vload1(const Q15 *p,index_t stride)
    {
       Q15 a = p[0];
       Q15 b = *(p+stride);

       return(Q15DSPVector(__PKHBT(a.v, b.v, 16)));
    }

    template<int S,
    typename std::enable_if<S==1,bool>::type = true>
    inline void vstore1(Q15 *p,const Q15DSPVector val)
    {
       write_q15x2 (p, val.v);
    };

    template<int S,
    typename std::enable_if<(S>1),bool>::type = true>
    inline void vstore1(Q15 *p,const Q15DSPVector val)
    {
       p[0] = Q15(val.v & 0x0FFFF);
       p[S] = Q15(val.v >> 16);
    };

    // dynamic stride 
    inline void vstore1(Q15 *p,const index_t stride,
                        const Q15DSPVector val)
    {
        p[0] = Q15(val.v & 0x0FFFF);
        *(p+stride) = Q15(val.v >> 16);
    }

    __STATIC_FORCEINLINE Q<33,30> vmacc(const Q<33,30> sum,
                                        const Q15DSPVector vala,
                                        const Q15DSPVector valb)
    {
       return(Q<33,30>(__SMLALD(vala.v,valb.v,sum.v)));
    };

     __STATIC_FORCEINLINE Q<33,30> vmacc(const Q15DSPVector vala,
                                         const Q15DSPVector valb)
    {
       return(Q<33,30>(__SMLALD(vala.v,valb.v,0)));
    };

    __STATIC_FORCEINLINE Q<33,30> vreduce(const Q<33,30> sum)
    {
       return(sum);
    };

  
};


#endif
#endif

/*! @} */
/*! @} */