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

namespace arm_cmsis_dsp {

/** \addtogroup Matrix
 *  @{
 */

template<typename T,int S=1>
struct MatrixView
{
   vector_length_t rows() const {return(nb_rows_);}
   vector_length_t columns() const {return(nb_cols_);}
   constexpr uint32_t stride() const {return(S);}

   explicit MatrixView(T* v,
              const vector_length_t rows,
              const vector_length_t cols):
   v_(v),nb_rows_(rows),nb_cols_(cols){};

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

   T& operator()(const index_t r,const index_t c)
   {
     return(v_[r*stride()+c]);
   }

   T const operator()(const index_t r,const index_t c) const
   {
     return(v_[r*stride()+c]);
   }


   template<typename Derived>
   MatrixView& operator=(const _Expr<Derived>&other)
   {
      eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   }

   MatrixView& operator=(const T val)
   {
        _Fill2D(*this,val,rows(),columns(),CURRENT_ARCH);
       
        return(*this);
   }


   template<typename Derived>
   MatrixView& operator +=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this + other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator +=(const MatrixView& other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator +=(const T other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   MatrixView& operator -=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this - other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   
   MatrixView& operator -=(const MatrixView& other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator -=(const T other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   MatrixView& operator *=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this * other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator *=(const MatrixView& other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator *=(const T other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

  friend std::ostream& operator<< (std::ostream& stream, const MatrixView& other) {
        for(index_t row=0;row<other.rows();row++)
        {
           for(index_t col=0;col<other.columns();col++)
           {
                stream << other(row,col)<< " , ";
           }
           stream << "\r\n";
        }
        stream << "\r\n";
        return(stream);
    }

   VectorView<T,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

   VectorView<T,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

   const VectorView<T,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

   const VectorView<T,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

   template<int CS=1>
   VectorView<T,CS*S> col(const index_t i,const index_t start=0)
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*rows()));
   }

   template<int CS=1>
   VectorView<T,CS*S> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*stop));
   }

   template<int CS=1>
   const VectorView<T,CS*S> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*rows()));
   }

   template<int CS=1>
   const VectorView<T,CS*S> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,CS*S>(v_,i+stride()*start,i+stride()*stop));
   }

   #if defined(HAS_VECTOR)
    using VectorType = typename vector_traits<T>::vector;
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        inner::vstore1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val,remaining,inner::vctpq<T>::mk(remaining));
    }

    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif

    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(inner::vload1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col])));
    }
#endif

    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            SameElementType<VA,T>::value,bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,this->length());
    }

    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> transpose() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> create() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(rows(),columns());
       return(res);
    }

    T* ptr() const {return(v_);}
    const T* const_ptr() const {return(v_);}

protected:
    T* const v_;
    const vector_length_t nb_rows_;
    const vector_length_t nb_cols_;
};

/*

When the stride if not known at build time AND different
from the nb_cols_

*/
template<typename T>
struct MatrixView<T,DYNAMIC>
{
   vector_length_t rows() const {return(nb_rows_);}
   vector_length_t columns() const {return(nb_cols_);}
   uint32_t stride() const {return(stride_);}

   explicit MatrixView(T* v,
              const vector_length_t rows,
              const vector_length_t cols,
              const uint32_t stride):
   v_(v),nb_rows_(rows),nb_cols_(cols),stride_(stride){};

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

   T& operator()(const index_t r,const index_t c)
   {
     return(v_[r*stride()+c]);
   }

   T const operator()(const index_t r,const index_t c) const
   {
     return(v_[r*stride()+c]);
   }


   template<typename Derived>
   MatrixView& operator=(const _Expr<Derived>&other)
   {
      eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   }

   MatrixView& operator=(const T val)
   {
        _Fill2D(*this,val,rows(),columns(),CURRENT_ARCH);
       
        return(*this);
   }


   template<typename Derived>
   MatrixView& operator +=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this + other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator +=(const MatrixView& other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator +=(const T other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   MatrixView& operator -=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this - other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   
   MatrixView& operator -=(const MatrixView& other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator -=(const T other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   MatrixView& operator *=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this * other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator *=(const MatrixView& other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator *=(const T other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

  friend std::ostream& operator<< (std::ostream& stream, const MatrixView& other) {
        for(index_t row=0;row<other.rows();row++)
        {
           for(index_t col=0;col<other.columns();col++)
           {
                stream << other(row,col)<< " , ";
           }
           stream << "\r\n";
        }
        stream << "\r\n";
        return(stream);
    }

   VectorView<T,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

   VectorView<T,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

   const VectorView<T,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+columns()));
   }

   const VectorView<T,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,1>(v_,i*stride()+start,i*stride()+stop));
   }

   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0)
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*rows(),stride()*CS));
   }

   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*stop,stride()*CS));
   }

   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*rows(),stride()*CS));
   }

   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,DYNAMIC>(v_,i+stride()*start,i+stride()*stop,stride()*CS));
   }

   #if defined(HAS_VECTOR)
    using VectorType = typename vector_traits<T>::vector;
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        inner::vstore1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),val,remaining,inner::vctpq<T>::mk(remaining));
    }

    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col]),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif
    
    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(inner::vload1<1>((typename std::remove_cv<T>::type*)(&v_[row*stride() + col])));
    }
