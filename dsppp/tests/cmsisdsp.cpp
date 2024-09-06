#include "allocator.h"

#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

using namespace arm_cmsis_dsp;


#include "dsp/basic_math_functions.h"
#include "dsp/basic_math_functions_f16.h"

#include "dsp/complex_math_functions.h"
#include "dsp/complex_math_functions_f16.h"

#include "dsp/filtering_functions.h"
#include "dsp/matrix_functions.h"
#include "dsp/matrix_functions_f16.h"


#include "bench.h"

#if !defined(DISABLEFLOAT16)

void cmsisdsp_add(const std::complex<float16_t>* a, 
                  const std::complex<float16_t>* b, 
                        std::complex<float16_t>* c, 
              uint32_t l)
{
   
   arm_add_f16(reinterpret_cast<const float16_t*>(a),
               reinterpret_cast<const float16_t*>(b),
               reinterpret_cast<float16_t*>(c),2*l);
};


void cmsisdsp_add(const float16_t* a, 
              const float16_t* b, 
                    float16_t* c, 
              uint32_t l)
{
   
   arm_add_f16(a,b,c,l);
};
#endif

void cmsisdsp_add(const std::complex<float64_t>* a, 
              const std::complex<float64_t>* b, 
                    std::complex<float64_t>* c, 
              uint32_t l)
{
   
   arm_add_f64(reinterpret_cast<const float64_t*>(a),
               reinterpret_cast<const float64_t*>(b),
               reinterpret_cast<float64_t*>(c),2*l);
};

void cmsisdsp_add(const float64_t* a, 
              const float64_t* b, 
                    float64_t* c, 
              uint32_t l)
{
   
   arm_add_f64(a,b,c,l);
};


void cmsisdsp_add(const std::complex<float32_t>* a, 
              const std::complex<float32_t>* b, 
                    std::complex<float32_t>* c, 
              uint32_t l)
{
   arm_add_f32(reinterpret_cast<const float32_t*>(a),
               reinterpret_cast<const float32_t*>(b),
               reinterpret_cast<float32_t*>(c),2*l);
};

void cmsisdsp_add(const float32_t* a, 
              const float32_t* b, 
                    float32_t* c, 
              uint32_t l)
{
   arm_add_f32(a,b,c,l);
};


void cmsisdsp_add(const std::complex<Q31>* a, 
              const std::complex<Q31>* b, 
                    std::complex<Q31>* c, 
              uint32_t l)
{
  
   arm_add_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),
               reinterpret_cast<q31_t*>(c),2*l);
};

void cmsisdsp_add(const Q31* a, 
              const Q31* b, 
                    Q31* c, 
              uint32_t l)
{
  
   arm_add_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),
               reinterpret_cast<q31_t*>(c),l);
};


void cmsisdsp_add(const std::complex<Q15>* a, 
              const std::complex<Q15>* b, 
                    std::complex<Q15>* c, 
              uint32_t l)
{
   
   arm_add_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),
               reinterpret_cast<q15_t*>(c),2*l);
};

void cmsisdsp_add(const Q15* a, 
              const Q15* b, 
                    Q15* c, 
              uint32_t l)
{
   
   arm_add_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),
               reinterpret_cast<q15_t*>(c),l);
};

void cmsisdsp_add(const std::complex<Q7>* a, 
              const std::complex<Q7>* b, 
                    std::complex<Q7>* c, 
              uint32_t l)
{
   
   arm_add_q7(reinterpret_cast<const q7_t*>(a),
               reinterpret_cast<const q7_t*>(b),
               reinterpret_cast<q7_t*>(c),2*l);
};

void cmsisdsp_add(const Q7* a, 
              const Q7* b, 
                    Q7* c, 
              uint32_t l)
{
   
   arm_add_q7(reinterpret_cast<const q7_t*>(a),
               reinterpret_cast<const q7_t*>(b),
               reinterpret_cast<q7_t*>(c),l);
};

#if !defined(DISABLEFLOAT16)

void cmsisdsp_mult(const std::complex<float16_t>* a, 
                   const std::complex<float16_t>* b, 
                    std::complex<float16_t>* c, 
                   uint32_t l)
{
   
   arm_cmplx_mult_cmplx_f16(reinterpret_cast<const float16_t*>(a),
                reinterpret_cast<const float16_t*>(b),
                reinterpret_cast<float16_t*>(c),l);
};

void cmsisdsp_mult(const std::complex<float16_t>* a, 
                   const float16_t* b, 
                    std::complex<float16_t>* c, 
                   uint32_t l)
{
   
   arm_cmplx_mult_real_f16(reinterpret_cast<const float16_t*>(a),
                reinterpret_cast<const float16_t*>(b),
                reinterpret_cast<float16_t*>(c),l);
};


void cmsisdsp_mult(const float16_t* a, 
              const float16_t* b, 
                    float16_t* c, 
              uint32_t l)
{
   
   arm_mult_f16(a,b,c,l);
};
#endif

void cmsisdsp_mult(const std::complex<float64_t>* a, 
              const std::complex<float64_t>* b, 
                    std::complex<float64_t>* c, 
              uint32_t l)
{
   
   arm_cmplx_mult_cmplx_f64(reinterpret_cast<const float64_t*>(a),
               reinterpret_cast<const float64_t*>(b),
               reinterpret_cast<float64_t*>(c),l);
};

void cmsisdsp_mult(const float64_t* a, 
              const float64_t* b, 
                    float64_t* c, 
              uint32_t l)
{
   
   arm_mult_f64(a,b,c,l);
};


void cmsisdsp_mult(const std::complex<float32_t>* a, 
              const std::complex<float32_t>* b, 
                    std::complex<float32_t>* c, 
              uint32_t l)
{
   arm_cmplx_mult_cmplx_f32(reinterpret_cast<const float32_t*>(a),
               reinterpret_cast<const float32_t*>(b),
               reinterpret_cast<float32_t*>(c),l);
};

void cmsisdsp_mult(const std::complex<float32_t>* a, 
              const float32_t* b, 
                    std::complex<float32_t>* c, 
              uint32_t l)
{
   arm_cmplx_mult_real_f32(reinterpret_cast<const float32_t*>(a),
               reinterpret_cast<const float32_t*>(b),
               reinterpret_cast<float32_t*>(c),l);
};

void cmsisdsp_mult(const float32_t* a, 
              const float32_t* b, 
                    float32_t* c, 
              uint32_t l)
{
   arm_mult_f32(a,b,c,l);
};


void cmsisdsp_mult(const std::complex<Q31>* a, 
              const std::complex<Q31>* b, 
                    std::complex<Q31>* c, 
              uint32_t l)
{
  
   arm_cmplx_mult_cmplx_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),
               reinterpret_cast<q31_t*>(c),l);
};

void cmsisdsp_mult(const std::complex<Q31>* a, 
              const Q31* b, 
                    std::complex<Q31>* c, 
              uint32_t l)
{
  
   arm_cmplx_mult_real_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),
               reinterpret_cast<q31_t*>(c),l);
};

void cmsisdsp_mult(const Q31* a, 
              const Q31* b, 
                    Q31* c, 
              uint32_t l)
{
  
   arm_mult_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),
               reinterpret_cast<q31_t*>(c),l);
};


void cmsisdsp_mult(const std::complex<Q15>* a, 
              const std::complex<Q15>* b, 
                    std::complex<Q15>* c, 
              uint32_t l)
{
   
   arm_cmplx_mult_cmplx_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),
               reinterpret_cast<q15_t*>(c),l);

};

void cmsisdsp_mult(const std::complex<Q15>* a, 
              const Q15* b, 
                    std::complex<Q15>* c, 
              uint32_t l)
{
   
   arm_cmplx_mult_real_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),
               reinterpret_cast<q15_t*>(c),l);

};

void cmsisdsp_mult(const Q15* a, 
              const Q15* b, 
                    Q15* c, 
              uint32_t l)
{
   
   arm_mult_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),
               reinterpret_cast<q15_t*>(c),l);
};


void cmsisdsp_mult(const Q7* a, 
              const Q7* b, 
                    Q7* c, 
              uint32_t l)
{
   
   arm_mult_q7(reinterpret_cast<const q7_t*>(a),
               reinterpret_cast<const q7_t*>(b),
               reinterpret_cast<q7_t*>(c),l);
};

#if !defined(DISABLEFLOAT16)
void cmsisdsp_dot(const std::complex<float16_t>* a, 
              const std::complex<float16_t>* b, 
                    std::complex<float16_t> &c, 
              uint32_t l)
{
   float16_t re,im;
   arm_cmplx_dot_prod_f16(reinterpret_cast<const float16_t*>(a),
                          reinterpret_cast<const float16_t*>(b),l,&re,&im);
   c=std::complex<float16_t>(re,im);

};

void cmsisdsp_hermitian(const std::complex<float16_t>* a, 
              std::complex<float16_t>* b, 
                    std::complex<float16_t> &c, 
              uint32_t l)
{
   float16_t re,im;
   arm_cmplx_conj_f16(reinterpret_cast<const float16_t*>(b),
                      reinterpret_cast<float16_t*>(b),l);
   arm_cmplx_dot_prod_f16(reinterpret_cast<const float16_t*>(a),
                          reinterpret_cast<float16_t*>(b),l,&re,&im);
   c=std::complex<float16_t>(re,im);
};

void cmsisdsp_dot(const float16_t* a, 
              const float16_t* b, 
                    float16_t &c, 
              uint32_t l)
{
   arm_dot_prod_f16(a,b,l,&c);
};

void cmsisdsp_hermitian(const float16_t* a, 
              float16_t* b, 
                    float16_t &c, 
              uint32_t l)
{
   arm_dot_prod_f16(a,b,l,&c);
};
#endif


void cmsisdsp_dot(const float64_t* a, 
              const float64_t* b, 
                    float64_t &c, 
              uint32_t l)
{
   arm_dot_prod_f64(a,b,l,&c);
};

void cmsisdsp_hermitian(const float64_t* a, 
              float64_t* b, 
                    float64_t &c, 
              uint32_t l)
{
   arm_dot_prod_f64(a,b,l,&c);
};

void cmsisdsp_dot(const std::complex<float32_t>* a, 
              const std::complex<float32_t>* b, 
                    std::complex<float32_t> &c, 
              uint32_t l)
{
   float32_t re,im;
   arm_cmplx_dot_prod_f32(reinterpret_cast<const float32_t*>(a),
                          reinterpret_cast<const float32_t*>(b),l,&re,&im);
   c=std::complex<float32_t>(re,im);
};

void cmsisdsp_dot(const float32_t* a, 
              const float32_t* b, 
                    float32_t &c, 
              uint32_t l)
{
   arm_dot_prod_f32(a,b,l,&c);
};

void cmsisdsp_hermitian(const std::complex<float32_t>* a, 
                    std::complex<float32_t>* b, 
                    std::complex<float32_t> &c, 
              uint32_t l)
{
   float32_t re,im;
   arm_cmplx_conj_f32(reinterpret_cast<const float32_t*>(b),
                      reinterpret_cast<float32_t*>(b),l);
   arm_cmplx_dot_prod_f32(reinterpret_cast<const float32_t*>(a),
                          reinterpret_cast<float32_t*>(b),l,&re,&im);
   c=std::complex<float32_t>(re,im);
};

void cmsisdsp_hermitian(const float32_t* a, 
              float32_t* b, 
                    float32_t &c, 
              uint32_t l)
{
   arm_dot_prod_f32(a,b,l,&c);
};

void cmsisdsp_hermitian(const std::complex<Q31>* a, 
              std::complex<Q31>* b, 
                    std::complex<Q<15,48>> &c, 
              uint32_t l)
{
   q63_t re,im;
   arm_cmplx_conj_q31(reinterpret_cast<const q31_t*>(b),
                      reinterpret_cast<q31_t*>(b),l);
   arm_cmplx_dot_prod_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),l,
               &re,&im);
   c = std::complex<Q<15,48>>(re,im);
};

void cmsisdsp_dot(const std::complex<Q31>* a, 
              const std::complex<Q31>* b, 
                    std::complex<Q<15,48>> &c, 
              uint32_t l)
{
   q63_t re,im;
   arm_cmplx_dot_prod_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),l,
               &re,&im);
   c = std::complex<Q<15,48>>(re,im);
};

void cmsisdsp_hermitian(const Q31* a, 
              Q31* b, 
                    Q<15,48> &c, 
              uint32_t l)
{
   arm_dot_prod_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),l,
               reinterpret_cast<q63_t*>(&c));
};

void cmsisdsp_dot(const Q31* a, 
              const Q31* b, 
                    Q<15,48> &c, 
              uint32_t l)
{
   arm_dot_prod_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),l,
               reinterpret_cast<q63_t*>(&c));
};

void cmsisdsp_hermitian(const std::complex<Q15>* a, 
              std::complex<Q15>* b, 
                    std::complex<Q<33,30>> &c, 
              uint32_t l)
{
   q31_t re,im;
   arm_cmplx_conj_q15(reinterpret_cast<const q15_t*>(b),
                      reinterpret_cast<q15_t*>(b),l);
   arm_cmplx_dot_prod_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),l,
               &re,&im);
   c = std::complex<Q<33,30>>(re,im);
};

void cmsisdsp_dot(const std::complex<Q15>* a, 
              const std::complex<Q15>* b, 
                    std::complex<Q<33,30>> &c, 
              uint32_t l)
{
   q31_t re,im;
   arm_cmplx_dot_prod_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),l,
               &re,&im);
   c = std::complex<Q<33,30>>(re,im);
};

void cmsisdsp_hermitian(const Q15* a, 
              Q15* b, 
                    Q<33,30> &c, 
              uint32_t l)
{
   arm_dot_prod_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),l,
               reinterpret_cast<q63_t*>(&c));
};

void cmsisdsp_dot(const Q15* a, 
              const Q15* b, 
                    Q<33,30> &c, 
              uint32_t l)
{
   arm_dot_prod_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),l,
               reinterpret_cast<q63_t*>(&c));
};



void cmsisdsp_hermitian(const Q7* a, 
              Q7* b, 
                    Q<17,14> &c, 
              uint32_t l)
{
   arm_dot_prod_q7(reinterpret_cast<const q7_t*>(a),
               reinterpret_cast<const q7_t*>(b),l,
               reinterpret_cast<q31_t*>(&c));
};

void cmsisdsp_dot(const Q7* a, 
              const Q7* b, 
                    Q<17,14> &c, 
              uint32_t l)
{
   arm_dot_prod_q7(reinterpret_cast<const q7_t*>(a),
               reinterpret_cast<const q7_t*>(b),l,
               reinterpret_cast<q31_t*>(&c));
};

