cmake_minimum_required (VERSION 3.6)

# Define the project
project (pythonwrapper VERSION 0.1)


###########
#
# CMSIS DSP
#

add_subdirectory(${CMSISDSP}/Source bin_dsp)

