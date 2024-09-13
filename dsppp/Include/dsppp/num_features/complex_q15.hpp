// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericComplexQ15Number Complex Q15
 *  \ingroup GenericNumber
 *  @{
 */

/**
 * @brief     Features for complex Q15
 */
template<>
struct number_traits<std::complex<Q15>>
{
   //! Is not float
   static constexpr bool is_float = false;
   //! Is fixed point
   static constexpr bool is_fixed = true;
   //! Accumulator datatype
   typedef std::complex<Q<33,30>> accumulator;
   /**
    * @brief      One value
    *
    * @return     One value in std::complex<Q15>
    */
   static constexpr std::complex<Q15> one() {return std::complex<Q15>(Q15::one(),Q15{});};
   //! Compute type
   typedef std::complex<Q15> compute_type;

   //! Display type for printf 
   typedef std::complex<Q15> display_type;
};

template<>
struct number_traits<std::complex<Q<33,30>>>
{
   static constexpr bool is_float = false;
   static constexpr bool is_fixed = true;
};

/**
 * @brief      Vector features for Q15 when no vector architecture
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<std::complex<Q15>,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value>::type> {
  //! Compute type
  typedef std::complex<Q15> type;

  //! Storage datatype (int16_t)
  typedef type::value_type storage_type;

  // No vector type but must still be defined
  //! Dummy type when no vector instructions
  typedef bool vector;
  typedef bool real_vector;
  //! Dummy type when no vector instructions
  typedef bool temp_accumulator;
  //! Dummy type when no vector instructions
  typedef uint32_t predicate_t;


  //! Has no vector instructions
  static constexpr bool has_vector = false;
  //! Is not float
  static constexpr bool is_float = false;
  //! Is fixed point
  static constexpr bool is_fixed = true;
  //! Has no predicated loop
  static constexpr bool has_predicate = false;

  //! Number of lanes
  static constexpr int nb_lanes = 1;

};

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericComplexQ15Number
 */
namespace inner {
#if defined(ARM_MATH_MVEI)
    /**
     * @brief      Convert from accumulator type
     *
     * @param[in]  a    The accumulator value
     *
     * @return     The converted value (with saturation)
     */
    __STATIC_FORCEINLINE std::complex<Q15> from_accumulator(const std::complex<Q<33,30>> &a)
    {
        return(std::complex<Q15>(
            (sqrshrl_sat48(a.real().v, -(32-15)) >> 32) & 0xffffffff,
            (sqrshrl_sat48(a.imag().v, -(32-15)) >> 32) & 0xffffffff
        ));
    };
#else 
    /**
     * @brief      Convert from accumulator type
     *
     * @param[in]  a    The accumulator value
     *
     * @return     The converted value (with saturation)
     */
     __STATIC_FORCEINLINE std::complex<Q15> from_accumulator(const std::complex<Q<33,30>>& a)
    {
       return(saturate(toFrac<15>(a)));
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
    __STATIC_FORCEINLINE std::complex<Q<33,30>> mac(const std::complex<Q<33,30>>& acc,
                                                    const std::complex<Q15> a,
                                                    const std::complex<Q15> b)
    {
      return(accumulate(acc , mult(a,b)));
    };
}

/*! @} */
/*! @} */