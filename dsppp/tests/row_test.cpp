extern "C" {
    extern void row_test();
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
   constexpr int NBOUT = C-1;
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";
   std::cout << "NBOUT = " << NBOUT << "\r\n";

   #if defined(STATIC_TEST)
   PMat<T,R,C> a;
   #else
   PMat<T> a(R,C);
   #endif

   init_array(a,R*C);


   INIT_SYSTICK
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PVector<T,NBOUT> res = a.row(0,1) + a.row(1,1);
   #else 
   PVector<T> res = a.row(0,1) + a.row(1,1);
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   PVector<T,NBOUT> da (a.row(0,1));
   PVector<T,NBOUT> db (a.row(1,1));
   PVector<T,NBOUT> ref;

   cmsisdsp_add(da.const_ptr(),db.const_ptr(),ref.ptr(),NBOUT);
   STOP_CYCLE_MEASUREMENT;

   if (!validate(res.const_ptr(),ref.const_ptr(),NBOUT))
   {
      printf("row add failed \r\n");
   }

   std::cout << "=====\r\n";
}

template<typename T,int R,int C>
static void swaptest()
{
   constexpr int NBOUT = C-2;
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";
   std::cout << "NBOUT = " << NBOUT << "\r\n";

   #if defined(STATIC_TEST)
   PMat<T,R,C> a;
   PMat<T,R,C> b;
   #else
   PMat<T> a(R,C);
   PMat<T> b(R,C);
   #endif

   init_array(a,R*C);
   init_array(b,R*C);


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   swap(a.row(0,2) , a.row(1,2));
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   typename CMSISMatrixType<T>::type mat;
   mat.numCols = C;
   mat.numRows = R;
   mat.pData = b.ptr();

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT; 
   SWAP_ROWS_F32(&mat,2,0,1);
   STOP_CYCLE_MEASUREMENT;

   if (!validate(a.const_ptr(),(const float32_t*)mat.pData,R*C))
   {
      printf("row add failed \r\n");
   }

   std::cout << "=====\r\n";


   
}




template<typename T>
void all_row_test()
{
   const int nb_tails = TailForTests<T>::tail;
   const int nb_loops = TailForTests<T>::loop;


    title<T>("Row test");
   
    test<T,2,NBVEC_4>();
    test<T,4,NBVEC_4>();
    test<T,5,NBVEC_4>();
    test<T,9,NBVEC_4>();

    test<T,2,NBVEC_8>();
    test<T,4,NBVEC_8>();
    test<T,5,NBVEC_8>();
    test<T,9,NBVEC_8>();

    test<T,2,NBVEC_16>();
    test<T,4,NBVEC_16>();
    test<T,5,NBVEC_16>();
    test<T,9,NBVEC_16>();

    test<T,2,nb_loops>();
    test<T,4,nb_loops>();
    test<T,5,nb_loops>();
    test<T,9,nb_loops>();

    test<T,2,nb_loops+1>();
    test<T,4,nb_loops+1>();
    test<T,5,nb_loops+1>();
    test<T,9,nb_loops+1>();

    test<T,2,nb_loops+nb_tails>();
    test<T,4,nb_loops+nb_tails>();
    test<T,5,nb_loops+nb_tails>();
    test<T,9,nb_loops+nb_tails>();

    if constexpr (std::is_same<T,float>::value)
    {
       title<T>("Swap test");

       swaptest<T,2,NBVEC_32>();
       swaptest<T,4,NBVEC_32>();
       swaptest<T,5,NBVEC_32>();
       swaptest<T,9,NBVEC_32>();

       swaptest<T,2,nb_loops>();
       swaptest<T,4,nb_loops>();
       swaptest<T,5,nb_loops>();
       swaptest<T,9,nb_loops>();

       swaptest<T,2,nb_loops+1>();
       swaptest<T,4,nb_loops+1>();
       swaptest<T,5,nb_loops+1>();
       swaptest<T,9,nb_loops+1>();

       swaptest<T,2,nb_loops+nb_tails>();
       swaptest<T,4,nb_loops+nb_tails>();
       swaptest<T,5,nb_loops+nb_tails>();
       swaptest<T,9,nb_loops+nb_tails>();
    }
    //print_map("Stats",max_stats);
}

void row_test()
{
#if defined(ROW_TEST)
   #if defined(F64_DT)
   all_row_test<double>();
   #endif
   #if defined(F32_DT)
   all_row_test<float>();
   #endif
   #if defined(F16_DT) && !defined(DISABLEFLOAT16)
   all_row_test<float16_t>();
   #endif
   #if defined(Q31_DT)
   all_row_test<Q31>();
   #endif
   #if defined(Q15_DT)
   all_row_test<Q15>();
   #endif
   #if defined(Q7_DT)
   all_row_test<Q7>();
   #endif
#endif
}
