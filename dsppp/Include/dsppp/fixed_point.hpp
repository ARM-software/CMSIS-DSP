// -*- C++ -*-
/** @file */ 
#pragma once 

#include <cstdint>
#include "arch.hpp"
#include <complex>
#include <cmath>
#include <cstdlib>

#include <type_traits>
namespace arm_cmsis_dsp {

/** \addtogroup FIXED Fixed point datatypes
 *  \ingroup DSPPP
 *  @{
 */

/*

Normally those kind of definitions are in a compiler file
in Core or Core_A.

But for MSVC compiler it is a bit special. The goal is very specific
to CMSIS-DSP and only to allow the use of this library from other
systems like Python or Matlab.

MSVC is not going to be used to cross-compile to ARM. So, having a MSVC
compiler file in Core or Core_A would not make sense.

*/
#if defined ( _MSC_VER ) || defined(__GNUC_PYTHON__) || defined(__APPLE_CC__)
__STATIC_FORCEINLINE uint8_t __CLZ(uint32_t data)
{
  if (data == 0U) { return 32U; }

  uint32_t count = 0U;
  uint32_t mask = 0x80000000U;

  while ((data & mask) == 0U)
  {
    count += 1U;
    mask = mask >> 1U;
  }
  return count;
}

__STATIC_FORCEINLINE int32_t __SSAT(int32_t val, uint32_t sat)
{
  if ((sat >= 1U) && (sat <= 32U))
  {
    const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);
    const int32_t min = -1 - max ;
    if (val > max)
    {
      return max;
    }
    else if (val < min)
    {
      return min;
    }
  }
  return val;
}

__STATIC_FORCEINLINE uint32_t __USAT(int32_t val, uint32_t sat)
{
  if (sat <= 31U)
  {
    const uint32_t max = ((1U << sat) - 1U);
    if (val > (int32_t)max)
    {
      return max;
    }
    else if (val < 0)
    {
      return 0U;
    }
  }
  return (uint32_t)val;
}
#endif

#if !defined(ARM_MATH_DSP)
__STATIC_FORCEINLINE int32_t clip_int64_to_q31(
  int64_t x)
  {
    return ((int32_t) (x >> 32) != ((int32_t) x >> 31)) ?
      ((0x7FFFFFFF ^ ((int32_t) (x >> 63)))) : (int32_t) x;
  }

__STATIC_FORCEINLINE int32_t __QADD(
  int32_t x,
  int32_t y)
  {
    return ((int32_t)(clip_int64_to_q31((int64_t)x + (int32_t)y)));
  }


  /*
   * @brief C custom defined QSUB
   */
  __STATIC_FORCEINLINE int32_t __QSUB(
  int32_t x,
  int32_t y)
  {
    return ((int32_t)(clip_int64_to_q31((int64_t)x - (int32_t)y)));
  }


#endif

/**
 * @brief  Function to identify the template for fixed number 
 *         representable on 64 bits
 *
 * @param M number of mantissa bit (without sign bit)
 * @param F number of fractional bits
 * @param S sign or unsigned
 * @return True if the template must be selected
 */
constexpr bool test64(const int M,const int F,const int S){return((M+F+S)>32 && (M+F+S)<=64);}

/**
 * @brief  Function to identify the template for fixed number 
 *         representable on 32 bits
 *
 * @param M number of mantissa bit (without sign bit)
 * @param F number of fractional bits
 * @param S sign or unsigned
 * @return True if the template must be selected
 */
constexpr bool test32(const int M,const int F,const int S){return((M+F+S)>16 && (M+F+S)<=32);}

/**
 * @brief  Function to identify the template for fixed number 
 *         representable on 16 bits
 *
 * @param M number of mantissa bit (without sign bit)
 * @param F number of fractional bits
 * @param S sign or unsigned
 * @return True if the template must be selected
 */
constexpr bool test16(const int M,const int F,const int S){return((M+F+S)>8  && (M+F+S)<=16);}

/**
 * @brief  Function to identify the template for fixed number 
 *         representable on 8 bits
 *
 * @param M number of mantissa bit (without sign bit)
 * @param F number of fractional bits
 * @param S sign or unsigned
 * @return True if the template must be selected
 */
constexpr bool test8 (const int M,const int F,const int S){return((M+F+S)<=8);}

/**
 * @brief  Storage type for a fixed point number
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * @tparam s signed or unsigned
 * 
 */
template<int M,int F,bool s = true,bool = true>
struct fixed_storage_type
{
};


