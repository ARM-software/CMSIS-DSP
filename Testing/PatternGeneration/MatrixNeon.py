import Tools
import itertools
import os
import numpy as np
from rich.progress import track
from rich.table import Column
from rich.progress import Progress, BarColumn, TextColumn
import argparse

parser = argparse.ArgumentParser(description='Generate more matrix multiplication patterns for Neon tests')

parser.add_argument('-d', action='store_true', help="Dry run")

args = parser.parse_args()

def cartesian(*somelists):
   r=[]
   for element in itertools.product(*somelists):
       r.append(element)
   return(r)

def randComplex(nb):
    data = np.random.randn(2*nb)
    data = Tools.normalize(data)
    data_comp = data.view(dtype=np.complex128)
    return(data_comp)

def asReal(a):
    #return(a.view(dtype=np.float64))
    return(a.reshape(np.size(a)).view(dtype=np.float64))

col_block=768
row_block=256
inner_block=128

#BLOCK_COLS=[0,col_block,2*col_block]
#BLOCK_ROWS=[0,row_block,2*row_block]
#BLOCK_INNER=[0,inner_block,2*inner_block]

BLOCK_COLS=[col_block,2*col_block,2*col_block+1]
BLOCK_ROWS=[row_block,2*row_block,2*row_block+1]
BLOCK_INNER=[inner_block,2*inner_block,2*inner_block+1]

def remove_zero(l):
    if len(l[0])==3:
       return list(filter(lambda x:x[0] != 0 or x[1] != 0 or x[2] != 0,l))
    else:
       return list(filter(lambda x:x[0] != 0 or x[1] != 0,l))

def sum(dims):
    if len(dims[0])==3:
       return [l[0]+l[1]+l[2] for l in dims]
    else:
       return [l[0]+l[1] for l in dims]
    
class Accept:
   def __init__(self,lim):
      self._lim = lim

   def accept(self,t):
      return t[0] <= self._lim and t[1] <= self._lim and t[2] <= self._lim

