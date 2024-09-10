extern "C" {
    extern void dot_test();
}

#include "allocator.h"

#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include <iostream>

#include <cmsis_tests.h>

#include "dsp/basic_math_functions.h"
#include "dsp/basic_math_functions_f16.h"





template<typename T,typename S,int NB,typename O>
static void complicated_test(const S scale)
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   PVector<T,NB> b;
   PVector<T,NB> c;
   PVector<T,NB> d;

   PVector<T,NB> res;
   #else 
   PVector<T> a(NB);
   PVector<T> b(NB);
   PVector<T> c(NB);
   PVector<T> d(NB);

   PVector<T> res(NB);
   #endif

   
   init_array(a,NB);
   init_array(b,NB);
   init_array(c,NB);
   init_array(d,NB);
    
   O result;
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   if constexpr(IsComplexNumber<T>::value)
   {
      result = dot(scale*(a+b),conjugate(c*d));
   }
   else 
   {
      result = dot(scale*(a+b),c*d);
   }
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;
   
   O ref;
   PVector<T,NB> tmp1;
   PVector<T,NB> tmp2;
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   cmsisdsp_dot_expr(a.const_ptr(),
                     b.const_ptr(),
                     c.const_ptr(),
                     d.const_ptr(),
                     tmp1.ptr(),
                     tmp2.ptr(),
                     scale,
                     ref,NB);
   STOP_CYCLE_MEASUREMENT;

   uint32_t ferr = 0;
   if (std::is_same<S,Q31>::value)
   {
         ferr = 9;
   }

   if constexpr (IsComplexNumber<T>::value)
   {
      if (std::is_same<S,Q31>::value)
      {
         ferr = 58858;
      }
      if (std::is_same<S,Q15>::value)
      {
         ferr = 65308496;
      }
   }

   if (!validate(result,ref,ErrT<T>::abs_error,
                            ErrT<T>::rel_error,
                            ferr))
   {
      printf("dot expr failed \r\n");

   }

   std::cout << "=====\r\n";

}


template<typename T,int NB,typename O>
static void test()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   PVector<T,NB> b;

   PVector<T,NB> res;
   #else
   PVector<T> a(NB);
   PVector<T> b(NB);

   PVector<T> res(NB);
   #endif

   init_array(a,NB);
   init_array(b,NB);

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   O result = dot(a,b);
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;


   O ref;
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   cmsisdsp_dot(a.const_ptr(),b.const_ptr(),ref,NB);
   STOP_CYCLE_MEASUREMENT;

   uint32_t ferr = 0;
   if (std::is_same<T,Q31>::value)
   {
         ferr = 2;
   }

   if (std::is_same<T,std::complex<Q31>>::value)
   {
         ferr = 1020;
   }

   if (std::is_same<T,std::complex<Q15>>::value)
   {
         ferr = 2815476048;
   }



   if (!validate(result,ref,ErrT<T>::abs_error,
                            ErrT<T>::rel_error,
                            ferr))
   {
      printf("dot failed \r\n");

   }

   std::cout << "=====\r\n";

}

template<typename TA,typename TB,int NB,typename O>
static void test_mixed()
{
   using Res = typename MixedRes<TA,TB>::type;
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<TA,NB> a;
   PVector<TB,NB> b;

   PVector<Res,NB> res;
   #else
   PVector<TA> a(NB);
   PVector<TB> b(NB);

   PVector<Res> res(NB);
   #endif

   init_array(a,NB);
   init_array(b,NB);

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   O result = dot(a,b);
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;


   O ref;

   #if defined(STATIC_TEST)
   PVector<Res,NB> acmplx;
   PVector<Res,NB> bcmplx;
   #else
   PVector<Res> acmplx(NB);
   PVector<Res> bcmplx(NB);
   #endif

   

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   if constexpr (IsComplexNumber<TA>::value)
   {
      bcmplx = copy(b);
      cmsisdsp_dot(a.const_ptr(),bcmplx.const_ptr(),ref,NB);
   }
   else 
   {
      acmplx = copy(a);
      cmsisdsp_dot(acmplx.const_ptr(),b.const_ptr(),ref,NB);
   }
   STOP_CYCLE_MEASUREMENT;

   uint32_t ferr = 0;
   if (std::is_same<TA,Q31>::value)
   {
         ferr = 2;
   }

   if (std::is_same<TA,std::complex<Q31>>::value)
   {
         ferr = 2;
   }

   if (std::is_same<TA,std::complex<Q15>>::value 
       || std::is_same<TB,std::complex<Q15>>::value)
   {
         ferr = 2816508241;
   }

   

   if (!validate(result,ref,ErrT<TA>::abs_error,
                            ErrT<TA>::rel_error,
                            ferr))
   {
      printf("dot mixed failed \r\n");

   }

   std::cout << "=====\r\n";

}