void cmsisdsp_dot_expr(const double* a, 
                       const double* b, 
                       const double* c, 
                       const double* d, 
                       double* tmp1, 
                       double* tmp2, 
                       const double scale,
                       double &r, 
                       uint32_t l)
{
   arm_add_f64(a,b,tmp1,l);
   arm_scale_f64(tmp1,scale,tmp1,l);
   arm_mult_f64(c,d,tmp2,l);
   arm_dot_prod_f64(tmp1,tmp2,l,&r);
};

void cmsisdsp_dot_expr(const float32_t* a, 
                       const float32_t* b, 
                       const float32_t* c, 
                       const float32_t* d, 
                       float32_t* tmp1, 
                       float32_t* tmp2, 
                       const float32_t scale,
                       float32_t &r, 
                       uint32_t l)
{
   arm_add_f32(a,b,tmp1,l);
   arm_scale_f32(tmp1,scale,tmp1,l);
   arm_mult_f32(c,d,tmp2,l);
   arm_dot_prod_f32(tmp1,tmp2,l,&r);
};

void cmsisdsp_dot_expr(const std::complex<float32_t>* a, 
                       const std::complex<float32_t>* b, 
                       const std::complex<float32_t>* c, 
                       const std::complex<float32_t>* d, 
                       std::complex<float32_t>* tmp1, 
                       std::complex<float32_t>* tmp2, 
                       const float32_t scale,
                       std::complex<float32_t> &r, 
                       uint32_t l)
{
   arm_add_f32((const float32_t*)(a),
               (const float32_t*)(b),
               (float32_t*)tmp1,2*l);
   arm_scale_f32((const float32_t*)(tmp1),
                 scale,
                 (float32_t*)(tmp1),2*l);
   arm_cmplx_mult_cmplx_f32((const float32_t*)(c),
                            (const float32_t*)(d),
                            (float32_t*)(tmp2),l);
   arm_cmplx_conj_f32((const float32_t*)(tmp2),
                      (float32_t*)(tmp2),l);
   float32_t re,im;
   arm_cmplx_dot_prod_f32((const float32_t*)(tmp1),
                          (const float32_t*)(tmp2),l,&re,&im);
   r = std::complex<float32_t>(re,im);
};

#if !defined(DISABLEFLOAT16)
void cmsisdsp_dot_expr(const float16_t* a, 
                       const float16_t* b, 
                       const float16_t* c, 
                       const float16_t* d, 
                       float16_t* tmp1, 
                       float16_t* tmp2, 
                       const float16_t scale,
                       float16_t &r, 
                       uint32_t l)
{
   arm_add_f16(a,b,tmp1,l);
   arm_scale_f16(tmp1,scale,tmp1,l);
   arm_mult_f16(c,d,tmp2,l);
   arm_dot_prod_f16(tmp1,tmp2,l,&r);
};

void cmsisdsp_dot_expr(const std::complex<float16_t>* a, 
                       const std::complex<float16_t>* b, 
                       const std::complex<float16_t>* c, 
                       const std::complex<float16_t>* d, 
                       std::complex<float16_t>* tmp1, 
                       std::complex<float16_t>* tmp2, 
                       const float16_t scale,
                       std::complex<float16_t> &r, 
                       uint32_t l)
{
   arm_add_f16((const float16_t*)(a),
               (const float16_t*)(b),
               (float16_t*)tmp1,2*l);
   arm_scale_f16((const float16_t*)(tmp1),
                 scale,
                 (float16_t*)(tmp1),2*l);
   arm_cmplx_mult_cmplx_f16((const float16_t*)(c),
                            (const float16_t*)(d),
                            (float16_t*)(tmp2),l);
   arm_cmplx_conj_f16((const float16_t*)(tmp2),
                      (float16_t*)(tmp2),l);
   float16_t re,im;
   arm_cmplx_dot_prod_f16((const float16_t*)(tmp1),
                          (const float16_t*)(tmp2),l,&re,&im);
   r = std::complex<float16_t>(re,im);
};
#endif

void cmsisdsp_fir(const arm_fir_instance_f32 * S,
                  const float32_t * pSrc,
                  float32_t * pDst,
                  uint32_t blockSize)
{
    arm_fir_f32(S,pSrc,pDst,blockSize);
};

void cmsisdsp_fir(const arm_fir_instance_q7 * S,
                  const Q7 * pSrc,
                  Q7 * pDst,
                  uint32_t blockSize)
{
    arm_fir_q7(S,reinterpret_cast<const q7_t*>(pSrc),
                  reinterpret_cast<q7_t*>(pDst),blockSize);
};

void cmsisdsp_fir(const arm_fir_instance_q15 * S,
                  const Q15 * pSrc,
                  Q15 * pDst,
                  uint32_t blockSize)
{
    arm_fir_q15(S,reinterpret_cast<const q15_t*>(pSrc),
                  reinterpret_cast<q15_t*>(pDst),blockSize);
};

void cmsisdsp_fir(const arm_fir_instance_q31 * S,
                  const Q31 * pSrc,
                  Q31 * pDst,
                  uint32_t blockSize)
{
    arm_fir_q31(S,reinterpret_cast<const q31_t*>(pSrc),
                  reinterpret_cast<q31_t*>(pDst),blockSize);
};


void cmsisdsp_dot_expr(const Q7* a, 
                       const Q7* b, 
                       const Q7* c, 
                       const Q7* d, 
                       Q7* tmp1, 
                       Q7* tmp2, 
                       const Q7 scale,
                       Q<17,14> &r, 
                       uint32_t l)
{
   arm_add_q7(reinterpret_cast<const q7_t*>(a),
               reinterpret_cast<const q7_t*>(b),
               reinterpret_cast<q7_t*>(tmp1),l);
   arm_scale_q7(reinterpret_cast<q7_t*>(tmp1),scale.v,0,
                 reinterpret_cast<q7_t*>(tmp1),l);


   arm_mult_q7(reinterpret_cast<const q7_t*>(c),
               reinterpret_cast<const q7_t*>(d),
               reinterpret_cast<q7_t*>(tmp2),l);

  
   arm_dot_prod_q7(reinterpret_cast<q7_t*>(tmp1),
                    reinterpret_cast<q7_t*>(tmp2),l,&r.v);
};

void cmsisdsp_dot_expr(const Q15* a, 
                       const Q15* b, 
                       const Q15* c, 
                       const Q15* d, 
                       Q15* tmp1, 
                       Q15* tmp2, 
                       const Q15 scale,
                       Q<33,30> &r, 
                       uint32_t l)
{
   arm_add_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),
               reinterpret_cast<q15_t*>(tmp1),l);
   arm_scale_q15(reinterpret_cast<q15_t*>(tmp1),scale.v,0,
                 reinterpret_cast<q15_t*>(tmp1),l);
   arm_mult_q15(reinterpret_cast<const q15_t*>(c),
               reinterpret_cast<const q15_t*>(d),
               reinterpret_cast<q15_t*>(tmp2),l);
   arm_dot_prod_q15(reinterpret_cast<q15_t*>(tmp1),
                    reinterpret_cast<q15_t*>(tmp2),l,&r.v);
};

void cmsisdsp_dot_expr(const std::complex<Q15>* a, 
                       const std::complex<Q15>* b, 
                       const std::complex<Q15>* c, 
                       const std::complex<Q15>* d, 
                       std::complex<Q15>* tmp1, 
                       std::complex<Q15>* tmp2,
                       const Q15 scale, 
                       std::complex<Q<33,30>> &r, 
                       uint32_t l)
{
   arm_add_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(b),
               reinterpret_cast<q15_t*>(tmp1),2*l);
   arm_scale_q15(reinterpret_cast<q15_t*>(tmp1),scale.v,0,
                 reinterpret_cast<q15_t*>(tmp1),2*l);
   arm_cmplx_mult_cmplx_q15(reinterpret_cast<const q15_t*>(c),
               reinterpret_cast<const q15_t*>(d),
               reinterpret_cast<q15_t*>(tmp2),l);
   arm_cmplx_conj_q15(reinterpret_cast<const q15_t*>(tmp2),
                      reinterpret_cast<q15_t*>(tmp2),l);
   arm_shift_q15(reinterpret_cast<q15_t*>(tmp2),2,
                 reinterpret_cast<q15_t*>(tmp2),2*l);
   int32_t re,im;
   arm_cmplx_dot_prod_q15(reinterpret_cast<q15_t*>(tmp1),
                    reinterpret_cast<q15_t*>(tmp2),l,&re,&im);
   r = std::complex<Q<33,30>>(Q<33,30>(re),Q<33,30>(im));
};

void cmsisdsp_dot_expr(const Q31* a, 
                       const Q31* b, 
                       const Q31* c, 
                       const Q31* d, 
                       Q31* tmp1, 
                       Q31* tmp2, 
                       const Q31 scale,
                       Q<15,48> &r, 
                       uint32_t l)
{
   arm_add_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),
               reinterpret_cast<q31_t*>(tmp1),l);
   arm_scale_q31(reinterpret_cast<q31_t*>(tmp1),scale.v,0,
                 reinterpret_cast<q31_t*>(tmp1),l);
   arm_mult_q31(reinterpret_cast<const q31_t*>(c),
               reinterpret_cast<const q31_t*>(d),
               reinterpret_cast<q31_t*>(tmp2),l);
   arm_dot_prod_q31(reinterpret_cast<q31_t*>(tmp1),
                    reinterpret_cast<q31_t*>(tmp2),l,&r.v);
};

void cmsisdsp_dot_expr(const std::complex<Q31>* a, 
                       const std::complex<Q31>* b, 
                       const std::complex<Q31>* c, 
                       const std::complex<Q31>* d, 
                       std::complex<Q31>* tmp1, 
                       std::complex<Q31>* tmp2,
                       const Q31 scale, 
                       std::complex<Q<15,48>> &r, 
                       uint32_t l)
{
   
   arm_add_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(b),
               reinterpret_cast<q31_t*>(tmp1),2*l);
   arm_scale_q31(reinterpret_cast<q31_t*>(tmp1),scale.v,0,
                 reinterpret_cast<q31_t*>(tmp1),2*l);
   arm_cmplx_mult_cmplx_q31(reinterpret_cast<const q31_t*>(c),
               reinterpret_cast<const q31_t*>(d),
               reinterpret_cast<q31_t*>(tmp2),l);
   arm_cmplx_conj_q31(reinterpret_cast<const q31_t*>(tmp2),
                      reinterpret_cast<q31_t*>(tmp2),l);
   arm_shift_q31(reinterpret_cast<const q31_t*>(tmp2),2,
                 reinterpret_cast<q31_t*>(tmp2),2*l);
   
   

   int64_t re,im;
   arm_cmplx_dot_prod_q31(reinterpret_cast<q31_t*>(tmp1),
                    reinterpret_cast<q31_t*>(tmp2),l,&re,&im);
   
   r = std::complex<Q<15,48>>(Q<15,48>(re),Q<15,48>(im));
};

void cmsisdsp_mat_add(const std::complex<float32_t>* a, 
              const std::complex<float32_t>* b, 
                    std::complex<float32_t>* c, 
              uint32_t row,uint32_t col)
{
   arm_matrix_instance_f32 srca;
   arm_matrix_instance_f32 srcb;

   arm_matrix_instance_f32 dst;


   srca.numRows = 2*row;
   srca.numCols = col;
   srca.pData = (float32_t*)a;

   srcb.numRows = 2*row;
   srcb.numCols = col;
   srcb.pData = (float32_t*)b;

   dst.numRows = 2*row;
   dst.numCols = col;
   dst.pData = (float32_t*)c;
   arm_mat_add_f32(&srca,&srcb,&dst);

}

void cmsisdsp_mat_add(const float32_t* a, 
              const float32_t* b, 
                    float32_t* c, 
              uint32_t row,uint32_t col)
{
   arm_matrix_instance_f32 srca;
   arm_matrix_instance_f32 srcb;

   arm_matrix_instance_f32 dst;


   srca.numRows = row;
   srca.numCols = col;
   srca.pData = (float32_t*)a;

   srcb.numRows = row;
   srcb.numCols = col;
   srcb.pData = (float32_t*)b;

   dst.numRows = row;
   dst.numCols = col;
   dst.pData = (float32_t*)c;
   arm_mat_add_f32(&srca,&srcb,&dst);

}

#if !defined(DISABLEFLOAT16)

void cmsisdsp_mat_add(const std::complex<float16_t>* a, 
              const std::complex<float16_t>* b, 
                    std::complex<float16_t>* c, 
              uint32_t row,uint32_t col)
{
   arm_matrix_instance_f16 srca;
   arm_matrix_instance_f16 srcb;

   arm_matrix_instance_f16 dst;


   srca.numRows = 2*row;
   srca.numCols = col;
   srca.pData = (float16_t*)a;

   srcb.numRows = 2*row;
   srcb.numCols = col;
   srcb.pData = (float16_t*)b;

   dst.numRows = 2*row;
   dst.numCols = col;
   dst.pData = (float16_t*)c;
   arm_mat_add_f16(&srca,&srcb,&dst);

}

void cmsisdsp_mat_add(const float16_t* a, 
              const float16_t* b, 
                    float16_t* c, 
              uint32_t row,uint32_t col)
{
   arm_matrix_instance_f16 srca;
   arm_matrix_instance_f16 srcb;

   arm_matrix_instance_f16 dst;


   srca.numRows = row;
   srca.numCols = col;
   srca.pData = (float16_t*)a;

   srcb.numRows = row;
   srcb.numCols = col;
   srcb.pData = (float16_t*)b;

   dst.numRows = row;
   dst.numCols = col;
   dst.pData = (float16_t*)c;
   arm_mat_add_f16(&srca,&srcb,&dst);

}
#endif

void cmsisdsp_mat_add(const std::complex<Q31>* a, 
                      const std::complex<Q31>* b, 
                            std::complex<Q31>* c, 
                      uint32_t row,uint32_t col)
{
   arm_matrix_instance_q31 srca;
   arm_matrix_instance_q31 srcb;

   arm_matrix_instance_q31 dst;


   srca.numRows = 2*row;
   srca.numCols = col;
   srca.pData = reinterpret_cast<q31_t *>(const_cast<std::complex<Q31>*>(a));

   srcb.numRows = 2*row;
   srcb.numCols = col;
   srcb.pData = reinterpret_cast<q31_t *>(const_cast<std::complex<Q31>*>(b));

   dst.numRows = 2*row;
   dst.numCols = col;
   dst.pData = reinterpret_cast<q31_t *>(c);
   arm_mat_add_q31(&srca,&srcb,&dst);

}

