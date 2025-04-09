import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *



def printTitle(s):
    pass

def printSubTitle(s):
    pass

class TestMaxMinMethods_Test4(unittest.TestCase):

    def setUp(self):
        self.a=normalize(np.array([1.,-3.,4.,0.,-10.,8.]))
        return super().setUp()
    
    def test_max_f32(self):
        res,idx=dsp.arm_absmax_f32(self.a)
        self.assertAlmostEqual(res,1.0)
        self.assertEqual(idx,4)

    def test_absmax_noidx_q31(self):
        a = toQ31(self.a)
        res=Q31toF32(dsp.arm_absmax_no_idx_q31(a))
        self.assertAlmostEqual(res,1.0)

    def test_max_noidx_q31(self):
        a = toQ31(self.a)
        res=Q31toF32(dsp.arm_max_no_idx_q31(a))
        self.assertAlmostEqual(res,0.8)

    def test_absmax_noidx_q15(self):
        a = toQ15(self.a)
        res=Q15toF32(dsp.arm_absmax_no_idx_q15(a))
        self.assertAlmostEqual(res,0.99997,places=5)

    def test_max_noidx_q15(self):
        a = toQ15(self.a)
        res=Q15toF32(dsp.arm_max_no_idx_q15(a))
        self.assertAlmostEqual(res,0.799988,places=5)

    def test_absmax_noidx_q7(self):
        a = toQ7(self.a)
        res=Q7toF32(dsp.arm_absmax_no_idx_q7(a))
        self.assertAlmostEqual(res,0.99219,places=5)

    def test_max_noidx_q7(self):
        a = toQ7(self.a)
        res=Q7toF32(dsp.arm_max_no_idx_q7(a))
        self.assertAlmostEqual(res,0.7969,places=3)

    def test_min_f32(self):
        res, idx = dsp.arm_min_f32(self.a)
        self.assertAlmostEqual(res, -1.0)
        self.assertEqual(idx, 4)

    def test_absmin_noidx_q31(self):
        a = toQ31(self.a)
        res = Q31toF32(dsp.arm_absmin_no_idx_q31(a))
        self.assertAlmostEqual(res, 0.0)

    def test_min_noidx_q31(self):
        a = toQ31(self.a)
        res = Q31toF32(dsp.arm_min_no_idx_q31(a))
        self.assertAlmostEqual(res, -1.0)

    def test_absmin_noidx_q15(self):
        a = toQ15(self.a)
        res = Q15toF32(dsp.arm_absmin_no_idx_q15(a))
        self.assertAlmostEqual(res, 0.0, places=5)

    def test_min_noidx_q15(self):
        a = toQ15(self.a)
        res = Q15toF32(dsp.arm_min_no_idx_q15(a))
        self.assertAlmostEqual(res, -1.0, places=5)

    def test_absmin_noidx_q7(self):
        a = toQ7(self.a)
        res = Q7toF32(dsp.arm_absmin_no_idx_q7(a))
        self.assertAlmostEqual(res, 0.0, places=5)

    def test_min_noidx_q7(self):
        a = toQ7(self.a)
        res = Q7toF32(dsp.arm_min_no_idx_q7(a))
        self.assertAlmostEqual(res, -1.0, places=3)

if __name__ == '__main__':
    unittest.main()
    

