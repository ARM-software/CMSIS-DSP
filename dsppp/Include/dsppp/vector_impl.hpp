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

/** @brief Storage for a vector
 *  @tparam P Type of the scalar
 */
template<typename P>
struct Vector_Base {

    //! Type of vector elements
    typedef P element_type;


    /**
    * @brief   Vector dimension
    * @return Vector dimension
    *
    */
    vector_length_t length() const   {return(length_);};

    /**
    * @brief      Pointer to storage buffer
    * @return Pointer to storage
    */
    P* ptr() const {return(values_);}

    /**
    * @brief      Pointer to storage buffer at index i
    *
    * @param i Index in buffer
    * @return Pointer to storage
    *
    */
    P* ptr(const index_t i) const {return(&values_[i]);}

    /**
    * @brief      Pointer to storage buffer
    * @return Pointer to constant storage
    *
    */
    const P* const_ptr() const {return(values_);}

    /**
    * @brief      Pointer to storage buffer at index i
    *
    * @param i Index in buffer
    * @return Pointer to constant storage
    *
    */
    const P* const_ptr(const index_t i) const {return(&values_[i]);}


    
    /**
    * @brief   Iterator begin
    *
    * @return Pointer to start of buffer
    *
    */
    P* begin() const {return(values_);}

    /**
    * @brief   Iterator end
    *
    * @return Pointer to first element after end of buffer
    *
    */
    P* end() const {return(values_+length_);}

