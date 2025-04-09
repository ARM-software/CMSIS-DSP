import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

import numpy as np
import math

def mse(a,b):
    err = a - b
    return(np.dot(err,err) / len(a))

class TestFastMathMethods_Test7(unittest.TestCase):

    def setUp(self):
        self.angles=[0,
           math.pi/4,
           math.pi/2,
           3*math.pi/4,
           math.pi,
           5*math.pi/4,
           3*math.pi/2,
           7*math.pi/4
           ]

        self.x = np.cos(self.angles)
        self.y = np.sin(self.angles)

        self.vals=list(zip(self.y,self.x))

        self.atanref=np.array([np.arctan2(yv,xv) for (yv,xv) in self.vals])/math.pi*180


        NBSAMPLES = 50

        a=np.random.randn(NBSAMPLES)
        self.a = a / np.max(np.abs(a))
        
        b=np.random.randn(NBSAMPLES)
        self.b = b / np.max(np.abs(b))

        self.mseref = mse(self.a,self.b)

        return super().setUp()

    def test_atan2_f32(self):
        res=np.array([dsp.arm_atan2_f32(yv,xv)[1]  for (yv,xv) in self.vals])/math.pi*180
        assert_allclose(res,self.atanref,1e-6,1e-6)

    def test_atan2_q31(self):
        xQ31=toQ31(self.x)
        yQ31=toQ31(self.y)
        valsQ31=list(zip(yQ31,xQ31))

        res=4*Q31toF32(np.array([dsp.arm_atan2_q31(yv,xv)[1]  for (yv,xv) in valsQ31]))/math.pi*180
        assert_allclose(res,self.atanref,1e-8,1e-8)

    def test_atan2_q15(self):
        xQ15=toQ15(self.x)
        yQ15=toQ15(self.y)
        valsQ15=list(zip(yQ15,xQ15))

        res=4*Q15toF32(np.array([dsp.arm_atan2_q15(yv,xv)[1]  for (yv,xv) in valsQ15]))/math.pi*180
        assert_allclose(res,self.atanref,1e-3,1e-3)

    def test_mse_f64(self):
        res = dsp.arm_mse_f64(self.a,self.b)
        assert_allclose(res,self.mseref,1e-14,1e-14)

    def test_mse_f32(self):
        res = dsp.arm_mse_f32(self.a,self.b)
        assert_allclose(res,self.mseref,1e-7,1e-7)

    def test_mse_q31(self):
        aQ31 = toQ31(self.a)
        bQ31 = toQ31(self.b)

        res = dsp.arm_mse_q31(aQ31,bQ31)
        res = Q31toF32(res)
        assert_allclose(res,self.mseref,1e-7,1e-7)

    def test_mse_q15(self):
        aQ15 = toQ15(self.a)
        bQ15 = toQ15(self.b)

        res = dsp.arm_mse_q15(aQ15,bQ15)
        res = Q15toF32(res)
        assert_allclose(res,self.mseref,1e-4,1e-4)

    def test_mse_q7(self):
        aQ7 = toQ7(self.a)
        bQ7 = toQ7(self.b)

        res = dsp.arm_mse_q7(aQ7,bQ7)
        res = Q7toF32(res)
        assert_allclose(res,self.mseref,1e-2,1e-2)

if __name__ == '__main__':
    unittest.main()