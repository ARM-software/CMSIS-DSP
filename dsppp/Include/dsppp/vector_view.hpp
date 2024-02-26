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

namespace arm_cmsis_dsp {

/** \addtogroup VECTOR
 *  @{
 */

template<typename T,int stride=1>
struct VectorView
{

    /*

    Start and stop are the position in the raw Vector_base pointer.
    Stop is the first sample outside of the vector

    */
    VectorView() = delete;

    constexpr static vector_length_t compute_length(const index_t start,const index_t stop)
    {
        return(1+(stop-1 -start)/stride);
    }

    explicit VectorView(T *v,const vector_length_t start,const vector_length_t stop):
      v_(v+start),nb_samples_(compute_length(start,stop)){};

    explicit VectorView(const Vector_Base<T> &v):
      v_(v.ptr()),nb_samples_(compute_length(0,v.length())){};

    explicit VectorView(const Vector_Base<T> &v,const index_t start,const index_t stop):
      v_(v.ptr()+start),nb_samples_(compute_length(start,stop)){};

    vector_length_t length() const {return(nb_samples_);};


    T* ptr() const {return(v_);}
    T* ptr(const index_t i) const {return(&v_[i*stride]);}

    const T* const_ptr() const {return(v_);}
    const T* const_ptr(const index_t i) const {return(&v_[i*stride]);}

    T& operator[](const index_t i)
    {
        return(v_[i*stride]);
    }
   
    T& operator[](const index_t i) const
    {
        return(v_[i*stride]);
    }
   
#if defined(HAS_VECTOR)
    using Vector = typename vector_traits<T>::vector;
    void vector_store(const index_t i,const Vector val)
    {
        inner::vstore1<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride]),val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void vector_store_tail(const index_t i,const vector_length_t remaining,const Vector val)
    {
        inner::vstore1_z<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride]),val,remaining,inner::vctpq<T>::mk(remaining));
    }

    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(inner::vload1_z<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride]),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif

    Vector const vector_op(const index_t i) const
    {
        return(inner::vload1<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride])));
    }
#endif

    virtual ~VectorView() {};

    VectorView(const VectorView& other):
    v_(other.v_),nb_samples_(other.nb_samples_){};
  

    VectorView(VectorView&& other) :
     v_(std::move(other.v_)),nb_samples_(other.nb_samples_)
     {
        other.v_ = nullptr;
     };