/**
 * @brief  Storage type for a fixed point number representable on int64
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F,true,test64(M,F,1)>
{
    //! Storage for the fixed point number
    typedef int64_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef int64_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef int32_t narrow_type;
};

/**
 * @brief  Storage type for a fixed point number representable on uint64
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F,false,test64(M,F,0)>
{
    //! Storage for the fixed point number
    typedef uint64_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef uint64_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef uint32_t narrow_type;
};


/**
 * @brief  Storage type for a fixed point number representable on int32
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F,true,test32(M,F,1)>
{
    //! Storage for the fixed point number
    typedef int32_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef int64_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef int16_t narrow_type;
};

/**
 * @brief  Storage type for a fixed point number representable on uint32
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F,false,test32(M,F,0)>
{
    //! Storage for the fixed point number
    typedef uint32_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef uint64_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef uint16_t narrow_type;
};


/**
 * @brief  Storage type for a fixed point number representable on int16
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F, true,test16(M,F,1)>
{
    //! Storage for the fixed point number
    typedef int16_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef int32_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef int8_t narrow_type;
};

/**
 * @brief  Storage type for a fixed point number representable on uint16
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F, false,test16(M,F,0)>
{
    //! Storage for the fixed point number
    typedef uint16_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef uint32_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef uint8_t narrow_type;
};

/**
 * @brief  Storage type for a fixed point number representable on int8
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F,true,test8(M,F,1)>
{
    //! Storage for the fixed point number
    typedef int8_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef int16_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef int8_t narrow_type;
};

/**
 * @brief  Storage type for a fixed point number representable on uint8
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct fixed_storage_type<M,F,false,test8(M,F,0)>
{
    //! Storage for the fixed point number
    typedef uint8_t value_type;
    //! Storage for the widening of this fixed point number datatype
    typedef uint16_t wider_type;
    //! Storage for the narrowing of this fixed point number datatype
    typedef uint8_t narrow_type;
};


/**
 * @brief  Fixed point template
 *
 * @tparam M Number of bits for mantissa (without sign bit)#
 * @tparam F Number of bits for fractional part
 * @tparam S Signed or unsigned
 * @tparam T storage datatype
 * 
 */
template<int M, int F, bool S = true,
         typename T=typename fixed_storage_type<M,F,S>::value_type>
struct Q {};

