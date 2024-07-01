import cmsisdsp as dsp 
import numpy as np
from numpy.testing import assert_allclose
from scipy.stats import entropy,tstd, tvar
from scipy.special import logsumexp
from scipy.linalg import cholesky,ldl,solve_triangular
from scipy import signal


import colorama
from colorama import init,Fore, Back, Style

init()

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print("\n" + Style.BRIGHT + s + Style.RESET_ALL)


print("Cholesky")

a=np.array([[4,12,-16],[12,37,-43],[-16,-43,98]])
ref=cholesky(a,lower=True)
print(ref)

status,res=dsp.arm_mat_cholesky_f32(a)
print(res)
assert_allclose(ref,res,1e-6,1e-6)

status,res=dsp.arm_mat_cholesky_f64(a)
print(res)
assert_allclose(ref,res,1e-10,1e-10)