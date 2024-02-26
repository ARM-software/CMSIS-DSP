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

namespace arm_cmsis_dsp {

/** \addtogroup VECTOR
 *  @{
 */

/*

Generic evaluators.

*/
#include "Scalar/basic.hpp"
#include "DSP/basic.hpp"
#include "Helium/basic.hpp"
#include "Neon/basic.hpp"


template<typename P>
struct Vector_Base {

    typedef P element_type;

    vector_length_t length() const   {return(length_);};

    P* ptr() const {return(values_);}
    P* ptr(const index_t i) const {return(&values_[i]);}

    const P* const_ptr() const {return(values_);}
    const P* const_ptr(const index_t i) const {return(&values_[i]);}


    P* begin() const {return(values_);}
    P* end() const {return(values_+length_);}


    friend std::ostream& operator<< (std::ostream& stream, const Vector_Base<P>& other) {
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

    virtual ~Vector_Base(){};

    Vector_Base(Vector_Base&& other) :
     length_(other.length_),values_(other.values_){
        other.values_=nullptr;
        other.length_ = 0;
     };

    // Done in derivated classes since they have
   // knowledge of the allocator to use
   // But used in VectorView
   Vector_Base(const Vector_Base& other) = delete;
   //   length_(other.length_),values_(other.values_){};

   P& operator[](const index_t i)
   {
       return(values_[i]);
   }

   P& operator[](const index_t i) const
   {
       return(values_[i]);
   }

#if defined(HAS_VECTOR)
    using Vector = typename vector_traits<P>::vector;

    template<typename T=P,
             typename std::enable_if<vector_traits<T>::has_vector,bool>::type = true>
    void vector_store(const index_t i,const Vector val) const
    {
        inner::vstore1<1>((typename std::remove_cv<P>::type*)(&values_[i]),val);
    }

#if defined(HAS_PREDICATED_LOOP)
    void vector_store_tail(const index_t i,const vector_length_t remaining,const Vector val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<P>::type*)(&values_[i]),val,remaining,inner::vctpq<P>::mk(remaining));
    }

    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<P>::type*)(&values_[i]),remaining,inner::vctpq<P>::mk(remaining)));
    }
#endif
    
    Vector const vector_op(const index_t i) const
    {
        return(inner::vload1<1>((typename std::remove_cv<P>::type*)(&values_[i])));
    }

#endif



protected:

   //Vector_Base():length_(0),values_(nullptr){};
    Vector_Base() = delete;

   explicit Vector_Base(vector_length_t length, char *val):
   length_(length),
   values_(reinterpret_cast<P*>(val)){};

   explicit Vector_Base(vector_length_t length, char *val,P init_val):
   length_(length),
   values_(reinterpret_cast<P*>(val)){
    _Fill(*this,init_val,length,CURRENT_ARCH);
   };

   
   Vector_Base& operator=(const Vector_Base& other) 
   {
        if ((length_ == other.length_) && (this != &other)) 
        {
            _Fill(*this,other,other.length_,CURRENT_ARCH);
             //std::memcpy(values_,other.values_,sizeof(P)*length_);
        }  
      return(*this);
   }

   // Done in derivated classes since we need
   // the allocator destroy
   Vector_Base& operator=(Vector_Base&& other) = delete; 
   



   vector_length_t length_;
   P* values_;
};


template<typename T>
struct traits<Vector_Base<T>>
{
    typedef T Scalar;
#if defined(HAS_VECTOR)
    typedef typename vector_traits<T>::vector Vector;
#endif
};

/** @brief Vector template
 *  @tparam P Type of the scalar
 *  @tparam L Vector length in number of elements. 
 *            Negative if length not known at build time. It is the default value
 *  @tparam Allocator Memory allocator to use. By default it is the macro `TMP_ALLOC`
 */
template<typename P,
         int L=DYNAMIC,
         template<int> typename Allocator = TMP_ALLOC>
struct Vector:Vector_Base<P> {

   
   //! Type of vector elements
   using element_type = P;

   //! Length of the vector when known at build time.
   constexpr static vector_length_t vector_size = sizeof(P)*L;

