cmake_minimum_required (VERSION 3.14)

if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE ComplexMathFunctions/ComplexMathFunctions.c)

  if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
    target_sources(CMSISDSP PRIVATE ComplexMathFunctions/ComplexMathFunctionsF16.c)
  endif()

else()


# MVE code is using a table for computing the fast sqrt arm_cmplx_mag_q31
# There is the possibility of not compiling this function and not including
# the table.
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_q31.c)

target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_q15.c)

target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_fast_q15.c)

target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_conj_f32.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_conj_q15.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_conj_q31.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_dot_prod_f32.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_dot_prod_q15.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_dot_prod_q31.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_f32.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_f64.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_squared_f32.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_squared_f64.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_squared_q15.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_squared_q31.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_cmplx_f64.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_real_f32.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_real_q15.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_real_q31.c)

if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_conj_f16.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_dot_prod_f16.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_f16.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mag_squared_f16.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_cmplx_f16.c)
target_sources(CMSISDSP PRIVATE ComplexMathFunctions/arm_cmplx_mult_real_f16.c)
endif()

endif()