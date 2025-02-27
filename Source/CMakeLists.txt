cmake_minimum_required (VERSION 3.14)
cmake_policy(SET CMP0077 NEW)
project(CMSISDSP)

# DSP Sources
SET(DSP ${CMAKE_CURRENT_SOURCE_DIR}/..)

option(NEON "Neon acceleration" OFF)
option(NEONEXPERIMENTAL "Neon experimental acceleration" OFF)
option(HELIUMEXPERIMENTAL "Helium experimental acceleration" OFF)
option(LOOPUNROLL "Loop unrolling" ON)
option(ROUNDING "Rounding" OFF)
option(MATRIXCHECK "Matrix Checks" OFF)
option(HELIUM "Helium acceleration (MVEF and MVEI supported)" OFF)
option(MVEF "MVEF intrinsics supported" OFF)
option(MVEI "MVEI intrinsics supported" OFF)
option(MVEFLOAT16 "Float16 MVE intrinsics supported" OFF)
option(DISABLEFLOAT16 "Disable building float16 kernels" OFF)
option(HOST "Build for host" OFF)
option(AUTOVECTORIZE "Prefer autovectorizable code to one using C intrinsics" OFF)
option(LAXVECTORCONVERSIONS "Lax vector conversions" ON)
option(NEON_RIFFT_SCALING "Scaling of RIFFT with Neon" ON)
option(FASTBUILD "Faster build" ON)

###########################
#
# CMSIS DSP
#
###########################

add_library(CMSISDSP STATIC)

include(BasicMathFunctions/Config.cmake)

include(ComplexMathFunctions/Config.cmake)

include(QuaternionMathFunctions/Config.cmake)

include(ControllerFunctions/Config.cmake)
 
include(FastMathFunctions/Config.cmake)
 
include(FilteringFunctions/Config.cmake)
 
include(MatrixFunctions/Config.cmake)

include(StatisticsFunctions/Config.cmake)

include(SupportFunctions/Config.cmake)

include(TransformFunctions/Config.cmake)
 
include(CommonTables/Config.cmake)
  
include(SVMFunctions/Config.cmake)

include(BayesFunctions/Config.cmake)

include(DistanceFunctions/Config.cmake)

include(InterpolationFunctions/Config.cmake)

include(WindowFunctions/Config.cmake)

### Includes
target_include_directories(CMSISDSP PUBLIC "${DSP}/Include")
if (DEFINED CMSISCORE)
target_include_directories(CMSISDSP PUBLIC "${CMSISCORE}/Include")
endif()

include(configDsp.cmake)

configDsp(CMSISDSP)
