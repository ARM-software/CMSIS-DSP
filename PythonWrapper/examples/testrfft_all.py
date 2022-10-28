import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f

import numpy as np
from scipy import signal
import matplotlib.pyplot as plt
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

nb = 32
signal = np.cos(2 * np.pi * np.arange(nb) / nb)*np.cos(0.2*2 * np.pi * np.arange(nb) / nb)

ref=scipy.fft.rfft(signal)
invref = scipy.fft.irfft(ref)

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

printTitle("RFFT FAST F64")

printSubTitle("RFFT")


rfftf64=dsp.arm_rfft_fast_instance_f64()
status=dsp.arm_rfft_fast_init_f64(rfftf64,nb)
result = dsp.arm_rfft_fast_f64(rfftf64,signal,0)


assert_allclose(referenceFloat,result)

printSubTitle("RIFFT")

rifftf64=dsp.arm_rfft_fast_instance_f64()
status=dsp.arm_rfft_fast_init_f64(rifftf64,nb)
result = dsp.arm_rfft_fast_f64(rifftf64,referenceFloat,1)

assert_allclose(invref,result,atol=1e-15)

printTitle("RFFT FAST F32")

printSubTitle("RFFT")


rfftf32=dsp.arm_rfft_fast_instance_f32()
status=dsp.arm_rfft_fast_init_f32(rfftf32,nb)
result = dsp.arm_rfft_fast_f32(rfftf32,signal,0)


assert_allclose(referenceFloat,result,rtol=3e-6)

printSubTitle("RIFFT")

rifftf32=dsp.arm_rfft_fast_instance_f32()
status=dsp.arm_rfft_fast_init_f32(rifftf32,nb)
result = dsp.arm_rfft_fast_f32(rifftf32,referenceFloat,1)

assert_allclose(invref,result,atol=1e-7)

# Fixed point
# Reference from fixed point arithmetric.
# The RFFT are not packing the Nyquist frequency
# real value in sample 0
referenceFloat=np.zeros(nb+2)
# Replace complex datatype by real datatype
referenceFloat[0::2] = np.real(ref)
referenceFloat[1::2] = np.imag(ref)

printTitle("RFFT Q31")

printSubTitle("RFFT")

signalQ31 = f.toQ31(signal)
rfftQ31=dsp.arm_rfft_instance_q31()
status=dsp.arm_rfft_init_q31(rfftQ31,nb,0,1)
resultQ31 = dsp.arm_rfft_q31(rfftQ31,signalQ31)
# Drop the conjugate part which is not computed by scipy
resultQ31 = resultQ31[:nb+2]
resultF = f.Q31toF32(resultQ31) * nb

assert_allclose(referenceFloat,resultF,rtol=1e-6,atol=1e-6)


printSubTitle("RIFFT")

rifftQ31=dsp.arm_rfft_instance_q31()
status=dsp.arm_rfft_init_q31(rifftQ31,nb,1,1)
# Apply CMSIS-DSP scaling
referenceQ31 = f.toQ31(referenceFloat / nb) 
resultQ31 = dsp.arm_rfft_q31(rifftQ31,referenceFloat)
resultF = f.Q31toF32(resultQ31)

assert_allclose(invref,result,atol=1e-6)

printTitle("RFFT Q15")

printSubTitle("RFFT")

signalQ15 = f.toQ15(signal)
rfftQ15=dsp.arm_rfft_instance_q15()
status=dsp.arm_rfft_init_q15(rfftQ15,nb,0,1)
resultQ15 = dsp.arm_rfft_q15(rfftQ15,signalQ15)
# Drop the conjugate part which is not computed by scipy
resultQ15 = resultQ15[:nb+2]
resultF = f.Q15toF32(resultQ15) * nb

assert_allclose(referenceFloat,resultF,rtol=1e-6,atol=1e-2)


printSubTitle("RIFFT")

rifftQ15=dsp.arm_rfft_instance_q15()
status=dsp.arm_rfft_init_q15(rifftQ15,nb,1,1)
# Apply CMSIS-DSP scaling
referenceQ15 = f.toQ15(referenceFloat / nb) 
resultQ15 = dsp.arm_rfft_q15(rifftQ15,referenceFloat)
resultF = f.Q15toF32(resultQ15)

assert_allclose(invref,result,atol=1e-2)

