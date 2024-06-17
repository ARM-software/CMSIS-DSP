import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f

import numpy as np
from scipy import signal
import matplotlib.pyplot as plt
import scipy.fft

import colorama
from colorama import init,Fore, Back, Style
from numpy.testing import assert_allclose
import scipy.spatial.distance as d 

init()

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print("\n" + Style.BRIGHT + s + Style.RESET_ALL)


def packset(a):
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
        print(c[i,:])
        #print("%X %X %X %X" % (c[i,0],c[i,1],c[i,2],c[i,3]))
        d = (c[i,0] << 24) | (c[i,1] << 16) | (c[i,2] << 8) | c[i,3] 
        result.append(np.uint32(d))
    return(result) 

nb = 34
#va = np.random.choice([0,1],nb)
# Array of word32 containing all of our bits
#pva = packset(va)


#vb = np.random.choice([0,1],nb)
# Array of word32 containing all of our bits
#pvb = packset(vb)
#
va=[1, 0, 1, 0, 1, 1, 1, 0 ,0, 1, 1, 0, 1, 0, 0, 0, 0, 1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1]
vb=[0,1,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,1,1,0,0,1,1,0,0,0,1,0]

va = np.array(va)
vb = np.array(vb)

pva=packset(va)
pvb=packset(vb)

#pva = [np.uint32(167), np.uint32(0)]
#pvb = [np.uint32(152), np.uint32(0)]

#print(va,pva)
#print(vb,pvb)

ctt=1.0*np.count_nonzero((va==1) & (vb==1))
ctf=1.0*np.count_nonzero((va==1) & (vb==0))
cft=1.0*np.count_nonzero((va==0) & (vb==1))

res=(cft+ctf)/(2*ctt+cft+ctf)

print(res)


print("\nDice")
ref=d.dice(va,vb)
res=dsp.arm_dice_distance(pva,pvb,nb)
print(ref)
print(res)
assert_allclose(ref,res,1e-6)
