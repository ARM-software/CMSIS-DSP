cmake_minimum_required (VERSION 3.14)



if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE InterpolationFunctions/InterpolationFunctions.c)

  if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
    target_sources(CMSISDSP PRIVATE InterpolationFunctions/InterpolationFunctionsF16.c)
  endif()

else()

target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_bilinear_interp_f32.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_bilinear_interp_q15.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_bilinear_interp_q31.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_bilinear_interp_q7.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_linear_interp_f32.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_linear_interp_q15.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_linear_interp_q31.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_linear_interp_q7.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_spline_interp_f32.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_spline_interp_init_f32.c)



if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_bilinear_interp_f16.c)
target_sources(CMSISDSP PRIVATE InterpolationFunctions/arm_linear_interp_f16.c)
endif()

endif()