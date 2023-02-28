cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(Example10)


add_executable(example10 main.cpp)

sdf(example10 graph.py test)
add_sdf_dir(example10)

target_include_directories(example10 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(example10 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated)
