extern "C" {
    extern void col_test();
}

#include "allocator.h"

#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <iostream>

#include <cmsis_tests.h>



#include "dsp/matrix_functions.h"
#include "matrix_utils.h"

template<typename T,int R,int C>
static void test()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

   #if defined(STATIC_TEST)
   PMat<T,R,C> a;
   PVector<T,R> ref;
   #else 
   PMat<T> a(R,C);
   PVector<T> ref(R);
   #endif

   init_array(a,R*C);



   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PVector<T,R> res = copy(a.col(4));
   #else 
   PVector<T> res = copy(a.col(4));
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   for(int i=0;i<R;i++)
   {
      ref[i] = a(i,4);

   }
   STOP_CYCLE_MEASUREMENT;

   if (!validate(res,ref))
   {
      printf("col failed \r\n");
   }

   std::cout << "=====\r\n";
}



template<typename T>
void all_col_test()
{
   const int nb_tails = TailForTests<T>::tail;
   const int nb_loops = TailForTests<T>::loop;

    title<T>("Col test");
    
    test<T,NBVEC_4,5>();
    test<T,NBVEC_8,5>();
    test<T,NBVEC_16,5>();
    test<T,NBVEC_32,5>();

    test<T,1,5>();
    test<T,nb_tails,5>();
    test<T,nb_loops,5>();
    test<T,nb_loops+1,5>();
    test<T,nb_loops+nb_tails,5>();

}

void col_test()
{
#if defined(COL_TEST)
   #if defined(F64_DT)
   all_col_test<double>();
   #endif
   #if defined(F32_DT)
   all_col_test<float>();
   #endif
   #if defined(F16_DT) && !defined(DISABLEFLOAT16)
   all_col_test<float16_t>();
   #endif
   #if defined(Q31_DT)
   all_col_test<Q31>();
   #endif
   #if defined(Q15_DT)
   all_col_test<Q15>();
   #endif
   #if defined(Q7_DT)
   all_col_test<Q7>();
   #endif
#endif
}