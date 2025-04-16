# Bug corrections for version 1.9
import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f
import numpy as np
from numpy.testing import assert_allclose
#import matplotlib.pyplot as plt
import scipy.signal.windows as win

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *


def genwelch(n):
    ik = 2*np.array(range(n)) / n
    w = ik -1;
    w = 1 - w**2 
    if len(w)!=n:
        print("Error with window len in Welch")
        exit(1)
    return(w)


def genbartlett(n):
    w = win.bartlett(n,sym=False)
    return(w)

def genhamming(n):
    w = win.hamming(n,sym=False)
    return(w)

def genhanning(n):
    w = win.hann(n,sym=False)
    return(w)

def gennuttall3(n):
    w = win.general_cosine(n,
        [0.375, 0.5 , 0.125 ],sym=False)
    return(w)

def gennuttall4(n):
    w = win.general_cosine(n,
        [0.3125, 0.46875,0.1875 , 0.03125],sym=False)
    return(w)

def gennuttall3a(n):
    w = win.general_cosine(n,
        [0.40897, 0.5 ,0.09103],sym=False)
    return(w)

def gennuttall3b(n):
    w = win.general_cosine(n,
        [0.4243801, 0.4973406 , 0.0782793 ],sym=False)
    return(w)

def gennuttall4a(n):
    w = win.general_cosine(n,
        [0.338946, 0.481973,0.161054 , 0.018027 ],sym=False)
    return(w)

def genblackman_harris_92db(n):
    w = win.blackmanharris(n,sym=False)
    return(w)

def gennuttall4b(n):
    w = win.general_cosine(n,
        [0.355768, 0.487396 ,
        0.144232 , 0.012604 ],sym=False)
    return(w)

def gennuttall4c(n):
    w = win.nuttall(n,sym=False)
    return(w)

def genhft90d(n):
    w = win.general_cosine(n,
        [1 ,1.942604 ,
         1.340318 , 0.440811 , 0.043097 ],sym=False)
    return(w)

def genhft95(n):
    w = win.general_cosine(n,
        [1, 1.9383379 ,
         1.3045202 ,0.4028270 ,0.0350665 ]
 ,sym=False)
    return(w)

def genhft116d(n):
    w = win.general_cosine(n,
        [1, 1.9575375 ,
         1.4780705 ,0.6367431 ,
         0.1228389 ,0.0066288 ]
 ,sym=False)
    return(w)

def genhft144d(n):
    w = win.general_cosine(n,
        [1 ,1.96760033 ,
         1.57983607 , 0.81123644 ,
         0.22583558 ,0.02773848 , 0.00090360  ]
 ,sym=False)
    return(w)

def genhft169d(n):
    w = win.general_cosine(n,
        [1, 1.97441843 ,
         1.65409889 , 0.95788187 ,
         0.33673420 , 0.06364622 ,
         0.00521942 ,0.00010599   ]
 ,sym=False)
    return(w)

def genhft196d(n):
    w = win.general_cosine(n,
        [1, 1.979280420 ,
         1.710288951 , 1.081629853 ,
         0.448734314 , 0.112376628 ,
         0.015122992 ,0.000871252 , 0.000011896 ]
 ,sym=False)
    return(w)

def genhft223d(n):
    w = win.general_cosine(n,
        [1, 1.98298997309,
         1.75556083063 , 1.19037717712 ,
         0.56155440797 , 0.17296769663 ,
         0.03233247087 ,0.00324954578 ,
         0.00013801040 ,0.00000132725 ]
 ,sym=False)
    return(w)

def genhft248d(n):
    w = win.general_cosine(n,
        [1, 1.985844164102 ,
         1.791176438506 , 1.282075284005 ,
         0.667777530266 , 0.240160796576 ,
         0.056656381764 ,0.008134974479 ,
         0.000624544650 ,0.000019808998 ,
         0.000000132974 ]
 ,sym=False)
    return(w)

