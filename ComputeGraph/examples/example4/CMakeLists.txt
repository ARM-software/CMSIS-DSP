cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(Example4)


add_custom_target(example4 ALL DEPENDS sched.py)

sdfpython(example4)

