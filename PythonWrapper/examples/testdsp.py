import cmsisdsp as dsp
import cmsisdsp.datatype as dt
import numpy as np
from scipy import signal
#import matplotlib.pyplot as plt
from scipy.fftpack import dct 
from scipy.signal import lfilter
import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

class TestBasicMathMethods_Test1(unittest.TestCase):

     def setUp(self):
        self.NBSAMPLES=256

        self.data1=np.random.randn(self.NBSAMPLES)
        return super().setUp()
     
     def test_add_f32(self):
         a=np.array([1.,2,3],dtype=np.float32)
         b=np.array([4.,5,7],dtype=np.float32)
         res = dsp.arm_add_f32(a,b)
         ref = a + b
         assert_allclose(ref,res,1e-6)

     def test_accumulate_f32(self):
         ref = np.sum(self.data1)
         t = dsp.arm_accumulate_f32(self.data1)
         assert_allclose(ref,t,rtol=2e-6,atol=1e-6)

     def test_accumulate_f64(self):
         ref = np.sum(self.data1)
         t = dsp.arm_accumulate_f64(self.data1)
         assert_allclose(ref,t)

     def test_add_q31(self):
         a = np.array([1,2,3],dtype=np.int32)
         b = np.array([4,5,7],dtype=np.int32)
         res = dsp.arm_add_q31(a,b)
         ref = a + b
         assert_equal(ref,res)
         
     def test_add_q15(self):
         a = np.array([1,2,3],dtype=np.int16)
         b = np.array([4,5,7],dtype=np.int16)
         res = dsp.arm_add_q15(a,b)
         ref = a + b
         assert_equal(ref,res)

     def test_add_q7(self):
         a = np.array([1,2,3],dtype=np.int8)
         b = np.array([4,5,7],dtype=np.int8)
         res = dsp.arm_add_q7(a,b)
         ref = a + b
         assert_equal(ref,res)

     def test_scale_f32(self):
         a = np.array([1.,2,3],dtype=np.float32)
         res = dsp.arm_scale_f32(a,2)
         ref = a * 2
         assert_allclose(ref,res,1e-6)

     def test_scale_q31(self):
         a = np.array([0x7FFF,0x3FFF,0x1FFF],dtype=np.int32)
         res = dsp.arm_scale_q31(a,1 << 20,2)
         ref = (a.astype(np.int64) * (1<<20) >> 32) << 3
         assert_equal(ref,res)

     def test_scale_q15(self):
         a = np.array([0x7F,0x3F,0x1F],dtype=np.int16)
         res = dsp.arm_scale_q15(a,1 << 10,2)
         ref = a.astype(np.int32) * (1<<10) >> (15-2)
         assert_equal(ref,res)

     def test_scale_q7(self):
         a = np.array([0x7F,0x3F,0x1F],dtype=np.int8)
         res = dsp.arm_scale_q7(a,1 << 5,2)
         ref = a.astype(np.int16) * (1<<5) >> (7-2)
         assert_equal(ref,res)

     def test_negate_f32(self):
         a = np.array([1.,2,3],dtype=np.float32)
         res = dsp.arm_negate_f32(a)
         ref = -a
         assert_allclose(ref,res,1e-6)

     def test_negate_q31(self):
          a = np.array([1,2,3],dtype=np.int32)
          res = dsp.arm_negate_q31(a)
          ref = -a
          assert_equal(ref,res)

     def test_negate_q15(self):
          a = np.array([0x7FFF,0x3FFF,0x1FFF],dtype=np.int16)
          res = dsp.arm_negate_q15(a)
          ref = -a
          assert_equal(ref,res)

     def test_negate_q7(self):
          a = np.array([0x7F,0x3F,0x1F],dtype=np.int8)
          res = dsp.arm_negate_q7(a)
          ref = -a
          assert_equal(ref,res)

