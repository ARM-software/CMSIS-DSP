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


FIFOSIZE0=256

buf0=np.empty(FIFOSIZE0,dtype=np.float32)
for i in range(FIFOSIZE0):
    buf0[i] = 0

FIFOSIZE1=256

buf1=np.empty(FIFOSIZE1,dtype=np.float32)
for i in range(FIFOSIZE1):
    buf1[i] = 0

FIFOSIZE2=256

buf2=np.empty(FIFOSIZE2,dtype=np.float32)
for i in range(FIFOSIZE2):
    buf2[i] = 0

FIFOSIZE3=512

buf3=np.empty(FIFOSIZE3,dtype=np.float32)
for i in range(FIFOSIZE3):
    buf3[i] = 0

FIFOSIZE4=512

buf4=np.empty(FIFOSIZE4,dtype=np.float32)
for i in range(FIFOSIZE4):
    buf4[i] = 0

FIFOSIZE5=512

buf5=np.empty(FIFOSIZE5,dtype=np.float32)
for i in range(FIFOSIZE5):
    buf5[i] = 0

FIFOSIZE6=256

buf6=np.empty(FIFOSIZE6,dtype=np.float32)
for i in range(FIFOSIZE6):
    buf6[i] = 0

FIFOSIZE7=256

buf7=np.empty(FIFOSIZE7,dtype=np.float32)
for i in range(FIFOSIZE7):
    buf7[i] = 0


def scheduler(dispbuf):
    cgStaticError=0
    nbSchedule=0
    debugCounter=42

    #
    #  Create FIFOs objects
    #
    fifo0=FIFO(FIFOSIZE0,buf0)
    fifo1=FIFO(FIFOSIZE1,buf1)
    fifo2=FIFO(FIFOSIZE2,buf2)
    fifo3=FIFO(FIFOSIZE3,buf3)
    fifo4=FIFO(FIFOSIZE4,buf4)
    fifo5=FIFO(FIFOSIZE5,buf5)
    fifo6=FIFO(FIFOSIZE6,buf6)
    fifo7=FIFO(FIFOSIZE7,buf7)

    # 
    #  Create node objects
    #
    audioOverlap = OverlapAdd(256,128,fifo6,fifo7)
    audioWin = SlidingBuffer(256,128,fifo0,fifo1)
    cfft = CFFT(512,512,fifo3,fifo4)
    icfft = ICFFT(512,512,fifo4,fifo5)
    sink = FileSink(192,fifo7,"output_example3.txt",dispbuf)
    src = FileSource(192,fifo0,"input_example3.txt")
    toCmplx = ToComplex(256,512,fifo2,fifo3)
    toReal = ToReal(512,256,fifo5,fifo6)

    while((cgStaticError==0) and (debugCounter > 0)):
       nbSchedule = nbSchedule + 1

       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       
       i0=fifo1.getReadBuffer(256)
       o2=fifo2.getWriteBuffer(256)
       o2[:]=dsp.arm_mult_f32(i0,HANN)
       cgStaticError = 0
       
       if cgStaticError < 0:
          break
       cgStaticError = toCmplx.run()
       if cgStaticError < 0:
          break
       cgStaticError = cfft.run()
       if cgStaticError < 0:
          break
       cgStaticError = icfft.run()
       if cgStaticError < 0:
          break
       cgStaticError = toReal.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioOverlap.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       
       i0=fifo1.getReadBuffer(256)
       o2=fifo2.getWriteBuffer(256)
       o2[:]=dsp.arm_mult_f32(i0,HANN)
       cgStaticError = 0
       
       if cgStaticError < 0:
          break
       cgStaticError = toCmplx.run()
       if cgStaticError < 0:
          break
       cgStaticError = cfft.run()
       if cgStaticError < 0:
          break
       cgStaticError = icfft.run()
       if cgStaticError < 0:
          break
       cgStaticError = toReal.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioOverlap.run()
       if cgStaticError < 0:
          break
       cgStaticError = sink.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       
       i0=fifo1.getReadBuffer(256)
       o2=fifo2.getWriteBuffer(256)
       o2[:]=dsp.arm_mult_f32(i0,HANN)
       cgStaticError = 0
       
       if cgStaticError < 0:
          break
       cgStaticError = toCmplx.run()
       if cgStaticError < 0:
          break
       cgStaticError = cfft.run()
       if cgStaticError < 0:
          break
       cgStaticError = icfft.run()
       if cgStaticError < 0:
          break
       cgStaticError = toReal.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioOverlap.run()
       if cgStaticError < 0:
          break
       cgStaticError = sink.run()
       if cgStaticError < 0:
          break

       debugCounter = debugCounter - 1 
    return(nbSchedule,cgStaticError)