/**
 * @brief  Signed fixed point datatypes on 64 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,true,int64_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;
  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;
  //! Is this number representation signed
  constexpr static bool isSigned = true;
  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,true>::value_type;
  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;

  //! Maximum representable positive value
  constexpr static value_type maxVal = 0x7FFFFFFFFFFFFFFFLL;

  //! Minimum representable negative value
  constexpr static value_type minVal = 0x8000000000000000LL;

  /**
   * @brief  Convert a float to fixed point
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((maxVal >> (63 - F)) ))));
  };

  //! Storage value
  value_type v;

  /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

  /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

  /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;


  /**
   * @brief  Convert an unsigned fixed point to this fixed point
   * @param other The unsigned fixed point number
   * 
   * Some problem may occur since the unsigned may not be representable
   * with the less bits required for the sign representation.
   * This conversion is not saturating.
   */
  constexpr explicit Q(const Q<M,F,false>&other) 
    :v{value_type(other.v)} {};

   /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
   bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }


   /**
   * @brief  this += other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator+=(const Q other)
   {
       v += other.v;
       return(*this);
   }

   /**
   * @brief  this -= other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator-=(const Q other)
   {
       v -= other.v;
       return(*this);
   }


   /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / (maxVal >> (63 - F))) << "_Q(" << M << "," << F << ")";;
        return(stream);
   }

};

/**
 * @brief  Unsigned fixed point datatypes on 64 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,false,uint64_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;
  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;
  //! Is this number representation signed
  constexpr static bool isSigned = false;
  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;
  //! Maximum representable positive value
  constexpr static value_type maxVal = 0xFFFFFFFFFFFFFFFFLL;

  /**
   * @brief  Convert a float to fixed point
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0LL : value_type(f * (float)((maxVal >> (64 - F))))));
  };
  
  //! Storage value
  value_type v;

  /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

   /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

  /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

    /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / (maxVal >> (64 - F))) << "_UQ(" << M << "," << F << ")";;
        return(stream);
   }

   /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
   bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

    /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

};

/**
 * @brief  Signed fixed point datatypes on 32 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,true,int32_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;

  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;

  //! Is this number representation signed
  constexpr static bool isSigned = true;

  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,true>::value_type;

  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;

  //! Maximum representable positive value
  constexpr static value_type maxVal = 0x7FFFFFFFL;

  //! Minimum representable negative value
  constexpr static value_type minVal = 0x80000000L;

  /**
   * @brief  Saturate a wider type to the current type
   *
   * @param i the wider integer type
   * @return the saturated value
   * 
   */
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : (i<(value_type)minVal ? minVal : i));
  };

  /**
   * @brief  Convert a float to fixed point with saturation
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((wider_type)1<<F))));
  };
  
  //! Storage value
  value_type v;

  /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

  /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

   /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  /**
   * @brief  Convert an unsigned fixed point to this fixed point
   * @param other The unsigned fixed point number
   * 
   * Some problem may occur since the unsigned may not be representable
   * with the less bits required for the sign representation.
   * This conversion is not saturating.
   */
  constexpr explicit Q(const Q<M,F,false>&other):
  v{value_type(other.v)} {};

  /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

   /**
   * @brief  this += other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator+=(const Q other)
   {
       v = __QADD(v,other.v);
       return(*this);
   }

   
   /**
   * @brief  this -= other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator-=(const Q other)
   {
       v = __QSUB(v,other.v);
       return(*this);
   }

    /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_Q(" << M << "," << F << ")";;
        return(stream);
   }

};

/**
 * @brief  Unsigned fixed point datatypes on 32 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,false,uint32_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;
  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;
  //! Is this number representation signed
  constexpr static bool isSigned = false;
  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;

  //! Maximum representable positive value
  constexpr static value_type maxVal = 0xFFFFFFFFL;

  /**
   * @brief  Saturate a wider type to the current type
   *
   * @param i the wider integer type
   * @return the saturated value
   * 
   */
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : i);
  };

  /**
   * @brief  Convert a float to fixed point with saturation
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0 : value_type(f * (float)((wider_type)1<<F))));
  };
  
  //! Storage value
  value_type v;

   /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

  /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

  /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;
 
   /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_UQ(" << M << "," << F << ")";;
        return(stream);
   }

   /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
   bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

    /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }
};

/**
 * @brief  Signed fixed point datatypes on 16 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,true,int16_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;
  
  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;

  //! Is this number representation signed
  constexpr static bool isSigned = true;

  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,true>::value_type;

  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;

  //! Maximum representable positive value
  constexpr static value_type maxVal = 0x7FFF;

  //! Minimum representable negative value
  constexpr static value_type minVal = 0x8000;

   /**
   * @brief  Saturate a wider type to the current type
   *
   * @param i the wider integer type
   * @return the saturated value
   * 
   */
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : (i<(value_type)minVal ? minVal : i));
  };

  /**
   * @brief  Convert a float to fixed point with saturation
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((wider_type)1<<F))));
  };
  
  //! Storage value
  value_type v;

  /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

  /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

   /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  /**
   * @brief  Convert an unsigned fixed point to this fixed point
   * @param other The unsigned fixed point number
   * 
   * Some problem may occur since the unsigned may not be representable
   * with the less bits required for the sign representation.
   * This conversion is not saturating.
   */
  constexpr explicit Q(const Q<M,F,false>&other):v{value_type(other.v)} {};

   /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
   bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

  /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

    /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

    /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

    /**
   * @brief  this += other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator+=(const Q other)
   {
    #if !defined(ARM_MATH_DSP)
      v = __SSAT((value_type)v + other.v,16);
    #else 
      v = (value_type) __QADD16(v, other.v);
    #endif
    return(*this);
   }

   /**
   * @brief  this -= other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator-=(const Q other)
   {
    #if !defined(ARM_MATH_DSP)
       v = __SSAT((value_type)v - other.v,16);
    #else 
       v = (value_type) __QSUB16(v, other.v);
    #endif
    return(*this);
   }

   /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / (((wider_type)1)<<F)) << "_Q(" << M << "," << F << ")";;
        return(stream);
   }

};

/**
 * @brief  Unsigned fixed point datatypes on 16 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,false,uint16_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;
  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;
  //! Is this number representation signed
  constexpr static bool isSigned = false;
  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;
  //! Maximum representable positive value
  constexpr static value_type maxVal = 0xFFFF;

  /**
   * @brief  Saturate a wider type to the current type
   *
   * @param i the wider integer type
   * @return the saturated value
   * 
   */
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : i);
  };

  /**
   * @brief  Convert a float to fixed point with saturation
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0 : value_type(f * (float)((wider_type)1<<F))));
  };
  
  //! Storage value
  value_type v;

  /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

  /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

   /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

    /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

    /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

   /**
   * @brief  this += other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator+=(const Q other)
   {
    v = __USAT((value_type)v + other.v,16);
    return(*this);
   }


   /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_UQ(" << M << "," << F << ")";;
        return(stream);
   }

};

/**
 * @brief  Signed fixed point datatypes on 8 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,true,int8_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;
  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;
  //! Is this number representation signed
  constexpr static bool isSigned = true;
  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,true>::value_type;
  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;
  //! Maximum representable positive value
  constexpr static value_type maxVal = 0x7F;
  //! Minimum representable negative value
  constexpr static value_type minVal = 0x80;

   /**
   * @brief  Saturate a wider type to the current type
   *
   * @param i the wider integer type
   * @return the saturated value
   * 
   */
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : (i<(value_type)minVal ? minVal : i));
  };

  
  /**
   * @brief  Convert a float to fixed point with saturation
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((wider_type)1<<F))));
  };
  
  //! Storage value
  value_type v;

   /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

   /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

   /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  /**
   * @brief  Convert an unsigned fixed point to this fixed point
   * @param other The unsigned fixed point number
   * 
   * Some problem may occur since the unsigned may not be representable
   * with the less bits required for the sign representation.
   * This conversion is not saturating.
   */
  constexpr explicit Q(const Q<M,F,false>&other):v{value_type(other.v)} {};

  /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

    /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

    /**
   * @brief  this += other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator+=(const Q other)
   {
     #if !defined(ARM_MATH_DSP)
        v = __SSAT((value_type)v + other.v,8);
     #else 
        v = (value_type) __QADD8(v, other.v);
     #endif
    return(*this);
   }

  /**
   * @brief  this -= other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator-=(const Q other)
   {
     #if !defined(ARM_MATH_DSP)
         v = __SSAT((value_type)v + other.v,8);
     #else 
         v = (value_type) __QSUB8(v, other.v);
     #endif
    return(*this);
   }

    /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_Q(" << M << "," << F << ")";
        return(stream);
   }

};

/**
 * @brief  Unsigned fixed point datatypes on 8 bits
 *
 * @tparam M Number of bits for mantissa (without sign bit)
 * @tparam F Number of bits for fractional part
 * 
 */