#endif

    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            SameElementType<VA,T>::value,bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,this->length());
    }

    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> transpose() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> create() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(rows(),columns());
       return(res);
    }

    T* ptr() const {return(v_);}

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

template<typename T>
struct MatrixView<T,CONSTRAINED_DYNAMIC>:VectorView<T,1>
{
   vector_length_t rows() const {return(nb_rows_);}
   vector_length_t columns() const {return(nb_cols_);}
   uint32_t stride() const {return(nb_cols_);}

   explicit MatrixView(T* v,
              const vector_length_t rows,
              const vector_length_t cols):
   VectorView<T,1>(v,0,rows*cols),
   nb_rows_(rows),nb_cols_(cols){};

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

   T& operator()(const index_t r,const index_t c)
   {
     return(&(*this)[r*stride()+c]);
   }

   T const operator()(const index_t r,const index_t c) const
   {
     return((*this)[r*stride()+c]);
   }


   template<typename Derived>
   MatrixView& operator=(const _Expr<Derived>&other)
   {
      eval2D(*this,other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   }

   MatrixView& operator=(const T val)
   {
        _Fill2D(*this,val,rows(),columns(),CURRENT_ARCH);
       
        return(*this);
   }


   template<typename Derived>
   MatrixView& operator +=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this + other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator +=(const MatrixView& other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator +=(const T other)
   {
      eval2D(*this,*this + other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   MatrixView& operator -=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this - other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   
   MatrixView& operator -=(const MatrixView& other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator -=(const T other)
   {
      eval2D(*this,*this - other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   MatrixView& operator *=(const _Expr<Derived>& other)
   {
      eval2D(*this,*this * other.derived(),rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator *=(const MatrixView& other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

   MatrixView& operator *=(const T other)
   {
      eval2D(*this,*this * other,rows(),columns(),CURRENT_ARCH);
      return(*this);
   };

  friend std::ostream& operator<< (std::ostream& stream, const MatrixView& other) {
        for(index_t row=0;row<other.rows();row++)
        {
           for(index_t col=0;col<other.columns();col++)
           {
                stream << other(row,col)<< " , ";
           }
           stream << "\r\n";
        }
        stream << "\r\n";
        return(stream);
    }

   VectorView<T,1> row(const index_t i,const index_t start=0)
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+columns()));
   }

   VectorView<T,1> row(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+stop));
   }

   const VectorView<T,1> row(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+columns()));
   }

   const VectorView<T,1> row(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,1>(this->ptr(),i*stride()+start,i*stride()+stop));
   }

   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0)
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*rows(),stride()*CS));
   }

   template<int CS=1>
   VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop)
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*stop,stride()*CS));
   }

   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start=0) const
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*rows(),stride()*CS));
   }

   template<int CS=1>
   const VectorView<T,DYNAMIC> col(const index_t i,const index_t start,const index_t stop) const
   {
     return(VectorView<T,DYNAMIC>(this->ptr(),i+stride()*start,i+stride()*stop,stride()*CS));
   }

   #if defined(HAS_VECTOR)
    using VectorType = typename vector_traits<T>::vector;
    void matrix_store(const index_t row,
                      const index_t col,
                      const VectorType val) const
    {
        inner::vstore1<1>((typename std::remove_cv<T>::type*)(ptr(row*stride() + col)),val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void matrix_store_tail(const index_t row,
                           const index_t col,
                           const vector_length_t remaining,
                           const VectorType val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<T>::type*)(ptr(row*stride() + col)),val,remaining,inner::vctpq<T>::mk(remaining));
    }

    VectorType const matrix_op_tail(const index_t row,
                                const index_t col,
                                const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<T>::type*)(VectorView<T,1>::ptr(row*stride() + col)),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif
    
    VectorType const matrix_op(const index_t row,
                           const index_t col) const
    {
        return(inner::vload1<1>((typename std::remove_cv<T>::type*)(VectorView<T,1>::ptr(row*stride() + col))));
    }
#endif

    template<typename VA,
            typename std::enable_if<IsVector<VA>::value && 
            SameElementType<VA,T>::value,bool>::type = true>
    void fill_diagonal(const VA& a)
    {
       _fill_diagonal(*this,a,this->length());
    }

    Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> transpose() const
    {
       Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(columns(),rows());
       transposeTo(res,*this);
       return(res);
    }

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