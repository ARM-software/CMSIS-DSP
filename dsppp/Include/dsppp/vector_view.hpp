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

/** @brief Vector view
 *  @tparam T Type of the scalar
 *  @tparam S Stride known at build time (default 1)
 */
template<typename T,int stride=1>
struct VectorView
{

    /*

    Start and stop are the position in the raw Vector_base pointer.
    Stop is the first sample outside of the vector

    */
    VectorView() = delete;

    /**
    * @brief  Compute the number of elements in the vector view
    * @param start Vector view start index
    * @param stop Vector view stop index (first elemnt after the view)
    * @return Vector dimension
    *
    */
    constexpr static vector_length_t compute_length(const index_t start,const index_t stop)
    {
        return(1+(stop-1 -start)/stride);
    }

    /**
    * @brief  Create a vector view on a buffer
    * @param v Buffer of samples (not owned by the view)
    * @param start Start index of the view
    * @param stop Stop index of the view (first elemnt after the view)
    *
    */
    explicit VectorView(T *v,const vector_length_t start,const vector_length_t stop):
      v_(v+start),nb_samples_(compute_length(start,stop)){};

   /**
    * @brief  Create a vector on a vector
    * @param v Vector storage (not owned by the view)
    *
    */
    explicit VectorView(const Vector_Base<T> &v):
      v_(v.ptr()),nb_samples_(compute_length(0,v.length())){};

    /**
    * @brief  Create a vector view on vector
    * @param v Vector storage (not owned by the view)
    * @param start Start index of the view
    * @param stop Stop index of the view (first elemnt after the view)
    *
    */
    explicit VectorView(const Vector_Base<T> &v,const index_t start,const index_t stop):
      v_(v.ptr()+start),nb_samples_(compute_length(start,stop)){};

    /**
    * @brief  Vector view dimension
    * @return Number of elements 
    *
    */
    vector_length_t length() const {return(nb_samples_);};


    /**
    * @brief  Pointer to view storage
    * @return Pointer to start of storage
    *
    */
    T* ptr() const {return(v_);}

    /**
    * @brief  Pointer to view storage at index i
    * @param i Index
    * @return Pointer to storage at index i
    *
    * The stride is used to compute this pointer.
    * The index is scaled by the stride.
    */
    T* ptr(const index_t i) const {return(&v_[i*stride]);}

    /**
    * @brief  Pointer to view constant storage
    * @return Pointer to start of constant storage
    *
    */
    const T* const_ptr() const {return(v_);}

    /**
    * @brief  Pointer to view constant storage at index i
    * @param i Index
    * @return Pointer to constant storage at index i
    *
    * The stride is used to compute this pointer.
    * The index is scaled by the stride.
    */
    const T* const_ptr(const index_t i) const {return(&v_[i*stride]);}

    /**
    * @brief  Element at index i
    * @param i Index
    * @return Reference to element
    *
    * The stride is used to compute this reference.
    * The index is scaled by the stride.
    */
    T& operator[](const index_t i)
    {
        return(v_[i*stride]);
    }
   
    /**
    * @brief  Element at index i
    * @param i Index
    * @return Reference to element
    *
    * The stride is used to compute this reference.
    * The index is scaled by the stride.
    */
    T& operator[](const index_t i) const
    {
        return(v_[i*stride]);
    }
   
#if defined(HAS_VECTOR)
     //! Type of vectors for a vector architecture and for scalar datatype P
    using Vector = typename vector_traits<T>::vector;

    /**
    * @brief   %Vector store at index i
    *
    * @param i index
    * @param val Vector value
    * 
    * On an architecture supporting vectors, if the scalar datatype T
    * has a corresponding vector datatype, this function stores a vector
    * value at index i in this vector datatype
    */
    void vector_store(const index_t i,const Vector val)
    {
        inner::vstore1<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride]),val);
    }

