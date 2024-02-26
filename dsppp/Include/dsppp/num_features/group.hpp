// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup GenericNumber 
 *  \ingroup NUMBER
 *  @{
 *  \addtogroup GenericTUPLENumber Tuple
 *  Tuples of numbers or expressions used for unrolling
 *  \ingroup GenericNumber
 *  @{
 */

template<typename ... E>
struct number_traits<std::tuple<E...>>
{
   static constexpr bool is_float = false;
   static constexpr bool is_fixed = false;
   typedef std::tuple<typename number_traits<E>::accumulator...> accumulator;
   typedef std::tuple<typename number_traits<E>::compute_type...> compute_type;

   static std::tuple<typename number_traits<E>::accumulator...> one()
   {
       return(std::make_tuple(vector_traits<E>::one()...));
   }

};

/*

Assume that all E are using the same scalar type or coherent types
like f32 and q13 that have same number of lanes.

Any other mix will not work and won't be catched at build time.

*/
template<typename arch,typename ... E>
struct vector_traits<std::tuple<E...>,arch> {
  using RefScalar = typename std::tuple_element<0,std::tuple<E...>>::type;

  typedef std::tuple<typename vector_traits<E,arch>::temp_accumulator...> temp_accumulator;
  typedef std::tuple<typename vector_traits<E,arch>::vector...> vector;
  typedef std::tuple<typename vector_traits<E,arch>::predicate_t...> predicate_t;

  static constexpr int nb_lanes = vector_traits<RefScalar,arch>::nb_lanes;

  static constexpr bool has_vector = vector_traits<RefScalar,arch>::has_vector;
  static constexpr bool is_float = vector_traits<RefScalar,arch>::is_float;
  static constexpr bool is_fixed = vector_traits<RefScalar,arch>::is_fixed;
  static constexpr bool has_predicate = vector_traits<RefScalar,arch>::has_predicate;

  static temp_accumulator temp_acc_zero()
  {
     return(std::make_tuple(vector_traits<E,arch>::temp_acc_zero()...));
  }

};

namespace inner {

 

  /*

  Assume that the vctpq is the same for all tuple elements.
  If it is not the case, we can't get a predicated loop and
  the code contains additional VPSTTTT and it is not
  efficient.

  */
#if defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
  template<typename ...E>
  struct vctpq<std::tuple<E...>>
  {
     static auto mk(const uint32_t v/*,
      typename std::enable_if<(vector_traits<E>::nb_lanes == ...),bool>::type* = nullptr*/)
     {
        return(vctpq<std::tuple_element_t<0, std::tuple<E...>>>::mk(v));
     };
  };
#endif
  /*

  Typical configuration is vmacc between tuple and tuple
  but also very common is vmacc between tuple and vector

  */

  template<typename A,typename V,std::size_t... Ns>
  __STATIC_FORCEINLINE A vmacc_impl(const A &acc,const V &a,const V &b, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(vmacc(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b))...));
  };

  template<typename A,typename ...E>
  __STATIC_FORCEINLINE A 
  vmacc(const A &acc,const std::tuple<E...> &a,const std::tuple<E...> &b)
  {
     return(vmacc_impl(acc,a,b,std::make_index_sequence<sizeof...(E)>()));
  };

  template<typename A,typename V,typename B,std::size_t... Ns>
  __STATIC_FORCEINLINE A vmacc_impl(const A &acc,const V &a,const V &b, const B p0,std::index_sequence<Ns...>)
  {
     return(std::make_tuple(vmacc(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b),p0)...));
  };

  template<typename A,typename B,typename ...E>
  __STATIC_FORCEINLINE A 
  vmacc(const A &acc,const std::tuple<E...> &a,const std::tuple<E...> &b,const B p0)
  {
     return(vmacc_impl(acc,a,b,p0,std::make_index_sequence<sizeof...(E)>()));
  };

 

  template<typename A,std::size_t... Ns>
  __STATIC_FORCEINLINE auto vreduce_impl(const A &acc, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(vreduce(std::get<Ns>(acc))...));
  };

  template<typename ...E>
  __STATIC_FORCEINLINE auto vreduce(const std::tuple<E...> &acc)
  {
     return(vreduce_impl(acc,std::make_index_sequence<sizeof...(E)>()));
  };

   template<typename A,std::size_t... Ns>
  __STATIC_FORCEINLINE auto from_accumulator_impl(const A &acc, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(from_accumulator(std::get<Ns>(acc))...));
  };

  template<typename ...E>
  __STATIC_FORCEINLINE auto from_accumulator(const std::tuple<E...> &acc)
  {
     return(from_accumulator_impl(acc,std::make_index_sequence<sizeof...(E)>()));
  };

  template<typename A,typename V,std::size_t... Ns>
  __STATIC_FORCEINLINE A mac_impl(const A &acc,const V &a,const V &b, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(mac(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b))...));
  };

  template<typename A,typename ...E>
  __STATIC_FORCEINLINE A 
  mac(const A &acc,const std::tuple<E...> &a,const std::tuple<E...> &b)
  {
     return(mac_impl(acc,a,b,std::make_index_sequence<sizeof...(E)>()));
  };

 template<typename A,typename V,typename B,std::size_t... Ns>
  __STATIC_FORCEINLINE A mac_impl(const A &acc,const V &a,const V &b, const B p0,std::index_sequence<Ns...>)
  {
     return(std::make_tuple(mac(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b),p0)...));
  };

   template<typename A,typename B,typename ...E>
  __STATIC_FORCEINLINE A 
  mac(const A &acc,const std::tuple<E...> &a,const std::tuple<E...> &b,const B p0)
  {
     return(mac_impl(acc,a,b,p0,std::make_index_sequence<sizeof...(E)>()));
  };

};


/*! @} */
/*! @} */