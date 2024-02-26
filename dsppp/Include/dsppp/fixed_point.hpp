// -*- C++ -*-
/** @file */ 
#pragma once 

#include <cstdint>
#include "arch.hpp"
#include <cstdlib>

#include <type_traits>
#include <iostream>
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


constexpr bool test64(const int M,const int F,const int S){return((M+F+S)>32 && (M+F+S)<=64);}
constexpr bool test32(const int M,const int F,const int S){return((M+F+S)>16 && (M+F+S)<=32);}
constexpr bool test16(const int M,const int F,const int S){return((M+F+S)>8  && (M+F+S)<=16);}
constexpr bool test8 (const int M,const int F,const int S){return((M+F+S)<=8);}

template<int M,int F,bool s = true,bool = true>
struct fixed_storage_type
{
};

template<int M,int F>
struct fixed_storage_type<M,F,true,test64(M,F,1)>
{
    typedef int64_t value_type;
    typedef int64_t wider_type;
    typedef int32_t narrow_type;
};

template<int M,int F>
struct fixed_storage_type<M,F,false,test64(M,F,0)>
{
    typedef uint64_t value_type;
    typedef uint64_t wider_type;
    typedef uint32_t narrow_type;
};


template<int M,int F>
struct fixed_storage_type<M,F,true,test32(M,F,1)>
{
    typedef int32_t value_type;
    typedef int64_t wider_type;
    typedef int16_t narrow_type;
};

template<int M,int F>
struct fixed_storage_type<M,F,false,test32(M,F,0)>
{
    typedef uint32_t value_type;
    typedef uint64_t wider_type;
    typedef uint16_t narrow_type;
};



template<int M,int F>
struct fixed_storage_type<M,F, true,test16(M,F,1)>
{
    typedef int16_t value_type;
    typedef int32_t wider_type;
    typedef int8_t narrow_type;
};

template<int M,int F>
struct fixed_storage_type<M,F, false,test16(M,F,0)>
{
    typedef uint16_t value_type;
    typedef uint32_t wider_type;
    typedef uint8_t narrow_type;
};


template<int M,int F>
struct fixed_storage_type<M,F,true,test8(M,F,1)>
{
    typedef int8_t value_type;
    typedef int16_t wider_type;
    typedef int8_t narrow_type;
};

template<int M,int F>
struct fixed_storage_type<M,F,false,test8(M,F,0)>
{
    typedef uint8_t value_type;
    typedef uint16_t wider_type;
    typedef uint8_t narrow_type;
};


template<int M, int F, bool S = true,
         typename T=typename fixed_storage_type<M,F,S>::value_type>
struct Q {};

template<int M,int F>
struct Q<M,F,true,int64_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = true;
  using value_type = typename fixed_storage_type<M,F,true>::value_type;
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;
  constexpr static value_type maxVal = 0x7FFFFFFFFFFFFFFFLL;
  constexpr static value_type minVal = 0x8000000000000000LL;


  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((maxVal >> (63 - F)) ))));
  };

  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;


  constexpr explicit Q(const Q<M,F,false>&other) 
    :v{value_type(other.v)} {};

   bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }


   Q & operator+=(const Q other)
   {
       v += other.v;
       return(*this);
   }

   Q & operator-=(const Q other)
   {
       v -= other.v;
       return(*this);
   }


   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / (maxVal >> (63 - F))) << "_Q(" << M << "," << F << ")";;
        return(stream);
   }

};

template<int M,int F>
struct Q<M,F,false,uint64_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = false;
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;
  constexpr static value_type maxVal = 0xFFFFFFFFFFFFFFFFLL;
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0LL : value_type(f * (float)((maxVal >> (64 - F))))));
  };
  
  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / (maxVal >> (64 - F))) << "_UQ(" << M << "," << F << ")";;
        return(stream);
   }

   bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

};


