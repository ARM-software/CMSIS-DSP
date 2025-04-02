import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f

import numpy as np
from scipy import signal
#import matplotlib.pyplot as plt
import scipy.fft


import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *



def chop(A, eps = 1e-6):
    B = np.copy(A)
    B[np.abs(A) < eps] = 0
    return B

# For fixed point version, compare that
# the conjugate part is really the conjugate part
def compareWithConjugatePart(nb,r):
    res = r[0::2] + 1j * r[1::2]
    conjPart = res[nb:nb//2:-1].conj()
    refPart = res[1:nb//2]
    assert(np.equal(refPart , conjPart).all())


class TestRFFTMethods_RFFT(unittest.TestCase):

     def setUp(self):
        self.nb = 32
        self.signal = np.cos(2 * np.pi * np.arange(self.nb) / self.nb)*np.cos(0.2*2 * np.pi * np.arange(self.nb) / self.nb)
        
        self.ref=scipy.fft.rfft(self.signal)
        self.invref = scipy.fft.irfft(self.ref)
        # Length of arrays for the float implementation
        # of the RFFT (in float so there is a factor 2
        # when the samples are complex)
        
        self.RFFT_F_IN_LENGTH = self.nb # real
        self.RFFT_F_OUT_LENGTH = self.nb # complex (so nb // 2 complex)
        
        self.RIFFT_F_IN_LENGTH = self.nb # complex
        self.RIFFT_F_OUT_LENGTH = self.nb # real
        
        # Length of arrays for the fixed point implementation
        # of the RFFT
        self.RFFT_Q_IN_LENGTH = self.nb 
        self.RFFT_Q_OUT_LENGTH = 2*self.nb 
        
        # Conjugate part ignored
        self.RIFFT_Q_IN_LENGTH = self.nb + 2
        self.RIFFT_Q_OUT_LENGTH = self.nb

        # Convert ref to CMSIS-DSP format 
        self.referenceFloat=np.zeros(self.nb)
        # Replace complex datatype by real datatype
        self.referenceFloat[0::2] = np.real(self.ref)[:-1]
        self.referenceFloat[1::2] = np.imag(self.ref)[:-1]
        # Copy Nyquist frequency value into first 
        # sample.This is just a storage trick so that the
        # output of the RFFT has same length as input
        # It is legacy behavior that we need to keep
        # for backward compatibility but it is not
        # very pretty
        self.referenceFloat[1] = np.real(self.ref[-1])

        self.referenceFixed=np.zeros(2*len(self.ref))
        self.referenceFixed[0::2] = np.real(self.ref)
        self.referenceFixed[1::2] = np.imag(self.ref)

        self.assertTrue(len(self.ref) == (self.nb // 2) + 1)
        self.assertTrue(len(self.invref) == self.nb)

        return super().setUp()
     
     def test_rfft_f64(self):
        rfftf64=dsp.arm_rfft_fast_instance_f64()
        status=dsp.arm_rfft_fast_init_f64(rfftf64,self.nb)
        self.assertTrue(status == 0)
        result = dsp.arm_rfft_fast_f64(rfftf64,self.signal,0)
        assert(len(self.signal) == self.RFFT_F_IN_LENGTH)
        assert(len(result) == self.RFFT_F_OUT_LENGTH)
        
        assert_allclose(self.referenceFloat,result)

     def test_rifft_f64(self):
        rifftf64=dsp.arm_rfft_fast_instance_f64()
        status=dsp.arm_rfft_fast_init_f64(rifftf64,self.nb)
        self.assertTrue(status == 0)
        result = dsp.arm_rfft_fast_f64(rifftf64,self.referenceFloat,1)
        assert(len(self.referenceFloat) == self.RIFFT_F_IN_LENGTH)
        assert(len(result) == self.RIFFT_F_OUT_LENGTH)
        assert_allclose(self.invref,result,atol=1e-15)


     def test_rfft_f32(self):
            rfftf32=dsp.arm_rfft_fast_instance_f32()
            status=dsp.arm_rfft_fast_init_f32(rfftf32,self.nb)
            self.assertTrue(status == 0)
            result = dsp.arm_rfft_fast_f32(rfftf32,self.signal,0)
            assert(len(self.signal) == self.RFFT_F_IN_LENGTH)
            assert(len(result) == self.RFFT_F_OUT_LENGTH)
            
            assert_allclose(self.referenceFloat,result,rtol=3e-6)

     def test_rifft_f32(self):
            rifftf32=dsp.arm_rfft_fast_instance_f32()
            status=dsp.arm_rfft_fast_init_f32(rifftf32,self.nb)
            self.assertTrue(status == 0)
            result = dsp.arm_rfft_fast_f32(rifftf32,self.referenceFloat,1)
            assert(len(self.referenceFloat) == self.RIFFT_F_IN_LENGTH)
            assert(len(result) == self.RIFFT_F_OUT_LENGTH)
            assert_allclose(self.invref,result,atol=1e-7)

     def test_rfft_q31(self):
            signalQ31 = f.toQ31(self.signal)
            rfftQ31=dsp.arm_rfft_instance_q31()
            status=dsp.arm_rfft_init_q31(rfftQ31,self.nb,0,1)
            result = dsp.arm_rfft_q31(rfftQ31,signalQ31)
            assert(len(signalQ31) == self.RFFT_Q_IN_LENGTH)
            assert(len(result) == self.RFFT_Q_OUT_LENGTH)
            compareWithConjugatePart(self.nb,result)
            
            # Drop the conjugate part which is not computed by scipy
            result = result[:self.nb+2]
            assert(len(result) == self.RIFFT_Q_IN_LENGTH)
            
            resultF = f.Q31toF32(result) * self.nb
            
            assert_allclose(self.referenceFixed,resultF,rtol=1e-6,atol=1e-6)

     def test_rifft_q31(self):
            rifftQ31=dsp.arm_rfft_instance_q31()
            status=dsp.arm_rfft_init_q31(rifftQ31,self.nb,1,1)
            # Apply CMSIS-DSP scaling
            referenceQ31 = f.toQ31(self.referenceFixed / self.nb) 
            result = dsp.arm_rfft_q31(rifftQ31,referenceQ31)
            resultF = f.Q31toF32(result)
            assert(len(referenceQ31) == self.RIFFT_Q_IN_LENGTH)
            assert(len(result) == self.RIFFT_Q_OUT_LENGTH)
            
            assert_allclose(self.invref/self.nb,resultF,atol=1e-6)

     def test_rfft_q15(self):
            signalQ15 = f.toQ15(self.signal)
            rfftQ15=dsp.arm_rfft_instance_q15()
            status=dsp.arm_rfft_init_q15(rfftQ15,self.nb,0,1)
            result = dsp.arm_rfft_q15(rfftQ15,signalQ15)
            assert(len(signalQ15) == self.RFFT_Q_IN_LENGTH)
            assert(len(result) == self.RFFT_Q_OUT_LENGTH)
            compareWithConjugatePart(self.nb,result)
            
            # Drop the conjugate part which is not computed by scipy
            result = result[:self.nb+2]
            assert(len(result) == self.RIFFT_Q_IN_LENGTH)
            
            resultF = f.Q15toF32(result) * self.nb
            
            assert_allclose(self.referenceFixed,resultF,rtol=1e-6,atol=1e-2)



     def test_rifft_q15(self):
            rifftQ15=dsp.arm_rfft_instance_q15()
            status=dsp.arm_rfft_init_q15(rifftQ15,self.nb,1,1)
            # Apply CMSIS-DSP scaling
            referenceQ15 = f.toQ15(self.referenceFixed / self.nb) 
            result = dsp.arm_rfft_q15(rifftQ15,referenceQ15)
            resultF = f.Q15toF32(result)
            assert(len(referenceQ15) == self.RIFFT_Q_IN_LENGTH)
            assert(len(result) == self.RIFFT_Q_OUT_LENGTH)
            
            assert_allclose(self.invref/self.nb,resultF,atol=1e-3)