void cmsisdsp_mat_add(const Q31* a, 
                      const Q31* b, 
                            Q31* c, 
                      uint32_t row,uint32_t col)
{
   arm_matrix_instance_q31 srca;
   arm_matrix_instance_q31 srcb;

   arm_matrix_instance_q31 dst;


   srca.numRows = row;
   srca.numCols = col;
   srca.pData = reinterpret_cast<q31_t *>(const_cast<Q31*>(a));

   srcb.numRows = row;
   srcb.numCols = col;
   srcb.pData = reinterpret_cast<q31_t *>(const_cast<Q31*>(b));

   dst.numRows = row;
   dst.numCols = col;
   dst.pData = reinterpret_cast<q31_t *>(c);
   arm_mat_add_q31(&srca,&srcb,&dst);

}

void cmsisdsp_mat_add(const std::complex<Q15>* a, 
                      const std::complex<Q15>* b, 
                            std::complex<Q15>* c, 
                      uint32_t row,uint32_t col)
{
   arm_matrix_instance_q15 srca;
   arm_matrix_instance_q15 srcb;

   arm_matrix_instance_q15 dst;


   srca.numRows = 2*row;
   srca.numCols = col;
   srca.pData = reinterpret_cast<q15_t *>(const_cast<std::complex<Q15>*>(a));

   srcb.numRows = 2*row;
   srcb.numCols = col;
   srcb.pData = reinterpret_cast<q15_t *>(const_cast<std::complex<Q15>*>(b));

   dst.numRows = 2*row;
   dst.numCols = col;
   dst.pData = reinterpret_cast<q15_t *>(c);
   arm_mat_add_q15(&srca,&srcb,&dst);

}

void cmsisdsp_mat_add(const Q15* a, 
                      const Q15* b, 
                            Q15* c, 
                      uint32_t row,uint32_t col)
{
   arm_matrix_instance_q15 srca;
   arm_matrix_instance_q15 srcb;

   arm_matrix_instance_q15 dst;


   srca.numRows = row;
   srca.numCols = col;
   srca.pData = reinterpret_cast<q15_t *>(const_cast<Q15*>(a));

   srcb.numRows = row;
   srcb.numCols = col;
   srcb.pData = reinterpret_cast<q15_t *>(const_cast<Q15*>(b));

   dst.numRows = row;
   dst.numCols = col;
   dst.pData = reinterpret_cast<q15_t *>(c);
   arm_mat_add_q15(&srca,&srcb,&dst);

}

void cmsisdsp_mat_add(const std::complex<Q7>* a, 
                      const std::complex<Q7>* b, 
                            std::complex<Q7>* c, 
                      uint32_t row,uint32_t col)
{
   (void)a;
   (void)b;
   (void)c;
   (void)row;
   (void)col;
}

void cmsisdsp_mat_add(const Q7* a, 
                      const Q7* b, 
                            Q7* c, 
                      uint32_t row,uint32_t col)
{
   (void)a;
   (void)b;
   (void)c;
   (void)row;
   (void)col;
   // Doing nothing since there is no equivalent CMSIS-DSP
   // function
   // Required to enable the build

   /*
   arm_matrix_instance_q7 srca;
   arm_matrix_instance_q7 srcb;

   arm_matrix_instance_q7 dst;


   srca.numRows = row;
   srca.numCols = col;
   srca.pData = reinterpret_cast<q7_t *>(const_cast<Q7*>(a));

   srcb.numRows = row;
   srcb.numCols = col;
   srcb.pData = reinterpret_cast<q7_t *>(const_cast<Q7*>(b));

   dst.numRows = row;
   dst.numCols = col;
   dst.pData = reinterpret_cast<q7_t *>(c);
   arm_mat_add_q7(&srca,&srcb,&dst);
*/
}

#if defined(ARM_MATH_MVEI) || defined(ARM_MATH_MVEF)
void _cmsis_outer(const float32_t *a,
                 const float32_t *b,
                 float32_t *res,
                 const uint32_t r,const uint32_t c)
{
   for(unsigned int row=0; row<r ; row ++ )
   {
       for(unsigned int col=0; col<c ; col +=4 )
       {
          mve_pred16_t p0 = vctp32q(c-col);
    
          float32x4_t v1 = vld1q_z(&b[col],p0);
          v1 = vmulq_x_n_f32(v1,a[row],p0);
          vstrwq_p(&res[row*c+col],v1,p0);
       }
   }
}


#if !defined(DISABLEFLOAT16)
void _cmsis_outer(const float16_t *a,
                 const float16_t *b,
                 float16_t *res,
                 const uint32_t r,const uint32_t c)
{
   for(unsigned int row=0; row<r ; row ++ )
   {
       for(unsigned int col=0; col<c ; col +=8 )
       {
          mve_pred16_t p0 = vctp16q(c-col);
    
          float16x8_t v1 = vld1q_z(&b[col],p0);
          v1 = vmulq_x_n_f16(v1,a[row],p0);
          vstrhq_p(&res[row*c+col],v1,p0);
       }
   }
}
#endif

void _cmsis_outer(const Q31 *a,
                 const Q31 *b,
                 Q31 *res,
                 const uint32_t r,const uint32_t c)
{
   const q31_t *pa = reinterpret_cast<const q31_t *>(a);
   const q31_t *pb = reinterpret_cast<const q31_t *>(b);
   q31_t *pr = reinterpret_cast<q31_t *>(res);
   for(unsigned int row=0; row<r ; row ++ )
   {
       for(unsigned int col=0; col<c ; col +=4 )
       {
          mve_pred16_t p0 = vctp32q(c-col);
    
          int32x4_t v1 = vld1q_z(&pb[col],p0);
          v1 = vqdmulhq_m_n_s32(vuninitializedq_s32(), v1,pa[row],p0);
          vstrwq_p(&pr[row*c+col],v1,p0);
       }
   }
}

void _cmsis_outer(const Q15 *a,
                 const Q15 *b,
                 Q15 *res,
                 const uint32_t r,const uint32_t c)
{
   const q15_t *pa = reinterpret_cast<const q15_t *>(a);
   const q15_t *pb = reinterpret_cast<const q15_t *>(b);
   q15_t *pr = reinterpret_cast<q15_t *>(res);
   for(unsigned int row=0; row<r ; row ++ )
   {
       for(unsigned int col=0; col<c ; col +=8 )
       {
          mve_pred16_t p0 = vctp16q(c-col);
    
          int16x8_t v1 = vld1q_z(&pb[col],p0);
          v1 = vqdmulhq_m_n_s16(vuninitializedq_s16(),v1,pa[row],p0);
          vstrhq_p(&pr[row*c+col],v1,p0);
       }
   }
}

void _cmsis_outer(const Q7 *a,
                 const Q7 *b,
                 Q7 *res,
                 const uint32_t r,const uint32_t c)
{
   const q7_t *pa = reinterpret_cast<const q7_t *>(a);
   const q7_t *pb = reinterpret_cast<const q7_t *>(b);
   q7_t *pr = reinterpret_cast<q7_t *>(res);
   for(unsigned int row=0; row<r ; row ++ )
   {
       for(unsigned int col=0; col<c ; col +=16 )
       {
          mve_pred16_t p0 = vctp8q(c-col);
    
          int8x16_t v1 = vld1q_z(&pb[col],p0);
          v1 = vqdmulhq_m_n_s8(vuninitializedq_s8(),v1,pa[row],p0);
          vstrbq_p(&pr[row*c+col],v1,p0);
       }
   }
}
#endif

const float32_t householder_pattern_4[4] = {-0.45143621, -1.        ,  0.07823104,  0.56856041};
const float32_t householder_pattern_16[16] ={ 0.5122637 , -0.86917937, -0.19042511,  0.46574409,  0.14121624,
       -0.19945171,  0.12364709,  0.56458161, -1.        , -0.46570847,
        0.37724627, -0.68176618,  0.66132393, -0.27301838, -0.30102456,
       -0.46863711};
const float32_t householder_pattern_32[32] ={-0.27607152, -0.64010692,  0.24924816, -0.1221291 ,  0.09912046,
        0.085217  ,  0.24820676, -0.19291274,  0.41545563, -0.02633568,
        0.60912863,  0.13165173, -0.20704703, -0.70260591,  0.69223474,
        0.50554362, -1.        , -0.02547366, -0.35907901,  0.11506218,
        0.26298233, -0.42770096, -0.01737023, -0.00818395,  0.17304538,
        0.25923665,  0.16836481,  0.08255562, -0.11357458,  0.02184729,
        0.76109268, -0.02213071};

static void copy_f32_to_f64(float64_t *dst,float32_t *src,int nb)
{
   for(int i=0;i<nb;i++)
   {
      dst[i] = (float64_t)src[i];
   }
}

void cmsis_init_householder(float32_t *f,const int nb)
{
   if (nb==4)
   {
      memcpy(f,householder_pattern_4,sizeof(float32_t)*nb);
   }
   if (nb==16)
   {
      memcpy(f,householder_pattern_16,sizeof(float32_t)*nb);
   }
   if (nb==32)
   {
      memcpy(f,householder_pattern_32,sizeof(float32_t)*nb);
   }
}

void cmsis_init_householder(float64_t *f,const int nb)
{
   if (nb==4)
   {
      copy_f32_to_f64(f,(float32_t*)householder_pattern_4,nb);
   }
   if (nb==16)
   {
      copy_f32_to_f64(f,(float32_t*)householder_pattern_16,nb);
   }
   if (nb==32)
   {
      copy_f32_to_f64(f,(float32_t*)householder_pattern_32,nb);
   }
}

#if !defined(DISABLEFLOAT16)
static void copy_f32_to_f16(float16_t *dst,float32_t *src,int nb)
{
   for(int i=0;i<nb;i++)
   {
      dst[i] = (float16_t)src[i];
   }
}
#endif

#if !defined(DISABLEFLOAT16)
void cmsis_init_householder(float16_t *f,const int nb)
{
   if (nb==4)
   {

      copy_f32_to_f16(f,(float32_t*)householder_pattern_4,nb);
   }
   if (nb==16)
   {
      copy_f32_to_f16(f,(float32_t*)householder_pattern_16,nb);
   }
   if (nb==32)
   {
      copy_f32_to_f16(f,(float32_t*)householder_pattern_32,nb);
   }
}
#endif

const float32_t random_isometry_4x4_f32[4*4] = {-0.46109607, -0.24352342, -0.81533958, -0.20615929, -0.3728161 ,
        0.62793435, -0.16185064,  0.32881827,  0.06821765,  0.46658338,
       -0.15378958,  0.43166011, -0.71032818,  0.16213376,  0.49825106,
       -0.37650103};

const float32_t random_isometry_16x16_f32[16*16] = {5.69380480e-02, -1.37633572e-01,  1.02055860e-01, -8.48624465e-02,
  1.94451169e-01,  1.03345545e-01, -3.74331557e-01, -4.13616325e-02,
  7.63005061e-02, -4.94134212e-01, -1.36399799e-01, -2.34170108e-01,
  2.21098471e-01,  4.51396341e-01, -1.67600155e-01, -3.11297704e-01,
  1.36993291e-01,  1.51453253e-01,  3.08991232e-01, -3.50304890e-02,
 -8.17043008e-02,  7.23517595e-02,  4.08315013e-02,  2.53488652e-01,
 -3.50447543e-02, -1.99633336e-01, -8.31197208e-02, -2.76697671e-02,
  2.66466406e-01, -1.78378772e-01, -3.66992587e-01, -5.04665668e-02,
 -2.51978196e-01,  2.02263809e-01,  8.35011008e-02, -4.91719830e-02,
 -1.29486498e-01,  1.04851169e-01, -1.88649843e-01, -5.32032212e-01,
  1.43910051e-01,  1.61278447e-01,  2.22376337e-01, -2.36662900e-01,
 -2.71846944e-01,  3.66596236e-01, -1.11185786e-01,  7.81310847e-02,
  3.53524969e-01, -1.35451309e-01, -3.65224785e-01, -1.73087258e-01,
  3.56828211e-01,  1.31555555e-01, -1.88240136e-01,  4.81464791e-02,
  1.67439223e-01,  1.89666937e-03, -1.71239944e-01, -4.21910491e-02,
  1.80633363e-02,  1.20020438e-01,  1.75296329e-01,  5.74151092e-01,
  2.13621928e-01, -7.10558544e-02,  1.83525432e-01,  4.08087648e-01,
  3.49914215e-01, -1.52501220e-01,  3.00653159e-01,  1.92397288e-01,
  6.65496283e-04,  2.04129695e-01,  2.44058447e-01, -5.00917810e-01,
 -1.66055735e-01,  1.86709041e-01, -1.02839917e-02, -2.63332969e-02,
  1.97894305e-01,  1.22772391e-01, -1.61258705e-01, -1.72451564e-01,
  3.09024149e-03,  9.12842953e-02,  3.09511036e-01, -2.07323365e-01,
 -6.99629188e-02, -6.97344404e-02, -2.50295951e-01, -1.53226594e-01,
 -2.93244592e-01, -1.34252109e-01, -6.64644986e-01,  1.55594413e-01,
 -1.47153070e-01, -3.98551792e-01,  1.74488203e-01, -6.32203920e-01,
  1.82021260e-01, -2.98756961e-01, -1.31454807e-02,  1.85808006e-01,
 -2.08605255e-02,  2.68635742e-01,  1.41703314e-01, -1.51864630e-01,
  1.97872685e-02, -3.14114244e-02, -1.68553523e-01, -4.41946998e-02,
  4.02577933e-02,  2.47885359e-01,  5.27003522e-01,  6.13041442e-03,
 -1.07534783e-01,  2.88014531e-01, -1.55107579e-01,  1.58441382e-01,
 -3.21054176e-02,  3.81734338e-01, -2.87779553e-01, -1.14830542e-01,
  2.00144582e-01,  1.07566114e-01,  3.69813222e-02,  3.34238020e-01,
  1.15886084e-01, -2.11580182e-02, -2.16095820e-01, -9.38499013e-02,
 -6.79561766e-02,  4.25121661e-01, -6.97635074e-02,  7.95747381e-02,
  2.13286846e-01,  2.58985918e-01, -2.38493454e-01, -3.77274172e-01,
 -1.33047386e-01, -2.85864539e-01,  1.90643744e-01, -5.21457552e-01,
  2.12904642e-01, -9.08100801e-02,  5.82814618e-02, -7.44380511e-02,
  3.10668097e-01,  1.74627375e-01,  1.19603505e-01, -4.26906593e-01,
 -5.43326591e-01,  3.06609246e-01, -9.20588152e-02,  2.61406134e-01,
  2.05725912e-01,  1.17398660e-01,  4.55734185e-02, -2.53345011e-01,
 -2.89322012e-01, -7.37293826e-02, -3.05009103e-01,  1.46566531e-01,
  6.68149325e-02,  2.70238436e-01,  3.62324813e-01,  1.22503173e-01,
  3.35018407e-01,  2.18284015e-01,  6.81461115e-02,  1.47314917e-01,
  4.55167266e-01,  2.89331790e-01, -2.79291688e-01, -2.99945263e-02,
  3.87733429e-01,  2.53180306e-01,  2.07995234e-01, -3.59696008e-02,
  3.08937582e-01,  1.29214761e-02, -8.02533250e-02, -2.16496133e-01,
  4.72150532e-01, -4.17375281e-04,  4.00640017e-01,  2.30224042e-01,
  1.07230047e-01, -2.75754164e-01, -5.68768879e-02, -1.08502400e-01,
 -2.13123631e-01,  2.20179510e-01, -2.04425269e-01, -7.12711002e-02,
  1.50341733e-01,  2.99767117e-01, -7.57381577e-02,  3.20772225e-02,
 -4.16191946e-01, -1.76969952e-01,  4.60127602e-01, -3.63194969e-01,
  2.64088733e-01, -2.78004410e-01,  3.45176282e-02,  1.66762311e-01,
  5.57703424e-01, -1.18007838e-01, -1.99062209e-01, -4.34665120e-03,
 -5.57962734e-01,  6.64024950e-03, -1.34880116e-01,  1.39829967e-01,
 -1.69707741e-01,  1.32167214e-01,  3.67184117e-01,  7.29227841e-04,
  8.16874452e-02,  2.51968616e-01, -1.26216578e-01, -3.72284365e-02,
 -6.79566707e-03,  2.97424012e-01, -1.07099948e-01,  1.73646793e-01,
  3.16005321e-01,  2.39779868e-02, -1.60636477e-01,  3.74439024e-01,
 -2.39797156e-01, -2.38134722e-02, -5.97277519e-02,  2.26667442e-01,
 -3.54040830e-01,  2.03223484e-01, -1.18698212e-01, -1.59747336e-01,
  1.37615392e-01, -2.44439739e-01,  1.06614263e-01,  2.58171650e-01,
 -6.86640701e-02, -9.59506452e-02,  2.68646691e-01, -2.80800039e-01,
 -3.64537678e-02, -2.64308516e-01, -1.87361734e-01, -2.38791138e-01,
  1.89438379e-01, -6.88119003e-02,  1.65692833e-01,  7.86450332e-02};


