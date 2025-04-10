import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

from scipy.stats import entropy,tstd, tvar
from scipy.special import logsumexp
from scipy.linalg import cholesky,ldl,solve_triangular
from scipy import signal


def printTitle(s):
    pass

def printSubTitle(s):
    pass

class TestMaxMinMethods_Test5(unittest.TestCase):

    def setUp(self):
        self.a=np.array([1.,-3.,4.,0.,-10.,8.])
        self.b=np.array([1.,-3.,4.,0.5,-10.,8.])
        return super().setUp()
    
    def test_absmax_noidx_f32(self):
        res=dsp.arm_absmax_no_idx_f32(self.a)
        self.assertAlmostEqual(res,10.0)

    def test_absmax_noidx_f64(self):
        res=dsp.arm_absmax_no_idx_f64(self.a)
        self.assertAlmostEqual(res,10.0)

    def test_absmax_f64(self):
        res,idx=dsp.arm_absmax_f64(self.a)
        self.assertAlmostEqual(res,10.0)
        self.assertEqual(idx,4)

    def test_max_f64(self):
        res,idx=dsp.arm_max_f64(self.a)
        self.assertAlmostEqual(res,8.0)
        self.assertEqual(idx,5)

    def test_max_noidx_f32(self):
        res=dsp.arm_max_no_idx_f32(self.a)
        self.assertAlmostEqual(res,8.0)

    def test_max_noidx_f64(self):
        res=dsp.arm_max_no_idx_f64(self.a)
        self.assertAlmostEqual(res,8.0)

    def test_absmin_noidx_f32(self):
        res = dsp.arm_absmin_no_idx_f32(self.b)
        self.assertAlmostEqual(res, 0.5)

    def test_absmin_noidx_f64(self):
        res = dsp.arm_absmin_no_idx_f64(self.b)
        self.assertAlmostEqual(res, 0.5)

    def test_absmin_f64(self):
        res, idx = dsp.arm_absmin_f64(self.b)
        self.assertAlmostEqual(res, 0.5)
        self.assertEqual(idx, 3)

    def test_min_f64(self):
        res, idx = dsp.arm_min_f64(self.b)
        self.assertAlmostEqual(res, -10.0)
        self.assertEqual(idx, 4)

    def test_min_noidx_f32(self):
        res = dsp.arm_min_no_idx_f32(self.b)
        self.assertAlmostEqual(res, -10.0)

    def test_min_noidx_f64(self):
        res = dsp.arm_min_no_idx_f64(self.b)
        self.assertAlmostEqual(res, -10.0)

class TestStatsMethods_Test5(unittest.TestCase):

    def setUp(self):
        self.a=np.zeros((12,3))
        self.w=np.array([[2.0] * 12])[0]
        self.w[0]=3
        self.w[11]=3
        self.a[0] =[0., 0., -0.951057]
        self.a[1] =[0., 0., 0.951057]
        self.a[2] =[-0.850651, 0., -0.425325]
        self.a[3] =[0.850651, 0., 0.425325]
        self.a[4] =[0.688191, -0.5, -0.425325]
        self.a[5] =[0.688191, 0.5, -0.425325]
        self.a[6] =[-0.688191, -0.5, 0.425325]
        self.a[7] =[-0.688191, 0.5, 0.425325]
        self.a[8] =[-0.262866, -0.809017, -0.425325]
        self.a[9] =[-0.262866, 0.809017, -0.425325]
        self.a[10]=[0.262866, -0.809017, 0.425325]
        self.a[11]=[0.262866, 0.809017, 0.425325]

        self.nb=10
        self.s = np.random.randn(self.nb)
        self.wb = np.random.randn(self.nb)

        self.sb = np.abs(np.random.randn(self.nb))
        self.sb = self.sb / np.sum(self.sb)

        self.pa = np.abs(np.random.randn(self.nb))
        self.pa = self.pa / np.sum(self.pa)

        self.pb = np.abs(np.random.randn(self.nb))
        self.pb = self.pb / np.sum(self.pb)

        return super().setUp()
    
    def test_barycenter_f32(self):
        scaled= np.dot(self.a.T , self.w)
        ref=scaled/np.sum(self.w)

        points = np.array(self.a).reshape(12*3)
        weights = self.w.reshape(12)
   
        res=dsp.arm_barycenter_f32(points,weights,12,3)

        assert_allclose(ref,res,rtol=1e-2)

    def test_weighted_average_f32(self):
        ref=np.dot(self.s,self.wb)/np.sum(self.wb)
   
        res=dsp.arm_weighted_average_f32(self.s,self.wb)
   
        assert_allclose(ref,res,2e-5)

    def test_entropy_f32(self):
        ref=entropy(self.sb)
        res=dsp.arm_entropy_f32(self.sb)
        assert_allclose(ref,res,1e-6)

    def test_entropy_f64(self):
        ref=entropy(self.sb)
        res=dsp.arm_entropy_f64(self.sb)
        assert_allclose(ref,res,1e-10)

    def test_kullback_leibler_f32(self):
        ref=entropy(self.pa,self.pb)
        res=dsp.arm_kullback_leibler_f32(self.pa,self.pb)
        assert_allclose(ref,res,2e-6)

    def test_kullback_leibler_f64(self):
        ref=entropy(self.pa,self.pb)
        res=dsp.arm_kullback_leibler_f64(self.pa,self.pb)
        assert_allclose(ref,res,1e-10)

    def test_logsumexp_f32(self):
        ref=logsumexp(self.sb)
        res=dsp.arm_logsumexp_f32(self.sb)
        assert_allclose(ref,res,1e-6)

    def test_logsumexp_dotprod_f32(self):
        d = 0.001
        # It is a proba so must be in [0,1]
        # But restricted to ]d,1] so that the log exists
        sa = (1-d)*self.pa + d
        sb = (1-d)*self.pb + d
   
        ref=np.log(np.dot(sa,sb))
   
        sa = np.log(sa)
        sb = np.log(sb)
   
        res=dsp.arm_logsumexp_dot_prod_f32(sa,sb)
        assert_allclose(ref,res,3e-6)

    def test_vexp_f32(self):
        ref = np.exp(self.s)
   
        res=dsp.arm_vexp_f32(self.s)
        assert_allclose(ref,res,1e-6)

    def test_vexp_f64(self):
        ref = np.exp(self.s)
   
        res=dsp.arm_vexp_f64(self.s)
        assert_allclose(ref,res,1e-10)

    def test_vlog_f32(self):
        sa = np.abs(self.s) + 0.001
        ref = np.log(sa)
   
        res=dsp.arm_vlog_f32(sa)
        assert_allclose(ref,res,2e-5,1e-5)

    def test_vlog_f64(self):
        sa = np.abs(self.s) + 0.001
        ref = np.log(sa)
   
        res=dsp.arm_vlog_f64(sa)
        assert_allclose(ref,res,2e-9,1e-9)

