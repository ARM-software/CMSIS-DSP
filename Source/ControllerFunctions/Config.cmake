cmake_minimum_required (VERSION 3.14)





if (CONFIGTABLE AND ALLFAST)
    target_compile_definitions(CMSISDSP PUBLIC ARM_ALL_FAST_TABLES)  
endif()

target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_init_f32.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_init_q15.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_init_q31.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_reset_f32.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_reset_q15.c)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_pid_reset_q31.c)

if (NOT CONFIGTABLE OR ALLFAST OR ARM_SIN_COS_F32)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_sin_cos_f32.c)
endif()

if (NOT CONFIGTABLE OR ALLFAST OR ARM_SIN_COS_Q31)
target_sources(CMSISDSP PRIVATE ControllerFunctions/arm_sin_cos_q31.c)
endif()



