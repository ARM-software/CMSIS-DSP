function(configDsp project)

if (HOST)
      target_compile_definitions(${project} PUBLIC __GNUC_PYTHON__)
endif()

if (LOOPUNROLL)
  target_compile_definitions(${project} PRIVATE ARM_MATH_LOOPUNROLL)
endif()

if (ROUNDING)
  target_compile_definitions(${project} PRIVATE ARM_MATH_ROUNDING)
endif()

if (MATRIXCHECK)
  target_compile_definitions(${project} PRIVATE ARM_MATH_MATRIX_CHECK)
endif()

if (AUTOVECTORIZE)
    target_compile_definitions(${project} PRIVATE ARM_MATH_AUTOVECTORIZE) 
endif()

if (NEON)
    # Used in arm_vec_math.h
    target_include_directories(${project} PUBLIC "${DSP}/ComputeLibrary/Include")
    target_compile_definitions(${project} PRIVATE ARM_MATH_NEON)

endif()

if (NEONEXPERIMENTAL)
    # Used in arm_vec_math.h
    target_include_directories(${project} PUBLIC "${DSP}/ComputeLibrary/Include")
    target_compile_definitions(${project} PRIVATE ARM_MATH_NEON_EXPERIMENTAL)
endif()

if (MVEFLOAT16)
    target_compile_definitions(${project} PRIVATE ARM_MATH_MVE_FLOAT16) 
endif()

target_include_directories(${project} PRIVATE "${DSP}/PrivateInclude")

if (MVEI OR MVEF OR HELIUM)
    # By default, GCC does not enable implicit conversion between vectors of different numbers or types of elements
    # which is required by some code in CMSIS-DSP
    if (LAXVECTORCONVERSIONS)
       target_compile_options(${project} PRIVATE $<$<STREQUAL:${CMAKE_C_COMPILER_ID},GNU>:-flax-vector-conversions>)
       target_compile_options(${project} PRIVATE $<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-flax-vector-conversions=integer>)
    else()
       target_compile_options(${project} PRIVATE $<$<STREQUAL:${CMAKE_C_COMPILER_ID},GNU>:-fno-lax-vector-conversions>)
       target_compile_options(${project} PRIVATE $<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-flax-vector-conversions=none>)
    endif()
endif()

if (DISABLEFLOAT16)
    target_compile_definitions(${project} PRIVATE DISABLEFLOAT16) 
endif()


endfunction()
