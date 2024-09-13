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

namespace arm_cmsis_dsp {

/** \addtogroup Matrix 
 *  @{
 */

/********************
 * 
 *  MATRIX
 * 
 ********************/

/**
* @brief   Slice
*/
struct Slice
{
    /**
    * @brief   Create new slice object
    * @param s start index
    * @param e stop index
    *
    */
    Slice(const index_t s,const index_t e):start(s),stop(e){};

    //! Start index
    const index_t start;

    //! Stop index
    const index_t stop;
};

/** @brief Matrix
 *  @tparam P Type of the scalar
 *  @tparam R Number of rows
 *  @tparam C Number of columns
 *  @tparam Allocator Memory allocator
 */
template<typename P,int R=DYNAMIC,int C=DYNAMIC,
         template<int> typename Allocator = TMP_ALLOC>
struct Matrix:Vector<P,R*C,Allocator>
{
   /** @brief Number of rows
    *  @return Number of rows
    */
   constexpr vector_length_t rows() const {return(R);}

   /** @brief Number of columns
    *  @return Number of columns
    */
   constexpr vector_length_t columns() const {return(C);}

   /** @brief Number of stride
    *  @return Number of stride
    */
   constexpr uint32_t stride() const {return(C);}


   /** @brief Create matrix
    */
   Matrix():Vector<P,R*C,Allocator>(){};

  /** @brief Create matrix
   * @param init_val Initialization value
   */
   explicit Matrix(P init_val):Vector<P,R*C,Allocator>(init_val){};

   Matrix(const Matrix& other) = default;
   Matrix(Matrix&& other) = default;

   /** @brief Create matrix from another matrix using different memory allocator
    * @tparam OtherAllocator other memory allocator
    * @param other Other matrix
    */
   template<template<int> typename OtherAllocator>
   explicit Matrix(const Matrix<P,R,C,OtherAllocator>& other):Vector<P,R*C,Allocator>()
   {
        eval(*this,+other,(vector_length_t)(R*C),CURRENT_ARCH);
   };

   /* Applies only when the AST does not contain any dynamic MatrixView */

   /** @brief Create matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * 
    * Only applies when the expression does not contain any MatrixView since
    * matrix view may have a stride and cannot be used as vectors.
    */
   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix(const _Expr<Derived>& other):Vector<P,R*C,Allocator>(other)
   {
   };

   /* Applies only when AST is containing any dynamic MatrixView */

   /** @brief Create matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * 
    * Applies when contain a matrix view that has a stride and thus force a 2D
    * evaluation loop.
    */
   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix(const _Expr<Derived>& other):Vector<P,R*C,Allocator>()
   {
     eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
   };

  /** @brief Assign matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * @return the matrix
    * 
    * Applies when expression does not contain matrix view
    */
   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
        eval(*this,other.derived(),(vector_length_t)R*C,CURRENT_ARCH);
        return(*this);
   }