#if defined(HAS_PREDICATED_LOOP)
    /**
    * @brief   %Vector store at index i with predicated tail
    *
    * @param i index
    * @param remaining Number of remaining samples in the loop
    * @param val Vector value to write at index i with tail predication
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function stores a vector value at index i in this vector datatype
    * with predication
    */
    void vector_store_tail(const index_t i,const vector_length_t remaining,const Vector val)
    {
        inner::vstore1_z<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride]),val,remaining,inner::vctpq<T>::mk(remaining));
    }

    /**
    * @brief   %Vector operation at index i with predicated tail
    *
    * @param i index
    * @param remaining Number of remaining samples in the loop
    * @return Result of operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function execute an operation at index i with predication.
    * In the case of a vector, this operation is a load
    */
    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(inner::vload1_z<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride]),remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif

    /**
    * @brief   %Vector operation at index i
    *
    * @param i index
    * @return Result of operation
    * 
    * On an architecture supporting vectors, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function execute an operation at index i.
    * In the case of a vector, this operation is a load
    */
    Vector const vector_op(const index_t i) const
    {
        return(inner::vload1<stride>((typename std::remove_cv<T>::type*)(&v_[i*stride])));
    }
#endif

    virtual ~VectorView() {};

    /**
    * @brief  Create a vector view from another view
    * @param other the other vector view
    *
    * The new vector view will point to the same storage as the
    * other vector view. No copy of element is occuring.
    * To copy vector view content, the expr and copy operators
    * are needed.
    */
    VectorView(const VectorView& other):
    v_(other.v_),nb_samples_(other.nb_samples_){};
  

    /**
    * @brief  Move a vector view to another view
    * @param other the other vector view
    *
    * The new vector view will point to the same storage as the
    * other vector view. No copy of element is occuring.
    *
    * The other vector view is no more valid (points to null storage)
    */
    VectorView(VectorView&& other) :
     v_(std::move(other.v_)),nb_samples_(other.nb_samples_)
     {
        other.v_ = nullptr;
     };

