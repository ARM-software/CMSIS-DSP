// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber Scalar number definitions
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericDoubleNumber Double
 *  \ingroup GenericNumber
 *  @{
 */


template<>
struct number_traits<double>
{
   static constexpr bool is_float = true;
   static constexpr bool is_fixed = false;
   typedef double accumulator;
   static constexpr double one() {return 1.0;};
   typedef double compute_type;
};

template<typename arch>
struct vector_traits<double,arch,void> {
  typedef double type;
  typedef double storage_type;

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
 __STATIC_FORCEINLINE double from_accumulator(const double a)
  {
     return(a);
  };

  __STATIC_FORCEINLINE double mac(const double acc,const double a,const double b)
  {
     return(acc+a*b);
  };

  __STATIC_FORCEINLINE void accumulate(double &a,const double &b)
{
   a += b;
}

__STATIC_FORCEINLINE double mult(double &a,const double &b)
{
   return(a*b);
}
}

/*! @} */
/*! @} */
