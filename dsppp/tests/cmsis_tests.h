#pragma once


extern "C" {
#include "arm_math_types.h"
#include "arm_math_types_f16.h"
#include "dsp/filtering_functions.h"
#include "dsp/matrix_functions.h"
#include "dsp/matrix_functions_f16.h"

}

template<typename T>
struct NameOfType;

template<typename T>
struct TailForTests;

template<>
struct NameOfType<float64_t>
{
    constexpr static const char* v="float64_t";
    constexpr static const char* xls="f64";
};

template<>
struct NameOfType<float32_t>
{
    constexpr static const char* v="float32_t";
    constexpr static const char* xls="f32";

};

#if !defined(DISABLEFLOAT16)
template<>
struct NameOfType<float16_t>
{
    constexpr static const char* v="float16_t";
    constexpr static const char* xls="f16";

};
#endif

template<>
struct NameOfType<Q31>
{
    constexpr static const char* v="q31";
    constexpr static const char* xls="q31";

};

template<>
struct NameOfType<Q15>
{
    constexpr static const char* v="q15";
    constexpr static const char* xls="q15";

};

template<>
struct NameOfType<Q7>
{
    constexpr static const char* v="q7";
    constexpr static const char* xls="q7";

};

template<>
struct TailForTests<double>
{
    constexpr static const int tail = 1;
    constexpr static const int loop = 2;

};

template<>
struct TailForTests<float>
{
    constexpr static const int tail = 3;
    constexpr static const int loop = 2*4;
};

#if !defined(DISABLEFLOAT16)
template<>
struct TailForTests<float16_t>
{
    constexpr static const int tail = 7;
    constexpr static const int loop = 2*8;

};
#endif

template<>
struct TailForTests<Q31>
{
    constexpr static const int tail = 3;
    constexpr static const int loop = 2*4;
};

template<>
struct TailForTests<Q15>
{
    constexpr static const int tail = 7;
    constexpr static const int loop = 2*8;
};

template<>
struct TailForTests<Q7>
{
    constexpr static const int tail = 15;
    constexpr static const int loop = 2*16;
};

#include "common_tests.h"

#if !defined(DISABLEFLOAT16)
extern void cmsisdsp_add(const float16_t* a, 
              const float16_t* b, 
                    float16_t* c, 
              uint32_t l);
#endif

extern void cmsisdsp_add(const float64_t* a, 
              const float64_t* b, 
                    float64_t* c, 
              uint32_t l);

extern void cmsisdsp_add(const float32_t* a, 
              const float32_t* b, 
                    float32_t* c, 
              uint32_t l);

extern void cmsisdsp_add(const Q31* a, 
              const Q31* b, 
                    Q31* c, 
              uint32_t l);

extern void cmsisdsp_add(const Q15* a, 
              const Q15* b, 
                    Q15* c, 
              uint32_t l);

extern void cmsisdsp_add(const Q7* a, 
              const Q7* b, 
                    Q7* c, 
              uint32_t l);

extern void cmsisdsp_mat_add(const float32_t* a, 
              const float32_t* b, 
                    float32_t* c, 
              uint32_t row,uint32_t col);

#if !defined(DISABLEFLOAT16)
extern void cmsisdsp_mat_add(const float16_t* a, 
              const float16_t* b, 
                    float16_t* c, 
              uint32_t row,uint32_t col);
#endif 

extern void cmsisdsp_mat_add(const Q31* a, 
              const Q31* b, 
                    Q31* c, 
              uint32_t row,uint32_t col);

extern void cmsisdsp_mat_add(const Q15* a, 
              const Q15* b, 
                    Q15* c, 
              uint32_t row,uint32_t col);

extern void cmsisdsp_mat_add(const Q7* a, 
              const Q7* b, 
                    Q7* c, 
             uint32_t row,uint32_t col);

#if !defined(DISABLEFLOAT16)
extern void cmsisdsp_dot(const float16_t* a, 
              const float16_t* b, 
                    float16_t &c, 
              uint32_t l);
#endif

extern void cmsisdsp_dot(const float64_t* a, 
              const float64_t* b, 
                    float64_t &c, 
              uint32_t l);

extern void cmsisdsp_dot(const float32_t* a, 
              const float32_t* b, 
                    float32_t &c, 
              uint32_t l);

extern void cmsisdsp_dot(const Q31* a, 
              const Q31* b, 
                    Q<15,48> &c, 
              uint32_t l);

extern void cmsisdsp_dot(const Q15* a, 
              const Q15* b, 
                    Q<33,30> &c, 
              uint32_t l);

extern void cmsisdsp_dot(const Q7* a, 
              const Q7* b, 
                    Q<17,14> &c, 
              uint32_t l);

