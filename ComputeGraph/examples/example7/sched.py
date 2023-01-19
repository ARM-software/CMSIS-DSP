#
# Generated with CMSIS-DSP Compute Graph Scripts.
# The generated code is not covered by CMSIS-DSP license.
# 
# The support classes and code is covered by CMSIS-DSP license.
#

import sys


import numpy as np
import cmsisdsp as dsp
from cmsisdsp.cg.nodes.simu import *
from appnodes import * 
from custom import *

DEBUGSCHED=False

# 
# FIFO buffers
# 


FIFOSIZE0=128

buf0=np.empty(FIFOSIZE0,dtype=np.int16)
for i in range(FIFOSIZE0):
    buf0[i] = 0

FIFOSIZE1=128

buf1=np.empty(FIFOSIZE1,dtype=np.int16)
for i in range(FIFOSIZE1):
    buf1[i] = 0


def scheduler():
    cgStaticError=0
    nbSchedule=0

    #
    #  Create FIFOs objects
    #
    fifo0=FIFO(FIFOSIZE0,buf0)
    fifo1=FIFO(FIFOSIZE1,buf1)

    # 
    #  Create node objects
    #
    proc = Processing(128,128,fifo0,fifo1)
    sink = VHTSink(128,fifo1,0)
    src = VHTSource(128,fifo0,0)

    while(cgStaticError==0):
       nbSchedule = nbSchedule + 1

       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = proc.run()
       if cgStaticError < 0:
          break
       cgStaticError = sink.run()
       if cgStaticError < 0:
          break

    return(nbSchedule,cgStaticError)
