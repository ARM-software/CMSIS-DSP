cmake_minimum_required(VERSION 3.23)

include(CMakePrintHelpers)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(Tests)


option(HOST "" ON)
option(NEON "" ON)
option(DISABLEFLOAT16 "" OFF)
add_subdirectory(../../Source bin_dsp)
#target_compile_definitions(CMSISDSP PRIVATE ARM_DSP_CUSTOM_CONFIG)
target_include_directories(CMSISDSP PRIVATE .)
target_compile_options(CMSISDSP PUBLIC /W4 /Oi /WX-)

target_compile_definitions(CMSISDSP PUBLIC ARM_DSP_TESTING)

add_executable(test main.c)

target_link_libraries(test PUBLIC CMSISDSP)
