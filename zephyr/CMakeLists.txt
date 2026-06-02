if(CONFIG_CMSIS_DSP)

set(CMSIS_DSP_DIR ${ZEPHYR_CURRENT_MODULE_DIR})

zephyr_library()

zephyr_library_compile_options(-Ofast)

zephyr_include_directories(
    ${CMSIS_DSP_DIR}/Include
)

zephyr_library_include_directories(
    ${CMSIS_DSP_DIR}/PrivateInclude
)

if (NOT CONFIG_CPU_CORTEX_A)
  zephyr_library_include_directories(${ZEPHYR_CMSIS_MODULE_DIR}/CMSIS/Core/Include)
else()
   zephyr_library_compile_definitions(__GNUC_PYTHON__) # Disable dependency to CMSIS Core
endif()

zephyr_library_compile_definitions(ZEPHYR_INCLUDE_TOOLCHAIN_STDINT_H_)


# Global Feature Definitions
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_NEON                  ARM_MATH_NEON)
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_NEON_EXPERIMENTAL     ARM_MATH_NEON_EXPERIMENTAL)
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_LOOPUNROLL            ARM_MATH_LOOPUNROLL)
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_ROUNDING              ARM_MATH_ROUNDING)
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_MATRIXCHECK           ARM_MATH_MATRIX_CHECK)
zephyr_library_compile_definitions_ifndef(CONFIG_CMSIS_DSP_FLOAT16              DISABLEFLOAT16)
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_AUTOVECTORIZE         ARM_MATH_AUTOVECTORIZE)
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_NE10_DSP_RIFFT_SCALING CMSIS_NE10_DSP_RIFFT_SCALING)
zephyr_library_compile_definitions_ifdef(CONFIG_CMSIS_DSP_CUSTOM_CONFIG  ARM_DSP_CUSTOM_CONFIG)

zephyr_compile_definitions_ifndef(CONFIG_ARM __GNUC_PYTHON__)

if (CONFIG_CMSIS_DSP_NEON OR CONFIG_CMSIS_DSP_NEON_EXPERIMENTAL)
   zephyr_include_directories("${CMSIS_DSP_DIR}/ComputeLibrary/Include")
   zephyr_library_include_directories("${CMSIS_DSP_DIR}/Ne10")
endif()

  if (CONFIG_ARMV8_1_M_MVEI OR CONFIG_ARMV8_1_M_MVEF OR CONFIG_CMSIS_DSP_NEON OR CONFIG_CMSIS_DSP_NEON_EXPERIMENTAL)
    if (CONFIG_CMSIS_DSP_LAX_VECTOR_CONVERSIONS)
      zephyr_library_compile_options($<$<STREQUAL:${CMAKE_C_COMPILER_ID},GNU>:-flax-vector-conversions>)
      zephyr_library_compile_options($<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-flax-vector-conversions=integer>)
    else()
      zephyr_library_compile_options($<$<STREQUAL:${CMAKE_C_COMPILER_ID},GNU>:-fno-lax-vector-conversions>)
      zephyr_library_compile_options($<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-flax-vector-conversions=none>)
    endif()
  endif()

# Basic math functions
if (CONFIG_CMSIS_DSP_BASICMATH)
zephyr_library_sources(${CMSIS_DSP_DIR}/Source/BasicMathFunctions/BasicMathFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/BasicMathFunctions/BasicMathFunctionsF16.c)
  endif()
endif()

  # Bayes functions
if (CONFIG_CMSIS_DSP_BAYES)
zephyr_library_sources(${CMSIS_DSP_DIR}/Source/BayesFunctions/BayesFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/BayesFunctions/BayesFunctionsF16.c)
  endif()
endif()

# Common tables
zephyr_library_sources(${CMSIS_DSP_DIR}/Source/CommonTables/arm_common_tables.c 
    ${CMSIS_DSP_DIR}/Source/CommonTables/arm_common_tables_f16.c)


zephyr_library_sources(${CMSIS_DSP_DIR}/Source/CommonTables/arm_const_structs.c)
zephyr_library_sources(${CMSIS_DSP_DIR}/Source/CommonTables/arm_const_structs_f16.c)



if (CONFIG_CMSIS_DSP_NEON OR CONFIG_CMSIS_DSP_NEON_EXPERIMENTAL)
    zephyr_library_sources(${CMSIS_DSP_DIR}/ComputeLibrary/Source/arm_cl_tables.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/CommonTables/arm_neon_tables.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/CommonTables/arm_neon_tables_f16.c)
endif()

