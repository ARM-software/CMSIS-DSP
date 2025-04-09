import cmsisdsp as dsp
import numpy as np
from scipy import signal
from scipy.fftpack import dct 
from pyquaternion import Quaternion

import statsmodels.tsa.stattools

import scipy.spatial

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *



def printTitle(s):
    pass

def printSubTitle(s):
    pass

def genBitvectors(nb,format):
      if format == 31:
         maxVal = 0xffffffff
         dt = np.uint32
      if format == 15:
         maxVal = 0xffff
         dt = np.uint16
      if format == 7:
         maxVal = 0xff 
         dt = np.uint8
  
      #minVal = -maxVal-1
      
      return(np.random.randint(0, maxVal, size=nb,dtype=dt))


def autocorr(x):
    result = np.correlate(x, x, mode='full')
    return result[result.size//2:]

class TestStatsMethods_Test2(unittest.TestCase):

    def setUp(self):
        return super().setUp()
     
    def test_absmax_f32(self):
        a=np.array([1.,-3.,4.,0.,-10.,8.])
        self.assertEqual(dsp.arm_absmax_f32(a), (10.0, 4))

    def test_max_q31(self):
        a=normalize(np.array([0.9,-3.,4.,0.,-10.,8.]))/2.0
        a31 = toQ31(a)
        self.assertEqual(dsp.arm_max_q31(a31), (0x33333333, 5))

    def test_absmax_q31(self):
        a=normalize(np.array([0.9,-3.,4.,0.,-10.,8.]))/2.0
        a31 = toQ31(a)
        self.assertEqual(dsp.arm_absmax_q31(a31), (0x40000000, 4))

    def test_max_q15(self):
        a=normalize(np.array([0.9,-3.,4.,0.,-10.,8.]))/2.0
        a15 = toQ15(a)
        self.assertEqual(dsp.arm_max_q15(a15), (0x3333, 5))

    def test_absmax_q15(self):
        a=normalize(np.array([0.9,-3.,4.,0.,-10.,8.]))/2.0
        a15 = toQ15(a)
        self.assertEqual(dsp.arm_absmax_q15(a15), (0x4000, 4))

    def test_max_q7(self):
        a=normalize(np.array([0.9,-3.,4.,0.,-10.,8.]))/2.0
        a7 = toQ7(a)
        self.assertEqual(dsp.arm_max_q7(a7), (0x33, 5))

    def test_absmax_q7(self):
        a=normalize(np.array([0.9,-3.,4.,0.,-10.,8.]))/2.0
        a7 = toQ7(a)
        self.assertEqual(dsp.arm_absmax_q7(a7), (0x40, 4))

    def test_min_f32(self):
        a = np.array([1., -3., 4., 0., -10., 8.])
        self.assertEqual(dsp.arm_min_f32(a), (-10.0, 4))

    def test_min_q31(self):
        a = normalize(np.array([0.9, -3., 4., 0., -10., 8.])) / 2.0
        a31 = toQ31(a)
        self.assertEqual(dsp.arm_min_q31(a31), (-0x40000000, 4))

    def test_min_q15(self):
        a = normalize(np.array([0.9, -3., 4., 0., -10., 8.])) / 2.0
        a15 = toQ15(a)
        self.assertEqual(dsp.arm_min_q15(a15), (-0x4000, 4))

    def test_min_q7(self):
        a = normalize(np.array([0.9, -3., 4., 0., -10., 8.])) / 2.0
        a7 = toQ7(a)
        self.assertEqual(dsp.arm_min_q7(a7), (-0x40, 4))

    def test_absmin_f32(self):
        a = np.array([1., -3., 4., 0., -10., 8.])
        self.assertEqual(dsp.arm_absmin_f32(a), (0.0, 3))

    def test_absmin_q31(self):
        a = normalize(np.array([0.9, -3., 4., 0., -10., 8.])) / 2.0
        a31 = toQ31(a)
        self.assertEqual(dsp.arm_absmin_q31(a31), (0x0, 3))

    def test_absmin_q15(self):
        a = normalize(np.array([0.9, -3., 4., 0., -10., 8.])) / 2.0
        a15 = toQ15(a)
        self.assertEqual(dsp.arm_absmin_q15(a15), (0x0, 3))

    def test_absmin_q7(self):
        a = normalize(np.array([0.9, -3., 4., 0., -10., 8.])) / 2.0
        a7 = toQ7(a)
        self.assertEqual(dsp.arm_absmin_q7(a7), (0x0, 3))

class TestBasicMethods_Test2(unittest.TestCase):

    def setUp(self):
        return super().setUp()
    
    def test_clip_f32(self):
        a = np.array([1., -3., 4., 0.5, -10., 8.])
        minBound = -5.0
        maxBound = 6.0
        b = dsp.arm_clip_f32(a, minBound, maxBound)
        ref = np.clip(a,minBound,maxBound)
        assert_allclose(b, ref, rtol=1e-5)

    def test_clip_q31(self):
        a = normalize(np.array([1., -3., 4., 0.5, -10., 8.])) / 2.0
        minBound = -0.2
        maxBound = 0.3
        b = Q31toF32(dsp.arm_clip_q31(toQ31(a), toQ31(minBound), toQ31(maxBound)))
        ref = np.clip(a,minBound,maxBound)
        assert_allclose(b, ref, rtol=1e-5)

    def test_clip_q15(self):
        a = normalize(np.array([1., -3., 4., 0.5, -10., 8.]))
        minBound = -0.2
        maxBound = 0.3
        b = Q15toF32(dsp.arm_clip_q15(toQ15(a), toQ15(minBound), toQ15(maxBound)))
        ref = np.clip(a,minBound,maxBound)
        assert_allclose(b, ref, rtol=3e-4)

    def test_clip_q7(self):
        a = normalize(np.array([1., -3., 4., 0.5, -10., 8.]))
        minBound = -0.2
        maxBound = 0.3
        b = Q7toF32(dsp.arm_clip_q7(toQ7(a), toQ7(minBound), toQ7(maxBound)))
        ref = np.clip(a,minBound,maxBound)
        assert_allclose(b, ref, rtol=1e-1)

    def test_fill_f32(self):
        v = 0.22
        nb = 10
        a = np.full((nb,), v)
        b = dsp.arm_fill_f32(v, nb)
        assert_allclose(a, b, rtol=1e-5)

    def test_fill_q31(self):
        v = 0.22
        nb = 10
        a = toQ31(np.full((nb,), v))
        b = dsp.arm_fill_q31(toQ31(v), nb)
        assert_equal(a, b)

    def test_fill_q15(self):
        v = 0.22
        nb = 10
        a = toQ15(np.full((nb,), v))
        b = dsp.arm_fill_q15(toQ15(v), nb)
        assert_equal(a, b)

    def test_fill_q7(self):
        v = 0.22
        nb = 10
        a = toQ7(np.full((nb,), v))
        b = dsp.arm_fill_q7(toQ7(v), nb)
        assert_equal(a, b)



class TestMatrixMethods_Test2(unittest.TestCase):

    def setUp(self):
        return super().setUp()
    
    def test_mat_vec_mult_f32(self):
        a = np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]])
        b = np.array([-2, -1, 3, 4])
        c = dsp.arm_mat_vec_mult_f32(a, b)
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=1e-5)

    def test_mat_vec_mult_q31(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([-2, -1, 3, 4]))/2.0
        c = Q31toF32(dsp.arm_mat_vec_mult_q31(toQ31(a), toQ31(b)))
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=1e-5)

    def test_mat_vec_mult_q15(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([-2, -1, 3, 4]))/2.0
        c = Q15toF32(dsp.arm_mat_vec_mult_q15(toQ15(a), toQ15(b)))
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=3e-4)

    def test_mat_vec_mult_q7(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([-2, -1, 3, 4]))/2.0
        c = Q7toF32(dsp.arm_mat_vec_mult_q7(toQ7(a), toQ7(b)))
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=1e-1)

    def test_arm_mat_mult_q31(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([[1., 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]))/2.0
        err,c = dsp.arm_mat_mult_q31(toQ31(a), toQ31(b))
        self.assertTrue(err == 0)
        c = Q31toF32(c)
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=1e-5)

    def test_arm_mat_mult_opt_q31(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([[1., 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]))/2.0
        s=b.shape 
        nb=s[0]*s[1]
        tmp=np.zeros(nb,dtype=np.int32)
        err,c = dsp.arm_mat_mult_opt_q31(toQ31(a), toQ31(b),tmp)
        self.assertTrue(err == 0)
        c = Q31toF32(c)
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=1e-5)

    def test_arm_mat_mult_fast_q31(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([[1., 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]))/2.0
        s=b.shape 
        nb=s[0]*s[1]
        tmp=np.zeros(nb,dtype=np.int32)
        err,c = dsp.arm_mat_mult_fast_q31(toQ31(a), toQ31(b))
        self.assertTrue(err == 0)
        c = Q31toF32(c)
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=1e-5)

    def test_arm_mat_mult_q15(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([[1., 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]))/2.0
        s=b.shape 
        nb=s[0]*s[1]
        tmp=np.zeros(nb,dtype=np.int16)
        err,c = dsp.arm_mat_mult_q15(toQ15(a), toQ15(b),tmp)
        self.assertTrue(err == 0)
        c = Q15toF32(c)
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=4e-4)

    def test_arm_mat_mult_fast_q15(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([[1., 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]))/2.0
        s=b.shape 
        nb=s[0]*s[1]
        tmp=np.zeros(nb,dtype=np.int16)
        err,c = dsp.arm_mat_mult_fast_q15(toQ15(a), toQ15(b),tmp)
        self.assertTrue(err == 0)
        c = Q15toF32(c)
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=4e-4)

    def test_arm_mat_mult_q7(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]))/2.0
        b = normalize(np.array([[1., 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]))/2.0
        s=b.shape 
        nb=s[0]*s[1]
        tmp=np.zeros(nb,dtype=np.int16)
        err,c = dsp.arm_mat_mult_q7(toQ7(a), toQ7(b),tmp)
        self.assertTrue(err == 0)
        c = Q7toF32(c)
        ref = np.dot(a, b)
        assert_allclose(c, ref, rtol=5e-2)

    def test_mat_trans_f32(self):
        a = normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]],dtype=np.float32))/2.0
        err,res=dsp.arm_mat_trans_f32(a)
        self.assertTrue(err == 0)
        ref=np.transpose(a)
        assert_equal(res, ref)

    def test_mat_trans_q31(self):
        a = toQ31(normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]],dtype=np.float32))/2.0)
        err,res=dsp.arm_mat_trans_q31(a)
        self.assertTrue(err == 0)
        ref=np.transpose(a)
        assert_equal(res, ref)

    def test_mat_trans_q15(self):
        a = toQ15(normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]],dtype=np.float32))/2.0)
        err,res=dsp.arm_mat_trans_q15(a)
        self.assertTrue(err == 0)
        ref=np.transpose(a)
        assert_equal(res, ref)

    def test_mat_trans_q7(self):
        a = toQ7(normalize(np.array([[1., 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]],dtype=np.float32))/2.0)
        err,res=dsp.arm_mat_trans_q7(a)
        self.assertTrue(err == 0)
        ref=np.transpose(a)
        assert_equal(res, ref)

    def test_mat_cmplx_trans_f32(self):
        a=np.array([[1. + 0.0j ,2 + 1.0j,3 + 0.0j,4 + 2.0j],
              [5 + 1.0j,6 + 2.0j,7 + 3.0j,8 + 1.0j],
              [9 - 2.0j,10 + 1.0j,11 - 4.0j,12 + 1.0j]],dtype=np.csingle)
        a = normalize(a)
        err,res=dsp.arm_mat_cmplx_trans_f32(imToReal2D(a))
        self.assertTrue(err == 0)
        ref=imToReal2D(np.transpose(a))
        assert_equal(res, ref)

    def test_mat_cmplx_trans_q31(self):
        a=np.array([[1. + 0.0j ,2 + 1.0j,3 + 0.0j,4 + 2.0j],
              [5 + 1.0j,6 + 2.0j,7 + 3.0j,8 + 1.0j],
              [9 - 2.0j,10 + 1.0j,11 - 4.0j,12 + 1.0j]],dtype=np.csingle)
        a = toQ31(imToReal2D(normalize(a)))
        err,res=dsp.arm_mat_cmplx_trans_q31(a)
        self.assertTrue(err == 0)
        ref=imToReal2D(np.transpose(realToIm2D(a)))
        assert_equal(res, ref)

    def test_mat_cmplx_trans_q15(self):
        a=np.array([[1. + 0.0j ,2 + 1.0j,3 + 0.0j,4 + 2.0j],
              [5 + 1.0j,6 + 2.0j,7 + 3.0j,8 + 1.0j],
              [9 - 2.0j,10 + 1.0j,11 - 4.0j,12 + 1.0j]],dtype=np.csingle)
        a = toQ15(imToReal2D(normalize(a)))
        err,res=dsp.arm_mat_cmplx_trans_q15(a)
        self.assertTrue(err == 0)
        ref=imToReal2D(np.transpose(realToIm2D(a)))
        assert_equal(res, ref)

    def test_levinson_durbin(self):
        na=5
        s = np.random.randn(na+1)
        s = normalize(s)
        phi = autocorr(s)
        phi = normalize(phi)
        
        _,arcoef,_,_,_=statsmodels.tsa.stattools.levinson_durbin(phi,nlags=na,isacov=True)
       
        
        (a,_)=dsp.arm_levinson_durbin_f32(phi,na)
        assert_allclose(arcoef,a,rtol=2e-5)

    def test_levinson_durbin_q31(self):
        na=5
        s = np.random.randn(na+1)
        s = normalize(s)
        phi = autocorr(s)
        phi = normalize(phi)

        _,arcoef,_,_,_=statsmodels.tsa.stattools.levinson_durbin(phi,nlags=na,isacov=True)

        
        phiQ31 = toQ31(phi)
        (aQ31,_)=dsp.arm_levinson_durbin_q31(phiQ31,na)
        assert_allclose(Q31toF32(aQ31),arcoef,rtol=1e-5,atol=1e-5)


