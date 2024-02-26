extern "C" {
    extern void matrix_test();
}

#include "allocator.h"

#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>
#include <dsppp/unroll.hpp>

#include <iostream>

#include <cmsis_tests.h>
#include "boost/mp11.hpp"
using namespace boost::mp11;


extern "C" {
#include "dsp/matrix_functions.h"
#include "dsp/matrix_utils.h"
}

template<typename T>
struct MatTestConstant;

template<>
struct MatTestConstant<double>
{
    constexpr static double value = 0.001;
    constexpr static double half = 0.5;
};

template<>
struct MatTestConstant<float32_t>
{
    constexpr static float value = 0.001f;
    constexpr static float half = 0.5f;
};

#if !defined(DISABLEFLOAT16)
template<>
struct MatTestConstant<float16_t>
{
    constexpr static float16_t value = (float16_t)0.001f;
    constexpr static float16_t half = (float16_t)0.5f;

};
#endif

template<>
struct MatTestConstant<Q7>
{
    constexpr static Q7 value = 0.001_q7;
    constexpr static Q7 half = 0.5_q7;
};

template<>
struct MatTestConstant<Q15>
{
    constexpr static Q15 value = 0.001_q15;
    constexpr static Q15 half = 0.5_q15;
};

template<>
struct MatTestConstant<Q31>
{
    constexpr static Q31 value = 0.001_q31;
    constexpr static Q31 half = 0.5_q31;
};


template<typename T>
struct ErrThreshold
{
   constexpr static float abserr = 0;
   constexpr static float relerr = 0;
   constexpr static float abserr_cholesky = 0;
   constexpr static float relerr_cholesky = 0;
   constexpr static float abserr_householder = 0;
   constexpr static float relerr_householder = 0;
   constexpr static float abserr_qr = 0;
   constexpr static float relerr_qr = 0;
   constexpr static float abserr_inv = 0;
   constexpr static float relerr_inv = 0;
};

// Should be more accurate than F32 but right know
// we only check there is no regression compared to f32
template<>
struct ErrThreshold<double>
{
   constexpr static float abserr = ABS_ERROR;
   constexpr static float relerr = REL_ERROR;
   constexpr static float abserr_cholesky = 3e-4;
   constexpr static float relerr_cholesky = 1e-4;

   constexpr static float abserr_householder = ABS_ERROR;
   constexpr static float relerr_householder = REL_ERROR;
   constexpr static float abserr_qr = ABS_ERROR;
   constexpr static float relerr_qr = REL_ERROR;

   constexpr static float abserr_inv = ABS_ERROR;
   constexpr static float relerr_inv = REL_ERROR;
};

template<>
struct ErrThreshold<float32_t>
{
   constexpr static float abserr = ABS_ERROR;
   constexpr static float relerr = REL_ERROR;
   constexpr static float abserr_cholesky = 3e-4;
   constexpr static float relerr_cholesky = 1e-4;

   constexpr static float abserr_householder = ABS_ERROR;
   constexpr static float relerr_householder = REL_ERROR;
   constexpr static float abserr_qr = ABS_ERROR;
   constexpr static float relerr_qr = REL_ERROR;

   constexpr static float abserr_inv = 4.0e-6;
   constexpr static float relerr_inv = 5.0e-6;
};

#if !defined(DISABLEFLOAT16)
template<>
struct ErrThreshold<float16_t>
{
   constexpr static float abserr = ABS_ERROR;
   constexpr static float relerr = REL_ERROR;
   constexpr static float abserr_cholesky = 2e-1;
   constexpr static float relerr_cholesky = 2e-1;

   constexpr static float abserr_householder = 2e-4;
   constexpr static float relerr_householder = 2e-3;
   // 32x32 is not numerically behaving well with
   // the matrix used as input
   constexpr static float abserr_qr = 2.0;
   constexpr static float relerr_qr = 1e-2;

   constexpr static float abserr_inv = 3e-2;
   constexpr static float relerr_inv = 3e-2;
};
#endif

void cmsisdsp_mat_inv(float64_t *amod,
                      float64_t* b, 
                      uint32_t r,uint32_t c)
{
   arm_matrix_instance_f64 src;
   arm_matrix_instance_f64 dst;


   src.numRows = r;
   src.numCols = c;
   src.pData = amod;

   dst.numRows = r;
   dst.numCols = c;
   dst.pData = b;

   arm_status status = arm_mat_inverse_f64(&src,&dst);
   (void)status;
};

void cmsisdsp_mat_inv(float32_t *amod,
                      float32_t* b, 
                      uint32_t r,uint32_t c)
{
   arm_matrix_instance_f32 src;
   arm_matrix_instance_f32 dst;


   src.numRows = r;
   src.numCols = c;
   src.pData = amod;

   dst.numRows = r;
   dst.numCols = c;
   dst.pData = b;

   arm_status status = arm_mat_inverse_f32(&src,&dst);
   (void)status;
};

#if !defined(DISABLEFLOAT16)
void cmsisdsp_mat_inv(float16_t *amod,
                      float16_t* b, 
                      uint32_t r,uint32_t c)
{
   arm_matrix_instance_f16 src;
   arm_matrix_instance_f16 dst;


   src.numRows = r;
   src.numCols = c;
   src.pData = amod;

   dst.numRows = r;
   dst.numCols = c;
   dst.pData = b;

   arm_status status = arm_mat_inverse_f16(&src,&dst);
   (void)status;
};
#endif

const float32_t mat64[64] = {0.395744, 0.623798, 0.885422, 0.95415, 0.310384, 0.257541, 
        0.631426, 0.424491, 0.130945, 0.799959, 0.133693, 0.479455, 
        0.519254, 0.381039, 0.617455, 0.748273, 0.146944, 0.928945, 
        0.430936, 0.508207, 0.829023, 0.358027, 0.999501, 0.851953, 
        0.273895, 0.685898, 0.0436612, 0.295212, 0.467651, 0.0515567, 
        0.21037, 0.607475, 0.570295, 0.281109, 0.979219, 0.0947969, 
        0.319016, 0.398405, 0.349953, 0.710002, 0.431597, 0.447659, 
        0.0747669, 0.057063, 0.165648, 0.773106, 0.135765, 0.709327, 
        0.873836, 0.292361, 0.00202529, 0.392942, 0.520183, 0.0528055, 
        0.797982, 0.613497, 0.509682, 0.0435791, 0.780526, 0.960582, 
        0.535914, 0.216113, 0.134108, 0.225859};

