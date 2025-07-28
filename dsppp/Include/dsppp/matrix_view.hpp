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

namespace arm_cmsis_dsp {

/** \addtogroup Matrix
 *  @{
 */

/** @brief Matrix
 *  @tparam T Type of the scalar
 *  @tparam S Stride
 */
template<typename T,int S=1>
struct MatrixView
{
   /** @brief Number of rows
    *  @return Number of rows
    */
   vector_length_t rows() const {return(nb_rows_);}

   /** @brief Number of columns
    *  @return Number of columns
    */
   vector_length_t columns() const {return(nb_cols_);}

    /** @brief Number of stride
    *  @return Number of stride
    */
   constexpr uint32_t stride() const {return(S);}

    /** @brief Create matrix view on a buffer (buffer not owned by the view)
    * @param v buffer
    * @param rows number of rows
    * @param cols number of columns
    */
   explicit MatrixView(T* v,
              const vector_length_t rows,
              const vector_length_t cols):
   v_(v),nb_rows_(rows),nb_cols_(cols){};

    /** @brief Create matrix view on vector (vector not owned by the view)
    * @param v vector
    * @param rows number of rows
    * @param cols number of columns
    */
   explicit MatrixView(const Vector_Base<T> &v,
              const vector_length_t rows,
              const vector_length_t cols):
   v_(v.ptr()),nb_rows_(rows),nb_cols_(cols){};

   virtual ~MatrixView() {};

   MatrixView(const MatrixView& other):
    v_(other.v_),
    nb_rows_(other.nb_rows_),nb_cols_(other.nb_cols_){};

   MatrixView(MatrixView&& other) :
    v_(other.v_),
    nb_rows_(other.nb_rows_),nb_cols_(other.nb_cols_){};

   
   MatrixView& operator=(const MatrixView& other) = delete;
   MatrixView& operator=(MatrixView&& other)  = delete;

   /** @brief Access matrix view element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   T& operator()(const index_t r,const index_t c)
   {
     return(v_[r*stride()+c]);
   }

    /** @brief Access matrix view element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   T const operator()(const index_t r,const index_t c) const
   {
     return(v_[r*stride()+c]);
   }


   /** @brief Assign matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator=(const _Expr<Derived>&other)
   {
      eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   }

    /** @brief Assign matrix view from constant
    * @param val The constant
    * @return the matrix
    * 
    */
   MatrixView& operator=(const T val)
   {
        _Fill2D(*this,val,rows(),columns(),CURRENT_ARCH);
       
        return(*this);
   }


    /** @brief Add matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator +=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this + other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Add matrix from matrix view
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator +=(const MatrixView& other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };


    /** @brief Add constant to matrix view
    * @param other The constant
    * @return the matrix
    * 
    */
   MatrixView& operator +=(const T other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator -=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this - other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract matrix view
    * @param other Other matrix view
    * @return the matrix
    * 
    */
   MatrixView& operator -=(const MatrixView& other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract constant
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator -=(const T other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Elementwise multiply matrix view with expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator *=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this * other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   /** @brief Elementwise multiply matrix view with matrix view
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator *=(const MatrixView& other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   /** @brief Elementwise multiply matrix view constant
    * @param other constant
    * @return the matrix
    * 
    */
   MatrixView& operator *=(const T other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

  /**
    * @brief  Display the matrix content for debug purpose
    * @param stream Output stream
    * @param other The matrix to display
    * @return the stream
    * 
    */
  friend std::ostream& operator<< (std::ostream& stream, const MatrixView& other) {
        using DT = typename number_traits<T>::display_type;
        for(index_t row=0;row<other.rows();row++)
        {
           for(index_t col=0;col<other.columns();col++)
           {
                stream << (DT)other(row,col)<< " , ";
           }
           stream << "\r\n";
        }
        stream << "\r\n";
        return(stream);
    }

   /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   VectorView<T,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

    /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   VectorView<T,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

   /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   const VectorView<T,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

   /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   const VectorView<T,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

    /** @brief Create a column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   VectorView<T,CS*S> col(const index_t i,const index_t start=0)
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*rows()));
   }

    /** @brief Create a column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @param stop Stop index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   VectorView<T,CS*S> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*stop));
   }

   /** @brief Create a constant column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   const VectorView<T,CS*S> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*rows()));
   }

    /** @brief Create a constant column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @param stop Stop index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   const VectorView<T,CS*S> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*stop));
   }

   #if defined(HAS_VECTOR)
    //! Type of vectors for a vector architecture and for scalar datatype P
    using VectorType = typename vector_traits<T>::vector;

    /**
    * @brief   %Vector store at a given row,column position
    *
    * @param row row index
    * @param col column index
    * @param val %Vector value
    * 
    * On an architecture supporting vectors, if the scalar datatype T
    * has a corresponding vector datatype, this function stores a vector
    * value at row,column in this matrix.
    */
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        inner::vstore1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val);
    }

#if defined(HAS_PREDICATED_LOOP)
    /**
    * @brief   %Vector store at a given row,column position with predicated tail
    *
    * @param row row index
    * @param col column index
    * @param remaining Number of remaining samples in the loop
    * @param val Vector value to write at index i with tail predication
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function stores a vector value at row,column index in this matrix datatype
    * with predication
    */
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val,remaining,inner::vctpq<T>::mk(remaining));
    }

