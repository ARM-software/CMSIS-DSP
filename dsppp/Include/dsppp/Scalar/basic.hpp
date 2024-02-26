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

template<typename T,typename DST,
typename std::enable_if<IsVector<DST>::value &&
         SameElementType<DST,T>::value,bool>::type = true>
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


template<typename T,typename DST,
typename std::enable_if<must_use_matrix_idx<DST>() &&
         SameElementType<DST,T>::value,bool>::type = true>
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


/*

Evaluation : used when result is a vector

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

template<typename DA,typename DB,
         typename std::enable_if<vector_idx_pair<DA,DB>(),bool>::type = true>
inline DotResult<DA> _dot(const DA& a,
                         const DB& b,
                         const vector_length_t l,
                         const Scalar* = nullptr)
{
    using Acc = DotResult<DA>;
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