const float32_t mat16[16] = {1.0, 2.0, 3.0, 4.0, 2.0, 4.0, 5.0, 6.0, 
         3.0, 5.0, 9.0, 10.0, 4.0, 6.0, 10.0, 16.0};

const float32_t mat256[256] = {0.97936, 0.498105, 0.452618, 0.299761, 0.688624, 0.247212, \
        0.228337, 0.22905, 0.563815, 0.251998, 0.5238, 0.141223, 0.0980689, \
        0.79112, 0.771182, 0.890995, 0.0256181, 0.0377277, 0.575629, \
        0.648138, 0.926218, 0.803878, 0.620333, 0.325635, 0.587355, 0.041795, \
        0.934271, 0.0690131, 0.0240136, 0.800828, 0.522999, 0.374706, \
        0.266977, 0.208028, 0.112878, 0.0389899, 0.658311, 0.205067, \
        0.244172, 0.0762778, 0.190575, 0.677312, 0.0682093, 0.367328, \
        0.0191464, 0.988968, 0.437477, 0.130622, 0.907823, 0.0116559, \
        0.614526, 0.447443, 0.0126975, 0.995496, 0.947676, 0.659996, \
        0.321547, 0.725415, 0.658426, 0.0243924, 0.0843519, 0.351748, \
        0.974332, 0.673381, 0.375012, 0.719626, 0.721219, 0.766905, \
        0.17065, 0.648905, 0.770983, 0.360008, 0.344226, 0.179633, 0.347905, \
        0.555561, 0.742615, 0.908389, 0.806959, 0.176078, 0.872167, \
        0.321839, 0.098607, 0.954515, 0.627286, 0.235082, 0.746179, 0.163606, \
        0.899323, 0.871471, 0.712448, 0.956971, 0.736687, 0.750702, 0.843348, \
        0.302435, 0.444862, 0.0644597, 0.765519, 0.518397, 0.765541, \
        0.900375, 0.201853, 0.490325, 0.721786, 0.893647, 0.774724, \
        0.0983631, 0.339887, 0.526084, 0.0786152, 0.515697, 0.438801, \
        0.226628, 0.125093, 0.886642, 0.617766, 0.71696, 0.473172, 0.640949, \
        0.67688, 0.676214, 0.453662, 0.345796, 0.608999, 0.904448, 0.0965741, \
        0.00461771, 0.467399, 0.292235, 0.0418646, 0.116632, 0.0766192, \
        0.269051, 0.411649, 0.0538381, 0.973959, 0.667106, 0.301662, \
        0.977206, 0.891751, 0.420267, 0.441334, 0.0896179, 0.249969, \
        0.672614, 0.623966, 0.609733, 0.320772, 0.39723, 0.845196, 0.653877, \
        0.0599186, 0.340188, 0.199787, 0.598104, 0.45664, 0.920485, 0.969439, \
        0.446555, 0.0932837, 0.0247635, 0.747644, 0.438759, 0.639154, \
        0.754049, 0.379433, 0.968655, 0.0452146, 0.208123, 0.252654, \
        0.261898, 0.608665, 0.145211, 0.395368, 0.799111, 0.697823, \
        0.382906, 0.456515, 0.262579, 0.284169, 0.881488, 0.860877, 0.155548, \
        0.537387, 0.804235, 0.311383, 0.183216, 0.677692, 0.829542, 0.406049, \
        0.860392, 0.467668, 0.385633, 0.654692, 0.841125, 0.178406, \
        0.668945, 0.369609, 0.809711, 0.454593, 0.632028, 0.605791, 0.643851, \
        0.787023, 0.285633, 0.832216, 0.30892, 0.303559, 0.704898, 0.61118, \
        0.435547, 0.173678, 0.788689, 0.319511, 0.648378, 0.635417, 0.125127, \
        0.310251, 0.800819, 0.4863, 0.924361, 0.308059, 0.952175, 0.449844, \
        0.215496, 0.257826, 0.556383, 0.259735, 0.197234, 0.0509903, 0.21474, \
        0.145085, 0.41288, 0.876758, 0.096721, 0.228955, 0.0152248, 0.126501, \
        0.28899, 0.336668, 0.580015, 0.932761, 0.989783, 0.667379, \
        0.798751, 0.587173, 0.445902, 0.041448, 0.311878, 0.0332857, \
        0.401984, 0.795049, 0.8222, 0.678648, 0.807558};

template<typename T,int R,int C, template<int> typename A>
void init_mat(Matrix<T,R,C,A> &pDst,std::size_t r,std::size_t c)
{
   const float32_t *p;
   if ((r==4) && (r==c))
   {
      p = mat16;
   }

   if ((r==8) && (r==c))
   {
      p = mat64;
   }

   if ((r==16) && (r==c))
   {
      p = mat256;
   }


   for(std::size_t i=0;i<r*c;i++)
   {
      pDst[i] = T(p[i]);
   }
}

#if !defined(DISABLEFLOAT16)
__STATIC_FORCEINLINE float16_t _abs(float16_t x)
{
   return((float16_t)fabsf((float)x));
};
#endif

__STATIC_FORCEINLINE float32_t _abs(float32_t x)
{
   return(fabsf(x));
};

__STATIC_FORCEINLINE double _abs(double x)
{
   return(fabs(x));
};

template<typename T,
         int NB,
         template<int> typename A,
         typename M>
void _matinv(const Matrix<T,NB,NB,A> &a,M && res)
{

  Matrix<T,NB,NB,TMP_ALLOC> b = a;

  const vector_length_t nb_rows = a.rows();
  const vector_length_t nb_cols = a.columns();


  for(index_t r=0;r < nb_rows ; r++)
  {
     res.row(r) = T{};
     res(r,r) = number_traits<T>::one();
  }


  for(index_t c=0;c < nb_cols ; c++)
  {
     T pivot = b(c,c);
     index_t selectedRow = c;

     
     for(index_t r=c+1;r < nb_rows ; r++)
     {
        T newPivot = b(r,c);
        if (_abs(newPivot)>_abs(pivot))
        {
            pivot = newPivot;
            selectedRow = r;
        }
     }

     if ((pivot!=T{}) && (selectedRow != c))
     {
         swap(b.row(c,c),b.row(selectedRow,c));
         swap(res.row(c),res.row(selectedRow));
     }
     else if (pivot == T{})
     {
        break;
     }

     pivot = number_traits<T>::one() / pivot;

     b.row(c,c) *= pivot;
     res.row(c) *= pivot;

     index_t r=0;

     for(;r < c ; r++)
     {
         const T tmp = b(r,c);
         b.row(r,c) -= b.row(c,c)*tmp;
         res.row(r) -= res.row(c)*tmp;
     }

     for(r=c+1;r < nb_rows ; r++)
     {
         const T tmp = b(r,c);
         b.row(r,c) -= b.row(c,c)*tmp;
         res.row(r) -= res.row(c)*tmp;
     }

  }

  
}

