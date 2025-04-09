import cmsisdsp as dsp
import numpy as np
from scipy import signal
#import matplotlib.pyplot as plt
from scipy.fftpack import dct 
from scipy.signal import lfilter
import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

class TestDTWMethods_Test10(unittest.TestCase):

    def setUp(self):
        self.refWin1=np.array([[1, 1, 1, 0, 0],
                               [1, 1, 1, 1, 0],
                               [1, 1, 1, 1, 1],
                               [0, 1, 1, 1, 1],
                               [0, 0, 1, 1, 1],
                               [0, 0, 0, 1, 1],
                               [0, 0, 0, 0, 1],
                               [0, 0, 0, 0, 0],
                               [0, 0, 0, 0, 0],
                               [0, 0, 0, 0, 0]], dtype=np.int8)

        self.dtwWindow=np.zeros((10,5),dtype=np.int8)
        self.wsize_ref1=2

        self.refWin2=np.array([[1, 1, 0, 0, 0],
                               [1, 1, 0, 0, 0],
                               [1, 1, 1, 0, 0],
                               [0, 1, 1, 0, 0],
                               [0, 1, 1, 1, 0],
                               [0, 0, 1, 1, 0],
                               [0, 0, 1, 1, 1],
                               [0, 0, 0, 1, 1],
                               [0, 0, 0, 1, 1],
                               [0, 0, 0, 0, 1]], dtype=np.int8)
        
        self.wsize_ref2=1

        self.QUERY_LENGTH = 10 
        self.TEMPLATE_LENGTH = 5
        
        self.query=np.array([ 0.08387197,  0.68082274,  1.06756417,  0.88914541,  0.42513398, -0.3259053,
         -0.80934885, -0.90979435, -0.64026483,  0.06923695])
        
        self.template=np.array([ 1.00000000e+00,  7.96326711e-04, -9.99998732e-01, -2.38897811e-03,
          9.99994927e-01])
        
        self.cols=np.array([1,2,3])
        self.rows=np.array([10,11,12])

        self.referenceCost=np.array([[0.91612804, 0.9992037 , 2.0830743 , 2.1693354 , 3.0854583 ],
                                     [1.2353053 , 1.6792301 , 3.3600516 , 2.8525472 , 2.8076797 ],
                                     [1.3028694 , 2.3696373 , 4.4372    , 3.9225004 , 2.875249  ],
                                     [1.4137241 , 2.302073  , 4.1912174 , 4.814035  , 2.9860985 ],
                                     [1.98859   , 2.2623994 , 3.6875322 , 4.115055  , 3.5609593 ],
                                     [3.3144953 , 2.589101  , 3.2631946 , 3.586711  , 4.8868594 ],
                                     [5.123844  , 3.3992462 , 2.9704008 , 3.7773607 , 5.5867043 ],
                                     [7.0336385 , 4.309837  , 3.0606053 , 3.9680107 , 5.8778    ],
                                     [8.673903  , 4.950898  , 3.420339  , 4.058215  , 5.698475  ],
                                     [9.604667  , 5.0193386 , 4.489575  , 3.563591  , 4.494349  ]],
                                     dtype=np.float32)

        self.referenceDistance = 0.2996232807636261

        nan = np.nan 

        self.referenceDistance_with = 0.617099940776825
        self.referenceCost_with=np.array([[9.1612804e-01, 9.9920368e-01, nan, nan,
                nan],
               [1.2353053e+00, 1.6792301e+00, nan, nan,
                nan],
               [1.3028694e+00, 2.3696373e+00, 4.4372001e+00, nan,
                nan],
               [nan, 3.0795674e+00, 4.9687119e+00, nan,
                nan],
               [nan, 3.5039051e+00, 4.9290380e+00, 5.3565612e+00,
                nan],
               [nan, nan, 4.8520918e+00, 5.1756082e+00,
                nan],
               [nan, nan, 5.0427418e+00, 5.8497019e+00,
                7.6590457e+00],
               [nan, nan, nan, 6.7571073e+00,
                8.6668968e+00],
               [nan, nan, nan, 7.3949833e+00,
                9.0352430e+00],
               [nan, nan, nan, nan,
                9.2564993e+00]], dtype=np.float32)
        
        return super().setUp()
    
    def test_dtw_window_sakoe_chiba_init(self):
        status,w=dsp.arm_dtw_init_window_q7(dsp.ARM_DTW_SAKOE_CHIBA_WINDOW,self.wsize_ref1,self.dtwWindow)
        self.assertEqual(status,0)
        self.assertTrue((w==self.refWin1).all())

    def test_dtw_window_slanted_init(self):
        status,w=dsp.arm_dtw_init_window_q7(dsp.ARM_DTW_SLANTED_BAND_WINDOW,self.wsize_ref2,self.dtwWindow)
        self.assertEqual(status,0)
        self.assertTrue((w==self.refWin2).all())

    def test_dtw_without_window_f32(self):
        a,b = np.meshgrid(self.template,self.query)
        distance=abs(a-b).astype(np.float32)
        
        status,dtwDistance,dtwMatrix = dsp.arm_dtw_distance_f32(distance,None)
        self.assertEqual(status,0)
        
        assert_allclose(self.referenceDistance,dtwDistance)
        assert_allclose(self.referenceCost,dtwMatrix)

    def test_dtw_with_window_f32(self):
        w = self.refWin2
        a,b = np.meshgrid(self.template,self.query)
        distance=abs(a-b).astype(np.float32)
        
        status,dtwDistance,dtwMatrix = dsp.arm_dtw_distance_f32(distance,w)
        self.assertEqual(status,0)
        
        assert_allclose(self.referenceDistance_with,dtwDistance)
        assert_allclose(self.referenceCost_with[w==1],dtwMatrix[w==1])
        
     
if __name__ == '__main__':
    unittest.main()