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

print(f"ref length = {len(ref)}")
print(ref)

# Convert ref to CMSIS-DSP format 
referenceFloat=np.zeros(2*len(ref))
print(f"referenceFloat length = {len(referenceFloat)}")
# Replace complex datatype by real datatype
referenceFloat[0::2] = np.real(ref)
referenceFloat[1::2] = np.imag(ref)
# Copy Nyquist frequency value into first 
# sample.This is just a storage trick so that the
# output of the RFFT has same length as input
# It is legacy behavior that we need to keep
# for backward compatibility but it is not
# very pretty
#referenceFloat[1] = np.real(ref[-1])

rifftQ31=dsp.arm_rfft_instance_q31()
status=dsp.arm_rfft_init_q31(rifftQ31,nb,1,1)
# Apply CMSIS-DSP scaling
referenceQ31 = f.toQ31(referenceFloat / nb) 

resultQ31 = dsp.arm_rfft_q31(rifftQ31,referenceQ31)
resultF = f.Q31toF32(resultQ31)

print(f"resultF length = {len(resultF)}")
assert_allclose(invref/nb,resultF,atol=1e-6)

signalQ31 = f.toQ31(signal)
rfftQ31=dsp.arm_rfft_instance_q31()
status=dsp.arm_rfft_init_q31(rfftQ31,nb,0,1)
resultQ31 = dsp.arm_rfft_q31(rfftQ31,signalQ31)
print(len(resultQ31))
print(2*nb)
resultF = f.Q31toF32(resultQ31) * nb

def compareWithConjugatePart(r):
    res = r[0::2] + 1j * r[1::2]
    conjPart = res[nb:nb//2:-1].conj()
    refPart = res[1:nb//2]
    assert(np.equal(refPart , conjPart).all())

compareWithConjugatePart(resultF)

res = resultF[0::2] + 1j * resultF[1::2]
print(res)

print(res[0:nb//2+1])
print(res[0:nb//2+1].shape)