     /**
    * @brief   %Vector operation at a given row,column position with predicated tail
    *
    * @param row row index
    * @param col column index
    * @param remaining Number of remaining samples in the loop
    * @return the vector result of the operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function compute an operation at row,column index in this matrix datatype
    * with predication
    */
    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif

     /**
    * @brief   %Vector operation at a given row,column position
    *
    * @param row row index
    * @param col column index
    * @return the vector result of the operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function compute an operation at row,column index in this matrix datatype
    */
    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(inner::vload1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col])));
    }
#endif

    /** @brief Fill diagonal of a matrix with a vector
    * @tparam VA Vector datatype
    * @param a Vector for initializing the diagonal
    * 
    */
    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            compatible_element<VA,T>(),bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,a->length());
    }

    /** @brief Create the transposed matrix
    * @return a matrix
    *   
    */
    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> transpose() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

    /** @brief Create a matrix of same type
    * @return a matrix
    *   
    */
    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> create() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(rows(),columns());
       return(res);
    }

     /**
    * @brief      Pointer to storage buffer
    * @return Pointer to storage
    */
    T* ptr() const {return(v_);}

     /**
    * @brief      Constant pointer to storage buffer
    * @return Pointer to storage
    */
    const T* const_ptr() const {return(v_);}

protected:
    T* const v_;
    const vector_length_t nb_rows_;
    const vector_length_t nb_cols_;
};

/*

When the stride is not known at build time AND different
from the nb_cols_

*/

/** @brief Dynamic Matrix View
 *  @tparam T Type of the scalar
 *  
 *  This template is used for dynamic matrix (stride not known
 *  at build time) and when we do not know if stride == number of
 *  columns.
 *  When stride is different from number of columns, the matrix cannot
 *  be seen as a vector.
 */
template<typename T>
struct MatrixView<T,DYNAMIC>
{
    /** @brief Number of rows
    *  @return Number of rows
    */
   vector_length_t rows() const {return(nb_rows_);}

   /** @brief Number of columns
    *  @return Number of columns
    */
   vector_length_t columns() const {return(nb_cols_);}

    /** @brief Number of stride
    *  @return Number of stride
    */
   uint32_t stride() const {return(stride_);}

   /** @brief Create matrix view on a buffer (buffer not owned by the view)
    * @param v buffer
    * @param rows number of rows
    * @param cols number of columns
    * @param stride stride
    */
   explicit MatrixView(T* v,
              const vector_length_t rows,
              const vector_length_t cols,
              const uint32_t stride):
   v_(v),nb_rows_(rows),nb_cols_(cols),stride_(stride){};

   /** @brief Create matrix view on vector (vector not owned by the view)
    * @param v vector
    * @param rows number of rows
    * @param cols number of columns
    * @param stride stride
    */
   explicit MatrixView(const Vector_Base<T> &v,
              const vector_length_t rows,
              const vector_length_t cols,
              const uint32_t stride):
   v_(v.ptr()),nb_rows_(rows),nb_cols_(cols),stride_(stride){};

   virtual ~MatrixView() {};

