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

def q15_from_bits(r):
    # SMLALD treats each 16-bit lane as a signed two's-complement value.
    r = int(r) & 0xFFFF
    return r - 0x10000 if r & 0x8000 else r

def smlald_ref(x, y, acc):
    # Model the Cortex-M DSP SMLALD instruction semantics: multiply the
    # corresponding signed bottom and top halfwords, then add both products
    # to the 64-bit accumulator.  This architectural model is intentionally
    # independent of the C fallback implementation under test.
    x = int(x)
    y = int(y)
    acc = int(acc)
    return (q15_from_bits(x) * q15_from_bits(y) +
            q15_from_bits(x >> 16) * q15_from_bits(y >> 16) +
            acc)

def smlaldx_ref(x, y, acc):
    # SMLALDX exchanges the halfwords of the second operand before applying
    # the same signed dual multiply-accumulate operation as SMLALD.
    x = int(x)
    y = int(y)
    acc = int(acc)
    return (q15_from_bits(x) * q15_from_bits(y >> 16) +
            q15_from_bits(x >> 16) * q15_from_bits(y) +
            acc)


def writeTests(config,format):

    NBSAMPLES = 16
   
    input64 = np.array([0xFFFF_FFFF_FFFF_FFFF,0,1,
                      0x7FFF_FFFF_FFFF_FFFF,
                      0x8000_0000_0000_0000,
                      0xFFFF_FFFE_FFFF_FFFF,
                      0xFFFF_FFFF_8000_0000,
                      0xFFFF_FFFF_8000_0001,
                      0x0000_0001_1000_0000,
                      0xFFFF_FFFF_7FFF_FFFF,
                      0x0000_0000_7FFF_FFFE,
                      0x0000_0000_7FFF_FFFF,
                      0x0000_0000_8000_0000,
                      0x0000_0000_FFFF_FFFF,
                      0x0000_0000_7FFF_0000,
                      0xFFFF_FFFF_7FFF_0000],dtype=np.uint64)
    input64 = input64.view(np.int64)
    config.writeInputS64(1,input64,"Input")

    input32 = np.array([0xFFFF_FFFF,0,1,
                          0x7FFF_FFFF,
                          0x8000_0000,
                          0xFFFE_FFFF,
                          0xFFFF_8000,
                          0xFFFF_8001,
                          0x0001_1000,
                          0xFFFF_7FFF,
                          0x0000_7FFE,
                          0x0000_7FFF,
                          0x0000_8000,
                          0x0000_FFFF,
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

    inputSMLALDX = np.array([0x0000_0000,
                             0x8000_8000,
                             0x7FFF_7FFF,
                             0x8000_8000,
                             0x8000_7FFF,
                             0x8000_7FFF,
                             0x0001_0002,
                             0xFFFF_0001,
                             0x0001_0001,
                             0xFFFF_FFFF,
                             0x4000_C000,
                             0x1234_FEDC], dtype=np.uint32).view(np.int32)

    inputSMLALDY = np.array([0x0000_0000,
                             0x8000_8000,
                             0x7FFF_7FFF,
                             0x7FFF_7FFF,
                             0x8000_7FFF,
                             0x7FFF_8000,
                             0x0003_0004,
                             0x0001_FFFF,
                             0x0001_0001,
                             0xFFFF_FFFF,
                             0x4000_C000,
                             0xFEDC_1234], dtype=np.uint32).view(np.int32)

    inputSMLALDAcc = np.array([0, 0, 0, 0, 0, 0, 0, 5,
                               0x0000_0001_0000_0000,
                              -0x0000_0001_0000_0000,
                               0x0000_0000_7FFF_FFFF,
                              -0x0000_0000_8000_0000], dtype=np.int64)

    refSMLALD = [smlald_ref(x, y, acc) for x, y, acc in
                 zip(inputSMLALDX, inputSMLALDY, inputSMLALDAcc)]
    refSMLALDX = [smlaldx_ref(x, y, acc) for x, y, acc in
                  zip(inputSMLALDX, inputSMLALDY, inputSMLALDAcc)]

    config.writeInputS32(2, inputSMLALDX, "Input")
    config.writeInputS32(3, inputSMLALDY, "Input")
    config.writeInputS64(2, inputSMLALDAcc, "Input")
    config.writeReferenceS64(2, np.array(refSMLALD, dtype=np.int64), "Ref")
    config.writeReferenceS64(3, np.array(refSMLALDX, dtype=np.int64), "Ref")


def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","DspOps","DspOps")
    PARAMDIR = os.path.join("Parameters","DSP","DspOps","DspOps")
    
    config=Tools.Config(PATTERNDIR,PARAMDIR,"")


    config.setOverwrite(True)
    writeTests(config,7)


if __name__ == '__main__':
  generatePatterns()