class TestBitwiseMethods_Test2(unittest.TestCase):

    def setUp(self):
        self.NBSAMPLES=10
        return super().setUp()
    
    def test_and_u32(self):
        su32A=genBitvectors(self.NBSAMPLES,31)
        su32B=genBitvectors(self.NBSAMPLES,31)
        
        ref=np.bitwise_and(su32A, su32B)
        result=dsp.arm_and_u32(su32A, su32B)
        assert_equal(result,ref)

    def test_or_u32(self):
        su32A=genBitvectors(self.NBSAMPLES,31)
        su32B=genBitvectors(self.NBSAMPLES,31)        
        
        ref=np.bitwise_or(su32A, su32B)
        result=dsp.arm_or_u32(su32A, su32B)
        assert_equal(result,ref)

    
    def test_xor_u32(self):
        su32A=genBitvectors(self.NBSAMPLES,31)
        su32B=genBitvectors(self.NBSAMPLES,31)        
        
        ref=np.bitwise_xor(su32A, su32B)
        result=dsp.arm_xor_u32(su32A, su32B)
        assert_equal(result,ref)

    def test_not_u32(self):
        su32A=genBitvectors(self.NBSAMPLES,31)
        
        ref=np.invert(su32A)
        result=dsp.arm_not_u32(su32A)
        assert_equal(result,ref)

    def test_and_u16(self):
        su16A=genBitvectors(self.NBSAMPLES,15)
        su16B=genBitvectors(self.NBSAMPLES,15)        
        
        ref=np.bitwise_and(su16A, su16B)
        result=dsp.arm_and_u16(su16A, su16B)
        assert_equal(result,ref)

    def test_or_u16(self):
        su16A=genBitvectors(self.NBSAMPLES,15)
        su16B=genBitvectors(self.NBSAMPLES,15)        
        
        ref=np.bitwise_or(su16A, su16B)
        result=dsp.arm_or_u16(su16A, su16B)
        assert_equal(result,ref)

    def test_xor_u16(self):
        su16A=genBitvectors(self.NBSAMPLES,15)
        su16B=genBitvectors(self.NBSAMPLES,15)        
        
        ref=np.bitwise_xor(su16A, su16B)
        result=dsp.arm_xor_u16(su16A, su16B)
        assert_equal(result,ref)

    def test_not_u16(self):
        su16A=genBitvectors(self.NBSAMPLES,15)
        
        ref=np.invert(su16A)
        result=dsp.arm_not_u16(su16A)
        assert_equal(result,ref)

    def test_and_u8(self):
        su8A = genBitvectors(self.NBSAMPLES, 7)
        su8B = genBitvectors(self.NBSAMPLES, 7)

        ref = np.bitwise_and(su8A, su8B)
        result = dsp.arm_and_u8(su8A, su8B)
        assert_equal(result, ref)

    def test_or_u8(self):
        su8A = genBitvectors(self.NBSAMPLES, 7)
        su8B = genBitvectors(self.NBSAMPLES, 7)

        ref = np.bitwise_or(su8A, su8B)
        result = dsp.arm_or_u8(su8A, su8B)
        assert_equal(result, ref)

    def test_xor_u8(self):
        su8A = genBitvectors(self.NBSAMPLES, 7)
        su8B = genBitvectors(self.NBSAMPLES, 7)

        ref = np.bitwise_xor(su8A, su8B)
        result = dsp.arm_xor_u8(su8A, su8B)
        assert_equal(result, ref)

    def test_not_u8(self):
        su8A = genBitvectors(self.NBSAMPLES, 7)

        ref = np.invert(su8A)
        result = dsp.arm_not_u8(su8A)
        assert_equal(result, ref)