def writeBinaryTests(config,format,desc):
    sizes=[Tools.loopnb(format,Tools.TAILONLY),
    Tools.loopnb(format,Tools.BODYONLY),
    Tools.loopnb(format,Tools.BODYANDTAIL)
    ]

    DR = 4
    DC = 4

    nl = Tools.nblane(format)

    drs = [1,2,3,4,8,9,10,11]
    dcs = [1,2,3,4,8,9]

    #rows = sum(remove_zero(cartesian(drs,BLOCK_ROWS,sizes)))
    #cols = sum(remove_zero(cartesian(dcs,BLOCK_COLS,sizes)))
    #inners = sum(remove_zero(cartesian(BLOCK_INNER,sizes)))
    #
    OTHER = 32
    rows=[(x,OTHER,OTHER) for x in (drs+BLOCK_ROWS)]
    cols=[(OTHER,OTHER,x) for x in (dcs+BLOCK_COLS)]
    inners=[(OTHER,x,OTHER) for x in (BLOCK_INNER+sizes)]

    kernels = cartesian([1,2,3,4],list(range(1,nl))+[nl*1,nl*2,nl*3,nl*4])
    kernels_dims=[(x[0],32,x[1]) for x in kernels]
    #print(kernels_dims)


    if format == Tools.Q15:
       # Size is limited for Q15 and Q7 datatype otherwise we get saturation
       # or loss of accuracy since we need to shift the data too much to avoid
       # saturation
       limited_rows = filter(Accept(256).accept,rows)
       limited_cols = filter(Accept(256).accept,cols)
       limited_inners = filter(Accept(256).accept,inners)

    if format == Tools.Q7:
       # Size is limited for Q15 and Q7 datatype otherwise we get saturation
       # or loss of accuracy since we need to shift the data too much to avoid
       # saturation
       limited_rows = filter(Accept(128).accept,rows)
       limited_cols = filter(Accept(128).accept,cols)
       limited_inners = filter(Accept(128).accept,inners)

    maxnb = np.max(np.hstack([drs,dcs,sizes,BLOCK_COLS,BLOCK_ROWS,BLOCK_INNER]))
    print(f"Max matrix dim = {maxnb}")

    # To have reproducible tests
    rng = np.random.default_rng(seed=10)

    data1=rng.standard_normal(maxnb*maxnb)
    data2=rng.standard_normal(maxnb*maxnb)
    
    data1 = Tools.normalize(data1)
    data2 = Tools.normalize(data2)

    data1C=randComplex(maxnb*maxnb)
    data2C=randComplex(maxnb*maxnb)

    config.writeInput(1, data1,"InputA")
    config.writeInput(1, data2,"InputB")

    config.setOverwrite(True)
    config.writeInput(1, asReal(data1C),"InputAC")
    config.writeInput(1, asReal(data2C),"InputBC")
    config.setOverwrite(False)

    #
    binarySizes = sorted(rows)+sorted(inners)+sorted(cols)
    if format == Tools.Q15 or format == Tools.Q7:
       limited_binarySizes = sorted(limited_rows)+sorted(limited_inners)+sorted(limited_cols)
   
   
    binarySizes = kernels_dims + binarySizes
    if format == Tools.Q15 or format == Tools.Q7:
       limited_binarySizes = kernels_dims + limited_binarySizes
    print(f"Nb matrixes {len(binarySizes)}")
    #return
    dims=[l[0]*l[2] for l in binarySizes]
    bytes=25*np.sum(dims)
    print(f"Estimated size of result text file : {bytes} bytes")

    if format == Tools.Q15 or format == Tools.Q7:
       limited_dims=[l[0]*l[2] for l in limited_binarySizes]
       limited_bytes=25*np.sum(limited_dims)
       print(f"Estimated size of limited result text file : {limited_bytes} bytes")

    if args.d:
         print("Dry run, no file generated")
         print("rows",rows)
         print("inners",inners)
         print("cols",cols)
         exit(0)
         
    dims=[] 
    vals=[]
    valsC=[]
    valsCR=[]


    nb = 0
    with Progress() as progress:
        for (a,b,c) in progress.track(binarySizes, description=desc):
           progress.console.print(f"{a} x {b} x {c}")
           # Compute a shift to avoid pverflow for veru big matrices
           m = int(np.log2(np.max([a,b,c])))
           dims.append(a)
           dims.append(b)
           dims.append(c)
           # Shift is part of parameters
           if format == Tools.Q31 or format == Tools.Q15 or format == Tools.Q7:
              dims.append(m)
    
           # Real matrixes
           ma = np.copy(data1[0:a*b]).reshape(a,b)
           mb = np.copy(data2[0:b*c]).reshape(b,c)
           #if nb == 0 and format == Tools.F32:
           #      np.savetxt('ma.txt', ma, delimiter=',')   # X is an array
           #      np.savetxt('mb.txt', mb.T, delimiter=',')   # X is an array
           r = np.matmul(ma , mb) 
           r = list(r.reshape(a*c))
           vals = vals + r

           # Complex matrixes
           ma = np.copy(data1C[0:a*b]).reshape(a,b)
           mb = np.copy(data2C[0:b*c]).reshape(b,c)
           #if nb == 0 and format == Tools.F32:
           #      np.savetxt('ma.txt', ma, delimiter=',')   # X is an array
           #      np.savetxt('mb.txt', mb.T, delimiter=',')   # X is an array
           r = np.matmul(ma , mb) 
           r = list(asReal(r.reshape(a*c)))
           valsC = valsC + r

           # Complex times real
           ma = np.copy(data1C[0:a*b]).reshape(a,b)
           mb = np.copy(data2[0:b*c]).reshape(b,c)
           #if nb == 0 and format == Tools.F32:
           #      np.savetxt('ma.txt', ma, delimiter=',')   # X is an array
           #      np.savetxt('mb.txt', mb.T, delimiter=',')   # X is an array
           r = np.matmul(ma , mb) 
           r = list(asReal(r.reshape(a*c)))
           valsCR = valsCR + r



           nb = nb + 1
    
    config.writeInputS16(1, dims,"DimsBinary")
    config.writeReference(1, vals,"RefMul")

    config.setOverwrite(True)
    config.writeReference(1, valsC,"RefCmplxMul")
    config.writeReference(1, valsCR,"RefCmplxRealMul")
    config.setOverwrite(False)

    dims=[] 
    vals=[]
    valsC=[]
    valsCR=[]


    # Limited sizes for the mult_fast version (less accuracy)
    # or the normal version (q7) when no fast version is provided and the
    # normal version is not accurate enough for big matrixes.
    if format == Tools.Q15 or format == Tools.Q7:
       with Progress() as progress:
           for (a,b,c) in progress.track(limited_binarySizes, description=desc):
              progress.console.print(f"{a} x {b} x {c}")
              m = int(np.log2(np.max([a,b,c])))
              dims.append(a)
              dims.append(b)
              dims.append(c)
              if format == Tools.Q31 or format == Tools.Q15 or format == Tools.Q7:
                 dims.append(m)
       
              # Real matrixes
              ma = np.copy(data1[0:a*b]).reshape(a,b)
              mb = np.copy(data2[0:b*c]).reshape(b,c)
              r = np.matmul(ma , mb) 
              r = list(r.reshape(a*c))
              vals = vals + r

              # Complex matrixes
              ma = np.copy(data1C[0:a*b]).reshape(a,b)
              mb = np.copy(data2C[0:b*c]).reshape(b,c)
              r = np.matmul(ma , mb) 
              r = list(asReal(r.reshape(a*c)))
              valsC = valsC + r

              # Complex times real matrixes
              ma = np.copy(data1C[0:a*b]).reshape(a,b)
              mb = np.copy(data2[0:b*c]).reshape(b,c)
              r = np.matmul(ma , mb) 
              r = list(asReal(r.reshape(a*c)))
              valsCR = valsCR + r
       
       
       config.writeInputS16(1, dims,"DimsLimitedBinary")
       config.writeReference(1, vals,"RefLimitedMul")

       config.setOverwrite(True)
       config.writeReference(1, valsC,"RefCmplxLimitedMul")
       config.writeReference(1, valsCR,"RefCmplxRealLimitedMul")
       config.setOverwrite(False)

    

