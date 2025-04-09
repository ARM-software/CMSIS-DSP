# Bug corrections for version 1.9
import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

from numpy.testing import assert_allclose
#import matplotlib.pyplot as plt
from scipy import signal

def processSignal(block_size,sig,dec,f):
    result = []
    for blockNb in range(len(sig) // block_size):
        s = blockNb * block_size 
        e = s + block_size
    
        r = f(dec,sig[s:e])
        result.append(r)
    output = np.hstack(result)
    return(output)

class TestFilterMethods_Test9(unittest.TestCase):

    def setUp(self):
        test_length_seconds = 0.1
        signal_frequency = 100
        sampling_freq = 8000
        self.nbSamples = int(test_length_seconds*sampling_freq)
        self.wave = np.sin(2*np.pi*signal_frequency*np.linspace(0,test_length_seconds,self.nbSamples))
        self.decimationFactor = 4 

        self.numTaps = 9
        self.downsamplingFilter = signal.firwin(self.numTaps,1.0 / self.decimationFactor)
        self.block_size = 160

        ref = signal.lfilter(self.downsamplingFilter, [1], self.wave)
        self.ref = ref[::self.decimationFactor]

        self.upsamplingFactor = 4 
   
        self.up_numTaps = 16
        self.upsamplingFilter = signal.firwin(self.up_numTaps,1.0 / self.upsamplingFactor)
        self.assertTrue(self.up_numTaps % self.upsamplingFactor == 0)
        self.up_block_size = 40

        input = np.zeros(self.wave.shape[0] * self.upsamplingFactor,dtype=np.float32)
        input[::self.upsamplingFactor] = self.wave
        self.upref = signal.lfilter(self.upsamplingFilter, [1], input)
        
        return super().setUp()
     
    def test_decimate_f32(self):
        self.assertTrue(self.block_size % self.decimationFactor == 0)

        ds_state = np.zeros(self.block_size + len(self.downsamplingFilter)-1)
        decimator = dsp.arm_fir_decimate_instance_f32()

        status = dsp.arm_fir_decimate_init_f32(decimator,
                                               self.numTaps,
                                               self.decimationFactor, 
                                               self.downsamplingFilter, 
                                               ds_state)

        self.assertTrue(status == 0)

        res = processSignal(self.block_size,self.wave,decimator,dsp.arm_fir_decimate_f32)
        assert_allclose(self.ref,res,rtol=1e-6)

    def test_decimate_q31(self):
        self.assertTrue(self.block_size % self.decimationFactor == 0)

        waveQ31 = toQ31(self.wave)
        downsamplingFilterQ31 = toQ31(self.downsamplingFilter)
        stateQ31 = np.zeros(self.block_size + len(self.downsamplingFilter)-1)
        decimatorQ31 = dsp.arm_fir_decimate_instance_q31()
        status = dsp.arm_fir_decimate_init_q31(decimatorQ31,self.numTaps,self.decimationFactor, 
                 downsamplingFilterQ31, stateQ31)
        
        self.assertTrue(status == 0)
        
        outputQ31 = processSignal(self.block_size,waveQ31,decimatorQ31,dsp.arm_fir_decimate_q31)
        res = Q31toF32(outputQ31)
        assert_allclose(self.ref,res,rtol=1e-6)

    def test_decimate_fast_q31(self):
        self.assertTrue(self.block_size % self.decimationFactor == 0)

        waveQ31 = toQ31(self.wave)
        downsamplingFilterQ31 = toQ31(self.downsamplingFilter)
        stateQ31 = np.zeros(self.block_size + len(self.downsamplingFilter)-1)
        decimatorQ31 = dsp.arm_fir_decimate_instance_q31()
        status = dsp.arm_fir_decimate_init_q31(decimatorQ31,self.numTaps,self.decimationFactor, 
                 downsamplingFilterQ31, stateQ31)
        
        self.assertTrue(status == 0)
        
        outputQ31 = processSignal(self.block_size,waveQ31,decimatorQ31,dsp.arm_fir_decimate_fast_q31)
        res = Q31toF32(outputQ31)
        assert_allclose(self.ref,res,rtol=1e-6)

    def test_decimate_q15(self):
        self.assertTrue(self.block_size % self.decimationFactor == 0)

        waveQ15 = toQ15(self.wave)
        downsamplingFilterQ15 = toQ15(self.downsamplingFilter)
        stateQ15 = np.zeros(self.block_size + len(self.downsamplingFilter)-1)
        decimatorQ15 = dsp.arm_fir_decimate_instance_q15()
        status = dsp.arm_fir_decimate_init_q15(decimatorQ15,self.numTaps,self.decimationFactor, 
                 downsamplingFilterQ15, stateQ15)
        
        self.assertTrue(status == 0)
        
        outputQ15 = processSignal(self.block_size,waveQ15,decimatorQ15,dsp.arm_fir_decimate_q15)
        res = Q15toF32(outputQ15)
        assert_allclose(self.ref,res,rtol=1e-4,atol=1e-4)

    def test_decimate_fast_q15(self):
        self.assertTrue(self.block_size % self.decimationFactor == 0)

        waveQ15 = toQ15(self.wave)
        downsamplingFilterQ15 = toQ15(self.downsamplingFilter)
        stateQ15 = np.zeros(self.block_size + len(self.downsamplingFilter)-1)
        decimatorQ15 = dsp.arm_fir_decimate_instance_q15()
        status = dsp.arm_fir_decimate_init_q15(decimatorQ15,self.numTaps,self.decimationFactor, 
                 downsamplingFilterQ15, stateQ15)
        
        self.assertTrue(status == 0)
        
        outputQ15 = processSignal(self.block_size,waveQ15,decimatorQ15,dsp.arm_fir_decimate_fast_q15)
        res = Q15toF32(outputQ15)
        assert_allclose(self.ref,res,rtol=1e-4,atol=1e-4)

    def test_interpolate_f32(self):
        state = np.zeros(self.up_block_size + len(self.upsamplingFilter)//self.upsamplingFactor-1)
        interpolator = dsp.arm_fir_interpolate_instance_f32()
        status = dsp.arm_fir_interpolate_init_f32(interpolator,self.upsamplingFactor,self.up_numTaps, 
            self.upsamplingFilter, state)
        
        self.assertTrue(status == 0)
   
        res = processSignal(self.up_block_size,self.wave,interpolator,dsp.arm_fir_interpolate_f32)
        #output = output / np.max(output)
  
        assert_allclose(self.upref,res,rtol=2e-6)

    def test_interpolate_q31(self):
        signalQ31 = toQ31(self.wave)
        upsamplingFilterQ31 = toQ31(self.upsamplingFilter)
        stateQ31 = np.zeros(self.up_block_size + len(self.upsamplingFilter)//self.upsamplingFactor-1)
        interpolatorQ31 = dsp.arm_fir_interpolate_instance_q31()
        status = dsp.arm_fir_interpolate_init_q31(interpolatorQ31,self.upsamplingFactor,self.up_numTaps, 
                 upsamplingFilterQ31, stateQ31)
        
        self.assertTrue(status == 0)
        
        outputQ31 = processSignal(self.up_block_size,signalQ31,interpolatorQ31,dsp.arm_fir_interpolate_q31)
        res = Q31toF32(outputQ31)
        assert_allclose(self.upref,res,rtol=2e-6)

    def test_interpolate_q15(self):
        signalQ15 = toQ15(self.wave)
        upsamplingFilterQ15 = toQ15(self.upsamplingFilter)
        stateQ15 = np.zeros(self.up_block_size + len(self.upsamplingFilter)//self.upsamplingFactor-1)
        interpolatorQ15 = dsp.arm_fir_interpolate_instance_q15()
        status = dsp.arm_fir_interpolate_init_q15(interpolatorQ15,self.upsamplingFactor,self.up_numTaps, 
                 upsamplingFilterQ15, stateQ15)
        
        self.assertTrue(status == 0)
        
        outputQ15 = processSignal(self.up_block_size,signalQ15,interpolatorQ15,dsp.arm_fir_interpolate_q15)
        res = Q15toF32(outputQ15)
        assert_allclose(self.upref,res,rtol=1e-4,atol=1e-4)
        


if __name__ == '__main__':
    unittest.main()