   /* Applies only when AST is containing any dynamic MatrixView */
  /** @brief Assign matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * @return the matrix
    * 
    * Applies when contain a matrix view that has a stride and thus force a 2D
    * evaluation loop.
    */
   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
       eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
       return(*this);
   }

  /** @brief Create a matrix view
    * @param rs start row
    * @param cs start column
    * @return matrix view
    *
    */
   MatrixView<P,C> sub(const index_t rs,const index_t cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols));
   }

  /** @brief Create a constant matrix view
    * @param rs start row
    * @param cs start column
    * @return matrix view
    *
    */
   const MatrixView<P,C> sub(const index_t rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols));
   }
   
  /** @brief Create a matrix view
    * @param rs Row slice (start and end row)
    * @param cs start column
    * @return matrix view
    *
    */
   MatrixView<P,C> sub(const Slice &rs,const index_t cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols));
   }

  /** @brief Create a constant matrix view
    * @param rs Row slice (start and end row)
    * @param cs start column
    * @return matrix view
    *
    */
   const MatrixView<P,C> sub(const Slice &rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols));
   }


  /** @brief Create a  matrix view
    * @param rs Row start index
    * @param cs Column slice
    * @return matrix view
    *
    */
   MatrixView<P,C> sub(const index_t rs,const Slice &cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols));
   }

  /** @brief Create a constant matrix view
    * @param rs Row start index
    * @param cs Column slice
    * @return matrix view
    *
    */
   const MatrixView<P,C> sub(const index_t rs,const Slice &cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols));
   }

   /** @brief Create a matrix view
    * @param rs Row slice
    * @param cs Column slice
    * @return matrix view
    *
    */
   MatrixView<P,C> sub(const Slice& rs,const Slice& cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols));
   }

   /** @brief Create a constant matrix view
    * @param rs Row slice
    * @param cs Column slice
    * @return matrix view
    *
    */
   const MatrixView<P,C> sub(const Slice& rs,const Slice& cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols));
   }

   /** @brief Create a matrix view
    * @param rs Row start
    * @param re Row end
    * @param cs Column start
    * @param ce Column end
    * @return matrix view
    *
    */
   MatrixView<P,C> sub(const index_t rs,
                       const index_t re,
                       const index_t cs,
                       const index_t ce)
   {
        const vector_length_t nb_rows = re - rs;
        const vector_length_t nb_cols = ce - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols));
   }

   /** @brief Create a constant matrix view
    * @param rs Row start
    * @param re Row end
    * @param cs Column start
    * @param ce Column end
    * @return matrix view
    *
    */
   const MatrixView<P,C> sub(const index_t rs,
                             const index_t re,
                             const index_t cs,
                             const index_t ce) const
   {
        const vector_length_t nb_rows = re - rs;
        const vector_length_t nb_cols = ce - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols));
   }


   Matrix& operator=(const Matrix& other) = default;
   
   Matrix& operator=(Matrix&& other) = default;

   /** @brief Access matrix element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   P& operator()(const index_t r,const index_t c)
   {
     return(Vector_Base<P>::ptr()[r*C+c]);
   }

   /** @brief Access matrix element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   P& operator()(const index_t r,const index_t c) const
   {
     return(Vector_Base<P>::ptr()[r*C+c]);
   }


   /**
    * @brief  Display the matrix content for debug purpose
    * @param stream Output stream
    * @param other The matrix to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Matrix& other) {
        using DT = typename number_traits<P>::display_type;
        int c=0;
        for(index_t k=0;k<other.length();k++)
        {
           stream << (DT)other[k] << " , ";
           c++;
           if (c == other.columns())
           {
              c=0;
              stream << "\r\n";
           }
        }
        stream << "\r\n";
        return(stream);
    }

   /** @brief Create a row view vector with stride
    * @tparam S stride
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    *             Default is number of columns
    * @return row view vector
    *
    */
   template<int S=1>
   VectorView<P,S> row(const index_t i,const index_t start=0,const index_t stop=C)
   {
     return(VectorView<P,S>(*this,i*stride()+start,i*stride()+stop));
   }

   /** @brief Create a constant row view vector with stride
    * @tparam S stride
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    *             Default is number of columns
    * @return row view vector
    *
    */
   template<int S=1>
   const VectorView<P,S> row(const index_t i,const index_t start=0,const index_t stop=C) const
   {
     return(VectorView<P,S>(*this,i*stride()+start,i*stride()+stop));
   }

   /** @brief Create a column view vector with stride
    * @tparam S stride
    * @param i column index
    * @param start Start index in row
    * @param stop Stop index in row
    *             Default is number of rows
    * @return column view vector
    *
    */
   template<int CS=1,int S=C>
   VectorView<P,CS*S> col(const index_t i,const index_t start=0,const index_t stop=R)
   {
     return(VectorView<P,CS*S>(*this,i+stride()*start,i+stride()*stop));
   }

   /** @brief Create a constant column view vector with stride
    * @tparam S stride
    * @param i column index
    * @param start Start index in row
    * @param stop Stop index in row
    *             Default is number of rows
    * @return column view vector
    *
    */
   template<int CS=1,int S=C>
   const VectorView<P,CS*S> col(const index_t i,const index_t start=0,const index_t stop=R) const
   {
     return(VectorView<P,CS*S>(*this,i+stride()*start,i+stride()*stop));
   }

    /** @brief Create a diagonal matrix
    * @tparam RA Number of rows
    * @tparam CA Number of columns
    * @tparam VA Vector datatype
    * @param a Vector for initializing the diagonal
    * @return a matrix
    * 
    * Only exists when RA == CA and the size is known at built time
    *
    */
    template<int RA=R,int CA=C,typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            (RA == CA) && (RA>0) &&
            compatible_element<VA,P>(),bool>::type = true>
    static Matrix<P,RA,CA,Allocator> diagonal(const VA& a)
    {
       Matrix<P,RA,CA,Allocator> res;
       _diagonal(res,a,RA);
       return(res);
    }

    /** @brief Fill diagonal of a matrix with a vector
    * @tparam RA Number of rows
    * @tparam CA Number of columns
    * @tparam VA Vector datatype
    * @param a Vector for initializing the diagonal
    * 
    * Only exists when RA == CA and the size is known at built time
    *
    */
    template<int RA=R,int CA=C,typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            (RA == CA) && (RA>0) &&
            compatible_element<VA,P>(),bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,RA);
    }

    /** @brief Create an identity matrix
    * @tparam RA Number of rows
    * @tparam CA Number of columns
    * @return a matrix
    * 
    * Only exists when RA == CA and the size is known at built time
    *
    */
    template<int RA=R, int CA=C,
            typename std::enable_if< 
            (RA == CA) && (RA>0),bool>::type = true>
    static Matrix<P,RA,CA,Allocator> identity()
    {
       Matrix<P,RA,CA,Allocator> res;
       _identity(res,RA);
       return(res);
    }

    /** @brief Create a matrix of same type
    * @return a matrix
    *   
    */
    Matrix<P,R,C,Allocator> create() const
    {
       Matrix<P,R,C,Allocator> res;
       return(res);
    }

    /** @brief Create the transposed matrix
    * @return a matrix
    *   
    */
    Matrix<P,C,R,Allocator> transpose() const
    {
       Matrix<P,C,R,Allocator> res;
       transposeTo(res,*this);
       return(res);
    }

