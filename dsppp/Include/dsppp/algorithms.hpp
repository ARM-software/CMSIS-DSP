// -*- C++ -*-
/** @file */ 
#pragma once 

#include <cmath>

/** \defgroup DSPPP C++ extension
 *  C++ template extension to CMSIS-DSP. It is not yet part of
 *  the pack but the headers can be found on the 
 *  [CMSIS-DSP github](https://github.com/ARM-software/CMSIS-DSP/tree/main/dsppp/Include)
 *  The principles are described in this @ref dsppp_main "page"
 */


/**
In this file we have kernels that are written in an
architecture independent way (using operators of the library)

*/

namespace arm_cmsis_dsp {

/** \addtogroup ALGO Architecture independent algorithms
 *  \ingroup DSPPP
 *  Algorithms written in an architecture independent way
 */

/** Result of a masked dot product and sum computed in one traversal. */
template<typename T>
struct MaskedDotSum
{
   T dot;
   T sum;
};

/**
 * Mask selecting values whose scaled nearest-even integer lies in a range.
 *
 * The test is inclusive and evaluates
 * `minimum <= nearbyint(value * multiplier) + offset <= maximum`.
 */
template<typename V>
class NearestEvenRangeMask
{
public:
   using Scalar = typename traits<V>::Scalar;
   using Stored = typename VecRef<V>::type;

   NearestEvenRangeMask(const V& values, float multiplier, Scalar offset,
                        Scalar minimum, Scalar maximum)
      : values_(VecRef<V>::ref(values)), multiplier_(multiplier),
        offset_(offset), minimum_(minimum), maximum_(maximum) {}

   bool operator[](index_t i) const
   {
      const float rounded = std::nearbyint(
         static_cast<float>(values_[i]) * multiplier_);
      const float selected = rounded + static_cast<float>(offset_);
      return selected >= static_cast<float>(minimum_) &&
             selected <= static_cast<float>(maximum_);
   }

   vector_length_t length() const { return values_.length(); }

   const Stored& values() const { return values_; }
   float multiplier() const { return multiplier_; }
   Scalar offset() const { return offset_; }
   Scalar minimum() const { return minimum_; }
   Scalar maximum() const { return maximum_; }

private:
   Stored values_;
   float multiplier_;
   Scalar offset_;
   Scalar minimum_;
   Scalar maximum_;
};

template<typename V>
inline auto nearest_even_range_mask(
   const V& values, float multiplier,
   typename traits<V>::Scalar offset, typename traits<V>::Scalar minimum,
   typename traits<V>::Scalar maximum)
{
   return NearestEvenRangeMask<V>(values, multiplier, offset,
                                  minimum, maximum);
}

/** Scale in float, round to nearest-even, offset, and clip. */
template<typename DST,typename SRC>
inline void round_scaled_to_nearest_clipped(
   DST& destination, const SRC& source, float multiplier,
   typename traits<DST>::Scalar offset,
   typename traits<DST>::Scalar minimum,
   typename traits<DST>::Scalar maximum)
{
   _round_scaled_to_nearest_clipped(
      destination, source, multiplier, offset, minimum, maximum,
      destination.length(), CURRENT_ARCH);
}

/** Element-wise nearest-even rounding into an existing vector. */
template<typename DST,typename SRC>
inline void round_to_nearest(DST& destination, const SRC& source)
{
   _round_to_nearest(destination, source, destination.length(), CURRENT_ARCH);
}

/** Nearest-even rounding followed by an offset and inclusive clipping. */
template<typename DST,typename SRC>
inline void round_to_nearest_clipped(
   DST& destination, const SRC& source,
   typename traits<DST>::Scalar offset,
   typename traits<DST>::Scalar minimum,
   typename traits<DST>::Scalar maximum)
{
   _round_to_nearest_clipped(destination, source, offset, minimum, maximum,
                             destination.length(), CURRENT_ARCH);
}

/** Accumulate `source * scale` into destination where mask is true. */
template<typename DST,typename SRC,typename MASK>
inline void masked_scale_add(DST& destination, const SRC& source,
                             const MASK& mask,
                             typename traits<DST>::Scalar scale)
{
   _masked_scale_add(destination, source, mask, scale,
                     destination.length(), CURRENT_ARCH);
}

/** Compute a masked dot product and masked sum of the first operand. */
template<typename A,typename B,typename MASK>
inline auto masked_dot_sum(const A& a, const B& b, const MASK& mask)
{
   return _masked_dot_sum(a, b, mask, a.length(), CURRENT_ARCH);
}

/*

Matrix transpose 

*/