extern void cmsisdsp_dot_expr(const double* a, 
                              const double* b, 
                              const double* c, 
                              const double* d, 
                              double* tmp1, 
                              double* tmp2, 
                              const double scale,
                              double &r, 
                              uint32_t l);

extern void cmsisdsp_dot_expr(const float32_t* a, 
                              const float32_t* b, 
                              const float32_t* c, 
                              const float32_t* d, 
                              float32_t* tmp1, 
                              float32_t* tmp2, 
                              const float32_t scale,
                              float32_t &r, 
                              uint32_t l);

#if !defined(DISABLEFLOAT16)
extern void cmsisdsp_dot_expr(const float16_t* a, 
                              const float16_t* b, 
                              const float16_t* c, 
                              const float16_t* d, 
                              float16_t* tmp1, 
                              float16_t* tmp2, 
                              const float16_t scale,
                              float16_t &r, 
                              uint32_t l);
#endif

extern void cmsisdsp_dot_expr(const Q7* a, 
                              const Q7* b, 
                              const Q7* c, 
                              const Q7* d, 
                              Q7* tmp1, 
                              Q7* tmp2,
                              const Q7 scale, 
                              Q<17,14> &r, 
                              uint32_t l);

extern void cmsisdsp_dot_expr(const Q15* a, 
                              const Q15* b, 
                              const Q15* c, 
                              const Q15* d, 
                              Q15* tmp1, 
                              Q15* tmp2,
                              const Q15 scale, 
                              Q<33,30> &r, 
                              uint32_t l);

extern void cmsisdsp_dot_expr(const Q31* a, 
                              const Q31* b, 
                              const Q31* c, 
                              const Q31* d, 
                              Q31* tmp1, 
                              Q31* tmp2,
                              const Q31 scale, 
                              Q<15,48> &r, 
                              uint32_t l);

extern void cmsisdsp_fir(const arm_fir_instance_f32 * S,
                         const float32_t * pSrc,
                         float32_t * pDst,
                         uint32_t blockSize);

extern void cmsisdsp_fir(const arm_fir_instance_q7 * S,
                         const Q7 * pSrc,
                         Q7 * pDst,
                         uint32_t blockSize);

extern void cmsisdsp_fir(const arm_fir_instance_q15 * S,
                         const Q15 * pSrc,
                         Q15 * pDst,
                         uint32_t blockSize);

extern void cmsisdsp_fir(const arm_fir_instance_q31 * S,
                         const Q31 * pSrc,
                         Q31 * pDst,
                         uint32_t blockSize);

template<typename T>
struct CMSISOuter {
    static void run(const T *a,
             const T *b,
                   T *res,
                   const uint32_t r,const uint32_t c)
    {
       DISABLE_LOOP_UNROLL
       for(unsigned int row=0; row<r ; row ++ )
       {
           DISABLE_LOOP_UNROLL
           for(unsigned int col=0; col<c ; col ++ )
           {
              res[row*c+col] = a[row]*b[col];
           }
       }
    }
};

#if defined(ARM_MATH_MVEI) || defined(ARM_MATH_MVEF)
extern void _cmsis_outer(const float32_t *a,
                        const float32_t *b,
                        float32_t *res,
                        const uint32_t r,const uint32_t c);

#if !defined(DISABLEFLOAT16)
extern void _cmsis_outer(const float16_t *a,
                        const float16_t *b,
                        float16_t *res,
                        const uint32_t r,const uint32_t c);
#endif

extern void _cmsis_outer(const Q7 *a,
                        const Q7 *b,
                        Q7 *res,
                        const uint32_t r,const uint32_t c);

extern void _cmsis_outer(const Q15 *a,
                        const Q15 *b,
                        Q15 *res,
                        const uint32_t r,const uint32_t c);

extern void _cmsis_outer(const Q31 *a,
                        const Q31 *b,
                        Q31 *res,
                        const uint32_t r,const uint32_t c);
template<>
struct CMSISOuter<float32_t> {
    static void run(const float32_t *a,
             const float32_t *b,
                   float32_t *res,
            const uint32_t r,const uint32_t c)
    {
        _cmsis_outer(a,b,res,r,c);
    }
};

#if !defined(DISABLEFLOAT16)
template<>
struct CMSISOuter<float16_t> {
    static void run(const float16_t *a,
             const float16_t *b,
                   float16_t *res,
            const uint32_t r,const uint32_t c)
    {
        _cmsis_outer(a,b,res,r,c);
    }
};
#endif

template<>
struct CMSISOuter<Q31> {
    static void run(const Q31 *a,
             const Q31 *b,
                   Q31 *res,
            const uint32_t r,const uint32_t c)
    {
        _cmsis_outer(a,b,res,r,c);
    }
};