template<int M,int F>
struct Q<M,F,false,uint8_t> {
  //! Number of fractional bits
  constexpr static int fracBits = F;
  //! Number of mantissa bits (without sign bit)
  constexpr static int mantissaBits = M;
  //! Is this number representation signed
  constexpr static bool isSigned = false;
  //! Storage type for the value
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  //! Storage type for the widening of the value
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;

  //! Maximum representable positive value
  constexpr static value_type maxVal = 0xFF;

  /**
   * @brief  Saturate a wider type to the current type
   *
   * @param i the wider integer type
   * @return the saturated value
   * 
   */
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : i);
  };

  /**
   * @brief  Convert a float to fixed point with saturation
   *
   * @param f float value
   * @return the fixed point value in the storage type
   * 
   */
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0 : value_type(f * (float)((wider_type)1<<F))));
  };
  
  //! Storage value
  value_type v;

  /**
   * @brief  Create a new zero fixed point
   */
  constexpr Q():v(0){};

  /**
   * @brief  Create a new fixed point from a raw integer
   * @param x the raw integer
   */
  constexpr explicit Q(const value_type x):v(x){};

   /**
   * @brief  Create a new fixed point from a float
   * @param x the float
   * @return The fixed point representing the float value with saturation
   */
  constexpr static Q f(const float x){return Q(convert(x));}

  /**
   * @brief  Fixed point number representing 1
   * @return Fixed point representing 1
   */
  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  /**
    * @brief  this == b
    * @param b the other fixed point
    * @return true if this == b
    */
  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   /**
    * @brief  this != b
    * @param b the other fixed point
    * @return true if this != b
    */
   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   /**
    * @brief  this < b
    * @param b the other fixed point
    * @return true if this < b
    */
   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   /**
    * @brief  this > b
    * @param b the other fixed point
    * @return true if this > b
    */
   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   /**
    * @brief  this <= b
    * @param b the other fixed point
    * @return true if this <= b
    */
   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   /**
    * @brief  this >= b
    * @param b the other fixed point
    * @return true if this >= b
    */
   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }
   
   /**
   * @brief  this += other
   * @param other the other fixed point
   * @return true if this += other
   */
   Q & operator+=(const Q other)
   {
    v = __USAT((value_type)v + other.v,8);
    return(*this);
   }

    /**
    * @brief  Display fixed point number for debug purpose
    * @param stream Output stream
    * @param other The fixed point to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_UQ(" << M << "," << F << ")";
        return(stream);
   }

};
    

//! Q63 datatype
using Q63 = Q<0,63>;

//! Q31 datatype
using Q31 = Q<0,31>;

//! Q15 datatype
using Q15 = Q<0,15>;

//! Q7 datatype
using Q7  = Q<0,7>;

/**
 * @brief q63 literal
 * @param x long double value
 * @return Q63 value
 * 
 * You can write 
 * \code{.cpp}
 * Q63 b = 0.4_q63;
 * \endcode
 * 
 * The float is converted to Q63 at build time.
 * 
 */
constexpr Q63 operator ""_q63(long double x){return Q63(Q63::convert((float)x));}

/**
 * @brief q31 literal
 * @param x long double value
 * @return Q31 value
 * 
 * You can write 
 * \code{.cpp}
 * Q31 b = 0.4_q31;
 * \endcode
 * 
 * The float is converted to Q31 at build time.
 * 
 */
constexpr Q31 operator ""_q31(long double x){return Q31(Q31::convert((float)x));}

/**
 * @brief q15 literal
 * @param x long double value
 * @return Q15 value
 * 
 * You can write 
 * \code{.cpp}
 * Q15 b = 0.4_q15;
 * \endcode
 * 
 * The float is converted to Q15 at build time.
 * 
 */
constexpr Q15 operator ""_q15(long double x){return Q15(Q15::convert((float)x));}

/**
 * @brief q7 literal
 * @param x long double value
 * @return Q7 value
 * 
 * You can write 
 * \code{.cpp}
 * Q7 b = 0.4_q7;
 * \endcode
 * 
 * The float is converted to Q7 at build time.
 * 
 */
constexpr Q7 operator ""_q7(long double x){return Q7(Q7::convert((float)x));}
}

