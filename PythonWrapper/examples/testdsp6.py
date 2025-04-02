import cmsisdsp as dsp
import numpy as np

import unittest
from numpy.testing import assert_allclose,assert_equal

from testtools import *

from scipy.interpolate import CubicSpline
from sklearn.naive_bayes import GaussianNB
from sklearn import svm
import math

SORT_BITONIC=0
SORT_BUBBLE=1
SORT_HEAP=2
SORT_INSERTION=3
SORT_QUICK=4
SORT_SELECTION=5


SORT_DESCENDING = 0
SORT_ASCENDING = 1

class TestSortMethods_Test6(unittest.TestCase):

    def setUp(self):
        self.a=[1,4,2,6,7,0,-3,5]
        self.ref=sorted(self.a)
        self.sortinst=dsp.arm_sort_instance_f32()


        return super().setUp()
    

    def test_bitonic_asc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_BITONIC,SORT_ASCENDING)
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,self.ref,1e-6,1e-6)

    def test_bubble_asc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_BUBBLE,SORT_ASCENDING)
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,self.ref,1e-6,1e-6)

    def test_heap_asc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_HEAP,SORT_ASCENDING)
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,self.ref,1e-6,1e-6)

    def test_insertion_asc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_INSERTION,SORT_ASCENDING)
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,self.ref,1e-6,1e-6)

    def test_quick_asc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_QUICK,SORT_ASCENDING)
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,self.ref,1e-6,1e-6)

    def test_selection_asc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_SELECTION,SORT_ASCENDING)
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,self.ref,1e-6,1e-6)

    def test_bubble_desc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_BUBBLE,SORT_DESCENDING)
        ref = self.ref.copy()
        ref.reverse()
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,ref,1e-6,1e-6)

    def test_heap_desc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_HEAP,SORT_DESCENDING)
        ref = self.ref.copy()
        ref.reverse()
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,ref,1e-6,1e-6)

    def test_insertion_desc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_INSERTION,SORT_DESCENDING)
        ref = self.ref.copy()
        ref.reverse()
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,ref,1e-6,1e-6)

    def test_quick_desc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_QUICK,SORT_DESCENDING)
        ref = self.ref.copy()
        ref.reverse()
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,ref,1e-6,1e-6)

    def test_selection_desc_f32(self):
        dsp.arm_sort_init_f32(self.sortinst,SORT_SELECTION,SORT_DESCENDING)
        ref = self.ref.copy()
        ref.reverse()
        res=dsp.arm_sort_f32(self.sortinst,self.a)
        assert_allclose(res,ref,1e-6,1e-6)


class TestInterpolateMethods_Test6(unittest.TestCase):

    def setUp(self):
        self.x = np.arange(0, 2*np.pi+np.pi/4, np.pi/4)
        self.y = np.sin(self.x)
        return super().setUp()
    
    def test_spline_f32(self):
        xnew = np.arange(0, 2*np.pi+np.pi/16, np.pi/16)
        ynew = CubicSpline(self.x,self.y,bc_type="natural")
        yref=ynew(xnew)

        splineInst = dsp.arm_spline_instance_f32()
        dsp.arm_spline_init_f32(splineInst,0,self.x,self.y)
        yres=dsp.arm_spline_f32(splineInst,xnew)
   
        assert_allclose(yref,yres,1e-6,1e-6)


