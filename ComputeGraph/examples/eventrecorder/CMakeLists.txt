cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(Simple)


add_executable(simple main.cpp)

sdf(simple create.py simple)
add_sdf_dir(simple)

target_include_directories(simple PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(simple PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated)
