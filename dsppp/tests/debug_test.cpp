extern "C" {
    extern void debug_test();
}

#include "allocator.h"

#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <iostream>

#include <cmsis_tests.h>
#include "dsp/basic_math_functions.h"


using namespace arm_cmsis_dsp;



extern Q15 external_debug(const PVector<Q15,16> &a0,
   const PVector<Q15,16> &a1,
   const PVector<Q15,16> &a2,
   const PVector<Q15,16> &a3,
                              const PVector<Q15,16> &b,
                           int l);

template<typename T,int NB>
static void test()
{

  PrintType<vector_traits<Q15>>();
}

void debug_test()
{
    title<Q15>("Debug test");

    
   
    test<Q15,NBVEC_16>();
    



}