namespace std {

template<int M,int F,bool S>
class complex<arm_cmsis_dsp::Q<M,F,S>>
{

public:
    typedef arm_cmsis_dsp::Q<M,F,S> value_type;
    using integer = typename value_type::value_type;
private:
  value_type __re_;
  value_type __im_;
public:
    constexpr complex(const value_type& re = value_type(), const value_type& im = value_type())
    : __re_(re), __im_(im) {};

    /* Create complex from raw ints */
    constexpr explicit complex(const integer& re, const integer& im)
    : __re_(value_type(re)), __im_(value_type(im)) {};

    constexpr explicit complex(const integer& re)
    : __re_(value_type(re)), __im_(value_type{}) {};

    template<class X> constexpr complex(const complex<X>& __c)
    : __re_(__c.real()), __im_(__c.imag()) {};

    constexpr value_type real() const { return __re_; }
    constexpr value_type imag() const { return __im_; }

    constexpr void real(value_type __re) { __re_ = __re; }
    constexpr void imag(value_type __im) { __im_ = __im; }


    constexpr complex& operator=(const value_type& __re) {
      __re_ = __re;
      __im_ = value_type();
      return *this;
    }

    constexpr complex& operator+=(const value_type& __re) {
       __re_ += __re;
       return *this;
     }

    constexpr complex& operator-=(const value_type& __re) {
    __re_ -= __re;
    return *this;
  }

  constexpr complex& operator*=(const value_type& __re) {
    __re_ = __re_ * __re;
    __im_ = __im_ * __re;
    return *this;
  }
  

  template <class _Xp>
  constexpr complex& operator=(const complex<_Xp>& __c) {
    __re_ = __c.real();
    __im_ = __c.imag();
    return *this;
  }
  template <class _Xp>
  constexpr complex& operator+=(const complex<_Xp>& __c) {
    __re_ += __c.real();
    __im_ += __c.imag();
    return *this;
  }
  template <class _Xp>
  constexpr complex& operator-=(const complex<_Xp>& __c) {
    __re_ -= __c.real();
    __im_ -= __c.imag();
    return *this;
  }


  template <class _Xp>
  constexpr complex& operator*=(const complex<_Xp>& __c) {
    *this = *this * complex(__c.real(), __c.imag());
    return *this;
  }


};



}

