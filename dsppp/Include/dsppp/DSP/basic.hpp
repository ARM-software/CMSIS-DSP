// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_MATH_DSP
#undef ARM_MATH_MVEI
#undef ARM_MATH_MVEF
#undef ARM_MATH_NEON
#endif

/** \addtogroup ARCHALG 
 *  \addtogroup DSPALG DSP Extension specific algorithm
 *  \ingroup ARCHALG
 *  @{
 */

#if defined(ARM_MATH_DSP)
#if !defined(ARM_MATH_MVEI) && !defined(ARM_MATH_MVEF) && !defined(ARM_MATH_NEON)

#define DSP_UNROLL 1

template<typename T,typename DST,
typename std::enable_if<has_vector_inst<DST>() &&
                        IsVector<DST>::value &&
         SameElementType<DST,T>::value,bool>::type = true>
inline void _Fill(DST &v,
                  const T val, 
                  vector_length_t l,
                  const DSP* = nullptr)
{
    constexpr int nb_lanes = vector_traits<T>::nb_lanes;
    index_t i;

    for(i=0 ; i <= l-(nb_lanes<<DSP_UNROLL); i += (nb_lanes<<DSP_UNROLL))
    {
        for(int k=0;k < (1<<DSP_UNROLL);k++)
        {
           v.vector_store(i + k*nb_lanes,inner::vconst(val));
        }
    }

    for(; i < l ; i++)
    {
       v[i] = val;
    }
}


template<typename T,typename DST,
typename std::enable_if<has_vector_inst<DST>() &&
         must_use_matrix_idx<DST>() &&
         SameElementType<DST,T>::value,bool>::type = true>
inline void _Fill2D(DST &v,
                    const T val, 
                    const vector_length_t rows,
                    const vector_length_t cols,
                    const DSP* = nullptr)
{
    constexpr int nb_lanes = vector_traits<T>::nb_lanes;
    index_t row=0;

      for(; row <= rows-(1<<DSP_UNROLL);row += (1<<DSP_UNROLL))
      {
          index_t col;

          for(col=0; col <= cols-nb_lanes;col += nb_lanes)
          {
              for(int k=0;k<(1<<DSP_UNROLL);k++)
              {
                  v.matrix_store(row+k,col,inner::vconst(val));
              }
          }

          for(; col < cols;col += nb_lanes)
          {
             for(int k=0;k<(1<<DSP_UNROLL);k++)
             {
                v(row+k,col) = val;
             }
          }

      }

      for(; row < rows;row ++)
      {
          index_t col;
          for(col=0; col <= cols-nb_lanes;col += nb_lanes)
          {
              v.matrix_store(row,col,inner::vconst(val));
          }

          for(; col < cols;col += nb_lanes)
          {
              v(row,col) = val;
          }
      }
}


/*

Evaluation : used when result is a vector

*/
template<typename DA,typename DB,
typename std::enable_if<has_vector_inst<DA>() &&
                        vector_idx_pair<DA,DB>(),bool>::type = true>
inline void eval(DA &v,
                 const DB& other,
                 const vector_length_t l,
                 const DSP* = nullptr)
{
    using T = typename traits<DA>::Scalar;
    constexpr int nb_lanes = vector_traits<T>::nb_lanes;
    constexpr unsigned int U = DSP_UNROLL;
    index_t i;

    for(i=0 ; i <= l-(nb_lanes<<U); i += (nb_lanes<<U))
    {
        for(int k=0;k < (1<<U);k++)
        {
           v.vector_store(i + k*nb_lanes,other.vector_op(i+k*nb_lanes));
        }
    }

    for(; i < l ; i++)
    {
       v[i] = other[i];
    }
}

template<typename DA,typename DB,
typename std::enable_if<has_vector_inst<DA>() &&
                        must_use_matrix_idx_pair<DA,DB>(),bool>::type = true>
inline void eval2D(DA &v,
                   const DB& other,
                   const vector_length_t rows,
                   const vector_length_t cols,
                   const DSP* = nullptr)
{
      using T = typename traits<DA>::Scalar;
      constexpr int nb_lanes = vector_traits<T>::nb_lanes;
      index_t row=0;

      for(; row <= rows-(1<<DSP_UNROLL);row += (1<<DSP_UNROLL))
      {
          index_t col;

          for(col=0; col <= cols-nb_lanes;col += nb_lanes)
          {
              for(int k=0;k<(1<<DSP_UNROLL);k++)
              {
                  v.matrix_store(row+k,col,other.matrix_op(row+k,col));
              }
          }

          for(; col < cols;col += nb_lanes)
          {
             for(int k=0;k<(1<<DSP_UNROLL);k++)
             {
                v(row+k,col) = other(row+k,col);
             }
          }

      }

      for(; row < rows;row ++)
      {
          index_t col;
          for(col=0; col <= cols-nb_lanes;col += nb_lanes)
          {
              v.matrix_store(row,col,other.matrix_op(row,col));
          }

          for(; col < cols;col += nb_lanes)
          {
              v(row,col) = other(row,col);
          }
      }
}

template<typename DA,typename DB,
         typename std::enable_if<has_vector_inst<DA>() &&
                                vector_idx_pair<DA,DB>(),bool>::type = true>
inline DotResult<DA> _dot(const DA& a,
                         const DB& b,
                         const vector_length_t l,
                         const DSP* = nullptr)
{
    using Acc = DotResult<DA>;
    using T = typename traits<DA>::Scalar;
    using Temp = typename vector_traits<T>::temp_accumulator;
    constexpr int nb_lanes = vector_traits<T>::nb_lanes;
    constexpr unsigned int U = DSP_UNROLL;
    index_t i;

    Acc acc = Acc{};
    Temp vacc = vector_traits<T>::temp_acc_zero();

    for(i=0 ; i <= l-(nb_lanes<<U); i += (nb_lanes<<U))
    {
        for(int k=0;k < (1<<U);k++)
        {
           vacc = inner::vmacc(vacc,a.vector_op(i+k*nb_lanes),b.vector_op(i+k*nb_lanes));
        }
    }

    acc = inner::vreduce(vacc);

    for(; i < l ; i++)
    {
       acc = inner::mac(acc , a[i] , b[i]);
    }

    return(acc);
}

template<typename DA,typename DB,
         typename std::enable_if<has_vector_inst<DA>() &&
                                 vector_idx_pair<DA,DB>(),bool>::type = true>
inline void _swap(DA&& a,
                  DB&& b,
                  const vector_length_t l,
                  const DSP* = nullptr)
{
      using Scalar = typename ElementType<DA>::type;
      using Vector = typename vector_traits<Scalar>::vector;

      constexpr int nb_lanes = vector_traits<typename ElementType<DA>::type>::nb_lanes;
      index_t i=0;
      Vector tmpa,tmpb;
    
      for(i=0 ; i <= l-nb_lanes; i += nb_lanes)
      {
        tmpa = a.vector_op(i);
        tmpb = b.vector_op(i);
        b.vector_store(i,tmpa);
        a.vector_store(i,tmpb);
      }

      for(;i<l;i++)
      {
         const auto tmp = a[i];
         a[i] = b[i];
         b[i] = tmp;
      }

}

#undef DSP_UNROLL

#endif
#endif

/*! @} */