class TestClassicalMLMethods_Test6(unittest.TestCase):

    def setUp(self):
        self.NBVECS = 100
        self.VECDIM = 2
   
        # 3 cluster of points are generated (3 classes)
        ballRadius = 1.0
        x1 = [1.5, 1] +  ballRadius * np.random.randn(self.NBVECS,self.VECDIM)
        x2 = [-1.5, 1] + ballRadius * np.random.randn(self.NBVECS,self.VECDIM)
        x3 = [0, -3] + ballRadius * np.random.randn(self.NBVECS,self.VECDIM)
   
        # All points are concatenated
        X_train=np.concatenate((x1,x2,x3))
   
        # The classes are 0,1 and 2.
        Y_train=np.concatenate((np.zeros(self.NBVECS),np.ones(self.NBVECS),2*np.ones(self.NBVECS)))
        
        self.gnb = GaussianNB()
        self.gnb.fit(X_train, Y_train)

        ballRadius = 0.5
        x = ballRadius * np.random.randn(self.NBVECS, 2)
        
        angle = 2.0 * math.pi * np.random.randn(1, self.NBVECS)
        radius = 3.0 + 0.1 * np.random.randn(1, self.NBVECS)
        
        xa = np.zeros((self.NBVECS,2))
        xa[:, 0] = radius * np.cos(angle)
        xa[:, 1] = radius * np.sin(angle)
        
        X_train = np.concatenate((x, xa))
        Y_train = np.concatenate((np.zeros(self.NBVECS), np.ones(self.NBVECS)))

        self.clf = svm.SVC(kernel='poly', gamma='auto', coef0=1.1)
        self.clf.fit(X_train, Y_train)

        return super().setUp()
    
    def test_bayes_f32(self):
        
        # Reusing example from https://developer.arm.com/documentation/102052/0000/Train-your-Bayesian-estimator-with-scikit-learn

        src1=[1.5,1.0]
        src2=[-1.5,1]
        src3=[0,-3]
        ref1 = self.gnb.predict([src1])
   
        ref2 = self.gnb.predict([src2])
   
        ref3 = self.gnb.predict([src3])

        theta=list(np.reshape(self.gnb.theta_,np.size(self.gnb.theta_)))
   
        # Gaussian variances
        sigma=list(np.reshape(self.gnb.var_,np.size(self.gnb.var_)))
   
        # Class priors
        prior=list(np.reshape(self.gnb.class_prior_,np.size(self.gnb.class_prior_)))
   
        epsilon=self.gnb.epsilon_

        bayesInst = dsp.arm_gaussian_naive_bayes_instance_f32(
          vectorDimension=self.VECDIM,numberOfClasses=3,
          theta=theta,sigma=sigma,classPriors=prior,epsilon=epsilon)

        _,res1=dsp.arm_gaussian_naive_bayes_predict_f32(bayesInst,src1)
        _,res2=dsp.arm_gaussian_naive_bayes_predict_f32(bayesInst,src2)
        _,res3=dsp.arm_gaussian_naive_bayes_predict_f32(bayesInst,src3)

        self.assertEqual(res1 , ref1)
        self.assertEqual(res2 , ref2)
        self.assertEqual(res3 , ref3)

    def test_svm_f32(self):

        test1 = np.array([0.4,0.1])
        test1 = test1.reshape(1,-1)
   
        refpredicted1 = self.clf.predict(test1)

        test2 = np.array([3.1,0.1])
        test2 = test2.reshape(1,-1)
   
        refpredicted2 = self.clf.predict(test2)

        supportShape = self.clf.support_vectors_.shape
   
        nbSupportVectors = supportShape[0]
        vectorDimensions = supportShape[1]
        
        degree=self.clf.degree
        coef0=self.clf.coef0
        gamma=self.clf._gamma
        
        intercept=self.clf.intercept_
        
        dualCoefs = self.clf.dual_coef_
        dualCoefs = dualCoefs.reshape(nbSupportVectors)
        supportVectors = self.clf.support_vectors_
        supportVectors = supportVectors.reshape(nbSupportVectors * self.VECDIM)
        
        svmInst=dsp.arm_svm_polynomial_instance_f32() 
        
        
        dsp.arm_svm_polynomial_init_f32(svmInst,nbSupportVectors,vectorDimensions,
            intercept[0],dualCoefs,supportVectors,
            [0,1],degree,coef0,gamma)
        
        test1 = np.array([0.4,0.1])
        predicted1 = dsp.arm_svm_polynomial_predict_f32(svmInst,test1)
   
        test2 = np.array([3.1,0.1])
        predicted2 = dsp.arm_svm_polynomial_predict_f32(svmInst,test2)

        self.assertEqual(predicted1,refpredicted1)
        self.assertEqual(predicted2,refpredicted2)
        
    
if __name__ == '__main__':
    unittest.main()