if (CONFIG_ARMV8_1_M_MVEI OR CONFIG_ARMV8_1_M_MVEF)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/CommonTables/arm_mve_tables.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/CommonTables/arm_mve_tables_f16.c)
endif()

# Complex math functions
if (CONFIG_CMSIS_DSP_COMPLEXMATH)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/ComplexMathFunctions/ComplexMathFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/ComplexMathFunctions/ComplexMathFunctionsF16.c)
  endif()
endif()

# Controller functions
if (CONFIG_CMSIS_DSP_CONTROLLER)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/ControllerFunctions/ControllerFunctions.c)
endif()

# Distance functions
if (CONFIG_CMSIS_DSP_DISTANCE)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/DistanceFunctions/DistanceFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/DistanceFunctions/DistanceFunctionsF16.c)
  endif()
endif()

# Fast math functions
if (CONFIG_CMSIS_DSP_FASTMATH)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/FastMathFunctions/FastMathFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/FastMathFunctions/FastMathFunctionsF16.c)
  endif()
endif()

  # Filtering functions
if (CONFIG_CMSIS_DSP_FILTERING)
zephyr_library_sources(${CMSIS_DSP_DIR}/Source/FilteringFunctions/FilteringFunctions.c)
    if (CONFIG_CMSIS_DSP_FLOAT16)
        zephyr_library_sources(${CMSIS_DSP_DIR}/Source/FilteringFunctions/FilteringFunctionsF16.c)
    endif()
endif()

# Interpolation functions
if (CONFIG_CMSIS_DSP_INTERPOLATION)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/InterpolationFunctions/InterpolationFunctions.c)
  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/InterpolationFunctions/InterpolationFunctionsF16.c)
  endif()
endif()

# Matrix functions
if (CONFIG_CMSIS_DSP_MATRIX)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/MatrixFunctions/MatrixFunctions.c)
    if (CONFIG_CMSIS_DSP_FLOAT16)
        zephyr_library_sources(${CMSIS_DSP_DIR}/Source/MatrixFunctions/MatrixFunctionsF16.c)
    endif()
endif()

# Quaternion functions
if (CONFIG_CMSIS_DSP_QUATERNIONMATH)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/QuaternionMathFunctions/QuaternionMathFunctions.c)
endif()

# Statistics functions
if (CONFIG_CMSIS_DSP_STATISTICS)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/StatisticsFunctions/StatisticsFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/StatisticsFunctions/StatisticsFunctionsF16.c)
  endif()
endif()

# Support functions
if (CONFIG_CMSIS_DSP_SUPPORT)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/SupportFunctions/SupportFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/SupportFunctions/SupportFunctionsF16.c)
  endif()
endif()

# SVM Functions
if (CONFIG_CMSIS_DSP_SVM)
  zephyr_library_sources(${CMSIS_DSP_DIR}/Source/SVMFunctions/SVMFunctions.c)
    if (CONFIG_CMSIS_DSP_FLOAT16)
        zephyr_library_sources(${CMSIS_DSP_DIR}/Source/SVMFunctions/SVMFunctionsF16.c)
    endif()
endif()

# Transform functions
if (CONFIG_CMSIS_DSP_TRANSFORM)
zephyr_library_sources(${CMSIS_DSP_DIR}/Source/TransformFunctions/TransformFunctions.c)

  if (CONFIG_CMSIS_DSP_FLOAT16)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Source/TransformFunctions/TransformFunctionsF16.c)
  endif()
endif()

if (CONFIG_CMSIS_DSP_NEON OR CONFIG_CMSIS_DSP_NEON_EXPERIMENTAL)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/NE10_fft_float32.neonintrinsic.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/NE10_fft_int32.neonintrinsic.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/NE10_fft_int16.neonintrinsic.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/NE10_rfft_float32.neonintrinsic.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/CMSIS_NE10_fft_init.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/CMSIS_NE10_fft_generic_float32.neonintrisic.c)
    zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/CMSIS_NE10_fft_generic_int32.neonintrisic.c)



    if (CONFIG_CMSIS_DSP_FLOAT16)
        zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/NE10_fft_float16.neonintrinsic.c)
        zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/NE10_rfft_float16.neonintrinsic.c)
        zephyr_library_sources(${CMSIS_DSP_DIR}/Ne10/CMSIS_NE10_fft_generic_float16.neonintrisic.c)
    endif()
endif()

# Windows functions
if (CONFIG_CMSIS_DSP_WINDOW)
zephyr_library_sources(${CMSIS_DSP_DIR}/Source/WindowFunctions/WindowFunctions.c)
endif()

endif()