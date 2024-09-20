import numpy as np
import math
import argparse
import sys
import struct

sys.path.append("PatternGeneration")

import Tools

# Force to f16 value
def f16(x):
    return struct.unpack('<e', struct.pack('<e', x))[0]

# Force to f32 value
def f32(x):
    return struct.unpack('<f', struct.pack('<f', x))[0]

parser = argparse.ArgumentParser(description='Generate C arrays')
parser.add_argument('-f', nargs='?',type = str, default="../Source/CommonTables/arm_neon_tables.c", help="C File path")
parser.add_argument('-f16', nargs='?',type = str, default="../Source/CommonTables/arm_neon_tables_f16.c", help="C File path")
parser.add_argument('-he', nargs='?',type = str, default="../Include/arm_neon_tables.h", help="H File path")
parser.add_argument('-he16', nargs='?',type = str, default="../Include/arm_neon_tables_f16.h", help="H File path")

args = parser.parse_args()

COLLIM = 80 

condition="""#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_TWIDDLECOEF_%s_%d) || defined(ARM_TABLE_TWIDDLECOEF_%s_%d)
"""

F32 = 1
F16 = 2
Q31 = 3
Q15 = 4
Q7  = 5

def printCUInt32Array(f,name,arr):
    nb = 0
    print("const uint32_t %s[%d]={" % (name,len(arr)),file=f)

    for d in arr:
        val = "%d," % d
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printCFloat32Array(f,name,arr):
    nb = 0
    print("const float32_t %s[%d]={" % (name,len(arr)),file=f)

    for d in arr:
        val = "%.20ff," % f32(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printCFloat16Array(f,name,arr):
    nb = 0
    print("const float16_t %s[%d]={" % (name,len(arr)),file=f)

    for d in arr:
        val = "(float16_t)%.13ff," % f16(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printCQ31Array(f,name,arr):
    nb = 0
    print("const q31_t %s[%d]={" % (name,len(arr)),file=f)

    for d in arr:
        val = "%s," % Tools.to_q31(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printCQ15Array(f,name,arr):
    nb = 0
    print("const q15_t %s[%d]={" % (name,len(arr)),file=f)

    for d in arr:
        val = "%s," % Tools.to_q15(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printCQ7Array(f,name,arr):
    nb = 0
    print("const q7_t %s[%d]={" % (name,len(arr)),file=f)

    for d in arr:
        val = "%s," % Tools.to_q7(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printHUInt32Array(f,name,arr):
 print("extern const uint32_t %s[%d];" % (name,len(arr)),file=f)

def printHFloat32Array(f,name,arr):
 print("extern const float32_t %s[%d];" % (name,len(arr)),file=f)

def printHFloat16Array(f,name,arr):
 print("extern const float16_t %s[%d];" % (name,len(arr)),file=f)

def printHQ31Array(f,name,arr):
 print("extern const q31_t %s[%d];" % (name,len(arr)),file=f)

def printHQ15Array(f,name,arr):
 print("extern const q15_t %s[%d];" % (name,len(arr)),file=f)

def printHQ7Array(f,name,arr):
 print("extern const q7_t %s[%d];" % (name,len(arr)),file=f)

def twiddle(n):
    a=2.0*math.pi*np.linspace(0,n,num=n,endpoint=False)/n
    c=np.cos(a)
    s=np.sin(a)

    r = np.empty((c.size + s.size,), dtype=c.dtype)
    r[0::2] = c
    r[1::2] = s
    return(r)

def radix8_group(i,mod,n):
   yield ((i))
   yield ((i+n*mod))
   yield ((i+n*mod*2))
   yield ((i+n*mod*3))
   return (i + n*mod * 3)
   
def radix8_butterfly_f32(fftLength,mod):
   n2 = fftLength >> 3
   while (n2 > 7):
    #yield "S"
    
    ia1 = 0
    ia2 = 0
    ia3 = 0
    ia4 = 0
    ia5 = 0
    ia6 = 0
    ia7 = 0

    block = (n2-1)>>2 
    remaining = (n2-1)&3
    print(f"(n2-1) = {n2-1}, block = {block}, remaining = {remaining}")
    #block = 0 
    #remaining = n2-1
    for _ in range(block):
       #yield "B"
       ia1 = ia1 + 1*mod 
       ia2 = ia2 + 2*mod 
       ia3 = ia3 + 3*mod 
       ia4 = ia4 + 4*mod 
       ia5 = ia5 + 5*mod 
       ia6 = ia6 + 6*mod 
       ia7 = ia7 + 7*mod 
       
       ia4 = yield from radix8_group(ia4,mod,4)
       ia2 = yield from radix8_group(ia2,mod,2)
       ia6 = yield from radix8_group(ia6,mod,6)
       ia1 = yield from radix8_group(ia1,mod,1)
       ia7 = yield from radix8_group(ia7,mod,7)
       ia5 = yield from radix8_group(ia5,mod,5)
       ia3 = yield from radix8_group(ia3,mod,3)

      

    for _ in range(remaining):
       #yield "E"
       ia1 = ia1 + 1*mod 
       ia2 = ia2 + 2*mod 
       ia3 = ia3 + 3*mod 
       ia4 = ia4 + 4*mod 
       ia5 = ia5 + 5*mod 
       ia6 = ia6 + 6*mod 
       ia7 = ia7 + 7*mod 
       # With this order we do not need to load all
       # twidle but only when they are needed
       yield (ia4)
       yield (ia2)
       yield (ia6)
       yield (ia1)
       yield (ia7)
       yield (ia5)
       yield (ia3)
   
    mod = mod << 3
    n2 = n2 >> 3 

def radix8by2_f32(fftLength):
   L = fftLength >> 1 
   tw = 0
   for _ in range(fftLength>>3):
      yield tw 
      tw = tw + 1
      yield tw 
      tw = tw + 1

   yield from radix8_butterfly_f32(L,2)

def radix8by4_f32(fftLength):
   L = fftLength >> 2
   mod2 = 1
   mod3 = 2 
   mod4 = 3
   tw2 = mod2
   tw3 = mod3
   tw4 = mod4

   for _ in range((fftLength>>3) - 1):
      yield tw2 
      tw2 = tw2 + mod2

      yield tw3
      tw3 = tw3 + mod3

      yield tw4
      tw4 = tw4 + mod4

   yield tw2 
   yield tw3
   yield tw4 
   yield from radix8_butterfly_f32(L,4)

      



def reorderTwiddle(theType,conjugate,f,h,n):
    numStages = 6
    coefs= twiddle(n)
    indices = np.array([])
    if theType == F32:
       if n in [32,256,2048]:
          indices = np.array(list(radix8by4_f32(n)) )
       elif n in [64,512,4096]:
          indices = np.array(list(radix8_butterfly_f32(n,1)))
       elif n in [16,128,1024]:
          indices = np.array(list(radix8by2_f32(n)))
       else:
          print(f"n= {n} not supported")
          exit(1)

       #print(indices)
       cplx_indices = np.zeros(len(indices)*2,dtype=np.int32)
       cplx_indices[0::2] = 2*indices
       cplx_indices[1::2] = 2*indices+1
       res=coefs[cplx_indices]
       printCFloat32Array(f,"twiddle_neon_%d_f32" % n,res)
       printHFloat32Array(h,"twiddle_neon_%d_f32" % n,res) 
    #else:
    #   if theType == F16:
    #      printCFloat16Array(f,"twiddle_neon_%d_f16" % n,[])
    #      printHFloat16Array(h,"twiddle_neon_%d_f16" % n,[]) 
    #
    #   if theType == Q31:
    #      printCQ31Array(f,"twiddle_neon_%d_q31" % n,[])
    #      printHQ31Array(h,"twiddle_neon_%d_q31" % n,[]) 
    #
    #   if theType == Q15:
    #      printCQ15Array(f,"twiddle_neon_%d_q15" % n,[])
    #      printHQ15Array(h,"twiddle_neon_%d_q15" % n,[]) 
   





#test = twiddle(16)
#printCFloat32Array("Test",list(test))

cheader="""/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_neon_tables%s.c
 * Description:  common tables like fft twiddle factors, Bitreverse, reciprocal etc
 *               used for Neon implementation only
 *
 * @date     18 September 2024
 *
 * Target Processor: Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2024 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 #include "arm_math_types%s.h"

 """ 

cifdeNEON="""

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

"""

cfooterNEON="""

#endif /* defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE) */
"""


hheader="""/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_neon_tables%s.h
 * Description:  common tables like fft twiddle factors, Bitreverse, reciprocal etc
 *               used for Neon implementation only
 *
 * @version  V1.10.0
 * @date     28 September 2024
 *
 * Target Processor: Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2024 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 #ifndef _ARM_NEON_TABLES_%sH
 #define _ARM_NEON_TABLES_%sH

#include "arm_math_types%s.h"

#ifdef   __cplusplus
extern "C"
{
#endif


 """ 

hifdefNEON="""
#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

"""

hfooterNEON="""

#endif /* defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE) */

"""



hfooter="""
#ifdef   __cplusplus
}
#endif

#endif /*_ARM_NEON_TABLES_%sH*/
"""

with open(args.f16,'w') as f:
  with open(args.he16,'w') as h:
     print(cheader % ("_f16","_f16"),file=f)
     print(hheader % ("_f16","F16_","F16_","_f16"),file=h)


     print(cifdeNEON ,file=f)
     print(hifdefNEON,file=h)

     print("#if defined(ARM_FLOAT16_SUPPORTED)",file=f)
     print("#if defined(ARM_FLOAT16_SUPPORTED)",file=h)

     reorderTwiddle(F16,False,f,h,16)
     reorderTwiddle(F16,False,f,h,32)
     reorderTwiddle(F16,False,f,h,64)
     reorderTwiddle(F16,False,f,h,128)
     reorderTwiddle(F16,False,f,h,256)
     reorderTwiddle(F16,False,f,h,512)
     reorderTwiddle(F16,False,f,h,1024)
     reorderTwiddle(F16,False,f,h,2048)
     reorderTwiddle(F16,False,f,h,4096)

     print("#endif /* if defined(ARM_FLOAT16_SUPPORTED) */",file=f)
     print("#endif /* if defined(ARM_FLOAT16_SUPPORTED) */",file=h)

     print(cfooterNEON ,file=f)
     print(hfooterNEON ,file=h)


     print(hfooter % "F16_",file=h)

with open(args.f,'w') as f:
  with open(args.he,'w') as h:
     print(cheader % ("",""),file=f)
     print(hheader % ("","","",""),file=h)

    
     print(cifdeNEON ,file=f)
     print(hifdefNEON ,file=h)
     reorderTwiddle(F32,False,f,h,16)
     reorderTwiddle(F32,False,f,h,32)
     reorderTwiddle(F32,False,f,h,64)
     reorderTwiddle(F32,False,f,h,128)
     reorderTwiddle(F32,False,f,h,256)
     reorderTwiddle(F32,False,f,h,512)
     reorderTwiddle(F32,False,f,h,1024)
     reorderTwiddle(F32,False,f,h,2048)
     reorderTwiddle(F32,False,f,h,4096)

     reorderTwiddle(Q31,False,f,h,16)
     reorderTwiddle(Q31,False,f,h,32)
     reorderTwiddle(Q31,False,f,h,64)
     reorderTwiddle(Q31,False,f,h,128)
     reorderTwiddle(Q31,False,f,h,256)
     reorderTwiddle(Q31,False,f,h,512)
     reorderTwiddle(Q31,False,f,h,1024)
     reorderTwiddle(Q31,False,f,h,2048)
     reorderTwiddle(Q31,False,f,h,4096)

     reorderTwiddle(Q15,False,f,h,16)
     reorderTwiddle(Q15,False,f,h,32)
     reorderTwiddle(Q15,False,f,h,64)
     reorderTwiddle(Q15,False,f,h,128)
     reorderTwiddle(Q15,False,f,h,256)
     reorderTwiddle(Q15,False,f,h,512)
     reorderTwiddle(Q15,False,f,h,1024)
     reorderTwiddle(Q15,False,f,h,2048)
     reorderTwiddle(Q15,False,f,h,4096)
 

     print(cfooterNEON ,file=f)
     print(hfooterNEON ,file=h)

     
     print(hfooter % "",file=h)