class TestWindowMethods_Test11(unittest.TestCase):

    def setUp(self):
        return super().setUp()

    def test_welch_f32(self):
        ref = genwelch(128)
        wf32 = dsp.arm_welch_f32(128)
        assert_allclose(ref, wf32)

    def test_welch_f64(self):
        ref = genwelch(128)
        wf64 = dsp.arm_welch_f64(128)
        assert_allclose(ref, wf64)

    def test_bartlett(self):
        ref = genbartlett(128)
        wf32 = dsp.arm_bartlett_f32(128)
        assert_allclose(ref, wf32)
        wf64 = dsp.arm_bartlett_f64(128)
        assert_allclose(ref, wf64)

    def test_hamming(self):
        ref = genhamming(128)
        wf32 = dsp.arm_hamming_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hamming_f64(128)
        assert_allclose(ref, wf64)

    def test_hanning(self):
        ref = genhanning(128)
        wf32 = dsp.arm_hanning_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hanning_f64(128)
        assert_allclose(ref, wf64)

    def test_nuttall3(self):
        ref = gennuttall3(128)
        wf32 = dsp.arm_nuttall3_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_nuttall3_f64(128)
        assert_allclose(ref, wf64)

    def test_nuttall4(self):
        ref = gennuttall4(128)
        wf32 = dsp.arm_nuttall4_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_nuttall4_f64(128)
        assert_allclose(ref, wf64)

    def test_nuttall3a(self):
        ref = gennuttall3a(128)
        wf32 = dsp.arm_nuttall3a_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_nuttall3a_f64(128)
        assert_allclose(ref, wf64)

    def test_nuttall3b(self):
        ref = gennuttall3b(128)
        wf32 = dsp.arm_nuttall3b_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_nuttall3b_f64(128)
        assert_allclose(ref, wf64)

    def test_nuttall4a(self):
        ref = gennuttall4a(128)
        wf32 = dsp.arm_nuttall4a_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_nuttall4a_f64(128)
        assert_allclose(ref, wf64, rtol=3e-16, atol=1e-16)

    def test_blackman_harris(self):
        ref = genblackman_harris_92db(128)
        wf32 = dsp.arm_blackman_harris_92db_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_blackman_harris_92db_f64(128)
        assert_allclose(ref, wf64)

    def test_nuttall4b(self):
        ref = gennuttall4b(128)
        wf32 = dsp.arm_nuttall4b_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_nuttall4b_f64(128)
        assert_allclose(ref, wf64, rtol=3e-16, atol=1e-16)

    def test_nuttall4c(self):
        ref = gennuttall4c(128)
        wf32 = dsp.arm_nuttall4c_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_nuttall4c_f64(128)
        assert_allclose(ref, wf64)

    def test_hft90d(self):
        ref = genhft90d(128)
        wf32 = dsp.arm_hft90d_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hft90d_f64(128)
        assert_allclose(ref, wf64, rtol=1e-15, atol=1e-15)

    def test_hft95(self):
        ref = genhft95(128)
        wf32 = dsp.arm_hft95_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hft95_f64(128)
        assert_allclose(ref, wf64)

    def test_hft116d(self):
        ref = genhft116d(128)
        wf32 = dsp.arm_hft116d_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hft116d_f64(128)
        assert_allclose(ref, wf64, rtol=3e-15, atol=3e-15)

    def test_hft144d(self):
        ref = genhft144d(128)
        wf32 = dsp.arm_hft144d_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hft144d_f64(128)
        assert_allclose(ref, wf64, rtol=3e-15, atol=3e-15)

    def test_hft196d(self):
        ref = genhft196d(128)
        wf32 = dsp.arm_hft196d_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hft196d_f64(128)
        assert_allclose(ref, wf64, rtol=2e-15, atol=2e-15)

    def test_hft223d(self):
        ref = genhft223d(128)
        wf32 = dsp.arm_hft223d_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hft223d_f64(128)
        assert_allclose(ref, wf64, rtol=3e-15, atol=3e-15)

    def test_hft248d(self):
        ref = genhft248d(128)
        wf32 = dsp.arm_hft248d_f32(128)
        assert_allclose(ref, wf32, rtol=1e-6, atol=2e-6)
        wf64 = dsp.arm_hft248d_f64(128)
        assert_allclose(ref, wf64, rtol=3e-15, atol=3e-15)


if __name__ == '__main__':
    unittest.main()