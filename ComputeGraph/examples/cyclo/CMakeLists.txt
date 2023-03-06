cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(cyclo)


add_executable(cyclo main.cpp)

sdf(cyclo create.py cyclo)
add_sdf_dir(cyclo)

target_include_directories(cyclo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(cyclo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated)
