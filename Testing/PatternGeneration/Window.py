import os.path
import numpy as np
import itertools
import Tools
import scipy.signal.windows as win
import matplotlib.pyplot as plt

def genwelch(n):
    ik = 2*np.array(range(n)) / n
    w = ik -1;
    w = 1 - w**2 
    if len(w)!=n:
        print("Error with window len in Welch")
        exit(1)
    return(w)


def genbartlett(n):
    w = win.bartlett(n,sym=False)
    return(w)

def genhamming(n):
    w = win.hamming(n,sym=False)
    return(w)

def genhanning(n):
    w = win.hann(n,sym=False)
    return(w)

def gennuttall3(n):
    w = win.general_cosine(n,
        [0.375, 0.5 , 0.125 ],sym=False)
    return(w)

def gennuttall4(n):
    w = win.general_cosine(n,
        [0.3125, 0.46875,0.1875 , 0.03125],sym=False)
    return(w)

def gennuttall3a(n):
    w = win.general_cosine(n,
        [0.40897, 0.5 ,0.09103],sym=False)
    return(w)

def gennuttall3b(n):
    w = win.general_cosine(n,
        [0.4243801, 0.4973406 , 0.0782793 ],sym=False)
    return(w)

def gennuttall4a(n):
    w = win.general_cosine(n,
        [0.338946, 0.481973,0.161054 , 0.018027 ],sym=False)
    return(w)

def genblackman_harris_92db(n):
    w = win.blackmanharris(n,sym=False)
    return(w)

def gennuttall4b(n):
    w = win.general_cosine(n,
        [0.355768, 0.487396 ,
        0.144232 , 0.012604 ],sym=False)
    return(w)

def gennuttall4c(n):
    w = win.nuttall(n,sym=False)
    return(w)

def genhft90d(n):
    w = win.general_cosine(n,
        [1 ,1.942604 ,
         1.340318 , 0.440811 , 0.043097 ],sym=False)
    return(w)

def genhft95(n):
    w = win.general_cosine(n,
        [1, 1.9383379 ,
         1.3045202 ,0.4028270 ,0.0350665 ]
 ,sym=False)
    return(w)

def genhft116d(n):
    w = win.general_cosine(n,
        [1, 1.9575375 ,
         1.4780705 ,0.6367431 ,
         0.1228389 ,0.0066288 ]
 ,sym=False)
    return(w)

def genhft144d(n):
    w = win.general_cosine(n,
        [1 ,1.96760033 ,
         1.57983607 , 0.81123644 ,
         0.22583558 ,0.02773848 , 0.00090360  ]
 ,sym=False)
    return(w)

def genhft169d(n):
    w = win.general_cosine(n,
        [1, 1.97441843 ,
         1.65409889 , 0.95788187 ,
         0.33673420 , 0.06364622 ,
         0.00521942 ,0.00010599   ]
 ,sym=False)
    return(w)

def genhft196d(n):
    w = win.general_cosine(n,
        [1, 1.979280420 ,
         1.710288951 , 1.081629853 ,
         0.448734314 , 0.112376628 ,
         0.015122992 ,0.000871252 , 0.000011896 ]
 ,sym=False)
    return(w)

def genhft223d(n):
    w = win.general_cosine(n,
        [1, 1.98298997309,
         1.75556083063 , 1.19037717712 ,
         0.56155440797 , 0.17296769663 ,
         0.03233247087 ,0.00324954578 ,
         0.00013801040 ,0.00000132725 ]
 ,sym=False)
    return(w)

def genhft248d(n):
    w = win.general_cosine(n,
        [1, 1.985844164102 ,
         1.791176438506 , 1.282075284005 ,
         0.667777530266 , 0.240160796576 ,
         0.056656381764 ,0.008134974479 ,
         0.000624544650 ,0.000019808998 ,
         0.000000132974 ]
 ,sym=False)
    return(w)


def writeTests(config,format):
    NBSAMPLES=128

    data1=np.random.randn(NBSAMPLES)
    data1 = Tools.normalize(data1)
    data1 = genwelch(NBSAMPLES)
    config.writeReference(1, data1,"RefWelch_")

    data1 = genbartlett(NBSAMPLES)
    config.writeReference(2, data1,"RefBartlett_")

    data1 = genhamming(NBSAMPLES)
    config.writeReference(3, data1,"RefHamming_")

    data1 = genhanning(NBSAMPLES)
    config.writeReference(4, data1,"RefHanning_")

    data1 = gennuttall3(NBSAMPLES)
    config.writeReference(5, data1,"RefNuttall3_")

    data1 = gennuttall4(NBSAMPLES)
    config.writeReference(6, data1,"RefNuttall4_")

    data1 = gennuttall3a(NBSAMPLES)
    config.writeReference(7, data1,"RefNuttall3a_")

    data1 = gennuttall3b(NBSAMPLES)
    config.writeReference(8, data1,"RefNuttall3b_")

    data1 = gennuttall4a(NBSAMPLES)
    config.writeReference(9, data1,"RefNuttall4a_")

    data1 = genblackman_harris_92db(NBSAMPLES)
    config.writeReference(10, data1,"RefBlackman_harris_92db_")

    data1 = gennuttall4b(NBSAMPLES)
    config.writeReference(11, data1,"RefNuttall4b_")

    data1 = gennuttall4c(NBSAMPLES)
    config.writeReference(12, data1,"RefNuttall4c_")

    data1 = genhft90d(NBSAMPLES)
    config.writeReference(13, data1,"RefHft90d_")

    data1 = genhft95(NBSAMPLES)
    config.writeReference(14, data1,"RefHft95_")

    data1 = genhft116d(NBSAMPLES)
    config.writeReference(15, data1,"RefHft116d_")

    data1 = genhft144d(NBSAMPLES)
    config.writeReference(16, data1,"RefHft144d_")

    data1 = genhft169d(NBSAMPLES)
    config.writeReference(17, data1,"RefHft169d_")

    data1 = genhft196d(NBSAMPLES)
    config.writeReference(18, data1,"RefHft196d_")

    data1 = genhft223d(NBSAMPLES)
    config.writeReference(19, data1,"RefHft223d_")

    data1 = genhft248d(NBSAMPLES)
    config.writeReference(20, data1,"RefHft248d_")


def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","Window","Window")
    PARAMDIR = os.path.join("Parameters","DSP","Window","Window")
    
    configf64=Tools.Config(PATTERNDIR,PARAMDIR,"f64")
    configf32=Tools.Config(PATTERNDIR,PARAMDIR,"f32")
    
    writeTests(configf64,Tools.F64)
    writeTests(configf32,Tools.F32)


    
if __name__ == '__main__':
  generatePatterns()