const float32_t random_isometry_32x32_f32[32*32] = {
   0.2561178037923184, 0.12684598745056058, -0.1917076894756357, -0.10063528247711584, 0.11693583745997702, -0.04671492794164545, -0.07624476799030552, 0.30189950350250494, 0.23310890269419243, 0.1386829877274808, -0.2663701610096585, 0.04246632758331309, -0.21673664107159046, -0.08088422032913818, 0.06156467481215969, 0.03341794504741294, 0.0067768692433383265, -0.16145279814163666, -0.23539592611537435, -0.030866635697717748, -0.13576095002151292, -0.03539874938471446, 0.25577510718870045, -0.05135496657724662, -0.17725793700446804, 0.1441799927702897, -0.09520657792755119, -0.0829904428835437, -0.07643382798626183, 0.0103549422503809, 0.03706561744379559, 0.1915738967205854, -0.10570006934208497, -0.17622733109065847, -0.10798867493313294, 0.20365667774247836, -0.03987816355768947, -0.11871419561812659, -0.2902905748026591, -0.03047258459569701, 0.044252100115127445, 0.09039343485126038, -0.17158351854245388, -0.004913415468891688, 0.2821763530331642, 0.0758076324854308, 0.22192341633413717, 0.11394456976452935, -0.04506972478619754, -0.11047989079552313, 0.055316728323324245, -0.19674317890403176, -0.2620859130894082, -0.20220090259783494, 0.22562636596391122, 0.2079132037845987, 0.3007258077032465, -0.12770191135652412, 0.1147187625872622, -0.08385198957235337, -0.16889144920579613, -0.15335823599250653, -0.10032911111524084, 0.15154496254960204, -0.27110430255331736, 0.03446590444825111, 0.011569347954848952, 0.12199803400778499, 0.03853130094342648, 0.04327847721840131, -0.07140001994011351, -0.06968255182190973, 0.09371312587629484, -0.0796257520410159, 0.09963927745937123, 0.08825034122941652, 0.026134222163579544, 0.10445374894647012, 0.009612145145104006, -0.0888659092353381, -0.10060493665314264, 0.17806090677054262, -0.34456529296806176, -0.22610574956595422, 0.2442709299379727, 0.27558699814619053, -0.02166696853280889, -0.06932987771181881, 0.10972961272808911, -0.02441705320252146, 0.015655150855478257, -0.12057080426341928, 0.07701417928154868, 0.14878359631523494, -0.17138748341826546, 0.15252347699142516, 0.07307874117692119, 0.2599945228212697, -0.19196817529229523, 0.23893586474101416, -0.07696262651096888, -0.1352211155526756, -0.041284295600799846, -0.21301715397179527, -0.2563211183482171, -0.3340021012892192, -0.04716693418400724, -0.09675400307718163, -0.018854948314686037, -0.19149500450816775, 0.16882225279666865, 0.20268698505306687, 0.013405172657128499, -0.05979550282061416, -0.032433133080206934, 0.07763877618480201, 0.03355076586756994, 0.056574287061141974, -0.1052728227980981, 0.0945889184782356, -0.14030390244022983, -0.16043105366130245, 0.037304705278071164, 0.04474869486186574, 0.052368429087749184, 0.0650786493415232, -0.2713473792246009, 0.27494626846571385, 0.028268454734329485, 0.3480451702808647, -0.04974495736760035, -0.2433707430132722, -0.05789585540739654, -0.09012593022296074, -0.30601458521037606, -0.1039674276492338, 0.026269600828528278, 0.0047719861976243325, 0.06401955646839647, 0.15774233020915135, 0.07618405846921543, -0.09503916249041991, -0.10695351845514715, -0.22847050770676344, 0.09028232721073787, 0.1160625046605568, -0.11393053313832605, -0.0086390746345271, -0.3742125095214824, -0.09911681417004073, -0.08593841289626461, -0.05751767662292259, 0.035454126573429987, -0.07218929694719181, -0.03073384340671336, -0.18303958077972324, 0.23503956085066274, -0.10822474547082754, -0.11007405828556358, 0.21190515111918792, -0.0038990384561383862, -0.04739961762613928, -0.12815401665453657, -0.1003128020536073, -0.02252448054702075, -0.15758625545483165, 0.1266132337998702, -0.21725602995974114, 0.03149917880408743, -0.00909335252543754, 0.035315621071771275, -0.08275213549137503, 0.03426829458468214, -0.30852315093568117, -0.3230221334146357, -0.07262921157711788, 0.23072168928420925, -0.16087779392884421, 0.08020960450704782, 0.08975714667808486, -0.09550248365062361, -0.07646796406699388, 0.056467673772112065, -0.27229915166121926, -0.004110167828106709, -0.1389216524764349, 0.11929904568430544, 0.1364113675062776, -0.014638640583718804, -0.31240243132266704, -0.03229212403822174, -0.004221995736745128, -0.14402246538226524, 0.0571342635921929, -0.32835824252753615, 0.06644559086044265, 0.22438679402648234, 0.1440731439998364, 0.030336681899681504, 0.1502422775169925, -0.23109608324250408, -0.1411459168968176, -0.11844680704246013, -0.30520244037022565, -0.02738381793479607, 0.0035442706572370846, 0.09669433997401872, -0.04613343950201049, 0.04732975087956472, 0.014549027076758099, -0.13121759468895322, -0.02460356523763056, -0.03232667431347976, -0.04845182066413095, -0.08235638016484574, 0.14031229797310532, 0.0027054463226032595, -0.027757254644707397, 0.1778802867881957, 0.17388526757999986, 0.42539349226150647, -0.2654577160148921, 0.065142774477254, -0.28357814944277326, 0.14366337749590563, -0.22725449247582014, -0.01094072435276211, -0.3359412918589741, 0.13015352997964805, 6.61381730776713e-05, 0.08456880983473965, -0.006407755340981518, -0.20468197843712385, -0.13425056077921313, 0.3659396678359845, 0.034244315221463026, 0.2211714539010516, -0.09356799438273207, 0.15484605034297158, -0.07455158489012252, 0.02431204214394494, -0.20894882404726028, -0.061184904003959685, -0.20326936016454725, -0.16713601372314507, 0.10251203228488513, 0.07051786519736776, -0.015850756839375067, 0.08698743682235752, 0.1808211918463936, -0.07027928645645996, 0.07228532352817084, -0.046685869245335535, 0.04695068580019925, -0.21228429025285783, -0.042819862002978465, -0.01641530617727657, 0.10437861890641827, -0.29047774620511024, -0.0025272113452721873, -0.15014654479414677, -0.09023228879133674, -0.2572436252403658, 0.13368736800193667, -0.12427412934089052, -0.27352343835927845, -0.11157545725513314, 0.0358341683354513, -0.041146733302599624, 0.20367355713217644, -0.22442550122296206, 0.058848612259126534, -0.04171407967178504, 0.0942820307815541, -0.10887728411171237, -0.036125804496997004, 0.03142928904292007, 0.026875340793428064, -0.13094262410679905, -0.3454922285904002, 0.06617687623889394, 0.06526560679207162, -0.2437200950162795, -0.10277695006179548, -0.1154201402845259, 0.05880025885478013, 0.10217653678407898, -0.17682216371606368, 0.1296681198309713, -0.04154197140733042, -0.247566185742127, 0.11367250354290868, -0.13171738584805343, 0.01386919903311146, -0.1786875657372822, -0.11819554116277885, 0.09632413081147624, -0.12171188383320061, 0.11346764265106, -0.000341461008715633, -0.18534776087822208, 0.034983866856437194, -0.07731778930904892, 0.1310631956390652, 9.802660529369879e-05, -0.2566486523002406, -0.10053418279600666, 0.11665691324950325, 0.1649942147666625, 0.0706473424464617, 0.253920504316277, -0.04970100548737276, 0.147035811639944, -0.019656497252087787, 0.02524423607366051, 0.09744888733945554, -0.20312395787565346, -0.017399080315047666, -0.11420779070918645, -0.09035521834583925, -0.05889905896759835, 0.1097338005911875, -0.24275087661833344, -0.0731637801578856, -0.014917794259080882, 0.20817836178470084, 0.08193733078867455, -0.015750226291373792, 0.1818438232854288, 0.12831409315890965, 0.0529640884692024, -0.34808451520778044, 0.22867881985871605, -0.1538200566062835, -0.1024700947516053, -0.08303207038161074, -0.1630929990662372, 0.18241980585294665, 0.31213534717490155, 0.0026446227377929757, -0.005878898805021282, 0.299825078910776, 0.11801786547261377, 0.058000400570935966, -0.12448397524927313, 0.036112836240251606, -0.2294994861755894, -0.06948439989328542, -0.03700083681452396, -0.09251408066762663, 0.07979535036142396, 0.2497096441696611, -0.20518640386471043, -0.03874910090319277, 0.2585740178600322, -0.02368749865907113, -0.03584334479242219, -0.2674979360876386, 0.240434637639629, -0.0683666287019284, 0.19258402434152408, -0.1573662960584378, -0.02754179109569939, -0.16692457100523547, -0.25912695416346465, 0.18096841459244678, 0.08874282619386742, -0.07010033210535456, 0.30840609449569933, 0.2472121920179338, -0.18867709336490102, 0.12147477277335744, -0.08057276075343331, 0.012160404226121116, 0.29311589430865137, -0.08378499506014465, 0.05604529677429519, -0.1621315713177062, 0.030444652850968704, 0.06210215503210762, 0.033674742745930464, -0.10505726087941071, 0.04626921795164311, 0.17382620008497077, -0.07328619634753421, 0.022098156903996805, -0.10026200787013871, 0.05465018238951794, 0.1604556066780265, 0.1466543923140461, 0.017902930194840735, -0.2624593034947462, 0.1993851914178314, -0.10375320444066778, 0.1983262549323277, 0.10841315202691629, -0.007487464484773034, 0.24209493710843374, -0.004083436673047244, 0.06368268726660438, 0.07705723454671544, -0.2249983332073088, -0.21291609117482685, -0.13037563048229045, -0.21133001480031602, -0.15749775764014268, 0.1213492169376623, 0.07996381281129579, -0.4115702205321703, 0.18149284590043518, -0.13206559528370365, 0.19315049918129332, 0.13492148858575792, 0.0918492794948074, 0.17666636351609327, -0.023061471478566818, 0.15133204809306544, -0.15535559663355772, -0.07497873851906624, -0.12414109479445153, -0.039373221753715104, -0.17381368752228446, 0.17838570811453014, -0.0636110952603086, -0.2197158741536992, 0.028442880552786345, -0.29422350314033396, -0.09959855358103781, 0.0027965813413910047, 0.21364906521977925, 0.013363760294760567, 0.1897594401945677, -0.0070972097699741514, 0.08273506520546635, 0.05690319929122007, -0.061666408405260636, -0.22648390867159962, 0.18275938146348367, 0.05044901843748163, -0.0026200473647586503, 0.007748839829812429, 0.029012856435583614, 0.14709554193014532, 0.12964080099818714, -0.11658180762115257, 0.16293823184044193, 0.04688488646000517, -0.0104152863619153, 0.06115092131516587, -0.20910933655694985, -0.35977989803684657, 0.009471940657896766, -0.07493366607248766, -0.13369370209602285, -0.083909063003001, -0.005746218092670262, 0.1244278769270802, 0.2565077357155445, 0.23069661573371963, 0.01609987186619864, -0.05551036820739742, -0.02689516419253168, -0.036461413781285514, -0.05637110802216756, 0.015146337208381292, 0.23225606552711045, -0.01366768726596876, 0.018906123708229798, -0.07457092047891911, 0.054836685560106575, 0.19073602481311228, -0.07883677199963147, 0.07165912249025612, -0.08425594530373907, 0.2911655576681845, -0.22515531568575609, 0.25344534931802065, 0.2039519582162844, 0.03188849190919943, -0.324652179951761, 0.011857039779151401, 0.33289177167682327, -0.020080112782194303, 0.14294434305306206, 0.22509208668674802, 0.09578234695972923, 0.2221391050270396, -0.09974658651923257, 0.0426145378703141, 0.18713363198031557, -0.11067963124634331, 0.10629696573474223, 0.22777580196435132, 0.03353592040337919, -0.2328365915609321, 0.2129943213534501, 0.43707089115249154, 0.05566384348817685, -0.13359110691682513, -0.0517535505305312, 0.006398993034951886, 0.20431371339896823, 0.06402823850418864, 0.18778353282727103, 0.11410065355785408, 0.10244376610181996, -0.01883393551590708, -0.1747101741128215, -0.17145398592590078, -0.047711502753134495, -0.2417337154493197, -0.26698684378491117, 0.013316461409577722, 0.14680713539876497, -0.02605847216031829, -0.15080533662137197, -0.07501494556539297, 0.09338555476844451, 0.011496448486698627, -0.08463832118231716, -0.13021925976062773, -0.0665295338451879, 0.1644176402548816, -0.125226185305711, 0.18225233761186183, 0.3480997811559141, 0.10520778959276025, 0.007937766500041224, 0.16509093415678036, 0.022689622570169116, 0.21053059456243886, -0.08196375114289262, -0.08549400472101745, 0.03830894059133025, 0.15383915632214595, -0.19823686086820527, -0.11256028844310179, -0.303325000726246, -0.015241824240610486, 0.0009702000539829959, 0.009851911959416535, 0.32486860622431896, -0.08313101457038266, 0.007868930586343057, -0.10209139729213472, 0.1018445625989364, -0.09472111158487753, 0.02158574463732234, 0.0007447315479310631, -0.14700821325499444, -0.08151465637456679, -0.09978005200105113, -0.03642674704924549, 0.2222522623700555, 0.1039092936979776, 0.16435031931580696, -0.16141559345602532, -0.0585451392966191, 0.2775748969149004, 0.192582643951237, 0.035933516734382005, 0.15844313347666247, 0.22957605131983702, 0.32051898376429055, 0.021789795682756512, -0.133738052197895, 0.10114612825527054, 0.2486176570123965, -0.08466720379267295, 0.027455118397749025, -0.27978134530405874, 0.014075938407688901, 0.1730405670528408, 0.15689832178917718, -0.08832306213365565, -0.10508536307862607, 0.19090427380366137, -0.06654583374149685, 0.07590600994935545, 0.04758524051570592, -0.05746876265534356, -0.08910304008721094, -0.15161981001215374, 0.21288009927445295, -0.12806849142071702, -0.0643953934417623, 0.0077005847796731575, 0.06695521166606018, -0.11624150708919286, 0.16407747847973364, 0.06576804325767316, 0.048998750646844696, 0.27430833064139043, 0.12225166589643804, 0.15234573464375112, -0.1732421988580575, -0.20327568326952003, 0.33123371384387273, -0.06304678300784218, 0.13131123361025374, 0.02285426872038852, -0.03663807549787293, -0.3281667718342268, 0.018744697930205233, 0.09224661589407485, -0.2838179930598059, 0.17895289725650118, -0.03993615984881065, -0.21307344234146677, 0.01720267772445222, 0.02498748092691066, -0.09779814372920163, -0.2084391276868567, -0.10197487616923799, -0.138611459232242, 0.11770727784056159, 0.1093575743073583, 0.1779346208898737, -0.13381986571128943, -0.09859537850017386, 0.12043983350786394, 0.07217676618982384, 0.13444303511211048, 0.031176071734401775, 0.18732807571993393, 0.042802868915874114, -0.018462319729519283, -0.1649316735567051, -0.016225593660044856, -0.033043475837437865, -0.3043172024021449, -0.0479629154811289, -0.25550529906551894, 0.33693283924599543, 0.3498026984378602, 0.0758626478903696, 0.006055531164562854, 0.05146810974609485, -0.15777807811572972, 0.06321037609158547, 0.10422473643714457, 0.26038788790602463, 0.04971527385154977, -0.18639620120840072, 0.01465660915875183, 0.024932025105752688, -0.02507035578131709, 0.16737627096607954, -0.05022755556871816, 0.09209629237756908, 0.08590222765557777, 0.07304464104169527, -0.17996746048370674, -0.04883325838480134, 0.04355652351349871, 0.18202783081542567, 0.09965388386158044, 0.09661181671002227, 0.1774379614068804, 0.04371905479471258, 0.029139084243306516, -0.12540369357599895, -0.06972542025320613, -0.21370326666631406, -0.03427170944258605, 0.03579947287057464, 0.1677629806791523, 0.19625282697679272, -0.16650552335306867, 0.029492759973581844, -0.18212429027607438, -0.08978623695968901, -0.10640401803240693, 0.20203751682346044, 0.05641980532248272, 0.19755579170191134, 0.017103718128771397, 0.3353343937998915, -0.046986842646847535, 0.17994212056919923, -0.04660542881869505, 0.19660598523684739, 0.24455360582365174, 0.07468901154026832, -0.19412511313402397, 0.12900105774203632, 0.22256121038789875, -0.09168317394725548, 0.04905800193624112, -0.13400818695281272, 0.12012600591845897, -0.11082125534632785, -0.024805873201654276, -0.010505657921361397, 0.07771291305202682, -0.24401243807174114, 0.22798978303874265, -0.07226937498823606, 0.04804240759905105, 0.12151381976632987, -0.00484077880945019, -0.24481617171522457, -0.09003810827469454, 0.044829646480003316, -0.2007484178561026, 0.021333928661019352, -0.18368744485192556, -0.3945278911064757, 0.3450388372202634, -0.31324871329444376, 0.0334238997401158, 0.08332480114547877, -0.034823258969994705, 0.023664719563604466, 0.03203614938650961, -0.01843495352062995, -0.07638801017196499, 0.07545142950586059, 0.15756801304618853, -0.06348029717905401, -0.06303266422577995, -0.07781948705092531, -0.06748234256064453, 0.15348656381492884, 0.17831085847902894, 0.16523660639762922, -0.008609493421024154, 0.05521137336581912, 0.25156537378140087, 0.15390819310273196, -0.11123274878708922, 0.05735904281790028, -0.009743606036425217, -0.04401248200963786, -0.15898221986219915, 0.10635383392245179, 0.1177347122219892, -0.039031430456485536, 0.14657083965776424, -0.28151804005331127, -0.0960298764735709, 0.22681346696514573, -0.3078510330883708, 0.05478756617211884, 0.19596365262602147, -0.22577487524366147, 0.05707322420763417, -0.10239420324194279, -0.029064217342557496, -0.1671115578223764, -0.07827174135948453, 0.20604178987214544, -0.04623452357723414, 0.36584223245167835, -0.11443440145139766, -0.12600902738047678, 0.08867466941498482, 0.021741014902388927, 0.21069964816467782, 0.26960609450304457, -0.01794086483398188, -0.053382715284288514, 0.0762512845898333, 0.3016537411505285, -0.02904674749836766, -0.08314348404859277, -0.16799614428133752, -0.11227284412590108, -0.01479364119851934, -0.03368335459633163, -0.035609471465698775, 0.012364453788310394, -0.27920782135499933, -0.022144743265321957, -0.15728611497174236, -0.3669613839802514, 0.03816337857200884, 0.14872904933635614, 0.10690424235363429, 0.06960071699468763, -0.18616569389912457, 0.192221683275039, -0.12896528951045944, -0.054508888891250454, -0.007301843225704774, -0.11232637109634373, 0.07468497791842349, 0.13463170024889806, -0.06187207258709113, 0.3409985850406003, -0.18653795960273561, 0.16500504690274606, 0.08917277898123656, 0.2743125500617754, 0.10765500318007457, 0.21103504654349084, 0.021157599513007084, -0.15797716090026265, 0.022891025896423604, 0.06371041711647721, 0.09636564177536265, 0.08280531125540562, 0.08362040225326248, -0.07484882416763194, -0.39712453274041726, 0.19292977115361273, 0.14270355346903432, 0.16184288773500113, 0.05226571918856513, -0.20504872669223403, 0.12331774191903737, -0.2639359067416911, 0.10650723746154868, 0.0899341119402021, 0.1657772352144212, 0.016142197177269762, 0.06921000695881123, -0.1354744159919178, -0.1591744407955443, 0.41626444614198616, -0.12118067242192326, -0.06281763311277432, 0.012239988480697477, 0.10922700436759217, 0.15812249173406812, 0.2543468103332324, 0.027493586861333887, -0.3498855414125585, 0.03212985982609805, -0.023713316146788517, -0.024163706233171482, -0.07294008432562879, -0.05477133348505127, 0.08034323112871752, -0.21017592572665902, 0.19062561950814758, -0.024524792991013705, -0.022393986110341838, -0.03549982219681733, 0.03324559884642771, 0.021579173477672357, 0.018130380157821775, 0.00893632613351368, -0.35146602375766556, -0.10976169856446748, 0.022157802079062493, 0.11548979108714448, -0.2702568654632607, 0.08506540722816211, 0.06383730244280185, 0.10656797930200143, -0.08922575030745049, 0.04298333961259477, 0.027340720762492814, 0.17574267615921693, -0.07256541289935946, -0.11874536381311326, 0.22292492942827627, -0.009642779777273526, 0.10845719457945065, -0.2558122207518897, 0.060858674070968115, -0.05872992798048492, 0.01767755611633381, -0.06868055250335897, -0.07231485522443135, -0.07250332119037206, -0.12692971733797612, 0.05321040116863758, -0.15360536021071566, -0.1584166701476434, 0.20092176258309025, 0.33756731923965233, -0.07815184407819252, 0.12879039204991216, -0.052057985599975244, 0.19282860851464123, 0.1758997731302304, -0.006819462175680617, 0.2439986253218668, 0.23084687649600116, -0.30140878955688505, 0.17743500789987515, 0.04863727353622686, 0.33096441642760355, 0.057768788197386955, -0.14855821913318967, 0.04621205400912512, -0.153009489286864, -0.26066425672441806, 0.05596292379080578, -0.10559774828119901, 0.06312967840949385, -0.12013582386124567, 0.09512326305324108, 0.04344167624767195, -0.1617265773685114, -0.08456191936572527, -0.1595255943647195, 0.10316601723604268, -0.04723428851593683, -0.0001976232806646932, 0.2846720769325796, 0.0846314304090992, 0.10898269027112098, -0.06555489774349015, 0.23462626928402278, 0.132302382138479, 0.0111949198576615, -0.1277851003857463, 0.08701771038004372, 0.5697742736960127, -0.17264584689348367, 0.196802705553214, -0.036756114778173865, -0.05484553762333096, -0.21857480395130202, 0.06979322560945575, -0.07600199475818348, -0.2719219876499719, 0.01602247150769837, 0.1356575987363262, 0.022145693322166943, -0.02859305587353583, 0.024704483724477137, -0.1778617456757566, 0.05994709013335252, 0.1602593924053039, 0.34871280113389724, 0.15613237290595078, -0.021069535581369418, -0.05876650939753909, 0.11657096507119778, 0.1380155137276868, 0.10317126111152579, 0.04414724139227518, 0.036502272001425286, 0.09427478269242494, -0.0251726967794742, 0.020182058697151412, -0.16880071504526833, 0.12878544057540436, 0.007785717185646125, 0.045378504383455, -0.17076077390975958, -0.11998867325318517, -0.05930651707161328, 0.022580367247136964, -0.03428610405575067, 0.288193828520059, -0.033307715875566496, 0.08874864509352656, 0.012770814780526388, -0.21925312115585802, 0.3704150240776723, -0.05698645490957832, 0.15398799420677453, 0.08099948941021562, 0.06710604973868305, 0.22788366325393555, 0.07926748978086051, -0.26140475943996094, 0.052065480057414175, 0.12225684516708936, -0.2673520789095843, -0.0010810637285782107, 0.029780725166791307, 0.10897552205026335, -0.10737424508932075, 0.13443660123943066, -0.05851102311616972, 0.058329713540546245, -0.04835652953159115, -0.3305492748522251, 0.14560636644260935, -0.05617096073198725, 0.2611876744059577, -0.15251957577059577, -0.13382027663481028, -0.12461249212337004, 0.15280884725862195, 0.25986605727449397, 0.08344931482972708, -0.11720043182192975, -0.20916219228881988, -0.03185161338677769, -0.1474518025129455, -0.05606297225265404, 0.23530315971809831, 0.04647468274074494, -0.2042854560499739, 0.01615658042657827, 0.2731923320544898, 0.035099115401969735, 0.029772519068574872, 0.2777021092790425, 0.2232684570869011, -0.3012412250925181, -0.003647882468379049, 0.06990472728960301, 0.08622117834143679, -0.2265744656745616, 0.3897846094959791, -0.03983623812673232, -0.09558366640194939, -0.1105330210528379, 0.05492805131034025, -0.17746150624680887, 0.08538716739210248, 0.15208612973878907, 0.09552636046829496, -0.12400994339552185, -0.02557268925240459, -0.02371829599712921, -0.17795161434234, 0.17017914652418786, -0.06616123412041572, 0.11104481873813121, 0.18819993055647785, -0.17227486978744855, -0.21921572989010069, 0.01068723206755737, 0.08684370014871967, -0.008758956850276112};