#if defined(HAS_VECTOR)
    //! Type of vectors for a vector architecture and for scalar datatype P
    using VectorType = typename vector_traits<P>::vector;

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
        Vector_Base<P>::vector_store(row*C + col,val);
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
       Vector_Base<P>::vector_store_tail(row*C + col,remaining,val);
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
        return(Vector_Base<P>::vector_op_tail(row*C + col,remaining));
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
        return(Vector_Base<P>::vector_op(row*C + col));
    }
#endif

};

/** @brief Matrix
 *  @tparam P Type of the scalar
 *  @tparam Allocator Memory allocator
 */
template<typename P,
         template<int> typename Allocator>
struct Matrix<P,DYNAMIC,DYNAMIC,Allocator>:Vector<P,DYNAMIC,Allocator>
{
    /** @brief Number of rows
    *  @return Number of rows
    */
   vector_length_t rows() const {return(rows_);}

   /** @brief Number of columns
    *  @return Number of columns
    */
   vector_length_t columns() const {return(columns_);}

    /** @brief Number of stride
    *  @return Number of stride
    */
   uint32_t stride() const {return(columns_);}

   /** @brief Create matrix
    * @param r number of rows
    * @param c number of columns
    */
   explicit Matrix(vector_length_t r,vector_length_t c):
   Vector<P,DYNAMIC,Allocator>(r*c),rows_(r),columns_(c){};

   /** @brief Create matrix
    * @param r number of rows
    * @param c number of columns
    * @param init_val Initialization value
    */
   explicit Matrix(vector_length_t r,vector_length_t c,P init_val):
   Vector<P,DYNAMIC,Allocator>(r*c,init_val),rows_(r),columns_(c){};

   Matrix(const Matrix& other) = default;
   Matrix(Matrix&& other) = default;

   /** @brief Access matrix element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   P& operator()(const index_t r,const index_t c)
   {
     return(Vector_Base<P>::ptr()[r*columns()+c]);
   }

   /** @brief Access matrix element at given position
    * @param r Row index
    * @param c Column index
    * @return reference to element
    *
    */
   P& operator()(const index_t r,const index_t c) const
   {
     return(Vector_Base<P>::ptr()[r*columns()+c]);
   }

