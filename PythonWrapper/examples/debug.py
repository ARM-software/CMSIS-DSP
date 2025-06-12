import cmsisdsp as dsp 
import cmsisdsp.fixedpoint as f 

import numpy as np
from numpy.testing import assert_allclose

def normalize(a):
    return(2*a-1.0)

NBR = 500
NBI = 400 
NBC = 600
m = max(NBR, NBI, NBC)
a = np.random.rand(NBR,NBI).astype(np.float32)
b = np.random.rand(NBI,NBC).astype(np.float32)
a= normalize(a)/m
b= normalize(b)/m

aQ15 = f.toQ15(a)
bQ15 = f.toQ15(b)

tmp=np.zeros(NBI*NBC,dtype=np.int8)
cQ15 = dsp.arm_mat_mult_q15(aQ15, bQ15,tmp)[1]
c = f.Q15toF32(cQ15)

#assert_allclose(c, np.dot(a,b), rtol=1e-4, atol=1e-4)