void cmsis_init_qr(float32_t *f,const int r,const int c)
{
   if ((r==4) && (c==4))
   {
      memcpy(f,random_isometry_4x4_f32,sizeof(float32_t)*r*c);
   }
   if ((r==16) && (c==16))
   {
      memcpy(f,random_isometry_16x16_f32,sizeof(float32_t)*r*c);
   }
   if ((r==32) && (c==32))
   {
      memcpy(f,random_isometry_32x32_f32,sizeof(float32_t)*r*c);
   }
}


void cmsis_init_qr(float64_t *f,const int r,const int c)
{
   if ((r==4) && (c==4))
   {
      copy_f32_to_f64(f,(float32_t*)random_isometry_4x4_f32,r*c);
   }
   if ((r==16) && (c==16))
   {
      copy_f32_to_f64(f,(float32_t*)random_isometry_16x16_f32,r*c);
   }
   if ((r==32) && (c==32))
   {
      copy_f32_to_f64(f,(float32_t*)random_isometry_32x32_f32,r*c);
   }
}

#if !defined(DISABLEFLOAT16)
void cmsis_init_qr(float16_t *f,const int r,const int c)
{
   if ((r==4) && (c==4))
   {
      copy_f32_to_f16(f,(float32_t*)random_isometry_4x4_f32,r*c);
   }
   if ((r==16) && (c==16))
   {
      copy_f32_to_f16(f,(float32_t*)random_isometry_16x16_f32,r*c);
   }
   if ((r==32) && (c==32))
   {
      copy_f32_to_f16(f,(float32_t*)random_isometry_32x32_f32,r*c);
   }
}
#endif

