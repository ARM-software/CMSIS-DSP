// -*- C++ -*-
/** @file */ 
#pragma once 

#include <cstdint>
#include <iostream>

// For compiler detection
#include "arch.hpp"


#define ARM_COMPUTE_DISABLE_UNROLL
// For loop (not for fusion unrolling functions)
#define MEMORY_POOL_ALIGNMENT 128
//#define MEMORY_ALLOCATION_DEBUG

// TMP_ALLOC must be defined to use the library
// It is generally defined in an external header not
// part of the library.
// By default it is using the malloc allocator

#ifndef TMP_ALLOC
#define TMP_ALLOC malloc_allocator
#endif

#if !defined(GCC_COMPILER)
// clang / AC6
#if defined(ARM_COMPUTE_DISABLE_UNROLL)
#define UNROLL_LOOP _Pragma ("clang loop unroll(disable)")
#else
#define UNROLL_LOOP _Pragma("clang loop unroll_count(4)")
#endif

#define DISABLE_LOOP_UNROLL _Pragma("clang loop unroll(disable)")

#else
// GCC
#define UNROLL_LOOP
#define DISABLE_LOOP_UNROLL
#endif

namespace arm_cmsis_dsp {

/** \addtogroup COMMON Common types and constants
 *  \ingroup DSPPP
 *  @{
 */
    //! Dynamic objects (dimensions only known at runtime)
    constexpr int DYNAMIC = -1;

    //! Dynamic objects (dimensions only known at runtime) but with some constraints (like stride == nb_cols)
    constexpr int CONSTRAINED_DYNAMIC = -2;

    //! index datatype. It must be a signed datatype
    typedef int32_t index_t;
    //! Vector length datatype. Iy must be a signed datatype.
    typedef int32_t vector_length_t;

/*! @} */

/** \addtogroup DEBUG Tools for debugging
 *  \ingroup DSPPP
 *  @{
 */

/**
 * @brief      Prints a textual representation of a type.
 *
 * @tparam     T     The datatype to display
 */
template <typename T>
void PrintType(void)
{
    //T t;
    std::cout << __PRETTY_FUNCTION__ << "\r\n";
};

/*! @} */
}
