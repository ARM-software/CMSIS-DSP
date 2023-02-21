# Bug corrections for version 1.9
import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f
import numpy as np
import math
import colorama
from colorama import init,Fore, Back, Style
from numpy.testing import assert_allclose
#import matplotlib.pyplot as plt
from scipy import signal

init()

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print("\n" + Style.BRIGHT + s + Style.RESET_ALL)

printTitle("Decimate")

test_length_seconds = 0.1
signal_frequency = 100
sampling_freq = 8000
nbSamples = int(test_length_seconds*sampling_freq)
wave = np.sin(2*np.pi*signal_frequency*np.linspace(0,test_length_seconds,nbSamples))

#plt.plot(wave)
#plt.show()

decimationFactor = 4 

numTaps = 9
downsamplingFilter = signal.firwin(numTaps,1.0 / decimationFactor)
block_size = 160
assert(block_size % decimationFactor == 0)

ds_state = np.zeros(block_size + len(downsamplingFilter)-1)
decimator = dsp.arm_fir_decimate_instance_f32()

status = dsp.arm_fir_decimate_init_f32(decimator,numTaps,decimationFactor, downsamplingFilter, ds_state)

def processSignal(sig,dec,f):
    result = []
    for blockNb in range(len(sig) // block_size):
        s = blockNb * block_size 
        e = s + block_size
    
        r = f(dec,sig[s:e])
        result.append(r)
    output = np.hstack(result)
    return(output)

ref = processSignal(wave,decimator,dsp.arm_fir_decimate_f32)
#plt.plot(ref)
#plt.show()

printSubTitle("Decimate Q31")
waveQ31 = f.toQ31(wave)
downsamplingFilterQ31 = f.toQ31(downsamplingFilter)
stateQ31 = np.zeros(block_size + len(downsamplingFilter)-1)
decimatorQ31 = dsp.arm_fir_decimate_instance_q31()
status = dsp.arm_fir_decimate_init_q31(decimatorQ31,numTaps,decimationFactor, 
         downsamplingFilterQ31, stateQ31)

outputQ31 = processSignal(waveQ31,decimatorQ31,dsp.arm_fir_decimate_q31)
outputF32 = f.Q31toF32(outputQ31)

printSubTitle("Decimate Fast Q31")
waveQ31 = f.toQ31(wave)
downsamplingFilterQ31 = f.toQ31(downsamplingFilter)
stateQ31 = np.zeros(block_size + len(downsamplingFilter)-1)
decimatorQ31 = dsp.arm_fir_decimate_instance_q31()
status = dsp.arm_fir_decimate_init_q31(decimatorQ31,numTaps,decimationFactor, 
         downsamplingFilterQ31, stateQ31)

outputQ31 = processSignal(waveQ31,decimatorQ31,dsp.arm_fir_decimate_fast_q31)
outputF32 = f.Q31toF32(outputQ31)

printSubTitle("Decimate Q15")

waveQ15 = f.toQ15(wave)
downsamplingFilterQ15 = f.toQ15(downsamplingFilter)
stateQ15 = np.zeros(block_size + len(downsamplingFilter)-1)
decimatorQ15 = dsp.arm_fir_decimate_instance_q15()
status = dsp.arm_fir_decimate_init_q15(decimatorQ15,numTaps,decimationFactor, 
         downsamplingFilterQ15, stateQ15)

outputQ15 = processSignal(waveQ15,decimatorQ15,dsp.arm_fir_decimate_q15)
outputF32 = f.Q15toF32(outputQ15)
#plt.plot(outputF32)
#plt.show()
assert_allclose(ref,outputF32,rtol=2e-3,atol=1e-3)

printSubTitle("Decimate Fast Q15")

waveQ15 = f.toQ15(wave)
downsamplingFilterQ15 = f.toQ15(downsamplingFilter)
stateQ15 = np.zeros(block_size + len(downsamplingFilter)-1)
decimatorQ15 = dsp.arm_fir_decimate_instance_q15()
status = dsp.arm_fir_decimate_init_q15(decimatorQ15,numTaps,decimationFactor, 
         downsamplingFilterQ15, stateQ15)

outputQ15 = processSignal(waveQ15,decimatorQ15,dsp.arm_fir_decimate_fast_q15)
outputF32 = f.Q15toF32(outputQ15)
#plt.plot(outputF32)
#plt.show()
assert_allclose(ref,outputF32,rtol=2e-3,atol=1e-3)


printTitle("Interpolate")

upsamplingFactor = 4 

numTaps = 16
upsamplingFilter = signal.firwin(numTaps,1.0 / upsamplingFactor)
assert(numTaps % upsamplingFactor == 0)
block_size = 40


printSubTitle("Interpolate F32")

state = np.zeros(block_size + len(upsamplingFilter)//upsamplingFactor-1)
interpolator = dsp.arm_fir_interpolate_instance_f32()
status = dsp.arm_fir_interpolate_init_f32(interpolator,upsamplingFactor,numTaps, 
         upsamplingFilter, state)

output = processSignal(ref,interpolator,dsp.arm_fir_interpolate_f32)
output = output / np.max(output)
#t = range(nbSamples)
#plt.plot(t,wave,t[:-11],output[11:])
#plt.show()

d = 11
assert_allclose(wave[:-d],output[d:],atol=0.1)

printSubTitle("Interpolate Q31")

upsamplingFilterQ31 = f.toQ31(upsamplingFilter)
stateQ31 = np.zeros(block_size + len(upsamplingFilter)//upsamplingFactor-1)
interpolatorQ31 = dsp.arm_fir_interpolate_instance_q31()
status = dsp.arm_fir_interpolate_init_q31(interpolatorQ31,upsamplingFactor,numTaps, 
         upsamplingFilterQ31, stateQ31)

outputQ31 = processSignal(outputQ31,interpolatorQ31,dsp.arm_fir_interpolate_q31)
outputF32 = f.Q31toF32(outputQ31)
outputF32 = outputF32 / np.max(outputF32)

assert_allclose(wave[:-d],outputF32[d:],atol=0.1)

printSubTitle("Interpolate Q15")

upsamplingFilterQ15 = f.toQ15(upsamplingFilter)
stateQ15 = np.zeros(block_size + len(upsamplingFilter)//upsamplingFactor-1)
interpolatorQ15 = dsp.arm_fir_interpolate_instance_q15()
status = dsp.arm_fir_interpolate_init_q15(interpolatorQ15,upsamplingFactor,numTaps, 
         upsamplingFilterQ15, stateQ15)

outputQ15 = processSignal(outputQ15,interpolatorQ15,dsp.arm_fir_interpolate_q15)
outputF32 = f.Q15toF32(outputQ15)
outputF32 = outputF32 / np.max(outputF32)

assert_allclose(wave[:-d],outputF32[d:],atol=0.1)
