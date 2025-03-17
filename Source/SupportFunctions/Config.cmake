cmake_minimum_required (VERSION 3.14)


if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE SupportFunctions/SupportFunctions.c)

  if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
    target_sources(CMSISDSP PRIVATE SupportFunctions/SupportFunctionsF16.c)
  endif()

else()

target_sources(CMSISDSP PRIVATE SupportFunctions/arm_barycenter_f32.c
SupportFunctions/arm_bitonic_sort_f32.c
SupportFunctions/arm_bubble_sort_f32.c
SupportFunctions/arm_copy_f32.c
SupportFunctions/arm_copy_f64.c
SupportFunctions/arm_copy_q15.c
SupportFunctions/arm_copy_q31.c
SupportFunctions/arm_copy_q7.c
SupportFunctions/arm_f16_to_float.c
SupportFunctions/arm_f16_to_q15.c
SupportFunctions/arm_fill_f32.c
SupportFunctions/arm_fill_f64.c
SupportFunctions/arm_fill_q15.c
SupportFunctions/arm_fill_q31.c
SupportFunctions/arm_fill_q7.c
SupportFunctions/arm_f64_to_float.c
SupportFunctions/arm_f64_to_q31.c
SupportFunctions/arm_f64_to_q15.c
SupportFunctions/arm_f64_to_q7.c
SupportFunctions/arm_float_to_f64.c
SupportFunctions/arm_float_to_q15.c
SupportFunctions/arm_float_to_q31.c
SupportFunctions/arm_float_to_q7.c
SupportFunctions/arm_heap_sort_f32.c
SupportFunctions/arm_insertion_sort_f32.c
SupportFunctions/arm_merge_sort_f32.c
SupportFunctions/arm_merge_sort_init_f32.c
SupportFunctions/arm_q15_to_f64.c
SupportFunctions/arm_q15_to_float.c
SupportFunctions/arm_q15_to_q31.c
SupportFunctions/arm_q15_to_q7.c
SupportFunctions/arm_q31_to_f64.c
SupportFunctions/arm_q31_to_float.c
SupportFunctions/arm_q31_to_q15.c
SupportFunctions/arm_q31_to_q7.c
SupportFunctions/arm_q7_to_f64.c
SupportFunctions/arm_q7_to_float.c
SupportFunctions/arm_q7_to_q15.c
SupportFunctions/arm_q7_to_q31.c
SupportFunctions/arm_quick_sort_f32.c
SupportFunctions/arm_selection_sort_f32.c
SupportFunctions/arm_sort_f32.c
SupportFunctions/arm_sort_init_f32.c
SupportFunctions/arm_weighted_average_f32.c
)




if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_copy_f16.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_fill_f16.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_f16_to_q15.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_q15_to_f16.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_float_to_f16.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_f16_to_float.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_weighted_average_f16.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_barycenter_f16.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_f16_to_f64.c)
target_sources(CMSISDSP PRIVATE SupportFunctions/arm_f64_to_f16.c)
endif()
endif()