template<int M,int F>
struct Q<M,F,true,int32_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = true;
  using value_type = typename fixed_storage_type<M,F,true>::value_type;
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;
  constexpr static value_type maxVal = 0x7FFFFFFFL;
  constexpr static value_type minVal = 0x80000000L;
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : (i<(value_type)minVal ? minVal : i));
  };

  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((wider_type)1<<F))));
  };
  
  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  constexpr explicit Q(const Q<M,F,false>&other):
  v{value_type(other.v)} {};

  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

   Q & operator+=(const Q other)
   {
       v = __QADD(v,other.v);
       return(*this);
   }

   Q & operator-=(const Q other)
   {
       v = __QSUB(v,other.v);
       return(*this);
   }

   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_Q(" << M << "," << F << ")";;
        return(stream);
   }

};


template<int M,int F>
struct Q<M,F,false,uint32_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = false;
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;
  constexpr static value_type maxVal = 0xFFFFFFFFL;
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : i);
  };
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0 : value_type(f * (float)((wider_type)1<<F))));
  };
  
  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;
 
   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_UQ(" << M << "," << F << ")";;
        return(stream);
   }

   bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }
};

template<int M,int F>
struct Q<M,F,true,int16_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = true;
  using value_type = typename fixed_storage_type<M,F,true>::value_type;
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;
  constexpr static value_type maxVal = 0x7FFF;
  constexpr static value_type minVal = 0x8000;
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : (i<(value_type)minVal ? minVal : i));
  };
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((wider_type)1<<F))));
  };
  
  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  constexpr explicit Q(const Q<M,F,false>&other):v{value_type(other.v)} {};

  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

   Q & operator+=(const Q other)
   {
    #if !defined(ARM_MATH_DSP)
      v = __SSAT((value_type)v + other.v,16);
    #else 
      v = (value_type) __QADD16(v, other.v);
    #endif
    return(*this);
   }

   Q & operator-=(const Q other)
   {
    #if !defined(ARM_MATH_DSP)
       v = __SSAT((value_type)v - other.v,16);
    #else 
       v = (value_type) __QSUB16(v, other.v);
    #endif
    return(*this);
   }

   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / (((wider_type)1)<<F)) << "_Q(" << M << "," << F << ")";;
        return(stream);
   }

};


template<int M,int F>
struct Q<M,F,false,uint16_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = false;
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;
  constexpr static value_type maxVal = 0xFFFF;
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : i);
  };
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0 : value_type(f * (float)((wider_type)1<<F))));
  };
  
  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

   Q & operator+=(const Q other)
   {
    v = __USAT((value_type)v + other.v,16);
    return(*this);
   }



   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_UQ(" << M << "," << F << ")";;
        return(stream);
   }

};

template<int M,int F>
struct Q<M,F,true,int8_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = true;
  using value_type = typename fixed_storage_type<M,F,true>::value_type;
  using wider_type = typename fixed_storage_type<M,F,true>::wider_type;
  constexpr static value_type maxVal = 0x7F;
  constexpr static value_type minVal = 0x80;
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : (i<(value_type)minVal ? minVal : i));
  };
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= -1.0f ? minVal : value_type(f * (float)((wider_type)1<<F))));
  };
  
  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  constexpr explicit Q(const Q<M,F,false>&other):v{value_type(other.v)} {};

  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }

   Q & operator+=(const Q other)
   {
     #if !defined(ARM_MATH_DSP)
        v = __SSAT((value_type)v + other.v,8);
     #else 
        v = (value_type) __QADD8(v, other.v);
     #endif
    return(*this);
   }

   Q & operator-=(const Q other)
   {
     #if !defined(ARM_MATH_DSP)
         v = __SSAT((value_type)v + other.v,8);
     #else 
         v = (value_type) __QSUB8(v, other.v);
     #endif
    return(*this);
   }

   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_Q(" << M << "," << F << ")";
        return(stream);
   }

};

