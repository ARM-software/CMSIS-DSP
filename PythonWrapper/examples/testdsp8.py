# New functions for version 1.5 of the Python wrapper
import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

import numpy as np
import math
from numpy.testing import assert_allclose

from numpy.linalg import qr 

def householder(x,eps=1e-16):
    #print(x)
    v=np.hstack([[1],x[1:]])
    
    alpha = x[0]
    xnorm2=x[1:].dot(x[1:])
    epsilon=eps

    #print(sigma)

    if xnorm2<=epsilon:
       tau = 0.0
       v = np.zeros(len(x))
    else:
       if np.sign(alpha) <= 0:
          beta = math.sqrt(alpha*alpha + xnorm2)
       else:
          beta = -math.sqrt(alpha*alpha + xnorm2)

       r =  (alpha - beta)
       v = x / r
       tau = (beta - alpha) / beta
       v[0] = 1

    return(v,tau)

def checkOrtho(A,err=1e-10):
    identity = np.eye(A.shape[0], dtype=np.float32)
    should_be_identity = A.T @ A
    return np.allclose(should_be_identity, identity, atol=err)


def randomIsometry(rows,cols,rank):
    if rank==1:
       r=np.random.randn(rows)
       r = normalize(r)[np.newaxis]
       c=np.random.randn(cols)
       c = normalize(c)[np.newaxis]
       result=r.T.dot(c)
    else:
        a = np.random.randn(rows,rows)
        b = np.random.randn(cols,cols)

        diagDim = min(rows,cols)
        d = np.zeros((rows,cols))
        
        diag = np.ones(diagDim)
        diag[rank:] = 0 
        np.fill_diagonal(d,diag)

        qa,_ = qr(a)
        qb,_ = qr(b)

        result = qa .dot(d.dot(qb))
    return(result)


class TestLinAlg_Test8(unittest.TestCase):

    def setUp(self):
        self.VECDIM = 10 

        self.a=np.random.randn(self.VECDIM)
        self.a = self.a / np.max(np.abs(self.a))

        self.rows = 8 
        self.columns = 5 
   
        self.m = randomIsometry(self.rows,self.columns,self.columns-1)

        return super().setUp()
    
    def test_householder_f32(self):
        # Reference
        vRef,betaRef = householder(self.a)
        betaF32,vF32 = dsp.arm_householder_f32(self.a,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32)
        assert_allclose(betaRef,betaF32,1e-6,1e-6)
        assert_allclose(vRef,vF32,1e-6,1e-6)

    def test_householder_f64(self):
        # Reference
        vRef,betaRef = householder(self.a)
        betaF64,vF64 = dsp.arm_householder_f64(self.a,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F64)
        assert_allclose(betaRef,betaF64,1e-10,1e-10)
        assert_allclose(vRef,vF64,1e-10,1e-10)
    

    def test_householder_proportional_f32(self):
        # Reference
        vRef,betaRef = householder(self.a*1e-7)

        # With the small vector, and the given threshold
        # the vector is understood as being proportional to (1,0,0,...)
        # So the result is 0 for beta and v
        betaF32,vF32 = dsp.arm_householder_f32(self.a*1e-7,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32)
        assert_equal(betaF32,0.0)
        assert_equal(vF32,np.zeros(self.VECDIM))

        # With a smaller threshold, a computation is taking place
        betaF32,vF32 = dsp.arm_householder_f32(self.a*1e-7,0.001*dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32)
        assert_allclose(betaRef,betaF32,1e-6,1e-6)
        assert_allclose(vRef,vF32,1e-6,1e-6)

    def _test_qr_f64(self):
        rows,columns = self.m.shape
   
        # The CMSIS-DSP C functions is requiring two temporary arrays
        # To follow the C function as closely as possible, we create
        # two arrays. Their size will be used internally by the Python
        # wrapper to allocate two temporary buffers.
        # Like that you can check you have dimensionned the arrays in the
        # right way.
        # The content of the temporary buffers is not accessible from the
        # Python API. tmpa and tmpb are not modified.
        tmpa=np.zeros(rows)
        tmpb=np.zeros(rows)

        status,r,q,tau = dsp.arm_mat_qr_f64(self.m,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F64,tmpa,tmpb)
        self.assertEqual(status,0,"QR decomposition failed")

        #self.assertTrue(checkOrtho(q,err=1e-6),"Matrix Q is not orthogonal")

        # Remove householder vectors from R matrix
        i=1
        for c in r.T:
            c[i:] = 0
            i = i+1

        # Check that M = Q R
        newm = np.dot(q,r)
        assert_allclose(newm,self.m)

    def _test_qr_f32(self):
        rows,columns = self.m.shape
   
        # The CMSIS-DSP C functions is requiring two temporary arrays
        # To follow the C function as closely as possible, we create
        # two arrays. Their size will be used internally by the Python
        # wrapper to allocate two temporary buffers.
        # Like that you can check you have dimensionned the arrays in the
        # right way.
        # The content of the temporary buffers is not accessible from the
        # Python API. tmpa and tmpb are not modified.
        tmpa=np.zeros(rows)
        tmpb=np.zeros(rows)

        status,r,q,tau = dsp.arm_mat_qr_f32(self.m.astype(np.float32),dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32,tmpa,tmpb)
        self.assertEqual(status,0,"QR decomposition failed")
        #self.assertTrue(checkOrtho(q,err=1.0e-1),"Matrix Q is not orthogonal")

        # Remove householder vectors from R matrix
        i=1
        for c in r.T:
            c[i:] = 0
            i = i+1

        # Check that M = Q R
        newm = np.dot(q,r)
        assert_allclose(newm,self.m,2e-5,1e-5)



if __name__ == '__main__':
    unittest.main()