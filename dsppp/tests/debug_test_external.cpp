#include "allocator.h"

#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <cmsis_tests.h>
#include <iostream>



#if defined(ARM_MATH_MVEI) || defined(ARM_MATH_MVEF)
Q15 external_debug(const PVector<Q15,16> &a0,
    const PVector<Q15,16> &a1,
    const PVector<Q15,16> &a2,
    const PVector<Q15,16> &a3,
                   const PVector<Q15,16> &b,
                    int l)
{
   int nb = l;
   Q<33,30> acc0;
   Q<33,30> acc1;
   Q<33,30> acc2;
   Q<33,30> acc3;
   for(index_t i=0; i<nb; i+=8)
   {
        acc0 = inner::vmacc(acc0,a0.vector_op_tail(i,nb-i),b.vector_op_tail(i,nb-i),inner::vctpq<Q15>::mk(nb-i));
        acc1 = inner::vmacc(acc1,a1.vector_op_tail(i,nb-i),b.vector_op_tail(i,nb-i),inner::vctpq<Q15>::mk(nb-i));
        acc2 = inner::vmacc(acc2,a2.vector_op_tail(i,nb-i),b.vector_op_tail(i,nb-i),inner::vctpq<Q15>::mk(nb-i));
        acc3 = inner::vmacc(acc3,a3.vector_op_tail(i,nb-i),b.vector_op_tail(i,nb-i),inner::vctpq<Q15>::mk(nb-i));
   }
   Q15 r0,r1,r2,r3;

   r0 = inner::from_accumulator(acc0);
   r1 = inner::from_accumulator(acc1);
   r2 = inner::from_accumulator(acc2);
   r3 = inner::from_accumulator(acc3);

   return(r0+r1+r2+r3);
}
#else
Q15 external_debug(const PVector<Q15,16> &a0,
    const PVector<Q15,16> &a1,
    const PVector<Q15,16> &a2,
    const PVector<Q15,16> &a3,
                   const PVector<Q15,16> &b,
                    int l)
{
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
    (void)b;
    (void)l;
    return(a0[0]);
}
#endif