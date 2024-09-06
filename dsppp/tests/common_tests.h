#pragma once


extern "C" {
#include "bench.h"
#include "test.h"
}


#include <type_traits>
#include "allocator.h"

using namespace arm_cmsis_dsp;

#define REL_ERROR (2.0e-6)
#define ABS_ERROR (1.0e-6)

template<typename T>
struct Display
{
    using type = T;
};

#if !defined(DISABLEFLOAT16)
template<>
struct Display<float16_t>
{
    using type = float;
};

template<>
struct Display<const float16_t>
{
    using type = const float;
};
#endif

static float myabs(double a) {return (float)abs(a);};
static float myabs(const std::complex<double> &a) {return (float)std::abs(a);};
static float myabs(const std::complex<float> &a) {return (float)std::abs(a);};
static float myabs(float a) {return (float)fabs(a);};

template <int M, int F, bool S>
static uint32_t fixabs(Q<M,F,S> qa,Q<M,F,S> qb){return (uint32_t)(llabs((int64_t)qa.v - (int64_t)qb.v));};

template <int M, int F, bool S>
static uint32_t fixabs(std::complex<Q<M,F,S>> qa,
                       std::complex<Q<M,F,S>> qb)
{
   uint32_t dr,di;
   dr = fixabs(qa.real(),qb.real());
   di = fixabs(qa.imag(),qb.imag());
   return(di > dr ? di : dr);
};

/*
static int myabs(Q31 a) 
{
   return _abs(a).v;
};

static int myabs(Q15 a) 
{
   return _abs(a).v;
};

static int myabs(Q7 a) 
{
   return _abs(a).v;
};

static int myabs(std::complex<Q31> a) 
{
   int r = _abs(a.real()).v+_abs(a.imag()).v;
   return int(sqrt(1.0*r));
};

static int myabs(std::complex<Q15> a) 
{
   int r = _abs(a.real()).v+_abs(a.imag()).v;
   return int(sqrt(1.0*r));
};

static int myabs(std::complex<Q7> a) 
{
   int r = _abs(a.real()).v+_abs(a.imag()).v;
   return int(sqrt(1.0*r));
};
*/

#if !defined(DISABLEFLOAT16)
static float myabs(const std::complex<float16_t>& a) {return (float)std::abs(a);};
static float myabs(float16_t a) {return (float)fabs((float)a);};
#endif

template<typename T>
float error(const T a,const T b) {
   using C = typename number_traits<T>::compute_type;

   return myabs((T)((C)a-(C)b));
};

#define ERROR(A,B,AE,RE) (error((A),(B)) > (float)(AE) + (float)(RE) * myabs((B)))
#define ERRVAL(VAL,REF,AE,RE) \
   std::cout << "Error = " << error((VAL),(REF)) << "\r\n"; \
   std::cout << "compared to error " << ((float)(AE) + (float)(RE) * myabs((REF))) << "\r\n";

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

template<int L,
         template<int> typename A>
void init_array(Vector<std::complex<double>,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = std::complex<double>(i,0.5*i);
   }
}

template<int L,
         template<int> typename A>
void init_array(Vector<std::complex<float>,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = std::complex<float>(i,0.5f*i);
   }
}


#if !defined(DISABLEFLOAT16)
template<int L,
         template<int> typename A>
void init_array(Vector<std::complex<float16_t>,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      // Scaled down to prevent saturations in the tests
      pDst[i] = std::complex<float16_t>(i*0.001,i*0.001*0.5);
   }
}

template<int L,
         template<int> typename A>
void init_array(Vector<float16_t,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      // Scaled down to prevent saturations in the tests
      pDst[i] = (float16_t)(i*0.001);
   }
}
#endif

template<int L,
         template<int> typename A>
void init_array(Vector<std::complex<Q31>,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = std::complex<Q31>(Q31(0x1ff*i),Q31((0x1ff*i)>>1));
   }
}

template<int L,
         template<int> typename A>
void init_array(Vector<Q31,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = Q31(0x1ff*i);
   }
}

template<int L,
         template<int> typename A>
void init_array(Vector<std::complex<Q15>,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = std::complex<Q15>(Q15(i),Q15((i)>>1));
   }
}

template<int L,
         template<int> typename A>
void init_array(Vector<Q15,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = Q15(i);
   }
}

template<int L,
         template<int> typename A>
