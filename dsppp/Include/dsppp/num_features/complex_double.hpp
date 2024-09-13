// -*- C++ -*-
/** @file */ 
#pragma once 


/** \addtogroup GenericNumber Scalar number definitions
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericComplexDoubleNumber Complex Double
 *  \ingroup GenericNumber
 *  @{
 */


/**
 * @brief      Features for complex double
 */
template<>
struct number_traits<std::complex<double>>
{
   //! It is a float number
   static constexpr bool is_float = true;

   //! It is not a fixed point
   static constexpr bool is_fixed = false;

   //! Accumulator datatype for this scalar datatype
   typedef std::complex<double> accumulator;

   /**
    * @brief      One for this datatype
    *
    * @return     Return 1 representation for this datatype
    */
   static constexpr std::complex<double> one() {return std::complex<double>(1.0,0.0);};

   //! Compute datatype for this scalar datatype
   typedef std::complex<double> compute_type;
};

/**
 * @brief      Default vector datatype description for this scalar datatype
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<std::complex<double>,arch,void> {

  /**
   * Scalar datatype
   */
  typedef std::complex<double> type;

  /**
   * Storage datatype
   */
  typedef std::complex<double> storage_type;

  // No vector type but must still be defined

  /**
   * Dummy  datatype. Must be present for building but not used
   * since by default there is no vector architecture assumed
   */
  typedef bool vector;
  typedef bool real_vector;

  /**
   * Dummy  datatype. Must be present for building but not used
   * since by default there is no vector architecture assumed
   */
  typedef bool temp_accumulator;

  /**
   * Dummy  datatype. Must be present for building but not used
   * since by default there is no vector architecture assumed
   */
  typedef uint32_t predicate_t;

  /**
   * By default : no vector architecture assumed
   */
  static constexpr bool has_vector = false;

  //! It is a float
  static constexpr bool is_float = true;
  //! Not a fixed point
  static constexpr bool is_fixed = false;
  //! No predicated loops
  static constexpr bool has_predicate = false;

  //! Number of lanes
  static constexpr int nb_lanes = 1;
};

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericComplexDoubleNumber
 */
namespace inner {
   /**
    * @brief      Convert from accumulator representation
    *
    * @param[in]  a     Value
    *
    * @return     Accumulator value converted to current datatype
    */
 __STATIC_FORCEINLINE std::complex<double> from_accumulator(const std::complex<double>& a)
  {
     return(a);
  };

/**
 * @brief      Multiply and accumulate for this datatype
 *
 * @param[in]  acc   The accumulated value
 * @param[in]  a     The left hand side
 * @param[in]  b     The right hand side
 *
 * @return     Return acc + a*b
 */
  __STATIC_FORCEINLINE std::complex<double> mac(const std::complex<double>& acc,
                                                const std::complex<double>& a,
                                                const std::complex<double>& b)
  {
     return(acc+a*b);
  };

/**
 * @brief      Accumulate
 *
 * @param      a     Accumulator
 * @param[in]  b     VAlue to be added
 */
  __STATIC_FORCEINLINE void accumulate(std::complex<double> &a,
                                       const std::complex<double> &b)
{
   a += b;
}

/**
 * @brief      Multiply
 *
 * @param      a     Left hand side
 * @param[in]  b     Right hand side
 *
 * @return     Return a*b
 */
__STATIC_FORCEINLINE std::complex<double> mult(const std::complex<double> &a,
                                               const std::complex<double> &b)
{
   return(a*b);
}
}

/*! @} */
/*! @} */
