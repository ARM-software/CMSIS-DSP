# Testing Neon matrix multiply on VERY long sequence of tests
# It is not called by the testall.py script because it is
# an additional test for testing the C using Python as an help.
# It is not a PythonWrapper test.
import itertools
import os
import numpy as np
from rich.progress import track
from rich.table import Column
from rich.progress import Progress, BarColumn, TextColumn
from testtools import *
TAILONLY = 1
BODYONLY = 2
BODYANDTAIL = 3

# Datatype formats
F64 = 64 
F32 = 0
F16 = 16
Q63 = 63 
Q31 = 31 
Q15 = 15
Q7 = 7

def nblane(format):
    nb = 0
    if format == F64:
        nb=2
    if format == F32:
        nb=4
    if format == F16:
        nb=8
    if format == Q63:
        nb=2
    if format == Q31:
        nb=4
    if format == Q15:
        nb=8
    if format == Q7:
        nb=16
    return(nb)
   
def loopnb(format,loopkind):
    nb = 0
    if loopkind == TAILONLY:
        if format == 64 or format == Q63:
            nb = 2 
        if format == 0 or format == 31:
            nb = 3 
        if format == 15 or format == 16:
            nb = 7
        if format == 7:
            nb = 15
    if loopkind == BODYONLY:
        if format == 64 or format == Q63:
            nb = 4 
        if format == 0 or format == 31:
            nb = 8 
        if format == 15 or format == 16:
            nb = 16
        if format == 7:
            nb = 32
    if loopkind == BODYANDTAIL:
        if format == 64 or format == Q63:
            nb = 5
        if format == 0 or format == 31:
            nb = 11 # 9
        if format == 15 or format == 16:
            nb = 23 # 17
        if format == 7:
            nb = 47 # 33

    return(nb)

def cartesian(*somelists):
   r=[]
   for element in itertools.product(*somelists):
       r.append(element)
   return(r)

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

format = F32

col_block=768
row_block=256
inner_block=128

BLOCK_COLS=[0,col_block,2*col_block]
BLOCK_ROWS=[0,row_block,2*row_block]
BLOCK_INNER=[0,inner_block,2*inner_block]


DR = 4
DC = 4

drs = [1,2,3,4,8,9,10,11]
dcs = [1,2,3,4,8,9]





def test_format(format):
    sizes=[loopnb(format,TAILONLY),
           loopnb(format,BODYONLY),
           loopnb(format,BODYANDTAIL)
           ]
    nl = nblane(format)

    rows = sum(remove_zero(cartesian(drs,BLOCK_ROWS,sizes)))
    rows = list(set(rows))
    rows.sort()
    
    cols = sum(remove_zero(cartesian(dcs,BLOCK_COLS,sizes)))
    cols = list(set(cols))
    cols.sort()
    
    inners = sum(remove_zero(cartesian(BLOCK_INNER,sizes)))
    inners = list(set(inners))
    inners.sort()
    
    #print(rows)
    #print(inners)
    #print(cols)
    
    
    kernels = cartesian([1,2,3,4],list(range(1,nl))+[nl*1,nl*2,nl*3,nl*4])
    kernels_dims=[(x[0],32,x[1]) for x in kernels]
    
    all_dims = kernels_dims + cartesian(rows,cols,inners)

    print(f"Nb tests = {len(all_dims)}")

    rng = np.random.default_rng(seed=10)

    with Progress() as progress:
        for (a,b,c) in progress.track(all_dims):
            l=rng.standard_normal((a,b))
            r=rng.standard_normal((b,c))
    
            l = normalize(l)
            r = normalize(r)


            err,res = dsp.arm_mat_mult_f32(l,r)
            if (err != 0):
                print(f"Status error in {a}x{b}x{c}")
                exit(0)
            ref = np.dot(l,r)
            if not np.allclose(res,ref,rtol=1e-5,atol=1e-5):
                print(f"Error in {a}x{b}x{c}")
                print(np.max(np.abs(res-ref)))
                exit(0)


            



test_format(F32)