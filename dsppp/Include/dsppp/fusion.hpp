// -*- C++ -*-
/** @file */ 
#pragma once 

namespace arm_cmsis_dsp {

/** \addtogroup FUSION Abstract syntax tree for fusion
 *  \ingroup DSPPP
 *  @{
 */

template<typename T> struct traits
{
    typedef T Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<T>::vector Vector;
#endif
};

template<typename M>
struct Complexity
{
   constexpr static int value = 0;
};

/*

An unregular datatype has different strides like MatrixView
and can only be assigned to a MatrixView using a 2D functions.
So all normal eval functions will reject unregular structures.

*/
template<typename T>
struct HasMatrixIndexing
{
    constexpr static bool value = false;
};

template<typename T>
struct HasStaticStride
{
    constexpr static bool value = false;
};



template<typename T>
struct IsVector
{
    constexpr static bool value = false;
};

template<typename T>
struct IsMatrix
{
    constexpr static bool value = false;
};



template<typename T>
struct StaticLength
{
    constexpr static vector_length_t value = 0;
};

template<typename T>
struct ElementType
{
    typedef T type;
};


template<typename A,typename B>
using SameElementType=std::is_same<typename ElementType<A>::type,typename ElementType<B>::type>;

template<typename DA>
constexpr bool has_vector_inst() {return (vector_traits<typename ElementType<DA>::type>::has_vector);}

template<typename DA>
constexpr bool has_predicate_inst() {return (vector_traits<typename ElementType<DA>::type>::has_predicate);}

template<typename DA>
constexpr bool is_scalar() {return (!IsVector<DA>::value && 
                                    !HasMatrixIndexing<DA>::value);}

template<typename DA>
constexpr bool must_use_matrix_idx() {return (!IsVector<DA>::value && 
                                       HasMatrixIndexing<DA>::value);}
template<typename DA,typename DB>
constexpr bool vector_idx_pair() {return (IsVector<DA>::value && 
                                          IsVector<DB>::value &&
                                          SameElementType<DA,DB>::value);}

// By default scalar has no vector size so can't be used
// to infer a size at build time. They are considered as dynamic
// Otherwise, by default vectors are considered static 
// except is there is a specialization of this template
// (and that is the case for dynamic vectors)
template<typename T>
struct IsDynamic
{
    constexpr static bool value = is_scalar<T>();
};

/*

Vector only not including matrixes (which are also vectors)

*/
template<typename DA>
constexpr bool is_only_vector() {return (IsVector<DA>::value && 
                                    !HasMatrixIndexing<DA>::value);}

template<typename DA,typename DB>
constexpr bool must_use_matrix_idx_pair() {return ((must_use_matrix_idx<DA>() || must_use_matrix_idx<DB>()) &&
                                                 SameElementType<DA,DB>::value);}


/*

Static length is 0 for scalar and Dynamic vectors
*/
template<typename DA,typename DB>
constexpr vector_length_t static_length() {
    return ((StaticLength<DA>::value==0) ? StaticLength<DB>::value : StaticLength<DA>::value);
}

/*

False only when DA and DB are static vector and with differet size
Anyother case is ok.

*/

template<typename DA,typename DB>
constexpr bool same_static_length()
{
    return((StaticLength<DA>::value == 0) || /* Scalar or dynamic case */
           (StaticLength<DB>::value == 0) || /* Scalar or dynamic case */
           (StaticLength<DA>::value == StaticLength<DB>::value));
}
/*

Vector operators at instruction level

*/
#include "fusion_ops.hpp"


template<typename T>
struct _Expr {

    using Scalar = typename traits<T>::Scalar;
#if defined(HAS_VECTOR)
    using Vector = typename traits<T>::Vector;
#endif

  T& derived()  {return(static_cast<T&>(*this));}

  T const& derived() const {return(static_cast<T const&>(*this));}

  Scalar const operator[](const index_t i) const {return(this->derived()[i]);}
  
  Scalar const operator()(const index_t r,const index_t c) const {return(this->derived()(r,c));}

#if defined(HAS_VECTOR)
  Vector const vector_op(const index_t i) const {return(this->derived().vector_op(i));}
  
  Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const {return(this->derived().vector_op_tail(i,remaining));}

