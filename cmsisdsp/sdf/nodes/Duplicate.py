###########################################
# Project:      CMSIS DSP Library
# Title:        Duplicate.py
# Description:  Duplicate nodes
# 
# $Date:        08 September 2022
# 
# Target Processor: Cortex-M and Cortex-A cores
# -------------------------------------------------------------------- */
# 
# Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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
from .simu import *
import cmsisdsp as dsp 

class Duplicate2(GenericNode12): 
    def __init__(self,inputSize,outputSize1,outputSize2,fifoin,fifoout1,fifoout2):
        GenericNode12.__init__(self,inputSize,outputSize1,outputSize2,fifoin,fifoout1,fifoout2)

    def run(self):
        a=self.getReadBuffer()
        b=self.getWriteBuffer1()
        c=self.getWriteBuffer2()
        b[:] = a[:]
        c[:] = a[:]

        return(0)

class Duplicate3(GenericNode13): 
    def __init__(self,inputSize,outputSize1,outputSize2,outputSize3,fifoin,fifoout1,fifoout2,fifoout3):
        GenericNode13.__init__(self,inputSize,outputSize1,outputSize2,outputSize3,fifoin,fifoout1,fifoout2,fifoout3)
     
    def run(self):
        a=self.getReadBuffer()
        b=self.getWriteBuffer1()
        c=self.getWriteBuffer2()
        d=self.getWriteBuffer3()
        b[:] = copy.deepcopy(a[:])
        c[:] = copy.deepcopy(a[:])
        d[:] = copy.deepcopy(a[:])

        return(0)