namespace arm_cmsis_dsp {

/**
 * @brief Multiplication of two fixed point numbers A and B
 * @tparam MA Number of mantissa bits for A
 * @tparam FA Number of fractional bits for A
 * @tparam MB Number of mantissa bits for B
 * @tparam FB Number of fractional bits for B
 * @tparam SA Is A using a signed representation
 * @tparam SB Is B using a signed representation
 * @param a First fixed point number
 * @param b Second fixed point number
 * @return return the product of the two fixed point (and use adapted type)
 * 
 * 
 */
template<int MA,int FA,int MB,int FB,bool SA,bool SB>
inline Q< MA+MB+1 , FA+FB,SA || SB> mult(const Q<MA,FA,SA> &a,
                                         const Q<MB,FB,SB> &b)
{
    /*

    Why mantissa of result is MA + MB + 1.
    If we take as example, Q7 * Q7 and we multiply
    0x80 * 0x80 (-128 * -128) we get 0x4000 and if we shift right by 7
    we get 0x080 (on 9 bits). If the additional mantissa bit was not 
    kept, we would get 0x80 (on 8 bits) which would mean a negative 
    number.

    Saturation of 0x080 (on 9 bits) will give 0x7F whereas 
    saturation of 0x80 (on 8 bits) would keep 0x80 and thus 
    the wrong sign.

    By using MA + MB + 1 we ensure that Q7 * Q7 is Q<1,14>
    and not Q<0,14>.

    To convert Q<1,14> to Q<0,7> we need a toFract and a saturate.

    */
    using ResType = typename Q< MA+MB+1 , FA+FB,SA || SB>::value_type;
    ResType res = ((ResType)a.v * (ResType)b.v);
    return(Q<MA+MB+1,FA+FB,SA || SB>(res));
}

template<int M,int F,bool S>
inline std::complex<Q< M+M+1 , F+F,S>> 
       mult(const std::complex<Q<M,F,S>> &a,
            const std::complex<Q<M,F,S>> &b)
{
  Q< M+M+1 , F+F,S> re,im;
  re = mult(a.real(),b.real()) - mult(a.imag(),b.imag());
  im = mult(a.real(),b.imag()) + mult(a.imag(),b.real());
  return(std::complex<Q< M+M+1 , F+F,S>>(re,im));
}

/**
 * @brief Add two fixed point numbers with saturation
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam F Number of fractional bits for the fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param a First fixed point number
 * @param b Second fixed point number
 * @return return the sum with saturation (if supported by the datatype)
 * 
 * 
 */
template<int M,int F,bool S>
inline Q<M,F,S> operator+(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    Q<M,F,S> ret(a);
    ret+=b;
    return ret;
}

/**
 * @brief Subtract two fixed point numbers with saturation
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam F Number of fractional bits for the fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param a First fixed point number
 * @param b Second fixed point number
 * @return return the subtraction with saturation (if supported by the datatype)
 * 
 * 
 */
template<int M,int F,bool S>
inline Q<M,F,S> operator-(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    Q<M,F,S> ret(a);
    ret-=b;
    return ret;
}

/**
 * @brief Negate a fixed point number with saturation
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam F Number of fractional bits for the fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param a First fixed point number
 * @return return negation with saturation (if supported by the datatype)
 * 
 * 
 */
template<int M,int F,bool S>
inline Q<M,F,S> operator-(const Q<M,F,S> &a)
{
    Q<M,F,S> ret;
    ret-=a;
    return ret;
}

// Unsaturating add
/**
 * @brief Add two fixed point numbers without saturation
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam F Number of fractional bits for the fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param a First fixed point number
 * @param b Second fixed point number
 * @return return the sum without saturation
 * 
 * 
 */
template<int M,int F,bool S>
inline Q<M,F,S> add(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    return Q<M,F,S>(a.v + b.v);
}

// Unsaturating sub
/**
 * @brief Subtract two fixed point numbers without saturation
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam F Number of fractional bits for the fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param a First fixed point number
 * @param b Second fixed point number
 * @return return the subtraction  without saturation
 * 
 * 
 */
template<int M,int F,bool S>
inline Q<M,F,S> sub(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    return Q<M,F,S>(a.v - b.v);
}


template<int N>
constexpr std::integral_constant<int, N> i_{};

/**
 * @brief Shift right a fixed point number with a shift known at build time
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam F Number of fractional bits for the fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param a First fixed point number
 * @return return the shifted fixed point number
 * 
 * 
 */
template<int M,int F, int N,bool S>
inline Q<M,F,S> operator >>(const Q<M,F,S> &a, std::integral_constant<int, N>) noexcept {
    return Q<M,F,S>(a.v >> N);
}


/**
 * @brief Shift left a fixed point number with a shift known at build time
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam F Number of fractional bits for the fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param a First fixed point number
 * @return return the shifted fixed point number
 * 
 * 
 */
template<int M,int F,int N,bool S>
inline Q< M+N , F,S> operator <<(const Q<M,F,S> &a,  std::integral_constant<int, N>) noexcept {
    using ResType = typename Q<M+N,F,S>::value_type;
    return Q<M+N,F,S>(ResType(a.v) << N);
}


/**
 * @brief Saturate a signed fixed point number
 * @tparam MD Number of mantissa bits for the destination fixed point number
 * @tparam MS Number of mantissa bits for the source fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param src First fixed point number
 * @return return the saturated fixed point number
 * 
 * Only applies if the number is signed, the representation requires less
 * than 32 bits (since there is no saturating instruction for 64 bits) and
 * if destination has less mantissa bits.
 * 
 * If destination has more or equal number of mantissa bits then it does
 * not make sense to saturate.
 */
template<int MD=0,int MS,int F>
inline Q<MD,F,true> saturate(const Q<MS,F,true> &src,
                             typename std::enable_if<(MD < MS) && ((MD+F)<31)>::type* = nullptr)
{
    return(Q<MD,F,true>(__SSAT(src.v, MD+F+1)));
}

template<int MD=0,int MS,int F>
inline std::complex<Q<MD,F,true>> saturate(const std::complex<Q<MS,F,true>> &src,
                             typename std::enable_if<(MD < MS) && ((MD+F)<31)>::type* = nullptr)
{
    return(std::complex<Q<MD,F,true>>(
      __SSAT(src.real().v, MD+F+1),__SSAT(src.imag().v, MD+F+1)
      )
    );
}


/**
 * @brief Saturate an unsigned fixed point number
 * @tparam MD Number of mantissa bits for the destination fixed point number
 * @tparam MS Number of mantissa bits for the source fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param src The fixed point number
 * @return return the saturated fixed point number
 * 
 * Only applies if the number is unsigned, the representation requires less
 * than 31 bits (since there is no saturating instruction for 64 bits) and
 * if destination has less mantissa bits.
 * 
 * If destination has more or equal number of mantissa bits then it does
 * not make sense to saturate.
 */
template<int MD=0,int MS,int F>
inline Q<MD,F,false> saturate(const Q<MS,F,false> &src,typename std::enable_if<(MD < MS) && ((MD+F)<31)>::type* = nullptr)
{
    return(Q<MD,F,false>(__USAT(src.v, MD+F+1)));
}

template<int MD=0,int MS,int F>
inline std::complex<Q<MD,F,false>> saturate(const std::complex<Q<MS,F,false>> &src,typename std::enable_if<(MD < MS) && ((MD+F)<31)>::type* = nullptr)
{
    return(std::complex<Q<MD,F,false>>(
      __USAT(src.real().v, MD+F+1),__USAT(src.imag().v, MD+F+1)
      ));
}

template<int M,int FD,int FS,bool S,bool = true>
struct FixedCastShift {};

/* Positive shift */

/**
 * @brief Changed fractional representation of a fixed point number using a shift
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam FD Number of fractional bits for the destination fixed point number
 * @tparam FS Number of fractional bits for the source fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param src The fixed point number
 * @return return the fixed point number with different fractional part format
 *
 * Only applies if FD > FS
 */
template<int M,int FD,int FS,bool S>
struct FixedCastShift<M,FD,FS,S,(FD>FS)> {
    constexpr static Q<M,FD,S> shift(const Q<M,FS,S> &src)
    {
        using DstType = typename Q<M,FD,S>::value_type;
        return(Q<M,FD,S>(DstType(src.v) << (FD-FS)));
    }
};

/**
 * @brief Changed fractional representation of a fixed point number using a shift
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam FD Number of fractional bits for the destination fixed point number
 * @tparam FS Number of fractional bits for the source fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param src The fixed point number
 * @return return the fixed point number with different fractional part format
 *
 * Only applies if FD < FS
 */
template<int M,int FD,int FS,bool S>
struct FixedCastShift<M,FD,FS,S,(FD<FS)> {
    constexpr static Q<M,FD,S> shift(const Q<M,FS,S> &src)
    {
        using DstType = typename Q<M,FD,S>::value_type;
        using SrcType = typename Q<M,FS,S>::value_type;

        return(Q<M,FD,S>(DstType(SrcType(src.v) >> (FS-FD))));
    }
};

/**
 * @brief Convert between different fractional part formats
 * @tparam M Number of mantissa bits for the fixed point number
 * @tparam FD Number of fractional bits for the destination fixed point number
 * @tparam FS Number of fractional bits for the source fixed point number
 * @tparam S Is the fixed point number using a signed representation
 * @param src The fixed point number
 * @return return the fixed point number with different fractional part format
 *
 */
template<int FD,int M,int FS,bool S>
inline Q<M,FD,S> toFrac(const Q<M,FS,S> &src)
{
    return(FixedCastShift<M,FD,FS,S>::shift(src));
}

template<int FD,int M,int FS,bool S>
inline std::complex<Q<M,FD,S>> toFrac(const std::complex<Q<M,FS,S>> &src)
{
    Q<M,FD,S> re,im;
    re = FixedCastShift<M,FD,FS,S>::shift(src.real());
    im = FixedCastShift<M,FD,FS,S>::shift(src.imag());

    return(std::complex<Q<M,FD,S>>(re,im));
}

/**
 * @brief Accumulation without saturation
 * @tparam MD Number of mantissa bits for the destination fixed point number
 * @tparam MS Number of mantissa bits for the source fixed point number
 * @tparam F Number of fractional bits for fixed point number
 * @tparam S Is the fixed point number using a signed representation
 *
 */
template<int MD,int MS,int F,bool S,bool = true>
struct Accumulate;

/**
 * @brief Accumulation without saturation
 * @tparam MD Number of mantissa bits for the destination fixed point number
 * @tparam MS Number of mantissa bits for the source fixed point number
 * @tparam F Number of fractional bits for fixed point number
 * @tparam S Is the fixed point number using a signed representation
 *
 */
template<int MD,int MS,int F,bool S>
struct Accumulate<MD,MS,F,S,true> {
  /**
   * @brief      Accumulation without saturation
   *
   * @param[in]  a     first fixed point number
   * @param[in]  b     second fixed point number
   *
   * @return     The sum of both fixed point number with more
   *             matissa bits.
   */
  static Q<MD,F,S> acc (const Q<MD,F,S> &a,const Q<MS,F,S> &b)
  {
     using DstType = typename Q<MD,F,S>::value_type;
     return(Q<MD,F,S>(DstType(a.v) + DstType(b.v)));
  }
};

/**
 * @brief      Accumulate without saturation
 *
 * @param[in]  a     First fixed point number
 * @param[in]  b     Second fixed point number
 *
 * @tparam     MD    Number of mantissa bits for destination
 * @tparam     MS    Number of mantissa bits for source
 * @tparam     F     Number of fractional bits
 * @tparam     S     Is the representation signed
 *
 * @return     Sum of two numbers without saturation and using the
 *             destination number of mantissa bits
 */
template<int MD,int MS,int F,bool S>
inline Q<MD,F,S> accumulate(const Q<MD,F,S> &a,const Q<MS,F,S> &b)
{
   return(Accumulate<MD,MS,F,S,(MD>MS)>::acc(a,b));
}

template<int MD,int MS,int F,bool S>
inline std::complex<Q<MD,F,S>> accumulate(const std::complex<Q<MD,F,S>> &a,
                                          const std::complex<Q<MS,F,S>> &b)
{
   Q<MD,F,S> re,im;
   re = accumulate(a.real(),b.real());
   im = accumulate(a.imag(),b.imag());

   return(std::complex<Q<MD,F,S>>(re,im));
}


template<int M,int F>
inline Q<M,F,true> _abs(const Q<M,F,true> a)
{
    using DestType = typename Q<M,F,true>::value_type;
    return(Q<M,F>(DestType(abs(a.v))));
}

/**
 * @brief      Multiplication operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @return     The result of the multiplication with saturation
 */
inline Q7 operator*(const Q7 &a,const Q7 &b)
{
    return(saturate(toFrac<7>(mult(a,b))));
}

/**
 * @brief      Multiplication operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @return     The result of the multiplication with saturation
 */
inline Q15 operator*(const Q15 &a,const Q15 &b)
{
    return (saturate(toFrac<15>(mult(a,b))));
}

/**
 * @brief      Multiplication operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @return     The result of the multiplication with saturation
 */
inline Q31 operator*(const Q31 &a,const Q31 &b)
{
    return (toFrac<31>(saturate(toFrac<30>(mult(a,b)))));
}

/**
 * @brief      Greater-than comparison operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 *
 * @return     The result of the greater-than comparison
 */
template<int M,int F>
inline bool operator>(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v>b.v);
}