def _swaprow(m,k,j):
       tmp = np.copy(m[j,:])
       m[j,:] = np.copy(m[k,:])
       m[k,:] = tmp 
       return(m)

class TestMatrixMethods_Test5(unittest.TestCase):

    def setUp(self):
        self.a=np.array([[4,12,-16],[12,37,-43],[-16,-43,98]])
        self.la = np.array([[3, 0, 0, 0], [2, 1, 0, 0], [1, 0, 1, 0], [1, 1, 1, 1]])
        self.lb = np.array([[4,2,4,2],[8,4,8,4]]).T
        return super().setUp()
    
    def test_cholesky_f32(self):
        ref=cholesky(self.a,lower=True)
   
        status,res=dsp.arm_mat_cholesky_f32(self.a)
        self.assertTrue(status==0)
        assert_allclose(ref,res,1e-6,1e-6)

    def test_cholesky_f64(self):
        ref=cholesky(self.a,lower=True)
   
        status,res=dsp.arm_mat_cholesky_f64(self.a)
        self.assertTrue(status==0)
        assert_allclose(ref,res,1e-6,1e-6)

    def test_ldlt_f32(self):
        status,resl,resd,resperm=dsp.arm_mat_ldlt_f32(self.a)
        self.assertTrue(status==0)
        n=3
        p=np.identity(n)
        for k in range(0,n):
            p = _swaprow(p,k,resperm[k])
        
        res=resl.dot(resd).dot(resl.T)
        
        permutedSrc=p.dot(self.a).dot(p.T)
        
        assert_allclose(permutedSrc,res,1e-5,1e-5)

    def test_ldlt_f64(self):
        status,resl,resd,resperm=dsp.arm_mat_ldlt_f64(self.a)
        self.assertTrue(status==0)
        n=3
        p=np.identity(n)
        for k in range(0,n):
            p = _swaprow(p,k,resperm[k])
        
        res=resl.dot(resd).dot(resl.T)
        
        permutedSrc=p.dot(self.a).dot(p.T)
        
        assert_allclose(permutedSrc,res,1e-9,1e-9)

    def test_lowertri_f32(self):
        x = solve_triangular(self.la, self.lb,lower=True)
        status,res=dsp.arm_mat_solve_lower_triangular_f32(self.la,self.lb)
        self.assertTrue(status==0)
        assert_allclose(x,res,1e-5,1e-5)

    def test_lowertri_f64(self):
        x = solve_triangular(self.la, self.lb,lower=True)
        status,res=dsp.arm_mat_solve_lower_triangular_f64(self.la,self.lb)
        self.assertTrue(status==0)
        assert_allclose(x,res,1e-9,1e-9)

    def test_uppertri_f32(self):
        x = solve_triangular(self.la.T, self.lb,lower=True)
        status,res=dsp.arm_mat_solve_upper_triangular_f32(self.la,self.lb)
        self.assertTrue(status==0)
        assert_allclose(x,res,1e-5,1e-5)

    def test_uppertri_f64(self):
        x = solve_triangular(self.la.T, self.lb,lower=True)
        status,res=dsp.arm_mat_solve_upper_triangular_f64(self.la,self.lb)
        self.assertTrue(status==0)
        assert_allclose(x,res,1e-9,1e-9)

    def test_matmul_f64(self):
        ref = self.la.dot(self.lb)
   
        status,res = dsp.arm_mat_mult_f64(self.la,self.lb)
        self.assertTrue(status==0)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_matsub_f64(self):
        ref = self.la-self.la.T
   
        status,res = dsp.arm_mat_sub_f64(self.la,self.la.T)
        self.assertTrue(status==0)
        assert_allclose(ref,res,1e-10,1e-10)

    