   /**
    * @brief      Allocate a buffer for this vector using the memory allocator
    *
    * @return     A new memory buffer
    */
   static char* allocate(){return(Allocator<vector_size>::allocate());};

   /**
    * @brief      Construct a new vector
    * 
    *             The length is known at build time.
    *             
    */
   Vector():Vector_Base<P>(L,Vector::allocate()){};

    /**
    * @brief      Construct a new vector and initialize it
    * 
    *             The length is known at build time.
    *      
    * @param init_val Initialization value         
    */
   explicit Vector(P init_val):Vector_Base<P>(L,Vector::allocate(),init_val){
   };

   /**
    * @brief      Construct a new vector and initialize it with a list
    * 
    *             The length is known at build time.
    *      
    * @param l Initialization list         
    */
   Vector(const std::initializer_list<P> &l)
   :Vector_Base<P>(L,Vector::allocate()){
     std::memcpy(Vector_Base<P>::values_,l.data(),vector_size);
   };

  Vector(Vector&& other) = default;

  Vector(const Vector& other):Vector_Base<P>(L,Vector::allocate())
  {
        eval(*this,+other,(vector_length_t)L,CURRENT_ARCH);

        //std::memcpy(Vector_Base<P>::values_,other.values_,vector_size);
  };

   template<template<int> typename OtherAllocator>
   explicit Vector(const Vector<P,L,OtherAllocator>& other):Vector_Base<P>(L,Vector::allocate())
   {
        eval(*this,+other,(vector_length_t)L,CURRENT_ARCH);
   };

   template<template<int> typename OtherAllocator>
   explicit Vector(const Vector<P,DYNAMIC,OtherAllocator>& other):Vector_Base<P>(L,Vector::allocate())
   {
        if (other.length() == Vector_Base<P>::length())
        {
           eval(*this,+other,(vector_length_t)L,CURRENT_ARCH);
        }
   };

   template<int S>
   explicit Vector(const VectorView<P,S>& other):Vector_Base<P>(L,Vector::allocate())
   {
        eval(*this,+other,(vector_length_t)L,CURRENT_ARCH);
   };
   

  

   template<typename Derived>
   Vector(const _Expr<Derived>& other):Vector_Base<P>(L,Vector::allocate())
   {
        eval(*this,other.derived(),(vector_length_t)L,CURRENT_ARCH);
   };


  Vector& operator=(const Vector& other) = default;

  Vector& operator=(Vector&& other)
   {
      if (this != &other)
      {
        if (Vector_Base<P>::values_!=nullptr)
        {
           Allocator<vector_size>::destroy(reinterpret_cast<char*>(Vector_Base<P>::values_));
        }
  
        Vector_Base<P>::length_= other.length_;
        Vector_Base<P>::values_ = other.values_;
        other.values_=nullptr;
        other.length_ = 0;
      }

      return(*this);
   }

