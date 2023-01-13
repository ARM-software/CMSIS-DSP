import os.path
import itertools
import Tools
import random
import numpy as np
import scipy.spatial

NBTESTSAMPLES = 10

VECDIM = [35,14,20]

query=np.array([ 0.08387197,  0.68082274,  1.06756417,  0.88914541,  0.42513398, -0.3259053,
 -0.80934885, -0.90979435, -0.64026483,  0.06923695])

template=np.array([ 1.00000000e+00,  7.96326711e-04, -9.99998732e-01, -2.38897811e-03,
  9.99994927e-01])

def euclidean(xa,xb):
        r = scipy.spatial.distance.euclidean(xa,xb)
        return(r)

def braycurtis(xa,xb):
        r = scipy.spatial.distance.braycurtis(xa,xb)
        return(r)

def canberra(xa,xb):
        r = scipy.spatial.distance.canberra(xa,xb)
        return(r)

def chebyshev(xa,xb):
        r = scipy.spatial.distance.chebyshev(xa,xb)
        return(r)

def cityblock(xa,xb):
        r = scipy.spatial.distance.cityblock(xa,xb)
        return(r)

def correlation(xa,xb):
        r = scipy.spatial.distance.correlation (xa,xb)
        return(r)

def cosine(xa,xb):
        r = scipy.spatial.distance.cosine (xa,xb)
        return(r)

def jensenshannon(xa,xb):
        r = scipy.spatial.distance.jensenshannon (xa,xb)
        return(r)

def minkowski (xa,xb,dim):
        r = scipy.spatial.distance.minkowski(xa,xb,p=dim)
        return(r)

def dice(xa,xb):
        r = scipy.spatial.distance.dice (xa,xb)
        return(r)

def hamming(xa,xb):
        r = scipy.spatial.distance.hamming (xa,xb)
        return(r)

def jaccard(xa,xb):
        r = scipy.spatial.distance.jaccard (xa,xb)
        return(r)

def kulsinski(xa,xb):
        r = scipy.spatial.distance.kulsinski (xa,xb)
        return(r)

def rogerstanimoto(xa,xb):
        r = scipy.spatial.distance.rogerstanimoto (xa,xb)
        return(r)

def russellrao(xa,xb):
        r = scipy.spatial.distance.russellrao (xa,xb)
        return(r)

def sokalmichener(xa,xb):
        r = scipy.spatial.distance.sokalmichener (xa,xb)
        return(r)

def sokalsneath(xa,xb):
        r = scipy.spatial.distance.sokalsneath (xa,xb)
        return(r)

def yule(xa,xb):
        r = scipy.spatial.distance.yule (xa,xb)
        return(r)

def writeDTW(config):
    config.setOverwrite(True)
    config.writeInput(10, query,"Query")
    config.writeInput(10, template,"Template")
    query_index    = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9]
    template_index = [0, 0, 0, 0, 0, 1, 2, 2, 2, 3, 4]
    path = np.array(list(zip(query_index,template_index))).flatten()
    config.writeReferenceS16(10,path ,"PathRef")
    # Distance computed with different windows
    references=[0.29962325787495825,
                0.41319151913793334,
                0.6170999383691333
               ]
    config.writeReference(10, references,"DTWRef")
    config.setOverwrite(False)

def writeFTest(config,funcList):
    dims=[] 
    dimsM=[]
    inputsA=[] 
    inputsB=[]
    inputsAJ=[] 
    inputsBJ=[]
    outputs=[] 
    outputMin=[] 
    outputJen=[] 
    for i in range(0,len(funcList)):
        outputs.append([])

    vecDim = VECDIM[0]

    dims.append(NBTESTSAMPLES)
    dims.append(vecDim)

    dimsM.append(NBTESTSAMPLES)
    dimsM.append(vecDim)

    for _ in range(0,NBTESTSAMPLES):
      normDim = np.random.choice([2,3,4])
      dimsM.append(normDim)
      va = np.random.randn(vecDim)
      # Normalization for distance assuming probability distribution in entry
      vb = np.random.randn(vecDim)
      for i in range(0,len(funcList)):
        func = funcList[i]
        outputs[i].append(func(va,vb))
      outputMin.append(minkowski(va,vb,normDim))

      inputsA += list(va) 
      inputsB += list(vb)

      va = np.abs(va)
      va = va / np.sum(va)

      vb = np.abs(vb)
      vb = vb / np.sum(vb)

      inputsAJ += list(va) 
      inputsBJ += list(vb)
      outputJen.append(jensenshannon(va,vb)) 


    inputsA=np.array(inputsA)
    inputsB=np.array(inputsB)
    for i in range(0,len(funcList)):
      outputs[i]=np.array(outputs[i])
    
    config.writeInput(1, inputsA,"InputA")
    config.writeInput(1, inputsB,"InputB")
    config.writeInput(8, inputsAJ,"InputA")
    config.writeInput(8, inputsBJ,"InputB")
    config.writeInputS16(1, dims,"Dims")
    config.writeInputS16(9, dimsM,"Dims")

    for i in range(0,len(funcList)):
       config.writeReference(i+1, outputs[i],"Ref")

    config.writeReference(8, outputJen,"Ref")
    config.writeReference(9, outputMin,"Ref")

    writeDTW(config)
    