    /** @brief Create matrix from another matrix using different memory allocator
     * @tparam RK Number of rows
     * @tparam CK Number of columns
    * @tparam OtherAllocator other memory allocator
    * @param other Other matrix
    */
   template<int RK,int CK,template<int> typename OtherAllocator>
   explicit Matrix(const Matrix<P,RK,CK,OtherAllocator>& other):
   Vector<P,DYNAMIC,Allocator>(other.rows()*other.columns()),
   rows_(other.rows()),columns_(other.columns())
   {
        if ((other.rows() == rows()) && (other.columns() == columns()))
        {
          eval(*this,+other,(vector_length_t)(other.rows()*other.columns()),CURRENT_ARCH);
        }
   };

   /** @brief Create matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * 
    * Only applies when the expression does not contain any MatrixView since
    * matrix view may have a stride and cannot be used as vectors.
    */
   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix(const _Expr<Derived>& other):Vector<P,DYNAMIC,Allocator>(other),
   rows_(other.rows()),columns_(other.columns())
   {
   };

    /** @brief Create matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * 
    * Applies when contain a matrix view that has a stride and thus force a 2D
    * evaluation loop.
    */
   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix(const _Expr<Derived>& other):
   Vector<P,DYNAMIC,Allocator>(other.rows()*other.columns()),
   rows_(other.rows()),columns_(other.columns())
   {
        eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
   };

  /** @brief Assign matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * @return the matrix
    * 
    * Applies when expression does not contain matrix view
    */
   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
      eval(*this,other.derived(),rows()*columns(),CURRENT_ARCH);
      return(*this);
   };


    /** @brief Assign matrix from expression
    * @tparam Derived Datatype representing the abstract syntax tree of the expression
    * @param other Other matrix
    * @return the matrix
    * 
    * Applies when contain a matrix view that has a stride and thus force a 2D
    * evaluation loop.
    */
   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
         eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
         return(*this);
   };

   Matrix& operator=(const Matrix& other) = default;
   
   Matrix& operator=(Matrix&& other) = default;

   /**
    * @brief  Display the matrix content for debug purpose
    * @param stream Output stream
    * @param other The matrix to display
    * @return the stream
    * 
    */
   friend std::ostream& operator<< (std::ostream& stream, const Matrix& other) {
        using DT = typename number_traits<P>::display_type;
        int c=0;
        for(index_t k=0;k<other.length();k++)
        {
           stream << (DT)other[k] << " , ";
           c++;
           if (c == other.columns())
           {
              c=0;
              stream << "\r\n";
           }
        }
        stream << "\r\n";
        return(stream);
    }

    /** @brief Create a diagonal matrix
    * @tparam VA Vector datatype
    * @param a Vector for initializing the diagonal
    * @return a matrix
    * 
    *
    */
    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            compatible_element<VA,P>(),bool>::type = true>
    static Matrix<P,DYNAMIC,DYNAMIC,Allocator> diagonal(const VA& a)
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(a.length(),a.length());
       _diagonal(res,a,a.length());
       return(res);
    }

    /** @brief Fill diagonal of a matrix with a vector
    * @tparam VA Vector datatype
    * @param a Vector for initializing the diagonal
    * 
    */
    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            compatible_element<VA,P>(),bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,this->length());
    }

     /** @brief Create an identity matrix
      * @param l Matrix dimension (l x l)
    * @return a matrix
    *     
    */
    static Matrix<P,DYNAMIC,DYNAMIC,Allocator> identity(const vector_length_t l)
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(l,l);
       _identity(res,l);
       return(res);
    }

    /** @brief Create a matrix of same type
    * @return a matrix
    *   
    */
    Matrix<P,DYNAMIC,DYNAMIC,Allocator> create() const
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(rows(),columns());
       return(res);
    }

    /** @brief Create the transposed matrix
    * @return a matrix
    *   
    */
    Matrix<P,DYNAMIC,DYNAMIC,Allocator> transpose() const
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

    /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   VectorView<P,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+this->columns()));
   }

   /** @brief Create a row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   VectorView<P,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+stop));
   }

   /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @return row view vector
    *
    */
   const VectorView<P,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+this->columns()));
   }

   /** @brief Create a constant row view with stride 1
    * @param i row index
    * @param start Start index in row
    * @param stop Stop index in row
    * @return row view vector
    *
    */
   const VectorView<P,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+stop));
   }

   /** @brief Create a column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   VectorView<P,DYNAMIC> col(const index_t i,const index_t start=0)
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*this->rows(),this->stride()*CS));
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
   VectorView<P,DYNAMIC> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*stop,this->stride()*CS));
   }

   /** @brief Create a constant column view vector
    * @tparam CS column stride
    * @param i column index
    * @param start Start index in column
    * @return column view vector
    *
    */
   template<int CS=1>
   const VectorView<P,DYNAMIC> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*this->rows(),this->stride()*CS));
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
   const VectorView<P,DYNAMIC> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*stop,this->stride()*CS));
   }

