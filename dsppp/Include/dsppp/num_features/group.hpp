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

/**
 * @brief      Number description for a tuple of numbers
 *
 * @tparam     E     Datatype for all numbers
 */
template<typename ... E>
struct number_traits<std::tuple<E...>>
{
   //! It is not a float number
   static constexpr bool is_float = false;

   //! It is not a fixed point number
   static constexpr bool is_fixed = false;

   //! Tuple of accumulator datatype for the accumulator type
   typedef std::tuple<typename number_traits<E>::accumulator...> accumulator;
   
   //! Tuple of compute datatype for the compute type
   typedef std::tuple<typename number_traits<E>::compute_type...> compute_type;

   /**
    * @brief      Return of tuples of one values
    *
    * @return     Tuples of one values with different datatypes
    */
   static std::tuple<typename number_traits<E>::accumulator...> one()
   {
       return(std::make_tuple(vector_traits<E>::one()...));
   }

};

/*

Assume that all E are using the same scalar type or coherent types
like f32 and q13 that have same number of lanes.

Any other mix will not work and won't be caught at build time.

*/

/**
 * @brief      Tuple of compatible vectors
 *
 * @tparam     arch  Current architecture
 * @tparam     E     List of vector dataypes
 * 
 * The vector datatypes must be coherent : have same number of lanes
 * or same lane datatype
 */
template<typename arch,typename ... E>
struct vector_traits<std::tuple<E...>,arch> {

  //! First element of tuple defines the scalar datatype
  using RefScalar = typename std::tuple_element<0,std::tuple<E...>>::type;


  //! Temporary accumulator datatype
  typedef std::tuple<typename vector_traits<E,arch>::temp_accumulator...> temp_accumulator;
  
  //! Vector datatype
  typedef std::tuple<typename vector_traits<E,arch>::vector...> vector;

  //! Real vector datatype
  typedef std::tuple<typename vector_traits<E,arch>::real_vector...> real_vector;
  
  //! Predicate datatype
  typedef std::tuple<typename vector_traits<E,arch>::predicate_t...> predicate_t;

  //! Number of lanes (from RefScalar)
  static constexpr int nb_lanes = vector_traits<RefScalar,arch>::nb_lanes;

  //! Has vector instructions
  static constexpr bool has_vector = vector_traits<RefScalar,arch>::has_vector;
  
  //! Is a float
  static constexpr bool is_float = vector_traits<RefScalar,arch>::is_float;
  
  //! Is fixed point
  static constexpr bool is_fixed = vector_traits<RefScalar,arch>::is_fixed;
  
  //! Has predicated loop
  static constexpr bool has_predicate = vector_traits<RefScalar,arch>::has_predicate;

  /**
   * @brief      Zero represented with temp accumulator datatype
   *
   * @return     Zero represented with temp accumulator datatype
   */
  static temp_accumulator temp_acc_zero()
  {
     return(std::make_tuple(vector_traits<E,arch>::temp_acc_zero()...));
  }

};

/**
 * Inner implementation of generic intrinsics
 * \ingroup GenericTUPLENumber
 */
namespace inner {

 

  /*

  Assume that the vctpq is the same for all tuple elements.
  If it is not the case, we can't get a predicated loop and
  the code contains additional VPSTTTT and it is not
  efficient.

  */
#if defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
  /**
   * @brief      Tuple of predicates
   *
   * @tparam     E     Tuple of datatypes
   * 
   * The datatypes must be coherent (same number of lanes).
   * The first element is used to infer the vctpq instruction to use
   */
  template<typename ...E>
  struct vctpq<std::tuple<E...>>
  {
     /**
      * @brief      Make a predicate for predicated loop
      *
      * @param[in]  v     Remaining number of iterations
      *
      * @return     Predicate
      */
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

