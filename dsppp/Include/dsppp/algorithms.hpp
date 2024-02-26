// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup DSPPP C++ extension
 *  C++ template extension to CMSIS-DSP. It is not yet part of
 *  the pack but the headers can be found on the 
 *  [CMSIS-DSP github](https://github.com/ARM-software/CMSIS-DSP/dsppp/Include)
 *  The principles are described in this @ref dsppp_main "page"
 *  @{
 *  @}
 */


/**
In this file we have kernels that are written in an
architecture independant way (using operators of the library)

*/

namespace arm_cmsis_dsp {

/** \addtogroup ALGO Architecture independent algorithms
 *  \ingroup DSPPP
 *  @{
 */

/*

Matrix transpose 

*/


/**
 * Transpose a matrix.
 *
 * @param dst Destination matrix.
 * @param src Source matrix.
 *
 */
template<typename MA,
         typename MB,
         typename std::enable_if<
         HasMatrixIndexing<MA>::value &&
         HasMatrixIndexing<MB>::value /*&& 
         SameElementType<MA,Q15>::value*/,bool>::type = true>
inline void transposeTo(MA &dst,
                        const MB& src)
{
  _arm_mat_trans(src,dst,CURRENT_ARCH);
}


/*

Init a diagonal matrix (0 outside of diagonal)

*/
template<typename P,int R,
         template<int> typename A,
         typename VB,
typename std::enable_if<IsVector<VB>::value &&
         SameElementType<VB,P>::value,bool>::type = true>
inline void _diagonal(Matrix<P,R,R,A> &v,
                      const VB& other,
                      const vector_length_t rows)
{
  UNROLL_LOOP
  for(index_t r=0;r < rows ; r++)
  {
     v.row(r) = P{};
     v(r,r) = other[r];
  }
}


/*


Fill diagonal of an existing matrix 

*/
template<typename P,int R,
         template<int> typename A,
         typename VB,
typename std::enable_if<IsVector<VB>::value &&
         SameElementType<VB,P>::value,bool>::type = true>
inline void _fill_diagonal(Matrix<P,R,R,A> &v,
                           const VB& other,
                           const vector_length_t rows)
{
  for(index_t r=0;r < rows ; r++)
  {
     v(r,r) = other[r];
  }
}

template<typename P,int R,
         template<int> typename A>
inline void _identity(Matrix<P,R,R,A> &v,
                      const vector_length_t rows)
{
  UNROLL_LOOP
  for(index_t r=0;r < rows ; r++)
  {
     v.row(r) = P{};
     v(r,r) = number_traits<P>::one();
  }
}



template<typename M,
         typename V,
         typename std::enable_if<CompatibleStaticMatVecProduct<M,V>::value,bool>::type = true>
inline typename OutputVector<M,V>::type dot(const M&m,const V&v)
{
   typename OutputVector<M,V>::type res;
   _dot_m_v(res,m,v,CURRENT_ARCH);
   return(res);
}

template<typename M,
         typename V,
         typename std::enable_if<CompatibleDynamicMatVecProduct<M,V>::value,bool>::type = true>
inline typename OutputVector<M,V>::type dot(const M&m,const V&v)
{
   typename OutputVector<M,V>::type res(m.rows());
   _dot_m_v(res,m,v,CURRENT_ARCH);
   return(res);
}

template<typename M,
         typename V,
         typename RES,
         typename std::enable_if<CompatibleDynamicMatVecProduct<M,V>::value,bool>::type = true>
inline void dot(RES && res,const M&m,const V&v)
{
   //typename OutputVector<M,V>::type res(m.rows());
   _dot_m_v(res,m,v,CURRENT_ARCH);
}

template<typename MA,
         typename MB,
         typename std::enable_if<CompatibleStaticMatMatProduct<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_fixed,bool>::type = true>
inline typename OutputMatrix<MA,MB>::type dot(const MA&ma,const MB&mb)
{
   
   typename OutputMatrix<MA,MB>::type res;
   auto BT = mb.transpose();

   //using M = MatMult<typename ElementType<MA>::type,MA,MB,typename OutputMatrix<MA,MB>::type,decltype(BT)>;
   _dot_m_m(ma,mb,res,BT,CURRENT_ARCH);
   return(res);
}

template<typename MA,
         typename MB,
         typename std::enable_if<CompatibleStaticMatMatProduct<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_float,bool>::type = true>
inline typename OutputMatrix<MA,MB>::type dot(const MA&ma,const MB&mb)
{
   
   typename OutputMatrix<MA,MB>::type res;

   //using M = MatMult<typename ElementType<MA>::type,MA,MB,typename OutputMatrix<MA,MB>::type,decltype(BT)>;
   _dot_m_m(ma,mb,res,CURRENT_ARCH);
   return(res);
}

template<typename MA,
         typename MB,
         typename std::enable_if<CompatibleDynamicMatMatProduct<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_fixed,bool>::type = true>
inline typename OutputMatrix<MA,MB>::type dot(const MA&ma,const MB&mb)
{
   typename OutputMatrix<MA,MB>::type res(ma.rows(),mb.columns());
   auto BT = mb.transpose();

   //using M = MatMult<typename ElementType<MA>::type,MA,MB,typename OutputMatrix<MA,MB>::type,decltype(BT)>;
   _dot_m_m(ma,mb,res,BT,CURRENT_ARCH);
   return(res);
}

template<typename MA,
         typename MB,
         typename std::enable_if<CompatibleDynamicMatMatProduct<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_float,bool>::type = true>
inline typename OutputMatrix<MA,MB>::type dot(const MA&ma,const MB&mb)
{
   typename OutputMatrix<MA,MB>::type res(ma.rows(),mb.columns());

   //using M = MatMult<typename ElementType<MA>::type,MA,MB,typename OutputMatrix<MA,MB>::type,decltype(BT)>;
   _dot_m_m(ma,mb,res,CURRENT_ARCH);
   return(res);
}

/*


Get res matrix as argument to avoid memory allocation when
assigning the result to a different type of Matrix (like a Matrix view).

*/
template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<CompatibleDynamicMatMatProduct<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_float,bool>::type = true>
inline void dot(RES &&res,const MA&ma,const MB&mb)
{
   //typename OutputMatrix<MA,MB>::type res(ma.rows(),mb.columns());

   //using M = MatMult<typename ElementType<MA>::type,MA,MB,typename OutputMatrix<MA,MB>::type,decltype(BT)>;
   _dot_m_m(ma,mb,std::forward<RES>(res),CURRENT_ARCH);
}

template<typename MA,
         typename MB,
         typename TMP,
         typename std::enable_if<CompatibleDynamicMatMatProductDynamicStride<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_float,bool>::type = true>
inline typename OutputMatrix<MA,MB>::type dot(const MA&ma,const MB&mb)
{
   typename OutputMatrix<MA,MB>::type res(ma.rows(),mb.columns());

   //using M = MatMult<typename ElementType<MA>::type,MA,MB,typename OutputMatrix<MA,MB>::type,decltype(mbt)>;
   _dot_m_m(ma,mb,res,CURRENT_ARCH);
   return(res);
}

template<typename MA,
         typename MB,
         typename TMP,
         typename std::enable_if<CompatibleDynamicMatMatProductDynamicStride<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_fixed,bool>::type = true>
inline typename OutputMatrix<MA,MB>::type dot(const MA&ma,const MB&mb,const TMP &mbt)
{
   typename OutputMatrix<MA,MB>::type res(ma.rows(),mb.columns());

   //using M = MatMult<typename ElementType<MA>::type,MA,MB,typename OutputMatrix<MA,MB>::type,decltype(mbt)>;
   _dot_m_m(ma,mb,res,mbt,CURRENT_ARCH);
   return(res);
}



template<typename P>
Matrix<P,DYNAMIC,DYNAMIC,TMP_ALLOC> mk_identity(const vector_length_t l)
{
       Matrix<P,DYNAMIC,DYNAMIC,TMP_ALLOC> res(l,l);
       _identity(res,l);
       return(res);
};


template<typename P,int L>
Matrix<P,L,L,TMP_ALLOC> mk_identity()
{
       Matrix<P,L,L,TMP_ALLOC> res;
       _identity(res,L);
       return(res);
};

/*! @} */

}