template<int M,int F>
struct Q<M,F,false,uint8_t> {
  constexpr static int fracBits = F;
  constexpr static int mantissaBits = M;
  constexpr static bool isSigned = false;
  using value_type = typename fixed_storage_type<M,F,false>::value_type;
  using wider_type = typename fixed_storage_type<M,F,false>::wider_type;
  constexpr static value_type maxVal = 0xFF;
  constexpr static value_type sat(const wider_type i) {
    return (i > (value_type)maxVal ? maxVal : i);
  };
  constexpr static value_type convert(const float f) {
    return(f >= 1.0f ? maxVal : (f <= 0.0f ? 0 : value_type(f * (float)((wider_type)1<<F))));
  };
  
  value_type v;
  constexpr Q():v(0){};
  constexpr explicit Q(const value_type x):v(x){};
  constexpr static Q f(const float x){return Q(convert(x));}

  constexpr static Q one() {return f(1.0f);};

  Q(Q&& other)=default;
  Q(const Q& other)=default;
  Q& operator=(Q&& other)=default;
  Q& operator=(const Q& other)=default;

  bool operator==(const Q& b) const
   {
     return(v == b.v);
   }

   bool operator!=(const Q& b) const
   {
     return(v != b.v);
   }

   bool operator<(const Q& b) const
   {
     return(v < b.v);
   }

   bool operator>(const Q& b) const
   {
     return(v > b.v);
   }

   bool operator<=(const Q& b) const
   {
     return(v <= b.v);
   }

   bool operator>=(const Q& b) const
   {
     return(v >= b.v);
   }
   
   Q & operator+=(const Q other)
   {
    v = __USAT((value_type)v + other.v,8);
    return(*this);
   }

   friend std::ostream& operator<< (std::ostream& stream, const Q& other) {
        stream << double(1.0*other.v / ((wider_type)1<<F)) << "_UQ(" << M << "," << F << ")";
        return(stream);
   }

};

using Q63 = Q<0,63>;
using Q31 = Q<0,31>;
using Q15 = Q<0,15>;
using Q7  = Q<0,7>;

constexpr Q63 operator ""_q63(long double x){return Q63(Q63::convert((float)x));}
constexpr Q31 operator ""_q31(long double x){return Q31(Q31::convert((float)x));}
constexpr Q15 operator ""_q15(long double x){return Q15(Q15::convert((float)x));}
constexpr Q7 operator ""_q7(long double x){return Q7(Q7::convert((float)x));}



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
inline Q<M,F,S> operator+(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    Q<M,F,S> ret(a);
    ret+=b;
    return ret;
}

template<int M,int F,bool S>
inline Q<M,F,S> operator-(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    Q<M,F,S> ret(a);
    ret-=b;
    return ret;
}

template<int M,int F,bool S>
inline Q<M,F,S> operator-(const Q<M,F,S> &a)
{
    Q<M,F,S> ret;
    ret-=a;
    return ret;
}

// Unsaturating add
template<int M,int F,bool S>
inline Q<M,F,S> add(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    return Q<M,F,S>(a.v + b.v);
}

// Unsaturating sub
template<int M,int F,bool S>
inline Q<M,F,S> sub(const Q<M,F,S> &a,const Q<M,F,S> &b)
{
    return Q<M,F,S>(a.v - b.v);
}


template<int N>
constexpr std::integral_constant<int, N> i_{};

template<int M,int F, int N,bool S>
inline Q<M,F,S> operator >>(const Q<M,F,S> &a, std::integral_constant<int, N>) noexcept {
    return Q<M,F,S>(a.v >> N);
}

template<int M,int F,int N,bool S>
inline Q< M+N , F,S> operator <<(const Q<M,F,S> &a,  std::integral_constant<int, N>) noexcept {
    using ResType = typename Q<M+N,F,S>::value_type;
    return Q<M+N,F,S>(ResType(a.v) << N);
}