  /**
   * @brief      Vector accumulate for tuples of vectors
   *
   * @param[in]  acc        The accumulator
   * @param[in]  a          First operand
   * @param[in]  b          Second operand
   *
   * @tparam     A          Accumulator datatype
   * @tparam     V          Vector datatype
   * @tparam     Ns         Tuple index
   *
   * @return     tuple of results
   */
  template<typename A,typename VA,typename VB,std::size_t... Ns>
  __STATIC_FORCEINLINE A vmacc_impl(const A &acc,const VA &a,const VB &b, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(vmacc(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b))...));
  };

  /**
   * @brief      Vector accumulate for tuples of vectors
   *  
   * @param[in]  acc   The accumulator
   * @param[in]  a     First operand
   * @param[in]  b     Second operand
   *
   * @tparam     A     Accumulator datatype
   * @tparam     E     Datatype of tuples elements
   *
   * @return     Accumulator result
   */
  template<typename A,typename ...EA, typename ...EB>
  __STATIC_FORCEINLINE A 
  vmacc(const A &acc,const std::tuple<EA...> &a,const std::tuple<EB...> &b)
  {
     return(vmacc_impl(acc,a,b,std::make_index_sequence<sizeof...(EA)>()));
  };

  /**
   * @brief      Predicated vector accumulate for tuple
   *
   * @param[in]  acc        Accumulator
   * @param[in]  a          First operand
   * @param[in]  b          Second operand
   * @param[in]  p0         Predicate
   *
   * @tparam     A          Accumulator datatype
   * @tparam     V          Vector datatype
   * @tparam     B          Predicate datatype
   * @tparam     Ns         Tuple indexes
   *
   * @return     Tuple of accumulated values
   */
  template<typename A,typename VA,typename VB,typename B,std::size_t... Ns>
  __STATIC_FORCEINLINE A vmacc_impl(const A &acc,const VA &a,const VB &b, const B p0,std::index_sequence<Ns...>)
  {
     return(std::make_tuple(vmacc(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b),p0)...));
  };

  /**
   * @brief      Predicated vector accumulate for tuples
   *
   * @param[in]  acc   Accumulator
   * @param[in]  a     First operand
   * @param[in]  b     Second operand
   * @param[in]  p0    Predicate
   *
   * @tparam     A     Accumulator datatype
   * @tparam     B     Predicate datatype
   * @tparam     E     Dadatype of tuples elements
   *
   * @return     Tuple of accumulated vectors
   */
  template<typename A,typename B,typename ...EA, typename ...EB>
  __STATIC_FORCEINLINE A 
  vmacc(const A &acc,const std::tuple<EA...> &a,const std::tuple<EB...> &b,const B p0)
  {
     return(vmacc_impl(acc,a,b,p0,std::make_index_sequence<sizeof...(EA)>()));
  };

 

  /**
   * @brief      Reduce function for tuple
   *
   * @param[in]  acc        Accumulator
   *
   * @tparam     A          Accumulator datatype
   * @tparam     Ns         Tuple indexes
   *
   * @return     Reduced accumulator values
   * 
   * Some vector instructions sets cannot accumulate vectors
   * into a scalar. They accumulate into this vector.
   * This vector must be reduced to a scalar at the end of
   * the accumulation loop.
   */
  template<typename A,std::size_t... Ns>
  __STATIC_FORCEINLINE auto vreduce_impl(const A &acc, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(vreduce(std::get<Ns>(acc))...));
  };