template<typename T,
         int NB,
         template<int> typename A,
         typename std::enable_if<(NB>0),bool>::type = true>
Matrix<T,NB,NB,TMP_ALLOC> matinv(const Matrix<T,NB,NB,A> &a)
{
   Matrix<T,NB,NB,TMP_ALLOC> res;
   _matinv(a,res);
   return(res);
}

template<typename T,
         int NB,
         template<int> typename A,
         typename std::enable_if<(NB<0),bool>::type = true>
Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> matinv(const Matrix<T,NB,NB,A> &a)
{
   Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> res(a.rows(),a.columns());
   return (_matinv(a,res));
   return(res);
}

template<typename T,
         int NB,
         template<int> typename A,
         typename std::enable_if<(NB<0),bool>::type = true>
void matinv(Matrix<T,DYNAMIC,DYNAMIC,TMP_ALLOC> &res, const Matrix<T,NB,NB,A> &a)
{
   (void)_matinv(a,res);
}


template<typename T,int R,int C>
void testinv()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

   #if defined(STATIC_TEST)
   PMat<T,R,C> a;
   #else
   PMat<T> a(R,C);
   #endif

   init_mat(a,R,C);

   #if !defined(STATIC_TEST)
   PMat<T> res(R,C);
   #endif

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PMat<T,R,C> res = matinv(a);
   #else 
   matinv(res,a);
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   PMat<T> amod(a);
   PMat<T> cmsis_res(R,C);

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   cmsisdsp_mat_inv(amod.ptr(),cmsis_res.ptr(),R,C);
   STOP_CYCLE_MEASUREMENT;

   

   if (!validate(res.const_ptr(),cmsis_res.const_ptr(),R*C,
       ErrThreshold<T>::abserr_inv,ErrThreshold<T>::relerr_inv))
   {
      printf("inv failed \r\n");

   }

   std::cout << "=====\r\n";

}

template<typename T,int R,int C>
void testadd()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

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
   #if defined(STATIC_TEST)
   PMat<T,R,C> res = a+b;
   #else
   PMat<T> res = a+b;
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //PrintType<decltype(a+b)>();
   //PrintType<decltype(res)>();
//
   //std::cout << "a: " << IsVector<decltype(a)>::value << "\r\n";
   //std::cout << "b: " << IsVector<decltype(b)>::value << "\r\n";
   //std::cout << "a+b: " << IsVector<decltype(a+b)>::value << "\r\n";
   //std::cout << "res: " << IsVector<decltype(res)>::value << "\r\n";
   //std::cout << "same: " << SameElementType<decltype(res),decltype(a+b)>::value << "\r\n";
//
   //std::cout << "vec inst: " << has_vector_inst<decltype(res)>() << "\r\n";
   //std::cout << "vec index pair: " << vector_idx_pair<decltype(res),decltype(a+b)>() << "\r\n";
   //std::cout << "must use mat idx: " << must_use_matrix_idx_pair<decltype(res),decltype(a+b)>() << "\r\n";

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PMat<T,R,C> cmsis_res;
   #else
   PMat<T> cmsis_res(R,C);
   #endif
   cmsisdsp_mat_add(a.const_ptr(),b.const_ptr(),cmsis_res.ptr(),R,C);
   STOP_CYCLE_MEASUREMENT;

   

   if (!validate(res.const_ptr(),cmsis_res.const_ptr(),R*C,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("add failed \r\n");
   }

   std::cout << "=====\r\n";

}


template<typename T,int R,int C>
void testdiag()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,R> a;
   #else
   PVector<T> a(R);
   #endif
   init_array(a,R);

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PMat<T,R,C> res=PMat<T,R,C>::diagonal(a);
   #else
   PMat<T> res=PMat<T>::diagonal(a);
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   
   const T* ap = a.const_ptr();

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PMat<T,R,C> cmsis_res;
   #else
   PMat<T> cmsis_res(R,C);
   #endif
   T* refp = cmsis_res.ptr();

   UNROLL_LOOP
   for(index_t row=0;row < R; row++)
   {
      UNROLL_LOOP
      for(index_t col=0;col < C; col++)
      {
         if (row != col)
         {
            refp[row*C+col] = T{};
         }
         else
         {
             refp[row*C+col] = ap[row];
         }
      }
   }
   STOP_CYCLE_MEASUREMENT;

   

   if (!validate(res.const_ptr(),cmsis_res.const_ptr(),R*C,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("diag failed \r\n");
   }

   std::cout << "=====\r\n";
}



template<typename T,int R,int C>
void testouter()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

   PVector<T,R> a;
   PVector<T,C> b;
   init_array(a,R);
   init_array(b,C);

   b = b + b;

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   PMat<T,R,C> res = outer(a,b);
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(2);
   #if defined(STATIC_TEST)
   PMat<T,R,C> cmsis_res;
   #else
   PMat<T> cmsis_res(R,C);
   #endif
   CMSISOuter<T>::run(a.const_ptr(),b.const_ptr(),cmsis_res.ptr(),R,C);
   startSectionNB(2);
   STOP_CYCLE_MEASUREMENT;

   //std::cout<<cmsis_res;

   if (!validate(res.const_ptr(),cmsis_res.const_ptr(),R*C,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("outer failed \r\n");
   }

   std::cout << "=====\r\n";

}

template<typename T,int R,int C>
void testview()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

   #if defined(STATIC_TEST)
   PVector<T,R> a;
   #else 
   PVector<T> a(R);
   #endif
   init_array(a,R);

   #if defined(STATIC_TEST)
   PMat<T,R,C> res=PMat<T,R,C>::diagonal(a);
   #else 
   PMat<T> res=PMat<T>::diagonal(a);
   #endif
   //std::cout << res;
   constexpr int subsize = 8;
   constexpr int subpos = 8;
   auto r = res.sub(Slice(subpos,subpos+subsize),Slice(subpos,subpos+subsize));

   #if defined(STATIC_TEST)
   PMat<T,subsize,subsize> resb;
   #else 
   PMat<T> resb(subsize,subsize);
   #endif

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   resb = r+r;
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //std::cout << IsMatrix<decltype(r+r)>::value << "\r\n";

   PMat<T,subsize,subsize> cmsis_res;
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(2);
   DISABLE_LOOP_UNROLL
   for(index_t row=0;row < subsize ; row++)
   {
      DISABLE_LOOP_UNROLL
      for(index_t col=0;col < subsize ; col++)
      {
         cmsis_res(row,col) = r(row,col)+r(row,col);
      }
   }
   startSectionNB(2);
   STOP_CYCLE_MEASUREMENT;

   //std::cout<<resb;
   //std::cout<<cmsis_res;

   if (!validate(resb,cmsis_res,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("sub matrix failed \r\n");
   }

   std::cout << "=====\r\n";
  
}



