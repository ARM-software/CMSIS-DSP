import numpy as np
import math
import argparse
import sys
import struct

# Force to f16 value
def f16(x):
    return struct.unpack('<e', struct.pack('<e', x))[0]

# Force to f32 value
def f32(x):
    return struct.unpack('<f', struct.pack('<f', x))[0]

parser = argparse.ArgumentParser(description='Generate C arrays')
parser.add_argument('-n', nargs='?',type = int, default=32, help="FFT size")

args = parser.parse_args()

COLLIM = 80 


def printCFloat32Array(arr):
    nb = 0
    fftLen2 = len(arr)
    fftLen = fftLen2 >> 1
    print(f"const float32_t twiddleCoef_{fftLen}[{fftLen2}]={{")

    for d in arr:
        val = "%.20ff," % f32(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("")
            nb = len(val)
        print(val,end="")

    print("};\n")

def printCFloat16Array(arr):
    nb = 0
    fftLen2 = len(arr)
    fftLen = fftLen2 >> 1
    print(f"const float16_t twiddleCoefF16_{fftLen}[{fftLen2}]={{")

    for d in arr:
        val = "(float16_t)%.13ff," % f16(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("")
            nb = len(val)
        print(val,end="")

    print("};\n")


def twiddle(n):
    a=2.0*math.pi*np.linspace(0,n,num=n,endpoint=False)/n
    c=np.cos(a)
    s=np.sin(a)

    r = np.empty((c.size + s.size,), dtype=c.dtype)
    r[0::2] = c
    r[1::2] = s
    return(r)

res=twiddle(args.n)
printCFloat32Array(res)

#printCFloat16Array(res)