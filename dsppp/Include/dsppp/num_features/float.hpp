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

/**
 * @brief      Features for float
 */
template<>
struct number_traits<float>
{
   //! It is a float number
   static constexpr bool is_float = true;

   //! It is not a fixed point
   static constexpr bool is_fixed = false;

   //! Accumulator datatype for this scalar datatype
   typedef float accumulator;

  /**
    * @brief      One for this datatype
    *
    * @return     Return 1 representation for this datatype
    */
   static constexpr float one() {return 1.0f;};

   //! Compute datatype for this scalar datatype
   typedef float compute_type;

   //! Display type for printf 
   typedef float display_type;
};


/*

If arch is not deriving from Neon or Helium, then there are
no vectors for float

*/

/**
 * @brief      Vector instructions for float when no Helium or Neon
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<float,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value>::type> {
  
  //! Current type
  typedef float type;

  //! Current storage type
  typedef float storage_type;

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
 * \ingroup GenericFloatNumber
 */
namespace inner {
   /**
    * @brief      Convert from accumulator representtaion
    *
    * @param[in]  a     Value
    *
    * @return     Accumulator value converted to current datatype
    */
  __STATIC_FORCEINLINE float from_accumulator(const float a)
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
  __STATIC_FORCEINLINE float mac(const float acc,const float a,const float b)
  {
     return(acc+a*b);
  };

/**
 * @brief      Scalar accumulate
 *
 * @param      a     Accumulator
 * @param[in]  b     Operand
 */
__STATIC_FORCEINLINE void accumulate(float &a,const float &b)
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
__STATIC_FORCEINLINE float mult(const float &a,const float &b)
{
   return(a*b);
}

}


/*! @} */
/*! @} */


