cmake_minimum_required (VERSION 3.14)


if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE ControllerFunctions/ControllerFunctions.c)


else()

target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_init_f32.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_init_q15.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_init_q31.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_reset_f32.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_reset_q15.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_reset_q31.c)

target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_sin_cos_f32.c)

target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_sin_cos_q31.c)

endif()