VectorView& operator=(const VectorView& other) = delete;
VectorView& operator=(VectorView&& other)  = delete;



   template<typename Derived>
   VectorView& operator=(const _Expr<Derived>&other)
   {
      eval(*this,other.derived(),length(),CURRENT_ARCH);
      return(*this);
   }


   VectorView& operator=(const T val)
   {
        _Fill(*this,val,length(),CURRENT_ARCH);
       
        return(*this);
   }

   template<typename Derived>
   VectorView& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator +=(const VectorView& other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator +=(const T other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   VectorView& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   
   VectorView& operator -=(const VectorView& other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator -=(const T other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   VectorView& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator *=(const VectorView& other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator *=(const T other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

  friend std::ostream& operator<< (std::ostream& stream, const VectorView<T,stride>& other) {
        constexpr int nb = 10;
        int i=0;
        for(index_t k=0;k<other.length();k++)
        {
           stream << other[k] << " , ";
           i++;
           if(i==nb)
           {
             i=0;
             stream << "\r\n";
           };
        }
        stream << "\r\n";
        return(stream);
    }

   template<int S=1>
   VectorView<T,S*stride> sub(const index_t start=0,const index_t stop=-1)
   {
     if (stop < 0)
     {
       return(VectorView<T,S*stride>(v_,stride*start,stride*length()));
     }
     else 
     {
       return(VectorView<T,S*stride>(v_,stride*start,stride*stop));
     }
   }

   template<int S=1>
   const VectorView<T,S*stride> sub(const index_t start=0,const index_t stop=-1) const
   {
     if (stop < 0)
     {
        return(VectorView<T,S*stride>(v_,stride*start,stride*length()));
     }
     else 
     {
        return(VectorView<T,S*stride>(v_,stride*start,stride*stop));
     }
   }


protected:
    T* const v_;
    const vector_length_t nb_samples_;
};

template<typename T>
struct VectorView<T,DYNAMIC>
{

    /*

    Start and stop are the position in the raw Vector_base pointer.
    Stop is the first sample outside of the vector

    */
    VectorView() = delete;

    vector_length_t compute_length(const index_t start,const index_t stop,const index_t stride) const
    {
        return(1+(stop-1 -start)/stride);
    }

    explicit VectorView(T *v,const index_t start,const index_t stop,const index_t stride):
      v_(v+start),nb_samples_(compute_length(start,stop,stride)),stride_(stride){};

    explicit VectorView(const Vector_Base<T> &v,const index_t stride):
      v_(v.ptr()),nb_samples_(compute_length(0,v.length(),stride)),stride_(stride){};

    explicit VectorView(const Vector_Base<T> &v,const index_t start,const index_t stop,const index_t stride):
      v_(v.ptr()+start),nb_samples_(compute_length(start,stop,stride)),stride_(stride){};

    vector_length_t length() const {return(nb_samples_);};


    T* ptr() const {return(v_);}
    T* ptr(const index_t i) const {return(&v_[i*stride_]);}

    const T* const_ptr() const {return(v_);}
    const T* const_ptr(const index_t i) const {return(&v_[i*stride_]);}

    T& operator[](index_t i)
    {
        return(v_[i*stride_]);
    }
   
    T& operator[](index_t i) const
    {
        return(v_[i*stride_]);
    }
   
#if defined(HAS_VECTOR)
    using Vector = typename vector_traits<T>::vector;
    void vector_store(index_t i,Vector val)
    {
        inner::vstore1((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_,val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void vector_store_tail(index_t i,vector_length_t remaining,Vector val)
    {
        inner::vstore1_z((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_,val,remaining,inner::vctpq<T>::mk(remaining));
    }

    Vector const vector_op_tail(index_t i,vector_length_t remaining) const
    {
        return(inner::vload1_z((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_,remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif
    
    Vector const vector_op(index_t i) const
    {
        return(inner::vload1((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_));
    }
#endif

    virtual ~VectorView() {};

    VectorView(const VectorView& other):
    v_(other.v_),nb_samples_(other.nb_samples_),stride_(other.stride_){};
  

    VectorView(VectorView&& other) :
     v_(std::move(other.v_)),nb_samples_(other.nb_samples_),stride_(other.stride_)
     {
        other.v_ = nullptr;
     };

VectorView& operator=(const VectorView& other) = delete;
VectorView& operator=(VectorView&& other)  = delete;



   template<typename Derived>
   VectorView& operator=(const _Expr<Derived>&other)
   {
      eval(*this,other.derived(),length(),CURRENT_ARCH);
      return(*this);
   }


   VectorView& operator=(const T val)
   {
        _Fill(*this,val,length(),CURRENT_ARCH);
       
        return(*this);
   }

   template<typename Derived>
   VectorView& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator +=(const VectorView& other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator +=(const T other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   VectorView& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   
   VectorView& operator -=(const VectorView& other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator -=(const T other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   VectorView& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator *=(const VectorView& other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

   VectorView& operator *=(const T other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

  friend std::ostream& operator<< (std::ostream& stream, const VectorView<T,DYNAMIC>& other) {
        constexpr int nb = 10;
        int i=0;
        for(index_t k=0;k<other.length();k++)
        {
           stream << other[k] << " , ";
           i++;
           if(i==nb)
           {
             i=0;
             stream << "\r\n";
           };
        }
        stream << "\r\n";
        return(stream);
    }

    index_t stride() const {return stride_;}

  
   template<int S=1>
   VectorView<T,DYNAMIC> sub(const index_t start=0,const index_t stop=-1)
   {
     if  (stop<0)
     {
        return(VectorView<T,DYNAMIC>(v_,stride()*start,stride()*length(),stride()*S));
     }
     else
     {
       return(VectorView<T,DYNAMIC>(v_,stride()*start,stride()*stop,stride()*S));
     }
   }

   template<int S=1>
   const VectorView<T,DYNAMIC> sub(const index_t start=0,const index_t stop=-1) const
   {
     if  (stop<0)
     {
        return(VectorView<T,DYNAMIC>(v_,stride()*start,stride()*length(),stride()*S));
     }
     else
     {
       return(VectorView<T,DYNAMIC>(v_,stride()*start,stride()*stop,stride()*S));
     }
   }

protected:
    T* const v_;
    const vector_length_t nb_samples_;
    const index_t stride_;
};

/*! @} */

}

