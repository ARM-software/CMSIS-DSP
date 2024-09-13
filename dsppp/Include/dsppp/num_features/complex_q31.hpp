// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericComplexQ31Number Complex Q31
 *  \ingroup GenericNumber
 *  @{
 */

/**
 * @brief      Features for complex Q31
 */
template<>
struct number_traits<std::complex<Q31>>
{
   //! Is not a float
   static constexpr bool is_float = false;
   //! Is fixed point
   static constexpr bool is_fixed = true;
   //! Accumulator datatype
   typedef std::complex<Q<15,48>> accumulator;
   /**
    * @brief      One value
    *
    * @return     One value
    */
   static constexpr std::complex<Q31> one() {return std::complex<Q31>(Q31::one(),Q31{});};

   //! Compute type
   typedef std::complex<Q31> compute_type;

   //! Display type for printf 
   typedef std::complex<Q31> display_type;
};

template<>
struct number_traits<std::complex<Q<15,48>>>
{
   static constexpr bool is_float = false;
   static constexpr bool is_fixed = true;
};

/**
 * @brief      Vector features for std::complex<Q31> when no vector instructions
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<std::complex<Q31>,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value>::type> {
  //! Datatype
  typedef std::complex<Q31> type;

  //! Storage tpe (int32_t)
  typedef type::value_type storage_type;

  // No vector type but must still be defined
  //! Dummy type when no vector instructions are supported
  typedef bool vector;
  typedef bool real_vector;
  //! Dummy type when no vector instructions are supported
  typedef bool temp_accumulator;
  //! Dummy type when no vector instructions are supported
  typedef uint32_t predicate_t;


  //! No vector instruction
  static constexpr bool has_vector = false;
  //! Is not float
  static constexpr bool is_float = false;
  //! Is fixed
  static constexpr bool is_fixed = true;
  //! No predicated loop
  static constexpr bool has_predicate = false;

  //! Number of lanes
  static constexpr int nb_lanes = 1;
};

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericComplexQ31Number
 */
namespace inner {
#if defined(ARM_MATH_MVEI)
  /**
   * @brief      Convert from accumulator (with no saturation)
   *
   * @param[in]  a     Accumulator value
   *
   * @return     Converted value
   */
  __STATIC_FORCEINLINE std::complex<Q31> from_accumulator(const std::complex<Q<15,48>> a)
  {
        return(std::complex<Q31>(Q31(asrl(a.real().v, 17)),Q31(asrl(a.imag().v, 17))));
  };
#else
   /**
   * @brief      Convert from accumulator (with no saturation)
   *
   * @param[in]  a     Accumulator value
   *
   * @return     Converted value
   */
  __STATIC_FORCEINLINE std::complex<Q31> from_accumulator(const std::complex<Q<15,48>> a)
  {
        return(std::complex<Q31>(Q31(a.real().v >> 17),Q31(a.imag().v >> 17)));
  };
#endif


/**
 * @brief      Multiply and accumulate
 *
 * @param[in]  acc   Accumulator
 * @param[in]  a     First operand
 * @param[in]  b     Second operand
 *
 * @return     acc + a*b
 */
__STATIC_FORCEINLINE std::complex<Q<15,48>> mac(const std::complex<Q<15,48>> acc,
                                                const std::complex<Q31> a,
                                                const std::complex<Q31> b)
{
    return(accumulate(acc , toFrac<48>(mult(a,b))));
};
    
  }

/*! @} */
/*! @} */