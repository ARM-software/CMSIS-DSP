// -*- C++ -*-
/** @file */ 
#pragma once 


#include <memory>
#include <cstring>
#include <algorithm>
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

/**
 * @brief Zero-copy transposed view of a matrix.
 *
 * The view only swaps matrix indexing and dimensions. Algorithms can
 * recognize this type and select a kernel which consumes the original
 * row-major storage without materializing a transposed matrix.
 *
 * @tparam M Matrix datatype
 */
template<typename M>
struct TransposeView
{
    using Scalar = typename traits<M>::Scalar;
    using Storage = typename VecRef<M>::type;

    explicit TransposeView(const M &matrix) : matrix_(VecRef<M>::ref(matrix)) {}

    vector_length_t rows() const { return matrix_.columns(); }
    vector_length_t columns() const { return matrix_.rows(); }

    Scalar operator()(const index_t row, const index_t column) const
    {
        return matrix_(column, row);
    }

    auto row(const index_t row) const { return matrix_.col(row); }
    auto col(const index_t column) const { return matrix_.row(column); }

    const Storage &original() const { return matrix_; }

private:
    Storage matrix_;
};

/**
 * @brief Create a zero-copy transposed matrix view.
 *
 * @tparam M Matrix datatype
 * @param matrix Matrix whose dimensions and indexing are to be transposed
 * @return A non-owning transposed view
 */
template<typename M,
         typename std::enable_if<HasMatrixIndexing<M>::value, bool>::type = true>
inline TransposeView<M> transpose_view(const M &matrix)
{
    return TransposeView<M>(matrix);
}

template<typename M>
struct traits<TransposeView<M>>
{
    using Scalar = typename traits<M>::Scalar;
#if defined(HAS_VECTOR)
    using Vector = typename traits<Scalar>::Vector;
#endif
};

template<typename M>
struct HasMatrixIndexing<TransposeView<M>>
{
    constexpr static bool value = true;
};

template<typename M>
struct ElementType<TransposeView<M>>
{
    using type = typename ElementType<M>::type;
};

template<typename M>
struct IsDynamic<TransposeView<M>>
{
    constexpr static bool value = IsDynamic<M>::value;
};

template<typename M>
struct StaticLength<TransposeView<M>>
{
    constexpr static vector_length_t value = StaticLength<M>::value;
};

template<typename M>
struct NbRows<TransposeView<M>>
{
    constexpr static vector_length_t value = NbCols<M>::value;
};

template<typename M>
struct NbCols<TransposeView<M>>
{
    constexpr static vector_length_t value = NbRows<M>::value;
};

template<typename M>
struct OutputVectorDim<TransposeView<M>>
{
    constexpr static vector_length_t value = NbCols<M>::value;
};

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
   && compatible_element<M,V>();

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
   compatible_element<MA,MB>();

};

template<typename M,typename V>
struct CompatibleDynamicMatVecProduct
{
   constexpr static bool value = 
   HasMatrixIndexing<M>::value &&
   IsDynamic<M>::value &&
   is_only_vector<V>() &&
   compatible_element<M,V>();

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
   compatible_element<MA,MB>();
};

template<typename MA,typename MB>
struct CompatibleDynamicMatMatProductDynamicStride
{
   constexpr static bool value = 
   HasMatrixIndexing<MA>::value &&
   IsMatrix<MB>::value &&
   IsDynamic<MA>::value &&
   !HasStaticStride<MB>::value &&
   compatible_element<MA,MB>();
};

template<typename MA,typename MB>
struct CompatibleDynamicMatMatProduct
{
   constexpr static bool value = 
   HasMatrixIndexing<MA>::value &&
   IsMatrix<MB>::value &&
   IsDynamic<MA>::value &&
   compatible_element<MA,MB>();
};

template<typename M,typename V>
struct OutputVector {
    typedef Vector<DotFieldResult<M,V>,
                   OutputVectorDim<M>::value,TMP_ALLOC> type;
};

template<typename MA,typename MB>
struct OutputMatrix {
    constexpr static bool dynamic = (NbRows<MA>::value < 0) || (NbCols<MB>::value < 0);
    constexpr static vector_length_t nbrows = dynamic ? DYNAMIC : NbRows<MA>::value;
    constexpr static vector_length_t nbcols = dynamic ? DYNAMIC : NbCols<MB>::value;

