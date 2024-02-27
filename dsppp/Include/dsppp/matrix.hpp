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
#include "vec.hpp"
#include "matrix_impl.hpp"
#include "matrix_view.hpp"

namespace arm_cmsis_dsp {

/** \addtogroup Matrix Matrixes
 *  \ingroup DSPPP
 *  @{
 */

template<typename P,int R,int C,
         template<int> typename A>
struct traits<Matrix<P,R,C,A>>
{
    typedef P Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<P>::vector Vector;
#endif
};

template<typename P,int S>
struct traits<MatrixView<P,S>>
{
    typedef P Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<P>::vector Vector;
#endif
};

template<typename P,int R,int C,
         template<int> typename A>
struct traits<const Matrix<P,R,C,A>&>
{
    typedef P Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<P>::vector Vector;
#endif
};

template<typename P,int S>
struct traits<const MatrixView<P,S>&>
{
    typedef P Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<P>::vector Vector;
#endif
};


template<typename P,int R,int C,
         template<int> typename Allocator>
struct IsVector<Matrix<P,R,C,Allocator>>
{
    constexpr static bool value = true;
};



template<typename P,int R,int C,
         template<int> typename Allocator>
struct HasStaticStride<Matrix<P,R,C,Allocator>>
{
    constexpr static bool value = (C>0);
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct StaticStride<Matrix<P,R,C,Allocator>>
{
    constexpr static index_t value = C;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct IsMatrix<Matrix<P,R,C,Allocator>>
{
    constexpr static bool value = true;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct HasMatrixIndexing<Matrix<P,R,C,Allocator>>
{
    constexpr static bool value = true;
};

template<typename P,int S>
struct IsMatrix<MatrixView<P,S>>
{
    constexpr static bool value = true;
};

template<typename P,int S>
struct HasStaticStride<MatrixView<P,S>>
{
    constexpr static bool value = (S>0);
};

template<typename P,int S>
struct StaticStride<MatrixView<P,S>>
{
    constexpr static index_t value = S;
};

template<typename P,int S>
struct HasMatrixIndexing<MatrixView<P,S>>
{
    constexpr static bool value = true;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct IsVector<const Matrix<P,R,C,Allocator>&>
{
    constexpr static bool value = true;
};


template<typename P>
struct IsVector<MatrixView<P,CONSTRAINED_DYNAMIC>>
{
    constexpr static bool value = true;
};

template<typename P>
struct IsVector<const MatrixView<P,CONSTRAINED_DYNAMIC>&>
{
    constexpr static bool value = true;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct HasStaticStride<const Matrix<P,R,C,Allocator>&>
{
    constexpr static bool value = (C>0);
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct StaticStride<const Matrix<P,R,C,Allocator>&>
{
    constexpr static index_t value = C;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct HasMatrixIndexing<const Matrix<P,R,C,Allocator>&>
{
    constexpr static bool value = true;
};


template<typename P,int S>
struct IsMatrix<const MatrixView<P,S>&>
{
    constexpr static bool value = true;
};

template<typename P,int S>
struct HasMatrixIndexing<const MatrixView<P,S>&>
{
    constexpr static bool value = true;
};

template<typename P,int S>
struct HasStaticStride<const MatrixView<P,S>&>
{
    constexpr static bool value = (S>0);
};

template<typename P,int S>
struct StaticStride<const MatrixView<P,S>&>
{
    constexpr static index_t value = S;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct ElementType<Matrix<P,R,C,Allocator>>
{
    typedef P type;
};


template<typename P,int S>
struct ElementType<MatrixView<P,S>>
{
    typedef P type;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct ElementType<const Matrix<P,R,C,Allocator>&>
{
    typedef P type;
};

template<typename P,int S>
struct ElementType<const MatrixView<P,S>&>
{
    typedef P type;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct StaticLength<Matrix<P,R,C,Allocator>>
{
    constexpr static vector_length_t value = (R*C<0) ? 0 : R*C;
};

template<typename P,int S>
struct StaticLength<MatrixView<P,S>>
{
    constexpr static vector_length_t value = 0;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct StaticLength<const Matrix<P,R,C,Allocator>&>
{
    constexpr static vector_length_t value = (R*C<0) ? 0 : R*C;
};

template<typename P,int S>
struct StaticLength<const MatrixView<P,S>&>
{
    constexpr static vector_length_t value = 0 ;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct IsDynamic<Matrix<P,R,C,Allocator>>
{
    constexpr static bool value = (R<0) || (C<0);
};

template<typename P,int S>
struct IsDynamic<MatrixView<P,S>>
{
    constexpr static bool value = true;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct IsDynamic<const Matrix<P,R,C,Allocator>&>
{
    constexpr static bool value = (R<0) || (C<0);
};

template<typename P,int S>
struct IsDynamic<const MatrixView<P,S>&>
{
    constexpr static bool value = true;
};

/*


Compatibility of vector and matrix dimensions at build time

*/

template<typename T>
struct NbRows
{
    constexpr static vector_length_t value = DYNAMIC;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct NbRows<Matrix<P,R,C,Allocator>>
{
    constexpr static vector_length_t value = R;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct NbRows<const Matrix<P,R,C,Allocator>&>
{
    constexpr static vector_length_t value = R;
};

template<typename T>
struct NbCols
{
    constexpr static vector_length_t value = DYNAMIC;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct NbCols<Matrix<P,R,C,Allocator>>
{
    constexpr static vector_length_t value = C;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct NbCols<const Matrix<P,R,C,Allocator>&>
{
    constexpr static vector_length_t value = C;
};


template<typename M,typename V>
struct CompatibleStaticMatVecProduct
{
   constexpr static bool value = 
   is_only_vector<V>() &&
   HasMatrixIndexing<M>::value &&
   (NbCols<M>::value == StaticLength<V>::value) &&
   !IsDynamic<M>::value
   && SameElementType<M,V>::value;

};

/* MB IsMatrix because we need transpose operator */
template<typename MA,typename MB>
struct CompatibleStaticMatMatProduct
{
   constexpr static bool value = 
   HasMatrixIndexing<MA>::value &&
   IsMatrix<MB>::value &&
   (NbCols<MA>::value == NbRows<MB>::value) &&
   !IsDynamic<MA>::value && 
   SameElementType<MA,MB>::value;

};

template<typename M,typename V>
struct CompatibleDynamicMatVecProduct
{
   constexpr static bool value = 
   HasMatrixIndexing<M>::value &&
   IsDynamic<M>::value &&
   is_only_vector<V>() &&
   SameElementType<M,V>::value;

};

/* MB IsMatrix because we need transpose operator */
template<typename MA,typename MB>
struct CompatibleDynamicMatMatProductStaticStride
{
   constexpr static bool value = 
   HasMatrixIndexing<MA>::value &&
   IsMatrix<MB>::value &&
   IsDynamic<MA>::value &&
   HasStaticStride<MB>::value &&
   SameElementType<MA,MB>::value;
};

template<typename MA,typename MB>
struct CompatibleDynamicMatMatProductDynamicStride
{
   constexpr static bool value = 
   HasMatrixIndexing<MA>::value &&
   IsMatrix<MB>::value &&
   IsDynamic<MA>::value &&
   !HasStaticStride<MB>::value &&
   SameElementType<MA,MB>::value;
};

template<typename MA,typename MB>
struct CompatibleDynamicMatMatProduct
{
   constexpr static bool value = 
   HasMatrixIndexing<MA>::value &&
   IsMatrix<MB>::value &&
   IsDynamic<MA>::value &&
   SameElementType<MA,MB>::value;
};

template<typename M,typename V>
struct OutputVector {
    typedef Vector<typename traits<V>::Scalar,
                   OutputVectorDim<M>::value,TMP_ALLOC> type;
};

template<typename MA,typename MB>
struct OutputMatrix {
    constexpr static bool dynamic = (NbRows<MA>::value < 0) || (NbCols<MB>::value < 0);
    constexpr static vector_length_t nbrows = dynamic ? DYNAMIC : NbRows<MA>::value;
    constexpr static vector_length_t nbcols = dynamic ? DYNAMIC : NbCols<MB>::value;

    typedef Matrix<typename traits<MA>::Scalar,nbrows,nbcols,TMP_ALLOC> type;
};



template<typename M>
struct OutputVectorDim 
{
    constexpr static vector_length_t value = DYNAMIC;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct OutputVectorDim<Matrix<P,R,C,Allocator>>
{
   constexpr static vector_length_t value = R;
};

template<typename P,int R,int C,
         template<int> typename Allocator>
struct OutputVectorDim<const Matrix<P,R,C,Allocator>&>
{
   constexpr static vector_length_t value = R;
};


template<typename T,int S>
struct VecRef<MatrixView<T,S>>
{
   typedef MatrixView<T,S> type;
   static type ref(const MatrixView<T,S>&a){
      return(a);
   };
};

template<typename P,int R,int C,
         template<int> typename A>
struct VecRef<Matrix<P,R,C,A>,((R>0) && (C>0))>
{
   typedef const Matrix<P,R,C,A>& type;
   static type ref(const Matrix<P,R,C,A>&a,typename std::enable_if<(R>0) && (C>0)>::type* = nullptr){
      return(a);
   };
};

template<typename P,int R,int C,
         template<int> typename A>
struct VecRef<Matrix<P,R,C,A>,((R<0) || (C<0))>
{
   typedef MatrixView<P,CONSTRAINED_DYNAMIC> type;
   static type ref(const Matrix<P,R,C,A>&a,typename std::enable_if<(R<0) || (C<0)>::type* = nullptr){
      return(type(a,a.rows(),a.columns()));
   };
};


/*****************
 * 
 * 
 *  Fused matrix operators
 * 
 ****************/

/**
 * @brief  Outer product operator for expressions
 *
 * @tparam LHS Left hand side datatype
 * @tparam RHS Right hand side datatype
 * @tparam DerivedOp Operator for the Outer operation
 * 
 * vector `op` vector (including matrix)
 */
template<typename LHS,typename RHS,typename DerivedOp>
struct _Outer: _Expr<_Outer<LHS,RHS,DerivedOp>>
{
    //! Type of vector elements
    using Scalar = typename traits<LHS>::Scalar;
#if defined(HAS_VECTOR)
    //! Type of vector in the architecture
    using Vector = typename traits<LHS>::Vector;
#endif
    /**
    * @brief      Create an Outer operator
    *
    * @param lhs Left hand side expression
    * @param rhs Right hand side expression
    * @param op operator
    */
    _Outer(const LHS &lhs,
            const RHS &rhs,
            const _BinaryOperator<Scalar,DerivedOp> &op):
            lhs_(lhs),rhs_(rhs),op_(op){
    }

    /**
    * @brief      Create an Outer operator from another operator of same type
    *
    * @param other the other operator
    */
    _Outer(const _Outer &other):
    lhs_(other.lhs_),rhs_(other.rhs_),op_(other.op_){
    }

    _Outer& operator=(const _Outer& other) = delete;
    _Outer& operator=(_Outer&& other) = delete;

    /**
    * @brief   Move semantic for _Outer operator
    *
    * @param other the other operator
    */
    _Outer(_Outer &&other): 
    lhs_(std::move(other.lhs_)),rhs_(std::move(other.rhs_)),op_(std::move(other.op_))
    {
    }

    

    /**
    * @brief   Length of the matrix (seen as vector) resulting from the outer operator
    * @tparam R Right hand side datatype
    * @tparam L Left hand side datatype
    *
    * @return  vector dimension
    */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<L>::value && IsVector<R>::value,bool>::type = true>
    vector_length_t length() const {
        return(lhs_.length() * rhs_.length());
    }

    /**
    * @brief   Rows of the matrix
    * @tparam R Right hand side datatype
    * @tparam L Left hand side datatype
    *
    * @return  number of rows
    */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<L>::value,bool>::type = true>
    vector_length_t rows() const {
        return(lhs_.length());
    }


    /**
    * @brief   Columns of the matrix
    * @tparam R Right hand side datatype
    * @tparam L Left hand side datatype
    *
    * @return  number of columns
    */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<R>::value,bool>::type = true>
    vector_length_t columns() const {
        return(rhs_.length());
    }


    /**
    * @brief   Expression value at given position
    * @tparam R Right hand side datatype
    * @tparam L Left hand side datatype
    * @param r row index
    * @param c column index
    *
    * @return  expression value
    */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<L>::value && 
                        IsVector<R>::value,bool>::type = true>
    Scalar const operator()(const index_t r,const index_t c) const
    {
        return(op_(lhs_[r],rhs_[c]));
    }

  
#if defined(HAS_VECTOR)
    /*
     * 
     * For matrix
     * 
     */

    /* V + V */

    /**
    * @brief   Expression vector value at given position
    * @tparam R Right hand side datatype
    * @tparam L Left hand side datatype
    * @param r row index
    * @param c column index
    *
    * @return  expression vector value
    *
    * Vector + Vector (matrix interpreted as a Vector)
    */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<L>::value && 
                        IsVector<R>::value,bool>::type = true>
    Vector const matrix_op(const index_t r,const index_t c) const
    {
        return(op_(lhs_[r],rhs_.vector_op(c)));
    }

    /**
    * @brief   Expression vector value at given position with tail predication
    * @tparam R Right hand side datatype
    * @tparam L Left hand side datatype
    * @param r row index
    * @param c column index
    * @param remaining remaining number of samples in loop
    *
    * @return  expression vector value
    *
    * Vector + Vector (matrix interpreted as a Vector)
    */
    template<typename R=RHS, typename L=LHS,
             typename std::enable_if<IsVector<L>::value && 
                        IsVector<R>::value,bool>::type = true>
    Vector const matrix_op_tail(const index_t r,const index_t c,const vector_length_t remaining) const
    {
        return(op_(lhs_[r],rhs_.vector_op_tail(c,remaining),inner::vctpq<Scalar>::mk(remaining)));
    }


#endif
    const LHS lhs_;
    const RHS rhs_;
    const _BinaryOperator<Scalar,DerivedOp> op_;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct IsVector<_Outer<LHS,RHS,DerivedOp>>
{
    constexpr static bool value = false;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct HasMatrixIndexing<_Outer<LHS,RHS,DerivedOp>>
{
    constexpr static bool value = true;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct StaticLength<_Outer<LHS,RHS,DerivedOp>>
{
    constexpr static vector_length_t value = StaticLength<LHS>::value * StaticLength<RHS>::value;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct IsDynamic<_Outer<LHS,RHS,DerivedOp>>
{
    constexpr static vector_length_t value = IsDynamic<LHS>::value || IsDynamic<RHS>::value;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct Complexity<_Outer<LHS,RHS,DerivedOp>>
{
   constexpr static int lhsv = Complexity<LHS>::value;
   constexpr static int rhsv = Complexity<RHS>::value;
   constexpr static int value = lhsv + rhsv + 1;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct ElementType<_Outer<LHS,RHS,DerivedOp>>
{
    typedef typename ElementType<LHS>::type type;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct traits<_Outer<LHS,RHS,DerivedOp>>
{
    typedef typename traits<LHS>::Scalar Scalar;
#if defined(HAS_VECTOR)
    typedef typename traits<LHS>::Vector Vector;
#endif
};

template<typename LHS,typename RHS,typename OP>
struct VecRef<_Outer<LHS,RHS,OP>>
{
   typedef _Outer<LHS,RHS,OP> type;
   static type ref(const _Outer<LHS,RHS,OP>&a){
      return(a);
   };
};

template<typename LHS,typename RHS,typename OP>
struct NbRows<_Outer<LHS,RHS,OP>>
{
    constexpr static vector_length_t value = NbRows<LHS>::value;
};


template<typename LHS,typename RHS,typename OP>
struct NbCols<_Outer<LHS,RHS,OP>>
{
    constexpr static vector_length_t value = NbCols<RHS>::value;
};


/**
* @brief   Outer product
* @tparam VA Right hand side datatype
* @tparam VB Left hand side datatype
* @param a Vector a
* @param b Vector b
*
* @return  Outer product of a and b
*
*/
template<typename VA,typename VB,
typename std::enable_if<vector_idx_pair<VA,VB>(),bool>::type = true>
inline auto outer(const VA&a,const VB&b)
{
   //constexpr int NBROWS = StaticLength<VA>::value;
   //constexpr int NBCOLS = StaticLength<VB>::value;

   //using T = typename traits<VA>::Scalar;

   //Matrix<T,NBROWS,NBCOLS,TMP_ALLOC> res;
   //_outer(res,a,b);
    using Scalar = typename traits<VA>::Scalar;
    using VecLHS = VecRef<VA>;
    using VecRHS = VecRef<VB>;

    return(_Outer<typename VecLHS::type,typename VecRHS::type,_MulOp<Scalar>>(VecLHS::ref(a),VecRHS::ref(b),_MulOp<Scalar>()));


}

/*! @} */
}