void init_array(Vector<std::complex<Q7>,L,A> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = std::complex<Q7>(Q7(i),Q7(i>>1));
   }
}

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
bool validate(const T a, 
              const T b, 
              std::size_t nb,
              float abser = ABS_ERROR, 
              float reler = REL_ERROR,
              uint32_t fixerr=0)
{
   (void)abser;
   (void)reler;
   using EA = typename Display<typename std::remove_pointer_t<T>>::type;
   for(std::size_t i=0;i<nb;i++)
   {
      if constexpr (number_traits<std::remove_cv_t<std::remove_reference_t<decltype(a[0])>>>::is_float)
      {
         if (ERROR(a[i],b[i],abser,reler) )
         {
            std::cout << "Error at:" << i << " ; res=" << (EA)a[i] << " ; ref=" << (EA)b[i] << "\r\n";
            ERRVAL(a[i],b[i],abser,reler);
            return(false);
         }
      }
      else
      {
         if (fixabs(a[i],b[i])>fixerr)
         {
            std::cout << "Error at:" << i << " ; res=" << (EA)a[i] << " ; ref=" << (EA)b[i] << "\r\n";
            std::cout << "Err =" << fixabs(a[i],b[i]) << "\r\n";
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
bool validate(const TA &a, 
              const TB &b,
              float abser = ABS_ERROR, 
              float reler = REL_ERROR, 
              uint32_t fixerr = 0)
{
   (void)abser;
   (void)reler;
   using EA = typename Display<typename ElementType<TA>::type>::type;
   using EB = typename Display<typename ElementType<TB>::type>::type;
   for(index_t i=0;i<a.length();i++)
   {
      if constexpr (number_traits<typename ElementType<TA>::type>::is_float)
      {
         if (ERROR(a[i],b[i],abser,reler) )
         {
            std::cout << "Error at:" << i << " ; res=" << (EA)a[i] << " ; ref=" << (EB)b[i] << "\r\n";
            ERRVAL(a[i],b[i],abser,reler);
            return(false);
         }
      }
      else 
      {
         if (fixabs(a[i],b[i])>fixerr)
         {
            std::cout << "Error at:" << i << " ; res=" << (EA)a[i] << " ; ref=" << (EB)b[i] << "\r\n";
            std::cout << "Err =" << fixabs(a[i],b[i]) << "\r\n";
            return(false);
         }
      }
   }
   return(true);
}


template<typename T,
 typename std::enable_if<!std::is_pointer<T>::value
 && !IsVector<T>::value && !HasMatrixIndexing<T>::value,bool>::type = true>
bool validate(const T a, 
              const T b,
              float abser = ABS_ERROR, 
              float reler = REL_ERROR,
              uint32_t fixerr = 0)
{
    (void)abser;
    (void)reler;
    using ET = typename Display<typename ElementType<T>::type>::type;
    if constexpr (number_traits<std::remove_cv_t<T>>::is_float)
    {
         if (ERROR(a,b,abser,reler))
         {
            std::cout << "Error: res=" << (ET)a << " ; ref=" << (ET)b << "\r\n";
            ERRVAL(a,b,abser,reler);
            return(false);
         }
    }
    else 
    {
        if (fixabs(a,b)>fixerr)
        {
             std::cout << "Error : res=" << (T)a << " ; ref=" << (T)b << "\r\n";
             std::cout << "Err =" << fixabs(a,b) << "\r\n";
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
bool validate(const MA& a, 
              const MB& b,
              float abser = ABS_ERROR, 
              float reler = REL_ERROR)
{
   using EA = typename Display<typename ElementType<MA>::type>::type;
   using EB = typename Display<typename ElementType<MB>::type>::type;
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

            std::cout << "Error at : (" << row << "," << col << ") ; res=" << (EA)a(row,col) << " ; ref=" << (EB)b(row,col) << "\r\n";
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
bool validateLT(const MA& a, 
                const MB& b,
                float abser = ABS_ERROR, 
                float reler = REL_ERROR)
{
   using EA = typename Display<typename ElementType<MA>::type>::type;
   using EB = typename Display<typename ElementType<MB>::type>::type;
   (void)abser;
   (void)reler;
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

            std::cout << "Error at : (" << row << "," << col << ") ; res=" << (EA)a(row,col) << " ; ref=" << (EB)b(row,col) << "\r\n";
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
         number_traits<typename ElementType<MA>::type>::is_fixed ,bool>::type = true>
bool validate(const MA& a, 
              const MB& b,
              uint32_t fixerr = 0)
{
   using EA = typename Display<typename ElementType<MA>::type>::type;
   using EB = typename Display<typename ElementType<MB>::type>::type;
   for(index_t row=0;row < a.rows() ; row++)
   {
      for(index_t col=0;col < a.columns() ; col++)
      {
         if (fixabs(a(row,col),b(row,col))>fixerr)
         {
            std::cout << "Error at : (" << row << "," << col << ") ; res=" << (EA)a(row,col) << " ; ref=" << (EB)b(row,col) << "\r\n";
            std::cout << "Error = " << fixabs(a(row,col) ,b(row,col)) << "\r\n";
            return(false);
         }
      }
   }
   return(true);
}

template<>
bool validate(const float32_t* a, 
              const float32_t* b, 
              std::size_t nb,
              float abser , 
              float reler,
              uint32_t );


extern template
bool validate<>(const float32_t* a, 
                const float32_t* b, 
                std::size_t nb,
                float abser , 
                float reler, 
                uint32_t );





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