// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericQ15Number Q15
 *  \ingroup GenericNumber
 *  @{
 */

template<>
struct number_traits<Q15>
{
   static constexpr bool is_float = false;
   static constexpr bool is_fixed = true;
   typedef Q<33,30> accumulator;
   static constexpr Q15 one() {return Q15::one();};
   typedef Q15 compute_type;
};

template<typename arch>
struct vector_traits<Q15,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value &&
                            !std::is_base_of<DSP,arch>::value>::type> {
  typedef Q15 type;
  typedef type::value_type storage_type;

  // No vector type but must still be defined
  typedef bool vector;
  typedef bool temp_accumulator;
  typedef uint32_t predicate_t;


  
  static constexpr bool has_vector = false;
  static constexpr bool is_float = false;
  static constexpr bool is_fixed = true;
  static constexpr bool has_predicate = false;

};

namespace inner {
#if defined(ARM_MATH_MVEI)
    __STATIC_FORCEINLINE Q15 from_accumulator(const Q<33,30> a)
    {
      //return(saturate(toFrac<15>(a)));
        return(Q15((sqrshrl_sat48(a.v, -(32-15)) >> 32) & 0xffffffff));
    };
#else 
     __STATIC_FORCEINLINE Q15 from_accumulator(const Q<33,30> a)
    {
       return(saturate(toFrac<15>(a)));
    };
#endif

    __STATIC_FORCEINLINE Q<33,30> mac(const Q<33,30> acc,const Q15 a,const Q15 b)
    {
      return(accumulate(acc , mult(a,b)));
    };
}

/*! @} */
/*! @} */