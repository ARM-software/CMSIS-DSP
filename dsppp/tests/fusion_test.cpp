extern "C" {
    extern void fusion_test();
}

#include "allocator.h"

#include <tuple>
#include <array>

#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>
#include <dsppp/unroll.hpp>

#include <iostream>

#include <cmsis_tests.h>

template<typename T,int NB>
static void test()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";

   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   PVector<T,NB> b;
   PVector<T,NB> c;
   #else
   PVector<T> a(NB);
   PVector<T> b(NB);
   PVector<T> c(NB);
   #endif


   init_array(a,NB);
   init_array(b,NB);
   init_array(c,NB);

   #if defined(STATIC_TEST)
   PVector<T,NB> resa;
   PVector<T,NB> resb;
   #else
   PVector<T> resa(NB);
   PVector<T> resb(NB);
   #endif

   
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   results(resa,resb) = Merged{a + b,a + c};
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   PVector<T,NB> refa;
   PVector<T,NB> refb;

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   cmsisdsp_add(a.const_ptr(),b.const_ptr(),refa.ptr(),NB);
   cmsisdsp_add(a.const_ptr(),c.const_ptr(),refb.ptr(),NB);
   STOP_CYCLE_MEASUREMENT;

   if (!validate(resa.const_ptr(),refa.const_ptr(),NB))
   {
      printf("add a failed \r\n");

   }

   if (!validate(resb.const_ptr(),refb.const_ptr(),NB))
   {
      printf("add b failed \r\n");

   }

   std::cout << "=====\r\n";
}


template<typename T,int NB>
static void test2()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   PVector<T,NB> b;
   PVector<T,NB> c;
   #else
   PVector<T> a(NB);
   PVector<T> b(NB);
   PVector<T> c(NB);
   #endif
   using Acc = typename number_traits<T>::accumulator;


   init_array(a,NB);
   init_array(b,NB);
   init_array(c,NB);

   Acc resa,resb,refa,refb;

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(2);
   std::tie(resa,resb) = dot(Merged{expr(a),expr(a)},
                             Merged{expr(b),expr(c)});
   stopSectionNB(2);
   STOP_CYCLE_MEASUREMENT;

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   cmsisdsp_dot(a.const_ptr(),b.const_ptr(),refa,NB);
   cmsisdsp_dot(a.const_ptr(),c.const_ptr(),refb,NB);
   STOP_CYCLE_MEASUREMENT;

   if (!validate(resa,refa))
   {
      printf("dot a failed \r\n");

   }

   if (!validate(resb,refb))
   {
      printf("dot b failed \r\n");

   }

   std::cout << "=====\r\n";


}

template<typename T,int NB>
static void test3()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";

   constexpr int U = 2;
   #if defined(STATIC_TEST)
   PVector<T,NB> a[U];
   PVector<T,NB> b[U];
   #else
   PVector<T> a[U]={PVector<T>(NB),PVector<T>(NB)};
   PVector<T> b[U]={PVector<T>(NB),PVector<T>(NB)};
   #endif

   using Acc = typename number_traits<T>::accumulator;

   for(int i=0;i<U;i++)
   {
      init_array(a[i],NB);
      init_array(b[i],NB);
   }

   std::array<Acc,U> res;
   Acc ref[U];

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(3);
   results(res) = dot(unroll<U>(
                       [&a](index_t k){return expr(a[k]);}),
                      unroll<U>(
                       [&b](index_t k){return expr(b[k]);})
              );
   stopSectionNB(3);
   STOP_CYCLE_MEASUREMENT;

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   for(int i=0;i<U;i++)
   {
      cmsisdsp_dot(a[i].const_ptr(),b[i].const_ptr(),ref[i],NB);
   }
   STOP_CYCLE_MEASUREMENT;

   for(int i=0;i<U;i++)
   {
      if (!validate(res[i],ref[i]))
      {
         printf("dot failed %d \r\n",i);

      }
   }

   std::cout << "=====\r\n";

}

template<typename T>
void all_fusion_test()
{
    
    const int nb_tails = TailForTests<T>::tail;
    const int nb_loops = TailForTests<T>::loop;

    title<T>("Vector Fusion");

    test<T,NBVEC_256>();
    test<T,1>();
    test<T,nb_tails>();
    test<T,nb_loops>();
    test<T,nb_loops+1>();
    test<T,nb_loops+nb_tails>();

    title<T>("Dot Product Fusion");

    test2<T,NBVEC_256>();
    test2<T,1>();
    test2<T,nb_tails>();
    test2<T,nb_loops>();
    test2<T,nb_loops+1>();
    test2<T,nb_loops+nb_tails>();




    title<T>("Unroll Fusion");

    test3<T,NBVEC_256>();
    test3<T,1>();
    test3<T,nb_tails>();
    test3<T,nb_loops>();
    test3<T,nb_loops+1>();
    test3<T,nb_loops+nb_tails>();

}

void fusion_test()
{
   /*

gcc has some issues with this code.
FVP is freezing when trying to run it.
Since those kind of fusion are not really used in the library
(because performance is not good) we can disable those tests
to at least be able to test other parts of the library with gcc.

   */
   #if !defined(GCC_COMPILER)
#if defined(FUSION_TEST)
   #if defined(F64_DT)
   all_fusion_test<double>();
   #endif
   #if defined(F32_DT)
   all_fusion_test<float>();
   #endif
   #if defined(F16_DT) && !defined(DISABLEFLOAT16)
   all_fusion_test<float16_t>();
   #endif
   #if defined(Q31_DT)
   all_fusion_test<Q31>();
   #endif
   #if defined(Q15_DT)
   all_fusion_test<Q15>();
   #endif
   #if defined(Q7_DT)
   all_fusion_test<Q7>();
   #endif
#endif
   #endif

}