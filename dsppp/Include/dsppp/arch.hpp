// -*- C++ -*-
/** @file */ 
#pragma once 


namespace arm_cmsis_dsp {


/** \addtogroup ARCH Architecture detection
 *  \ingroup DSPPP
 *  @{
 */

/**
 *  Scalar architecture
 */
class Scalar {};

/**
 *  Architecture supporting DSP extensions
 */
class DSP:public Scalar {};

/**
 *  v8.1M Architecture
 */
class Helium:public DSP {};

/**
 *  v8.2M Architecture
 */
class Helium82:public Helium {};

/**
 *  Architecture supporting Neon
 */
class Neon:public Scalar {};

/*! @} */


}

#include "arch_detection.hpp"


#if defined(ARM_MATH_MVEI) || defined(ARM_MATH_MVEF)
#define ARCH Helium82
#elif defined(ARM_MATH_DSP)
#define ARCH DSP 
#elif defined(ARM_MATH_NEON)
#define ARCH Neon
#else 
#define ARCH Scalar
#endif

#define CURRENT_ARCH (ARCH*)nullptr

#if defined(ARM_MATH_MVEI) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_DSP) || defined(ARM_MATH_NEON)
#define HAS_VECTOR
#endif

#if defined(ARM_MATH_MVEI) || defined(ARM_MATH_MVEF) 
#define HAS_PREDICATED_LOOP
#endif

