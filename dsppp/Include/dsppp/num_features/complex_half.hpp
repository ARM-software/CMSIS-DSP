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
struct number_traits<std::complex<float16_t>>
{
   //! It is a float number
   static constexpr bool is_float = true;
   //! It is not a fixed point number
   static constexpr bool is_fixed = false;
   //! Accumulator datatype
   typedef std::complex<float16_t> accumulator;

   /**
    * @brief      One value
    *
    * @return     One value in f16
    */
   static constexpr std::complex<float16_t> one() {return  (std::complex<float16_t>(1.0f,0.0f));};

   //! Compute datatype
   typedef std::complex<_Float16> compute_type;

   //! Display type for printf 
   typedef std::complex<float> display_type;
};

/**
 * @brief      float16 vector descrition when no vector architecture
 */
template<typename arch>
struct vector_traits<std::complex<float16_t>,arch,void> {
  //! Float16 datatype
  typedef std::complex<float16_t> type;
  //! Float16 storage type
  typedef std::complex<float16_t> storage_type;

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
   __STATIC_FORCEINLINE std::complex<float16_t> from_accumulator(const std::complex<float16_t> a)
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
  __STATIC_FORCEINLINE std::complex<float16_t> mac(const std::complex<float16_t> acc,
                                                   const std::complex<float16_t> a,
                                                   const std::complex<float16_t> b)
  {
     float16_t re,im;
     re = (_Float16)acc.real() + (_Float16)a.real() * (_Float16)b.real() - (_Float16)a.imag() * (_Float16)b.imag();
     im = (_Float16)acc.imag() + (_Float16)a.real() * (_Float16)b.imag() + (_Float16)a.imag() * (_Float16)b.real();


     return(std::complex<float16_t>(re,im));
  };
  

/**
 * @brief      Accumulate
 *
 * @param      a     Accumulator
 * @param[in]  b     Value to accumulate
 */
__STATIC_FORCEINLINE void accumulate(std::complex<float16_t> &a,
                                     const std::complex<float16_t> &b)
{
   float16_t re,im;
   re = (_Float16)a.real() + (_Float16)b.real();
   im = (_Float16)a.imag() + (_Float16)b.imag();

   a = std::complex<float16_t>(re,im);
}

/**
 * @brief      Multiply
 *
 * @param      a     First operand
 * @param[in]  b     Second operand
 *
 * @return     a*b
 */
__STATIC_FORCEINLINE std::complex<float16_t> mult(const std::complex<float16_t> &a,
                                                  const std::complex<float16_t> &b)
{
   float16_t re,im;
   re = (_Float16)a.real() * (_Float16)b.real() - (_Float16)a.imag() * (_Float16)b.imag();
   im = (_Float16)a.real() * (_Float16)b.imag() + (_Float16)a.imag() * (_Float16)b.real();

   return(std::complex<float16_t>(re,im));
}

}

#endif

/*! @} */
/*! @} */