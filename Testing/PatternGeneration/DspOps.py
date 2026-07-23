import os.path
import numpy as np
import itertools
import Tools

def sat_q63(r):
    if (r > 0x07FFF_FFFF_FFFF_FFFF):
      r = 0x07FFF_FFFF_FFFF_FFFF
    if (r < -0x0800_0000_0000_0000):
      r = -0x0800_0000_0000_0000
    return r

def sat_q31(r):
    if (r > 0x7FFF_FFFF):
      r = 0x7FFF_FFFF
    if (r < -0x8000_0000):
      r = -0x8000_0000
    return r

def sat_q15(r):
    if (r > 0x07FFF):
      r = 0x07FFF
    if (r < -0x08000):
      r = -0x08000
    return r

def sat_q7(r):
    if (r > 0x07F):
      r = 0x07F
    if (r < -0x080):
      r = -0x080
    return r


def writeTests(config,format):

    NBSAMPLES = 10
   
    input64 = np.array([0xFFFF_FFFF_FFFF_FFFF,0,1,
                      0x7FFF_FFFF_FFFF_FFFF,
                      0x8000_0000_0000_0000,
                      0x0000_0001_1000_0000,
                      0xFFFF_FFFF_7FFF_FFFF,
                      0x0000_0000_7FFF_0000,
                      0xFFFF_FFFF_7FFF_0000],dtype=np.uint64)
    input64 = input64.view(np.int64)
    config.writeInputS64(1,input64,"Input")

    input32 = np.array([0xFFFF_FFFF,0,1,
                          0x7FFF_FFFF,
                          0x8000_0000,
                          0x0001_1000,
                          0xFFFF_7FFF,
                          0x0001_0010,
                          0xFFFF_007F],dtype=np.uint32)
    input32 = input32.astype(np.int32)
    config.writeInputS32(1,input32,"Input")

    refQ31 = np.vectorize(sat_q31)(input64)
    ref = np.array(refQ31,dtype=np.int32)
    config.writeReferenceS32(1, ref, "Ref")

    refQ15 = np.vectorize(sat_q15)(input32)
    ref = np.array(refQ15,dtype=np.int16)
    config.writeReferenceS16(1, ref, "Ref")


def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","DspOps","DspOps")
    PARAMDIR = os.path.join("Parameters","DSP","DspOps","DspOps")
    
    config=Tools.Config(PATTERNDIR,PARAMDIR,"")


    config.setOverwrite(True)
    writeTests(config,7)


if __name__ == '__main__':
  generatePatterns()
