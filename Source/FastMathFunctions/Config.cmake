cmake_minimum_required (VERSION 3.14)

if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE FastMathFunctions/FastMathFunctions.c)

  if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
    target_sources(CMSISDSP PRIVATE FastMathFunctions/FastMathFunctionsF16.c)
  endif()

else()

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_cos_f32.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_cos_q15.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_cos_q31.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_sin_f32.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_sin_q15.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_sin_q31.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_sqrt_q31.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_sqrt_q15.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vlog_f32.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vlog_f64.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vexp_f32.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vexp_f64.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vlog_q31.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vlog_q15.c)


if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vlog_f16.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vexp_f16.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_vinverse_f16.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_atan2_f16.c)
endif()

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_divide_q15.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_divide_q31.c)

target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_atan2_f32.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_atan2_q31.c)
target_sources(CMSISDSP PRIVATE FastMathFunctions/arm_atan2_q15.c)


endif()


