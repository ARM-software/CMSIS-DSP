// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericQ7Number Q7
 *  \ingroup GenericNumber
 *  @{
 */

template<>
struct number_traits<Q7>
{
   static constexpr bool is_float = false;
   static constexpr bool is_fixed = true;
   typedef Q<17,14> accumulator;
   static constexpr Q7 one() {return Q7::one();};
   typedef Q7 compute_type;
};

template<typename arch>
struct vector_traits<Q7,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value &&
                            !std::is_base_of<DSP,arch>::value>::type> {
  typedef Q7 type;
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
    __STATIC_FORCEINLINE Q7 from_accumulator(const Q<17,14> a)
    {
        return(Q7(__SSAT(a.v >> 7, 8)));
    };

    __STATIC_FORCEINLINE Q<17,14> mac(const Q<17,14> acc,const Q7 a,const Q7 b)
    {
      return(accumulate(acc , mult(a,b)));
    };
}
   
/*! @} */
/*! @} */