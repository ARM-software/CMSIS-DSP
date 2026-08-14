// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup ARCHALG Architecture specific algorithm
 *  \ingroup DSPPP
 *  \addtogroup SCALARALG Scalar algorithm
 *  \ingroup ARCHALG
 *  @{
 */


#define SCALAR_UNROLL 2

template<typename DST,typename SRC>
inline void _round_to_nearest(DST& destination, const SRC& source,
                              const vector_length_t length,
                              const Scalar* = nullptr)
{
   using T = typename traits<DST>::Scalar;
   for (index_t i = 0; i < length; ++i)
      destination[i] = static_cast<T>(
         std::nearbyint(static_cast<float>(source[i])));
}

template<typename DST,typename SRC>
inline void _round_to_nearest_clipped(
   DST& destination, const SRC& source,
   typename traits<DST>::Scalar offset,
   typename traits<DST>::Scalar minimum,
   typename traits<DST>::Scalar maximum,
   const vector_length_t length, const Scalar* = nullptr)
{
   using T = typename traits<DST>::Scalar;
   for (index_t i = 0; i < length; ++i)
   {
      float value = std::nearbyint(static_cast<float>(source[i])) +
                    static_cast<float>(offset);
      if (value < static_cast<float>(minimum))
         value = static_cast<float>(minimum);
      if (value > static_cast<float>(maximum))
         value = static_cast<float>(maximum);
      destination[i] = static_cast<T>(value);
   }
}

template<typename DST,typename SRC>
inline void _round_scaled_to_nearest_clipped(
   DST& destination, const SRC& source, float multiplier,
   typename traits<DST>::Scalar offset,
   typename traits<DST>::Scalar minimum,
   typename traits<DST>::Scalar maximum,
   const vector_length_t length, const Scalar* = nullptr)
{
   using T = typename traits<DST>::Scalar;
   for (index_t i = 0; i < length; ++i)
   {
      float value = std::nearbyint(
         static_cast<float>(source[i]) * multiplier) +
         static_cast<float>(offset);
      if (value < static_cast<float>(minimum))
         value = static_cast<float>(minimum);
      if (value > static_cast<float>(maximum))
         value = static_cast<float>(maximum);
      destination[i] = static_cast<T>(value);
   }
}

template<typename MASK>
inline bool _nearest_even_range_predicate(
   const MASK& mask, index_t i, vector_length_t,
   const Scalar* = nullptr)
{
   return mask[i];
}

template<typename DST,typename SRC,typename MASK>
inline void _masked_scale_add(DST& destination, const SRC& source,
                              const MASK& mask,
                              typename traits<DST>::Scalar scale,
                              const vector_length_t length,
                              const Scalar* architecture = nullptr)
{
   for (index_t i = 0; i < length; ++i)
      if (_nearest_even_range_predicate(mask, i, 1, architecture))
         destination[i] += source[i] * scale;
}

template<typename A,typename B,typename MASK>
inline auto _masked_dot_sum(const A& a, const B& b, const MASK& mask,
                            const vector_length_t length,
                            const Scalar* architecture = nullptr)
{
   using T = typename traits<A>::Scalar;
   MaskedDotSum<T> result{T{}, T{}};
   for (index_t i = 0; i < length; ++i)
      if (_nearest_even_range_predicate(mask, i, 1, architecture))
      {
         result.dot += a[i] * b[i];
         result.sum += a[i];
      }
   return result;
}

/**
 * @brief      Fill evaluator for scalar architecture
 *
 * @param      v          Destination vector 
 * @param[in]  val        Initialization value
 * @param[in]  l          Length of vector
 *
 * @tparam     T          Scalar datatype
 * @tparam     DST        VEctor / Matrix datatype
 * @tparam     <unnamed>  Test to restrict to vector addressing 
 *                        and compatible datatype
 * 
 */
template<typename T,typename DST,
typename std::enable_if<IsVector<DST>::value &&
         compatible_element<DST,T>(),bool>::type = true>
inline void _Fill(DST &v,
                  const T val, 
                  vector_length_t l,
                  const Scalar* = nullptr)
{
    constexpr unsigned int U = SCALAR_UNROLL;
    index_t i;

    UNROLL_LOOP
    for(i=0 ; i <= l-(1<<U); i += (1<<U))
    {
        for(int k=0;k < (1<<U);k++)
        {
           v[i+k] = val;
        }
    }

    for(; i < l ; i++)
    {
       v[i] = val;
    }
}

/**
 * @brief      Fill2D evaluator for scalar architecture
 *
 * @param      v          Matrix value
 * @param[in]  val        Initialization value
 * @param[in]  rows       Number of rows
 * @param[in]  cols       Number of columns
 *
 * @tparam     T          Scalar datatype
 * @tparam     DST        Matrix datatype
 * @tparam     <unnamed>  Check DST has matrix indexing only
 */
template<typename T,typename DST,
typename std::enable_if<must_use_matrix_idx<DST>() &&
         compatible_element<DST,T>(),bool>::type = true>
inline void _Fill2D(DST &v,
                    const T val, 
                    const vector_length_t rows,
                    const vector_length_t cols,
                    const Scalar* = nullptr)
{
      constexpr unsigned int U = SCALAR_UNROLL;
      index_t row=0;

      
      for(; row <= rows-(1<<U);row += (1<<U))
      {
          
          for(index_t col=0; col < cols;col ++)
          {
             for(int k=0;k<(1<<U);k++)
             {
                v(row+k,col) = val;
             }
          }

      }

      for(; row < rows;row ++)
      {
          for(index_t col=0; col < cols;col ++)
          {
              v(row,col) = val;
          }
      }
}


/**
 * @brief      Expression evaluator for vector in scalar mode
 *
 * @param      v          Vector
 * @param[in]  other      Expression
 * @param[in]  l          Length of expression (number of samples)
 *
 * @tparam     DA         Destination datatype
 * @tparam     DB         Source datatype
 * @tparam     <unnamed>  Check vectors are compatible
 */
template<typename DA,typename DB,
typename std::enable_if<vector_idx_pair<DA,DB>(),bool>::type = true>
inline void eval(DA &v,
                 const DB& other,
                 const vector_length_t l,
                 const Scalar* = nullptr)
{
    constexpr unsigned int U = SCALAR_UNROLL;
    index_t i=0;

    for(i=0 ; i <= l-(1<<U); i += (1<<U))
    {
        for(int k=0;k < (1<<U);k++)
        {
           v[i+k] = other[i+k];
        }
    }

    for(; i < l ; i++)
    {
       v[i] = other[i];
    }
}

/**
 * @brief      2D expression evaluator for scalar architecture
 *
 * @param      v          Destination value
 * @param[in]  other      The expression
 * @param[in]  rows       Number of rows
 * @param[in]  cols       Number of columns
 *
 * @tparam     DA         Destination datatype
 * @tparam     DB         Source datatype
 * @tparam     <unnamed>  Check only support matrix indexing
 */
template<typename DA,typename DB,
typename std::enable_if<must_use_matrix_idx_pair<DA,DB>(),bool>::type = true>
inline void eval2D(DA &v,
                   const DB& other,
                   const vector_length_t rows,
                   const vector_length_t cols,
                   const Scalar* = nullptr)
{
      constexpr unsigned int U = SCALAR_UNROLL;
      index_t row=0;

      
      for(; row <= rows-(1<<U);row += (1<<U))
      {
          for(index_t col=0; col < cols;col ++)
          {
             for(int k=0;k<(1<<U);k++)
             {
                v(row+k,col) = other(row+k,col);
             }
          }

      }


      for(; row < rows;row ++)
      {
          
          for(index_t col=0; col < cols;col ++)
          {
              v(row,col) = other(row,col);
          }
      }
}

// Some compilers are generating incorrect code when this value > 1
#undef SCALAR_UNROLL
#define SCALAR_UNROLL 1
/**
 * @brief      Dot product evaluator for scalar architectuire
 *
 * @param[in]  a          Left hand side
 * @param[in]  b          Right hand side
 * @param[in]  l          Vector length
 *
 * @tparam     DA         Left hand side datatype (may be expression)
 * @tparam     DB         Right hand side datatype (may be expression)
 * @tparam     <unnamed>  Check vector expressions are compatible
 *
 * @return     Dot product result
 */
template<typename DA,typename DB,
         typename std::enable_if<vector_idx_pair<DA,DB>(),bool>::type = true>
inline DotResult<DotFieldResult<DA,DB>> _dot(const DA& a,
                                              const DB& b,
                                              const vector_length_t l,
                                              const Scalar* = nullptr)
{
    using ScalarResult = DotFieldResult<DA,DB>;
    using Acc = DotResult<ScalarResult>;
    constexpr unsigned int U = SCALAR_UNROLL;
    index_t i;

    Acc acc = Acc{};

    for(i=0 ; i <= l-(1<<U); i += (1<<U))
    {
        for(int k=0;k < (1<<U);k++)
        {
           acc = inner::mac(acc , a[i+k] , b[i+k]);
        }
    }

    for(; i < l ; i++)
    {
       acc = inner::mac(acc , a[i] , b[i]);
    }

    return(acc);
}

#undef SCALAR_UNROLL
#define SCALAR_UNROLL 2
/**
 * @brief      Swap evaluator for scalar architecture
 *
 * @param      a          Left hand side
 * @param      b          Right hand side
 * @param[in]  l          Vector length
 *
 * @tparam     DA         Left hand side datatype
 * @tparam     DB         Right hand side datatype
 * @tparam     <unnamed>  Check vectors are compatible
 */
template<typename DA,typename DB,
         typename std::enable_if<vector_idx_pair<DA,DB>(),bool>::type = true>
inline void _swap(DA&& a,
                  DB&& b,
                  const vector_length_t l,
                  const Scalar* = nullptr)
{
   for(index_t i=0;i<l;i++)
   {
     const auto tmp = a[i];
     a[i] = b[i];
     b[i] = tmp;
   }

}

#undef SCALAR_UNROLL

/*! @} */