class TestComplexMathMethods_Test1(unittest.TestCase):

     def setUp(self):
        return super().setUp()
     
     def test_cmplx_conj_f32(self):
          a = np.array([1.,2,3,4],dtype=np.float32)
          res = dsp.arm_cmplx_conj_f32(a)
          ref = imToReal(np.conj(realToIm(a)))
          assert_allclose(ref,res,1e-6)

     def test_cmplx_conj_q31(self):
          a = np.array([0x7FFF,0x3FFF,0x1FFF,0x3FFF],dtype=np.int32)
          res = dsp.arm_cmplx_conj_q31(a)
          ref = imToReal(np.conj(realToIm(a)))
          assert_equal(ref,res)

     def test_cmplx_conj_q15(self):
          a = np.array([0x7FFF,0x3FFF,0x1FFF,0x3FFF],dtype=np.int16)
          res = dsp.arm_cmplx_conj_q15(a)
          ref = imToReal(np.conj(realToIm(a)))
          assert_equal(ref,res)

     def test_cmplx_dot_prod_f32(self):
          a = np.array([1.,2,3,4],dtype=np.float32)
          b = np.array([1.,2,3,4],dtype=np.float32)
          res = np.array(dsp.arm_cmplx_dot_prod_f32(a,b))
          ref = imToReal(np.array([np.dot(realToIm(a),realToIm(b))]))
          assert_allclose(ref,res,1e-6)

     def test_cmplx_dot_prod_q31(self):
          a = np.array([0x7FFF,0x3FFF,0x1FFF,0x3FFF],dtype=np.int32)
          b = np.array([0x7FFF,0x3FFF,0x1FFF,0x3FFF],dtype=np.int32)
          res = np.array(dsp.arm_cmplx_dot_prod_q31(a,b))
          ref = imToReal(np.array([np.dot(realToIm(a),realToIm(b))])).astype(np.int64)>>14
          self.assertTrue((np.abs(ref-res)<=1).all())

     def test_cmplx_dot_prod_q15(self):
          a = np.array([0x7FFF,0x3FFF,0x1FFF,0x3FFF],dtype=np.int16)
          b = np.array([0x7FFF,0x3FFF,0x1FFF,0x3FFF],dtype=np.int16)
          res = np.array(dsp.arm_cmplx_dot_prod_q15(a,b))
          ref = imToReal(np.array([np.dot(realToIm(a),realToIm(b))])).astype(np.int32)>>6
          self.assertTrue((np.abs(ref-res)<=1).all())

     def test_cmplx_mult_real_f32(self):
          a = np.array([1.,2,3,4],dtype=np.float32)
          b = np.array([1.,2],dtype=np.float32)
          res = dsp.arm_cmplx_mult_real_f32(a,b)
          ref = imToReal(realToIm(a)*b)
          assert_allclose(ref,res,1e-6)


class TestPIDMethods_Test1(unittest.TestCase):

     def setUp(self):
        return super().setUp()
     
     def test_pid_instance_f32(self):
          pidf32 = dsp.arm_pid_instance_f32(Kp=1.0,Ki=1.2,Kd=0.4)
          assert_allclose(pidf32.Kp(),1.0,1e-6)
          assert_allclose(pidf32.Ki(),1.2,1e-6)
          assert_allclose(pidf32.Kd(),0.4,1e-6)

     def test_pid_init_f32(self):
          pidf32 = dsp.arm_pid_instance_f32(Kp=1.0,Ki=1.2,Kd=0.4)
          dsp.arm_pid_init_f32(pidf32,0)
          assert_allclose(pidf32.A0(),2.6,1e-6)

     
class TestFastMathMethods_Test1(unittest.TestCase):

     def setUp(self):
        return super().setUp()
     
     def test_cos_f32(self):
          ref = np.cos(3.14/4.)
          res = dsp.arm_cos_f32(3.14/4.)
          assert_allclose(ref,res,3e-6)
     
     def test_sin_f32(self):
          ref = np.sin(3.14/4.)
          res = dsp.arm_sin_f32(3.14/4.)
          assert_allclose(ref,res,3e-6)

     def test_sqrt_f32(self):
          ref = np.sqrt(2.0)
          _,res = dsp.arm_sqrt_f32(2.0)
          assert_allclose(ref,res,1e-6)

     def test_sqrt_q31(self):
          ref = toQ31(np.sqrt(Q31toF32(0x7FFF)))
          _,res = dsp.arm_sqrt_q31(0x7FFF)
          self.assertTrue(np.abs(ref-res)<=1)

class TestFirMethods_Test1(unittest.TestCase):

     def setUp(self):
        return super().setUp()
     
     def test_fir_f32(self):
          firf32 = dsp.arm_fir_instance_f32()
          dsp.arm_fir_init_f32(firf32,3,[1.,2,3],[0,0,0,0,0,0,0])
          filtered_x = lfilter([3,2,1.], 1.0, [1,2,3,4,5,1,2,3,4,5])
          ref = filtered_x
          res1 = dsp.arm_fir_f32(firf32,[1,2,3,4,5])
          res2 = dsp.arm_fir_f32(firf32,[1,2,3,4,5])
          res = np.hstack((res1,res2))
          assert_allclose(ref,res,1e-6)

     def test_fir_q31(self):
          firq31 = dsp.arm_fir_instance_q31()
          x=np.array([1,2,3,4,5])/10.0
          taps=np.array([1,2,3])/10.0
          xQ31=toQ31(x)
          tapsQ31=toQ31(taps)
          dsp.arm_fir_init_q31(firq31,3,tapsQ31,[0,0,0,0,0,0,0])
          self.assertTrue(firq31.numTaps()==3)
          filtered_x = toQ31(lfilter(np.array([3,2,1.])/10.0, 1.0, np.array([1,2,3,4,5,1,2,3,4,5])/10.0))
          ref = filtered_x
          res1 = dsp.arm_fir_q31(firq31,xQ31)
          res2 = dsp.arm_fir_q31(firq31,xQ31)
          res = np.hstack((res1,res2))
          self.assertTrue((np.abs(ref-res)<=1).all())



