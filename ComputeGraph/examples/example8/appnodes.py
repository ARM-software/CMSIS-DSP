###########################################
# Project:      CMSIS DSP Library
# Title:        appnodes.py
# Description:  Application nodes for Example 8
# 
# 
# Target Processor: Cortex-M and Cortex-A cores
# -------------------------------------------------------------------- */
# 
# Copyright (C) 2021-2023 ARM Limited or its affiliates. All rights reserved.
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
from cmsisdsp.cg.nodes.simu import *
from custom import *
from cmsisdsp.cg.nodes.Duplicate import *

class Sink(GenericSink):
    def __init__(self,inputSize,fifoin):
        GenericSink.__init__(self,inputSize,fifoin)
        

    def run(self):
        # The null sink must at least get a buffer from the FIFO
        # or the FIFO will never be emptied
        # and the scheduling will fail
        print("Sink")
        i=self.getReadBuffer()
        for c in i:
            print("%f + I %f" % (c.re,c.im))
        return(0)

class ProcessingNode(GenericNode12): 
    def __init__(self,inputSize,outputSize1,outputSize2,fifoin,fifoout1,fifoout2,i,s,v):
        GenericNode12.__init__(self,inputSize,outputSize1,outputSize2,fifoin,fifoout1,fifoout2)

    def run(self):
        print("ProcessingNode");
        a=self.getReadBuffer()
        b=self.getWriteBuffer1()
        c=self.getWriteBuffer2()
        # Python objects have reference semantic and not
        # value semantic.
        # So in a write buffer, we can change the
        # fields of an object but we should not
        # replace the object and risk creating sharing
        # Duplicating the a object may be ok
        b[0]=a[3]
        c[0]=a[3]
        return(0)

class Source(GenericSource):
    def __init__(self,outputSize,fifoout):
        GenericSource.__init__(self,outputSize,fifoout)
        self._counter=0

    def run(self):
        print("Source");
        a=self.getWriteBuffer()

        # Python objects have reference semantic and not
        # value semantic.
        # So in a write buffer, we can change the
        # fields of an object but we should not
        # replace the object and risk creating sharing
        # Creating a new object may be ok
        for i in range(self._outputSize):
            #a[i].re = 1.0*self._counter
            #a[i].im = 0.0
            a[i] = MyComplex(1.0*self._counter, 0.0)
            self._counter = self._counter + 1

        return(0)

   