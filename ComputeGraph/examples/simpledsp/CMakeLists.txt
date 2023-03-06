cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(simpledsp)


add_executable(simpledsp main.cpp)

sdf(simpledsp create.py simpledsp)
add_sdf_dir(simpledsp)

target_include_directories(simpledsp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(simpledsp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated)
target_link_libraries(simpledsp PRIVATE CMSISDSP)