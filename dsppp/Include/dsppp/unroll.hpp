// -*- C++ -*-
/** @file */ 
#pragma once 

#include <utility>
#include <tuple>

#include "common.hpp"
#include "arch.hpp"
#include <type_traits>
#include "number.hpp"
#include "forward.hpp"
#include "fusion.hpp"
#include "matrix.hpp"

namespace arm_cmsis_dsp {

/** \addtogroup UNROLLING Unrolling
 *  \ingroup DSPPP
 *  @{
 */

template<typename ... E>
struct Merged
{

   using ScalarResult = std::tuple<typename traits<std::remove_reference_t<E>>::Scalar...>;
   using TypeOfElement = typename std::tuple_element<0,ScalarResult>::type;

   constexpr explicit Merged(const E& ... values) : vals { values ...} { }

   constexpr Merged(Merged&& other) = default; 
   constexpr Merged(const Merged& other) = default; 
   constexpr Merged& operator=(const Merged& other) = delete; 
   constexpr Merged& operator=(Merged&& other) = delete; 
   ~Merged() = default;

   constexpr vector_length_t length() const noexcept {return std::get<0>(vals).length();};

   template<std::size_t... Ns>
   constexpr ScalarResult val_impl(const int i, const std::index_sequence<Ns...>) const noexcept
   {
     return std::tuple(std::get<Ns>(vals)[i]...);
   }

   constexpr ScalarResult operator[](const int i)  noexcept{
    return val_impl(i,std::make_index_sequence<sizeof...(E)>());
   }  

   constexpr ScalarResult const operator[](const int i) const  noexcept{
    return val_impl(i,std::make_index_sequence<sizeof...(E)>());
   } 

#if defined(HAS_VECTOR)

   using Vector = std::tuple<typename vector_traits<typename traits<std::remove_reference_t<E>>::Scalar>::vector...>;

   template<std::size_t... Ns>
   void vector_store_impl(const index_t i,const Vector &val, const std::index_sequence<Ns...>) const noexcept
   {
        (inner::vstore1<1>((std::get<Ns>(vals).ptr(i)),std::get<Ns>(val)),...);
   }

   void vector_store(const index_t i,const Vector &val) const noexcept
   {
     vector_store_impl(i,val,std::make_index_sequence<sizeof...(E)>());
   }
   
#if defined(HAS_PREDICATED_LOOP)
   template<std::size_t... Ns>
   void vector_store_tail_impl(const index_t i,const vector_length_t remaining,const Vector &val, const std::index_sequence<Ns...>) const noexcept
   {
        (inner::vstore1_z<1>((std::get<Ns>(vals).ptr(i)),std::get<Ns>(val),remaining,inner::vctpq<TypeOfElement>::mk(remaining)),...);
   }


   void vector_store_tail(const index_t i,const vector_length_t remaining,const Vector &val) const noexcept
   {
     vector_store_tail_impl(i,remaining,val,std::make_index_sequence<sizeof...(E)>());
   }
#endif

 
   template<std::size_t... Ns>
   Vector vector_op_impl(const int i, const std::index_sequence<Ns...>) const noexcept
   {
     return std::make_tuple(std::get<Ns>(vals).vector_op(i)...);
   }

   Vector vector_op(const index_t i) const noexcept
   {
        return(vector_op_impl(i,std::make_index_sequence<sizeof...(E)>()));
   }

#if defined(HAS_PREDICATED_LOOP)
   template<std::size_t... Ns>
   Vector vector_op_tail_impl(const index_t i,const vector_length_t remaining, const std::index_sequence<Ns...>) const noexcept
   {
        return std::make_tuple(std::get<Ns>(vals).vector_op_tail(i,remaining)...);
   }

   Vector vector_op_tail(const index_t i,const vector_length_t remaining) const noexcept
    {
        return(vector_op_tail_impl(i,remaining,std::make_index_sequence<sizeof...(E)>()));
    }
#endif
#endif

   template<typename ... EA>
   Merged& operator=(const Merged<EA...>& other) noexcept
   {
       eval(*this,other,std::get<0>(vals).length(),CURRENT_ARCH);
       return(*this);
   }

   const std::tuple<E...> vals;
};

template <typename ..._Tp>
static inline Merged<_Tp&...>
results(_Tp&... __t) noexcept {return Merged<_Tp&...>(__t...);}


template<typename ... E>
struct traits<Merged<E...>>
{
    typedef std::tuple<typename traits<std::remove_reference_t<E>>::Scalar...> Scalar;

#if defined(HAS_VECTOR)
    typedef std::tuple<typename vector_traits<typename traits<std::remove_reference_t<E>>::Scalar>::vector...> Vector;
#endif
};

template<typename ... E>
struct IsVector<Merged<E...>>
{
    constexpr static bool value = true;
};

template<typename ... E>
struct IsDynamic<Merged<E...>>
{
    constexpr static bool value = (... && IsDynamic<std::remove_reference_t<E>>::value);
};

template<typename ... E>
struct ElementType<Merged<E...>>
{
    typedef std::tuple<typename ElementType<std::remove_reference_t<E>>::type...> type;
};

constexpr vector_length_t max_length(const vector_length_t a,const vector_length_t b) noexcept
{
   return((a>b) ? a : b);
};


template<typename F,typename ... N>
constexpr vector_length_t max_vec_length(F a,N ...b) noexcept
{
    if constexpr (sizeof...(b) == 0) 
    {
       return(a);
    }
    else 
    {
        return max_length(a,max_vec_length(b...));
    }
};


template<typename ... E>
struct StaticLength<Merged<E...>>
{
    constexpr static vector_length_t value = max_vec_length(StaticLength<std::remove_reference_t<E>>::value...);
};


  template<typename F,std::size_t... Ns>
  auto unroll_impl(const F& func,std::index_sequence<Ns...>) noexcept
  {
     return Merged{func(Ns)...};
  };

  template<int N,typename F>
  auto unroll(const F& func) noexcept
  {
     return unroll_impl(func,std::make_index_sequence<N>());
  };

  template<typename E>
  constexpr static const E& constres(const E& r,const std::size_t) noexcept
  {
    return(r);
  }

  template<typename E,std::size_t... Ns>
  auto replicate_impl(const E& expr,std::index_sequence<Ns...>) noexcept
  {
     return Merged{constres(expr,Ns)...};
  };

  template<int N,typename E>
  auto replicate(const E& expr) noexcept
  {
     return replicate_impl(expr,std::make_index_sequence<N>());
  };

  /*

  We don't want to replicate the Vector but only a reference
  to the vector. So it is packed into an expr

  */
  template<int N,typename P,int L,template<int> typename A>
  auto replicate(const Vector<P,L,A>& e) noexcept
  {
     //return replicate_impl(expr(e),std::make_index_sequence<N>());
     return replicate_impl(VectorView<P,1>(e),std::make_index_sequence<N>());
  };

  template<typename T,unsigned int N,std::size_t... Ns>
  auto results_impl(std::array<T,N> &a,std::index_sequence<Ns...>) noexcept
  {
     return std::tie(a[Ns]...);
  };

  template<unsigned int N,typename T>
  auto results(std::array<T,N> &a) noexcept
  {
     return results_impl(a,std::make_index_sequence<N>());
  };

  template<typename F,std::size_t... Ns>
  auto result_impl_func(const F& func,std::index_sequence<Ns...>) noexcept
  {
     return std::tie(*func(Ns)...);
  };

  template<int N,typename F>
  auto results(const F& func) noexcept
  {
     return result_impl_func(func,std::make_index_sequence<N>());
  };

/*! @} */
}
