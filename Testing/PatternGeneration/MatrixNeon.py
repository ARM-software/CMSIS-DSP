import Tools
import itertools
import os
import numpy as np
from rich.progress import track
from rich.table import Column
from rich.progress import Progress, BarColumn, TextColumn

def cartesian(*somelists):
   r=[]
   for element in itertools.product(*somelists):
       r.append(element)
   return(r)


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


def writeBinaryTests(config,format,desc):
    sizes=[Tools.loopnb(format,Tools.TAILONLY),
    Tools.loopnb(format,Tools.BODYONLY),
    Tools.loopnb(format,Tools.BODYANDTAIL)
    ]

    DR = 4
    DC = 4

    drs = [1,2,3,4,8,9,10,11]
    dcs = [1,2,3,4,8,9]

    #rows = sum(remove_zero(cartesian(drs,BLOCK_ROWS,sizes)))
    #cols = sum(remove_zero(cartesian(dcs,BLOCK_COLS,sizes)))
    #inners = sum(remove_zero(cartesian(BLOCK_INNER,sizes)))
    #
    OTHER = 32
    rows=[(x,OTHER,OTHER) for x in (drs+BLOCK_ROWS)]
    cols=[(OTHER,x,OTHER) for x in (dcs+BLOCK_COLS)]
    inners=[(OTHER,OTHER,x) for x in (BLOCK_INNER+sizes)]

    maxnb = np.max(np.hstack([drs,dcs,sizes,BLOCK_COLS,BLOCK_ROWS,BLOCK_INNER]))
    print(f"Max nb = {maxnb}")

    data1=np.random.randn(maxnb*maxnb)
    data2=np.random.randn(maxnb*maxnb)
    
    data1 = Tools.normalize(data1)
    data2 = Tools.normalize(data2)

    config.writeInput(1, data1,"InputA")
    config.writeInput(1, data2,"InputB")
    #
    binarySizes = sorted(rows)+sorted(inners)+sorted(cols)
    #print(len(binarySizes))
    #return
    dims=[l[0]*l[2] for l in binarySizes]
    bytes=25*np.sum(dims)
    print(f"Estimated size of result text file : {bytes} bytes")

    dims=[] 
    vals=[]


    with Progress() as progress:
        for (a,b,c) in progress.track(binarySizes, description=desc):
           progress.console.print(f"{a} x {b} x {c}")
           m = int(np.log2(np.max([a,b,c])))
           dims.append(a)
           dims.append(b)
           dims.append(c)
           if format == Tools.Q31 or format == Tools.Q15 or format == Tools.Q7:
              dims.append(m)
    
           ma = np.copy(data1[0:a*b]).reshape(a,b)
           mb = np.copy(data2[0:b*c]).reshape(b,c)
           r = np.matmul(ma , mb) 
           r = list(r.reshape(a*c))
           vals = vals + r
    
    config.writeInputS16(1, dims,"DimsBinary")
    config.writeReference(1, vals,"RefMul")

    

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

    ##writeBinaryTests(configBinaryf64,Tools.F64,"F64")
    writeBinaryTests(configBinaryf32,Tools.F32,"F32")
    writeBinaryTests(configBinaryf16,Tools.F16,"F16")
    writeBinaryTests(configBinaryq31,Tools.Q31,"Q31")
    writeBinaryTests(configBinaryq15,Tools.Q15,"Q15")
    writeBinaryTests(configBinaryq7,Tools.Q7,"Q7")

    
if __name__ == '__main__':
   generatePatterns()