/**
 * @brief      Less-than comparison operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 *
 * @return     The result of the less-than comparison
 */
template<int M,int F>
inline bool operator<(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v<b.v);
}

/**
 * @brief      Greater-than-or-equal comparison operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 *
 * @return     The result of the greater-than-or-equal comparison
 */
template<int M,int F>
inline bool operator>=(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v>=b.v);
}

/**
 * @brief      Less-than-or-equal comparison operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 *
 * @return     The result of the less-than-or-equal comparison
 */
template<int M,int F>
inline bool operator<=(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v<=b.v);
}

/**
 * @brief      Equality operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 *
 * @return     The result of the equality
 */
template<int M,int F>
inline bool operator==(const Q<M,F> a,const Q<M,F> b)
{
    return(a.v==b.v);
}

/**
 * @brief      Inequality operator.
 *
 * @param[in]  a     First value
 * @param[in]  b     Second value
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 *
 * @return     The result of the inequality
 */
template<int M,int F>
inline bool operator!=(const Q<M,F> a,const Q<M,F> b)
{
    return(a.v!=b.v);
}

/**
 * @brief      Division operator.
 *
 * @param[in]  a     First fixed point value
 * @param[in]  b     Integer
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 * @tparam     S     Is representation signed
 *
 * @return     The result of the division
 */