    typedef Matrix<DotFieldResult<MA,MB>,nbrows,nbcols,TMP_ALLOC> type;
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

/** Lazy transposed-matrix times vector expression. */
template<typename M, typename V>
struct _TransposedMatVec: _Expr<_TransposedMatVec<M,V>>
{
    using MatrixScalar = typename traits<M>::Scalar;
    using VectorScalar = typename traits<V>::Scalar;
    using Scalar = typename MixedRes<MatrixScalar,VectorScalar>::type;
    using Accumulator = typename number_traits<Scalar>::accumulator;
#if defined(HAS_VECTOR)
    using Vector = typename traits<Scalar>::Vector;
#endif

    _TransposedMatVec(const TransposeView<M> &matrix, const V &vector)
        : matrix_(matrix), vector_(vector) {}

    vector_length_t length() const { return matrix_.rows(); }

    Scalar operator[](const index_t column) const
    {
        Accumulator sum{};
        const auto &original = matrix_.original();
        for (index_t row = 0; row < original.rows(); ++row)
            sum = inner::mac(sum,original(row,column),vector_[row]);
        return inner::from_accumulator(sum);
    }

#if defined(HAS_VECTOR)
    auto vector_op(const index_t column) const
    {
        if constexpr (has_vector_inst<M>() && has_vector_inst<V>() &&
                      same_nb_lanes<M,V>() && is_float<M>())
        {
            using VectorAccumulator =
                typename vector_traits<Scalar>::temp_accumulator;
            VectorAccumulator sum = vector_traits<Scalar>::temp_acc_zero();
            const auto &original = matrix_.original();
            for (index_t row = 0; row < original.rows(); ++row)
                sum = inner::vmacc(sum,
                                   original.row(row).vector_op(column),
                                   vector_[row]);
            return sum;
        }
        else
        {
            constexpr int lanes = vector_traits<Scalar>::nb_lanes;
            Accumulator sums[lanes] = {};
            Scalar values[lanes] = {};
            const auto &original = matrix_.original();
            for (index_t row = 0; row < original.rows(); ++row)
            {
                const VectorScalar value = vector_[row];
                for (index_t lane = 0; lane < lanes; ++lane)
                    sums[lane] = inner::mac(
                        sums[lane],original(row,column + lane),value);
            }
            for (index_t lane = 0; lane < lanes; ++lane)
                values[lane] = inner::from_accumulator(sums[lane]);
            return inner::vload1<1>(values);
        }
    }