template<typename T,int R,int C>
void testmatvec()
{

   using STO = typename vector_traits<T>::storage_type;

   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

   #if defined(STATIC_TEST)
   PVector<T,C> a;
   #else
   PVector<T> a(C);
   #endif
   init_array(a,C);

   #if defined(STATIC_TEST)
   PMat<T,R,C> m;
   #else
   PMat<T> m(R,C);
   #endif
   init_array(m,R*C);

   
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PVector<T,R> res = dot(m,a);
   #else
   PVector<T> res = dot(m,a);
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //std::cout << IsMatrix<decltype(r+r)>::value << "\r\n";

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PVector<T,R> cmsis_res;
   #else
   PVector<T> cmsis_res(R);
   #endif
   typename CMSISMatrixType<T>::type S;
   S.numRows = R;
   S.numCols = C;
   S.pData = reinterpret_cast<STO*>(const_cast<T*>(m.ptr()));

  
   startSectionNB(2);
   cmsis_mat_vec_mult(&S, a.const_ptr(), cmsis_res.ptr());
   startSectionNB(2);
   STOP_CYCLE_MEASUREMENT;

   //std::cout << cmsis_res;
   
   if (!validate(res.const_ptr(),cmsis_res.const_ptr(),R,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("matrix times vector failed \r\n");
   }
   std::cout << "=====\r\n";
  
}

template<typename T,int R,int C>
void testcomplexmatvec()
{
   const T scalar = MatTestConstant<T>::half;
   using STO = typename vector_traits<T>::storage_type;

   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

   #if defined(STATIC_TEST)
   PVector<T,C> a;
   PVector<T,C> b;
   #else 
   PVector<T> a(C);
   PVector<T> b(C);
   #endif
   init_array(a,C);
   init_array(b,C);

   #if defined(STATIC_TEST)
   PMat<T,R,C> m;
   #else
   PMat<T> m(R,C);
   #endif
   init_array(m,R*C);

   
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PVector<T,C> tmpv = a + b * scalar;
   PVector<T,R> res = dot(m,tmpv);
   #else
   PVector<T> tmpv = a + b * scalar;
   PVector<T> res = dot(m,tmpv);
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //std::cout << IsMatrix<decltype(r+r)>::value << "\r\n";

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PVector<T,R> cmsis_res;
   PVector<T,C> tmp;
   #else 
   PVector<T> cmsis_res(R);
   PVector<T> tmp(C);
   #endif
   typename CMSISMatrixType<T>::type S;
   S.numRows = R;
   S.numCols = C;
   S.pData = reinterpret_cast<STO*>(const_cast<T*>(m.ptr()));

   
   startSectionNB(2);
     cmsis_complex_mat_vec(&S,
                         a.const_ptr(),
                         b.const_ptr(),
                         scalar,
                         tmp.ptr(),
                         cmsis_res.ptr());
   startSectionNB(2);
   STOP_CYCLE_MEASUREMENT;



   //std::cout << cmsis_res;
   
   if (!validate(res.const_ptr(),cmsis_res.const_ptr(),R,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("matrix times vector expression failed \r\n");
   }

   std::cout << "=====\r\n";
  
}


template<typename T,int R, int K,int C>
void testmatmult()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << K << " x " << C << "\r\n";

   using S = typename CMSISMatrixType<T>::scalar;

   #if defined(STATIC_TEST)
   PMat<T,R,K> ma;
   #else
   PMat<T> ma(R,K);
   #endif
   init_array(ma,R*K);

   #if defined(STATIC_TEST)
   PMat<T,K,C> mb;
   #else
   PMat<T> mb(K,C);
   #endif
   init_array(mb,K*C);



   mb += TestConstant<T>::small;

   //std::cout << ma;
   //std::cout << mb;
   

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PMat<T,R,C> res = dot(ma,mb);
   #else
   PMat<T> res = dot(ma,mb);
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //PrintType<decltype(ma)>();
   //PrintType<decltype(mb)>();
   //std::cout << ma;
   //std::cout << mb;
   //std::cout << res;
   

   //std::cout << IsMatrix<decltype(r+r)>::value << "\r\n";

   PMat<T> tmp(C,K);

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PMat<T,R,C> cmsis_res;
   #else
   PMat<T> cmsis_res(R,C);
   #endif


   typename CMSISMatrixType<T>::type SA;
   SA.numRows = R;
   SA.numCols = K;
   SA.pData = reinterpret_cast<S*>(ma.ptr());

   typename CMSISMatrixType<T>::type SB;
   SB.numRows = K;
   SB.numCols = C;
   SB.pData = reinterpret_cast<S*>(mb.ptr());

   typename CMSISMatrixType<T>::type RES;
   RES.numRows = R;
   RES.numCols = C;
   RES.pData = reinterpret_cast<S*>(cmsis_res.ptr());

   
   startSectionNB(2);
   cmsis_mat_mult(&SA, &SB, &RES,reinterpret_cast<S*>(tmp.ptr()));
   startSectionNB(2);
   STOP_CYCLE_MEASUREMENT;

  
   //std::cout << cmsis_res;
   
   if (!validate(res,cmsis_res,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("matrix times matrix expression failed \r\n");
   }

   std::cout << "=====\r\n";
  
}

template<typename T,int R,int K, int C>
void testsubmatmult()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << K << " x " << C << "\r\n";

   using S = typename CMSISMatrixType<T>::scalar;
   constexpr int TOTALA = 4 + 2*K + 2*R + K*R;
   constexpr int TOTALB = 4 + 2*C + 2*K + C*K;

   #if defined(STATIC_TEST)
   PMat<T,R+2,K+2> ma;
   #else
   PMat<T> ma(R+2,K+2);
   #endif
   init_array(ma,TOTALA);

   #if defined(STATIC_TEST)
   PMat<T,K+2,C+2> mb;
   #else
   PMat<T> mb(K+2,C+2);
   #endif
   init_array(mb,TOTALB);



   mb += MatTestConstant<T>::value;

   //std::cout << ma;
   //std::cout << mb;
   
   
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PMat<T,R,C> res(T{});
   #else
   PMat<T> res(R,C,T{});
   #endif
   startSectionNB(1);
   res.sub(Slice(0,R),Slice(0,C)) = copy(dot(ma.sub(Slice(0,R),Slice(0,K)),mb.sub(Slice(0,K),Slice(0,C))));
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //PrintType<decltype(ma)>();
   //PrintType<decltype(mb)>();
   //std::cout << ma;
   //std::cout << mb;
   //std::cout << res;
   

   //std::cout << IsMatrix<decltype(r+r)>::value << "\r\n";

   PMat<T> cmsis_res(R,C);
   PMat<T> cmsis_ma(R,K);
   PMat<T> cmsis_mb(K,C);
   PMat<T> tmp(C,K);

   typename CMSISMatrixType<T>::type SA;
   SA.numRows = R;
   SA.numCols = K;
   SA.pData = reinterpret_cast<S*>(cmsis_ma.ptr());

   typename CMSISMatrixType<T>::type SB;
   SB.numRows = K;
   SB.numCols = C;
   SB.pData = reinterpret_cast<S*>(cmsis_mb.ptr());

   typename CMSISMatrixType<T>::type RES;
   RES.numRows = R;
   RES.numCols = C;
   RES.pData = reinterpret_cast<S*>(cmsis_res.ptr());

  

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(2);
   cmsis_ma = copy(ma.sub(Slice(0,R),Slice(0,K)));
   cmsis_mb = copy(mb.sub(Slice(0,K),Slice(0,C)));
   cmsis_mat_mult(&SA, &SB, &RES,reinterpret_cast<S*>(tmp.ptr()));
   startSectionNB(2);
   STOP_CYCLE_MEASUREMENT;

  
   //std::cout << cmsis_res;
   
   if (!validate(res.sub(Slice(0,R),Slice(0,C)),cmsis_res,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("matrix times matrix expression failed \r\n");
   }


   std::cout << "=====\r\n";
}


template<typename T,int R,int C>
void testmattranspose()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";

   #if defined(STATIC_TEST)
   PMat<T,R,C> ma;
   #else
   PMat<T> ma(R,C);
   #endif
   init_array(ma,R*C);

   
   //PrintType<decltype(ma)>();
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   #if defined(STATIC_TEST)
   PMat<T,C,R> res = ma.transpose();
   #else
   PMat<T> res = ma.transpose();
   #endif
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //std::cout << IsMatrix<decltype(r+r)>::value << "\r\n";

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PMat<T,C,R> cmsis_res;
   #else
   PMat<T> cmsis_res(C,R);
   #endif

   typename CMSISMatrixType<T>::type SA;
   SA.numRows = R;
   SA.numCols = C;
   SA.pData = reinterpret_cast<typename CMSISMatrixType<T>::scalar*>(ma.ptr());

   typename CMSISMatrixType<T>::type RES;
   RES.numRows = C;
   RES.numCols = R;
   RES.pData = reinterpret_cast<typename CMSISMatrixType<T>::scalar*>(cmsis_res.ptr());

   
   startSectionNB(2);
   cmsis_mat_trans(&SA, &RES);
   startSectionNB(2);
   STOP_CYCLE_MEASUREMENT;

   //std::cout << cmsis_res;
   
   if (!validate(res,cmsis_res,
       ErrThreshold<T>::abserr,ErrThreshold<T>::relerr))
   {
      printf("matrix transpose failed \r\n");
   }


   std::cout << "=====\r\n";
}


#if !defined(DISABLEFLOAT16)
static float16_t _gen_sqrt(const float16_t v)
{
   return((float16_t)sqrtf(v));
}
#endif

static float32_t _gen_sqrt(const float32_t v)
{
   return(sqrtf(v));
}

static float64_t _gen_sqrt(const float64_t v)
{
   return(sqrt(v));
}

template<int L,template<int> typename A,
         typename V,typename T>
inline T _householder(Vector<T,L,A> &res,const V&v,const T eps)
{
   T alpha = v[0];
   T tau;
   T beta;
   if (v.length()==1)
   {
      res[0] = T{};
      return(T{});
   }
   T xnorm2 = dot(v.sub(1),v.sub(1));

   //std::cout << xnorm2 << "\r\n";
   if (xnorm2 <= eps)
   {
      tau = T{};
      res = T{};
   }
   else 
   {
      if (alpha<=0)
      {
         beta = _gen_sqrt(alpha*alpha+xnorm2);
      }
      else 
      {
         beta = -_gen_sqrt(alpha*alpha+xnorm2);
      }
      T r = number_traits<T>::one() / (alpha - beta);
      res = v * r;
      tau = (beta - alpha)/beta;
      res[0] = number_traits<T>::one();
   }
   return(tau);
}

template<typename V,typename T,
         typename std::enable_if<
         !IsDynamic<V>::value &&
         SameElementType<V,T>::value &&
         IsVector<V>::value,bool>::type = true>
auto householder(const V&v,const T threshold)
{
  constexpr int NB = StaticLength<V>::value;
  Vector<T,NB,TMP_ALLOC> res;
  T beta = _householder(res,v,threshold);
  return std::tuple<T,Vector<T,NB,TMP_ALLOC>>(beta,res);
}

template<typename V,typename T,
         typename std::enable_if<
         IsDynamic<V>::value &&
         SameElementType<V,T>::value &&
         IsVector<V>::value,bool>::type = true>
auto householder(const V&v,const T threshold)
{
  Vector<T,DYNAMIC,TMP_ALLOC> res(v.length());
  T beta = _householder(res,v,threshold);
  return std::tuple<T,Vector<T,DYNAMIC,TMP_ALLOC>>(beta,res);
}

template<typename V,typename T,typename TMP,
         typename std::enable_if<
         IsDynamic<V>::value &&
         SameElementType<V,T>::value &&
         IsVector<V>::value,bool>::type = true>
auto householder(const V&v,const T threshold,TMP &res)
{
  T beta = _householder(res,v,threshold);
  return beta;
}

template<typename T>
struct HouseholderThreshold;

#if !defined(DISABLEFLOAT16)
template<>
struct HouseholderThreshold<float16_t>
{
   static constexpr float16_t value = DEFAULT_HOUSEHOLDER_THRESHOLD_F16;
};
#endif

template<>
struct HouseholderThreshold<float64_t>
{
   static constexpr float64_t value = DEFAULT_HOUSEHOLDER_THRESHOLD_F64;
};


template<>
struct HouseholderThreshold<float32_t>
{
   static constexpr float32_t value = DEFAULT_HOUSEHOLDER_THRESHOLD_F32;
};


template<typename T,int NB>
static void testHouseholder()
{
   std::cout << "----\r\n" << "N = " << NB << "\r\n";
   #if defined(STATIC_TEST)
   PVector<T,NB> a;
   #else
   PVector<T> a(NB);
   #endif

   cmsis_init_householder(a.ptr(),NB);


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   auto res = householder(a,HouseholderThreshold<T>::value);
   //PVector<T,NB> res;// = a + b;
   //float res_beta=0;
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

  
   

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #if defined(STATIC_TEST)
   PVector<T,NB> ref;
   #else
   PVector<T> ref(NB);
   #endif
   T ref_beta = cmsis_householder(a.const_ptr(),ref.ptr(),NB);
   STOP_CYCLE_MEASUREMENT;

   if (!validate(std::get<1>(res).const_ptr(),ref.const_ptr(),NB,
       ErrThreshold<T>::abserr_householder,ErrThreshold<T>::relerr_householder))
   {
      printf("householder vector failed \r\n");
   }


   if (!validate(std::get<0>(res),ref_beta,
       ErrThreshold<T>::abserr_householder,ErrThreshold<T>::relerr_householder))
   {
      printf("householder beta failed \r\n");
   }
   std::cout << "=====\r\n";
} 

#include "debug_mat.h"

#if 1
// R >= C
template<typename T,int R,int C,template<int> typename A>
auto QR(const Matrix<T,R,C,A>&m,const T eps,bool wantQ)
{
   #if defined(STATIC_TEST)
   Vector<T,C,TMP_ALLOC> tau;
   Matrix<T,R,C,TMP_ALLOC> RM = m;
   Matrix<T,R,R,TMP_ALLOC> Q = Matrix<T,R,R>::identity();


   // Temporaries
   Vector<T,R,TMP_ALLOC> tmpvec;
   Matrix<T,1,R,TMP_ALLOC> tmpmat;
   #else
   Vector<T> tau(m.columns());
   Matrix<T> RM = m;
   Matrix<T> Q = Matrix<T>::identity(m.rows());


   // Temporaries
   Vector<T> tmpvec(m.rows());
   Matrix<T> tmpmat(1,m.rows());
   #endif

   const int NBC = m.columns();
   const int NBR = m.rows();

   
   for(index_t c=0;c<NBC-1;c++)
   {
      auto beta = householder(RM.col(c,c),eps,tmpvec);
      tau[c] = beta;

      MatrixView<T> vt(tmpvec,1,NBR-c);
      dot(tmpmat.sub(0,1,0,NBC-c),vt,RM.sub(c,c));
      
      RM.sub(c,c) = 
        RM.sub(c,c) - beta * outer(tmpvec.sub(0,NBR-c),tmpmat.row(0,0,NBC-c));
     
      // Copy householder reflector 
      // Not valid when c == C-1
      // We don't want to use a  test since CMSIS-DSP is not using
      // one and introducing a test would give worse performance
      RM.col(c,c+1) = copy(tmpvec.sub(1,NBR-c));
     
   }


   auto beta = householder(RM.col(NBC-1,NBC-1),eps,tmpvec);
   tau[NBC-1] = beta;

   MatrixView<T> vt(tmpvec,1,NBR-(NBC-1));
   dot(tmpmat.sub(0,1,0,NBC-(NBC-1)),vt,RM.sub(NBC-1,NBC-1));
      
   RM.sub(NBC-1,NBC-1) = 
        RM.sub(NBC-1,NBC-1) - beta * outer(tmpvec.sub(0,NBR-(NBC-1)),tmpmat.row(0,0,NBC-(NBC-1)));
     

   

   if (wantQ)
   {
      for(index_t c=NBC-1;c>=0;c--)
      {
         tmpvec.sub(1) = copy(RM.col(c,c+1));
         tmpvec[0] = number_traits<T>::one();
   
         MatrixView<T> vt(tmpvec,1,NBR-c);
         dot(tmpmat.sub(0,1,0,NBR-c),vt,Q.sub(c,c));

         Q.sub(c,c) = 
           Q.sub(c,c) - tau[c] * outer(tmpvec.sub(0,NBR-c),tmpmat.row(0,0,NBR-c));

      }
   }

   return std::make_tuple(RM,Q,tau);

}

template<typename T,int R,int C>
static void testQR()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << C << "\r\n";
   #if defined(STATIC_TEST)
   PMat<T,R,C> a;
   #else
   PMat<T> a(R,C);
   #endif

   cmsis_init_qr(a.ptr(),R,C);


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   auto res = QR(a,HouseholderThreshold<T>::value,true);
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

   //std::cout << "next\r\n";

   //std::cout << std::get<0>(res);
   //std::cout << std::get<1>(res);
   //std::cout << std::get<2>(res);

   // For fair comparison, in dynamic mode we must take into
   // account the memory allocations since they are made
   // by the QR algorithms
   #if !defined(STATIC_TEST)
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #endif 

   #if 0 //defined(STATIC_TEST)
   PMat<T,R,C> cmsis_res;
   PMat<T,R,C> cmsis_outRp;
   PMat<T,R,R> cmsis_outQp;
   PVector<T,C> cmsis_tau;
   PVector<T,R> cmsis_tmpa;
   PVector<T,C> cmsis_tmpb;
   #else
   PMat<T> cmsis_res(R,C);
   PMat<T> cmsis_outRp(R,C);
   PMat<T> cmsis_outQp(R,R);
   PVector<T> cmsis_tau(C);
   PVector<T> cmsis_tmpa(R);
   PVector<T> cmsis_tmpb(C);
   #endif

   typename CMSISMatrixType<T>::type RP;
   RP.numRows = R;
   RP.numCols = C;
   RP.pData = cmsis_outRp.ptr();

   typename CMSISMatrixType<T>::type QP;
   QP.numRows = R;
   QP.numCols = R;
   QP.pData = cmsis_outQp.ptr();

   typename CMSISMatrixType<T>::type IN;
   IN.numRows = R;
   IN.numCols = C;
   IN.pData = a.ptr();

   //std::cout << "-------\r\n";

   #if defined(STATIC_TEST)
   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   #endif
   arm_status status=cmsis_qr(&IN,HouseholderThreshold<T>::value,
                                    &RP,&QP,
                                    cmsis_tau.ptr(),
                                    cmsis_tmpa.ptr(),
                                    cmsis_tmpb.ptr());
   (void)status;
   STOP_CYCLE_MEASUREMENT;

   //std::cout << cmsis_outRp;
   //std::cout << cmsis_outQp;
   //std::cout << cmsis_tau;

   if (!validate(std::get<0>(res),cmsis_outRp,
       ErrThreshold<T>::abserr_qr,ErrThreshold<T>::relerr_qr))
   {
      printf("QR Rp matrix failed \r\n");
   }


   if (!validate(std::get<1>(res),cmsis_outQp,
       ErrThreshold<T>::abserr_qr,ErrThreshold<T>::relerr_qr))
   {
      printf("QR Qp matrix failed \r\n");
   }

   if (!validate(std::get<2>(res),cmsis_tau,
       ErrThreshold<T>::abserr_qr,ErrThreshold<T>::relerr_qr))
   {
      printf("QR tau failed \r\n");
   }
   std::cout << "=====\r\n";
} 

