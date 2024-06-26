#pragma once


extern "C" {
#include "bench.h"
#include "test.h"
}


#include <type_traits>
#include "allocator.h"

using namespace arm_cmsis_dsp;

#define REL_ERROR (1.0e-6)
#define ABS_ERROR (1.0e-6)
#define ERROR(A,B,AE,RE) ((fabs((A) - (B)) > (AE + RE * fabs((B)))))
#define ERRVAL(VAL,REF,AE,RE) \
   std::cout << "Error = " << fabs(VAL-REF) << "\r\n"; \
   std::cout << "compared to " << (AE + RE * abs((REF))) << "\r\n";

/************
 * 
 *  Data types
 * 
 */

#if defined(TESTMODE)
  template<typename P,int L=arm_cmsis_dsp::DYNAMIC>
  using PVector = Vector<P,L,malloc_allocator>;

  template<typename P,int R=arm_cmsis_dsp::DYNAMIC,int C=arm_cmsis_dsp::DYNAMIC>
  using PMat = Matrix<P,R,C,malloc_allocator>;

#else
#if defined(POOL_ALLOCATOR)

  template<typename P,int L=arm_cmsis_dsp::DYNAMIC>
  using PVector = Vector<P,L,pool_allocator>;
  
  template<typename P,int R=arm_cmsis_dsp::DYNAMIC,int C=arm_cmsis_dsp::DYNAMIC>
  using PMat = Matrix<P,R,C,pool_allocator>;

#else

  template<typename P,int L=arm_cmsis_dsp::DYNAMIC>
  using PVector = Vector<P,L,stat_allocator>;

  template<typename P,int R=arm_cmsis_dsp::DYNAMIC,int C=arm_cmsis_dsp::DYNAMIC>
  using PMat = Matrix<P,R,C,stat_allocator>;

#endif
#endif

template<typename P,int stride=1>
using PView = VectorView<P,stride>;

template<typename T,
         int L,
         template<int> typename A>
void init_array(Vector<T,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = T(i);
   }
}

#if !defined(DISABLEFLOAT16)
template<int L,
         template<int> typename A>
void init_array(Vector<float16_t,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      // Scaled down to prevent saturations in the tests
      pDst[i] = (float16_t)i*0.001;
   }
}
#endif

template<typename T>
void init_array(Vector_Base<T> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = T(i);
   }
}


//template<int L,
//         template<int> typename A>
//void init_array(Vector<float16_t,L,A> &pDst,std::size_t nb);


//extern template void init_array<>(Vector_Base<float16_t> &pDst,std::size_t nb);


template<typename T,
 typename std::enable_if<std::is_pointer<T>::value,bool>::type = true>
bool validate(const T a, const T b, std::size_t nb,float abser = ABS_ERROR, float reler = REL_ERROR)
{
   for(std::size_t i=0;i<nb;i++)
   {
      if constexpr (number_traits<std::remove_cv_t<std::remove_reference_t<decltype(a[0])>>>::is_float)
      {
         if (ERROR(a[i],b[i],abser,reler) )
         {
            std::cout << "Error at:" << i << " ; res=" << a[i] << " ; ref=" << b[i] << "\r\n";
            ERRVAL(a[i],b[i],abser,reler);
            return(false);
         }
      }
      else
      {
         if (a[i]!=b[i])
         {
            std::cout << "Error at:" << i << " ; res=" << a[i] << " ; ref=" << b[i] << "\r\n";
            return(false);
         }
      }
   }
   return(true);
}

template<typename TA,typename TB,
 typename std::enable_if<IsVector<TA>::value && 
                        !HasMatrixIndexing<TA>::value &&
                        IsVector<TB>::value && 
                        !HasMatrixIndexing<TB>::value,bool>::type = true>
bool validate(const TA &a, const TB &b,float abser = ABS_ERROR, float reler = REL_ERROR)
{
   for(index_t i=0;i<a.length();i++)
   {
      if constexpr (number_traits<typename ElementType<TA>::type>::is_float)
      {
         if (ERROR(a[i],b[i],abser,reler) )
         {
            std::cout << "Error at:" << i << " ; res=" << a[i] << " ; ref=" << b[i] << "\r\n";
            ERRVAL(a[i],b[i],abser,reler);
            return(false);
         }
      }
      else 
      {
         if (a[i]!=b[i])
         {
            std::cout << "Error at:" << i << " ; res=" << a[i] << " ; ref=" << b[i] << "\r\n";
            return(false);
         }
      }
   }
   return(true);
}


template<typename T,
 typename std::enable_if<!std::is_pointer<T>::value
 && !IsVector<T>::value && !HasMatrixIndexing<T>::value,bool>::type = true>