const float32_t random_sdp_4x4_f32[4*4]={
25.0, 39.75, 66.0, 93.5, 39.75, 63.75, 105.25, 147.5, 66.0, 105.25, 175.5, 245.5, 93.5, 147.5, 245.5, 353.0
};

const float32_t random_sdp_16x16_f32[16*16]={
2.700945297501233, 2.279307100092882, 1.5849755137283175, 2.417789946689702, 2.1514472142359065, 2.6413305532099938, 2.026227680747844, 1.7078250022934063, 1.5348109459558188, 2.4012149501845625, 1.7725768495595937, 2.488697467808775, 2.068715814553819, 2.5311387770052503, 1.0231398628800124, 2.4969556781492686, 2.279307100092882, 2.6564388387340614, 1.4512508865033824, 2.3362931631915225, 2.2022641318875498, 2.544070682449193, 2.1668252749472816, 2.0319723933818348, 1.3947248959004686, 2.202687851122394, 1.6281812459575296, 2.2257145135264063, 2.023398481811725, 2.2396249860896185, 1.1154785609593063, 2.2212595341670935, 1.5849755137283175, 1.4512508865033824, 1.6569550617596642, 1.4108021059813987, 1.6627059842699814, 2.076225217809575, 1.3765836151027875, 1.589420912872039, 1.3403316198834774, 1.836722771319919, 0.8678634773434062, 1.728402611152431, 1.3615509552340186, 1.6102504498177936, 0.47068891979500005, 1.684990944826532, 2.417789946689702, 2.3362931631915225, 1.4108021059813987, 3.2977954871174324, 2.3700971746706934, 2.7750190489484314, 2.248672968327009, 1.8687438430842913, 1.6209886829713436, 2.63830775439295, 2.1089115458534433, 2.718742724433719, 2.6323234102836435, 2.3460912975174124, 1.2270607357960501, 2.5721257623316998, 2.151447214235906, 2.2022641318875498, 1.6627059842699814, 2.3700971746706934, 3.0230567587214376, 3.191103128332187, 1.875287551874037, 2.319758396156411, 2.247936302529488, 2.9501892532332747, 1.833720516752425, 2.58300998901625, 2.6962033493638122, 2.6361607307799377, 0.9104534672305687, 2.8915802013860312, 2.641330553209994, 2.544070682449193, 2.076225217809575, 2.7750190489484314, 3.1911031283321876, 4.3484734981060615, 2.569210676449175, 2.855623851444787, 3.1344020403009623, 3.1472490536236997, 1.9814080616896939, 3.0774078450536253, 3.2778222516109374, 2.9671743747278128, 1.4048415258735187, 3.016934200980412, 2.026227680747844, 2.1668252749472816, 1.3765836151027875, 2.248672968327009, 1.8752875518740373, 2.569210676449175, 2.634968121194319, 2.2493369236498197, 1.7144578362626999, 1.8431862033710502, 1.8101187705570811, 2.491183185729788, 2.195985858813213, 2.2790423526339376, 1.3014559195883564, 2.169629117150412, 1.7078250022934063, 2.0319723933818348, 1.589420912872039, 1.8687438430842913, 2.319758396156411, 2.8556238514447867, 2.24933692364982, 2.6338671573465913, 2.052039836699484, 2.1572063363553937, 1.5406292680679787, 2.3405114868522885, 2.2642224985836377, 2.2623963844332184, 0.9967642116659812, 2.2464621063026424, 1.534810945955819, 1.3947248959004686, 1.3403316198834774, 1.6209886829713434, 2.2479363025294874, 3.1344020403009623, 1.7144578362626999, 2.052039836699484, 2.7144108908441327, 2.0370544226628753, 1.2935791532813437, 2.1703470519822, 2.3749127415903377, 2.1674537956084623, 0.8344205409590062, 1.967349528138, 2.4012149501845625, 2.202687851122394, 1.8367227713199188, 2.63830775439295, 2.9501892532332747, 3.1472490536236997, 1.8431862033710502, 2.1572063363553937, 2.0370544226628753, 3.205410914995665, 2.0481982559972085, 2.65239077271705, 2.701559542415575, 2.7468960455555504, 0.8823172086187251, 3.0500211870766316, 1.772576849559594, 1.6281812459575296, 0.8678634773434062, 2.1089115458534433, 1.833720516752425, 1.9814080616896936, 1.8101187705570811, 1.5406292680679787, 1.2935791532813437, 2.048198255997209, 2.2547060600561646, 2.109266757642494, 2.172574216035175, 2.238825547924319, 0.9422534917819564, 2.43656985711335, 2.488697467808775, 2.2257145135264063, 1.728402611152431, 2.718742724433719, 2.5830099890162503, 3.077407845053625, 2.4911831857297875, 2.3405114868522885, 2.1703470519822, 2.65239077271705, 2.109266757642494, 3.2866709398645626, 2.4857920499819373, 2.8814369596795624, 1.0989863855011937, 2.985768815796319, 2.068715814553819, 2.023398481811725, 1.3615509552340186, 2.6323234102836435, 2.6962033493638127, 3.2778222516109374, 2.195985858813213, 2.2642224985836377, 2.3749127415903377, 2.701559542415575, 2.172574216035175, 2.4857920499819377, 3.103332874731625, 2.5354740586623126, 1.2815604601575314, 2.708101500955981, 2.5311387770052503, 2.2396249860896185, 1.6102504498177934, 2.3460912975174124, 2.636160730779938, 2.9671743747278128, 2.2790423526339376, 2.2623963844332184, 2.1674537956084623, 2.7468960455555504, 2.238825547924319, 2.881436959679562, 2.5354740586623126, 3.402328502508312, 1.0105553901175188, 3.0030756040726687, 1.0231398628800126, 1.1154785609593063, 0.47068891979500005, 1.2270607357960501, 0.9104534672305687, 1.4048415258735187, 1.3014559195883564, 0.9967642116659812, 0.8344205409590062, 0.8823172086187251, 0.9422534917819564, 1.0989863855011937, 1.2815604601575314, 1.0105553901175188, 0.9633674825639494, 0.9878779132923687, 2.4969556781492686, 2.2212595341670935, 1.684990944826532, 2.5721257623316998, 2.8915802013860312, 3.016934200980412, 2.169629117150412, 2.2464621063026424, 1.967349528138, 3.0500211870766316, 2.43656985711335, 2.985768815796319, 2.7081015009559812, 3.0030756040726687, 0.9878779132923688, 3.4586125928033367
};