#endif 


template<typename T,int R,template<int> typename A>
auto cholesky(const Matrix<T,R,R,A>&a)
{
     // Temporaries
   #if defined(STATIC_TEST)
   Matrix<T,R,R,TMP_ALLOC> g = a;
   Vector<T,R,TMP_ALLOC> tmp;
   #else
   Matrix<T> g = a;
   Vector<T> tmp(a.rows());
   #endif

   const int NBR = a.rows();

   g.col(0,0) = g.col(0,0) * (T)(number_traits<T>::one() / _gen_sqrt(g(0,0)));

   for(int j=1;j<NBR;j++)
   {
      dot(tmp.sub(j),g.sub(j,NBR,0,j) , g.row(j,0,j));
      
      g.col(j,j) = (g.col(j,j) - tmp.sub(j)) * (T)(number_traits<T>::one() / _gen_sqrt(g(j,j)- tmp[j]));
   
   }
   return(g);
}


template<typename T,int R>
static void testCholesky()
{
   std::cout << "----\r\n";
   std::cout << R << " x " << R << "\r\n";
   #if defined(STATIC_TEST)
   PMat<T,R,R> a;
   #else
   PMat<T> a(R,R);
   #endif

   cmsis_init_cholesky(a.ptr(),R,R);

   //std::cout << a;

   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   startSectionNB(1);
   // Not totally equivalent to CMSIS implementation
   // It should be possible to rewrite it to avoid use of
   // temporary buffer like CMSIS-DSP
   auto res = cholesky(a);
   stopSectionNB(1);
   STOP_CYCLE_MEASUREMENT;

  
   //std::cout << res;

   PMat<T,R,R> cmsis_res(T{});

   typename CMSISMatrixType<T>::type OUT;
   OUT.numRows = R;
   OUT.numCols = R;
   OUT.pData = cmsis_res.ptr();


   typename CMSISMatrixType<T>::type IN;
   IN.numRows = R;
   IN.numCols = R;
   IN.pData = a.ptr();

   //std::cout << "-------\r\n";


   INIT_SYSTICK;
   START_CYCLE_MEASUREMENT;
   arm_status status=cmsis_cholesky(&IN,&OUT);
   (void)status;
   STOP_CYCLE_MEASUREMENT;

   //std::cout << cmsis_res;

 
   if (!validateLT(res,cmsis_res,
       ErrThreshold<T>::abserr_cholesky,ErrThreshold<T>::relerr_cholesky))
   {
      printf("cholesky failed \r\n");
   }
   std::cout << "=====\r\n";
}