  Vector const matrix_op(const index_t r,const index_t c) const {return(this->derived().matrix_op(r,c));}
  
  Vector const matrix_op_tail(const index_t r,const index_t c,const vector_length_t remaining) const {return(this->derived().matrix_op_tail(r,c,remaining));}
#endif 

  vector_length_t length() const {return(this->derived().length());}
  vector_length_t rows() const {return(this->derived().rows());}
  vector_length_t columns() const {return(this->derived().columns());}

  virtual ~_Expr(){};

protected:
   _Expr() = default;
   _Expr(const _Expr&) = default;
   _Expr(_Expr&&) = default;
   _Expr& operator=(const _Expr& other) = delete;
   _Expr& operator=(_Expr&& other) = delete;
};

/*****************
 * 
 * BINARY AST
 */

template<typename LHS,typename RHS,typename DerivedOp>
struct _Binary: _Expr<_Binary<LHS,RHS,DerivedOp>>
{
    using Scalar = typename traits<LHS>::Scalar;
#if defined(HAS_VECTOR)
    using Vector = typename traits<LHS>::Vector;
#endif
    _Binary(const LHS &lhs,
            const RHS &rhs,
            const _BinaryOperator<Scalar,DerivedOp> &op):
            lhs_(lhs),rhs_(rhs),op_(op){
    }

    
    _Binary(const _Binary &other):
    lhs_(other.lhs_),rhs_(other.rhs_),op_(other.op_){
    }

    _Binary& operator=(const _Binary& other) = delete;
    _Binary& operator=(_Binary&& other) = delete;