    /**
    * @brief  Display the vector content for debug purpose
    * @param stream Output stream
    * @param other The vector to display
    * @return the stream
    * 
    */
    friend std::ostream& operator<< (std::ostream& stream, const Vector_Base<P>& other) {
        using DT = typename number_traits<P>::display_type;
        constexpr int nb = 10;
        int i=0;

        for(index_t k=0;k<other.length();k++)
        {
           stream << (DT)other[k] << " , ";
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


   /**
    * @brief      Element at index i
    *
    * @param i index
    * @return     Reference to element
    */
   P& operator[](const index_t i)
   {
       return(values_[i]);
   }

   /**
    * @brief      Element at index i
    *
    * @param i index
    * @return     Reference to element
    */
   P& operator[](const index_t i) const
   {
       return(values_[i]);
   }

#if defined(HAS_VECTOR)
    //! Type of vectors for a vector architecture and for scalar datatype P
    using Vector = typename vector_traits<P>::vector;

    /**
    * @brief   %Vector store at index i
    *
    * @tparam T scalar datatype
    * @param i index
    * @param val Vector value
    * 
    * On an architecture supporting vectors, if the scalar datatype T
    * has a corresponding vector datatype, this function stores a vector
    * value at index i in this vector datatype
    */
    template<typename T=P,
             typename std::enable_if<vector_traits<T>::has_vector,bool>::type = true>
    void vector_store(const index_t i,const Vector val) const
    {
        inner::vstore1<1>((typename std::remove_cv<P>::type*)(&values_[i]),val);
    }

    template<typename T=P,
             typename std::enable_if<vector_traits<T>::has_vector
             && IsComplexNumber<T>::value
             && vector_traits<T>::support_mixed,bool>::type = true>
    void vector_store(const index_t i,const typename vector_traits<T>::real_vector val) const
    {
        inner::vstore1<1>((typename std::remove_cv<P>::type*)(&values_[i]),val);
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
    template<typename T=P,
    typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    void vector_store_tail(const index_t i,const vector_length_t remaining,const Vector val) const
    {
        inner::vstore1_z<1>((typename std::remove_cv<P>::type*)(&values_[i]),val,remaining,inner::vctpq<P>::mk(remaining));
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
    template<typename T=P,
    typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const vector_op_tail(const index_t i,const vector_length_t remaining) const
    {
        return(inner::vload1_z<1>((typename std::remove_cv<P>::type*)(&values_[i]),remaining,inner::vctpq<P>::mk(remaining)));
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

/** @brief Vector template for size knonw at build time
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
    *             A vector can be initialized like an array using {} syntax
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

  /**
    * @brief      Create a vector from a vector using a different memory allocator
    * 
    * @param other Other vector using a different memory allocator
    */
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

    /**
    * @brief      Create a vector from a VectorView
    * 
    * @tparam S The stride of the vector view known at build time
    * @param other The vector view
    */
   template<int S>
   explicit Vector(const VectorView<P,S>& other):Vector_Base<P>(L,Vector::allocate())
   {
        eval(*this,+other,(vector_length_t)L,CURRENT_ARCH);
   };
   

  
    /**
    * @brief      Create a vector from an expression
    * 
    * @tparam Derived The type representing the abstract syntax tree
    * @param other The expression
    * 
    * It is the mechanism allowing to evaluate an expression
    * and merge all of the operators the of the expression in the
    * same loop
    */
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

  /**
  * @brief      Copy result of an expression to a vector content
  * 
  * @tparam Derived The type representing the abstract syntax tree
  * @param other The expression
  * @return A reference to the vector
  * 
  * It is the mechanism allowing to evaluate an expression
  * and merge all of the operators the of the expression in the
  * same loop
  */
  template<typename Derived>
  Vector& operator=(const _Expr<Derived>&other)
  {
      eval(*this,other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
  }


  /**
  * @brief      Fill a vector with a constant
  * 
  * @tparam T The constant datatype
  * @param other The const
  * @return A reference to the vector
  * 
  */
  template<typename T,
           typename std::enable_if<is_scalar<T>(),bool>::type = true>
  Vector& operator=(const T other)
  {
      _Fill(*this,other,L,CURRENT_ARCH);
      return(*this);
  }


   

   /**
   * @brief    Elementwise add the result of an expression to a vector
   * 
   * @tparam Derived The type representing the abstract syntax tree of the expression
   * @param other The expression
   * @return A reference to the vector
   * 
   */
   template<typename Derived>
   Vector& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   /**
   * @brief    Elementwise add vector to another vector
   * 
   * @param other The vector
   * @return A reference to the vector
   * 
   */
   Vector& operator +=(const Vector& other)
   {
      eval(*this,*this + other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };


   /**
   * @brief    Elementwise add a constant to a vector
   * 
   * @tparam P The constant datatype
   * @param other The expression
   * @return A reference to the vector
   * 
   */
   Vector& operator +=(const P other)
   {
      eval(*this,*this + other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   /**
   * @brief    Elementwise subtract the result of an expression from a vector
   * 
   * @tparam Derived The type representing the abstract syntax tree of the expression
   * @param other The expression
   * @return A reference to the vector
   * 
   */
   template<typename Derived>
   Vector& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };


   /**
   * @brief    Elementwise subtract a vector from a vector
   * 
   * @param other The other vector
   * @return A reference to the vector
   * 
   */
   Vector& operator -=(const Vector& other)
   {
      eval(*this,*this - other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   /**
   * @brief    Elementwise subtract a constant from a vector
   * 
   * @tparam P Datatype of the constant
   * @param other The constant
   * @return A reference to the vector
   * 
   */
   Vector& operator -=(const P other)
   {
      eval(*this,*this - other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };


   /**
   * @brief    Elementwise multiply the result of an expression with a vector
   * 
   * @tparam Derived The type representing the abstract syntax tree of the expression
   * @param other The expression
   * @return A reference to the vector
   * 
   */
   template<typename Derived>
   Vector& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   /**
   * @brief    Elementwise multiply a vector with a vector
   * 
   * @param other The othr vector
   * @return A reference to the vector
   * 
   */
   Vector& operator *=(const Vector& other)
   {
      eval(*this,*this * other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };

   /**
   * @brief    Elementwise multiply a constant with a vector
   * 
   * @tparam Derived Constant datatype
   * @param other The constant
   * @return A reference to the vector
   * 
   */
   Vector& operator *=(const P other)
   {
      eval(*this,*this * other,(vector_length_t)L,CURRENT_ARCH);
      return(*this);
   };



   /**
   * @brief    Create a vector view
   * 
   * @tparam S Stride known at build time
   * @param start Start index for the vector view
   * @param stop Stop index for the vector view (first element after the vector view)
   *             Default is length of the vector if known at build time.
   * @return A reference to the vector view
   * 
   */
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


/** @brief Vector template for dynamic vector (size known at runtime)
 *  @tparam P Type of the scalar
 *  @tparam Allocator Memory allocator to use. By default it is the macro `TMP_ALLOC`
 */
template<typename P,
         template<int> typename Allocator>
struct Vector<P,DYNAMIC,Allocator>:Vector_Base<P> {

    /**
    * @brief      Allocate a buffer for this vector using the memory allocator
    *
    * @param length Vector dimension
    * @return     A new memory buffer
    */
    static char* allocate(vector_length_t length){return(Allocator<DYNAMIC>::allocate(sizeof(P)*length));};

    Vector() = delete;

    /**
    * @brief  Create a new vector
    *
    * @param length Vector dimension
    * @param init_val Initialization value
    */
    explicit Vector(vector_length_t length,P init_val):
    Vector_Base<P>(length,Vector::allocate(length),init_val){};
      
    /**
    * @brief  Create a new vector
    *
    * @param length Vector dimension
    */
    explicit Vector(vector_length_t length):
    Vector_Base<P>(length,Vector::allocate(length)){};


    /**
    * @brief  Create a new vector
    *
    * @param l Initializer list
    *          A vector can be initialized like an array using {} syntax
    */
    explicit Vector(const std::initializer_list<P> &l)
    :Vector_Base<P>(l.size(),Vector::allocate(l.size())){
       std::memcpy(Vector_Base<P>::values_,l.data(),sizeof(P)*l.size());
    };
   
   /**
    * @brief  Create a new vector from a vector using a different memory allocator
    *
    * @tparam K Dimension of other vector (statically known or dynamic)
    * @param other The vector to copy
    */
   template<int K,template<int> typename OtherAllocator>
   explicit Vector(const Vector<P,K,OtherAllocator>& other):Vector_Base<P>(other.length(),Vector::allocate(other.length()))
   {
        eval(*this,+other,Vector_Base<P>::length(),CURRENT_ARCH);
   };

   
   /**
    * @brief  Create a new vector from a vector of same type
    *
    * @param other The vector to copy
    */
   Vector(const Vector& other):Vector_Base<P>(other.length(),Vector::allocate(other.length()))
   {
        eval(*this,+other,Vector_Base<P>::length(),CURRENT_ARCH);

        //std::memcpy(Vector_Base<P>::values_,other.values_,vector_size);
   };

   /**
    * @brief  Create a new vector from a vector view
    *
    * @tparam S Stride of vector view known at build time
    * @param other The vector to copy
    */
   template<int S>
   explicit Vector(const VectorView<P,S>& other):Vector_Base<P>(other.length(),Vector::allocate(other.length()))
   {
        eval(*this,+other,Vector_Base<P>::length(),CURRENT_ARCH);
   };

   /**
    * @brief  Create a new vector from an expressipn
    *
    * @tparam Derived Type representing the abstract syntax tree of the expression
    * @param other The expression to evaluate
    */
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

  /**
    * @brief  Fill a vector with an expression
    *
    * @tparam Derived Type representing the abstract syntax tree of the expression
    * @param other The expression to evaluate
    * @return A reference to the vector
    */
  template<typename Derived>
  Vector& operator=(const _Expr<Derived>&other)
  {
      eval(*this,other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
  }

  /**
    * @brief  Fill a vector with a scalar
    *
    * @tparam T Scalar datatype
    * @param other The scalar
    * @return A reference to the vector
    */
  template<typename T,
           typename std::enable_if<is_scalar<T>(),bool>::type = true>
  Vector& operator=(const T other)
  {
      _Fill(*this,other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
  }

    /**
    * @brief  Elementwise add an expression to a vector
    *
    * @tparam Derived Type representing the abstract syntax tree of the expression
    * @param other The expression to evaluate
    * @return A reference to the vector
    */
   template<typename Derived>
   Vector& operator +=(const _Expr<Derived>& other)
   {
      eval(*this,*this + other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise add a vector to a vector
    *
    * @param other The vector to add
    * @return A reference to the vector
    */
   Vector& operator +=(const Vector& other)
   {
      eval(*this,*this + other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise add a scalar to a vector
    *
    * @tparam P Scalar datatype
    * @param other The scalar
    * @return A reference to the vector
    */
   Vector& operator +=(const P other)
   {
      eval(*this,*this + other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise subtract an expression to a vector
    *
    * @tparam Derived Type representing the abstract syntax tree of the expression
    * @param other The expression to evaluate
    * @return A reference to the vector
    */
   template<typename Derived>
   Vector& operator -=(const _Expr<Derived>& other)
   {
      eval(*this,*this - other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise subtract a vector to a vector
    *
    * @param other The vector to add
    * @return A reference to the vector
    */
   Vector& operator -=(const Vector& other)
   {
      eval(*this,*this - other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise subtract a scalar to a vector
    *
    * @tparam P Scalar datatype
    * @param other The scalar
    * @return A reference to the vector
    */
   Vector& operator -=(const P other)
   {
      eval(*this,*this - other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise multiply an expression with a vector
    *
    * @tparam Derived Type representing the abstract syntax tree of the expression
    * @param other The expression to evaluate
    * @return A reference to the vector
    */
   template<typename Derived>
   Vector& operator *=(const _Expr<Derived>& other)
   {
      eval(*this,*this * other.derived(),Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Elementwise multiply a vector with a vector
    *
    * @param other The vector to add
    * @return A reference to the vector
    */
   Vector& operator *=(const Vector& other)
   {
      eval(*this,*this * other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };


  /**
    * @brief  Elementwise multiply a scalar with a vector
    *
    * @tparam P Scalar datatype
    * @param other The scalar
    * @return A reference to the vector
    */
   Vector& operator *=(const P other)
   {
      eval(*this,*this * other,Vector_Base<P>::length(),CURRENT_ARCH);
      return(*this);
   };

   /**
    * @brief  Create a vector view
    *
    * @tparam S stride
    * @param start Start index of view
    * @param stop Stop index of view (first index after end of view)
    *              By default it is the length of the vector.
    * @return The vector view
    */
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