def writeBTest(config,funcList):
    dims=[] 
    inputsA=[] 
    inputsB=[]
    outputs=[] 
    for i in range(0,len(funcList)):
        outputs.append([])

    vecDim = VECDIM[0]

    dims.append(NBTESTSAMPLES)
    dims.append(vecDim)
    va = np.random.choice([0,1],vecDim)
    # Number of word32 containing all of our bits
    pva = Tools.packset(va)
    dims.append(len(pva))

    for _ in range(0,NBTESTSAMPLES):
      va = np.random.choice([0,1],vecDim)
      vb = np.random.choice([0,1],vecDim)
      # Boolean arrays are packed for the C code
      pva = Tools.packset(va)
      pvb = Tools.packset(vb)
      for i in range(0,len(funcList)):
        func = funcList[i]
        outputs[i].append(func(va,vb))

      inputsA += pva 
      inputsB += pvb

    inputsA=np.array(inputsA)
    inputsB=np.array(inputsB)
    for i in range(0,len(funcList)):
      outputs[i]=np.array(outputs[i])
    
    config.writeInput(1, inputsA,"InputA")
    config.writeInput(1, inputsB,"InputB")
    config.writeInputS16(1, dims,"Dims")

    for i in range(0,len(funcList)):
       config.writeReferenceF32(i+1, outputs[i],"Ref")

def writeFTests(config):
    writeFTest(config,[braycurtis,canberra,chebyshev,cityblock,correlation,cosine,euclidean])

def writeBTests(config):
    writeBTest(config,[dice,hamming,jaccard,kulsinski,rogerstanimoto,russellrao,sokalmichener,sokalsneath,yule])

def writeFBenchmark(config):
    NBSAMPLES=256

    va = np.random.randn(NBSAMPLES)
    vb = np.random.randn(NBSAMPLES)

    inputsA = list(va) 
    inputsB = list(vb)

    va = np.abs(va)
    va = list(va / np.sum(va))

    vb = np.abs(vb)
    vb = list(vb / np.sum(vb))

    config.writeInput(1, inputsA,"InputBenchA")
    config.writeInput(1, inputsB,"InputBenchB")

    config.writeInput(1, va,"InputBenchProbaA")
    config.writeInput(1, vb,"InputBenchProbaB")

def writeUBenchmark(config):
    NBSAMPLES=256*32
    va = np.random.choice([0,1],NBSAMPLES)
    vb = np.random.choice([0,1],NBSAMPLES)
    pva = list(Tools.packset(va))
    pvb = list(Tools.packset(vb))
    config.writeInput(1, pva,"InputBenchA")
    config.writeInput(1, pvb,"InputBenchB")



def  generatePatterns():
     PATTERNDIR = os.path.join("Patterns","DSP","Distance","Distance")
     PARAMDIR = os.path.join("Parameters","DSP","Distance","Distance")
     
     configf64=Tools.Config(PATTERNDIR,PARAMDIR,"f64")
     configf32=Tools.Config(PATTERNDIR,PARAMDIR,"f32")
     configf16=Tools.Config(PATTERNDIR,PARAMDIR,"f16")
     configu32=Tools.Config(PATTERNDIR,PARAMDIR,"u32")
     
     configf64.setOverwrite(False)
     configf32.setOverwrite(False)
     configf16.setOverwrite(False)
     configu32.setOverwrite(False)


     writeFTests(configf64)
     writeFTests(configf32)
     writeFTests(configf16)
     writeBTests(configu32)

     writeFBenchmark(configf32)
     writeFBenchmark(configf16)
     writeUBenchmark(configu32)

if __name__ == '__main__':
  generatePatterns()