class TestMatrixMethods_Test1(unittest.TestCase):

     def setUp(self):
        return super().setUp()
     
     def test_mat_add_f32(self):
          a=np.array([[1.,2,3,4],[5,6,7,8],[9,10,11,12]])
          b=np.array([[1.,2,3,4],[5.1,6,7,8],[9.1,10,11,12]])
          ref = a + b
          err,res = dsp.arm_mat_add_f32(a,b)
          self.assertTrue(err==0)
          res = res.reshape(a.shape)
          assert_allclose(ref,res,1e-6)

     def test_mat_sub_f32(self):
          a=np.array([[1.,2,3,4],[5,6,7,8],[9,10,11,12]])
          b=np.array([[1.,2,3,4],[5.1,6,7,8],[9.1,10,11,12]])
          ref = a - b
          err,res = dsp.arm_mat_sub_f32(a,b)
          self.assertTrue(err==0)
          res = res.reshape(a.shape)
          assert_allclose(ref,res,4e-6)

     def test_mat_mult_f32(self):
          a=np.array([[1.,2,3,4],[5,6,7,8],[9,10,11,12]])
          b=np.array([[1.,2,3],[5.1,6,7],[9.1,10,11],[5,8,4]])
          ref = np.dot(a,b)
          err,res = dsp.arm_mat_mult_f32(a,b)
          self.assertTrue(err==0)
          res = res.reshape((a.shape[0],b.shape[1]))
          assert_allclose(ref,res,1e-6)
     
     def test_mat_cmplx_mult_f32(self):
          a=np.array([[1. + 2j,3 + 4j],[5 + 6j,7 + 8j],[9 + 10j,11 + 12j]])
          b=np.array([[1. + 2j, 3 + 5.1j ,6 + 7j],[9.1 + 10j,11 + 5j,8 +4j]])
          ref = np.dot(a,b)
          err,res = dsp.arm_mat_cmplx_mult_f32(imToReal2D(a),imToReal2D(b))
          self.assertTrue(err==0)
          res = realToIm2D(res)
          res = res.reshape((a.shape[0],b.shape[1]))
          assert_allclose(ref,res,1e-6)

     def test_mat_add_q31(self):
          a=np.array([[1.,2,3,4],[5,6,7,8],[9,10,11,12]]) / 30.0
          b=np.array([[1.,2,3,4],[5.1,6,7,8],[9.1,10,11,12]]) / 30.0
          ref = a + b
          aQ31=toQ31(a)
          bQ31=toQ31(b)
          err,res = dsp.arm_mat_add_q31(aQ31,bQ31)
          self.assertTrue(err==0)
          res = Q31toF32(res)
          res = res.reshape(a.shape)
          assert_allclose(ref,res,1e-6)

     def test_mat_trans_f32(self):
          a=np.array([[1.,2,3,4],[5,6,7,8],[9,10,11,12]])
          ref = np.transpose(a)
          err,res = dsp.arm_mat_trans_f32(a)
          self.assertTrue(err==0)
          res = res.reshape(a.shape[::-1])
          assert_allclose(ref,res,1e-6)

     def test_mat_inverse_f32(self):
          a = np.array([[1., 2.], [3., 4.]])
          ref = np.linalg.inv(a)
          err,res = dsp.arm_mat_inverse_f32(a)
          self.assertTrue(err==0)
          assert_allclose(ref,res,1e-6)

     def test_mat_inverse_f64(self):
          a = np.array([[1., 2.], [3., 4.]])
          ref = np.linalg.inv(a)
          err,res = dsp.arm_mat_inverse_f64(a)
          self.assertTrue(err==0)
          assert_allclose(ref,res,1e-6)

     def test_mat_scale_f32(self):
          a=np.array([[1.,2,3,4],[5,6,7,8],[9,10,11,12]])
          ref = 2.5*a
          err,res = dsp.arm_mat_scale_f32(a,2.5)
          self.assertTrue(err==0)
          res = res.reshape(a.shape)
          assert_allclose(ref,res,1e-6)

