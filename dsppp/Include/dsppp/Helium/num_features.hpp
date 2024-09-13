// -*- C++ -*-
/** @file */ 
#pragma once 


/*

vreduce is going from vector accumulator to scalar accumulator
from_accumulator is going from scalar accumulator to scalar datatype


*/
#if defined(ARM_MATH_MVEF)

template<>
struct ComplexVector<float32x4_t>
{
    explicit constexpr ComplexVector(float32x4_t ma,float32x4_t mb):va(ma),vb(mb){};
    explicit constexpr ComplexVector(float32x4_t ma):va(ma),vb{}{};
    explicit constexpr ComplexVector():va{},vb{}{};
    typedef float32x4_t type;
    float32x4_t va,vb;

    friend std::ostream& operator<< (std::ostream& stream, const ComplexVector<float32x4_t>& other) 
    {
        stream << "(" << other.va[0] << "," << other.va[1] << ") (" << other.va[2] << "," << other.va[3] << ") ";
        stream << "(" << other.vb[0] << "," << other.vb[1] << ") (" << other.vb[2] << "," << other.vb[3] << ") ";

        return(stream);
    };
};




template<>
struct HalfComplexVector<float32x4_t>
{
    explicit constexpr HalfComplexVector(float32x4_t ma):va(ma){};
    explicit constexpr HalfComplexVector():va{}{};
    typedef float32x4_t type;
    float32x4_t va;

    friend std::ostream& operator<< (std::ostream& stream, const HalfComplexVector<float32x4_t>& other) 
    {
        stream << "(" << other.va[0] << "," << other.va[1] << ") (" << other.va[2] << "," << other.va[3] << ") ";
        return(stream);
    };
};



#endif

#if defined(ARM_MATH_MVEI)

template<>
struct ComplexVector<int32x4_t>
{
    explicit constexpr ComplexVector(int32x4_t ma,int32x4_t mb):va(ma),vb(mb){};
    explicit constexpr ComplexVector(int32x4_t ma):va(ma),vb{}{};
    explicit constexpr ComplexVector():va{},vb{}{};
    typedef int32x4_t type;
    int32x4_t va,vb;

    friend std::ostream& operator<< (std::ostream& stream, const ComplexVector<int32x4_t>& other) 
    {
        stream << "(" << other.va[0] << "," << other.va[1] << ") (" << other.va[2] << "," << other.va[3] << ") ";
        stream << "(" << other.vb[0] << "," << other.vb[1] << ") (" << other.vb[2] << "," << other.vb[3] << ") ";

        return(stream);
    };
};

template<>
struct ComplexVector<int16x8_t>
{
    explicit constexpr ComplexVector(int16x8_t ma,int16x8_t mb):va(ma),vb(mb){};
    explicit constexpr ComplexVector(int16x8_t ma):va(ma),vb{}{};
    explicit constexpr ComplexVector():va{},vb{}{};
    typedef int16x8_t type;
    int16x8_t va,vb;

    friend std::ostream& operator<< (std::ostream& stream, const ComplexVector<int16x8_t>& other) 
    {
        stream << "(" 
               << other.va[0] << "," << other.va[1] << ") (" 
               << other.va[2] << "," << other.va[3] << ") (" 
               << other.va[4] << "," << other.va[5] << ") (" 
               << other.va[6] << "," << other.va[7] << ") ";
        stream << "(" 
               << other.vb[0] << "," << other.vb[1] << ") (" 
               << other.vb[2] << "," << other.vb[3] << ") (" 
               << other.vb[4] << "," << other.vb[5] << ") (" 
               << other.vb[6] << "," << other.vb[7] << ") ";

        return(stream);
    };
};

template<>
struct HalfComplexVector<int32x4_t>
{
    explicit constexpr HalfComplexVector(int32x4_t ma):va(ma){};
    explicit constexpr HalfComplexVector():va{}{};
    typedef int32x4_t type;
    int32x4_t va;

    friend std::ostream& operator<< (std::ostream& stream, const HalfComplexVector<int32x4_t>& other) 
    {
        stream << "(" << other.va[0] << "," << other.va[1] << ") (" << other.va[2] << "," << other.va[3] << ") ";
        return(stream);
    };
};


template<>
struct HalfComplexVector<int16x8_t>
{
    explicit constexpr HalfComplexVector(int16x8_t ma):va(ma){};
    explicit constexpr HalfComplexVector():va{}{};
    typedef int16x8_t type;
    int16x8_t va;

    friend std::ostream& operator<< (std::ostream& stream, const HalfComplexVector<int16x8_t>& other) 
    {
        stream << "(" << other.va[0] << "," << other.va[1] << ") (" 
                      << other.va[2] << "," << other.va[3] << ") ("
                      << other.va[4] << "," << other.va[5] << ") ("
                      << other.va[6] << "," << other.va[7] << ") ";
        return(stream);
    };
};

#endif

