cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(Example9)


add_executable(example9 main.cpp)

sdf(example9 graph.py test)
add_sdf_dir(example9)

target_include_directories(example9 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(example9 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated)
