cmake_minimum_required (VERSION 3.14)



target_sources(CMSISDSP PRIVATE CommonTables/arm_common_tables.c 
    CommonTables/arm_common_tables_f16.c)


target_sources(CMSISDSP PRIVATE CommonTables/arm_const_structs.c)
target_sources(CMSISDSP PRIVATE CommonTables/arm_const_structs_f16.c)



if (NEON OR NEONEXPERIMENTAL)
    target_sources(CMSISDSP PRIVATE "${DSP}/ComputeLibrary/Source/arm_cl_tables.c")
endif()

if (HELIUM OR MVEF)
    target_sources(CMSISDSP PRIVATE "CommonTables/arm_mve_tables.c")
    target_sources(CMSISDSP PRIVATE "CommonTables/arm_mve_tables_f16.c")
endif()


if (WRAPPER)
    target_compile_definitions(CMSISDSP PUBLIC ARM_TABLE_BITREV_1024)
    target_compile_definitions(CMSISDSP PUBLIC ARM_TABLE_TWIDDLECOEF_F32_4096)
    target_compile_definitions(CMSISDSP PUBLIC ARM_TABLE_TWIDDLECOEF_Q31_4096)
    target_compile_definitions(CMSISDSP PUBLIC ARM_TABLE_TWIDDLECOEF_Q15_4096)
    if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
    target_compile_definitions(CMSISDSP PUBLIC ARM_TABLE_TWIDDLECOEF_F16_4096)
    endif()
endif()