template<int MD=0,int MS,int F>
inline Q<MD,F,true> saturate(const Q<MS,F,true> &src,
                             typename std::enable_if<(MD < MS) && ((MD+F)<31)>::type* = nullptr)
{
    return(Q<MD,F,true>(__SSAT(src.v, MD+F+1)));
}


template<int MD=0,int MS,int F>
inline Q<MD,F,false> saturate(const Q<MS,F,false> &src,typename std::enable_if<(MD < MS) && ((MD+F)<31)>::type* = nullptr)
{
    return(Q<MD,F,false>(__USAT(src.v, MD+F+1)));
}


template<int M,int FD,int FS,bool S,bool = true>
struct FixedCastShift {};

/* Positive shift */
template<int M,int FD,int FS,bool S>
struct FixedCastShift<M,FD,FS,S,(FD>FS)> {
    constexpr static Q<M,FD,S> shift(const Q<M,FS,S> &src)
    {
        using DstType = typename Q<M,FD,S>::value_type;
        return(Q<M,FD,S>(DstType(src.v) << (FD-FS)));
    }
};

template<int M,int FD,int FS,bool S>
struct FixedCastShift<M,FD,FS,S,(FD<FS)> {
    constexpr static Q<M,FD,S> shift(const Q<M,FS,S> &src)
    {
        using DstType = typename Q<M,FD,S>::value_type;
        using SrcType = typename Q<M,FS,S>::value_type;

        return(Q<M,FD,S>(DstType(SrcType(src.v) >> (FS-FD))));
    }
};

template<int FD,int M,int FS,bool S>
inline Q<M,FD,S> toFrac(const Q<M,FS,S> &src)
{
    return(FixedCastShift<M,FD,FS,S>::shift(src));
}


template<int MD,int MS,int F,bool S,bool = true>
struct Accumulate;

template<int MD,int MS,int F,bool S>
struct Accumulate<MD,MS,F,S,true> {
  static Q<MD,F,S> acc (const Q<MD,F,S> &a,const Q<MS,F,S> &b)
  {
     using DstType = typename Q<MD,F,S>::value_type;
     return(Q<MD,F,S>(DstType(a.v) + DstType(b.v)));
  }
};

template<int MD,int MS,int F,bool S>
inline Q<MD,F,S> accumulate(const Q<MD,F,S> &a,const Q<MS,F,S> &b)
{
   return(Accumulate<MD,MS,F,S,(MD>MS)>::acc(a,b));
}


template<int M,int F>
inline Q<M,F,true> _abs(const Q<M,F,true> a)
{
    using DestType = typename Q<M,F,true>::value_type;
    return(Q<M,F>(DestType(abs(a.v))));
}

inline Q7 operator*(const Q7 &a,const Q7 &b)
{
    return(saturate(toFrac<7>(mult(a,b))));
}

inline Q15 operator*(const Q15 &a,const Q15 &b)
{
    return (saturate(toFrac<15>(mult(a,b))));
}

inline Q31 operator*(const Q31 &a,const Q31 &b)
{
    return (toFrac<31>(saturate(toFrac<30>(mult(a,b)))));
}

template<int M,int F>
inline bool operator>(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v>b.v);
}

template<int M,int F>
inline bool operator<(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v<b.v);
}

template<int M,int F>
inline bool operator>=(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v>=b.v);
}


template<int M,int F>
inline bool operator<=(const Q<M,F> &a,const Q<M,F> &b)
{
    return(a.v<=b.v);
}

template<int M,int F>
inline bool operator==(const Q<M,F> a,const Q<M,F> b)
{
    return(a.v==b.v);
}

template<int M,int F>
inline bool operator!=(const Q<M,F> a,const Q<M,F> b)
{
    return(a.v!=b.v);
}


template<int M,int F,bool S>
inline Q<M,F,S> operator/(const Q<M,F,S> a,const int32_t b)
{
    return(Q<M,F,S>(a.v / b));
}


template<int M,int F, bool S>
inline Q<M,F,S> operator+(const Q<M,F,S> &a)
{
    return(a);
}

/*! @} */

}