const float32_t random_sdp_32x32_f32[32*32] = {
   3.962982337952489, 3.258063047322271, 4.049537103963542, 3.4596919611189594, 2.608748240260557, 3.457762006881469, 3.2193769362666877, 2.36878681220616, 2.608860056155112, 2.9359903261121003, 3.4347952190769093, 3.3489562216269997, 3.236532222240136, 3.3908161478252565, 2.6004287471585146, 3.4778799533527907, 3.9441297355021367, 2.690339079959059, 2.7040435571588723, 4.097516198078844, 2.689397904612436, 3.1757221504502433, 2.7840089755113824, 3.282253321577887, 3.5861682887660606, 3.2593594395795438, 2.856336126475509, 2.875790806724925, 3.5697944697713715, 3.0718633171796257, 2.8639288036044017, 3.4953216860726917, 3.2580630473222714, 5.023658450702154, 4.724924357607405, 4.2959691524241785, 3.0328418814455684, 4.379776948368773, 4.531656553346246, 2.0805311484707274, 4.146861899214031, 2.8448770454684675, 4.298596385456439, 3.5677857895875187, 3.8868091580495783, 4.172158250801878, 3.077421836311209, 3.8000981155967586, 4.764590496557555, 3.404267929293151, 4.099327726255604, 3.993442101897572, 3.2371606240613864, 3.389573370397508, 2.7108312600028444, 3.7189728972187863, 4.832988078372525, 4.0438269034720635, 3.637021975641108, 3.069547233798435, 3.8911683886781083, 4.002699656759023, 3.686659336383217, 4.420883318787573, 4.049537103963542, 4.724924357607405, 7.284179248268556, 5.214541222622886, 3.887970860004088, 5.753806053322463, 5.629546679017673, 3.1802228348908663, 4.152764541595372, 4.036275511661986, 5.254827170066871, 5.532275675413409, 4.654156631928212, 4.841400421776575, 4.2678346017184055, 5.064399938356294, 5.787496583752425, 4.061770065163565, 4.561935395436077, 5.338178627385598, 4.631982669583265, 4.04817911502475, 4.074773528636097, 4.0556967255953875, 6.547668531149082, 5.060337202052291, 4.1585976592724805, 4.179632273577691, 5.390197171213056, 5.109426074778116, 4.47639612115159, 5.501184786945122, 3.4596919611189594, 4.2959691524241785, 5.214541222622886, 5.755337351440731, 3.2793603164682876, 4.507148475335209, 4.719209780833385, 2.7260604546475093, 3.8596642985116865, 4.050808909983587, 4.296660527027206, 4.467077491977087, 3.807955281455994, 4.236755979586986, 3.091042946631457, 4.64090491578105, 4.762949639787548, 3.434756962715794, 3.9622911604240936, 4.9414617708424755, 3.5718883848046374, 4.18403299118855, 3.2526585082487345, 4.375478116829063, 5.019622247849673, 4.328262826144412, 3.5869724275885746, 3.7991926581926996, 4.49573655533737, 4.1338105680488315, 3.8378506208796788, 4.6474502405231695, 2.608748240260557, 3.0328418814455684, 3.887970860004088, 3.2793603164682876, 4.04308751252136, 4.119152859559557, 3.8500257234268096, 2.880692983322458, 3.022137277417667, 3.2126022287886626, 2.859833633423297, 3.3549685517285246, 3.211429795975937, 3.3415961605570947, 3.000485405462325, 2.943122255088694, 3.5880171934791205, 2.3462117508666482, 3.741867881663351, 3.5815075799994185, 2.977398687181556, 3.0738523051908526, 2.647727588076209, 3.1846864162261803, 4.243204991826781, 3.808550080377437, 3.048375263329588, 3.184050092366138, 2.9819193805283186, 4.130140010902999, 3.3669248875223405, 4.209854238756024, 3.457762006881469, 4.379776948368773, 5.753806053322463, 4.50714847533521, 4.119152859559557, 6.546506251335145, 5.5051153713896, 3.6885831799404025, 4.2935116379229, 4.937349888136175, 4.649634614150263, 5.162565873103582, 4.321208816517622, 4.860526725438859, 4.165866130280626, 4.508121000863119, 5.000798950996206, 3.853839597047175, 4.681209355831753, 5.205088044633326, 4.656066608350676, 3.763668449755257, 3.7186768765948246, 4.4137552971329965, 5.972371664188578, 4.870960893115659, 4.0940154113020935, 4.256499943049193, 4.935598187319021, 5.193115956877178, 4.538659358985781, 5.467162237890344, 3.2193769362666877, 4.531656553346247, 5.629546679017673, 4.719209780833385, 3.8500257234268096, 5.5051153713896, 6.22927527739691, 3.1619392465984997, 4.272672935545915, 4.215494152853278, 4.747103580602807, 4.4113724085939685, 4.394126410946309, 4.876346299318329, 4.3970941492352065, 4.690453908817078, 4.987124888771773, 3.8720128577642714, 5.005936538290864, 5.135590050457474, 4.430956864154369, 4.282234094786728, 3.4980695116273197, 4.651259656732732, 6.098362448110332, 4.902641138451369, 4.002140716219812, 4.180661781965751, 4.531866821353604, 5.19681235887693, 4.420638184264228, 5.239797812982575, 2.36878681220616, 2.0805311484707274, 3.1802228348908663, 2.7260604546475093, 2.8806929833224584, 3.6885831799404025, 3.1619392465984997, 3.6015844418444867, 2.061690049445126, 3.1774296031892844, 2.638687436275006, 3.3259966748517122, 2.916641244437966, 3.4147088239201686, 2.995511844750009, 3.1144775076574, 2.8617292785893884, 2.44407453417666, 2.8462162417036643, 3.6254255401247812, 2.4228595252895717, 2.8049680051424444, 2.6415721529678855, 3.0381755089847204, 3.7032550120916836, 3.0862509694866507, 2.7427512583789655, 3.0183933997984527, 3.1416872372889757, 3.4657503224322217, 2.84541646065416, 3.1309563860597294, 2.6088600561551116, 4.146861899214031, 4.152764541595372, 3.8596642985116865, 3.022137277417667, 4.2935116379229, 4.272672935545915, 2.061690049445126, 4.7294795506368175, 3.0221070327854753, 4.025459294089731, 3.080139219364956, 3.631347582120466, 3.5106350455758206, 2.6597059216322783, 3.4239263410600724, 4.129353706826948, 2.9820850873953497, 3.9822531680505335, 3.555341926595753, 3.232590122945775, 3.0681871064352157, 2.641712330539264, 3.563373999945094, 4.621924180487924, 4.029114523703862, 3.667069952178138, 3.017108598729165, 3.4655186480439966, 3.7571439035724405, 3.7973654368338905, 4.429482895305322, 2.9359903261121003, 2.844877045468468, 4.036275511661986, 4.050808909983588, 3.2126022287886626, 4.937349888136175, 4.215494152853278, 3.1774296031892844, 3.0221070327854753, 5.507250764726119, 3.6082520296113896, 4.37712941178483, 3.0201828483854625, 3.532212628781479, 3.3784887957799734, 3.6430306828318626, 3.533512253218044, 2.6605862162118044, 3.1879783187523123, 4.73475228265243, 3.5585532841037466, 3.37039024073841, 3.11298560417523, 4.165794870909168, 4.107268173069444, 3.3972906236575535, 3.0254360330349517, 3.820546211777247, 4.118197397914825, 3.88092917593326, 3.319431850310794, 4.543079325847466, 3.434795219076909, 4.298596385456439, 5.254827170066871, 4.296660527027206, 2.859833633423297, 4.649634614150263, 4.747103580602807, 2.638687436275006, 4.025459294089731, 3.608252029611389, 5.489222332395801, 4.321653434790665, 4.048299728306069, 4.2655438311448926, 3.5161464946579217, 4.12528702657051, 4.846068415371156, 3.641773586306044, 3.9391712251653876, 4.361063940213462, 3.3283375684527057, 3.3471760835941597, 3.2652377881591974, 3.743159929772741, 4.981291662201602, 4.1136187187582065, 3.710298330122175, 3.398733012364021, 4.581966936566409, 4.135729857066328, 3.4875870437888126, 4.315861446066634, 3.3489562216269997, 3.5677857895875187, 5.532275675413409, 4.467077491977087, 3.354968551728525, 5.162565873103582, 4.4113724085939685, 3.3259966748517122, 3.080139219364956, 4.37712941178483, 4.321653434790666, 5.8116977142889334, 4.248043030563612, 4.634109230793916, 3.755341543781859, 4.232720913713794, 4.4373230263368875, 3.5373288401130125, 3.753067541946593, 5.107994467928271, 3.703709687087838, 3.4017887580895625, 3.7166820404002197, 3.477478511872312, 5.271159972924597, 3.9185500461955938, 3.570272481340088, 3.648356694041031, 4.832017644670343, 4.496102872835249, 3.375479815407288, 4.593329950518181, 3.236532222240136, 3.8868091580495787, 4.654156631928212, 3.807955281455994, 3.211429795975937, 4.321208816517622, 4.394126410946309, 2.916641244437966, 3.631347582120466, 3.020182848385463, 4.048299728306069, 4.248043030563612, 4.800715845660676, 4.775593312563134, 3.567661110748682, 3.934824454477356, 4.421474943200809, 3.7596900032465976, 4.00720439929119, 4.080602229227414, 2.9301499225361947, 3.5559236690468814, 3.3068988390064016, 3.3129740057910935, 5.104192922328095, 4.335540173260794, 3.9003990175161416, 3.099814362883594, 4.2276464665513185, 4.409178153986425, 3.347618191967478, 4.1400665110533135, 3.3908161478252565, 4.172158250801878, 4.841400421776575, 4.236755979586986, 3.3415961605570947, 4.86052672543886, 4.87634629931833, 3.4147088239201686, 3.5106350455758206, 3.532212628781479, 4.265543831144893, 4.634109230793916, 4.775593312563134, 6.3161185512667375, 4.3607357543538745, 4.472926823961644, 4.787804168868831, 4.480257059053383, 4.351129672886095, 4.956723805718066, 3.2817180767772847, 4.064608483458253, 3.4630363070355243, 4.082032324226313, 5.456701268308576, 4.425087165705437, 4.207166220406497, 3.648249402047222, 4.762939171146694, 4.59238479787869, 3.7084370569394034, 4.8961512102843665, 2.6004287471585146, 3.077421836311209, 4.2678346017184055, 3.0910429466314566, 3.000485405462325, 4.165866130280626, 4.3970941492352065, 2.995511844750009, 2.6597059216322787, 3.3784887957799734, 3.5161464946579217, 3.755341543781859, 3.5676611107486824, 4.3607357543538745, 4.504732532790424, 3.7786294994658562, 3.506558137892223, 3.2051962359778194, 3.275049853320842, 4.232134097007591, 2.885351767644541, 3.297796466284369, 2.989726470512907, 3.258897345148313, 4.601283551978352, 3.2143148920973594, 3.506895006774462, 3.1650104186493535, 3.8577079292344436, 4.420040964607784, 3.2248880254093124, 4.228615535093103, 3.47787995335279, 3.8000981155967586, 5.064399938356294, 4.64090491578105, 2.943122255088694, 4.508121000863119, 4.690453908817077, 3.1144775076574, 3.4239263410600724, 3.6430306828318626, 4.12528702657051, 4.232720913713794, 3.934824454477356, 4.472926823961644, 3.778629499465856, 5.160690642707333, 4.70288675157755, 3.592106107308394, 3.6150011096851773, 4.974456614987606, 3.5948072939803066, 4.066356141365569, 3.5263453556874835, 4.008246249772924, 5.157227995207579, 3.910049366071169, 3.737927553944732, 3.586808327844562, 4.714437165385775, 4.361707870106988, 4.098479783361043, 4.328992257281945, 3.9441297355021363, 4.764590496557555, 5.787496583752425, 4.762949639787548, 3.5880171934791205, 5.000798950996206, 4.987124888771774, 2.861729278589389, 4.129353706826948, 3.533512253218044, 4.846068415371156, 4.4373230263368875, 4.421474943200809, 4.787804168868831, 3.506558137892223, 4.70288675157755, 6.281630386609974, 3.695682860273643, 4.323695795169043, 4.582339249584636, 3.6999533439715244, 4.389942828045708, 3.496355211553264, 4.02004497778957, 5.6123415073582725, 4.91454946859334, 3.8478620477990098, 3.3830386760358624, 4.817651280938564, 4.415266293875235, 4.377789722768812, 4.769824720234984, 2.690339079959059, 3.404267929293151, 4.061770065163565, 3.4347569627157943, 2.3462117508666482, 3.8538395970471746, 3.872012857764271, 2.44407453417666, 2.9820850873953497, 2.6605862162118035, 3.641773586306044, 3.537328840113012, 3.7596900032465976, 4.480257059053383, 3.2051962359778194, 3.592106107308394, 3.695682860273643, 4.086893915175692, 3.6637315499518297, 3.7972745471459, 2.6123858780303184, 3.260962256638132, 2.5107417878406024, 3.147522234777949, 4.507682555601473, 3.9817157236029717, 3.548071933465432, 2.8485210896754625, 3.4347312974375157, 3.4773243786626478, 2.7884515756799844, 3.46627039278344, 2.7040435571588723, 4.099327726255604, 4.561935395436076, 3.9622911604240936, 3.741867881663351, 4.681209355831753, 5.005936538290864, 2.8462162417036643, 3.9822531680505335, 3.1879783187523127, 3.9391712251653876, 3.7530675419465935, 4.00720439929119, 4.3511296728860955, 3.275049853320842, 3.6150011096851773, 4.323695795169043, 3.6637315499518297, 5.478257310901688, 4.19993313888363, 3.614452539945133, 3.6089548995618363, 2.700334364077501, 3.7689206389182917, 5.517721253756119, 4.810875315673601, 3.739654328170675, 3.355587862975343, 3.1178505116500475, 4.432696657662017, 3.6115104635959305, 4.45407178999449, 4.097516198078844, 3.993442101897572, 5.338178627385598, 4.9414617708424755, 3.5815075799994185, 5.205088044633326, 5.135590050457474, 3.6254255401247812, 3.555341926595753, 4.73475228265243, 4.361063940213462, 5.107994467928271, 4.080602229227414, 4.956723805718066, 4.232134097007591, 4.974456614987606, 4.582339249584636, 3.7972745471459, 4.19993313888363, 6.391155065398847, 4.205786901340056, 4.345539543307413, 3.9040138628914605, 4.630881140699793, 5.378053961665847, 4.0485975810778, 3.985507482186319, 4.188519410754106, 4.79323310700591, 4.805461546372272, 4.142294602254569, 4.997091818139618, 2.689397904612436, 3.2371606240613864, 4.631982669583265, 3.5718883848046374, 2.977398687181556, 4.656066608350676, 4.430956864154369, 2.4228595252895717, 3.232590122945775, 3.558553284103746, 3.3283375684527057, 3.7037096870878377, 2.9301499225361947, 3.281718076777285, 2.885351767644541, 3.5948072939803066, 3.6999533439715244, 2.6123858780303184, 3.614452539945133, 4.205786901340056, 4.461577543541839, 2.768835855171012, 2.800749837488376, 3.5029818891504254, 4.703943150080182, 3.3498285948948254, 2.8376627557852707, 3.620245352174845, 3.6054379243286756, 3.6884397530429562, 3.83220999148105, 4.236864571331281, 3.1757221504502433, 3.389573370397508, 4.04817911502475, 4.18403299118855, 3.073852305190852, 3.763668449755257, 4.282234094786728, 2.8049680051424444, 3.068187106435216, 3.37039024073841, 3.3471760835941597, 3.401788758089563, 3.5559236690468814, 4.064608483458253, 3.297796466284369, 4.066356141365569, 4.389942828045708, 3.260962256638132, 3.608954899561836, 4.345539543307413, 2.768835855171012, 4.778666558950309, 2.7252758569408844, 4.139799353641271, 4.358587299358624, 4.336419723335723, 3.463399986647839, 3.2431942050084186, 3.599032326588088, 3.656256519712694, 3.4610318583337536, 4.012940929905369, 2.7840089755113824, 2.7108312600028444, 4.074773528636097, 3.2526585082487345, 2.647727588076209, 3.718676876594824, 3.4980695116273193, 2.6415721529678855, 2.641712330539264, 3.1129856041752295, 3.265237788159197, 3.7166820404002197, 3.3068988390064016, 3.463036307035525, 2.989726470512907, 3.526345355687484, 3.496355211553264, 2.5107417878406024, 2.7003343640775013, 3.904013862891461, 2.8007498374883757, 2.7252758569408844, 3.437434769935966, 2.721578934269042, 3.889481141612586, 2.803310271841279, 2.7204072641058397, 2.820035885184703, 3.939030619721151, 3.6190065375626665, 3.008567058328858, 3.5147109177202065, 3.2822533215778864, 3.7189728972187868, 4.0556967255953875, 4.375478116829063, 3.1846864162261803, 4.4137552971329965, 4.651259656732731, 3.0381755089847204, 3.563373999945094, 4.165794870909169, 3.743159929772741, 3.477478511872312, 3.3129740057910935, 4.082032324226313, 3.2588973451483128, 4.008246249772924, 4.02004497778957, 3.147522234777949, 3.768920638918291, 4.630881140699793, 3.5029818891504254, 4.139799353641271, 2.721578934269042, 5.007191587031482, 4.587256368407053, 4.295549772520548, 3.428474472825706, 4.0557837598725754, 3.8107006803081807, 3.6694459006224625, 3.8238305401306754, 4.646115194066262, 3.5861682887660598, 4.832988078372525, 6.547668531149082, 5.019622247849673, 4.243204991826781, 5.972371664188578, 6.098362448110332, 3.703255012091683, 4.621924180487924, 4.107268173069444, 4.981291662201602, 5.271159972924597, 5.104192922328095, 5.456701268308576, 4.601283551978352, 5.157227995207579, 5.6123415073582725, 4.507682555601473, 5.517721253756119, 5.378053961665848, 4.703943150080182, 4.358587299358624, 3.889481141612586, 4.587256368407053, 7.4682073838626275, 5.786947450753669, 4.584325682050921, 4.465525005563425, 5.070608638207554, 5.700669236606462, 4.862583198804054, 5.829303490547293, 3.2593594395795438, 4.0438269034720635, 5.060337202052291, 4.328262826144412, 3.808550080377437, 4.870960893115659, 4.902641138451369, 3.0862509694866507, 4.029114523703862, 3.3972906236575535, 4.1136187187582065, 3.9185500461955938, 4.335540173260794, 4.425087165705437, 3.2143148920973594, 3.910049366071169, 4.91454946859334, 3.981715723602971, 4.810875315673601, 4.0485975810778, 3.3498285948948254, 4.336419723335723, 2.8033102718412795, 4.295549772520548, 5.786947450753669, 5.9708576015969514, 3.862990457206838, 3.688856786458443, 3.8123793851787267, 4.263857914345461, 3.793624490926946, 4.56651781737832, 2.856336126475509, 3.6370219756411086, 4.1585976592724805, 3.5869724275885746, 3.0483752633295884, 4.0940154113020935, 4.002140716219812, 2.7427512583789655, 3.667069952178138, 3.0254360330349517, 3.710298330122175, 3.570272481340088, 3.9003990175161416, 4.207166220406497, 3.5068950067744624, 3.737927553944732, 3.84786204779901, 3.548071933465432, 3.739654328170675, 3.985507482186319, 2.8376627557852707, 3.463399986647839, 2.7204072641058397, 3.4284744728257066, 4.584325682050921, 3.8629904572068376, 4.227355585470887, 2.9691756342532503, 3.417068845143175, 4.075641623192482, 3.406268092268903, 4.115988350722507, 2.875790806724925, 3.0695472337984353, 4.179632273577691, 3.7991926581927, 3.184050092366138, 4.256499943049193, 4.180661781965751, 3.0183933997984527, 3.017108598729165, 3.820546211777247, 3.3987330123640214, 3.648356694041031, 3.099814362883594, 3.6482494020472225, 3.1650104186493535, 3.5868083278445626, 3.3830386760358624, 2.8485210896754625, 3.355587862975343, 4.188519410754106, 3.620245352174845, 3.243194205008418, 2.820035885184703, 4.0557837598725754, 4.465525005563425, 3.688856786458443, 2.9691756342532503, 4.151691400203102, 3.764884638050069, 3.625803929274025, 3.5481220676177156, 4.376103742915374, 3.5697944697713715, 3.891168388678108, 5.390197171213057, 4.495736555337369, 2.9819193805283186, 4.935598187319021, 4.531866821353604, 3.1416872372889757, 3.4655186480439966, 4.118197397914825, 4.581966936566409, 4.832017644670343, 4.2276464665513185, 4.762939171146694, 3.8577079292344436, 4.714437165385775, 4.817651280938564, 3.4347312974375153, 3.117850511650048, 4.79323310700591, 3.6054379243286756, 3.599032326588088, 3.939030619721151, 3.8107006803081807, 5.070608638207554, 3.8123793851787267, 3.4170688451431754, 3.7648846380500696, 5.825676096413891, 4.34505859006525, 3.9109971626599247, 4.6718773894135035, 3.0718633171796257, 4.002699656759023, 5.109426074778116, 4.1338105680488315, 4.130140010902999, 5.193115956877178, 5.19681235887693, 3.4657503224322217, 3.7571439035724405, 3.88092917593326, 4.135729857066328, 4.496102872835249, 4.409178153986424, 4.59238479787869, 4.420040964607785, 4.361707870106988, 4.415266293875235, 3.4773243786626473, 4.432696657662016, 4.805461546372272, 3.6884397530429562, 3.656256519712694, 3.619006537562667, 3.6694459006224625, 5.700669236606462, 4.263857914345462, 4.075641623192482, 3.6258039292740247, 4.34505859006525, 5.70044338947371, 4.048640396071456, 4.818445249885493, 2.8639288036044017, 3.686659336383218, 4.47639612115159, 3.8378506208796788, 3.3669248875223405, 4.538659358985781, 4.420638184264228, 2.84541646065416, 3.7973654368338905, 3.319431850310794, 3.4875870437888126, 3.375479815407288, 3.347618191967478, 3.7084370569394034, 3.2248880254093124, 4.098479783361043, 4.377789722768812, 2.7884515756799844, 3.61151046359593, 4.142294602254569, 3.83220999148105, 3.4610318583337536, 3.0085670583288575, 3.8238305401306754, 4.862583198804054, 3.793624490926946, 3.4062680922689035, 3.5481220676177156, 3.910997162659925, 4.048640396071456, 4.453197123559195, 4.5010891645489, 3.4953216860726917, 4.420883318787573, 5.501184786945122, 4.647450240523169, 4.209854238756024, 5.467162237890344, 5.239797812982575, 3.1309563860597294, 4.429482895305322, 4.543079325847466, 4.315861446066634, 4.593329950518181, 4.140066511053313, 4.8961512102843665, 4.228615535093103, 4.328992257281945, 4.7698247202349835, 3.4662703927834397, 4.45407178999449, 4.9970918181396184, 4.236864571331281, 4.012940929905369, 3.514710917720206, 4.646115194066262, 5.829303490547293, 4.56651781737832, 4.115988350722507, 4.376103742915374, 4.6718773894135035, 4.818445249885493, 4.5010891645489, 6.4605565465606265
};

