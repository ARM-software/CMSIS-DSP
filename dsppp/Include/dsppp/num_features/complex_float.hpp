// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericComplexFloatNumber Complex Float
 *  \ingroup GenericNumber
 *  @{
 */

/**
 * @brief      Features for complex float
 */
template<>
struct number_traits<std::complex<float>>
{
   //! It is a float number
   static constexpr bool is_float = true;

   //! It is not a fixed point
   static constexpr bool is_fixed = false;

   //! Accumulator datatype for this scalar datatype
   typedef std::complex<float> accumulator;

  /**
    * @brief      One for this datatype
    *
    * @return     Return 1 representation for this datatype
    */
   static constexpr std::complex<float> one() {return std::complex<float>(1.0f,0.0f);};

   //! Compute datatype for this scalar datatype
   typedef std::complex<float> compute_type;

   //! Display type for printf 
   typedef std::complex<float> display_type;
};


/*

If arch is not deriving from Neon or Helium, then there are
no vectors for float

*/

/**
 * @brief      Vector instructions for complex float when no Helium or Neon
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<std::complex<float>,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value>::type> {
  
  //! Current type
  typedef std::complex<float> type;

  //! Current storage type
  typedef std::complex<float> storage_type;
  // No vector type but must still be defined

  //! Dummy type. Not used when no vector instructions
  typedef bool vector;
  typedef bool real_vector;
  //! Dummy type. Not used when no vector instructions
  typedef bool temp_accumulator;
  //! Dummy type. Not used when no vector instructions
  typedef uint32_t predicate_t;

  
  //! No vector instructions for this datatype
  static constexpr bool has_vector = false;
  //! Is float
  static constexpr bool is_float = true;
  //! Is fixed
  static constexpr bool is_fixed = false;

  //! No predicated loop
  static constexpr bool has_predicate = false;

  //! Number of lanes
  static constexpr int nb_lanes = 1;

};

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericComplexFloatNumber
 */
namespace inner {
   /**
    * @brief      Convert from accumulator representtaion
    *
    * @param[in]  a     Value
    *
    * @return     Accumulator value converted to current datatype
    */
  __STATIC_FORCEINLINE std::complex<float> from_accumulator(const std::complex<float>& a)
  {
     return(a);
  };

/**
 * @brief      Scalar multiply and accumulate
 *
 * @param[in]  acc   Accumulator
 * @param[in]  a     Operand
 * @param[in]  b     Operand
 *
 * @return     acc + a*b
 */
  __STATIC_FORCEINLINE std::complex<float> mac(const std::complex<float>& acc,
                                               const std::complex<float>& a,
                                               const std::complex<float>& b)
  {
     return(acc+a*b);
  };

/**
 * @brief      Scalar accumulate
 *
 * @param      a     Accumulator
 * @param[in]  b     Operand
 */
__STATIC_FORCEINLINE void accumulate(std::complex<float> &a,
                                     const std::complex<float> &b)
{
   a += b;
}

/**
 * @brief      Scalar multiply
 *
 * @param      a     Operand
 * @param[in]  b     Operand
 *
 * @return     a*b
 */
__STATIC_FORCEINLINE std::complex<float> mult(const std::complex<float> &a,
                                              const std::complex<float> &b)
{
   return(a*b);
}

}


/*! @} */
/*! @} */


