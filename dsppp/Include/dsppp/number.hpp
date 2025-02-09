// -*- C++ -*-
/** @file */ 
#pragma once 

#include "fixed_point.hpp"
#include <type_traits>

#include "arm_math_types.h"

#if defined(ARM_FLOAT16_SUPPORTED)
#include "arm_math_types_f16.h"
#endif

#if defined(ARM_MATH_DSP)
#include "DSP/memory.hpp"
#endif

namespace arm_cmsis_dsp {

/** \addtogroup NUMBER Number datatypes
 *  \ingroup DSPPP
 * Number datatypes expressing different properties of the numbers
 * according to the architecture.
 * 
 * Those definitions are used to write architecture independents
 * algorithms.
 *  @{
 */

constexpr uint32_t maskFromShift(const uint32_t shift)
{
     return ((1<<shift)-1);
}



constexpr uint32_t shiftFromValue(const uint32_t val)
{
     return (val == 1 ? 0 : 1 + shiftFromValue(val>>1));
}

/** @brief Properties of a scalar datatype
 * @tparam T datatype
 * 
 * Needs to contain two static bool : is_float and is_fixed
 * 
 * Needs to contain a static function `one` returning the value
 * 1 for this datatype (used to write some datatype generic
 * algorithms)
 */
template<typename T>
struct number_traits;


/**
 * @brief      Vector containing complex values and not just real values
 *             but with same number of lanes than a normal float vector
 *
 * @tparam     T     Datatype of vector
 */
template<typename T>
struct ComplexVector;

/**
 * @brief      Vector of complex but with half lanes compared to real (2 complex)
 *
 * @tparam     T     Data type of vector
 */
template<typename T>
struct HalfComplexVector;


/**
 * @brief      Dual representation for complex
 *
 * @tparam     T     Scalar datatype for complex
 */
template<typename T>
struct Dual;

/*

When vector is true we have a vector datatype
A temporary accumulator datatype and an accumulator datatype.
For most types the temporary and accumulator are the same.
For float, vector instruction mac is doing a mac per lane.
So temporary is a vector and the final accumulator is a float.

*/

/** @brief Properties of a vector datatype linked to a scalar datatype
 *  @tparam T Type of the scalar
 *  @tparam arch Architecture. It is defined by the 
 *               architecture selection code and should never be
 *               set by the user.   
 */
template<typename T,typename arch = ARCH,typename = void>
struct vector_traits {
  typedef T type; //!< Scalar datatype
  typedef T storage_type; //!< Storage type (for instance for Q15 scalar the storage is int16_t)
  static constexpr bool has_vector = false; //!< True if scalar type has a related vector type 
  static constexpr bool is_float = false; //!< True if scalar type is a float (half, float or double)
  static constexpr bool is_fixed = false; //!< True if scalar type is fixed point
  static constexpr int nb_lanes = 1; //! Number of lanes
};

/** @brief Scalar properties of fixed point datatype
 *  @tparam M Mantissa bits (not including sign bit)
 *  @tparam F Fractional bits
 *  @tparam S Signed or unsigned 
 *  @tparam T Storage datatype
 */
template<int M, int F, bool S,typename T>
struct number_traits<Q<M,F,S,T>>
{
   static constexpr bool is_float = false; //!< False because scalar is not a float datatype (half, float, double)
   static constexpr bool is_fixed = true; //!< True because datatype is a fixed point arithmetic one
   
   /** @brief Return 1 for this datatype
    * 
    *  Used for writing datatype generic algorithms
   */
   static constexpr Q<M,F,S,T> one() {return Q<M,F,S,T>::one();};
};


namespace inner {

/** @brief Predicate (only defined for vector architectures)
 *  @tparam T scalar data type
 *  @param v Number of loops
 *  @return Predicate for the given architecture
 */
template<typename T,typename = void>
struct vctpq {
static typename vector_traits<T>::predicate_t mk(uint32_t v);
};

};


/*

vconst 
vconst_tail
vadd
vsub
vmul
vacc


vload1
vstore1

// When predicate
vctpq
vload1_z
vstore1_z

// When predicated loop
vadd_x
vsub_x
vmul_x 
vmacc_p 


*/



// Common to all architectures
#include "num_features/complex_double.hpp"
#include "num_features/double.hpp"

#include "num_features/complex_float.hpp"
#include "num_features/float.hpp"

#include "num_features/complex_half.hpp"
#include "num_features/half.hpp"

#include "num_features/q31.hpp"
#include "num_features/complex_q31.hpp"

#include "num_features/q15.hpp"
#include "num_features/complex_q15.hpp"

#include "num_features/q7.hpp"
#include "num_features/complex_q7.hpp"


// Specific for some architecture
//#include <Scalar/num_features>
#include "DSP/num_features.hpp"
#include "Helium/num_features.hpp"
//#include <Neon/num_features>


#include "num_features/group.hpp"

/*

If there is the need to tune the intrinsics depending on the
Helium variant of the architecture, something like that could be used.
In practice, selection is done at level of of algorithms more than
instructions where it may be simple to just use a #if to use the
right intrinsics when it is available.

*/
#if 0
template<typename T>
__STATIC_FORCEINLINE mve_pred16_t _vctpq(uint32_t v,Helium * = nullptr);

template<>
__STATIC_FORCEINLINE mve_pred16_t _vctpq<float>(uint32_t v,Helium *)
{
     return(vctp32q(v));
};

template<typename T>
__STATIC_FORCEINLINE mve_pred16_t vctpq(uint32_t v)
{
   return(_vctpq<T>(v,CURRENT_ARCH));
}

#endif 

/*! @} */

} // cmsis-dsp namespace