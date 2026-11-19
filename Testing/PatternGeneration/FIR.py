import os.path
import numpy as np
import itertools
import Tools
from scipy import signal
#from pylab import figure, clf, plot, xlabel, ylabel, xlim, ylim, title, grid, axes, show,semilogx, semilogy

# Those patterns are used for tests and benchmarks.
# For tests, there is the need to add tests for saturation

def cartesian(*somelists):
   r=[]
   for element in itertools.product(*somelists):
       r.append(element)
   return(r)

def writeTests(config,format):
    NBSAMPLES=256
    NUMTAPS = 64

    samples=np.random.randn(NBSAMPLES)
    refs=np.random.randn(NBSAMPLES)
    taps=np.random.randn(NUMTAPS)

    samples = Tools.normalize(samples)
    refs = Tools.normalize(refs)
    taps = Tools.normalize(taps)
    

    ### For benchmarks

    config.writeInput(1, samples,"Samples")
    config.writeInput(1, taps,"Coefs")
    config.writeInput(1, refs,"Refs")

    ### For tests

    # blocksize 1 2 3 8 11 
    # taps 1 2 3 4 5 6 7 8 11 25
    # state numTaps + blockSize - 1
    # ref blockSize

    # Maximum number of samples for all tested FIR configurations is 2*23
    t = np.linspace(0, 1, 2*23)

    x = np.sin(2*np.pi*50*t)+np.random.randn(len(t)) * 0.08
    x = Tools.normalize(x)
    # To avoid saturation
    x = x / 30.0
    

    config.writeInput(1, x,"FirInput")
    tapConfigs=[] 
    output=[] 
    defs=[] 

    if format == Tools.F64 or format == 0 or format == 31:
       blk = [1, 2, 3, 8, 9,10,11, 16, 23]
       taps = [1, 2, 3, 4, 5, 6, 7, 8, 11, 16, 23, 25]
    elif format == 15 or format == 16:
       blk = [1, 2, 3, 12,13,14,15]
       taps = [2, 3, 4, 5, 6, 7, 8, 11, 25]
    elif format == 7:
       blk = [1, 2, 3 ,20,21,22,23]
       taps = [1, 2, 3, 4, 5, 6, 7, 8, 11, 25]

    configs = cartesian(blk,taps)

    nb=1

    for (b,t) in configs:
        nbTaps=t
        # nbTaps + 2 to be sure all coefficients are not saturated
        pythonCoefs = np.array(list(range(1,nbTaps+1)))/(1.0*(nbTaps+2))
        coefs=pythonCoefs

        if format == 15:
          if t % 2 == 1:
            nbTaps = nbTaps + 1 
            coefs = np.append(coefs,[0.0])
        
        out=signal.lfilter(pythonCoefs,[1.0],x[0:2*b])

        output += list(out)
        coefs = list(coefs)
        coefs.reverse()
        tapConfigs += coefs
        defs += [b,nbTaps]

        nb = nb + 1

    config.writeInput(1, output,"FirRefs")
    config.writeInput(1, tapConfigs,"FirCoefs")
    config.writeReferenceS16(1,defs,"FirConfigs")

# Number of successive calls on the same instance in the long FIR tests
LONG_NBCALLS = 3

def writeLongTests(config,format):
    """ Q15 and Q7 FIR with a number of taps above the limit of the
    specialized Helium kernels (32 for Q15, 64 for Q7), for all the values
    of blockSize & 3.

    The references are computed with an exact model of the fixed point
    FIR, so the test is bit exact :
    - Q15 : 64-bit accumulator, >> 15, saturation to 16 bits
    - Q7  : 32-bit accumulator, >> 7, saturation to 8 bits

    The coefficients are written in the CMSIS-DSP order
    (b[numTaps-1], ..., b[0]) and the filter is
    y[n] = sum_k state[n + k] * pCoeffs[k]
    where state is the history (numTaps - 1 zeros after the init)
    followed by the input samples.
    """
    rng = np.random.default_rng(1234 + format)
    blk = [1, 2, 3, 4, 5, 6, 7, 8, 13, 16]
    if format == 15:
       # Even numbers of taps : arm_fir_init_q15 requires an even numTaps
       # with ARM_MATH_LOOPUNROLL
       taps = [34, 40, 48, 64, 100, 200]
       scale = 2**15
       shift = 15
       satmin, satmax = -2**15, 2**15 - 1
       coefmax = 2**12
    else:
       taps = [65, 72, 96, 128, 200]
       scale = 2**7
       shift = 7
       satmin, satmax = -2**7, 2**7 - 1
       coefmax = 2**4

    nbInput = LONG_NBCALLS * max(blk)
    # Full scale input : the outputs saturate for some configurations
    x = [int(v) for v in rng.integers(satmin, satmax + 1, nbInput)]

    tapConfigs=[]
    output=[]
    defs=[]

    for b in blk:
        for t in taps:
            c = [int(v) for v in rng.integers(-coefmax, coefmax + 1, t)]
            state = [0] * (t - 1) + x[0:LONG_NBCALLS * b]
            for n in range(LONG_NBCALLS * b):
                acc = sum(state[n + k] * c[k] for k in range(t))
                output.append(min(max(acc >> shift, satmin), satmax))
            tapConfigs += c
            defs += [b, t]

    config.writeInput(2, [v / scale for v in x],"FirInput")
    config.writeInput(2, [v / scale for v in output],"FirRefs")
    config.writeInput(2, [v / scale for v in tapConfigs],"FirCoefs")
    config.writeReferenceS16(2,defs,"FirConfigs")


def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","Filtering","FIR","FIR")
    PARAMDIR = os.path.join("Parameters","DSP","Filtering","FIR","FIR")
    
    configf64=Tools.Config(PATTERNDIR,PARAMDIR,"f64")
    configf32=Tools.Config(PATTERNDIR,PARAMDIR,"f32")
    configf16=Tools.Config(PATTERNDIR,PARAMDIR,"f16")
    configq31=Tools.Config(PATTERNDIR,PARAMDIR,"q31")
    configq15=Tools.Config(PATTERNDIR,PARAMDIR,"q15")
    configq7=Tools.Config(PATTERNDIR,PARAMDIR,"q7")
    
    configf64.setOverwrite(False)
    configf32.setOverwrite(False)
    configf16.setOverwrite(False)
    configq31.setOverwrite(False)
    configq15.setOverwrite(False)
    configq7.setOverwrite(False)
    
    writeTests(configf64,Tools.F64)
    writeTests(configf32,0)
    writeTests(configf16,16)
    writeTests(configq31,31)
    writeTests(configq15,15)
    writeTests(configq7,7)

    writeLongTests(configq15,15)
    writeLongTests(configq7,7)


if __name__ == '__main__':
  generatePatterns()