   MatrixView(const MatrixView& other):
    v_(other.v_),
    nb_rows_(other.nb_rows_),nb_cols_(other.nb_cols_),stride_(other.stride_){};

   MatrixView(MatrixView&& other) :
    v_(other.v_),
    nb_rows_(other.nb_rows_),nb_cols_(other.nb_cols_),stride_(other.stride_){};

   
   MatrixView& operator=(const MatrixView& other) = delete;
   MatrixView& operator=(MatrixView&& other)  = delete;

   /** @brief Access matrix view element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   T& operator()(const index_t r,const index_t c)
   {
     return(v_[r*stride()+c]);
   }

    /** @brief Access matrix view element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   T const operator()(const index_t r,const index_t c) const
   {
     return(v_[r*stride()+c]);
   }


   /** @brief Assign matrix view from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator=(const _Expr<Derived>&other)
   {
      eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   }

   /** @brief Assign matrix view from constant
    * @param val The constant
    * @return the matrix
    * 
    */
   MatrixView& operator=(const T val)
   {
        _Fill2D(*this,val,rows(),columns(),CURRENT_ARCH);
       
        return(*this);
   }

 
    /** @brief Add matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator +=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this + other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Add matrix from matrix view
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator +=(const MatrixView& other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Add constant to matrix view
    * @param other The constant
    * @return the matrix
    * 
    */
   MatrixView& operator +=(const T other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator -=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this - other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract matrix view
    * @param other Other matrix view
    * @return the matrix
    * 
    */
   MatrixView& operator -=(const MatrixView& other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract constant
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator -=(const T other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Elementwise multiply matrix view with expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator *=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this * other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Elementwise multiply matrix view with matrix view
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator *=(const MatrixView& other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Elementwise multiply matrix view constant
    * @param other constant
    * @return the matrix
    * 
    */
   MatrixView& operator *=(const T other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Display the matrix content for debug purpose
    * @param stream Output stream
    * @param other The matrix to display
    * @return the stream
    * 
    */
  friend std::ostream& operator<< (std::ostream& stream, const MatrixView& other) {
        using DT = typename number_traits<T>::display_type;
        for(index_t row=0;row<other.rows();row++)
        {
           for(index_t col=0;col<other.columns();col++)
           {
                stream << (DT)other(row,col)<< " , ";
           }
           stream << "\r\n";
        }
        stream << "\r\n";
        return(stream);
    }

    /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   VectorView<T,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

    /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   VectorView<T,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

    /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   const VectorView<T,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

    /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   const VectorView<T,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

   
    /** @brief Create a column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0)
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*rows(),stride()*CS));
   }

  
    /** @brief Create a column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @param stop Stop index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*stop,stride()*CS));
   }

   /** @brief Create a constant column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*rows(),stride()*CS));
   }

    /** @brief Create a constant column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @param stop Stop index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*stop,stride()*CS));
   }

   #if defined(HAS_VECTOR)
    //! Type of vectors for a vector architecture and for scalar datatype P
    using VectorType = typename vector_traits<T>::vector;
    

    /**
    * @brief   %Vector store at a given row,column position
    *
    * @param row row index
    * @param col column index
    * @param val %Vector value
    * 
    * On an architecture supporting vectors, if the scalar datatype T
    * has a corresponding vector datatype, this function stores a vector
    * value at row,column in this matrix.
    */
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        inner::vstore1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val);
    }

#if defined(HAS_PREDICATED_LOOP)
     /**
    * @brief   %Vector store at a given row,column position with predicated tail
    *
    * @param row row index
    * @param col column index
    * @param remaining Number of remaining samples in the loop
    * @param val Vector value to write at index i with tail predication
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function stores a vector value at row,column index in this matrix datatype
    * with predication
    */
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val,remaining,inner::vctpq<T>::mk(remaining));
    }

     /**
    * @brief   %Vector operation at a given row,column position with predicated tail
    *
    * @param row row index
    * @param col column index
    * @param remaining Number of remaining samples in the loop
    * @return the vector result of the operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function compute an operation at row,column index in this matrix datatype
    * with predication
    */
    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif
    
     /**
    * @brief   %Vector operation at a given row,column position
    *
    * @param row row index
    * @param col column index
    * @return the vector result of the operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function compute an operation at row,column index in this matrix datatype
    */
    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(inner::vload1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col])));
    }