  template<typename Derived>
  Vector& operator=(const _Expr<Derived>&other)
  {
      eval(*this,other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
  }


  template<typename T,
           typename std::enable_if<is_scalar<T>(),bool>::type = true>
  Vector& operator=(const T other)
  {
      _Fill(*this,other,L,CURRENT_ARCH);
      return(*this);
  }


   

   
   template<typename Derived>
   Vector& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   Vector& operator +=(const Vector& other)
   {
      eval(*this,*this + other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   Vector& operator +=(const P other)
   {
      eval(*this,*this + other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   Vector& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   Vector& operator -=(const Vector& other)
   {
      eval(*this,*this - other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   Vector& operator -=(const P other)
   {
      eval(*this,*this - other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   Vector& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   Vector& operator *=(const Vector& other)
   {
      eval(*this,*this * other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   Vector& operator *=(const P other)
   {
      eval(*this,*this * other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };



  
   template<int S=1>
   VectorView<P,S> sub(const index_t start=0,const index_t stop=L)
   {
     return(VectorView<P,S>(*this,start,stop));
   }

   template<int S=1>
   const VectorView<P,S> sub(const index_t start=0,const index_t stop=L) const
   {
     return(VectorView<P,S>(*this,start,stop));
   }


   virtual ~Vector() {
      if (Vector_Base<P>::values_!=nullptr)
      {
         Allocator<vector_size>::destroy(reinterpret_cast<char*>(Vector_Base<P>::values_));
      }
    }




};



template<typename P,
         template<int> typename Allocator>
struct Vector<P,DYNAMIC,Allocator>:Vector_Base<P> {

    static char* allocate(vector_length_t length){return(Allocator<DYNAMIC>::allocate(sizeof(P)*length));};

    Vector() = delete;

    explicit Vector(vector_length_t length,P init_val):
    Vector_Base<P>(length,Vector::allocate(length),init_val){};
      
    explicit Vector(vector_length_t length):
    Vector_Base<P>(length,Vector::allocate(length)){};

    explicit Vector(const std::initializer_list<P> &l)
    :Vector_Base<P>(l.size(),Vector::allocate(l.size())){
       std::memcpy(Vector_Base<P>::values_,l.data(),sizeof(P)*l.size());
    };
   
   template<int K,template<int> typename OtherAllocator>
   explicit Vector(const Vector<P,K,OtherAllocator>& other):Vector_Base<P>(other.length(),Vector::allocate(other.length()))
   {
        eval(*this,+other,Vector_Base<P>::length(),CURRENT_ARCH);
   };

   
   Vector(const Vector& other):Vector_Base<P>(other.length(),Vector::allocate(other.length()))
   {
        eval(*this,+other,Vector_Base<P>::length(),CURRENT_ARCH);

        //std::memcpy(Vector_Base<P>::values_,other.values_,vector_size);
   };

   template<int S>
   explicit Vector(const VectorView<P,S>& other):Vector_Base<P>(other.length(),Vector::allocate(other.length()))
   {
        eval(*this,+other,Vector_Base<P>::length(),CURRENT_ARCH);
   };

   template<typename Derived>
   Vector(const _Expr<Derived>& other):Vector_Base<P>(other.length(),Vector::allocate(other.length()))
   {
        eval(*this,other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
   };

   Vector(Vector&& other) = default;


  
   Vector& operator=(const Vector& other) = default;
   
   Vector& operator=(Vector&& other)
   {
      if (this != &other)
      {
        if (Vector_Base<P>::values_!=nullptr)
        {
           Allocator<DYNAMIC>::destroy(reinterpret_cast<char*>(Vector_Base<P>::values_));
        }
  
        Vector_Base<P>::length_= other.length_;
        Vector_Base<P>::values_ = other.values_;
        other.values_=nullptr;
        other.length_ = 0;
      }

      return(*this);
   }

  template<typename Derived>
  Vector& operator=(const _Expr<Derived>&other)
  {
      eval(*this,other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
  }

  template<typename T,
           typename std::enable_if<is_scalar<T>(),bool>::type = true>
  Vector& operator=(const T other)
  {
      _Fill(*this,other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
  }

   template<typename Derived>
   Vector& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   Vector& operator +=(const Vector& other)
   {
      eval(*this,*this + other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   Vector& operator +=(const P other)
   {
      eval(*this,*this + other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   Vector& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   Vector& operator -=(const Vector& other)
   {
      eval(*this,*this - other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   Vector& operator -=(const P other)
   {
      eval(*this,*this - other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   template<typename Derived>
   Vector& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   Vector& operator *=(const Vector& other)
   {
      eval(*this,*this * other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };


   Vector& operator *=(const P other)
   {
      eval(*this,*this * other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   template<int S=1>
   VectorView<P,S> sub(const index_t start=0,const index_t stop=-1)
   {
     if  (stop<0)
     {
        return(VectorView<P,S>(*this,start,Vector_Base<P>::length()));
     }
     else
     {
       return(VectorView<P,S>(*this,start,stop));
     }
   }

   template<int S=1>
   const VectorView<P,S> sub(const index_t start=0,const index_t stop=-1) const
   {
     if  (stop<0)
     {
        return(VectorView<P,S>(*this,start,Vector_Base<P>::length()));
     }
     else
     {
       return(VectorView<P,S>(*this,start,stop));
     }
   }

   

   virtual ~Vector() {
      if (Vector_Base<P>::values_!=nullptr)
      {
         Allocator<DYNAMIC>::destroy(reinterpret_cast<char*>(Vector_Base<P>::values_));
      }
    }

};

/*! @} */

}

