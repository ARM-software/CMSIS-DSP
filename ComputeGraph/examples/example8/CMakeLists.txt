cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(Example8)


add_executable(example8 main.cpp)

sdf(example8 graph.py test)
add_sdf_dir(example8)

target_include_directories(example8 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(example8 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated)
