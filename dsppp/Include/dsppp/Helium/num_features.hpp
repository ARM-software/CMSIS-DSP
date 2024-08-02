// -*- C++ -*-
/** @file */ 
#pragma once 

#include <iostream>


/*

vreduce is going from vector accumulator to scalar accumulator
from_accumulator is going from scalar accumulator to scalar datatype


*/


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


namespace inner {

template<typename T,int ...S>
struct vload1_gen_stride;

template<typename T,int ...S>
struct vload1_gen_stride_z;

template<typename T,int ...S>
struct vstore1_gen_stride;

template<typename T,int ...S>
struct vstore1_gen_stride_z;

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

template<int NB,int...D>
struct TakeDrop
{
   using type = typename TakeDropComp<NB,std::integer_sequence<int>,
                                         std::integer_sequence<int,D...>>::type;

   using la = typename type::la;
   using lb = typename type::lb;
};


}

#include "complex_float.hpp"
#include "float.hpp"
#include "half.hpp"
#include "q31.hpp"
#include "q15.hpp"
#include "q7.hpp"
