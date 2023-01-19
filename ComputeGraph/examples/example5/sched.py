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


FIFOSIZE0=384

buf0=np.empty(FIFOSIZE0,dtype=np.int16)
for i in range(FIFOSIZE0):
    buf0[i] = 0

FIFOSIZE1=768

buf1=np.empty(FIFOSIZE1,dtype=np.int16)
for i in range(FIFOSIZE1):
    buf1[i] = 0

FIFOSIZE2=1024

buf2=np.empty(FIFOSIZE2,dtype=np.int16)
for i in range(FIFOSIZE2):
    buf2[i] = 0

FIFOSIZE3=377

buf3=np.empty(FIFOSIZE3,dtype=np.int16)
for i in range(FIFOSIZE3):
    buf3[i] = 0

FIFOSIZE4=754

buf4=np.empty(FIFOSIZE4,dtype=np.int16)
for i in range(FIFOSIZE4):
    buf4[i] = 0


def scheduler(mfccConfig,dispbuf):
    cgStaticError=0
    nbSchedule=0
    debugCounter=12

    #
    #  Create FIFOs objects
    #
    fifo0=FIFO(FIFOSIZE0,buf0)
    fifo1=FIFO(FIFOSIZE1,buf1)
    fifo2=FIFO(FIFOSIZE2,buf2)
    fifo3=FIFO(FIFOSIZE3,buf3)
    fifo4=FIFO(FIFOSIZE4,buf4)

    # 
    #  Create node objects
    #
    audioWin = SlidingBuffer(1024,256,fifo1,fifo2)
    mfcc = MFCC(1024,13,fifo2,fifo3,mfccConfig)
    mfccWin = SlidingBuffer(754,377,fifo3,fifo4)
    sink = NumpySink(754,fifo4,dispbuf)
    src = WavSource(384,fifo0,True,"test_stereo.wav")
    toMono = InterleavedStereoToMono(384,192,fifo0,fifo1)

    while((cgStaticError==0) and (debugCounter > 0)):
       nbSchedule = nbSchedule + 1

       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = src.run()
       if cgStaticError < 0:
          break
       cgStaticError = toMono.run()
       if cgStaticError < 0:
          break
       cgStaticError = audioWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfcc.run()
       if cgStaticError < 0:
          break
       cgStaticError = mfccWin.run()
       if cgStaticError < 0:
          break
       cgStaticError = sink.run()
       if cgStaticError < 0:
          break

       debugCounter = debugCounter - 1 
    return(nbSchedule,cgStaticError)
