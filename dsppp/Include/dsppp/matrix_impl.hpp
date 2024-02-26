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

namespace arm_cmsis_dsp {

/** \addtogroup Matrix 
 *  @{
 */

/********************
 * 
 *  MATRIX
 * 
 ********************/

struct Slice
{
    Slice(const index_t s,const index_t e):start(s),stop(e){};

    const index_t start;
    const index_t stop;
};

template<typename P,int R=DYNAMIC,int C=DYNAMIC,
         template<int> typename Allocator = TMP_ALLOC>
struct Matrix:Vector<P,R*C,Allocator>
{
   constexpr vector_length_t rows() const {return(R);}
   constexpr vector_length_t columns() const {return(C);}
   constexpr uint32_t stride() const {return(C);}


   Matrix():Vector<P,R*C,Allocator>(){};
   explicit Matrix(P init_val):Vector<P,R*C,Allocator>(init_val){};

   Matrix(const Matrix& other) = default;
   Matrix(Matrix&& other) = default;

   template<template<int> typename OtherAllocator>
   explicit Matrix(const Matrix<P,R,C,OtherAllocator>& other):Vector<P,R*C,Allocator>()
   {
        eval(*this,+other,(vector_length_t)(R*C),CURRENT_ARCH);
   };

   /* Applies only when the AST does not contain any dynamic MatrixView */
   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix(const _Expr<Derived>& other):Vector<P,R*C,Allocator>(other)
   {
   };

   /* Applies only when AST is containing any dynamic MatrixView */
   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix(const _Expr<Derived>& other):Vector<P,R*C,Allocator>()
   {
     eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
   };

   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
        eval(*this,other.derived(),(vector_length_t)R*C,CURRENT_ARCH);
        return(*this);
   }

   /* Applies only when AST is containing any dynamic MatrixView */
   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
       eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
       return(*this);
   }

   MatrixView<P,C> sub(const index_t rs,const index_t cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols));
   }

   const MatrixView<P,C> sub(const index_t rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols));
   }
   
   MatrixView<P,C> sub(const Slice &rs,const index_t cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols));
   }

   const MatrixView<P,C> sub(const Slice &rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols));
   }

   MatrixView<P,C> sub(const index_t rs,const Slice &cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols));
   }

   const MatrixView<P,C> sub(const index_t rs,const Slice &cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols));
   }

   MatrixView<P,C> sub(const Slice& rs,const Slice& cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols));
   }

   const MatrixView<P,C> sub(const Slice& rs,const Slice& cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols));
   }

   MatrixView<P,C> sub(const index_t rs,
                       const index_t re,
                       const index_t cs,
                       const index_t ce)
   {
        const vector_length_t nb_rows = re - rs;
        const vector_length_t nb_cols = ce - cs;

        return(MatrixView<P,C>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols));
   }

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

   P& operator()(const index_t r,const index_t c)
   {
     return(Vector_Base<P>::ptr()[r*C+c]);
   }

   P& operator()(const index_t r,const index_t c) const
   {
     return(Vector_Base<P>::ptr()[r*C+c]);
   }


   friend std::ostream& operator<< (std::ostream& stream, const Matrix& other) {
        int c=0;
        for(index_t k=0;k<other.length();k++)
        {
           stream << other[k] << " , ";
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

   template<int S=1>
   VectorView<P,S> row(const index_t i,const index_t start=0,const index_t stop=C)
   {
     return(VectorView<P,S>(*this,i*stride()+start,i*stride()+stop));
   }

   template<int S=1>
   const VectorView<P,S> row(const index_t i,const index_t start=0,const index_t stop=C) const
   {
     return(VectorView<P,S>(*this,i*stride()+start,i*stride()+stop));
   }

  
   template<int CS=1,int S=C>
   VectorView<P,CS*S> col(const index_t i,const index_t start=0,const index_t stop=R)
   {
     return(VectorView<P,CS*S>(*this,i+stride()*start,i+stride()*stop));
   }

   template<int CS=1,int S=C>
   const VectorView<P,CS*S> col(const index_t i,const index_t start=0,const index_t stop=R) const
   {
     return(VectorView<P,CS*S>(*this,i+stride()*start,i+stride()*stop));
   }

    template<int RA=R,int CA=C,typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            (RA == CA) && (RA>0) &&
            SameElementType<VA,P>::value,bool>::type = true>
    static Matrix<P,RA,CA,Allocator> diagonal(const VA& a)
    {
       Matrix<P,RA,CA,Allocator> res;
       _diagonal(res,a,RA);
       return(res);
    }

    template<int RA=R,int CA=C,typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            (RA == CA) && (RA>0) &&
            SameElementType<VA,P>::value,bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,RA);
    }

    template<int RA=R, int CA=C,
            typename std::enable_if< 
            (RA == CA) && (RA>0),bool>::type = true>
    static Matrix<P,RA,CA,Allocator> identity()
    {
       Matrix<P,RA,CA,Allocator> res;
       _identity(res,RA);
       return(res);
    }

    Matrix<P,R,C,Allocator> create() const
    {
       Matrix<P,R,C,Allocator> res;
       return(res);
    }

    Matrix<P,C,R,Allocator> transpose() const
    {
       Matrix<P,C,R,Allocator> res;
       transposeTo(res,*this);
       return(res);
    }

#if defined(HAS_VECTOR)
    using VectorType = typename vector_traits<P>::vector;
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        Vector_Base<P>::vector_store(row*C + col,val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
       Vector_Base<P>::vector_store_tail(row*C + col,remaining,val);
    }

    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(Vector_Base<P>::vector_op_tail(row*C + col,remaining));
    }
#endif

    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(Vector_Base<P>::vector_op(row*C + col));
    }
#endif

};

template<typename P,
         template<int> typename Allocator>
