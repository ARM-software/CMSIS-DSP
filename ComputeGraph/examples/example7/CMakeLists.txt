cmake_minimum_required (VERSION 3.14)
include(CMakePrintHelpers)

project(Example7)


add_custom_target(example7 ALL DEPENDS sched.py)

sdfpython(example7)

