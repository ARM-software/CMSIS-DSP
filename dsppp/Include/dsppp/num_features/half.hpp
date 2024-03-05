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

/**
 * @brief      Feature of float16 datatype
 */
template<>
struct number_traits<float16_t>
{
   //! It is a float number
   static constexpr bool is_float = true;
   //! It is not a fixed point number
   static constexpr bool is_fixed = false;
   //! Accumulator datatype
   typedef float16_t accumulator;

   /**
    * @brief      One value
    *
    * @return     One value in f16
    */
   static constexpr float16_t one() {return  ((float16_t)1.0f);};

   //! Compute datatype
   typedef _Float16 compute_type;
};


#if !defined(ARM_MATH_MVE_FLOAT16)
/**
 * @brief      float16 vector descrition when no vector architecture
 */
template<>
struct vector_traits<float16_t> {
  //! Float16 datatype
  typedef float16_t type;
  //! Float16 storage type
  typedef float16_t storage_type;

  // No vector type but must still be defined
  //! Dummy type when no vector instruction is supported
  typedef bool vector;
  //! Dummy type when no vector instruction is supported
  typedef bool temp_accumulator;
  //! Dummy type when no vector instruction is supported
  typedef uint32_t predicate_t;

  //! No vector instruction
  static constexpr bool has_vector = false;
  //! Is float
  static constexpr bool is_float = true;
  //! Not fixed point
  static constexpr bool is_fixed = false;
  //! Has predicated loop
  static constexpr bool has_predicate = false;
};
#endif 

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericNumber
 */
namespace inner {
   /**
    * @brief      Convert from accumulator datatype
    *
    * @param[in]  a     Value
    *
    * @return     Converted from accumulator datatype
    */
   __STATIC_FORCEINLINE float16_t from_accumulator(const float16_t a)
   {
     return(a);
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
  __STATIC_FORCEINLINE float16_t mac(const float16_t acc,const float16_t a,const float16_t b)
  {
     return((_Float16)acc+(_Float16)a*(_Float16)b);
  };
  

/**
 * @brief      Accumulate
 *
 * @param      a     Accumulator
 * @param[in]  b     Value to accumulate
 */
__STATIC_FORCEINLINE void accumulate(float16_t &a,const float16_t &b)
{
   a += (_Float16)b;
}

/**
 * @brief      Multiply
 *
 * @param      a     First operand
 * @param[in]  b     Second operand
 *
 * @return     a*b
 */
__STATIC_FORCEINLINE float16_t mult(float16_t &a,const float16_t &b)
{
   return((_Float16)a*(_Float16)b);
}

}

#endif

/*! @} */
/*! @} */