template<typename T,int NB,typename O>
static void test_hermitian()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   PVector<T,NB> b;

   PVector<T,NB> res;
   #else
   PVector<T> a(NB);
   PVector<T> b(NB);

   PVector<T> res(NB);
   #endif

   init_array(a,NB);
   init_array(b,NB);

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   O result = dot(a,conjugate(b));
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;


   O ref;
   // b modified by this call
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   cmsisdsp_hermitian(a.const_ptr(),b.ptr(),ref,NB);
   STOP_CYCLE_MEASUREMENT;

   uint32_t ferr = 0;
   if (std::is_same<T,Q31>::value)
   {
         ferr = 2;
   }

   if (std::is_same<T,std::complex<Q31>>::value)
   {
         ferr = 1020;
   }

   if (std::is_same<T,std::complex<Q15>>::value)
   {
         ferr = 3520119457;
   }

   
   if (!validate(result,ref,ErrT<T>::abs_error,
                            ErrT<T>::rel_error,
                            ferr))
   {
      printf("dot failed \r\n");

   }

   std::cout << "=====\r\n";

}


template<typename T>
void all_dot_test()
{

   const int nb_tails = TailForTests<T>::tail;
   const int nb_loops = TailForTests<T>::loop;

    using ACC = typename number_traits<T>::accumulator;
    constexpr auto v = TestConstant<T>::v;

    title<T>("Dot product");

    
    test<T,NBVEC_4,ACC>();
    test<T,NBVEC_8,ACC>();
    test<T,NBVEC_9,ACC>();
    test<T,NBVEC_16,ACC>();
    test<T,NBVEC_32,ACC>();
    test<T,NBVEC_64,ACC>();
    test<T,NBVEC_128,ACC>();
    test<T,NBVEC_256,ACC>();
    test<T,NBVEC_258,ACC>();
    test<T,NBVEC_512,ACC>();
#if defined(ARM_FLOAT16_SUPPORTED)
    if constexpr (!std::is_same<T,std::complex<float16_t>>::value)
    {
       test<T,NBVEC_1024,ACC>();
    }
    if constexpr (!std::is_same<T,std::complex<float16_t>>::value)

    {
       test<T,NBVEC_2048,ACC>();
    }
#else
    if constexpr (!std::is_same<T,double>::value)
    {
       test<T,NBVEC_2048,ACC>();
    }
#endif

    test<T,1,ACC>();
    test<T,nb_tails,ACC>();
    test<T,nb_loops,ACC>();
    test<T,nb_loops+1,ACC>();
    test<T,nb_loops+nb_tails,ACC>();

    if constexpr (IsComplexNumber<T>::value)
    {
      title<T>("Dot mixed product c X r");

      test_mixed<T,typename T::value_type,NBVEC_4,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_8,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_9,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_16,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_32,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_64,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_128,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_256,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_258,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_512,ACC>();
      test_mixed<T,typename T::value_type,NBVEC_1024,ACC>();
      if constexpr (!std::is_same<T,double>::value)
      {
         test_mixed<T,typename T::value_type,NBVEC_2048,ACC>();
      }

      test_mixed<T,typename T::value_type,1,ACC>();
      test_mixed<T,typename T::value_type,nb_tails,ACC>();
      test_mixed<T,typename T::value_type,nb_loops,ACC>();
      test_mixed<T,typename T::value_type,nb_loops+1,ACC>();
      test_mixed<T,typename T::value_type,nb_loops+nb_tails,ACC>();

      title<T>("Dot mixed product r X c");

      test_mixed<typename T::value_type,T,NBVEC_4,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_8,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_9,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_16,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_32,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_64,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_128,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_256,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_258,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_512,ACC>();
      test_mixed<typename T::value_type,T,NBVEC_1024,ACC>();
      if constexpr (!std::is_same<T,double>::value)
      {
         test_mixed<typename T::value_type,T,NBVEC_2048,ACC>();
      }

      test_mixed<typename T::value_type,T,1,ACC>();
      test_mixed<typename T::value_type,T,nb_tails,ACC>();
      test_mixed<typename T::value_type,T,nb_loops,ACC>();
      test_mixed<typename T::value_type,T,nb_loops+1,ACC>();
      test_mixed<typename T::value_type,T,nb_loops+nb_tails,ACC>();

    }

    if constexpr (IsComplexNumber<T>::value)
    {
       title<T>("Hermitian product");
   
       
       test_hermitian<T,NBVEC_4,ACC>();
       test_hermitian<T,NBVEC_8,ACC>();
       test_hermitian<T,NBVEC_9,ACC>();
       test_hermitian<T,NBVEC_16,ACC>();
       test_hermitian<T,NBVEC_32,ACC>();
       test_hermitian<T,NBVEC_64,ACC>();
       test_hermitian<T,NBVEC_128,ACC>();
       test_hermitian<T,NBVEC_256,ACC>();
       test_hermitian<T,NBVEC_258,ACC>();
       test_hermitian<T,NBVEC_512,ACC>();

#if defined(ARM_FLOAT16_SUPPORTED)
       if constexpr (!std::is_same<T,std::complex<float16_t>>::value)
       {
         test_hermitian<T,NBVEC_1024,ACC>();
         test_hermitian<T,NBVEC_2048,ACC>();
       }
#endif
   
       test_hermitian<T,1,ACC>();
       test_hermitian<T,nb_tails,ACC>();
       test_hermitian<T,nb_loops,ACC>();
       test_hermitian<T,nb_loops+1,ACC>();
       test_hermitian<T,nb_loops+nb_tails,ACC>();
     }


    if constexpr (!IsComplexNumber<T>::value)
    {
       title<T>("Dot product with expressions");
   
       
       complicated_test<T,T,NBVEC_4,ACC>(v);
       complicated_test<T,T,NBVEC_8,ACC>(v);
       complicated_test<T,T,NBVEC_9,ACC>(v);
       complicated_test<T,T,NBVEC_32,ACC>(v);
       complicated_test<T,T,NBVEC_64,ACC>(v);
       complicated_test<T,T,NBVEC_128,ACC>(v);
       
       complicated_test<T,T,NBVEC_256,ACC>(v);
       
       complicated_test<T,T,NBVEC_258,ACC>(v);
       complicated_test<T,T,NBVEC_512,ACC>(v);
       complicated_test<T,T,NBVEC_1024,ACC>(v);
       if constexpr (!std::is_same<T,double>::value)
       {
          complicated_test<T,T,NBVEC_2048,ACC>(v);
       }
   
       complicated_test<T,T,1,ACC>(v);
       complicated_test<T,T,nb_tails,ACC>(v);
       complicated_test<T,T,nb_loops,ACC>(v);
       complicated_test<T,T,nb_loops+1,ACC>(v);
       complicated_test<T,T,nb_loops+nb_tails,ACC>(v);
    }
    else 
    {
       using S = typename ComplexNumberType<T>::type;

       constexpr auto v = TestConstant<S>::v;

       title<T>("Dot product with expressions");
   
       
       complicated_test<T,S,NBVEC_4,ACC>(v);
       complicated_test<T,S,NBVEC_8,ACC>(v);
       complicated_test<T,S,NBVEC_9,ACC>(v);
       complicated_test<T,S,NBVEC_32,ACC>(v);
       complicated_test<T,S,NBVEC_64,ACC>(v);
       complicated_test<T,S,NBVEC_128,ACC>(v);
       
       complicated_test<T,S,NBVEC_256,ACC>(v);
       
       complicated_test<T,S,NBVEC_258,ACC>(v);
       complicated_test<T,S,NBVEC_512,ACC>(v);
       complicated_test<T,S,NBVEC_1024,ACC>(v);
    }

    //print_map("Stats",max_stats);

}

