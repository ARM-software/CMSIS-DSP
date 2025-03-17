cmake_minimum_required (VERSION 3.14)

if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE WindowFunctions/WindowFunctions.c)

else()

set(SRCF64 WindowFunctions/arm_welch_f64.c
WindowFunctions/arm_bartlett_f64.c
WindowFunctions/arm_hamming_f64.c
WindowFunctions/arm_hanning_f64.c
WindowFunctions/arm_nuttall3_f64.c
WindowFunctions/arm_nuttall4_f64.c
WindowFunctions/arm_nuttall3a_f64.c
WindowFunctions/arm_nuttall3b_f64.c
WindowFunctions/arm_nuttall4a_f64.c
WindowFunctions/arm_blackman_harris_92db_f64.c
WindowFunctions/arm_nuttall4b_f64.c
WindowFunctions/arm_nuttall4c_f64.c
WindowFunctions/arm_hft90d_f64.c
WindowFunctions/arm_hft95_f64.c
WindowFunctions/arm_hft116d_f64.c
WindowFunctions/arm_hft144d_f64.c
WindowFunctions/arm_hft169d_f64.c
WindowFunctions/arm_hft196d_f64.c
WindowFunctions/arm_hft223d_f64.c
WindowFunctions/arm_hft248d_f64.c
)

set(SRCF32 WindowFunctions/arm_welch_f32.c
WindowFunctions/arm_bartlett_f32.c
WindowFunctions/arm_hamming_f32.c
WindowFunctions/arm_hanning_f32.c
WindowFunctions/arm_nuttall3_f32.c
WindowFunctions/arm_nuttall4_f32.c
WindowFunctions/arm_nuttall3a_f32.c
WindowFunctions/arm_nuttall3b_f32.c
WindowFunctions/arm_nuttall4a_f32.c
WindowFunctions/arm_blackman_harris_92db_f32.c
WindowFunctions/arm_nuttall4b_f32.c
WindowFunctions/arm_nuttall4c_f32.c
WindowFunctions/arm_hft90d_f32.c
WindowFunctions/arm_hft95_f32.c
WindowFunctions/arm_hft116d_f32.c
WindowFunctions/arm_hft144d_f32.c
WindowFunctions/arm_hft169d_f32.c
WindowFunctions/arm_hft196d_f32.c
WindowFunctions/arm_hft223d_f32.c
WindowFunctions/arm_hft248d_f32.c
)


target_sources(CMSISDSP PRIVATE ${SRCF64})
target_sources(CMSISDSP PRIVATE ${SRCF32})


endif()