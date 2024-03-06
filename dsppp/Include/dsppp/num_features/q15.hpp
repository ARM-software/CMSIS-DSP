// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericQ15Number Q15
 *  \ingroup GenericNumber
 *  @{
 */

/**
 * @brief      Q15 features
 */
template<>
struct number_traits<Q15>
{
   //! Is not float
   static constexpr bool is_float = false;
   //! Is fixed point
   static constexpr bool is_fixed = true;
   //! Accumulator datatype
   typedef Q<33,30> accumulator;
   /**
    * @brief      One value
    *
    * @return     One value in Q15
    */
   static constexpr Q15 one() {return Q15::one();};
   //! Compute type
   typedef Q15 compute_type;
};

/**
 * @brief      Vector features for Q15 when no vector architecture
 *
 * @tparam     arch  Current architecture
 */
template<typename arch>
struct vector_traits<Q15,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value &&
                            !std::is_base_of<DSP,arch>::value>::type> {
  //! Compute type
  typedef Q15 type;

  //! Storage datatype (int16_t)
  typedef type::value_type storage_type;

  // No vector type but must still be defined
  //! Dummy type when no vector instructions
  typedef bool vector;
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

};

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericNumber
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
    __STATIC_FORCEINLINE Q15 from_accumulator(const Q<33,30> a)
    {
      //return(saturate(toFrac<15>(a)));
        return(Q15((sqrshrl_sat48(a.v, -(32-15)) >> 32) & 0xffffffff));
    };
#else 
    /**
     * @brief      Convert from accumulator type
     *
     * @param[in]  a    The accumulator value
     *
     * @return     The converted value (with saturation)
     */
     __STATIC_FORCEINLINE Q15 from_accumulator(const Q<33,30> a)
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
    __STATIC_FORCEINLINE Q<33,30> mac(const Q<33,30> acc,const Q15 a,const Q15 b)
    {
      return(accumulate(acc , mult(a,b)));
    };
}

/*! @} */
/*! @} */