VectorView& operator=(const VectorView& other) = delete;
VectorView& operator=(VectorView&& other)  = delete;


   /**
    * @brief  Assign an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    * Evaluate an expression an assign its result to the vector view
    */
   template<typename Derived>
   VectorView& operator=(const _Expr<Derived>&other)
   {
      eval(*this,other.derived(),length(),CURRENT_ARCH);
      return(*this);
   }


   /**
    * @brief  Assign a scalar to a vector view
    * @param val the scalar
    * @return the vector view
    * 
    */
   VectorView& operator=(const T val)
   {
        _Fill(*this,val,length(),CURRENT_ARCH);
       
        return(*this);
   }

   /**
    * @brief  Elementwise  add an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    */
   template<typename Derived>
   VectorView& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise add a vector view to a vector view
    * @param other the vector view to add
    * @return the vector view
    * 
    */
   VectorView& operator +=(const VectorView& other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise  add a scalar to a vector view
    * @param other the scalar
    * @return the vector view
    * 
    */
   VectorView& operator +=(const T other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise  subtract an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    */
   template<typename Derived>
   VectorView& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise subtract a vector view to a vector view
    * @param other the vector view to add
    * @return the vector view
    * 
    */
   VectorView& operator -=(const VectorView& other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise  subtract a scalar to a vector view
    * @param other the scalar
    * @return the vector view
    * 
    */
   VectorView& operator -=(const T other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise multiply an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    */
   template<typename Derived>
   VectorView& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise multiply a vector view to a vector view
    * @param other the vector view to add
    * @return the vector view
    * 
    */
   VectorView& operator *=(const VectorView& other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise  multiply a scalar to a vector view
    * @param other the scalar
    * @return the vector view
    * 
    */
   VectorView& operator *=(const T other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

  /**
    * @brief  Display the vector view content for debug purpose
    * @param stream Output stream
    * @param other The vector view to display
    * @return the stream
    * 
    */
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

   /**
    * @brief  Create a sub vector (a view of a view)
    * @tparam S stride known at build time
    * @param start Start index
    * @param stop Stop index (first element after the view)
    *             By default it is the vector view length
    * @return the vector view
    * 
    */
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

   /**
    * @brief  Create a constant sub vector (a view of a view)
    * @tparam S stride known at build time
    * @param start Start index
    * @param stop Stop index (first element after the view)
    *             By default it is the vector view length
    * @return the vector view
    * 
    */
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

/** @brief Vector view with dynamic stride (not known at build time)
 *  @tparam T Type of the scalar
 */
template<typename T>
struct VectorView<T,DYNAMIC>
{

    /*

    Start and stop are the position in the raw Vector_base pointer.
    Stop is the first sample outside of the vector

    */
    VectorView() = delete;

    /**
    * @brief  Compute the number of elements in the vector view
    * @param start Vector view start index
    * @param stop Vector view stop index (first elemnt after the view)
    * @param stride Stride (only known at runtime)
    * @return Vector dimension
    *
    */
    vector_length_t compute_length(const index_t start,const index_t stop,const index_t stride) const
    {
        return(1+(stop-1 -start)/stride);
    }

    /**
    * @brief  Create a vector view on a buffer
    * @param v Buffer of samples (not owned by the view)
    * @param start Start index of the view
    * @param stop Stop index of the view (first elemnt after the view)
    * @param stride Stride (only known at runtime)
    *
    */
    explicit VectorView(T *v,const index_t start,const index_t stop,const index_t stride):
      v_(v+start),nb_samples_(compute_length(start,stop,stride)),stride_(stride){};

    
    /**
    * @brief  Create a vector view on a vector
    * @param v Vector owning the storage (not owned by the view)
    * @param stride Stride (only known at runtime)
    * 
    * start is 0
    * stop is defined by the length of the vector
    *
    */
    explicit VectorView(const Vector_Base<T> &v,const index_t stride):
      v_(v.ptr()),nb_samples_(compute_length(0,v.length(),stride)),stride_(stride){};

    /**
    * @brief  Create a vector view on a vector
    * @param v Vector owning the storage (not owned by the view)
    * @param start Start index of the view
    * @param stop Stop index
    * @param stride Stride (only known at runtime)
    * 
    *
    */
    explicit VectorView(const Vector_Base<T> &v,const index_t start,const index_t stop,const index_t stride):
      v_(v.ptr()+start),nb_samples_(compute_length(start,stop,stride)),stride_(stride){};

    /**
    * @brief  Vector view dimension
    * @return Number of elements 
    *
    */
    vector_length_t length() const {return(nb_samples_);};


    /**
    * @brief  Pointer to view storage
    * @return Pointer to start of storage
    *
    */
    T* ptr() const {return(v_);}

    /**
    * @brief  Pointer to view storage at index i
    * @param i Index
    * @return Pointer to storage at index i
    *
    * The stride is used to compute this pointer.
    * The index is scaled by the stride.
    */
    T* ptr(const index_t i) const {return(&v_[i*stride_]);}


    /**
    * @brief  Pointer to view constant storage
    * @return Pointer to start of constant storage
    *
    */
    const T* const_ptr() const {return(v_);}

    /**
    * @brief  Pointer to view constant storage at index i
    * @param i Index
    * @return Pointer to constant storage at index i
    *
    * The stride is used to compute this pointer.
    * The index is scaled by the stride.
    */
    const T* const_ptr(const index_t i) const {return(&v_[i*stride_]);}

    /**
    * @brief  Element at index i
    * @param i Index
    * @return Reference to element
    *
    * The stride is used to compute this reference.
    * The index is scaled by the stride.
    */
    T& operator[](index_t i)
    {
        return(v_[i*stride_]);
    }
   
    /**
    * @brief  Element at index i
    * @param i Index
    * @return Reference to element
    *
    * The stride is used to compute this reference.
    * The index is scaled by the stride.
    */
    T& operator[](index_t i) const
    {
        return(v_[i*stride_]);
    }
   
#if defined(HAS_VECTOR)
    //! Type of vectors for a vector architecture and for scalar datatype P
    using Vector = typename vector_traits<T>::vector;

    /**
    * @brief   %Vector store at index i
    *
    * @param i index
    * @param val Vector value
    * 
    * On an architecture supporting vectors, if the scalar datatype T
    * has a corresponding vector datatype, this function stores a vector
    * value at index i in this vector datatype
    */
    void vector_store(index_t i,Vector val)
    {
        inner::vstore1((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_,val);
    }

#if defined(HAS_PREDICATED_LOOP)
    /**
    * @brief   %Vector store at index i with predicated tail
    *
    * @param i index
    * @param remaining Number of remaining samples in the loop
    * @param val Vector value to write at index i with tail predication
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function stores a vector value at index i in this vector datatype
    * with predication
    */
    void vector_store_tail(index_t i,vector_length_t remaining,Vector val)
    {
        inner::vstore1_z((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_,val,remaining,inner::vctpq<T>::mk(remaining));
    }

     /**
    * @brief   %Vector operation at index i with predicated tail
    *
    * @param i index
    * @param remaining Number of remaining samples in the loop
    * @return Result of operation
    * 
    * On an architecture supporting vectors and predicated loops, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function execute an operation at index i with predication.
    * In the case of a vector, this operation is a load
    */
    Vector const vector_op_tail(index_t i,vector_length_t remaining) const
    {
        return(inner::vload1_z((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_,remaining,inner::vctpq<T>::mk(remaining)));
    }
#endif
    
    /**
    * @brief   %Vector operation at index i
    *
    * @param i index
    * @return Result of operation
    * 
    * On an architecture supporting vectors, if the 
    * scalar datatype T has a corresponding vector datatype, this 
    * function execute an operation at index i.
    * In the case of a vector, this operation is a load
    */
    Vector const vector_op(index_t i) const
    {
        return(inner::vload1((typename std::remove_cv<T>::type*)(&v_[i*stride_]),stride_));
    }
#endif

    virtual ~VectorView() {};

    /**
    * @brief  Create a vector view from another view
    * @param other the other vector view
    *
    * The new vector view will point to the same storage as the
    * other vector view. No copy of element is occuring.
    * To copy vector view content, the expr and copy operators
    * are needed.
    */
    VectorView(const VectorView& other):
    v_(other.v_),nb_samples_(other.nb_samples_),stride_(other.stride_){};
  

    /**
    * @brief  Move a vector view to another view
    * @param other the other vector view
    *
    * The new vector view will point to the same storage as the
    * other vector view. No copy of element is occuring.
    *
    * The other vector view is no more valid (points to null storage)
    */
    VectorView(VectorView&& other) :
     v_(std::move(other.v_)),nb_samples_(other.nb_samples_),stride_(other.stride_)
     {
        other.v_ = nullptr;
     };

VectorView& operator=(const VectorView& other) = delete;
VectorView& operator=(VectorView&& other)  = delete;



   /**
    * @brief  Assign an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    * Evaluate an expression an assign its result to the vector view
    */
   template<typename Derived>
   VectorView& operator=(const _Expr<Derived>&other)
   {
      eval(*this,other.derived(),length(),CURRENT_ARCH);
      return(*this);
   }


    /**
    * @brief  Assign a scalar to a vector view
    * @param val the scalar
    * @return the vector view
    * 
    */
   VectorView& operator=(const T val)
   {
        _Fill(*this,val,length(),CURRENT_ARCH);
       
        return(*this);
   }

   /**
    * @brief  Elementwise  add an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    */
   template<typename Derived>
   VectorView& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise add a vector view to a vector view
    * @param other the vector view to add
    * @return the vector view
    * 
    */
   VectorView& operator +=(const VectorView& other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise  add a scalar to a vector view
    * @param other the scalar
    * @return the vector view
    * 
    */
   VectorView& operator +=(const T other)
   {
      eval(*this,*this + other,length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise  subtract an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    */
   template<typename Derived>
   VectorView& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise subtract a vector view to a vector view
    * @param other the vector view to add
    * @return the vector view
    * 
    */
   VectorView& operator -=(const VectorView& other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise  subtract a scalar to a vector view
    * @param other the scalar
    * @return the vector view
    * 
    */
   VectorView& operator -=(const T other)
   {
      eval(*this,*this - other,length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise multiply an expression to a vector view
    * @tparam Derived the datatype representing the abstract syntax tree of the view
    * @param other the expression
    * @return the vector view
    * 
    */
   template<typename Derived>
   VectorView& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),length(),CURRENT_ARCH);
      return(*this);
   };

    /**
    * @brief  Elementwise multiply a vector view to a vector view
    * @param other the vector view to add
    * @return the vector view
    * 
    */
   VectorView& operator *=(const VectorView& other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise  multiply a scalar to a vector view
    * @param other the scalar
    * @return the vector view
    * 
    */
   VectorView& operator *=(const T other)
   {
      eval(*this,*this * other,length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Display the vector view content for debug purpose
    * @param stream Output stream
    * @param other The vector view to display
    * @return the stream
    * 
    */
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

    /**
    * @brief  Stride of the vector view
    * @return the stride
    * 
    */
    index_t stride() const {return stride_;}

  
   /**
    * @brief  Create a sub vector (a view of a view)
    * @tparam S stride known at build time
    * @param start Start index
    * @param stop Stop index (first element after the view)
    *             By default it is the vector view length
    * @return the vector view
    * 
    */
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

   /**
    * @brief  Create a constant sub vector (a view of a view)
    * @tparam S stride known at build time
    * @param start Start index
    * @param stop Stop index (first element after the view)
    *             By default it is the vector view length
    * @return the vector view
    * 
    */
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