#if defined(ARM_MATH_MVE_FLOAT16)

template<>
struct ComplexVector<float16x8_t>
{
    explicit constexpr ComplexVector(float16x8_t ma,float16x8_t mb):va(ma),vb(mb){};
    explicit constexpr ComplexVector(float16x8_t ma):va(ma),vb{}{};
    explicit constexpr ComplexVector():va{},vb{}{};
    typedef float16x8_t type;
    float16x8_t va,vb;

    friend std::ostream& operator<< (std::ostream& stream, const ComplexVector<float16x8_t>& other) 
    {
        stream << "(" << (float)other.va[0] << "," << (float)other.va[1] << ") (" 
                      << (float)other.va[2] << "," << (float)other.va[3] << ") (" 
                      << (float)other.va[4] << "," << (float)other.va[5] << ") ("
                      << (float)other.va[6] << "," << (float)other.va[7] << ") ";

        stream << "(" << (float)other.vb[0] << "," << (float)other.vb[1] << ") (" 
                      << (float)other.vb[2] << "," << (float)other.vb[3] << ") (" 
                      << (float)other.vb[4] << "," << (float)other.vb[5] << ") ("
                      << (float)other.vb[6] << "," << (float)other.vb[7] << ") ";
        return(stream);
    };
};



template<>
struct HalfComplexVector<float16x8_t>
{
    explicit constexpr HalfComplexVector(float16x8_t ma):va(ma){};
    explicit constexpr HalfComplexVector():va{}{};
    typedef float16x8_t type;
    float16x8_t va;

    friend std::ostream& operator<< (std::ostream& stream, const HalfComplexVector<float16x8_t>& other) 
    {
        stream << "(" << (float)other.va[0] << "," << (float)other.va[1] << ") (" 
                      << (float)other.va[2] << "," << (float)other.va[3] << ") (" 
                      << (float)other.va[4] << "," << (float)other.va[5] << ") ("
                      << (float)other.va[6] << "," << (float)other.va[7] << ") ";
        return(stream);
    };
};

#endif

namespace inner {

template<typename T,int ...S>
struct vload1_gen_stride;

template<typename T,int ...S>
struct vload1_gen_stride_z;

template<typename T,int ...S>
struct vstore1_gen_stride;

template<typename T,int ...S>
struct vstore1_gen_stride_z;

template<typename T,int ...S>
struct vload1_half_gen_stride;

template<typename VA, typename VB>
struct ToComplexStrideComp;

template<int ...D, int H>
struct ToComplexStrideComp<std::integer_sequence<int,D...>, std::integer_sequence<int,H>>
{
    using type = typename ToComplexStrideComp<std::integer_sequence<int,D...,2*H,2*H+1>,std::integer_sequence<int>>::type;
};

template<int ...D, int H, int ...R>
struct ToComplexStrideComp<std::integer_sequence<int,D...>, std::integer_sequence<int,H,R...>>
{
    using type = typename ToComplexStrideComp<std::integer_sequence<int,D...,2*H,2*H+1>,std::integer_sequence<int,R...>>::type;
};

template<int ...D>
struct ToComplexStrideComp<std::integer_sequence<int,D...>, std::integer_sequence<int>>
{
    using type = std::integer_sequence<int,D...>;
};

/*
Convert a sequence of stride a,b,c ... to

2*a, 2*a+1, 2*b, 2*b+1, 2*c, 2*c+1 ...
*/
template<int...D>
struct ToComplexStride
{
   using type = typename ToComplexStrideComp<std::integer_sequence<int>,
                                             std::integer_sequence<int,D...>>::type;
};

template<int NB,typename VA, typename VB>
struct TakeDropComp;

template<int ...VA, int ...VB>
struct TakeDropComp<0,std::integer_sequence<int,VA...>,std::integer_sequence<int,VB...>>
{
    using la = std::integer_sequence<int,VA...>;
    using lb = std::integer_sequence<int,VB...>;
};

template<int NB,int...H, int E, int ...R>
struct TakeDropComp<NB,std::integer_sequence<int,H...>,std::integer_sequence<int,E,R...>>
{
    using type = TakeDropComp<NB-1,std::integer_sequence<int,H...,E>,std::integer_sequence<int,R...>>;
    using la = typename type::la;
    using lb = typename type::lb;
};

/*

Split a sequence of length 2*NB a ... x ... into two sequences of length NB
a...
and
x...

*/
template<int NB,int...D>
struct TakeDrop
{
   using type = typename TakeDropComp<NB,std::integer_sequence<int>,
                                         std::integer_sequence<int,D...>>::type;

   using la = typename type::la;
   using lb = typename type::lb;
};


}

#include "float.hpp"
#include "complex_float.hpp"

#include "half.hpp"
#include "complex_half.hpp"

#include "q31.hpp"
#include "complex_q31.hpp"

#include "q15.hpp"
#include "complex_q15.hpp"

#include "q7.hpp"