#endif

    /** @brief Fill diagonal of a matrix with a vector
    * @tparam VA Vector datatype
    * @param a Vector for initializing the diagonal
    * 
    */
    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            compatible_element<VA,T>(),bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,a->length());
    }

    /** @brief Create the transposed matrix
    * @return a matrix
    *   
    */
    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> transpose() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

     /** @brief Create a matrix of same type
    * @return a matrix
    *   
    */
    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> create() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(rows(),columns());
       return(res);
    }

      /**
    * @brief      Pointer to storage buffer
    * @return Pointer to storage
    */
    T* ptr() const {return(v_);}

     /**
    * @brief      Constant pointer to storage buffer
    * @return Pointer to storage
    */
    const T* const_ptr() const {return(v_);}


protected:
    T* const v_;
    const vector_length_t nb_rows_;
    const vector_length_t nb_cols_;
    const uint32_t stride_;
};

/*


Dynamic but with stride == nb_cols_

*/

/** @brief Dynamic Matrix View
 *  @tparam T Type of the scalar
 *  
 *  This template is used for dynamic matrix (stride not known
 *  at build time) and when we do know that stride == number of
 *  columns.
 *  When stride is equal to the number of columns, the matrix can
 *  be seen as a vector and it enables to use the vector eval loop
 *  in the operator fusion mechanism.
 *  
 *  Those matrix views are created by expression when a reference to
 *  a matrix is used in the expression tree (to avoid copying the matrix).
 *  In this case, we do know that the matrix view is the full matrix and thus
 *  stride == number of columns
 */
template<typename T>
struct MatrixView<T,CONSTRAINED_DYNAMIC>:VectorView<T,1>
{
    /** @brief Number of rows
    *  @return Number of rows
    */
   vector_length_t rows() const {return(nb_rows_);}

    /** @brief Number of columns
    *  @return Number of columns
    */
   vector_length_t columns() const {return(nb_cols_);}

    /** @brief Number of stride
    *  @return Number of stride
    */
   uint32_t stride() const {return(nb_cols_);}

   /** @brief Create matrix view on a buffer (buffer not owned by the view)
    * @param v buffer
    * @param rows number of rows
    * @param cols number of columns
    */
   explicit MatrixView(T* v,
              const vector_length_t rows,
              const vector_length_t cols):
   VectorView<T,1>(v,0,rows*cols),
   nb_rows_(rows),nb_cols_(cols){};

   /** @brief Create matrix view on vector (vector not owned by the view)
    * @param v vector
    * @param rows number of rows
    * @param cols number of columns
    */
   explicit MatrixView(const Vector_Base<T> &v,
              const vector_length_t rows,
              const vector_length_t cols):
   VectorView<T,1>(v.ptr(),0,rows*cols),
   nb_rows_(rows),nb_cols_(cols){};

   virtual ~MatrixView() {};

   MatrixView(const MatrixView& other):
    VectorView<T,1>(other),
    nb_rows_(other.nb_rows_),nb_cols_(other.nb_cols_){};

   MatrixView(MatrixView&& other) :
    VectorView<T,1>(std::forward<MatrixView>(other)),
    nb_rows_(other.nb_rows_),nb_cols_(other.nb_cols_){};

   
   MatrixView& operator=(const MatrixView& other) = delete;
   MatrixView& operator=(MatrixView&& other)  = delete;

   /** @brief Access matrix view element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   T& operator()(const index_t r,const index_t c)
   {
     return(&(*this)[r*stride()+c]);
   }

    /** @brief Access matrix view element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   T const operator()(const index_t r,const index_t c) const
   {
     return((*this)[r*stride()+c]);
   }

    /** @brief Assign matrix view from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator=(const _Expr<Derived>&other)
   {
      eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   }

   /** @brief Assign matrix view from constant
    * @param val The constant
    * @return the matrix
    * 
    */
   MatrixView& operator=(const T val)
   {
        _Fill2D(*this,val,rows(),columns(),CURRENT_ARCH);
       
        return(*this);
   }


