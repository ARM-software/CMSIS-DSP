// -*- C++ -*-
/** @file */ 
#pragma once 

#include <memory>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "common.hpp"
#include "arch.hpp"
#include <type_traits>
#include "number.hpp"
#include "forward.hpp"
#include "fusion.hpp"
#include "unroll.hpp"
#include "algorithms.hpp"
#include "vector_impl.hpp"
#include "vector_view.hpp"

namespace arm_cmsis_dsp {

/** \addtogroup VECTOR Vectors
 *  \ingroup DSPPP
 *  @{
 */

template<typename T,bool = true>
struct VecRef;

template<typename T>
struct VecRef<Vector_Base<T>>
{
   typedef VectorView<T,1> type;
   static type ref(const Vector_Base<T>&a){
      return(type(a));
   };
};

template<typename T,int S>
struct VecRef<VectorView<T,S>>
{
   typedef VectorView<T,S> type;
   static type ref(const VectorView<T,S>&a){
      return(a);
   };
};


template<typename P,int L,
         template<int> typename A>
struct VecRef<Vector<P,L,A>,(L<0)>
{
  
   typedef VectorView<P,1> type;
   static VectorView<P,1> ref(const Vector<P,L,A>&a,typename std::enable_if<(L<0)>::type* = nullptr){
      return(VectorView<P,1>(a));
   };

};

template<typename P,int L,
         template<int> typename A>
struct VecRef<Vector<P,L,A>,(L>0)>
{
   typedef const Vector<P,L,A>& type;
   static const Vector<P,L,A>& ref(const Vector<P,L,A>&a,typename std::enable_if<(L>0)>::type* = nullptr){
      return(a);
   };
};



template<typename LHS,typename RHS,typename OP>
struct VecRef<_Binary<LHS,RHS,OP>>
{
   typedef _Binary<LHS,RHS,OP> type;
   static type ref(const _Binary<LHS,RHS,OP>&a){
      return(a);
   };
};

template<typename LHS,typename OP>
struct VecRef<_Unary<LHS,OP>>
{
   typedef _Unary<LHS,OP> type;
   static type ref(const _Unary<LHS,OP>&a){
      return(a);
   };
};

template<typename Derived>
struct VecRef<_Expr<Derived>>
{
   typedef Derived type;
   static type ref(const _Expr<Derived>&a){
      return(a.derived());
   };
};

template<>
struct VecRef<double>
{
   typedef double type;
   static type ref(const double a){
      return(a);
   };
};

template<>
struct VecRef<float>
{
   typedef float type;
   static type ref(const float a){
      return(a);
   };
};

#if defined(ARM_FLOAT16_SUPPORTED)
template<>
struct VecRef<float16_t>
{
   typedef float16_t type;
   static type ref(const float16_t a){
      return(a);
   };
};
#endif

template<>
struct VecRef<Q7>
{
   typedef Q7 type;
   static type ref(const Q7 a){
      return(a);
   };
};

template<>
struct VecRef<Q15>
{
   typedef Q15 type;
   static type ref(const Q15 a){
      return(a);
   };
};

template<>
struct VecRef<Q31>
{
   typedef Q31 type;
   static type ref(const Q31 a){
      return(a);
   };
};


template<typename T,int S>
struct traits<VectorView<T,S>>
{
    typedef T Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<T>::vector Vector;
#endif
};


template<typename P,int L,
         template<int> typename Allocator>
struct traits<Vector<P,L,Allocator>>
{
    typedef P Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<P>::vector Vector;
#endif
};


template<typename P,int L,
         template<int> typename Allocator>
struct traits<const Vector<P,L,Allocator>&>
{
    typedef P Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<P>::vector Vector;
#endif
};



template<typename T>
struct StaticStride
{
    constexpr static std::size_t value = 1;
};

template<typename T,int S>
struct StaticStride<VectorView<T,S>>
{
    constexpr static std::size_t value = S;
};


template<typename P,int L,
         template<int> typename Allocator>
struct IsVector<Vector<P,L,Allocator>>
{
    constexpr static bool value = true;
};

template<typename P,int L,
         template<int> typename Allocator>
struct IsVector<const Vector<P,L,Allocator>&>
{
    constexpr static bool value = true;
};

template<typename P>
struct IsVector<const Vector_Base<P>&>
{
    constexpr static bool value = true;
};

template<typename P>
struct IsVector<Vector_Base<P>>
{
    constexpr static bool value = true;
};

template<typename P,int L,
         template<int> typename Allocator>
struct ElementType<Vector<P,L,Allocator>>
{
    typedef P type;
};

template<typename P,int L,
         template<int> typename Allocator>
struct ElementType<const Vector<P,L,Allocator>&>
{
    typedef P type;
};


template<typename P>
struct ElementType<Vector_Base<P>>
{
    typedef P type;
};

template<typename P>
struct ElementType<const Vector_Base<P>&>
{
    typedef P type;
};

template<typename T,int stride>
struct IsVector<VectorView<T,stride>>
{
    constexpr static bool value = true;
};


template<typename P,int L,
         template<int> typename Allocator>
struct StaticLength<Vector<P,L,Allocator>>
{
    constexpr static vector_length_t value = (L<0) ? 0 : L;
};

template<typename P,int L,
         template<int> typename Allocator>
struct StaticLength<const Vector<P,L,Allocator>&>
{
    constexpr static vector_length_t value = (L<0) ? 0 : L;
};


template<typename T,int stride>
struct ElementType<VectorView<T,stride>>
{
    typedef T type;
};

template<typename T,int stride>
struct ElementType<const VectorView<T,stride>&>
{
    typedef T type;
};


template<typename P,int L,
         template<int> typename Allocator>
struct IsDynamic<Vector<P,L,Allocator>>
{
    constexpr static bool value = (L<0);
};

template<typename P,int L,
         template<int> typename Allocator>
struct IsDynamic<const Vector<P,L,Allocator>&>
{
    constexpr static bool value = (L<0);
};

template<typename T,int stride>
struct IsDynamic<VectorView<T,stride>>
{
    constexpr static bool value = true;
};




// Assume one at least is static
template<typename VA,typename VB>
using StaticType=typename std::conditional<IsDynamic<VA>::value,VB,VA>::type;



/**
 * @brief  Addition operator for expressions
 *
 * @tparam LHS Left hand side datatype
 * @tparam RHS Right hand side datatype
 * @param a Left hand side expression tree
 * @param b Right hand side expression tree
 * @return Expression representing the add
 * 
 * vector + vector (including matrix)
 */
template<typename LHS,typename RHS,
typename std::enable_if<(!is_scalar<LHS>() || 
                        !is_scalar<RHS>()) && 
                        SameElementType<LHS,RHS>::value && 
                        same_static_length<LHS,RHS>(),bool>::type = true>
inline auto operator+(const LHS &a,const RHS &b)
{ 
    using Scalar = typename traits<LHS>::Scalar;
    using VecLHS = VecRef<LHS>;
    using VecRHS = VecRef<RHS>;

    return(_Binary<typename VecLHS::type,typename VecRHS::type,_AddOp<Scalar>>(VecLHS::ref(a),VecRHS::ref(b),_AddOp<Scalar>()));
};


/**
 * @brief  + operator for expressions
 *
 * @tparam LHS Left hand side datatype
 * @param a Left hand side expression tree
 * @return Expression representing + vector
 * 
 * +vector  (including matrix)
 */
template<typename LHS,
typename std::enable_if<!is_scalar<LHS>(),bool>::type = true>
inline auto operator+(const LHS &a)
{ 
    using Scalar = typename traits<LHS>::Scalar;
    using VecLHS = VecRef<LHS>;

    return(_Unary<typename VecLHS::type,_NoOp<Scalar>>(VecLHS::ref(a),_NoOp<Scalar>()));
};


/*

VectorView = VectorView must be a cheap copy of reference only.
So when we want to copy a VectorView onto another we need to
write
VectorView = expr(VectorView) or copy

we cannot rely on the copy or move constructors.

*/

/**
 * @brief  Identity operator for expression
 *
 * @tparam LHS Left hand side datatype
 * @param a Left hand side expression tree
 * @return Expression representing the identity
 * 
 * Used to consider a vector view as an expression and force the copy
 * of this vector view when assigned to another vector entity.
 * 
 */
template<typename LHS,
typename std::enable_if<!is_scalar<LHS>(),bool>::type = true>
inline auto expr(const LHS &a)
{ 
    using Scalar = typename traits<LHS>::Scalar;
    using VecLHS = VecRef<LHS>;
    return(_Unary<typename VecLHS::type,_NoOp<Scalar>>(VecLHS::ref(a),_NoOp<Scalar>()));
};

/**
 * @brief  Identity operator for expression
 *
 * @tparam LHS Left hand side datatype
 * @param a Left hand side expression tree
 * @return Expression representing the identity
 * 
 * Used to consider a vector view as an expression and force the copy
 * of this vector view when assigned to another vector entity.
 * 
 */
template<typename LHS,
typename std::enable_if<!is_scalar<LHS>(),bool>::type = true>
inline auto copy(const LHS &a)
{ 
    using Scalar = typename traits<LHS>::Scalar;
    using VecLHS = VecRef<LHS>;
    return(_Unary<typename VecLHS::type,_NoOp<Scalar>>(VecLHS::ref(a),_NoOp<Scalar>()));
};


/**
 * @brief  Subtraction operator for expressions
 *
 * @tparam LHS Left hand side datatype
 * @tparam RHS Right hand side datatype
 * @param a Left hand side expression tree
 * @param b Right hand side expression tree
 * @return Expression representing the add
 * 
 * vector - vector (including matrix)
 */
template<typename LHS,typename RHS,
typename std::enable_if<(!is_scalar<LHS>() || 
                        !is_scalar<RHS>()) && 
                        SameElementType<LHS,RHS>::value && 
                        same_static_length<LHS,RHS>(),bool>::type = true>
inline auto operator-(const LHS &a,const RHS &b)
{ 
    using Scalar = typename traits<LHS>::Scalar;
    using VecLHS = VecRef<LHS>;
    using VecRHS = VecRef<RHS>;

    return(_Binary<typename VecLHS::type,typename VecRHS::type,_SubOp<Scalar>>(
        VecLHS::ref(a),VecRHS::ref(b),_SubOp<Scalar>()));
};


/**
 * @brief  - operator for expressions
 *
 * @tparam LHS Left hand side datatype
 * @param a Left hand side expression tree
 * @return Expression representing the - vector
 * 
 * -vector (including matrix)
 */
template<typename LHS,
typename std::enable_if<!is_scalar<LHS>(),bool>::type = true>
inline auto operator-(const LHS &a)
{ 
    using Scalar = typename traits<LHS>::Scalar;
    using VecLHS = VecRef<LHS>;

    return(_Unary<typename VecLHS::type,_NegOp<Scalar>>(VecLHS::ref(a),_NegOp<Scalar>()));
};


/**
 * @brief  Element wise multiplication operator for expressions
 *
 * @tparam LHS Left hand side datatype
 * @tparam RHS Right hand side datatype
 * @param a Left hand side expression tree
 * @param b Right hand side expression tree
 * @return Expression representing the *
 * 
 * elementwise vector * vector (including matrix)
 */
template<typename LHS,typename RHS,
typename std::enable_if<(!is_scalar<LHS>() || 
                        !is_scalar<RHS>())  && 
                        SameElementType<LHS,RHS>::value && 
                        same_static_length<LHS,RHS>(),bool>::type = true>
inline auto operator*(const LHS &a,const RHS &b)
{ 
    using Scalar = typename traits<LHS>::Scalar;
    using VecLHS = VecRef<LHS>;
    using VecRHS = VecRef<RHS>;

    return(_Binary<typename VecLHS::type,typename VecRHS::type,_MulOp<Scalar>>(
        VecLHS::ref(a),VecRHS::ref(b),_MulOp<Scalar>()));
};



#if 0
template<typename VA,typename VB,typename OP,
std::enable_if<IsVector<VA>::value && 
               IsVector<VB>::value && 
               SameElementType<VA,VB>::value && 
               (same_static_length(StaticLength<VA>::value , StaticLength<VB>::value)),bool>::type = true>
inline _Expr<OP> operator+(const VA &a,
                           const VB &b)  
{ 
    
    return(_Add<decltype(pA),decltype(pB)>(a,b));
};
#endif

/*

Core algorithms that cannot be expressed only with high level
abstractions and need intrinsincs.

*/
#include "Helium/matrix_multiply.hpp"
#include "DSP/matrix_multiply.hpp"
#include "Scalar/matrix_multiply.hpp"

/*! @} */

}