bool validate(const T a, const T b,float abser = ABS_ERROR, float reler = REL_ERROR)
{
   
    if constexpr (number_traits<std::remove_cv_t<T>>::is_float)
    {
         if (ERROR(a,b,abser,reler))
         {
            std::cout << "Error: res=" << a << " ; ref=" << b << "\r\n";
            ERRVAL(a,b,abser,reler);
            return(false);
         }
    }
    else 
    {
        if (a != b )
        {
             std::cout << "Error : res=" << a << " ; ref=" << b << "\r\n";
             return(false);
        }
    } 
   
   return(true);
}

template<typename MA,typename MB,
         typename std::enable_if<
         HasMatrixIndexing<MA>::value && 
         HasMatrixIndexing<MB>::value &&
         number_traits<typename ElementType<MA>::type>::is_float,bool>::type = true>
bool validate(const MA& a, const MB& b,float abser = ABS_ERROR, float reler = REL_ERROR)
{
   for(index_t row=0;row < a.rows() ; row++)
   {
      for(index_t col=0;col < a.columns() ; col++)
      {
         if (ERROR(a(row,col),b(row,col),abser,reler) )
         {
            //std::cout << fabs(a(row,col)-b(row,col)) << "\r\n";
            //std::cout << REL_ERROR*fabsf(a(row,col)) << "\r\n";
            //std::cout << a(row,col) << "\r\n";
            //std::cout << b(row,col) << "\r\n";

            std::cout << "Error at : (" << row << "," << col << ") ; res=" << a(row,col) << " ; ref=" << b(row,col) << "\r\n";
            ERRVAL(a(row,col),b(row,col),abser,reler);
            return(false);
         }
      }
   }
   return(true);
}

template<typename MA,typename MB,
         typename std::enable_if<
         HasMatrixIndexing<MA>::value && 
         HasMatrixIndexing<MB>::value &&
         number_traits<typename ElementType<MA>::type>::is_float,bool>::type = true>
bool validateLT(const MA& a, const MB& b,float abser = ABS_ERROR, float reler = REL_ERROR)
{
   for(index_t row=0;row < a.rows() ; row++)
   {
      for(index_t col=0;col <= row ; col++)
      {
         if (ERROR(a(row,col),b(row,col),abser,reler) )
         {
            //std::cout << fabs(a(row,col)-b(row,col)) << "\r\n";
            //std::cout << REL_ERROR*fabsf(a(row,col)) << "\r\n";
            //std::cout << a(row,col) << "\r\n";
            //std::cout << b(row,col) << "\r\n";

            std::cout << "Error at : (" << row << "," << col << ") ; res=" << a(row,col) << " ; ref=" << b(row,col) << "\r\n";
            ERRVAL(a(row,col),b(row,col),abser,reler);
            return(false);
         }
      }
   }
   return(true);
}

template<typename MA,typename MB,
         typename std::enable_if<
         HasMatrixIndexing<MA>::value && 
         HasMatrixIndexing<MB>::value &&
         number_traits<typename ElementType<MA>::type>::is_fixed,bool>::type = true>
bool validate(const MA& a, const MB& b,float abser = ABS_ERROR, float reler = REL_ERROR)
{
   (void)abser;
   (void)reler;
   for(index_t row=0;row < a.rows() ; row++)
   {
      for(index_t col=0;col < a.columns() ; col++)
      {
         if (a(row,col).v != b(row,col).v)
         {
            std::cout << "Error at : (" << row << "," << col << ") ; res=" << a(row,col) << " ; ref=" << b(row,col) << "\r\n";
            std::cout << "Error = " << abs(a(row,col).v - b(row,col).v) << "\r\n";
            return(false);
         }
      }
   }
   return(true);
}

template<>
bool validate(const float32_t* a, const float32_t* b, std::size_t nb,float abser , float reler );


extern template
bool validate<>(const float32_t* a, const float32_t* b, std::size_t nb,float abser , float reler );





template<typename T>
void title(const std::string &s)
{
#if !defined(SERIAL_DUMP)
#if defined(STATIC_TEST)
    std::cout<<"\r\n\033[31;1;4m" << s << " " << NameOfType<T>::xls << "\033[0m\r\n";
#else 
    std::cout<<"\r\n\033[31;1;4m" << s << " dynamic " << NameOfType<T>::xls << "\033[0m\r\n";
#endif
#else 
#if defined(STATIC_TEST)
    std::cout << "\r\n" << s << " " << NameOfType<T>::xls << "\r\n";
#else 
    std::cout << "\r\n" << s << " dynamic " << NameOfType<T>::xls << "\r\n";
#endif
#endif
};