/**
 * @brief      Reduce function for tuples
 *
 * @param[in]  acc   The accumulator
 *
 * @tparam     E     Datatypes for tuples
 *
 * @return     Tuples of reduced values
 * 
 * Some vector instructions sets cannot accumulate vectors
 * into a scalar. They accumulate into this vector.
 * This vector must be reduced to a scalar at the end of
 * the accumulation loop.
 * 
 */
  template<typename ...E>
  __STATIC_FORCEINLINE auto vreduce(const std::tuple<E...> &acc)
  {
     return(vreduce_impl(acc,std::make_index_sequence<sizeof...(E)>()));
  };

   /**
    * @brief      Convert from accumulator value
    *
    * @param[in]  acc        The accumulator
    *
    * @tparam     A          Accumulator datatype
    * @tparam     Ns         Tuples indexes
    *
    * @return     Tuples of values
    */
   template<typename A,std::size_t... Ns>
  __STATIC_FORCEINLINE auto from_accumulator_impl(const A &acc, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(from_accumulator(std::get<Ns>(acc))...));
  };

  /**
   * @brief      Convert from tuple of accumulator values
   *
   * @param[in]  acc   Accumulator
   *
   * @tparam     E     Datatypes for tuple
   *
   * @return     Tuples of converted accumulator values
   * 
   * Accumulator may use more bits to avoid saturations.
   * At the end of the accumulation, the final result must
   * be converted to the current datatype (it may implies saturation)
   */
  template<typename ...E>
  __STATIC_FORCEINLINE auto from_accumulator(const std::tuple<E...> &acc)
  {
     return(from_accumulator_impl(acc,std::make_index_sequence<sizeof...(E)>()));
  };

  /**
   * @brief      Multiply accumulate for tuple of scalar
   *
   * @param[in]  acc        Accumulator 
   * @param[in]  a          First operand
   * @param[in]  b          Second operand
   *
   * @tparam     A          Accumulator datatype
   * @tparam     V          Scalar datatype
   * @tparam     Ns         Tuple indexes
   *
   * @return     Tuples of accumulated values
   */
  template<typename A,typename VA,typename VB,std::size_t... Ns>
  __STATIC_FORCEINLINE A mac_impl(const A &acc,const VA &a,const VB &b, std::index_sequence<Ns...>)
  {
     return(std::make_tuple(mac(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b))...));
  };

/**
 * @brief      Multiply accumulate
 *
 * @param[in]  acc   Accumulator
 * @param[in]  a     First operand
 * @param[in]  b     Second operand
 *
 * @tparam     A     Accumulator datatype
 * @tparam     E     Datatypes for tuple
 *
 * @return     Accumulated values
 */
  template<typename A,typename ...EA, typename ...EB>
  __STATIC_FORCEINLINE A 
  mac(const A &acc,const std::tuple<EA...> &a,const std::tuple<EB...> &b)
  {
     return(mac_impl(acc,a,b,std::make_index_sequence<sizeof...(EA)>()));
  };

/**
   * @brief      Multiply accumulate for tuple of scalar
   *
   * @param[in]  acc        Accumulator 
   * @param[in]  a          First operand
   * @param[in]  b          Second operand
   * @param[in]  p0         Predicate
   *
   * @tparam     A          Accumulator datatype
   * @tparam     V          Scalar datatype
   * @tparam     B          Predicate datatype
   * @tparam     Ns         Tuple indexes
   *
   * @return     Tuples of accumulated values
   */
 template<typename A,typename VA,typename VB,typename B,std::size_t... Ns>
  __STATIC_FORCEINLINE A mac_impl(const A &acc,const VA &a,const VB &b, const B p0,std::index_sequence<Ns...>)
  {
     return(std::make_tuple(mac(std::get<Ns>(acc),std::get<Ns>(a),std::get<Ns>(b),p0)...));
  };

/**
 * @brief      Multiply accumulate
 *
 * @param[in]  acc   Accumulator
 * @param[in]  a     First operand
 * @param[in]  b     Second operand
 * @param[in]  p0    Predicate
 *
 * @tparam     A     Accumulator datatype
 * @tparam     B          Predicate datatype
 * @tparam     E     Datatypes for tuple
 *
 * @return     Accumulated values
 */
   template<typename A,typename B,typename ...EA, typename ...EB>
  __STATIC_FORCEINLINE A 
  mac(const A &acc,const std::tuple<EA...> &a,const std::tuple<EB...> &b,const B p0)
  {
     return(mac_impl(acc,a,b,p0,std::make_index_sequence<sizeof...(EA)>()));
  };

};


/*! @} */
/*! @} */