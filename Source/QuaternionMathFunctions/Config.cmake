cmake_minimum_required (VERSION 3.14)



if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/QuaternionMathFunctions.c)


else()


target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_quaternion_norm_f32.c)
target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_quaternion_inverse_f32.c)
target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_quaternion_conjugate_f32.c)
target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_quaternion_normalize_f32.c)
target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_quaternion_product_single_f32.c)
target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_quaternion_product_f32.c)
target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_quaternion2rotation_f32.c)
target_sources(CMSISDSP PRIVATE QuaternionMathFunctions/arm_rotation2quaternion_f32.c)


if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
endif()
endif()




