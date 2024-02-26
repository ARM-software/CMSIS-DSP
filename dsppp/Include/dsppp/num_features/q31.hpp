// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericQ31Number Q31
 *  \ingroup GenericNumber
 *  @{
 */

template<>
struct number_traits<Q31>
{
   static constexpr bool is_float = false;
   static constexpr bool is_fixed = true;
   typedef Q<15,48> accumulator;
   static constexpr Q31 one() {return Q31::one();};
   typedef Q31 compute_type;
};

template<typename arch>
struct vector_traits<Q31,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value>::type> {
  typedef Q31 type;
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
  __STATIC_FORCEINLINE Q31 from_accumulator(const Q<15,48> a)
  {
        return(Q31(asrl(a.v, 17)));
  };
#else
  __STATIC_FORCEINLINE Q31 from_accumulator(const Q<15,48> a)
  {
        return(Q31(a.v >> 17));
  };
#endif


__STATIC_FORCEINLINE Q<15,48> mac(const Q<15,48> acc,const Q31 a,const Q31 b)
{
    return(accumulate(acc , toFrac<48>(mult(a,b))));
};
    
  }

/*! @} */
/*! @} */