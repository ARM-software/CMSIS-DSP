// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_FLOAT16_SUPPORTED
#endif

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericHalfNumber Half
 *  \ingroup GenericNumber
 *  @{
 */

#if defined(ARM_FLOAT16_SUPPORTED)
template<>
struct number_traits<float16_t>
{
   static constexpr bool is_float = true;
   static constexpr bool is_fixed = false;
   typedef float16_t accumulator;
   static constexpr float16_t one() {return  ((float16_t)1.0f);};

   typedef _Float16 compute_type;
};


#if !defined(ARM_MATH_MVE_FLOAT16)
template<>
struct vector_traits<float16_t> {
  typedef float16_t type;
  typedef float16_t storage_type;

  // No vector type but must still be defined
  typedef bool vector;
  typedef bool temp_accumulator;
  typedef uint32_t predicate_t;

  
  static constexpr bool has_vector = false;
  static constexpr bool is_float = true;
  static constexpr bool is_fixed = false;
  static constexpr bool has_predicate = false;
};
#endif 

namespace inner {
   __STATIC_FORCEINLINE float16_t from_accumulator(const float16_t a)
   {
     return(a);
   };

  __STATIC_FORCEINLINE float16_t mac(const float16_t acc,const float16_t a,const float16_t b)
  {
     return((_Float16)acc+(_Float16)a*(_Float16)b);
  };
  

__STATIC_FORCEINLINE void accumulate(float16_t &a,const float16_t &b)
{
   a += (_Float16)b;
}

__STATIC_FORCEINLINE float16_t mult(float16_t &a,const float16_t &b)
{
   return((_Float16)a*(_Float16)b);
}

}

#endif

/*! @} */
/*! @} */