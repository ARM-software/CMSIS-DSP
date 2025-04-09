import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

import cmsisdsp.mfcc as mfcc
import scipy.signal as sig
from mfccdebugdata import *
from cmsisdsp.datatype import F32,Q31,Q15

import cmsisdsp.datatype as dt

class TestMFCC_MFCC(unittest.TestCase):

    def setUp(self):
        self.sample_rate = 16000
        self.FFTSize = 256
        self.numOfDctOutputs = 13
            
        self.freq_min = 64
        self.freq_high = self.sample_rate / 2
        self.numOfMelFilters = 20
        return super().setUp()
    
    def test_mfcc_f32(self):
        mfccf32=dsp.arm_mfcc_instance_f32()
        window = sig.windows.hamming(self.FFTSize, sym=False)
      
        filtLen,filtPos,packedFilters = mfcc.melFilterMatrix(F32,
                                                             self.freq_min, 
                                                             self.freq_high, 
                                                             self.numOfMelFilters,
                                                             self.sample_rate,
                                                             self.FFTSize)

       
        dctMatrixFilters = mfcc.dctMatrix(F32,self.numOfDctOutputs, self.numOfMelFilters)




        status=dsp.arm_mfcc_init_f32(mfccf32,
                                     self.FFTSize,
                                     self.numOfMelFilters,
                                     self.numOfDctOutputs,
                                     dctMatrixFilters,
                                     filtPos,filtLen,packedFilters,window)

        
     
        tmp_nb = dsp.arm_mfcc_tmp_buffer_size(dt.F32,self.FFTSize,1)
        tmp = np.zeros(tmp_nb,dtype=np.float32)
        if dsp.has_neon():
           tmp2_nb = dsp.arm_mfcc_tmp_buffer_size(dt.F32,self.FFTSize,2)
           tmp2 = np.zeros(tmp2_nb,dtype=np.float32)
           
           res=dsp.arm_mfcc_f32(mfccf32,debug,tmp,tmp2=tmp2)
        else:
           res=dsp.arm_mfcc_f32(mfccf32,debug,tmp)
        assert_allclose(res,ref,3e-6,3e-6)


        self.assertEqual(status,0)
        self.assertEqual(mfccf32.fftLen(),self.FFTSize)
        self.assertEqual(mfccf32.nbMelFilters(),self.numOfMelFilters)
        self.assertEqual(mfccf32.nbDctOutputs(),self.numOfDctOutputs)

    def test_mfcc_q31(self):
        mfccq31=dsp.arm_mfcc_instance_q31()

        windowQ31 = dt.convert(sig.windows.hamming(self.FFTSize, sym=False),Q31)
        filtLen,filtPos,packedFiltersQ31 = mfcc.melFilterMatrix(Q31,
                                                                self.freq_min, 
                                                                self.freq_high, 
                                                                self.numOfMelFilters,
                                                                self.sample_rate,
                                                                self.FFTSize)
        dctMatrixFiltersQ31 = mfcc.dctMatrix(Q31,self.numOfDctOutputs, self.numOfMelFilters)


        status=dsp.arm_mfcc_init_q31(mfccq31,self.FFTSize,self.numOfMelFilters,self.numOfDctOutputs,
            dctMatrixFiltersQ31,
            filtPos,filtLen,packedFiltersQ31,windowQ31)
        self.assertEqual(status,0)


        debugQ31 = toQ31(debug)

        tmp_nb = dsp.arm_mfcc_tmp_buffer_size(dt.Q31,self.FFTSize,1)
        tmp = np.zeros(tmp_nb,dtype=np.int32)

        if dsp.has_neon():
           tmp2_nb = dsp.arm_mfcc_tmp_buffer_size(dt.Q31,self.FFTSize,2)
           tmp2 = np.zeros(tmp2_nb,dtype=np.int32)
           errorStatus,resQ31=dsp.arm_mfcc_q31(mfccq31,debugQ31,tmp,tmp2=tmp2)
        else:
           errorStatus,resQ31=dsp.arm_mfcc_q31(mfccq31,debugQ31,tmp)
        res=self.FFTSize*Q31toF32(resQ31)

        assert_allclose(res,ref,4e-5,4e-5)


        self.assertEqual(status,0)
        self.assertEqual(errorStatus,0)
        self.assertEqual(mfccq31.fftLen(),self.FFTSize)
        self.assertEqual(mfccq31.nbMelFilters(),self.numOfMelFilters)
        self.assertEqual(mfccq31.nbDctOutputs(),self.numOfDctOutputs)

    def test_mfcc_q15(self):
        mfccq15=dsp.arm_mfcc_instance_q15()

        windowQ15 = dt.convert(sig.windows.hamming(self.FFTSize, sym=False),Q15)
        filtLen,filtPos,packedFiltersQ15 = mfcc.melFilterMatrix(Q15,
                                                                self.freq_min, 
                                                                self.freq_high, 
                                                                self.numOfMelFilters,
                                                                self.sample_rate,
                                                                self.FFTSize)
        dctMatrixFiltersQ15 = mfcc.dctMatrix(Q15,self.numOfDctOutputs, self.numOfMelFilters)


        status=dsp.arm_mfcc_init_q15(mfccq15,self.FFTSize,self.numOfMelFilters,self.numOfDctOutputs,
            dctMatrixFiltersQ15,
            filtPos,filtLen,packedFiltersQ15,windowQ15)
        self.assertEqual(status,0)

        

        debugQ15 = toQ15(debug)

        tmp_nb = dsp.arm_mfcc_tmp_buffer_size(dt.Q15,self.FFTSize,1)
        tmp = np.zeros(tmp_nb,dtype=np.int16)

        if dsp.has_neon():
           tmp2_nb = dsp.arm_mfcc_tmp_buffer_size(dt.Q15,self.FFTSize,2)
           tmp2 = np.zeros(tmp2_nb,dtype=np.int16)
           errorStatus,resQ15=dsp.arm_mfcc_q15(mfccq15,debugQ15,tmp,tmp2=tmp2)
        else:
           errorStatus,resQ15=dsp.arm_mfcc_q15(mfccq15,debugQ15,tmp)
        res=self.FFTSize*Q15toF32(resQ15)

        assert_allclose(res,ref,3e-3,1e-2)


        self.assertEqual(status,0)
        self.assertEqual(errorStatus,0)
        self.assertEqual(mfccq15.fftLen(),self.FFTSize)
        self.assertEqual(mfccq15.nbMelFilters(),self.numOfMelFilters)
        self.assertEqual(mfccq15.nbDctOutputs(),self.numOfDctOutputs)

if __name__ == '__main__':
    unittest.main()