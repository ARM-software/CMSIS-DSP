cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(Example5)


add_custom_target(example5 ALL DEPENDS sched.py)

sdfpython(example5)

