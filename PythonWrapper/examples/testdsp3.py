import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

class TestFastMathMethods_Test3(unittest.TestCase):

    def setUp(self):
        # Test vlog q31 and q15
        self.x = np.array([0.9,0.5,2**-16])
        self.t=20
   
        self.sinRef=np.sin(self.t * np.pi / 180)
        self.cosRef=np.cos(self.t * np.pi / 180)
        return super().setUp()
    
    def test_vlog_q15(self):
        res=dsp.arm_vlog_q15(toQ15(self.x))
        res=Q15toF32(res)*16.0
        ref=np.log(self.x)
   
        assert_allclose(ref,res,rtol=1e-2)

    def test_vlog_q31(self):
        res=dsp.arm_vlog_q31(toQ31(self.x))
        res=Q31toF32(res)*32.0
        ref=np.log(self.x)
   
        assert_allclose(ref,res,rtol=1e-6)

    def test_sincos_f32(self):
        s,c=dsp.arm_sin_cos_q31(toQ31(self.t/180.0))
        s = Q31toF32(s)
        c = Q31toF32(c)
        assert_allclose(self.sinRef,s,rtol=1e-6)
        assert_allclose(self.cosRef,c,rtol=1e-6)

    def test_sincos_q31(self):
        s,c=dsp.arm_sin_cos_q31(toQ31(self.t/180.0))
        s = Q31toF32(s)
        c = Q31toF32(c)
        assert_allclose(self.sinRef,s,rtol=1e-6)
        assert_allclose(self.cosRef,c,rtol=1e-6)

    def test_sqrt_f32(self):
        a=0.6
        err,r=dsp.arm_sqrt_f32(a)
        self.assertTrue(err==0)
        self.assertAlmostEqual(np.sqrt(a),r,places=6)

    def test_sqrt_q31(self):#
        a=0.6
        err,r=dsp.arm_sqrt_q31(toQ31(a))
        self.assertTrue(err==0)
        self.assertAlmostEqual(np.sqrt(a),Q31toF32(r),places=6)

    def test_sqrt_q15(self):#
        a=0.6
        err,r=dsp.arm_sqrt_q15(toQ15(a))
        self.assertTrue(err==0)
        self.assertAlmostEqual(np.sqrt(a),Q15toF32(r),places=6)

    def test_sqrt_neg_f32(self):
        a=0.6
        err,r=dsp.arm_sqrt_f32(-a)
        self.assertTrue(err==-1)

    def test_sqrt_neg_q31(self):#
        a=0.6
        err,r=dsp.arm_sqrt_q31(toQ31(-a))
        self.assertTrue(err==-1)

    def test_sqrt_neg_q15(self):#
        a=0.6
        err,r=dsp.arm_sqrt_q15(toQ15(-a))
        self.assertTrue(err==-1)

if __name__ == '__main__':
    unittest.main()

