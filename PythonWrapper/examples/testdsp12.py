import cmsisdsp as dsp
import cmsisdsp.datatype as dt
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

class TestBufferSizeMethods_Test12(unittest.TestCase):

    def setUp(self):
        self.NBSAMPLES=256
        return super().setUp()
     
    def test_arm_cfft_tmp_buffer_size_f32(self):
        res = dsp.arm_cfft_tmp_buffer_size(dt.F32,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, 2 * self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_cfft_tmp_buffer_size_f16(self):
        res = dsp.arm_cfft_tmp_buffer_size(dt.F16,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, 2 * self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_cfft_tmp_buffer_size_q31(self):
        res = dsp.arm_cfft_tmp_buffer_size(dt.Q31,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, 2 * self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_cfft_tmp_buffer_size_q15(self):
        res = dsp.arm_cfft_tmp_buffer_size(dt.Q15,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, 2 * self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_cfft_output_buffer_size_f32(self):
        res = dsp.arm_cfft_output_buffer_size(dt.F32,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)

    def test_arm_cfft_output_buffer_size_f16(self):
        res = dsp.arm_cfft_output_buffer_size(dt.F16,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)

    def test_arm_cfft_output_buffer_size_q31(self):
        res = dsp.arm_cfft_output_buffer_size(dt.Q31,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)

    def test_arm_cfft_output_buffer_size_q15(self):
        res = dsp.arm_cfft_output_buffer_size(dt.Q15,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)

    def test_arm_cifft_output_buffer_size_f32(self):
        res = dsp.arm_cifft_output_buffer_size(dt.F32,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)

    def test_arm_cifft_output_buffer_size_f16(self):
        res = dsp.arm_cifft_output_buffer_size(dt.F16,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)

    def test_arm_cifft_output_buffer_size_q31(self):
        res = dsp.arm_cifft_output_buffer_size(dt.Q31,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)

    def test_arm_cifft_output_buffer_size_q15(self):
        res = dsp.arm_cifft_output_buffer_size(dt.Q15,self.NBSAMPLES)
        self.assertEqual(res, 2 * self.NBSAMPLES)
        
    def test_arm_rfft_tmp_buffer_size_f32(self):
        res = dsp.arm_rfft_tmp_buffer_size(dt.F32,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_rfft_tmp_buffer_size_f16(self):
        res = dsp.arm_rfft_tmp_buffer_size(dt.F16,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_rfft_tmp_buffer_size_q31(self):
        res = dsp.arm_rfft_tmp_buffer_size(dt.Q31,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, 2*self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_rfft_tmp_buffer_size_q15(self):
        res = dsp.arm_rfft_tmp_buffer_size(dt.Q15,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, 2*self.NBSAMPLES)
        else:
            self.assertEqual(res, 0)

    def test_arm_rfft_output_buffer_size_f32(self):
        res = dsp.arm_rfft_output_buffer_size(dt.F32,self.NBSAMPLES)
        self.assertEqual(res, self.NBSAMPLES)
        

    def test_arm_rfft_output_buffer_size_f16(self):
        res = dsp.arm_rfft_output_buffer_size(dt.F16,self.NBSAMPLES)
        self.assertEqual(res, self.NBSAMPLES)
       

    def test_arm_rfft_output_buffer_size_q31(self):
        res = dsp.arm_rfft_output_buffer_size(dt.Q31,self.NBSAMPLES)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES+2)
        else:
            self.assertEqual(res, 2*self.NBSAMPLES)

    def test_arm_rfft_output_buffer_size_q15(self):
        res = dsp.arm_rfft_output_buffer_size(dt.Q15,self.NBSAMPLES)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES+2)
        else:
            self.assertEqual(res, 2*self.NBSAMPLES)

    def test_arm_rifft_input_buffer_size_f32(self):
        res = dsp.arm_rifft_input_buffer_size(dt.F32,self.NBSAMPLES)
        self.assertEqual(res, self.NBSAMPLES)
       
    def test_arm_rifft_input_buffer_size_f16(self):
        res = dsp.arm_rifft_input_buffer_size(dt.F16,self.NBSAMPLES)
        self.assertEqual(res, self.NBSAMPLES)

    def test_arm_rifft_input_buffer_size_q31(self):
        res = dsp.arm_rifft_input_buffer_size(dt.Q31,self.NBSAMPLES)
        self.assertEqual(res, self.NBSAMPLES+2)

    def test_arm_rifft_input_buffer_size_q15(self):
        res = dsp.arm_rifft_input_buffer_size(dt.Q15,self.NBSAMPLES)
        self.assertEqual(res, self.NBSAMPLES+2)

    def test_arm_mfcc_tmp_buffer_size_f32(self):
        res = dsp.arm_mfcc_tmp_buffer_size(dt.F32,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES)
            res2 = dsp.arm_mfcc_tmp_buffer_size(dt.F32,self.NBSAMPLES,2)
            self.assertEqual(res2, self.NBSAMPLES)
        else:
            self.assertEqual(res, self.NBSAMPLES)

    def test_arm_mfcc_tmp_buffer_size_f16(self):
        res = dsp.arm_mfcc_tmp_buffer_size(dt.F16,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES)
            res2 = dsp.arm_mfcc_tmp_buffer_size(dt.F16,self.NBSAMPLES,2)
            self.assertEqual(res2, self.NBSAMPLES)
        else:
            self.assertEqual(res, self.NBSAMPLES)

    def test_arm_mfcc_tmp_buffer_size_q31(self):
        res = dsp.arm_mfcc_tmp_buffer_size(dt.Q31,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES+2)
            res2 = dsp.arm_mfcc_tmp_buffer_size(dt.Q31,self.NBSAMPLES,2)
            self.assertEqual(res2, 2*self.NBSAMPLES)
        else:
            self.assertEqual(res, 2*self.NBSAMPLES)

    def test_arm_mfcc_tmp_buffer_size_q15(self):
        res = dsp.arm_mfcc_tmp_buffer_size(dt.Q15,self.NBSAMPLES,1)
        if dsp.has_neon():
            self.assertEqual(res, self.NBSAMPLES+2)
            res2 = dsp.arm_mfcc_tmp_buffer_size(dt.Q15,self.NBSAMPLES,2)
            self.assertEqual(res2, 2*self.NBSAMPLES)
        else:
            self.assertEqual(res, 2*self.NBSAMPLES)
     
if __name__ == '__main__':
    unittest.main()