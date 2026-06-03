cmake_minimum_required (VERSION 3.14)
cmake_policy(SET CMP0077 NEW)
project(CMSISDSP)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

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
option(FASTBUILD "Use aggregate source files to reduce build time" OFF)
if (DEFINED CMSISCORE)
  set(CMSISDSP_INSTALL_DEFAULT OFF)
else()
  set(CMSISDSP_INSTALL_DEFAULT ON)
endif()
option(CMSISDSP_INSTALL "Install CMSIS-DSP and export a CMake package" ${CMSISDSP_INSTALL_DEFAULT})
set(CMSISDSP_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/CMSIS-DSP"
    CACHE STRING "CMSIS-DSP headers install directory")

###########################
#
# CMSIS DSP
#
###########################

add_library(CMSISDSP STATIC)
add_library(CMSISDSP::CMSISDSP ALIAS CMSISDSP)

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
target_include_directories(CMSISDSP PUBLIC
  $<BUILD_INTERFACE:${DSP}/Include>
  $<INSTALL_INTERFACE:${CMSISDSP_INSTALL_INCLUDEDIR}>)
if (DEFINED CMSISCORE)
target_include_directories(CMSISDSP PUBLIC
  $<BUILD_INTERFACE:${CMSISCORE}/Include>)
endif()

include(configDsp.cmake)

configDsp(CMSISDSP)

if (CMSISDSP_INSTALL AND DEFINED CMSISCORE)
  message(FATAL_ERROR "CMSISDSP_INSTALL is only supported for host builds without CMSISCORE")
endif()

if (CMSISDSP_INSTALL)
  set(CMSISDSP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/CMSISDSP"
      CACHE STRING "CMake package install directory")

  install(TARGETS CMSISDSP
    EXPORT CMSISDSPTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  install(DIRECTORY "${DSP}/Include/"
    DESTINATION ${CMSISDSP_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h")

  if (NEON OR NEONEXPERIMENTAL)
    install(DIRECTORY "${DSP}/ComputeLibrary/Include/"
      DESTINATION ${CMSISDSP_INSTALL_INCLUDEDIR}
      FILES_MATCHING PATTERN "*.h")
  endif()

  configure_package_config_file(
    "${DSP}/cmake/CMSISDSPConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CMSISDSPConfig.cmake"
    INSTALL_DESTINATION ${CMSISDSP_INSTALL_CMAKEDIR})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CMSISDSPConfig.cmake"
    DESTINATION ${CMSISDSP_INSTALL_CMAKEDIR})

  install(EXPORT CMSISDSPTargets
    NAMESPACE CMSISDSP::
    DESTINATION ${CMSISDSP_INSTALL_CMAKEDIR})
endif()
