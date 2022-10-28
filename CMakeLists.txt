# Copyright (c) 2022, Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This CMakeLists.txt in the root of the repository ensures that, when another
# project fetches CMSIS-DSP by calling FetchContent_MakeAvailable(), the call
# automatically adds CMSIS-DSP as a subdirectory. To maintain compatibility with
# existing projects that assume CMSIS-DSP's entry point to be Source/, we only
# add a redirection in the root CMakeLists.txt.
add_subdirectory(Source)