template<typename TT,typename ...T>
struct TESTINV
{
   static void all()
   {
      testinv<TT,T::value...>();
   }
};

template<typename TT,typename ...T>
struct TESTOUTER
{
   static void all()
   {
      testouter<TT,T::value...>();
   }
};

template<typename TT,typename ...T>
struct TESTMATVEC
{
   static void all()
   {
      testmatvec<TT,T::value...>();
   }
};

template<typename TT,typename ...T>
struct TESTCOMPLEXMATVEC
{
   static void all()
   {
      testcomplexmatvec<TT,T::value...>();
   }
};

template<typename TT,typename ...T>
struct TESTADD
{
   static void all()
   {
      testadd <TT,T::value...>();
   }
};

template<typename TT,typename ...T>
struct TESTMATTRANSPOSE
{
   static void all()
   {
      testmattranspose<TT,T::value...>();
   }
};

template<typename TT,typename ...T>
struct TESTMATMULT
{
   static void all()
   {
      testmatmult<TT,T::value...>();
   }
};

template<typename TT,typename ...T>
struct TESTSUBMATMULT
{
   static void all()
   {
      testsubmatmult<TT,T::value...>();
   }
};


template<typename TT,typename... T>
struct TEST_CASES
{
   static void all()
   {
      (mp_push_front<T,TT>::all(),...);
   }
};

