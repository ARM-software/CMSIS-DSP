# CMSIS Build CMakeLists generated on 2024-09-02T09:13:52

cmake_minimum_required(VERSION 3.22)

# Target options

set(TARGET matrix)
set(CPU Cortex-M55)
set(PRJ_DIR "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/cprj")
set(OUT_DIR "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/cprj/out/matrix/VHT-Corstone-300/Release")
set(INT_DIR "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/cprj/tmp/matrix/VHT-Corstone-300/Release")
set(FPU DP_FPU)
set(DSP DSP)
set(TZ TZ)
set(MVE FP_MVE)
set(DEBUG on)
set(AS_FLAGS_GLOBAL "-masm=auto")
set(CC_FLAGS_GLOBAL "-DSEMIHOSTING -Wsign-compare -Wdouble-promotion -DNDEBUG -Wall -Wextra -Werror -std=c11 -Ofast -ffast-math -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality -Wno-reserved-identifier")
set(CXX_FLAGS_GLOBAL "-fno-rtti -DNDEBUG -Wall -Wextra -std=c++11 -Ofast -ffast-math")
set(LD_FLAGS_GLOBAL "--entry=Reset_Handler --info=summarysizes --info=sizes --info=totals --info=unused --info=veneers")
set(LD_SCRIPT "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/RTE/Device/SSE-300-MPS3/linker_SSE300MPS3_secure.sct")

set(DEFINES
  _RTE_
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BasicMathFunctions/BasicMathFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BayesFunctions/BayesFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/CommonTables/CommonTables.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/CommonTables/CommonTablesF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/ComplexMathFunctions/ComplexMathFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/ControllerFunctions/ControllerFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/DistanceFunctions/DistanceFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/DistanceFunctions/DistanceFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FastMathFunctions/FastMathFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FastMathFunctions/FastMathFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FilteringFunctions/FilteringFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FilteringFunctions/FilteringFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/InterpolationFunctions/InterpolationFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/InterpolationFunctions/InterpolationFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/MatrixFunctions/MatrixFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/MatrixFunctions/MatrixFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SVMFunctions/SVMFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SVMFunctions/SVMFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/StatisticsFunctions/StatisticsFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/StatisticsFunctions/StatisticsFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SupportFunctions/SupportFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SupportFunctions/SupportFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/TransformFunctions/TransformFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/TransformFunctions/TransformFunctionsF16.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(DEFINES_C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/WindowFunctions/WindowFunctions.c
  _RTE_
  ARM_MATH_ROUNDING
)

set(INC_PATHS
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/ARM/arm_matrix_example"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Include"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/PrivateInclude"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/RTE/Device/SSE-300-MPS3"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/projects/RTE/_Release_VHT-Corstone-300"
  "C:/ctools_pack/ARM/CMSIS/6.0.0/CMSIS/Core/Include"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Board/Device_Definition"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Board/Platform"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Device/Include"
)

set(CC_SRC_FILES
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/ARM/arm_matrix_example/arm_matrix_example_f32.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/ARM/arm_matrix_example/math_helper.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/RTE/Device/SSE-300-MPS3/startup_SSE300MPS3.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Examples/cmsis_build/RTE/Device/SSE-300-MPS3/system_SSE300MPS3.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BasicMathFunctions/BasicMathFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BayesFunctions/BayesFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/CommonTables/CommonTables.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/CommonTables/CommonTablesF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/ComplexMathFunctions/ComplexMathFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/ControllerFunctions/ControllerFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/DistanceFunctions/DistanceFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/DistanceFunctions/DistanceFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FastMathFunctions/FastMathFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FastMathFunctions/FastMathFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FilteringFunctions/FilteringFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/FilteringFunctions/FilteringFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/InterpolationFunctions/InterpolationFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/InterpolationFunctions/InterpolationFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/MatrixFunctions/MatrixFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/MatrixFunctions/MatrixFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SVMFunctions/SVMFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SVMFunctions/SVMFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/StatisticsFunctions/StatisticsFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/StatisticsFunctions/StatisticsFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SupportFunctions/SupportFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/SupportFunctions/SupportFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/TransformFunctions/TransformFunctions.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/TransformFunctions/TransformFunctionsF16.c"
  "C:/Users/chrfav01/benchresults/cmsis/CMSIS-DSP/Source/WindowFunctions/WindowFunctions.c"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Board/Device_Definition/device_definition.c"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Device/Source/syscounter_armv8-m_cntrl_drv.c"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Device/Source/syscounter_armv8-m_read_drv.c"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Device/Source/systimer_armv8-m_drv.c"
  "C:/ctools_pack/ARM/V2M_MPS3_SSE_300_BSP/1.4.0/Device/Source/systimer_armv8-m_timeout.c"
)

# Toolchain config map

set(TOOLCHAIN_VERSION_MIN "6.22.0")
include ("C:/ctools/etc/AC6.6.22.0.cmake")
include ("C:/ctools/etc/CMSIS-Build-Utils.cmake")

# Setup project

project(${TARGET} LANGUAGES C)

cbuild_get_running_toolchain(TOOLCHAIN_ROOT TOOLCHAIN_VERSION C)

# Global Flags

cbuild_get_system_includes(CC_SYS_INC_PATHS_LIST CC_SYS_INC_PATHS)
set(CMAKE_C_FLAGS "${CC_CPU} ${CC_OPTIONS_FLAGS} ${CC_FLAGS} ${CC_FLAGS_GLOBAL} ${CC_SYS_INC_PATHS}")
set(CMAKE_C_LINK_FLAGS "${LD_CPU} ${_LS}\"${LD_SCRIPT}\" ${LD_FLAGS_GLOBAL} ${LD_OPTIONS_FLAGS} ${LD_FLAGS}")

# File Includes, Defines and Options

foreach(SRC ${CC_SRC_FILES})
  string(REPLACE " " "?" S ${SRC})
  get_source_file_property(FILE_FLAGS ${SRC} COMPILE_FLAGS)
  if(FILE_FLAGS STREQUAL "NOTFOUND")
    set(FILE_FLAGS)
  endif()
  if(DEFINED DEFINES_${S})
    cbuild_set_defines(CC DEFINES_${S})
    string(APPEND FILE_FLAGS " ${DEFINES_${S}}")
  else()
    string(APPEND FILE_FLAGS " ${CC_DEFINES}")
  endif()
  set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${FILE_FLAGS}")
endforeach()

# Compilation Database

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(database COMMAND ${CMAKE_COMMAND} -E copy_if_different "${INT_DIR}/compile_commands.json" "${OUT_DIR}")

# Setup Target

add_executable(${TARGET} ${CC_SRC_FILES})
set_target_properties(${TARGET} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "matrix")
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR} LINK_DEPENDS ${LD_SCRIPT})
target_include_directories(${TARGET} PUBLIC ${INC_PATHS})
