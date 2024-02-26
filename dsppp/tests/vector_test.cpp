extern "C" {
    extern void vector_test();
}

#include "allocator.h"


#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <iostream>

#include <cmsis_tests.h>

template<typename T,int NB>
static void test()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   PVector<T,NB> b;
   #else 
   PVector<T> a(NB);
   PVector<T> b(NB);
   #endif

   init_array(a,NB);
   init_array(b,NB);


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PVector<T,NB> res = a + b;
   #else 
   PVector<T> res = copy(a + b);
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

  

   INIT_SYSTICK
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PVector<T,NB> ref;
   #else
   PVector<T> ref(NB);
   #endif
   cmsisdsp_add(a.const_ptr(),b.const_ptr(),ref.ptr(),NB);
   STOP_CYCLE_MEASUREMENT;

   if (!validate(res.const_ptr(),ref.const_ptr(),NB))
   {
      printf("add failed \r\n");
   }

   std::cout << "=====\r\n";
}


template<typename T,int NB>
void test_view()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   PVector<T,NB> b;
   #else 
   PVector<T> a(NB);
   PVector<T> b(NB);
   #endif

   init_array(a,NB);
   init_array(b,NB);

   //std::cout << a;
   //std::cout << "\r\n";

   //std::cout << PVector<T,NB/2>(a.template sub<2>());

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PVector<T,NB/2> res = a.template sub<2>() + b.template sub<2>();
   #else 
   PVector<T> res = a.template sub<2>() + b.template sub<2>();
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

  
   PVector<T,NB/2> ref;
  
   

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   PVector<T,NB/2> da(a.template sub<2>());
   PVector<T,NB/2> db(b.template sub<2>());
   cmsisdsp_add(da.const_ptr(),db.const_ptr(),ref.ptr(),NB/2);
   STOP_CYCLE_MEASUREMENT;

   if (!validate(res.const_ptr(),ref.const_ptr(),NB/2))
   {
      printf("add failed \r\n");
   }
   std::cout << "=====\r\n";
}

template<typename T,int NB>
void test_fill()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PVector<T,NB> res(T(1));
   #else 
   PVector<T> res(NB,T(1));
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

}


template<typename T>
void all_vector_test()
{


   const int nb_tails = TailForTests<T>::tail;
   const int nb_loops = TailForTests<T>::loop;

    title<T>("Vector");

    // For benchmarks
    test<T,NBVEC_4>();
    test<T,NBVEC_8>();
    test<T,NBVEC_9>();
    test<T,NBVEC_16>();
    test<T,NBVEC_32>();
    test<T,NBVEC_64>();
    test<T,NBVEC_128>();
    test<T,NBVEC_256>();
    test<T,NBVEC_258>();
    test<T,NBVEC_512>();
    test<T,NBVEC_1024>();
    test<T,NBVEC_2048>();

    // For tests
    test<T,1>();
    test<T,nb_tails>();
    test<T,nb_loops>();
    test<T,nb_loops+1>();
    test<T,nb_loops+nb_tails>();


    title<T>("Vector View");

    test_view<T,NBVEC_4>();
    test_view<T,NBVEC_8>();
    test_view<T,NBVEC_16>();
    test_view<T,NBVEC_32>();
    test_view<T,NBVEC_64>();

    if constexpr (nb_tails>1)
    {
       test_view<T,nb_tails>();
    }
    test_view<T,nb_loops>();
    test_view<T,nb_loops+1>();
    test_view<T,nb_loops+nb_tails>();


    title<T>("Vector fill");
    test_fill<T,NBVEC_4>();
    test_fill<T,NBVEC_8>();
    test_fill<T,NBVEC_16>();
    test_fill<T,NBVEC_32>();
    test_fill<T,NBVEC_64>();

    test_fill<T,1>();
    test_fill<T,nb_tails>();
    test_fill<T,nb_loops>();
    test_fill<T,nb_loops+1>();
    test_fill<T,nb_loops+nb_tails>();

}

void vector_test()
{
#if defined(VECTOR_TEST)
   #if defined(F64_DT)
   all_vector_test<double>();
   #endif
   #if defined(F32_DT)
   all_vector_test<float>();
   #endif
   #if defined(F16_DT) && !defined(DISABLEFLOAT16)
   all_vector_test<float16_t>();
   #endif
   #if defined(Q31_DT)
   all_vector_test<Q31>();
   #endif
   #if defined(Q15_DT)
   all_vector_test<Q15>();
   #endif
   #if defined(Q7_DT)
   all_vector_test<Q7>();
   #endif
#endif
}
