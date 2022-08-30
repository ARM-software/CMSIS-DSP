# New functions for version 1.5 of the Python wrapper
import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f
import numpy as np
import math
import colorama
from colorama import init,Fore, Back, Style
from numpy.testing import assert_allclose


NBSAMPLES=256

data1=np.random.randn(NBSAMPLES)

ref = np.sum(data1)
#print(ref)
t = dsp.arm_accumulate_f64(data1)
#print(t)
assert_allclose(ref,t)

t = dsp.arm_accumulate_f32(data1)
#print(t)
assert_allclose(ref,t,rtol=2e-6)