###########################################
# Project:      CMSIS DSP Library
# Title:        config.py
# Description:  Configuration of the code generator
# 
# $Date:        29 July 2021
# $Revision:    V1.10.0
# 
# Target Processor: Cortex-M and Cortex-A cores
# -------------------------------------------------------------------- */
# 
# Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
# 
# SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
############################################
"""Configuration of C code generation"""
class Configuration:

    def __init__(self):

        #########################
        #
        # SCHEDULING OPTIONS
        #

        # If FIFOs size must be dumped during computation of the schedule
        self.displayFIFOSizes=False

        # Experimental so disabled by default
        self.memoryOptimization = False

        # Give priority to sink with topological sort 
        self.sinkPriority = True

        # Print schedule in a human understandble form
        self.dumpSchedule = False

        #############################
        #
        # GRAPHVIZ GENERATION OPTIONS
        #

        # True for an horizontal graphviz layout
        self.horizontal = True

        # Display FIFO buffers in graph instead of datatype
        self.displayFIFOBuf = False

        #########################
        #
        # CODE GENERATION OPTIONS
        #

        # Number of iterations of the schedule. By default it is infinite
        # represented by the value 0
        self.debugLimit = 0 

        # If FIFO content must be dumped during execution of the schedule.
        self.dumpFIFO = False

        # Name of scheduling function in the generated code
        # (Must be valid C and Python name)
        self.schedName = "scheduler"

        # Additional arguments for the scheduler API
        # must be valid C
        self.cOptionalArgs=""

        # Additional arguments for the scheduler API
        # must be valid C
        self.pyOptionalArgs=""

        # Prefix to add before the global FIFO buffer names
        self.prefix = ""

        # Path to CG  module for Python simu 
        self.pathToCGModule="../../.."

        # When codeArray  is true, instead of using
        # function calls we parse un array giving
        # the index of functions to call in another array
        self.codeArray = True
        # If users do not want to use function pointers,
        # we can generate a switch/case instead
        self.switchCase = True

        # Enable support for CMSIS Event Recorder 
        self.eventRecorder = False

        # Name of AppNode file
        self.appNodesCName = "AppNodes.h"
        self.appNodesPythonName = "appnodes"

        # Name of custom file
        self.customCName = "custom.h"
        self.customPythonName = "custom"

        # Name of post custom files
        self.postCustomCName = ""

        # Name of generic nodes headers
        self.genericNodeCName = "GenericNodes.h"

        # Name of scheduler source and header files
        self.schedulerCFileName = "scheduler"
        self.schedulerPythonFileName = "sched"

        # True is C API for the scheduler
        self.CAPI = True

        # By default arm_math.h is included
        self.CMSISDSP = True

        # Asynchronous scheduling using
        # synchronous as first start
        # It implies switchCase and codeArray
        # It disables memory optimizations
        # Also FIFO cannot be used any more as just
        # buffers.
        self.asynchronous = False

        # Increase in percent of the FIFOs.
        # Used in asynchronous mode
        # where the FIFOs may need to be bigger
        # than what is computed assuming a 
        # synchronous scheduling.
        self.FIFOIncrease = 0

        # Default asynchronous more for pure functions 
        # like CMSIS-DSP
        self.asyncDefaultSkip = True

       
    @property
    def debug(self):
        return (self.debugLimit > 0)
    