def flattenQuat(l):
    return(np.array([list(x) for x in l]).reshape(4*len(l)))
  
def flattenRot(l):
    return(np.array([list(x) for x in l]).reshape(9*len(l)))
  
# q and -q are representing the same rotation.
# So there is an ambiguity for the tests.
# We force the real part of be positive.
def standardizeQuat(q):
    if q.scalar < 0:
          return(-q)
    else:
          return(q)
    
def mkQuaternion(mat):
      q=Quaternion(matrix=mat)
      return standardizeQuat(q)
      

      
class TestQuaternionMethods_Test2(unittest.TestCase):

    def setUp(self):
        self.NBSAMPLES=3
        self.a=[2.0*Quaternion.random() for x in range(self.NBSAMPLES)]
        return super().setUp()
    
    def test_quaternion_normalize(self):
  
        ref=flattenQuat([x.normalised for x in self.a])

        src=flattenQuat(self.a)
        res=dsp.arm_quaternion_normalize_f32(src)

        assert_allclose(res, ref, rtol=1e-6)

    def test_quaternion_conjugate(self):
        ref=flattenQuat([x.conjugate for x in self.a])

        src=flattenQuat(self.a)
        res=dsp.arm_quaternion_conjugate_f32(src)

        assert_allclose(res, ref, rtol=1e-6)

    def test_quaternion_inverse(self):
        ref=flattenQuat([x.inverse for x in self.a])

        src=flattenQuat(self.a)
        res=dsp.arm_quaternion_inverse_f32(src)

        assert_allclose(res, ref, rtol=1e-6)

    def test_quaternion_norm(self):
        ref=[x.norm for x in self.a]

        src=flattenQuat(self.a)
        res=dsp.arm_quaternion_norm_f32(src)

        assert_allclose(res, ref, rtol=1e-6)

    def test_quaternion_rotation(self):
         na=[x.normalised for x in self.a]
         refa=[x.rotation_matrix for x in na]
         refb=[mkQuaternion(x) for x in refa]

         srca=flattenQuat(na)
         resa=dsp.arm_quaternion2rotation_f32(srca)
         resb=dsp.arm_rotation2quaternion_f32(resa).astype(np.float32)

         resb=np.array([standardizeQuat(Quaternion(x)) for x in resb.reshape(self.NBSAMPLES,4)])
         resb=flattenQuat(resb)

         assert_allclose(resa, np.array(refa).reshape(3*3*self.NBSAMPLES), rtol=1e-4)
         assert_allclose(resb, flattenQuat(refb), rtol=1e-4)

    def test_quaternion_multiply(self):
        a=[2.0*Quaternion.random() for x in range(self.NBSAMPLES)]
        b=[2.0*Quaternion.random() for x in range(self.NBSAMPLES)]
  
        refc = flattenQuat(np.array(a) * np.array(b))
  
        srca=flattenQuat(a)
        srcb=flattenQuat(b)
        resc=dsp.arm_quaternion_product_f32(srca,srcb)
        assert_allclose(resc,refc,rtol=2e-5)

    def test_quaternion_multiply_single(self):
        a=2.0*Quaternion.random() 
        b=2.0*Quaternion.random()
  
        refc = flattenQuat([a * b])
  
        srca=flattenQuat([a])
        srcb=flattenQuat([b])
        resc=dsp.arm_quaternion_product_single_f32(srca,srcb)
        assert_allclose(resc,refc,rtol=1e-5)


if __name__ == '__main__':
    unittest.main()