#if defined(HAS_VECTOR)
    //! Type of vectors for a vector architecture and for scalar datatype P
    using VectorType = typename vector_traits<P>::vector;

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
        Vector_Base<P>::vector_store(row*stride() + col,val);
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
       Vector_Base<P>::vector_store_tail(row*stride() + col,remaining,val);
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
        return(Vector_Base<P>::vector_op_tail(row*stride() + col,remaining));
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
        return(Vector_Base<P>::vector_op(row*stride() + col));
    }
#endif

   /** @brief Create a matrix view
    * @param rs start row
    * @param cs start column
    * @return matrix view
    *
    */
   MatrixView<P,DYNAMIC> sub(const index_t rs,const index_t cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols,stride()));
   }

    /** @brief Create a constant matrix view
    * @param rs start row
    * @param cs start column
    * @return matrix view
    *
    */
   const MatrixView<P,DYNAMIC> sub(const index_t rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols,stride()));
   }
   
   /** @brief Create a matrix view
    * @param rs Row slice (start and end row)
    * @param cs start column
    * @return matrix view
    *
    */
   MatrixView<P,DYNAMIC> sub(const Slice &rs,const index_t cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols,stride()));
   }

    /** @brief Create a constant matrix view
    * @param rs Row slice (start and end row)
    * @param cs start column
    * @return matrix view
    *
    */
   const MatrixView<P,DYNAMIC> sub(const Slice &rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols,stride()));
   }

   /** @brief Create a  matrix view
    * @param rs Row start index
    * @param cs Column slice
    * @return matrix view
    *
    */
   MatrixView<P,DYNAMIC> sub(const index_t rs,const Slice &cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

   
  /** @brief Create a constant matrix view
    * @param rs Row start index
    * @param cs Column slice
    * @return matrix view
    *
    */
   const MatrixView<P,DYNAMIC> sub(const index_t rs,const Slice &cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

    /** @brief Create a matrix view
    * @param rs Row slice
    * @param cs Column slice
    * @return matrix view
    *
    */
   MatrixView<P,DYNAMIC> sub(const Slice& rs,const Slice& cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

   /** @brief Create a constant matrix view
    * @param rs Row slice
    * @param cs Column slice
    * @return matrix view
    *
    */
   const MatrixView<P,DYNAMIC> sub(const Slice& rs,const Slice& cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

   /** @brief Create a matrix view
    * @param rs Row start
    * @param re Row end
    * @param cs Column start
    * @param ce Column end
    * @return matrix view
    *
    */
   MatrixView<P,DYNAMIC> sub(const index_t rs,
                             const index_t re,
                             const index_t cs,
                             const index_t ce)
   {
        const vector_length_t nb_rows = re - rs;
        const vector_length_t nb_cols = ce - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols,stride()));
   }

    /** @brief Create a constant matrix view
    * @param rs Row start
    * @param re Row end
    * @param cs Column start
    * @param ce Column end
    * @return matrix view
    *
    */
   const MatrixView<P,DYNAMIC> sub(const index_t rs,
                                   const index_t re,
                                   const index_t cs,
                                   const index_t ce) const
   {
        const vector_length_t nb_rows = re - rs;
        const vector_length_t nb_cols = ce - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols,stride()));
   }
  
protected:
    vector_length_t rows_,columns_;
};


/*! @} */
}