void cmsis_init_cholesky(float32_t *f,const int r,const int c)
{
   if ((r==4) && (c==4))
   {
      memcpy(f,random_sdp_4x4_f32,sizeof(float32_t)*r*c);
   }
   if ((r==16) && (c==16))
   {
      memcpy(f,random_sdp_16x16_f32,sizeof(float32_t)*r*c);
   }
   if ((r==32) && (c==32))
   {
      memcpy(f,random_sdp_32x32_f32,sizeof(float32_t)*r*c);
   }
}

void cmsis_init_cholesky(float64_t *f,const int r,const int c)
{
   if ((r==4) && (c==4))
   {
      copy_f32_to_f64(f,(float32_t*)random_sdp_4x4_f32,r*c);
   }
   if ((r==16) && (c==16))
   {
      copy_f32_to_f64(f,(float32_t*)random_sdp_16x16_f32,r*c);
   }
   if ((r==32) && (c==32))
   {
      copy_f32_to_f64(f,(float32_t*)random_sdp_32x32_f32,r*c);
   }
}

#if !defined(DISABLEFLOAT16)
void cmsis_init_cholesky(float16_t *f,const int r,const int c)
{
   if ((r==4) && (c==4))
   {
      copy_f32_to_f16(f,(float32_t*)random_sdp_4x4_f32,r*c);
   }
   if ((r==16) && (c==16))
   {
      copy_f32_to_f16(f,(float32_t*)random_sdp_16x16_f32,r*c);
   }
   if ((r==32) && (c==32))
   {
      copy_f32_to_f16(f,(float32_t*)random_sdp_32x32_f32,r*c);
   }
}
#endif

void cmsis_mat_mult(const arm_matrix_instance_f64* a, 
                    const arm_matrix_instance_f64* b, 
                          arm_matrix_instance_f64 *c,
                          float64_t * pState=nullptr)
{
   (void)pState;
   arm_mat_mult_f64(a,b,c);
}

void cmsis_cmplx_mat_mult(const arm_matrix_instance_f32* a, 
                    const arm_matrix_instance_f32* b, 
                          arm_matrix_instance_f32 *c,
                          float32_t * pState=nullptr)
{
   (void)pState;
   arm_mat_cmplx_mult_f32(a,b,c);
}

void cmsis_mat_mult(const arm_matrix_instance_f32* a, 
                    const arm_matrix_instance_f32* b, 
                          arm_matrix_instance_f32 *c,
                          float32_t * pState=nullptr)
{
   (void)pState;
   arm_mat_mult_f32(a,b,c);
}

#if !defined(DISABLEFLOAT16)
void cmsis_cmplx_mat_mult(const arm_matrix_instance_f16* a, 
                    const arm_matrix_instance_f16* b, 
                          arm_matrix_instance_f16 *c,
                          float16_t * pState=nullptr)
{
   (void)pState;
   arm_mat_cmplx_mult_f16(a,b,c);
}
void cmsis_mat_mult(const arm_matrix_instance_f16* a, 
                    const arm_matrix_instance_f16* b, 
                          arm_matrix_instance_f16 *c,
                          float16_t * pState=nullptr)
{
   (void)pState;
   arm_mat_mult_f16(a,b,c);
}
#endif

void cmsis_mat_mult(const arm_matrix_instance_q7* a, 
                    const arm_matrix_instance_q7* b, 
                          arm_matrix_instance_q7 *c,
                          q7_t *pState)
{
   arm_mat_mult_q7(a,b,c,pState);
}

void cmsis_cmplx_mat_mult(const arm_matrix_instance_q15* a, 
                    const arm_matrix_instance_q15* b, 
                          arm_matrix_instance_q15 *c,
                          q15_t *pState)
{
   arm_mat_cmplx_mult_q15(a,b,c,pState);
}

void cmsis_mat_mult(const arm_matrix_instance_q15* a, 
                    const arm_matrix_instance_q15* b, 
                          arm_matrix_instance_q15 *c,
                          q15_t *pState)
{
   arm_mat_mult_q15(a,b,c,pState);
}

void cmsis_cmplx_mat_mult(const arm_matrix_instance_q31* a, 
                    const arm_matrix_instance_q31* b, 
                          arm_matrix_instance_q31 *c,
                          q31_t *pState)
{
   (void)pState;
   arm_mat_cmplx_mult_q31(a,b,c);
}

void cmsis_mat_mult(const arm_matrix_instance_q31* a, 
                    const arm_matrix_instance_q31* b, 
                          arm_matrix_instance_q31 *c,
                          q31_t *pState)
{
   (void)pState;
   arm_mat_mult_q31(a,b,c);
}

void cmsis_mat_trans(const arm_matrix_instance_q7* a, 
                            arm_matrix_instance_q7* b)
{
   arm_mat_trans_q7(a,b);

}

void cmsis_cmplx_mat_trans(const arm_matrix_instance_q15* a, 
                            arm_matrix_instance_q15* b)
{
   arm_mat_cmplx_trans_q15(a,b);

}

void cmsis_mat_trans(const arm_matrix_instance_q15* a, 
                            arm_matrix_instance_q15* b)
{
   arm_mat_trans_q15(a,b);

}

void cmsis_cmplx_mat_trans(const arm_matrix_instance_q31* a, 
                            arm_matrix_instance_q31* b)
{
   arm_mat_cmplx_trans_q31(a,b);

}

void cmsis_mat_trans(const arm_matrix_instance_q31* a, 
                            arm_matrix_instance_q31* b)
{
   arm_mat_trans_q31(a,b);

}

void cmsis_mat_trans(const arm_matrix_instance_f64* a, 
                            arm_matrix_instance_f64* b)
{
   arm_mat_trans_f64(a,b);
}

void cmsis_cmplx_mat_trans(const arm_matrix_instance_f32* a, 
                            arm_matrix_instance_f32* b)
{
   arm_mat_cmplx_trans_f32(a,b);
}

void cmsis_mat_trans(const arm_matrix_instance_f32* a, 
                            arm_matrix_instance_f32* b)
{
   arm_mat_trans_f32(a,b);
}

#if !defined(DISABLEFLOAT16)
void cmsis_cmplx_mat_trans(const arm_matrix_instance_f16* a, 
                            arm_matrix_instance_f16* b)
{
   arm_mat_cmplx_trans_f16(a,b);
}

void cmsis_mat_trans(const arm_matrix_instance_f16* a, 
                            arm_matrix_instance_f16* b)
{
   arm_mat_trans_f16(a,b);
}
#endif

float64_t cmsis_householder(const float64_t *src,float64_t* dst,uint32_t nb)
{
return arm_householder_f64(src,DEFAULT_HOUSEHOLDER_THRESHOLD_F64,nb,dst);
}


float32_t cmsis_householder(const float32_t *src,float32_t* dst,uint32_t nb)
{
return arm_householder_f32(src,DEFAULT_HOUSEHOLDER_THRESHOLD_F32,nb,dst);
}

#if !defined(DISABLEFLOAT16)
float16_t cmsis_householder(const float16_t *src,float16_t* dst,uint32_t nb)
{
return arm_householder_f16(src,DEFAULT_HOUSEHOLDER_THRESHOLD_F16,nb,dst);

}
#endif

arm_status cmsis_qr(
    const arm_matrix_instance_f64 * pSrc,
    const float64_t threshold,
    arm_matrix_instance_f64 * pOutR,
    arm_matrix_instance_f64 * pOutQ,
    float64_t * pOutTau,
    float64_t *pTmpA,
    float64_t *pTmpB
    )
{
return arm_mat_qr_f64(pSrc,threshold,
                       pOutR,pOutQ,
                       pOutTau,
                       pTmpA,
                       pTmpB);
}

arm_status cmsis_qr(
    const arm_matrix_instance_f32 * pSrc,
    const float32_t threshold,
    arm_matrix_instance_f32 * pOutR,
    arm_matrix_instance_f32 * pOutQ,
    float32_t * pOutTau,
    float32_t *pTmpA,
    float32_t *pTmpB
    )
{
return arm_mat_qr_f32(pSrc,threshold,
                       pOutR,pOutQ,
                       pOutTau,
                       pTmpA,
                       pTmpB);
}

#if !defined(DISABLEFLOAT16)
arm_status cmsis_qr(
    const arm_matrix_instance_f16 * pSrc,
    const float16_t threshold,
    arm_matrix_instance_f16 * pOutR,
    arm_matrix_instance_f16 * pOutQ,
    float16_t * pOutTau,
    float16_t *pTmpA,
    float16_t *pTmpB
    )
{
return arm_mat_qr_f16(pSrc,threshold,
                       pOutR,pOutQ,
                       pOutTau,
                       pTmpA,
                       pTmpB);
}
#endif

arm_status cmsis_cholesky(
  const arm_matrix_instance_f64 * src,
  arm_matrix_instance_f64 * dst)
{
  return arm_mat_cholesky_f64(src,dst);
}

arm_status cmsis_cholesky(
  const arm_matrix_instance_f32 * src,
  arm_matrix_instance_f32 * dst)
{
  return arm_mat_cholesky_f32(src,dst);
}

#if !defined(DISABLEFLOAT16)
arm_status cmsis_cholesky(
  const arm_matrix_instance_f16 * src,
  arm_matrix_instance_f16 * dst)
{
  return arm_mat_cholesky_f16(src,dst);

}
#endif


void cmsis_mat_vec_mult(
  const arm_matrix_instance_f32 *pSrcMat, 
  const float32_t *pVec, 
  float32_t *pDst)
{
arm_mat_vec_mult_f32(pSrcMat, pVec, pDst);
}

#if !defined(DISABLEFLOAT16)
void cmsis_mat_vec_mult(
  const arm_matrix_instance_f16 *pSrcMat, 
  const float16_t *pVec, 
  float16_t *pDst)
{
  arm_mat_vec_mult_f16(pSrcMat, pVec, pDst);
 
}
#endif

void cmsis_mat_vec_mult(
  const arm_matrix_instance_q31 *pSrcMat, 
  const Q31 *pVec, 
  Q31 *pDst)
{
arm_mat_vec_mult_q31(pSrcMat, 
   reinterpret_cast<const q31_t*>(pVec), 
   reinterpret_cast<q31_t*>(pDst));
}

void cmsis_mat_vec_mult(
  const arm_matrix_instance_q15 *pSrcMat, 
  const Q15 *pVec, 
  Q15 *pDst)
{
arm_mat_vec_mult_q15(pSrcMat, 
   reinterpret_cast<const q15_t*>(pVec), 
   reinterpret_cast<q15_t*>(pDst));
}

void cmsis_mat_vec_mult(
  const arm_matrix_instance_q7 *pSrcMat, 
  const Q7 *pVec, 
  Q7 *pDst)
{
arm_mat_vec_mult_q7(pSrcMat, 
   reinterpret_cast<const q7_t*>(pVec), 
   reinterpret_cast<q7_t*>(pDst));
}

extern void cmsis_complicated_mat_vec(
  const arm_matrix_instance_f32 * src,
  const float32_t * a,
  const float32_t * b,
  const float32_t scalar,
  float32_t * tmp,
  float32_t * dst)
{
   arm_scale_f32(b,scalar,tmp,src->numCols);
   arm_add_f32(a,tmp,tmp,src->numCols);
   arm_mat_vec_mult_f32(src, tmp, dst);
}

#if !defined(DISABLEFLOAT16)
extern void cmsis_complicated_mat_vec(
  const arm_matrix_instance_f16 * src,
  const float16_t * a,
  const float16_t * b,
  const float16_t scalar,
  float16_t * tmp,
  float16_t * dst)
{
   arm_scale_f16(b,scalar,tmp,src->numCols);
   arm_add_f16(a,tmp,tmp,src->numCols);
   arm_mat_vec_mult_f16(src, tmp, dst);
}
#endif

extern void cmsis_complicated_mat_vec(
  const arm_matrix_instance_q31 * src,
  const Q31 * a,
  const Q31 * b,
  const Q31 scalar,
  Q31 * tmp,
  Q31 * dst)
{
   arm_scale_q31(reinterpret_cast<const q31_t*>(b),
                 scalar.v,0,
                 reinterpret_cast<q31_t*>(tmp),src->numCols);
   arm_add_q31(reinterpret_cast<const q31_t*>(a),
               reinterpret_cast<const q31_t*>(tmp),
               reinterpret_cast<q31_t*>(tmp),src->numCols);
   arm_mat_vec_mult_q31(src, 
                        reinterpret_cast<const q31_t*>(tmp), 
                        reinterpret_cast<q31_t*>(dst));
}

extern void cmsis_complicated_mat_vec(
  const arm_matrix_instance_q15 * src,
  const Q15 * a,
  const Q15 * b,
  const Q15 scalar,
  Q15 * tmp,
  Q15 * dst)
{
   arm_scale_q15(reinterpret_cast<const q15_t*>(b),
                 scalar.v,0,
                 reinterpret_cast<q15_t*>(tmp),src->numCols);
   arm_add_q15(reinterpret_cast<const q15_t*>(a),
               reinterpret_cast<const q15_t*>(tmp),
               reinterpret_cast<q15_t*>(tmp),src->numCols);
   arm_mat_vec_mult_q15(src, 
                        reinterpret_cast<const q15_t*>(tmp), 
                        reinterpret_cast<q15_t*>(dst));
}

extern void cmsis_complicated_mat_vec(
  const arm_matrix_instance_q7 * src,
  const Q7 * a,
  const Q7 * b,
  const Q7 scalar,
  Q7 * tmp,
  Q7 * dst)
{
   arm_scale_q7(reinterpret_cast<const q7_t*>(b),
                 scalar.v,0,
                 reinterpret_cast<q7_t*>(tmp),src->numCols);
   arm_add_q7(reinterpret_cast<const q7_t*>(a),
               reinterpret_cast<const q7_t*>(tmp),
               reinterpret_cast<q7_t*>(tmp),src->numCols);
   arm_mat_vec_mult_q7(src, 
                        reinterpret_cast<const q7_t*>(tmp), 
                        reinterpret_cast<q7_t*>(dst));
}