template<>
struct CMSISOuter<Q15> {
    static void run(const Q15 *a,
             const Q15 *b,
                   Q15 *res,
            const uint32_t r,const uint32_t c)
    {
        _cmsis_outer(a,b,res,r,c);
    }
};

template<>
struct CMSISOuter<Q7> {
    static void run(const Q7 *a,
             const Q7 *b,
                   Q7 *res,
            const uint32_t r,const uint32_t c)
    {
        _cmsis_outer(a,b,res,r,c);
    }
};

#endif 

extern void cmsis_init_householder(double *f,const int nb);
extern void cmsis_init_householder(float32_t *f,const int nb);

#if !defined(DISABLEFLOAT16)
extern void cmsis_init_householder(float16_t *f,const int nb);
#endif

extern void cmsis_init_qr(double *f,const int r,const int c);
extern void cmsis_init_qr(float32_t *f,const int r,const int c);

#if !defined(DISABLEFLOAT16)
extern void cmsis_init_qr(float16_t *f,const int r,const int c);
#endif

extern void cmsis_init_cholesky(double *f,const int r,const int c);
extern void cmsis_init_cholesky(float32_t *f,const int r,const int c);

#if !defined(DISABLEFLOAT16)
extern void cmsis_init_cholesky(float16_t *f,const int r,const int c);
#endif

extern void cmsis_mat_mult(const arm_matrix_instance_f64* a, 
                           const arm_matrix_instance_f64* b, 
                                 arm_matrix_instance_f64 *c,
                                 double *pState);

extern void cmsis_mat_mult(const arm_matrix_instance_f32* a, 
                           const arm_matrix_instance_f32* b, 
                                 arm_matrix_instance_f32 *c,
                                 float32_t *pState);
#if !defined(DISABLEFLOAT16)
extern void cmsis_mat_mult(const arm_matrix_instance_f16* a, 
                           const arm_matrix_instance_f16* b, 
                                 arm_matrix_instance_f16 *c,
                                 float16_t *pState);
#endif

extern void cmsis_mat_mult(const arm_matrix_instance_q7* a, 
                           const arm_matrix_instance_q7* b, 
                                 arm_matrix_instance_q7 *c,
                                 q7_t *pState);

extern void cmsis_mat_mult(const arm_matrix_instance_q15* a, 
                           const arm_matrix_instance_q15* b, 
                                 arm_matrix_instance_q15 *c,
                                 q15_t *pState);

extern void cmsis_mat_mult(const arm_matrix_instance_q31* a, 
                           const arm_matrix_instance_q31* b, 
                                 arm_matrix_instance_q31 *c,
                                 q31_t *pState);

extern void cmsis_mat_trans(const arm_matrix_instance_q7* a, 
                            arm_matrix_instance_q7* b);

extern void cmsis_mat_trans(const arm_matrix_instance_q15* a, 
                            arm_matrix_instance_q15* b);

extern void cmsis_mat_trans(const arm_matrix_instance_q31* a, 
                            arm_matrix_instance_q31* b);

#if !defined(DISABLEFLOAT16)
extern void cmsis_mat_trans(const arm_matrix_instance_f16* a, 
                            arm_matrix_instance_f16* b);
#endif

extern void cmsis_mat_trans(const arm_matrix_instance_f64* a, 
                            arm_matrix_instance_f64* b);

extern void cmsis_mat_trans(const arm_matrix_instance_f32* a, 
                            arm_matrix_instance_f32* b);

extern double cmsis_householder(const double *,double* ,uint32_t);

extern float32_t cmsis_householder(const float32_t *,float32_t* ,uint32_t);

#if !defined(DISABLEFLOAT16)
extern float16_t cmsis_householder(const float16_t *,float16_t* ,uint32_t);
#endif

extern void cmsis_mat_vec_mult(
  const arm_matrix_instance_f64 *pSrcMat, 
  const double *pVec, 
  double *pDst);

extern void cmsis_mat_vec_mult(
  const arm_matrix_instance_f32 *pSrcMat, 
  const float32_t *pVec, 
  float32_t *pDst);

#if !defined(DISABLEFLOAT16)
extern void cmsis_mat_vec_mult(
  const arm_matrix_instance_f16 *pSrcMat, 
  const float16_t *pVec, 
  float16_t *pDst);
#endif

extern void cmsis_mat_vec_mult(
  const arm_matrix_instance_q31 *pSrcMat, 
  const Q31 *pVec, 
  Q31 *pDst);

extern void cmsis_mat_vec_mult(
  const arm_matrix_instance_q15 *pSrcMat, 
  const Q15 *pVec, 
  Q15 *pDst);

extern void cmsis_mat_vec_mult(
  const arm_matrix_instance_q7 *pSrcMat, 
  const Q7 *pVec, 
  Q7 *pDst);

