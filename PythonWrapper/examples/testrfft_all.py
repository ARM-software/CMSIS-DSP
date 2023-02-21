import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f

import numpy as np
from scipy import signal
#import matplotlib.pyplot as plt
import scipy.fft

import colorama
from colorama import init,Fore, Back, Style
from numpy.testing import assert_allclose

init()

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print("\n" + Style.BRIGHT + s + Style.RESET_ALL)


def chop(A, eps = 1e-6):
    B = np.copy(A)
    B[np.abs(A) < eps] = 0
    return B

# For fixed point version, compare that
# the conjugate part is really the conjugate part
def compareWithConjugatePart(r):
    res = r[0::2] + 1j * r[1::2]
    conjPart = res[nb:nb//2:-1].conj()
    refPart = res[1:nb//2]
    assert(np.equal(refPart , conjPart).all())

nb = 32
signal = np.cos(2 * np.pi * np.arange(nb) / nb)*np.cos(0.2*2 * np.pi * np.arange(nb) / nb)

ref=scipy.fft.rfft(signal)
invref = scipy.fft.irfft(ref)

assert(len(ref) == (nb // 2) + 1)
assert(len(invref) == nb)

# Length of arrays for the float implementation
# of the RFFT (in float so there is a factor 2
# when the samples are complex)

RFFT_F_IN_LENGTH = nb # real
RFFT_F_OUT_LENGTH = nb # complex (so nb // 2 complex)

RIFFT_F_IN_LENGTH = nb # complex
RIFFT_F_OUT_LENGTH = nb # real

# Length of arrays for the fixed point implementation
# of the RFFT
RFFT_Q_IN_LENGTH = nb 
RFFT_Q_OUT_LENGTH = 2*nb 

# Conjugate part ignored
RIFFT_Q_IN_LENGTH = nb + 2
RIFFT_Q_OUT_LENGTH = nb

# Convert ref to CMSIS-DSP format 
referenceFloat=np.zeros(nb)
# Replace complex datatype by real datatype
referenceFloat[0::2] = np.real(ref)[:-1]
referenceFloat[1::2] = np.imag(ref)[:-1]
# Copy Nyquist frequency value into first 
# sample.This is just a storage trick so that the
# output of the RFFT has same length as input
# It is legacy behavior that we need to keep
# for backward compatibility but it is not
# very pretty
referenceFloat[1] = np.real(ref[-1])

referenceFixed=np.zeros(2*len(ref))
referenceFixed[0::2] = np.real(ref)
referenceFixed[1::2] = np.imag(ref)

printTitle("RFFT FAST F64")

printSubTitle("RFFT")


rfftf64=dsp.arm_rfft_fast_instance_f64()
status=dsp.arm_rfft_fast_init_f64(rfftf64,nb)
result = dsp.arm_rfft_fast_f64(rfftf64,signal,0)
assert(len(signal) == RFFT_F_IN_LENGTH)
assert(len(result) == RFFT_F_OUT_LENGTH)

assert_allclose(referenceFloat,result)

printSubTitle("RIFFT")

rifftf64=dsp.arm_rfft_fast_instance_f64()
status=dsp.arm_rfft_fast_init_f64(rifftf64,nb)
result = dsp.arm_rfft_fast_f64(rifftf64,referenceFloat,1)
assert(len(referenceFloat) == RIFFT_F_IN_LENGTH)
assert(len(result) == RIFFT_F_OUT_LENGTH)

assert_allclose(invref,result,atol=1e-15)

printTitle("RFFT FAST F32")

printSubTitle("RFFT")


rfftf32=dsp.arm_rfft_fast_instance_f32()
status=dsp.arm_rfft_fast_init_f32(rfftf32,nb)
result = dsp.arm_rfft_fast_f32(rfftf32,signal,0)
assert(len(signal) == RFFT_F_IN_LENGTH)
assert(len(result) == RFFT_F_OUT_LENGTH)


assert_allclose(referenceFloat,result,rtol=3e-6)

printSubTitle("RIFFT")

rifftf32=dsp.arm_rfft_fast_instance_f32()
status=dsp.arm_rfft_fast_init_f32(rifftf32,nb)
result = dsp.arm_rfft_fast_f32(rifftf32,referenceFloat,1)
assert(len(referenceFloat) == RIFFT_F_IN_LENGTH)
assert(len(result) == RIFFT_F_OUT_LENGTH)

assert_allclose(invref,result,atol=1e-7)

# Fixed point

printTitle("RFFT Q31")

printSubTitle("RFFT")

signalQ31 = f.toQ31(signal)
rfftQ31=dsp.arm_rfft_instance_q31()
status=dsp.arm_rfft_init_q31(rfftQ31,nb,0,1)
resultQ31 = dsp.arm_rfft_q31(rfftQ31,signalQ31)
assert(len(signalQ31) == RFFT_Q_IN_LENGTH)
assert(len(resultQ31) == RFFT_Q_OUT_LENGTH)
compareWithConjugatePart(resultQ31)

# Drop the conjugate part which is not computed by scipy
resultQ31 = resultQ31[:nb+2]
assert(len(resultQ31) == RIFFT_Q_IN_LENGTH)

resultF = f.Q31toF32(resultQ31) * nb

assert_allclose(referenceFixed,resultF,rtol=1e-6,atol=1e-6)


printSubTitle("RIFFT")

rifftQ31=dsp.arm_rfft_instance_q31()
status=dsp.arm_rfft_init_q31(rifftQ31,nb,1,1)
# Apply CMSIS-DSP scaling
referenceQ31 = f.toQ31(referenceFixed/ nb) 
resultQ31 = dsp.arm_rfft_q31(rifftQ31,referenceQ31)
resultF = f.Q31toF32(resultQ31)
assert(len(referenceQ31) == RIFFT_Q_IN_LENGTH)
assert(len(resultQ31) == RIFFT_Q_OUT_LENGTH)

assert_allclose(invref/nb,resultF,atol=1e-6)

printTitle("RFFT Q15")

printSubTitle("RFFT")

signalQ15 = f.toQ15(signal)
rfftQ15=dsp.arm_rfft_instance_q15()
status=dsp.arm_rfft_init_q15(rfftQ15,nb,0,1)
resultQ15 = dsp.arm_rfft_q15(rfftQ15,signalQ15)
assert(len(signalQ15) == RFFT_Q_IN_LENGTH)
assert(len(resultQ15) == RFFT_Q_OUT_LENGTH)
compareWithConjugatePart(resultQ15)

# Drop the conjugate part which is not computed by scipy
resultQ15 = resultQ15[:nb+2]
assert(len(resultQ15) == RIFFT_Q_IN_LENGTH)

resultF = f.Q15toF32(resultQ15) * nb

assert_allclose(referenceFixed,resultF,rtol=1e-6,atol=1e-2)


printSubTitle("RIFFT")

rifftQ15=dsp.arm_rfft_instance_q15()
status=dsp.arm_rfft_init_q15(rifftQ15,nb,1,1)
# Apply CMSIS-DSP scaling
referenceQ15 = f.toQ15(referenceFixed / nb) 
resultQ15 = dsp.arm_rfft_q15(rifftQ15,referenceQ15)
resultF = f.Q15toF32(resultQ15)
assert(len(referenceQ15) == RIFFT_Q_IN_LENGTH)
assert(len(resultQ15) == RIFFT_Q_OUT_LENGTH)

assert_allclose(invref/nb,resultF,atol=1e-3)

