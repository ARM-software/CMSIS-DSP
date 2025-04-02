import unittest
import cmsisdsp as dsp 
import numpy as np
import scipy.spatial.distance as d 
from numpy.testing import assert_allclose
from testtools import *

def kulsinski(va,vb):
    n = len(va)
    ctt=1.0*np.count_nonzero((va==1) & (vb==1))
    ctf=1.0*np.count_nonzero((va==1) & (vb==0))
    cft=1.0*np.count_nonzero((va==0) & (vb==1))
    return(1.0*(ctf + cft - ctt+n)/(cft + ctf + n))

def sokalmichener(va,vb):
    n = len(va)
    ctt=1.0*np.count_nonzero((va==1) & (vb==1))
    ctf=1.0*np.count_nonzero((va==1) & (vb==0))
    cft=1.0*np.count_nonzero((va==0) & (vb==1))
    cff=1.0*np.count_nonzero((va==0) & (vb==0))
    r = 2*(ctf + cft)
    s = cff + ctt
    return(1.0*r/(r+s))

# Int distance
# For CMSIS-DSP the bool must be packed as bit arrays

# Pack an array of boolean into uint32
def _packset(a):
    b = np.packbits(a)
    newSize = int(np.ceil(b.shape[0] / 4.0)) * 4
    c = np.copy(b).astype(np.uint32)
    c.resize(newSize)
    #print(c)
    vecSize = round(newSize/4)
    c=c.reshape(vecSize,4)
    #print(c)
    r = np.zeros(vecSize)
    result = []
    for i in range(0,vecSize):
        #print(c[i,:])
        #print("%X %X %X %X" % (c[i,0],c[i,1],c[i,2],c[i,3]))
        d = (c[i,0] << 24) | (c[i,1] << 16) | (c[i,2] << 8) | c[i,3] 
        result.append(np.uint32(d))
    return(result) 

class TestFloatDistanceMethods(unittest.TestCase):

    def setUp(self):
        self.a=[1,2,3]
        self.b=[1,5,2]
        return super().setUp()
    
    def test_braycurtis(self):
        ref=d.braycurtis(self.a,self.b)
        res=dsp.arm_braycurtis_distance_f32(self.a,self.b)
        assert_allclose(ref,res,1e-6)    

    def test_canberra(self):
        ref=d.canberra(self.a,self.b)
        res=dsp.arm_canberra_distance_f32(self.a,self.b)
        assert_allclose(ref,res,1e-6)

    def test_chebyshev_f32(self):
        ref=d.chebyshev(self.a,self.b)
        res=dsp.arm_chebyshev_distance_f32(self.a,self.b)
        assert_allclose(ref,res,1e-6)

    def test_chebyshev_f64(self):
        ref=d.chebyshev(self.a,self.b)
        res=dsp.arm_chebyshev_distance_f64(self.a,self.b)
        assert_allclose(ref,res,1e-6)

    def test_cityblock_f32(self):
        ref=d.cityblock(self.a,self.b)
        res=dsp.arm_cityblock_distance_f32(self.a,self.b)
        assert_allclose(ref,res,1e-6) 

    def test_cityblock_f64(self):
        ref=d.cityblock(self.a,self.b)
        res=dsp.arm_cityblock_distance_f64(self.a,self.b)
        assert_allclose(ref,res,1e-6) 

    def test_correlation(self):
        ref=d.correlation(self.a,self.b)
        res=dsp.arm_correlation_distance_f32(self.a,self.b)
        assert_allclose(ref,res,1e-6)

    def test_cosine_f32(self):
        ref=d.cosine(self.a,self.b)
        res=dsp.arm_cosine_distance_f32(self.a,self.b)
        assert_allclose(ref,res,1e-6)

    def test_cosine_f64(self):
        ref=d.cosine(self.a,self.b)
        res=dsp.arm_cosine_distance_f64(self.a,self.b)
        assert_allclose(ref,res,1e-6)

    def test_euclidean_f32(self):
        ref=d.euclidean(self.a,self.b)
        res=dsp.arm_euclidean_distance_f32(self.a,self.b)
        assert_allclose(ref,res,1e-6)
    
    def test_euclidean_f64(self):
        ref=d.euclidean(self.a,self.b)  
        res=dsp.arm_euclidean_distance_f64(self.a,self.b)
        assert_allclose(ref,res,1e-6)

    def test_jensenshannon(self):
        pa=self.a/np.sum(self.a)
        pb=self.b/np.sum(self.b)
        ref=d.jensenshannon(pa,pb)
        res=dsp.arm_jensenshannon_distance_f32(pa,pb)
        assert_allclose(ref,res,1e-6)

    def test_minkowski_f32(self):
        ref=d.minkowski(self.a,self.b,3)
        res=dsp.arm_minkowski_distance_f32(self.a,self.b,3)
        assert_allclose(ref,res,1e-6)


class TestIntegerDistanceMethods(unittest.TestCase):

    def setUp(self):
        self.nb = 34
        self.va = np.random.choice([0,1],self.nb)
        # Array of word32 containing all of our bits
        self.pva = _packset(self.va)
        self.vb = np.random.choice([0,1],self.nb)
        # Array of word32 containing all of our bits
        self.pvb = _packset(self.vb)
        return super().setUp()
    
    def test_dice(self):
        ref=d.dice(self.va,self.vb)
        res=dsp.arm_dice_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_hamming(self):
        ref=d.hamming(self.va,self.vb)
        res=dsp.arm_hamming_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_jaccard(self):
        ref=d.jaccard(self.va,self.vb)
        res=dsp.arm_jaccard_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_kulsinski(self):
        ref=kulsinski(self.va,self.vb)
        res=dsp.arm_kulsinski_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_rogerstanimoto(self):
        ref=d.rogerstanimoto(self.va,self.vb)
        res=dsp.arm_rogerstanimoto_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_russellrao(self):
        ref=d.russellrao(self.va,self.vb)
        res=dsp.arm_russellrao_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_sokalmichener(self):
        ref=sokalmichener(self.va,self.vb)
        res=dsp.arm_sokalmichener_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_sokalsneath(self):
        ref=d.sokalsneath(self.va,self.vb)
        res=dsp.arm_sokalsneath_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

    def test_yule(self):
        ref=d.yule(self.va,self.vb)
        res=dsp.arm_yule_distance(self.pva,self.pvb,self.nb)
        assert_allclose(ref,res,1e-6)

if __name__ == '__main__':
    unittest.main()