cmake_minimum_required (VERSION 3.14)


set(SRCF64 BasicMathFunctions/arm_abs_f64.c
BasicMathFunctions/arm_add_f64.c
BasicMathFunctions/arm_dot_prod_f64.c
BasicMathFunctions/arm_mult_f64.c
BasicMathFunctions/arm_negate_f64.c
BasicMathFunctions/arm_offset_f64.c
BasicMathFunctions/arm_scale_f64.c
BasicMathFunctions/arm_sub_f64.c
)

set(SRCF32 BasicMathFunctions/arm_abs_f32.c
BasicMathFunctions/arm_add_f32.c
BasicMathFunctions/arm_clip_f32.c
BasicMathFunctions/arm_dot_prod_f32.c
BasicMathFunctions/arm_mult_f32.c
BasicMathFunctions/arm_negate_f32.c
BasicMathFunctions/arm_offset_f32.c
BasicMathFunctions/arm_scale_f32.c
BasicMathFunctions/arm_sub_f32.c
)

set(SRCF16 BasicMathFunctions/arm_abs_f16.c
BasicMathFunctions/arm_add_f16.c
BasicMathFunctions/arm_clip_f16.c
BasicMathFunctions/arm_dot_prod_f16.c
BasicMathFunctions/arm_mult_f16.c
BasicMathFunctions/arm_negate_f16.c
BasicMathFunctions/arm_offset_f16.c
BasicMathFunctions/arm_scale_f16.c
BasicMathFunctions/arm_sub_f16.c
)

set(SRCQ31 BasicMathFunctions/arm_abs_q31.c
BasicMathFunctions/arm_add_q31.c
BasicMathFunctions/arm_clip_q31.c
BasicMathFunctions/arm_dot_prod_q31.c
BasicMathFunctions/arm_mult_q31.c
BasicMathFunctions/arm_negate_q31.c
BasicMathFunctions/arm_offset_q31.c
BasicMathFunctions/arm_scale_q31.c
BasicMathFunctions/arm_shift_q31.c
BasicMathFunctions/arm_sub_q31.c
)

set(SRCQ15 BasicMathFunctions/arm_abs_q15.c
BasicMathFunctions/arm_add_q15.c
BasicMathFunctions/arm_clip_q15.c
BasicMathFunctions/arm_dot_prod_q15.c
BasicMathFunctions/arm_mult_q15.c
BasicMathFunctions/arm_negate_q15.c
BasicMathFunctions/arm_offset_q15.c
BasicMathFunctions/arm_scale_q15.c
BasicMathFunctions/arm_shift_q15.c
BasicMathFunctions/arm_sub_q15.c
)

set(SRCQ7  BasicMathFunctions/arm_abs_q7.c
BasicMathFunctions/arm_add_q7.c
BasicMathFunctions/arm_clip_q7.c
BasicMathFunctions/arm_dot_prod_q7.c
BasicMathFunctions/arm_mult_q7.c
BasicMathFunctions/arm_negate_q7.c
BasicMathFunctions/arm_offset_q7.c
BasicMathFunctions/arm_scale_q7.c
BasicMathFunctions/arm_shift_q7.c
BasicMathFunctions/arm_sub_q7.c
)

set(SRCU32 BasicMathFunctions/arm_and_u32.c
BasicMathFunctions/arm_not_u32.c
BasicMathFunctions/arm_or_u32.c
BasicMathFunctions/arm_xor_u32.c
)

set(SRCU16 BasicMathFunctions/arm_and_u16.c
BasicMathFunctions/arm_not_u16.c
BasicMathFunctions/arm_or_u16.c
BasicMathFunctions/arm_xor_u16.c
)

set(SRCU8  BasicMathFunctions/arm_and_u8.c  
BasicMathFunctions/arm_or_u8.c
BasicMathFunctions/arm_not_u8.c  
BasicMathFunctions/arm_xor_u8.c)

target_sources(CMSISDSP PRIVATE ${SRCF64})
target_sources(CMSISDSP PRIVATE ${SRCF32})

if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE ${SRCF16})
endif()

target_sources(CMSISDSP PRIVATE ${SRCQ31})
target_sources(CMSISDSP PRIVATE ${SRCQ15})
target_sources(CMSISDSP PRIVATE ${SRCQ7})

target_sources(CMSISDSP PRIVATE ${SRCU32})
target_sources(CMSISDSP PRIVATE ${SRCU16})
target_sources(CMSISDSP PRIVATE ${SRCU8})