void dot_test()
{
#if 0
   constexpr int NB = 2048;
   using T = std::complex<Q15>;
   using S = typename ComplexNumberType<T>::type;
   constexpr auto scale = TestConstant<S>::v;
   using ACC = typename number_traits<T>::accumulator;

   complicated_test<T,S,NB,ACC>(scale);

#else
#if defined(DOT_TEST)

   // No f64 complex dot product in CMSIS-DSP

   #if defined(F64_DT)
   all_dot_test<double>();
   #endif

   #if defined(COMPLEX_F32_DT)
   all_dot_test<std::complex<float>>();
   #endif

   #if defined(F32_DT)
   all_dot_test<float>();
   #endif

   #if defined(COMPLEX_F16_DT) && !defined(DISABLEFLOAT16)
   all_dot_test<std::complex<float16_t>>();
   #endif

   #if defined(F16_DT) && !defined(DISABLEFLOAT16)
   all_dot_test<float16_t>();
   #endif

   #if defined(COMPLEX_Q31_DT)
   all_dot_test<std::complex<Q31>>();
   #endif

   #if defined(Q31_DT)
   all_dot_test<Q31>();
   #endif

   #if defined(COMPLEX_Q15_DT)
   all_dot_test<std::complex<Q15>>();
   #endif

   #if defined(Q15_DT)
   all_dot_test<Q15>();
   #endif

   // No Q7 complex dot product in CMSIS-DSP

   #if defined(Q7_DT)
   all_dot_test<Q7>();
   #endif

#endif
#endif
}
