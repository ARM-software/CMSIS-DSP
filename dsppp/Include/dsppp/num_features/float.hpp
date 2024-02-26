// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericFloatNumber Float
 *  \ingroup GenericNumber
 *  @{
 */

template<>
struct number_traits<float>
{
   static constexpr bool is_float = true;
   static constexpr bool is_fixed = false;
   typedef float accumulator;
   static constexpr float one() {return 1.0f;};
   typedef float compute_type;
};


/*

If arch is not deriving from Neon or Helium, then there are
no vectors for float

*/
template<typename arch>
struct vector_traits<float,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value>::type> {
  typedef float type;
  typedef float storage_type;

  // No vector type but must still be defined
  typedef bool vector;
  typedef bool temp_accumulator;
  typedef uint32_t predicate_t;

  
  static constexpr bool has_vector = false;
  static constexpr bool is_float = true;
  static constexpr bool is_fixed = false;
  static constexpr bool has_predicate = false;

};

namespace inner {
  __STATIC_FORCEINLINE float from_accumulator(const float a)
  {
     return(a);
  };

  __STATIC_FORCEINLINE float mac(const float acc,const float a,const float b)
  {
     return(acc+a*b);
  };

__STATIC_FORCEINLINE void accumulate(float &a,const float &b)
{
   a += b;
}

__STATIC_FORCEINLINE float mult(float &a,const float &b)
{
   return(a*b);
}

}


/*! @} */
/*! @} */