class TestF64Methods_Test5(unittest.TestCase):

    def setUp(self):
        self.nb = 10
        self.sa = np.random.randn(self.nb)
        self.sb = np.random.randn(self.nb)
        return super().setUp()
    
    def test_abs_f64(self):
        ref = np.abs(self.sa)
        res = dsp.arm_abs_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_add_f64(self):
        ref = self.sa + self.sb
        res = dsp.arm_add_f64(self.sa,self.sb)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_sub_f64(self):
        ref = self.sa - self.sb
        res = dsp.arm_sub_f64(self.sa,self.sb)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_dotprod_f64(self):
        ref = self.sa.dot(self.sb)
        res = dsp.arm_dot_prod_f64(self.sa,self.sb)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_mult_f64(self):
        ref = self.sa * self.sb
        res = dsp.arm_mult_f64(self.sa,self.sb)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_negate_f64(self):
        ref = -self.sa
        res = dsp.arm_negate_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_offset_f64(self):
        ref = self.sa + 0.1
        res=dsp.arm_offset_f64(self.sa,0.1)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_scale_f64(self):
        ref = self.sa * 0.1
        res=dsp.arm_scale_f64(self.sa,0.1)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_mean_f64(self):
        ref = np.mean(self.sa)
        res=dsp.arm_mean_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_power_f64(self):
        ref = np.sum(self.sa*self.sa)
        res=dsp.arm_power_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_std_f64(self):
        ref = tstd(self.sa)
        res=dsp.arm_std_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_var_f64(self):
        ref = tvar(self.sa)
        res=dsp.arm_var_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_fill_f64(self):
        ref = np.ones(self.nb)*4.0 
        res = dsp.arm_fill_f64(4.0,self.nb)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_copy_f64(self):
        ref=self.sa
        res = dsp.arm_copy_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_div_int64_f64(self):
        den=0x7FFF00000000 
        num=0x10000
        ref=den//num
   
        res=dsp.arm_div_int64_to_int32(den,num)
        self.assertEqual(ref,res)

    def test_fir_f64(self):
        firf64 = dsp.arm_fir_instance_f64()
        dsp.arm_fir_init_f64(firf64,3,[1.,2,3],[0,0,0,0,0,0,0])
        filtered_x = signal.lfilter([3,2,1.], 1.0, [1,2,3,4,5,1,2,3,4,5])
        ra=dsp.arm_fir_f64(firf64,[1,2,3,4,5])
        rb=dsp.arm_fir_f64(firf64,[1,2,3,4,5])
        assert_allclose(filtered_x , np.hstack([ra,rb]))

class TestCmplxMethods_Test5(unittest.TestCase):

    def setUp(self):
        self.nb = 10
        self.sa = np.random.randn(self.nb)
        self.sb = np.random.randn(self.nb)
        return super().setUp()
    
    def test_cmplx_mag_f32(self):
        ca = realToIm(self.sa)
        ref = np.abs(ca)
        res=dsp.arm_cmplx_mag_f32(self.sa)
        assert_allclose(ref,res,1e-6,1e-6)

    def test_cmplx_mag_f64(self):
        ca = realToIm(self.sa)
        ref = np.abs(ca)
        res=dsp.arm_cmplx_mag_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_cmplx_mag_squared_f32(self):
        ca = realToIm(self.sa)
        ref = np.abs(ca)*np.abs(ca)
        res=dsp.arm_cmplx_mag_squared_f32(self.sa)
        assert_allclose(ref,res,1e-6,1e-6)

    def test_cmplx_mag_squared_f64(self):
        ca = realToIm(self.sa)
        ref = np.abs(ca)*np.abs(ca)
        res=dsp.arm_cmplx_mag_squared_f64(self.sa)
        assert_allclose(ref,res,1e-10,1e-10)

    def test_cmplx_mul_f32(self):
        ca = realToIm(self.sa)
        cb = realToIm(self.sb)
        ref = imToReal(ca * cb)
        res = dsp.arm_cmplx_mult_cmplx_f32(self.sa,self.sb)
        assert_allclose(ref,res,1e-6,1e-6)

    def test_cmplx_mul_f64(self):
        ca = realToIm(self.sa)
        cb = realToIm(self.sb)
        ref = imToReal(ca * cb)
        res = dsp.arm_cmplx_mult_cmplx_f64(self.sa,self.sb)
        assert_allclose(ref,res,1e-10,1e-10)

    
if __name__ == '__main__':
    unittest.main()