def generatePatterns():
   
    PATTERNBINDIR = os.path.join("Patterns","DSP","MatrixNeon","Binary","Binary")
    PARAMBINDIR = os.path.join("Parameters","DSP","MatrixNeon","Binary","Binary")
    
    configBinaryf64=Tools.Config(PATTERNBINDIR,PARAMBINDIR,"f64")
    configBinaryf32=Tools.Config(PATTERNBINDIR,PARAMBINDIR,"f32")
    configBinaryf16=Tools.Config(PATTERNBINDIR,PARAMBINDIR,"f16")
    configBinaryq31=Tools.Config(PATTERNBINDIR,PARAMBINDIR,"q31")
    configBinaryq15=Tools.Config(PATTERNBINDIR,PARAMBINDIR,"q15")
    configBinaryq7=Tools.Config(PATTERNBINDIR,PARAMBINDIR,"q7")

    configBinaryf64.setOverwrite(False)
    configBinaryf32.setOverwrite(False)
    configBinaryf16.setOverwrite(False)
    configBinaryq31.setOverwrite(False)
    configBinaryq15.setOverwrite(False)
    configBinaryq7.setOverwrite(False)

    writeBinaryTests(configBinaryf64,Tools.F64,"F64")
    writeBinaryTests(configBinaryf32,Tools.F32,"F32")
    writeBinaryTests(configBinaryf16,Tools.F16,"F16")
    writeBinaryTests(configBinaryq31,Tools.Q31,"Q31")
    writeBinaryTests(configBinaryq15,Tools.Q15,"Q15")
    writeBinaryTests(configBinaryq7,Tools.Q7,"Q7")

    
if __name__ == '__main__':
   generatePatterns()