class TestStatsMethods_Test1(unittest.TestCase):

     def setUp(self):
        return super().setUp()
     
     def test_max_f32(self):
          a=np.array([1.,2,3,4,5,6,7,8,9,10,11,12])
          ref = np.max(a)
          res,_ = dsp.arm_max_f32(a)
          self.assertTrue(ref==res)

     def test_min_f32(self):
          a=np.array([1.,2,3,4,5,6,7,8,9,10,11,12])
          ref = np.min(a)
          res,_ = dsp.arm_min_f32(a)
          self.assertTrue(ref==res)

     def test_mean_f32(self):
          a=np.array([1.,2,3,4,5,6,7,8,9,10,11,12])
          ref = np.mean(a)
          res = dsp.arm_mean_f32(a)
          self.assertTrue(ref==res)

     def test_power_f32(self):
          a=np.array([1.,2,3,4,5,6,7,8,9,10,11,12])
          ref = np.dot(a,a)
          res = dsp.arm_power_f32(a)
          self.assertTrue(ref==res)



class TestTransformMethods_Test1(unittest.TestCase):

     def setUp(self):
        self.nb = 128
        self.signal = np.cos(2 * np.pi * np.arange(self.nb) / self.nb)
        return super().setUp()

     def test_cfft_f32(self):
          signal = self.signal
          result=np.fft.fft(signal)
          signalR = imToReal(signal)
          cfftf32=dsp.arm_cfft_instance_f32()
          status=dsp.arm_cfft_init_f32(cfftf32,self.nb)
          self.assertTrue(status==0)
          if dsp.has_neon():
             tmp_nb = dsp.arm_cfft_tmp_buffer_size(dt.F32,self.nb,1)
             tmp = np.zeros(tmp_nb,dtype=np.float32)
             resultR = dsp.arm_cfft_f32(cfftf32,signalR,0,tmp=tmp)
          else:
             resultR = dsp.arm_cfft_f32(cfftf32,signalR,0,1)
          resultI = realToIm(resultR)
          assert_allclose(resultI,result,rtol=3e-6,atol=3e-6)

     def test_cfft_q31(self):
          signal = self.signal
          result=np.fft.fft(signal)
          signalR = imToReal(signal)
          signalRQ31=toQ31(signalR)
          cfftq31=dsp.arm_cfft_instance_q31()
          status=dsp.arm_cfft_init_q31(cfftq31,self.nb)
          self.assertTrue(status==0)
          if dsp.has_neon():
             tmp_nb = dsp.arm_cfft_tmp_buffer_size(dt.Q31,self.nb,1)
             tmp = np.zeros(tmp_nb,dtype=np.int32)
             resultR = dsp.arm_cfft_q31(cfftq31,signalRQ31,0,tmp=tmp)
          else:
             resultR = dsp.arm_cfft_q31(cfftq31,signalRQ31,0,1)
          resultI = realToIm(Q31toF32(resultR))*self.nb
          assert_allclose(resultI,result,rtol=3e-6,atol=3e-6)

     def test_cfft_q15(self):
          signal = self.signal / self.nb
          result=np.fft.fft(signal)
          signalR = imToReal(signal)
          signalRQ15=toQ15(signalR)
          cfftq15=dsp.arm_cfft_instance_q15()
          status=dsp.arm_cfft_init_q15(cfftq15,self.nb)
          self.assertTrue(status==0)
          if dsp.has_neon():
             tmp_nb = dsp.arm_cfft_tmp_buffer_size(dt.Q15,self.nb,1)
             tmp = np.zeros(tmp_nb,dtype=np.int16)
             resultR = dsp.arm_cfft_q15(cfftq15,signalRQ15,0,tmp=tmp)
          else:
             resultR = dsp.arm_cfft_q15(cfftq15,signalRQ15,0,1)
          resultR=Q15toF32(resultR)
          resultI = realToIm(resultR)*self.nb
          assert_allclose(resultI,result,rtol=5e-3,atol=4e-2)

    

class TestBiquadMethods_Test1(unittest.TestCase):
     
     def setUp(self):
        return super().setUp()
     
     def test_biquad_cascade_df1_f32(self):
          biquadf32 = dsp.arm_biquad_casd_df1_inst_f32()
          numStages=1
          state=np.zeros(numStages*4)
          coefs=[1.,2,3,4,5]
          dsp.arm_biquad_cascade_df1_init_f32(biquadf32,1,coefs,state)
          ref = signal.lfilter([1.,2,3],[1,-4.,-5.],[1.,2,3,4,5])
          res = dsp.arm_biquad_cascade_df1_f32(biquadf32,[1,2,3,4,5])
          assert_allclose(ref,res,1e-6)


if __name__ == '__main__':
    unittest.main()