    auto vector_op_tail(const index_t column,
                        const vector_length_t remaining) const
    {
        if constexpr (has_vector_inst<M>() && has_vector_inst<V>() &&
                      same_nb_lanes<M,V>() && is_float<M>())
        {
            using VectorAccumulator =
                typename vector_traits<Scalar>::temp_accumulator;
            VectorAccumulator sum = vector_traits<Scalar>::temp_acc_zero();
            const auto &original = matrix_.original();
            for (index_t row = 0; row < original.rows(); ++row)
                sum = inner::vmacc(
                    sum,
                    original.row(row).vector_op_tail(column,remaining),
                    vector_[row]);
            return sum;
        }
        else
        {
            constexpr int lanes = vector_traits<Scalar>::nb_lanes;
            Accumulator sums[lanes] = {};
            Scalar values[lanes] = {};
            const auto &original = matrix_.original();
            for (index_t row = 0; row < original.rows(); ++row)
            {
                const VectorScalar value = vector_[row];
                for (index_t lane = 0; lane < remaining; ++lane)
                    sums[lane] = inner::mac(
                        sums[lane],original(row,column + lane),value);
            }
            for (index_t lane = 0; lane < remaining; ++lane)
                values[lane] = inner::from_accumulator(sums[lane]);
            return inner::vload1<1>(values);
        }
    }
#endif

private:
    TransposeView<M> matrix_;
    V vector_;
};

template<typename M, typename V>
struct traits<_TransposedMatVec<M,V>>
{
    using Scalar = typename MixedRes<typename traits<M>::Scalar,
                                     typename traits<V>::Scalar>::type;
#if defined(HAS_VECTOR)
    using Vector = typename traits<Scalar>::Vector;
#endif
};

template<typename M, typename V>
struct ElementType<_TransposedMatVec<M,V>>
{
    using type = typename MixedRes<typename traits<M>::Scalar,
                                   typename traits<V>::Scalar>::type;
};

template<typename M, typename V>
struct IsVector<_TransposedMatVec<M,V>>
{
    constexpr static bool value = true;
};

template<typename M, typename V>
struct IsDynamic<_TransposedMatVec<M,V>>
{
    constexpr static bool value = IsDynamic<M>::value;
};

template<typename M, typename V>
struct StaticLength<_TransposedMatVec<M,V>>
{
    constexpr static vector_length_t value = NbCols<M>::value;
};

template<typename M, typename V>
struct Complexity<_TransposedMatVec<M,V>>
{
    constexpr static int value = 1;
};

template<typename M, typename V>
struct VecRef<_TransposedMatVec<M,V>>
{
    using type = _TransposedMatVec<M,V>;
    static type ref(const type &expression) { return expression; }
};

#if !defined(ARM_MATH_NEON)

template<typename M,
         typename V,
         typename std::enable_if<
             (CompatibleStaticMatVecProduct<TransposeView<M>,V>::value ||
              CompatibleDynamicMatVecProduct<TransposeView<M>,V>::value),
             bool>::type = true>
inline auto dot(const TransposeView<M> &matrix, const V &vector)
{
    using VectorRef = VecRef<V>;
    return _TransposedMatVec<M,typename VectorRef::type>(
        matrix,VectorRef::ref(vector));
}

#endif


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
    using ScalarLHS = typename traits<LHS>::Scalar;
    using ScalarRHS = typename traits<RHS>::Scalar;
#if defined(HAS_VECTOR)
    //! Type of vector in the architecture
    using VectorLHS = typename traits<LHS>::Vector;
    using VectorRHS = typename traits<RHS>::Vector;
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
            const _BinaryOperator<ScalarLHS,ScalarRHS,DerivedOp> &op):
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
    auto  operator()(const index_t r,const index_t c) const
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
    auto  matrix_op(const index_t r,const index_t c) const
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
    auto  matrix_op_tail(const index_t r,const index_t c,const vector_length_t remaining) const
    {
        return(op_(lhs_[r],rhs_.vector_op_tail(c,remaining),inner::vctpq<ScalarLHS>::mk(remaining)));
    }


#endif
    const LHS lhs_;
    const RHS rhs_;
    const _BinaryOperator<ScalarLHS,ScalarRHS,DerivedOp> op_;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct IsMixed<_Outer<LHS,RHS,DerivedOp>>
{
    using EA = typename ElementType<remove_constref_t<LHS>>::type;
    using EB = typename ElementType<remove_constref_t<RHS>>::type;
    constexpr static bool value = (IsComplexNumber<EA>::value != IsComplexNumber<EB>::value)
      || IsMixed<LHS>::value || IsMixed<RHS>::value;
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
    typedef typename ElementType<remove_constref_t<LHS>>::type type;
};

template<typename LHS,typename RHS,typename DerivedOp>
struct traits<_Outer<LHS,RHS,DerivedOp>>
{
    using LScalar = typename traits<LHS>::Scalar;
    using RScalar = typename traits<RHS>::Scalar;
    
    typedef typename MixedRes<LScalar,RScalar>::type  Scalar;
#if defined(HAS_VECTOR)
    typedef typename traits<Scalar>::Vector Vector;
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
    using ScalarLHS = typename traits<VA>::Scalar;
    using ScalarRHS = typename traits<VB>::Scalar;
    using VecLHS = VecRef<VA>;
    using VecRHS = VecRef<VB>;

    return(_Outer<typename VecLHS::type,typename VecRHS::type,_MulOp<ScalarLHS,ScalarRHS>>(VecLHS::ref(a),VecRHS::ref(b),_MulOp<ScalarLHS,ScalarRHS>()));


}

/*! @} */
}
