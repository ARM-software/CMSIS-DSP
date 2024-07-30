// -*- C++ -*-
/** @file */ 
#pragma once 

#include <iostream>

/*

vreduce is going from vector accumulator to scalar accumulator
from_accumulator is going from scalar accumulator to scalar datatype


*/

/*

Must have 4 complex to be able to do mixed
complex / real operations

*/
template<>
struct ComplexVector<float32x4_t>
{
    explicit constexpr ComplexVector(float32x4_t m):v(m){};
    explicit constexpr ComplexVector():v{}{};
    typedef float32x4_t type;
    float32x4_t v;

    friend std::ostream& operator<< (std::ostream& stream, const ComplexVector<float32x4_t>& other) 
    {
        stream << "(" << other.v[0] << "," << other.v[1] << ") (" << other.v[2] << "," << other.v[3] << ") ";
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
}

#include "complex_float.hpp"
#include "float.hpp"
#include "half.hpp"
#include "q31.hpp"
#include "q15.hpp"
#include "q7.hpp"
