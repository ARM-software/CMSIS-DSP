# New functions for version 1.5 of the Python wrapper
import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f
import numpy as np
import math
import colorama
from colorama import init,Fore, Back, Style
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

init()

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print("\n" + Style.BRIGHT + s + Style.RESET_ALL)

printTitle("Householder")

VECDIM = 10 

a=np.random.randn(VECDIM)
a = a / np.max(np.abs(a))

# Reference
vRef,betaRef = householder(a)

printSubTitle("Householder F32")
betaF32,vF32 = dsp.arm_householder_f32(a,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32)
print(np.isclose(betaRef,betaF32,1e-6,1e-6))
print(np.isclose(vRef,vF32,1e-6,1e-6))

printSubTitle("Householder F64")
betaF64,vF64 = dsp.arm_householder_f64(a,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F64)
print(np.isclose(betaRef,betaF64,1e-6,1e-6))
print(np.isclose(vRef,vF64,1e-6,1e-6))

printSubTitle("Householder Proportional F32")
a=np.random.randn(5)
# With the threshold defined with DEFAULT_HOUSEHOLDER_THRESHOLD_F32
# this vector is considered as proportional to (1,0,...)
# and thus the function will return (0,[0,...,0])
a = a / np.max(np.abs(a)) * 1.0e-7
resF32 = dsp.arm_householder_f32(a,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32)
print(resF32)

# With a smaller threshold, a computation is taking place
resF32 = dsp.arm_householder_f32(a,0.001*dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32)
print(resF32)

printTitle("QR decomposition")

def checkOrtho(A,err=1e-10):
    product = A.T.dot(A)
    #print(A)
    np.fill_diagonal(product,0)
    #print(product)
    print(np.max(np.abs(product)))
    return (np.all(np.abs(product)<=err))

m=np.array([[-0.35564874, -0.07809871, -0.10350569, -0.50633135, -0.65073484],
            [-0.71887395,  0.45257918,  0.29606363,  0.1497621 ,  0.07002738],
            [-0.50586141, -0.50613839, -0.01650463, -0.29693649,  0.47667742],
            [ 0.06802137,  0.07689169, -0.02726221, -0.09996672,  0.15521956],
            [ 0.21220523, -0.22273009,  0.78247386, -0.2760002 , -0.24438688],
            [ 0.09683658,  0.62026597,  0.26771763, -0.26935342,  0.18443573],
            [-0.01014268,  0.27578087, -0.44635721, -0.21827312, -0.26463186],
            [-0.20420646, -0.12880459,  0.13207738,  0.65319578, -0.3956695 ]])

rows,columns = m.shape


# The CMSIS-DSP C functions is requiring two temporary arrays
# To follow the C function as closely as possible, we create
# two arrays. Their size will be used internally by the Python
# wrapper to allocate two temporary buffers.
# Like that you can check you have dimensionned the arrays in the
# right way.
# The content of the temporary buffers is not accesible from the
# Python API. tmpa and tmpb are not modified.
tmpa=np.zeros(rows)
tmpb=np.zeros(rows)

printSubTitle("QR F32")

status,r,q,tau = dsp.arm_mat_qr_f32(m,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F32,tmpa,tmpb)

# Status different from 0 if matrix dimensions are not right
# (rows must be >= columns)
#print(status)
#print(q)
#print(r)
#print(tau)

# Check that the matrix Q is orthogonal
assert(checkOrtho(q,err=1.0e-6))

# Remove householder vectors from R matrix
i=1
for c in r.T:
    c[i:] = 0
    i = i+1

# Check that M = Q R
newm = np.dot(q,r)
assert_allclose(newm,m,2e-6,1e-7)


printSubTitle("QR F64")

status,r,q,tau = dsp.arm_mat_qr_f64(m,dsp.DEFAULT_HOUSEHOLDER_THRESHOLD_F64,tmpa,tmpb)

# Status different from 0 if matrix dimensions are not right
# (rows must be >= columns)
#print(status)
#print(q)
#print(r)
#print(tau)

# Check that the matrix Q is orthogonal
assert(checkOrtho(q,err=1e-14))

# Remove householder vectors from R matrix
i=1
for c in r.T:
    c[i:] = 0
    i = i+1

# Check that M = Q R
newm = np.dot(q,r)
assert_allclose(newm,m)