template<int M,int F,bool S>
inline Q<M,F,S> operator/(const Q<M,F,S> a,const int32_t b)
{
    return(Q<M,F,S>(a.v / b));
}

/**
 * @brief      No op operator.
 *  
 * @param[in]  a     Fixed point number
 *
 * @tparam     M     Number of mantissa bits
 * @tparam     F     Number of fractional bits
 * @tparam     S     Is the representation signed
 *
 * @return     The result of the addition
 */
template<int M,int F, bool S>
inline Q<M,F,S> operator+(const Q<M,F,S> &a)
{
    return(a);
}

/*! @} */

}

namespace std {
  template<int M,int F,bool S>
constexpr complex<arm_cmsis_dsp::Q<M,F,S>>
operator*(const complex<arm_cmsis_dsp::Q<M,F,S>>& __z, const complex<arm_cmsis_dsp::Q<M,F,S>>& __w) 
{
  arm_cmsis_dsp::Q<M,F,S> __a = __z.real();
  arm_cmsis_dsp::Q<M,F,S> __b = __z.imag();
  arm_cmsis_dsp::Q<M,F,S> __c = __w.real();
  arm_cmsis_dsp::Q<M,F,S> __d = __w.imag();

  arm_cmsis_dsp::Q<M,F,S> re,im;

  re = arm_cmsis_dsp::saturate(arm_cmsis_dsp::toFrac<F>(arm_cmsis_dsp::mult(__a,__c) - arm_cmsis_dsp::mult(__b,__d)));
  im = arm_cmsis_dsp::saturate(arm_cmsis_dsp::toFrac<F>(arm_cmsis_dsp::mult(__a,__d) + arm_cmsis_dsp::mult(__b,__c)));

  return(complex<arm_cmsis_dsp::Q<M,F,S>>(re,im));
};

static inline complex<arm_cmsis_dsp::Q31>
operator*(const complex<arm_cmsis_dsp::Q31>& __z, const complex<arm_cmsis_dsp::Q31>& __w) 
{
  arm_cmsis_dsp::Q31 __a = __z.real();
  arm_cmsis_dsp::Q31 __b = __z.imag();
  arm_cmsis_dsp::Q31 __c = __w.real();
  arm_cmsis_dsp::Q31 __d = __w.imag();

  arm_cmsis_dsp::Q31::wider_type re{},im{};

  re = arm_cmsis_dsp::toFrac<31>(arm_cmsis_dsp::mult(__a,__c) - arm_cmsis_dsp::mult(__b,__d)).v;
  im = arm_cmsis_dsp::toFrac<31>(arm_cmsis_dsp::mult(__a,__d) + arm_cmsis_dsp::mult(__b,__c)).v;

  return(complex<arm_cmsis_dsp::Q31>(re,im));
};

}