struct Matrix<P,DYNAMIC,DYNAMIC,Allocator>:Vector<P,DYNAMIC,Allocator>
{
   vector_length_t rows() const {return(rows_);}
   vector_length_t columns() const {return(columns_);}
   uint32_t stride() const {return(columns_);}


   explicit Matrix(vector_length_t r,vector_length_t c):
   Vector<P,DYNAMIC,Allocator>(r*c),rows_(r),columns_(c){};
   explicit Matrix(vector_length_t r,vector_length_t c,P init_val):
   Vector<P,DYNAMIC,Allocator>(r*c,init_val),rows_(r),columns_(c){};

   Matrix(const Matrix& other) = default;
   Matrix(Matrix&& other) = default;

   P& operator()(const index_t r,const index_t c)
   {
     return(Vector_Base<P>::ptr()[r*columns()+c]);
   }

   P& operator()(const index_t r,const index_t c) const
   {
     return(Vector_Base<P>::ptr()[r*columns()+c]);
   }

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

   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix(const _Expr<Derived>& other):Vector<P,DYNAMIC,Allocator>(other),
   rows_(other.rows()),columns_(other.columns())
   {
   };

   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix(const _Expr<Derived>& other):
   Vector<P,DYNAMIC,Allocator>(other.rows()*other.columns()),
   rows_(other.rows()),columns_(other.columns())
   {
        eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
   };

   template<typename Derived,
            typename std::enable_if<IsVector<Derived>::value,bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
      eval(*this,other.derived(),rows()*columns(),CURRENT_ARCH);
      return(*this);
   };


   template<typename Derived,
            typename std::enable_if<must_use_matrix_idx<Derived>(),bool>::type = true>
   Matrix& operator=(const _Expr<Derived>& other)
   {
         eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
         return(*this);
   };

   Matrix& operator=(const Matrix& other) = default;
   
   Matrix& operator=(Matrix&& other) = default;

   friend std::ostream& operator<< (std::ostream& stream, const Matrix& other) {
        int c=0;
        for(index_t k=0;k<other.length();k++)
        {
           stream << other[k] << " , ";
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

    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            SameElementType<VA,P>::value,bool>::type = true>
    static Matrix<P,DYNAMIC,DYNAMIC,Allocator> diagonal(const VA& a)
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(a.length(),a.length());
       _diagonal(res,a,a.length());
       return(res);
    }

    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            SameElementType<VA,P>::value,bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,this->length());
    }

    static Matrix<P,DYNAMIC,DYNAMIC,Allocator> identity(const vector_length_t l)
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(l,l);
       _identity(res,l);
       return(res);
    }

    Matrix<P,DYNAMIC,DYNAMIC,Allocator> create() const
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(rows(),columns());
       return(res);
    }

    Matrix<P,DYNAMIC,DYNAMIC,Allocator> transpose() const
    {
       Matrix<P,DYNAMIC,DYNAMIC,Allocator> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

   VectorView<P,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+this->columns()));
   }

   VectorView<P,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+stop));
   }

   const VectorView<P,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+this->columns()));
   }

   const VectorView<P,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<P,1>(*this,i*this->stride()+start,i*this->stride()+stop));
   }

   template<int CS=1>
   VectorView<P,DYNAMIC> col(const index_t i,const index_t start=0)
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*this->rows(),this->stride()*CS));
   }

   template<int CS=1>
   VectorView<P,DYNAMIC> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*stop,this->stride()*CS));
   }

   template<int CS=1>
   const VectorView<P,DYNAMIC> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*this->rows(),this->stride()*CS));
   }

   template<int CS=1>
   const VectorView<P,DYNAMIC> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<P,DYNAMIC>(*this,i+this->stride()*start,i+this->stride()*stop,this->stride()*CS));
   }

#if defined(HAS_VECTOR)
    using VectorType = typename vector_traits<P>::vector;
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        Vector_Base<P>::vector_store(row*stride() + col,val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
       Vector_Base<P>::vector_store_tail(row*stride() + col,remaining,val);
    }

    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(Vector_Base<P>::vector_op_tail(row*stride() + col,remaining));
    }
#endif

    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(Vector_Base<P>::vector_op(row*stride() + col));
    }
#endif

   MatrixView<P,DYNAMIC> sub(const index_t rs,const index_t cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols,stride()));
   }

   const MatrixView<P,DYNAMIC> sub(const index_t rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols,stride()));
   }
   
   MatrixView<P,DYNAMIC> sub(const Slice &rs,const index_t cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols,stride()));
   }

   const MatrixView<P,DYNAMIC> sub(const Slice &rs,const index_t cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = columns() - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs),nb_rows,nb_cols,stride()));
   }

   MatrixView<P,DYNAMIC> sub(const index_t rs,const Slice &cs)
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

   const MatrixView<P,DYNAMIC> sub(const index_t rs,const Slice &cs) const
   {
        const vector_length_t nb_rows = rows() - rs;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

   MatrixView<P,DYNAMIC> sub(const Slice& rs,const Slice& cs)
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

   const MatrixView<P,DYNAMIC> sub(const Slice& rs,const Slice& cs) const
   {
        const vector_length_t nb_rows = rs.stop - rs.start;
        const vector_length_t nb_cols = cs.stop - cs.start;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs.start*stride()+cs.start),nb_rows,nb_cols,stride()));
   }

   MatrixView<P,DYNAMIC> sub(const index_t rs,
                             const index_t re,
                             const index_t cs,
                             const index_t ce)
   {
        const vector_length_t nb_rows = re - rs;
        const vector_length_t nb_cols = ce - cs;

        return(MatrixView<P,DYNAMIC>(Vector_Base<P>::ptr(rs*stride()+cs),nb_rows,nb_cols,stride()));
   }

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