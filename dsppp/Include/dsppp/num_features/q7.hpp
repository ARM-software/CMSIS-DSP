// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericQ7Number Q7
 *  \ingroup GenericNumber
 *  @{
 */

/**
 * @brief      Q7 features
 */
template<>
struct number_traits<Q7>
{
   //! Is not float
   static constexpr bool is_float = false;

   //! Is fixed point
   static constexpr bool is_fixed = true;

   //! Accumulator datatype
   typedef Q<17,14> accumulator;

   /**
    * @brief      One value
    *
    * @return     One value in Q7
    */
   static constexpr Q7 one() {return Q7::one();};


   //! Compute type
   typedef Q7 compute_type;

   //! Display type for printf 
   typedef Q7 display_type;
};

template<>
struct number_traits<Q<17,14>>
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
struct vector_traits<Q7,arch,
    typename std::enable_if<!std::is_base_of<Helium,arch>::value &&
                            !std::is_base_of<Neon,arch>::value &&
                            !std::is_base_of<DSP,arch>::value>::type> {
  //! Current datatype
  typedef Q7 type;

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
 * \ingroup GenericQ7Number
 */
namespace inner {
    /**
     * @brief      Convert from accumulator with saturation
     *
     * @param[in]  a     Accumulator value
     *
     * @return     Q7 value
     */
    __STATIC_FORCEINLINE Q7 from_accumulator(const Q<17,14> a)
    {
        return(Q7(__SSAT(a.v >> 7, 8)));
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
    __STATIC_FORCEINLINE Q<17,14> mac(const Q<17,14> acc,const Q7 a,const Q7 b)
    {
      return(accumulate(acc , mult(a,b)));
    };
}
   
/*! @} */
/*! @} */