 /** @ingroup ALGO
  *  @brief Transpose a matrix.
  *
  * @tparam MA Any matrix type
  * @tparam MB Any matrix type
  * @param dst Destination matrix.
  * @param src Source matrix.
  *
  */
template<typename MA,
         typename MB,
         typename std::enable_if<
         HasMatrixIndexing<MA>::value &&
         HasMatrixIndexing<MB>::value,bool>::type = true>
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
         compatible_element<VB,P>(),bool>::type = true>
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
         compatible_element<VB,P>(),bool>::type = true>
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


/**
 * @ingroup ALGO
 * @brief Matrix x Vector product.
 *
 * @tparam M Any matrix type
 * @tparam V Any vector type
 * @param m matrix.
 * @param v vector.
 * @return The matrix x vector product
 *
 */
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


 /** @ingroup ALGO
  *  @brief Matrix x Matrix product.
  *
  * @tparam MA Any matrix type
  * @tparam MB Any matrix type
  * @param ma Matrix.
  * @param mb Matrix.
  * @return ma x mb matrix product
  *
  */
template<typename MA,
         typename MB,
         typename std::enable_if<CompatibleStaticMatMatProduct<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_fixed,bool>::type = true>
inline typename OutputMatrix<MA,MB>::type dot(const MA&ma,const MB&mb)
{
   
   typename OutputMatrix<MA,MB>::type res;
   auto BT = mb.transpose();

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

   _dot_m_m(ma,mb,res,CURRENT_ARCH);
   return(res);
}

 /** @ingroup ALGO
  *  @brief Matrix x Matrix product
  *
  * @tparam MA Any matrix type
  * @tparam MB Any matrix type
  * @tparam RES Any matrix type
  * @param res Output matrix. Result of ma x mb is written to this argument
  * @param ma Matrix.
  * @param mb Matrix.
  * 
  * Used in dynamic mode (dimension of matrix not know at build time)
  * to avoid a memory allocation if the result matrix is already available
  * (Enable to reuse the same matrix storage for the result in some algorithms)
  *
  */
template<typename MA,
         typename MB,
         typename RES,
         typename std::enable_if<CompatibleDynamicMatMatProduct<MA,MB>::value &&
                                 number_traits<typename traits<MA>::Scalar>::is_float,bool>::type = true>
inline void dot(RES &&res,const MA&ma,const MB&mb)
{
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

   _dot_m_m(ma,mb,res,mbt,CURRENT_ARCH);
   return(res);
}


 /** @ingroup ALGO
  *  @brief Create identity matrix
  *
  * @tparam P Datatype of matrix elements
  * @param l Dimension of matrix (l x l)
  * @return Identity matrix. It is a dynamic matrix (size not know at build time)
  * 
  */
template<typename P>
Matrix<P,DYNAMIC,DYNAMIC,TMP_ALLOC> mk_identity(const vector_length_t l)
{
       Matrix<P,DYNAMIC,DYNAMIC,TMP_ALLOC> res(l,l);
       _identity(res,l);
       return(res);
};


 /** @ingroup ALGO
  *  @brief Create identity matrix
  *
  * @tparam P Datatype of matrix elements
  * @tparam L Matrix dimension (L x L)
  * @return Identity matrix. It is a static matrix : size known at build time.
  * 
  */
template<typename P,int L>
Matrix<P,L,L,TMP_ALLOC> mk_identity()
{
       Matrix<P,L,L,TMP_ALLOC> res;
       _identity(res,L);
       return(res);
};

}
