cmake_minimum_required (VERSION 3.10)

# Define the project
project (pythonwrapper VERSION 0.1)


###########
#
# CMSIS DSP
#

add_subdirectory(${CMSISDSP}/Source bin_dsp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