template<template <typename, typename...> typename F,
         typename... A>
using ALL_TESTS = mp_rename<mp_push_front<mp_product<F, A...>,float>,TEST_CASES>;
  

template<typename T>
void matrix_all_test()
{

#if defined(MATRIX_TEST)
   #if !defined(SUBTEST1) && !defined(SUBTEST2)
   const int nb_tails = TailForTests<T>::tail;
   const int nb_loops = TailForTests<T>::loop;
   using UNROLL = mp_rename<mp_list_v<1,2,4,8,9,11>,mp_list>;
   #endif 

   #if defined(SUBTEST8) || defined(SUBTEST14) 
   using UNROLLA = mp_rename<mp_list_v<1>,mp_list>;
   #endif
   #if defined(SUBTEST9) || defined(SUBTEST15) 
   using UNROLLA = mp_rename<mp_list_v<2>,mp_list>;
   #endif
   #if defined(SUBTEST10) || defined(SUBTEST16)
   using UNROLLA = mp_rename<mp_list_v<4>,mp_list>;
   #endif

   #if defined(SUBTEST11) || defined(SUBTEST17)
   using UNROLLA = mp_rename<mp_list_v<8>,mp_list>;
   #endif
   #if defined(SUBTEST12) || defined(SUBTEST18)
   using UNROLLA = mp_rename<mp_list_v<9>,mp_list>;
   #endif
   #if defined(SUBTEST13) || defined(SUBTEST19)
   using UNROLLA = mp_rename<mp_list_v<11>,mp_list>;
   #endif

   #if !defined(SUBTEST1) && !defined(SUBTEST2)
   using VEC = mp_rename<mp_list_v<1,
                                   nb_tails,
                                   nb_loops,
                                   nb_loops+1,
                                   nb_loops+nb_tails>,mp_list>;
   #endif
   

   //using res = mp_rename<mp_push_front<mp_product<TEST, A, B,C>,float>,TEST_CASES>;
   //using res = ALL_TESTS<TEST,A,B,C>;
   //PrintType<test>();


   if constexpr (number_traits<T>::is_float)
   {

#if defined(SUBTEST1)
      
      title<T>("Householder");
      testHouseholder<T,NBVEC_4>();
      testHouseholder<T,NBVEC_16>();
      testHouseholder<T,NBVEC_32>();

      title<T>("QR");
      testQR<T,NBVEC_4,NBVEC_4>();
      testQR<T,NBVEC_16,NBVEC_16>();
      testQR<T,NBVEC_32,NBVEC_32>();

      title<T>("Cholesky");
      testCholesky<T,NBVEC_4>();
      testCholesky<T,NBVEC_16>();
      testCholesky<T,NBVEC_32>();

#endif

#if defined(SUBTEST2)
      title<T>("Matrix inverse");
      testinv<T,NBVEC_4,NBVEC_4>();
      testinv<T,NBVEC_8,NBVEC_8>();
      testinv<T,NBVEC_16,NBVEC_16>();
#endif
      

   }

#if defined(SUBTEST1)
      title<T>("Matrix outer product");
   
      testouter<T,NBVEC_4,NBVEC_4>();
      testouter<T,NBVEC_8,NBVEC_8>();
      testouter<T,NBVEC_16,NBVEC_16>();
#endif

#if defined(SUBTEST3)
      title<T>("Matrix outer product");
      ALL_TESTS<TESTOUTER,UNROLL,VEC>::all();
#endif

      if constexpr (!std::is_same<T,double>::value)
      {
#if defined(SUBTEST1)
          title<T>("Matrix times vector");
    
          testmatvec<T,NBVEC_4 ,NBVEC_4>();
          testmatvec<T,NBVEC_16,NBVEC_16>();
          testmatvec<T,NBVEC_32,NBVEC_32>();
          testmatvec<T,NBVEC_44,NBVEC_44>();
          testmatvec<T,NBVEC_47,NBVEC_47>();
#endif

#if defined(SUBTEST4)
          title<T>("Matrix times vector");
          ALL_TESTS<TESTMATVEC,UNROLL,VEC>::all();
#endif 

#if defined(SUBTEST1)
          title<T>("Matrix times vector expression");
    
          testcomplexmatvec<T,NBVEC_4 ,NBVEC_4>();
          testcomplexmatvec<T,NBVEC_16,NBVEC_16>();
          testcomplexmatvec<T,NBVEC_32,NBVEC_32>();
          testcomplexmatvec<T,NBVEC_44,NBVEC_44>();
          testcomplexmatvec<T,NBVEC_47,NBVEC_47>();
#endif

#if defined(SUBTEST5)
          title<T>("Matrix times vector expression");
          ALL_TESTS<TESTCOMPLEXMATVEC,UNROLL,VEC>::all();
#endif
      }

   if constexpr (!std::is_same<T,Q7>::value && !std::is_same<T,double>::value)
   {
#if defined(SUBTEST1)
      title<T>("Matrix add");
       
      testadd<T,NBVEC_4,NBVEC_4>();
      testadd<T,NBVEC_8,NBVEC_8>();
      testadd<T,NBVEC_16,NBVEC_16>();
#endif

#if defined(SUBTEST6)
      title<T>("Matrix add");
      ALL_TESTS<TESTADD,UNROLL,VEC>::all();
#endif
    }
 
 #if defined(SUBTEST1)
   title<T>("Matrix diag");
   
   testdiag<T,NBVEC_4,NBVEC_4>();
   testdiag<T,NBVEC_8,NBVEC_8>();
   testdiag<T,NBVEC_16,NBVEC_16>();

   title<T>("Matrix submatrix");

   testview<T,NBVEC_16,NBVEC_16>();
#endif

#if defined(SUBTEST1)
   title<T>("Matrix multiply");
   testmatmult<T,NBVEC_4,NBVEC_4,NBVEC_4>();
   testmatmult<T,NBVEC_16,NBVEC_16,NBVEC_16>();
   testmatmult<T,NBVEC_32,NBVEC_32,NBVEC_32>();
#endif



#if defined(SUBTEST1)
   title<T>("Matrix transpose");
   testmattranspose<T,NBVEC_2,NBVEC_2>();
   testmattranspose<T,NBVEC_3,NBVEC_3>();
   testmattranspose<T,NBVEC_4,NBVEC_4>();
   testmattranspose<T,NBVEC_16,NBVEC_16>();
   testmattranspose<T,NBVEC_32,NBVEC_32>();
#endif 

#if defined(SUBTEST7)
   title<T>("Matrix transpose");
   ALL_TESTS<TESTMATTRANSPOSE,UNROLL,VEC>::all();
#endif

#if defined(SUBTEST8) || defined(SUBTEST9)|| defined(SUBTEST10)|| defined(SUBTEST11)|| defined(SUBTEST12)|| defined(SUBTEST13)
   title<T>("Matrix multiply");
   ALL_TESTS<TESTMATMULT,UNROLLA,VEC,UNROLL>::all();
#endif

#if defined(SUBTEST1)
   title<T>("Submatrix multiply");
   testsubmatmult<T,NBVEC_4,NBVEC_4,NBVEC_4>();
   testsubmatmult<T,NBVEC_16,NBVEC_16,NBVEC_16>();
#endif 

#if defined(SUBTEST14) || defined(SUBTEST15) || defined(SUBTEST16)|| defined(SUBTEST17)|| defined(SUBTEST18)|| defined(SUBTEST19)
   title<T>("Submatrix multiply");
   ALL_TESTS<TESTSUBMATMULT,UNROLLA,VEC,UNROLL>::all();
#endif

   //testsubmatmult<T,NBVEC_32>();
#endif

};



void matrix_test()
{
#if defined(MATRIX_TEST)
   #if defined(F64_DT)
   matrix_all_test<double>();
   #endif
   #if defined(F32_DT)
   matrix_all_test<float>();
   #endif
   #if defined(F16_DT) && !defined(DISABLEFLOAT16)
   matrix_all_test<float16_t>();
   #endif
   #if defined(Q31_DT)
   matrix_all_test<Q31>();
   #endif
   #if defined(Q15_DT)
   matrix_all_test<Q15>();
   #endif
   #if defined(Q7_DT)
   matrix_all_test<Q7>();
   #endif
#endif
}