    _Binary(_Binary &&other): 
    lhs_(std::move(other.lhs_)),rhs_(std::move(other.rhs_)),op_(std::move(other.op_))
    {
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<L>::value,bool>::type = true>
    vector_length_t length() const {
        return(lhs_.length());
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<!IsVector<L>::value && IsVector<R>::value,bool>::type = true>
    vector_length_t length() const {
        return(rhs_.length());
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value,bool>::type = true>
    vector_length_t rows() const {
        return(lhs_.rows());
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<!HasMatrixIndexing<L>::value && HasMatrixIndexing<R>::value,bool>::type = true>
    vector_length_t rows() const {
        return(rhs_.rows());
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value,bool>::type = true>
    vector_length_t columns() const {
        return(lhs_.columns());
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<!HasMatrixIndexing<L>::value && HasMatrixIndexing<R>::value,bool>::type = true>
    vector_length_t columns() const {
        return(rhs_.columns());
    }


   
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if< 
                        IsVector<L>::value && 
                        IsVector<R>::value,bool>::type = true>
    Scalar const operator[](const index_t i) const {
        return(op_(lhs_[i],rhs_[i]));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<L>::value && 
                       is_scalar<R>(),bool>::type = true>
    Scalar const operator[](const index_t i) const {
        return(op_(lhs_[i],rhs_));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<is_scalar<L>() && 
                        IsVector<R>::value,bool>::type = true>
    Scalar const operator[](const index_t i) const {
        return(op_(lhs_,rhs_[i]));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value && 
                        HasMatrixIndexing<R>::value,bool>::type = true>
    Scalar const operator()(const index_t r,const index_t c) const
    {
        return(op_(lhs_(r,c),rhs_(r,c)));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<is_scalar<L>() && 
                        HasMatrixIndexing<R>::value,bool>::type = true>
    Scalar const operator()(const index_t r,const index_t c) const
    {
        return(op_(lhs_,rhs_(r,c)));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value && 
                        is_scalar<R>(),bool>::type = true>
    Scalar const operator()(const index_t r,const index_t c) const
    {
        return(op_(lhs_(r,c),rhs_));
    }

#if defined(HAS_VECTOR)
    /* V + V */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if< 
                        IsVector<L>::value && 
                        IsVector<R>::value,bool>::type = true>
    Vector const vector_op(const index_t i) const
    {
        return(op_(lhs_.vector_op(i),rhs_.vector_op(i)));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if< 
                        has_predicate_inst<L>() &&
                        IsVector<L>::value && 
                        IsVector<R>::value,bool>::type = true>
    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(op_(lhs_.vector_op_tail(i,remaining),rhs_.vector_op_tail(i,remaining),inner::vctpq<Scalar>::mk(remaining)));
    }

    /* V + S */
    template<typename R=RHS, typename L=LHS,
            typename std::enable_if< 
                        IsVector<L>::value && 
                        is_scalar<R>(),bool>::type = true>
    Vector const vector_op(const index_t i) const
    {
        return(op_(lhs_.vector_op(i),rhs_));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if< 
                        has_predicate_inst<L>() &&
                        IsVector<L>::value && 
                        is_scalar<R>(),bool>::type = true>
    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(op_(lhs_.vector_op_tail(i,remaining),rhs_,inner::vctpq<Scalar>::mk(remaining)));
    }



    /* S + V */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<is_scalar<L>() && 
                        IsVector<R>::value,bool>::type = true>
    Vector const vector_op(const index_t i) const
    {
        return(op_(lhs_,rhs_.vector_op(i)));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if< 
                        has_predicate_inst<L>() &&
                        is_scalar<L>() && 
                        IsVector<R>::value,bool>::type = true>
    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(op_(lhs_,rhs_.vector_op_tail(i,remaining),inner::vctpq<Scalar>::mk(remaining)));
    }


    /*************
     * 
     * For matrix
     * 
     */

    /* V + V */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value && 
                        HasMatrixIndexing<R>::value,bool>::type = true>
    Vector const matrix_op(const index_t r,const index_t c) const
    {
        return(op_(lhs_.matrix_op(r,c),rhs_.matrix_op(r,c)));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<
                        has_predicate_inst<L>() &&
                        HasMatrixIndexing<L>::value && 
                        HasMatrixIndexing<R>::value,bool>::type = true>
    Vector const matrix_op_tail(const index_t r,const index_t c,const vector_length_t remaining) const
    {
        return(op_(lhs_.matrix_op_tail(r,c,remaining),rhs_.matrix_op_tail(r,c,remaining),inner::vctpq<Scalar>::mk(remaining)));
    }

    /* V + S */
    template<typename R=RHS, typename L=LHS,
            typename std::enable_if<HasMatrixIndexing<L>::value && 
                                    is_scalar<R>(),bool>::type = true>
    Vector const matrix_op(const index_t r,const index_t c) const
    {
        return(op_(lhs_.matrix_op(r,c),rhs_));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<has_predicate_inst<L>() &&
                                     HasMatrixIndexing<L>::value && 
                                     is_scalar<R>(),bool>::type = true>
    Vector const matrix_op_tail(const index_t r,const index_t c,const vector_length_t remaining) const
    {
        return(op_(lhs_.matrix_op_tail(r,c,remaining),rhs_,inner::vctpq<Scalar>::mk(remaining)));
    }



    /* S + V */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<is_scalar<L>() && 
                        HasMatrixIndexing<R>::value,bool>::type = true>
    Vector const matrix_op(const index_t r,const index_t c) const
    {
        return(op_(lhs_,rhs_.matrix_op(r,c)));
    }

    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<has_predicate_inst<L>() &&
                                     is_scalar<L>() && 
                                     HasMatrixIndexing<R>::value,bool>::type = true>
    Vector const matrix_op_tail(const index_t r,const index_t c,const vector_length_t remaining) const
    {
        return(op_(lhs_,rhs_.matrix_op_tail(r,c,remaining),inner::vctpq<Scalar>::mk(remaining)));
    }


#endif
    const LHS lhs_;
    const RHS rhs_;
    const _BinaryOperator<Scalar,DerivedOp> op_;
};

template<typename DerivedOp>
struct Complexity<_Expr<DerivedOp>>
{
   constexpr static int value = Complexity<DerivedOp>::value;
};

template<typename DerivedOp>
struct ElementType<_Expr<DerivedOp>>
{
    typedef typename ElementType<DerivedOp>::type type;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct Complexity<_Binary<LHS,RHS,DerivedOp>>
{
   constexpr static int lhsv = Complexity<LHS>::value;
   constexpr static int rhsv = Complexity<RHS>::value;
   constexpr static int value = lhsv + rhsv + 1;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct ElementType<_Binary<LHS,RHS,DerivedOp>>
{
    typedef typename ElementType<LHS>::type type;
};


template<typename DerivedOp>
struct IsVector<_Expr<DerivedOp>>
{
    constexpr static bool value = IsVector<DerivedOp>::value;
};

template<typename DerivedOp>
struct HasMatrixIndexing<_Expr<DerivedOp>>
{
    constexpr static bool value = HasMatrixIndexing<DerivedOp>::value;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct IsVector<_Binary<LHS,RHS,DerivedOp>>
{
    constexpr static bool value = 
    (IsVector<LHS>::value && IsVector<RHS>::value) ||
    (IsVector<LHS>::value && is_scalar<RHS>()) ||
    (is_scalar<LHS>()     && IsVector<RHS>::value);
};

template<typename LHS,typename RHS,typename DerivedOp>
struct HasMatrixIndexing<_Binary<LHS,RHS,DerivedOp>>
{
    constexpr static bool value = 
    (HasMatrixIndexing<LHS>::value && HasMatrixIndexing<RHS>::value) ||
    (HasMatrixIndexing<LHS>::value && is_scalar<RHS>()) ||
    (is_scalar<LHS>()     && HasMatrixIndexing<RHS>::value);
};

template<typename DerivedOp>
struct IsDynamic<_Expr<DerivedOp>>
{
    constexpr static bool value = IsDynamic<DerivedOp>::value;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct IsDynamic<_Binary<LHS,RHS,DerivedOp>>
{
    constexpr static bool value = IsDynamic<LHS>::value && IsDynamic<RHS>::value;
};

template<typename DerivedOp>
struct StaticLength<_Expr<DerivedOp>>
{
    constexpr static vector_length_t value = StaticLength<DerivedOp>::value;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct StaticLength<_Binary<LHS,RHS,DerivedOp>>
{
    constexpr static vector_length_t value = static_length<LHS,RHS>();
   
};

template<typename DerivedOp>
struct traits<_Expr<DerivedOp>>
{
    typedef typename traits<DerivedOp>::Scalar Scalar;
#if defined(HAS_VECTOR)
    typedef typename traits<DerivedOp>::Vector Vector;
#endif
};

template<typename LHS,typename RHS,typename DerivedOp>
struct traits<_Binary<LHS,RHS,DerivedOp>>
{
    typedef typename traits<LHS>::Scalar Scalar;
#if defined(HAS_VECTOR)
    typedef typename traits<LHS>::Vector Vector;
#endif
};


/*****************
 * 
 * UNARY AST
 */

template<typename LHS,typename DerivedOp>
struct _Unary: _Expr<_Unary<LHS,DerivedOp>>
{
    using Scalar = typename traits<LHS>::Scalar;
#if defined(HAS_VECTOR)
    using Vector = typename traits<LHS>::Vector;
#endif
    _Unary(const LHS &lhs,
           const _UnaryOperator<Scalar,DerivedOp> &op):
            lhs_(lhs),op_(op){
    }

    _Unary(const _Unary &other):
    lhs_(other.lhs_),op_(other.op_){
    }

    _Unary(LHS &&other):
    lhs_(std::move(other.lhs_)),op_(std::move(other.op_)){
    }

    _Unary& operator=(const _Unary& other) = delete;
    _Unary& operator=(_Unary&& other) = delete;


    vector_length_t length() const {
        return(lhs_.length());
    }

    template<typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value,bool>::type = true>
    vector_length_t rows() const {
        return(lhs_.rows());
    }

    template<typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value,bool>::type = true>
    vector_length_t columns() const {
        return(lhs_.columns());
    }

    template<typename L=LHS,
             typename std::enable_if<IsVector<L>::value ,bool>::type = true>
    Scalar const operator[](const index_t i) const {
        return(op_(lhs_[i]));
    }

    template<typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value ,bool>::type = true>
    Scalar const operator()(const index_t r,const index_t c) const
    {
        return(op_(lhs_(r,c)));
    }


#if defined(HAS_VECTOR)
    /* V */
    template<typename L=LHS,
             typename std::enable_if<
                        IsVector<L>::value ,bool>::type = true>
    Vector const vector_op(const index_t i) const
    {
        return(op_(lhs_.vector_op(i)));
    }

    template<typename L=LHS,
             typename std::enable_if<has_predicate_inst<L>() &&
                                     IsVector<L>::value ,bool>::type = true>
    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(op_(lhs_.vector_op_tail(i,remaining),inner::vctpq<Scalar>::mk(remaining)));
    }

    /*

    For Matrix

    */

    /* V */
    template<typename L=LHS,
             typename std::enable_if<HasMatrixIndexing<L>::value ,bool>::type = true>
    Vector const matrix_op(const index_t r,const index_t c) const
    {
        return(op_(lhs_.matrix_op(r,c)));
    }

    template<typename L=LHS,
             typename std::enable_if<has_predicate_inst<L>() &&
                                     HasMatrixIndexing<L>::value ,bool>::type = true>
    Vector const matrix_op_tail(const index_t r,const index_t c,const vector_length_t remaining) const
    {
        return(op_(lhs_.matrix_op_tail(r,c,remaining),inner::vctpq<Scalar>::mk(remaining)));
    }

    
#endif
    const LHS lhs_;
    const _UnaryOperator<Scalar,DerivedOp> op_;
};

template<typename LHS,typename DerivedOp>
struct Complexity<_Unary<LHS,DerivedOp>>
{
   constexpr static int value = 1 + Complexity<LHS>::value;
};

template<typename LHS,typename DerivedOp>
struct ElementType<_Unary<LHS,DerivedOp>>
{
    typedef typename ElementType<LHS>::type type;
};

template<typename LHS,typename DerivedOp>
struct IsVector<_Unary<LHS,DerivedOp>>
{
    constexpr static bool value = IsVector<LHS>::value;
};

template<typename LHS,typename DerivedOp>
struct HasMatrixIndexing<_Unary<LHS,DerivedOp>>
{
    constexpr static bool value = HasMatrixIndexing<LHS>::value;
};

template<typename LHS,typename DerivedOp>
struct IsDynamic<_Unary<LHS,DerivedOp>>
{
    constexpr static bool value = IsDynamic<LHS>::value;
};

template<typename LHS,typename DerivedOp>
struct StaticLength<_Unary<LHS,DerivedOp>>
{
    constexpr static vector_length_t value = StaticLength<LHS>::value;
};


template<typename LHS,typename DerivedOp>
struct traits<_Unary<LHS,DerivedOp>>
{
    typedef typename traits<LHS>::Scalar Scalar;
#if defined(HAS_VECTOR)
    typedef typename traits<LHS>::Vector Vector;
#endif
};




/*

Dot product 

*/

template<typename DA>
using DotResult = typename number_traits<typename traits<DA>::Scalar>::accumulator;




template<typename VA,typename VB,
         typename std::enable_if<vector_idx_pair<VA,VB>() &&
         is_only_vector<VA>() &&
         is_only_vector<VB>() &&
         (!IsDynamic<VA>::value || !IsDynamic<VB>::value),bool>::type = true>
inline DotResult<VA> dot(const VA& a,
                         const VB& b)
{
   constexpr vector_length_t l = static_length<VA,VB>();
   return(_dot(a,b,l,CURRENT_ARCH));
}

template<typename VA,typename VB,
         typename std::enable_if<vector_idx_pair<VA,VB>() &&
         is_only_vector<VA>() &&
         is_only_vector<VB>() &&
         (IsDynamic<VA>::value && IsDynamic<VB>::value),bool>::type = true>
inline DotResult<VA> dot(const VA& a,
                         const VB& b)
{
   const vector_length_t l = a.length();
   
   return(_dot(a,b,l,CURRENT_ARCH));
}





template<typename VA,typename VB,
         typename std::enable_if<vector_idx_pair<VA,VB>() &&
         (!IsDynamic<VA>::value || !IsDynamic<VB>::value),bool>::type = true>
inline void swap(VA&& a,
                 VB&& b)
{
   constexpr vector_length_t l = static_length<VA,VB>();
   
   _swap(std::forward<VA>(a),std::forward<VB>(b),l,CURRENT_ARCH);
}

template<typename VA,typename VB,
         typename std::enable_if<vector_idx_pair<VA,VB>() &&
         (IsDynamic<VA>::value && IsDynamic<VB>::value),bool>::type = true>
inline void swap(VA&& a,
                 VB&& b)
{
   const vector_length_t l = a.length();
   
   _swap(std::forward<VA>(a),std::forward<VB>(b),l,CURRENT_ARCH);
}

/*! @} */

}