     /** @brief Add matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator +=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this + other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Add matrix from matrix view
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator +=(const MatrixView& other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   /** @brief Add constant to matrix view
    * @param other The constant
    * @return the matrix
    * 
    */
   MatrixView& operator +=(const T other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator -=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this - other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract matrix view
    * @param other Other matrix view
    * @return the matrix
    * 
    */
   MatrixView& operator -=(const MatrixView& other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Subtract constant
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator -=(const T other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   /** @brief Elementwise multiply matrix view with expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other expression
    * @return the matrix
    * 
    */
   template<typename Derived>
   MatrixView& operator *=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this * other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   /** @brief Elementwise multiply matrix view with matrix view
    * @param other Other matrix
    * @return the matrix
    * 
    */
   MatrixView& operator *=(const MatrixView& other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

    /** @brief Elementwise multiply matrix view constant
    * @param other constant
    * @return the matrix
    * 
    */
   MatrixView& operator *=(const T other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Display the matrix content for debug purpose
    * @param stream Output stream
    * @param other The matrix to display
    * @return the stream
    * 
    */
  friend std::ostream& operator<< (std::ostream& stream, const MatrixView& other) {
        using DT = typename number_traits<T>::display_type;
        for(index_t row=0;row<other.rows();row++)
        {
           for(index_t col=0;col<other.columns();col++)
           {
                stream << (DT)other(row,col)<< " , ";
           }
           stream << "\r\n";
        }
        stream << "\r\n";
        return(stream);
    }

    /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   VectorView<T,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+columns()));
   }

   /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   VectorView<T,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+stop));
   }

    /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   const VectorView<T,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+columns()));
   }

    /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   const VectorView<T,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+stop));
   }

    /** @brief Create a column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0)
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*rows(),stride()*CS));
   }

    /** @brief Create a column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @param stop Stop index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*stop,stride()*CS));
   }

    /** @brief Create a constant column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*rows(),stride()*CS));
   }

   /** @brief Create a constant column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @param stop Stop index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*stop,stride()*CS));
   }

   #if defined(HAS_VECTOR)
    //! Type of vectors for a vector architecture and for scalar datatype P
    using VectorType = typename vector_traits<T>::vector;

     /**
    * @brief   %Vector store at a given row,column position
    *
    * @param row row index
    * @param col column index
    * @param val %Vector value
    * 
    * On an architecture supporting vectors, if the scalar datatype T
    * has a corresponding vector datatype, this function stores a vector
    * value at row,column in this matrix.
    */
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        inner::vstore1<1>((typename std::remove_cv<T>::type*)(ptr(row*stride() + col)),val);
    }

#if defined(HAS_PREDICATED_LOOP)
     /**
    * @brief   %Vector store at a given row,column position with predicated tail
    *
    * @param row row index
    * @param col column index
    * @param remaining Number of remaining samples in the loop
    * @param val Vector value to write at index i with tail predication
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function stores a vector value at row,column index in this matrix datatype
    * with predication
    */
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<T>::type*)(ptr(row*stride() + col)),val,remaining,inner::vctpq<T>::mk(remaining));
    }

     /**
    * @brief   %Vector operation at a given row,column position with predicated tail
    *
    * @param row row index
    * @param col column index
    * @param remaining Number of remaining samples in the loop
    * @return the vector result of the operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function compute an operation at row,column index in this matrix datatype
    * with predication
    */
    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<T>::type*)(VectorView<T,1>::ptr(row*stride() + col)),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif
    
     /**
    * @brief   %Vector operation at a given row,column position
    *
    * @param row row index
    * @param col column index
    * @return the vector result of the operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function compute an operation at row,column index in this matrix datatype
    */
    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(inner::vload1<1>((typename std::remove_cv<T>::type*)(VectorView<T,1>::ptr(row*stride() + col))));
    }
#endif

     /** @brief Fill diagonal of a matrix with a vector
    * @tparam VA Vector datatype
    * @param a Vector for initializing the diagonal
    * 
    */
    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            compatible_element<VA,T>(),bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,a->length());
    }

     /** @brief Create the transposed matrix
    * @return a matrix
    *   
    */
    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> transpose() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

      /** @brief Create a matrix of same type
    * @return a matrix
    *   
    */
    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> create() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(rows(),columns());
       return(res);
    }


protected:
    const vector_length_t nb_rows_;
    const vector_length_t nb_cols_;
};

/*! @} */

}