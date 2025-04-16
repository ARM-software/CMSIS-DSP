cmake_minimum_required (VERSION 3.14)



target_sources(CMSISDSP PRIVATE CommonTables/arm_common_tables.c 
    CommonTables/arm_common_tables_f16.c)


target_sources(CMSISDSP PRIVATE CommonTables/arm_const_structs.c)
target_sources(CMSISDSP PRIVATE CommonTables/arm_const_structs_f16.c)



if (NEON OR NEONEXPERIMENTAL)
    target_sources(CMSISDSP PRIVATE "${DSP}/ComputeLibrary/Source/arm_cl_tables.c")
    target_sources(CMSISDSP PRIVATE "CommonTables/arm_neon_tables.c")
    target_sources(CMSISDSP PRIVATE "CommonTables/arm_neon_tables_f16.c")
endif()

if (HELIUM OR MVEF)
    target_sources(CMSISDSP PRIVATE "CommonTables/arm_mve_tables.c")
    target_sources(CMSISDSP PRIVATE "CommonTables/arm_mve_tables_f16.c")
endif()

