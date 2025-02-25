// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericComplexQ7Number Complex Q7
 *  \ingroup GenericNumber
 *  @{
 */

/**
 * @brief      Features for Complex Q7
 */
template<>
struct number_traits<std::complex<Q7>>
{
   //! Is not float
   static constexpr bool is_float = false;

   //! Is fixed point
   static constexpr bool is_fixed = true;

   //! Accumulator datatype
   typedef std::complex<Q<17,14>> accumulator;

   /**
    * @brief      One value
    *
    * @return     One value in std::complex<Q7>
    */
   static constexpr std::complex<Q7> one() {return std::complex<Q7>(Q7::one(),Q7{});};


   //! Compute type
   typedef std::complex<Q7> compute_type;

   //! Display type for printf 
   typedef std::complex<Q7> display_type;
};

template<>
struct number_traits<std::complex<Q<17,14>>>
{
   static constexpr bool is_float = false;
   static constexpr bool is_fixed = true;
};

/**
 * @brief      Vector description when no vector architecture
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<std::complex<Q7>,arch,void> {
  //! Current datatype
  typedef std::complex<Q7> type;

  //! Storage datatype (int8_t)
  typedef type::value_type storage_type;

  // No vector type but must still be defined
  //! Dummy datatype when no vector instructions
  typedef bool vector;
  typedef bool real_vector;
  //! Dummy datatype when no vector instructions
  typedef bool temp_accumulator;
  //! Dummy datatype when no vector instructions
  typedef uint32_t predicate_t;


  //! No vector instructions
  static constexpr bool has_vector = false;
  //! Is not float
  static constexpr bool is_float = false;
  //! Is fixed point
  static constexpr bool is_fixed = true;
  //! No predicated loop
  static constexpr bool has_predicate = false;

  //! Number of lanes
  static constexpr int nb_lanes = 1;
};

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericComplexQ7Number
 */
namespace inner {
    /**
     * @brief      Convert from accumulator with saturation
     *
     * @param[in]  a     Accumulator value
     *
     * @return     std::complex<Q7> value
     */
    __STATIC_FORCEINLINE std::complex<Q7> from_accumulator(const std::complex<Q<17,14>>& a)
    {
        return(std::complex<Q7>(
            __SSAT(a.real().v >> 7, 8),__SSAT(a.imag().v >> 7, 8)
            ));
    };

/**
 * @brief      Multiply and accumulate
 *
 * @param[in]  acc   Accumulator
 * @param[in]  a     First operand
 * @param[in]  b     Second operand
 *
 * @return     acc + a*b
 */
    __STATIC_FORCEINLINE std::complex<Q<17,14>> mac(const std::complex<Q<17,14>> acc,
                                                    const std::complex<Q7> a,
                                                    const std::complex<Q7> b)
    {
      return(accumulate(acc , mult(a,b)));
    };
}
   
/*! @} */
/*! @} */