extern arm_status cmsis_qr(
    const arm_matrix_instance_f64 * pSrc,
    const double threshold,
    arm_matrix_instance_f64 * pOutR,
    arm_matrix_instance_f64 * pOutQ,
    double * pOutTau,
    double *pTmpA,
    double *pTmpB
    );

extern arm_status cmsis_qr(
    const arm_matrix_instance_f32 * pSrc,
    const float32_t threshold,
    arm_matrix_instance_f32 * pOutR,
    arm_matrix_instance_f32 * pOutQ,
    float32_t * pOutTau,
    float32_t *pTmpA,
    float32_t *pTmpB
    );

#if !defined(DISABLEFLOAT16)
extern arm_status cmsis_qr(
    const arm_matrix_instance_f16 * pSrc,
    const float16_t threshold,
    arm_matrix_instance_f16 * pOutR,
    arm_matrix_instance_f16 * pOutQ,
    float16_t * pOutTau,
    float16_t *pTmpA,
    float16_t *pTmpB
    );
#endif

extern arm_status cmsis_cholesky(
  const arm_matrix_instance_f64 * src,
  arm_matrix_instance_f64 * dst);

extern arm_status cmsis_cholesky(
  const arm_matrix_instance_f32 * src,
  arm_matrix_instance_f32 * dst);

#if !defined(DISABLEFLOAT16)
extern arm_status cmsis_cholesky(
  const arm_matrix_instance_f16 * src,
  arm_matrix_instance_f16 * dst);
#endif

extern void cmsis_complex_mat_vec(
  const arm_matrix_instance_f64 * src,
  const double * a,
  const double * b,
  const double scalar,
  double * tmp,
  double * dst);

extern void cmsis_complex_mat_vec(
  const arm_matrix_instance_f32 * src,
  const float32_t * a,
  const float32_t * b,
  const float32_t scalar,
  float32_t * tmp,
  float32_t * dst);

#if !defined(DISABLEFLOAT16)
extern void cmsis_complex_mat_vec(
  const arm_matrix_instance_f16 * src,
  const float16_t * a,
  const float16_t * b,
  const float16_t scalar,
  float16_t * tmp,
  float16_t * dst);
#endif

extern void cmsis_complex_mat_vec(
  const arm_matrix_instance_q31 * src,
  const Q31 * a,
  const Q31 * b,
  const Q31 scalar,
  Q31 * tmp,
  Q31 * dst);

extern void cmsis_complex_mat_vec(
  const arm_matrix_instance_q15 * src,
  const Q15 * a,
  const Q15 * b,
  const Q15 scalar,
  Q15 * tmp,
  Q15 * dst);

extern void cmsis_complex_mat_vec(
  const arm_matrix_instance_q7 * src,
  const Q7 * a,
  const Q7 * b,
  const Q7 scalar,
  Q7 * tmp,
  Q7 * dst);

template<typename T>
struct CMSISMatrixType;

template<>
struct CMSISMatrixType<double>
{
   typedef arm_matrix_instance_f64 type;
   typedef double scalar;
};

template<>
struct CMSISMatrixType<float32_t>
{
   typedef arm_matrix_instance_f32 type;
   typedef float32_t scalar;
};

#if !defined(DISABLEFLOAT16)
template<>
struct CMSISMatrixType<float16_t>
{
   typedef arm_matrix_instance_f16 type;
   typedef float16_t scalar;
};
#endif

template<>
struct CMSISMatrixType<Q7>
{
   typedef arm_matrix_instance_q7 type;
   typedef q7_t scalar;

};

template<>
struct CMSISMatrixType<Q15>
{
   typedef arm_matrix_instance_q15 type;
   typedef q15_t scalar;

};

template<>
struct CMSISMatrixType<Q31>
{
   typedef arm_matrix_instance_q31 type;
   typedef q31_t scalar;

};

template<typename T>
struct TestConstant;

template<>
struct TestConstant<double>
{
   constexpr static double v = 0.2;
   constexpr static double small = 0.001;
};

template<>
struct TestConstant<float32_t>
{
   constexpr static float v = 0.2f;
   constexpr static float small = 0.001f;
};

#if !defined(DISABLEFLOAT16)
template<>
struct TestConstant<float16_t>
{
   constexpr static float16_t v = 0.2f;
   constexpr static float16_t small = 0.001f;

};
#endif

template<>
struct TestConstant<Q7>
{
   constexpr static Q7 v = 0.2_q7;
   constexpr static Q7 small = 0.001_q7;
};


template<>
struct TestConstant<Q15>
{
   constexpr static Q15 v = 0.2_q15;
   constexpr static Q15 small = 0.001_q15;
};

template<>
struct TestConstant<Q31>
{
   constexpr static Q31 v = 0.2_q31;
   constexpr static Q31 small = 0.001_q31;
};