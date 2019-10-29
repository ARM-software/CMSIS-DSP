import os.path
import itertools
import Tools
import random
import numpy as np

NBTESTSAMPLES = 10

# Nb vectors for barycenter
NBVECTORS = [4,10,16]

VECDIM = [12,14,20]

def genWsum(config,nb):
    DIM=10
    inputs=[] 
    weights=[]
    output=[] 


    va = np.random.rand(DIM)
    vb = np.random.rand(DIM)
    inputs += list(va)
    weights += list(vb)

    nbiters = Tools.loopnb(0,Tools.TAILONLY)
    e = np.sum(va[0:nbiters].T * vb[0:nbiters]) / np.sum(vb[0:nbiters]) 
    output.append(e)

    nbiters = Tools.loopnb(0,Tools.BODYONLY)
    e = np.sum(va[0:nbiters].T * vb[0:nbiters]) / np.sum(vb[0:nbiters]) 
    output.append(e)

    nbiters = Tools.loopnb(0,Tools.BODYANDTAIL)
    e = np.sum(va[0:nbiters].T * vb[0:nbiters]) / np.sum(vb[0:nbiters]) 
    output.append(e)

    inputs=np.array(inputs)
    weights=np.array(weights)
    output=np.array(output)
    config.writeInput(nb, inputs,"Inputs")
    config.writeInput(nb, weights,"Weights")
    config.writeReference(nb, output,"Ref")

def genBarycenter(config,nb,nbTests,nbVecsArray,vecDimArray):
    dims=[] 
    inputs=[] 
    weights=[]
    output=[] 

    dims.append(nbTests)
    

    for i in range(0,nbTests):
      nbVecs = nbVecsArray[i % len(nbVecsArray)]
      vecDim = vecDimArray[i % len(vecDimArray)]
      dims.append(nbVecs )
      dims.append(vecDim)

      vecs = []
      b = np.zeros(vecDim)
      coefs = np.random.rand(nbVecs)
  
      for i in range(nbVecs):
            va = np.random.rand(vecDim)
            b += va * coefs[i]
            vecs += list(va)
                     
      b = b / np.sum(coefs)
     
      inputs += list(vecs)
      weights += list(coefs)
      output += list(b)
    inputs=np.array(inputs)
    weights=np.array(weights)
    output=np.array(output)
    config.writeInput(nb, inputs,"Inputs")
    config.writeInputS16(nb, dims,"Dims")
    config.writeInput(nb, weights,"Weights")
    config.writeReference(nb, output,"Ref")

def writeTestsF32(config):
    NBSAMPLES=256

    va = np.random.rand(NBSAMPLES)
    va = Tools.normalize(va)
    config.writeInput(1,va,"Samples")

    config.writeInputQ15(3,va,"Samples")
    config.writeInputQ31(4,va,"Samples")
    config.writeInputQ7(5,va,"Samples")

    # This is for benchmarking the weighted sum and we use only one test pattern
    genWsum(config,6)
    


def writeTestsQ31(config):
    NBSAMPLES=256

    va = np.random.rand(NBSAMPLES)
    va = Tools.normalize(va)
    config.writeInputF32(1,va,"Samples")
    config.writeInputQ15(3,va,"Samples")
    config.writeInput(4,va,"Samples")
    config.writeInputQ7(5,va,"Samples")


def writeTestsQ15(config):
    NBSAMPLES=256

    va = np.random.rand(NBSAMPLES)
    va = Tools.normalize(va)
    config.writeInputF32(1,va,"Samples")
    config.writeInput(3,va,"Samples")
    config.writeInputQ31(4,va,"Samples")
    config.writeInputQ7(5,va,"Samples")

def writeTestsQ7(config):
    NBSAMPLES=256

    va = np.random.rand(NBSAMPLES)
    va = Tools.normalize(va)
    config.writeInputF32(1,va,"Samples")
    config.writeInputQ15(3,va,"Samples")
    config.writeInputQ31(4,va,"Samples")
    config.writeInput(5,va,"Samples")

def writeBarTests(config):
    # For testing
    NBSAMPLES = 10
    nbVecsArray = [4,8,9] 
    vecDimArray = [4,4,4,8,8,8,9,9,9]
    genBarycenter(config,1,NBTESTSAMPLES,nbVecsArray,vecDimArray)

    # For benchmarks
    va = np.random.rand(128*15)
    vb = np.random.rand(128)
    config.writeInput(1,va,"Samples")
    config.writeInput(1,vb,"Coefs")

def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","Support","Support")
    PARAMDIR = os.path.join("Parameters","DSP","Support","Support")
    
    configf32=Tools.Config(PATTERNDIR,PARAMDIR,"f32")
    configq31=Tools.Config(PATTERNDIR,PARAMDIR,"q31")
    configq15=Tools.Config(PATTERNDIR,PARAMDIR,"q15")
    configq7=Tools.Config(PATTERNDIR,PARAMDIR,"q7")
    
    writeTestsF32(configf32)
    writeTestsQ31(configq31)
    writeTestsQ15(configq15)
    writeTestsQ7(configq7)
    
    
    # For benchmarking we need to vary number of vectors and vector dimension separately
    PATTERNBARDIR = os.path.join("Patterns","DSP","SupportBar")
    PARAMBARDIR = os.path.join("Parameters","DSP","SupportBar")
    
    configBarf32=Tools.Config(PATTERNBARDIR,PARAMBARDIR,"f32")
    
    writeBarTests(configBarf32)

if __name__ == '__main__':
  generatePatterns()