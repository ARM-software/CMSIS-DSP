import numpy as np
import math
import cmath
import argparse
import sys
import struct

sys.path.append("PatternGeneration")

import Tools

# Force to f16 value
def f16(x):
    return struct.unpack('<e', struct.pack('<e', np.half(x)))[0]

# Force to f32 value
def f32(x):
    return struct.unpack('<f', struct.pack('<f', np.float32(x)))[0]

parser = argparse.ArgumentParser(description='Generate C arrays')
parser.add_argument('-f', nargs='?',type = str, default="../Source/CommonTables/arm_neon_tables.c", help="C File path")
parser.add_argument('-f16', nargs='?',type = str, default="../Source/CommonTables/arm_neon_tables_f16.c", help="C File path")
parser.add_argument('-he', nargs='?',type = str, default="../Include/arm_neon_tables.h", help="H File path")
parser.add_argument('-he16', nargs='?',type = str, default="../Include/arm_neon_tables_f16.h", help="H File path")

args = parser.parse_args()

COLLIM = 80 

F32 = 1
F16 = 2
Q31 = 3
Q15 = 4
Q7  = 5

def float_to_hex(f):
    """ Convert and x86 float to an ARM unsigned long int.
  
    Args:
      f (float): value to be converted
    Raises:
      Nothing 
    Returns:
      str : representation of the hex value
    """
    return hex(struct.unpack('<I', struct.pack('<f', f))[0])


def printCUInt32Array(f,name,arr):
    nb = 0
    if (len(arr)==0):
        print(f"const uint32_t *{name}=NULL;\n",file=f)
        return
    print(f"const uint32_t {name}[{name.upper()}_LEN]={{",file=f)

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
    if (len(arr)==0):
        print(f"const float32_t *{name}=NULL;\n",file=f)
        return
    print(f"__ALIGNED(16) const float32_t {name}[{name.upper()}_LEN]={{",file=f)

    for d in arr:
        val = "%.20ff," % f32(d)
        #val=f"(float32_t){float_to_hex(float(d))},"
        #val=f"{float.hex(f32(d))},"
        #val=f"{str(repr(f32(d)))}f,"
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printCFloat16Array(f,name,arr):
    nb = 0
    if (len(arr)==0):
        print(f"const float16_t *{name}=NULL;\n",file=f)
        return
    print(f"__ALIGNED(16) const float16_t {name}[{name.upper()}_LEN]={{",file=f)

    for d in arr:
        val = "(float16_t)%.13ff," % f16(d)
        #val = f"(float16_t){f16(d).hex()},"
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def printCQ31Array(f,name,arr):
    nb = 0
    if (len(arr)==0):
        print(f"const q31_t *{name}=NULL;\n",file=f)
        return
    print(f"__ALIGNED(16) const q31_t {name}[{name.upper()}_LEN]={{",file=f)

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
    if (len(arr)==0):
        print(f"const q15_t *{name}=NULL;\n",file=f)
        return
    print(f"__ALIGNED(16) const q15_t {name}[{name.upper()}_LEN]={{",file=f)

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
    if (len(arr)==0):
        print(f"const q7_t *{name}=NULL;\n",file=f)
        return
    print(f"__ALIGNED(16) const q7_t {name}[{name.upper()}_LEN]={{",file=f)

    for d in arr:
        val = "%s," % Tools.to_q7(d)
        nb = nb + len(val)
        if nb > COLLIM:
            print("",file=f)
            nb = len(val)
        print(val,end="",file=f)

    print("};\n",file=f)

def type_to_ext(theType):
    if theType == F32:
        ext="f32"
    if theType == F16:
        ext="f16"
    if theType == Q31:
        ext="q31"
    if theType == Q15:
        ext="q15"
    return ext

def write_const(theType,h,n,name,val):
    ext = type_to_ext(theType)
    print(f"#define {name}_{n}_{ext.upper()} {val}",file=h)

def printHUInt32Array(f,name,arr):
    print(f"#define {name.upper()}_LEN {len(arr)}",file=h)
    if (len(arr)==0):
        print(f"extern const uint32_t *{name};\n",file=f)
        return
    print(f"extern const uint32_t {name}[{name.upper()}_LEN];",file=f)

def printHFloat32Array(f,name,arr):
    print(f"#define {name.upper()}_LEN {len(arr)}",file=h)
    if (len(arr)==0):
        print(f"extern const float32_t *{name};\n",file=f)
        return
    print(f"extern const float32_t {name}[{name.upper()}_LEN];",file=f)

def printHFloat16Array(f,name,arr):
    print(f"#define {name.upper()}_LEN {len(arr)}",file=h)
    if (len(arr)==0):
        print(f"extern const float16_t *{name};\n",file=f)
        return
    print(f"extern const float16_t {name}[{name.upper()}_LEN];",file=f)

def printHQ31Array(f,name,arr):
    print(f"#define {name.upper()}_LEN {len(arr)}",file=h)
    if (len(arr)==0):
        print(f"extern const q31_t *{name};\n",file=f)
        return
    print(f"extern const q31_t {name}[{name.upper()}_LEN];",file=f)

def printHQ15Array(f,name,arr):
    print(f"#define {name.upper()}_LEN {len(arr)}",file=h)
    if (len(arr)==0):
        print(f"extern const q15_t *{name};\n",file=f)
        return
    print(f"extern const q15_t {name}[{name.upper()}_LEN];",file=f)

def printHQ7Array(f,name,arr):
    print(f"#define {name.upper()}_LEN {len(arr)}",file=h)
    if (len(arr)==0):
        print(f"extern const q7_t *{name};\n",file=f)
        return
    print(f"extern const q7_t {name}[{name.upper()}_LEN];",file=f)


NE10_FFT_PARA_LEVEL = 4
NE10_FACTOR_DEFAULT=0
NE10_FACTOR_EIGHT_FIRST_STAGE=1
NE10_FACTOR_EIGHT=2

SIZES=[16,32,64,128,256,512,1024,2048,4096]

SIZES_RFFT_Q31=[16,32,64,128,256,512,1024,2048,4096,8192]
SIZES_RFFT_Q15=[16,32,64,128,256,512,1024,2048,4096,8192]

def factors(nfft,factor_height_first_stage=True):
    n = nfft
    i = 1
    stage_num = 0
    stride_max = n
    p = 0
    factors = []

    done = False
    while (n>1):
        if factor_height_first_stage and (n==8):
           p = 8
        elif (n % 4) == 0:
            p = 4 
        elif (n % 2) == 0:
            p = 2
        else:
            print(f"Error factoring {nfft}. Error at {n}")
            exit(1)
    
        n = n // p
        factors.append({"f":p,"n":n})
        i = i + 1
        stage_num = stage_num + 1 

    # Update for neon

    r = {"stage_count":stage_num,
         "f_stride":(stride_max // p),
         "factors":factors}
    
    return r



def gen_coefs(twiddles,idx,mstride,fstride,radix,nfft):
    #print(f"{mstride} {fstride} {radix} {nfft}")
    for j in range(mstride):
        for k in range(1,radix):
            phase = (-2.0 * math.pi * fstride * k * j / nfft);
            twiddles[idx + mstride * (k - 1) + j]= complex((math.cos(phase)) , (math.sin(phase)))

def gen_transposed_coefs(twiddles,idx,mstride,fstride,radix,nfft):
    #print(f"{mstride} {fstride} {radix} {nfft}")
    for j in range(mstride):
        for k in range(1,radix):
            phase = (-2.0 * math.pi * fstride * k * j / nfft);
            twiddles[idx + (radix - 1) * j + k - 1]= complex((math.cos(phase)) , (math.sin(phase)))


def genTwiddles(theType,f,nfft,transposed=False,firstStage=True):
    if transposed:
        gen = gen_transposed_coefs
    else:
        gen = gen_coefs
    cur_radix = f["factors"][-1]["f"]
    twiddles = np.zeros(nfft,dtype=complex)

    idx = 0;
    fstride = f["f_stride"]
    if firstStage and cur_radix % 2 != 0:
        if theType == F32 or theType == F16:
           twiddles[0] = 1
        idx = idx + 1
        gen(twiddles,idx,1, fstride, cur_radix, nfft)
        idx = idx + (cur_radix - 1)

    #print(f"cur={cur_radix}")
    for stage in reversed(f["factors"][:-1]):
        cur_radix = stage["f"]
        fstride = fstride // cur_radix;
        mstride = stage["n"]
        gen (twiddles, idx,mstride, fstride, cur_radix, nfft)
        idx = idx +  mstride * (cur_radix - 1)

    return idx,twiddles[:idx]

def superTwiddle(theType,nfft):
    maxidx = 0
    twiddles = np.zeros((nfft//32)*3*4+12,dtype=complex)
    for i in range(1,4):
        for j in range(4):
            phase = (-2.0 * math.pi * i * j / nfft);
            twiddles[4*i-4+j]= complex((math.cos(phase)) , (math.sin(phase)))
            if (4*i-4+j)>maxidx:
                maxidx = 4*i-4+j
            

    for k in range(1,nfft//32):
        for s in range(1,4):
            for j in range(4):
                phase = f32(-2.0 * math.pi * (k*4+j) * s / nfft);
                twiddles[12*k+j+4*(s-1)]= complex((math.cos(phase)) , (math.sin(phase)))
                if (12*k+j+4*(s-1))>maxidx:
                   maxidx = 12*k+j+4*(s-1)
                

    return twiddles[:(maxidx+1)]

def superTwiddle_int(theType,nfft):
    maxidx = 0
    twiddles = np.zeros(nfft//2,dtype=complex)
    for i in range(nfft//2):
        phase = (-1.0 * math.pi * ((i+1) / nfft+0.5));
        twiddles[i]= complex((math.cos(phase)) , (math.sin(phase)))
        if i>maxidx:
            maxidx = i
    return twiddles

def factor_desc(f):
    facts = f["factors"]
    stage_count = f["stage_count"]
    fstride = f["f_stride"]
    first_radix = facts[-1]["f"]
    if len(facts)>1:
       mstride = facts[-2]["n"]
    else:
       mstride = facts[-1]["n"]
    return {
        "stage_count" : stage_count,
        "fstride" : fstride,
        "first_radix" :first_radix,
        "mstride": mstride
    }

def c_to_r(t):
    twiddles = np.zeros(2*len(t),dtype=float)
    twiddles[0::2] = t.real
    twiddles[1::2] = t.imag
    return list(twiddles)

def computeRFFTArrays(theType,nfft):
    r_factors = factors(nfft)
    
    offset_twid,r_twiddles = genTwiddles(theType,r_factors,nfft)
    #print(r_factors)

    r_factors_neon = factors(nfft//4)
    offset_twid_neon,r_twiddles_neon = genTwiddles(theType,r_factors_neon,nfft//4,transposed=True)
    #print(r_twiddles_neon)
    #print(r_factors_neon)
    r_super_twiddles_neon = superTwiddle(theType,nfft)
    #print(r_super_twiddles_neon)
    return {
       "r_factors":factor_desc(r_factors),
       "r_twiddles":c_to_r(r_twiddles),
       "r_factors_neon":factor_desc(r_factors_neon),
       "r_twiddles_neon":c_to_r(r_twiddles_neon),
       "r_super_twiddles_neon":c_to_r(r_super_twiddles_neon),
       "offset_twid":offset_twid,
       "offset_twid_neon":offset_twid_neon
    }
    
    
def computeRFFTArrays_int(theType,nfft):
    
    ncfft = nfft // 2
    r_factors = factors(ncfft)
    offset_twid,r_twiddles = genTwiddles(theType,r_factors,ncfft,firstStage=False)
    #print(r_factors)
    
    r_super_twiddles_neon = superTwiddle_int(theType,ncfft)
    #print(r_super_twiddles_neon)
    return {
       "r_factors":factor_desc(r_factors),
       "r_twiddles":c_to_r(r_twiddles),
       "r_super_twiddles_neon":c_to_r(r_super_twiddles_neon),
       "offset_twid":offset_twid,
    }
    

def computeCFFTArrays(theType,nfft):
    f = factors(nfft // NE10_FFT_PARA_LEVEL)
    f["stage_count"] = f["stage_count"] + 1 
    f["f_stride"] = 4*f["f_stride"]
    f["factors"] = [{"f":4,"n":nfft// NE10_FFT_PARA_LEVEL}] + f["factors"]

    firstStage=True
    if theType == Q15:
        firstStage = False
    offset,c_twiddles = genTwiddles(theType,f,nfft,firstStage=firstStage)

    return{
      "factors":factor_desc(f),
      "twiddles":c_to_r(c_twiddles),
      "offset":offset
    }

#def test(nfft):
#    f = factors(nfft // NE10_FFT_PARA_LEVEL)
#    f["stage_count"] = f["stage_count"] + 1 
#    f["f_stride"] = 4*f["f_stride"]
#    f["factors"] = [{"f":4,"n":nfft// NE10_FFT_PARA_LEVEL}] + f["factors"]
#
#    facts = f["factors"]
#    stage_count = f["stage_count"]
#    fstride = f["f_stride"]
#    first_radix = facts[-1]["f"]
#    mstride = facts[-1]["n"]
#
#    print(f"{stage_count} {fstride} {first_radix} {mstride}")
#
#for s in SIZES:
#    test(s)
#
#exit(0)

def write_twiddles(theType,name,n,f,h,twiddles):
    if theType == F32:
       printCFloat32Array(f,f"arm_neon_{name}_{n}_f32",twiddles)
       printHFloat32Array(h,f"arm_neon_{name}_{n}_f32",twiddles)

    if theType == F16:
       printCFloat16Array(f,f"arm_neon_{name}_{n}_f16",twiddles)
       printHFloat16Array(h,f"arm_neon_{name}_{n}_f16",twiddles)

    if theType == Q31:
       printCQ31Array(f,f"arm_neon_{name}_{n}_q31",twiddles)
       printHQ31Array(h,f"arm_neon_{name}_{n}_q31",twiddles)

    if theType == Q15:
       printCQ15Array(f,f"arm_neon_{name}_{n}_q15",twiddles)
       printHQ15Array(h,f"arm_neon_{name}_{n}_q15",twiddles)

def write_factors(theType,name,n,f,h,factors):
    factors_list = [factors["stage_count"], 
                    factors["fstride"],
                    factors["first_radix"],
                    factors["mstride"]
                    ]

    ext = type_to_ext(theType)
    
    printCUInt32Array(f,f"arm_neon_{name}_{n}_{ext}",factors_list)
    printHUInt32Array(h,f"arm_neon_{name}_{n}_{ext}",factors_list)

def neonRFFTTwiddle(theType,f,h,n):
    
    if theType == Q31 or theType == Q15: 
        rfft = computeRFFTArrays_int(theType,n)

        write_twiddles(theType,"rfft_twiddles",n,f,h,rfft["r_twiddles"])
        write_factors(theType,"rfft_factors",n,f,h,rfft["r_factors"])
   
        write_twiddles(theType,"rfft_super_twiddles_neon",n,f,h,rfft["r_super_twiddles_neon"])

        write_const(theType,h,n,"ARM_NE10_OFFSET_BACKWARD_TWID_RFFT",rfft["offset_twid"])
        print("",file=h)

    else:
       rfft = computeRFFTArrays(theType,n)
   
       
       write_twiddles(theType,"rfft_twiddles",n,f,h,rfft["r_twiddles"])
       write_factors(theType,"rfft_factors",n,f,h,rfft["r_factors"])
   
       write_twiddles(theType,"rfft_twiddles_neon",n,f,h,rfft["r_twiddles_neon"])
       write_factors(theType,"rfft_factors_neon",n,f,h,rfft["r_factors_neon"])
   
       write_twiddles(theType,"rfft_super_twiddles_neon",n,f,h,rfft["r_super_twiddles_neon"])
   
       write_const(theType,h,n,"ARM_NE10_OFFSET_BACKWARD_TWID_RFFT",rfft["offset_twid"])
       write_const(theType,h,n,"ARM_NE10_OFFSET_BACKWARD_TWID_NEON",rfft["offset_twid_neon"])
       print("",file=h)

def neonCFFTTwiddle(theType,conjugate,f,h,n):
    
    r = computeCFFTArrays(theType,n)

    factors = r["factors"]
    twiddles = r["twiddles"]
    offset = r["offset"]
    
    write_twiddles(theType,"twiddles",n,f,h,twiddles)
    write_factors(theType,"factors",n,f,h,factors)
    write_const(theType,h,n,"ARM_NE10_OFFSET_BACKWARD_TWID",offset)
   
   
#test = twiddle(16)
#printCFloat32Array("Test",list(test))

cheader="""/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_neon_tables%s.c
 * Description:  common tables like fft twiddle factors, Bitreverse, reciprocal etc
 *               used for NEON implementation only
 *
 * @date     02 October 2024
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

#if defined(%s)

"""

cfooterNEON="""

#endif /* %s */
"""


hheader="""/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_neon_tables%s.h
 * Description:  common tables like fft twiddle factors, Bitreverse, reciprocal etc
 *               used for NEON implementation only
 *
 * @date     02 October 2024
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

 #ifndef ARM_NEON_TABLES_%sH
 #define ARM_NEON_TABLES_%sH

#include "arm_math_types%s.h"

#ifdef   __cplusplus
extern "C"
{
#endif


 """ 

hifdefNEON="""
#if %s

"""

hfooterNEON="""

#endif /* %s */

"""


hfooter="""
#ifdef   __cplusplus
}
#endif

#endif /*ARM_NEON_TABLES_%sH*/
"""




with open(args.f16,'w') as f:
  with open(args.he16,'w') as h:
     print(cheader % ("_f16","_f16"),file=f)
     print(hheader % ("_f16","F16_","F16_","_f16"),file=h)

     print("#if defined(ARM_FLOAT16_SUPPORTED)",file=f)

     print(cifdeNEON % ("ARM_MATH_NEON_FLOAT16",),file=f)
     print(hifdefNEON % ("defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)"),file=h)
     print('#include "arm_neon_tables_f16.h"',file=f)
     
     for s in SIZES:
         neonCFFTTwiddle(F16,False,f,h,s)

     for s in SIZES:
         neonRFFTTwiddle(F16,f,h,s)
     
     print(cfooterNEON % ("defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)"),file=f)
     print(hfooterNEON % ("defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED)"),file=h)

     print("#endif /* if defined(ARM_MATH_NEON_FLOAT16) && defined(ARM_FLOAT16_SUPPORTED) */",file=f)

     print(hfooter % "F16_",file=h)

with open(args.f,'w') as f:
  with open(args.he,'w') as h:
     print(cheader % ("",""),file=f)
     print(hheader % ("","","",""),file=h)

    
     print(cifdeNEON % ("ARM_MATH_NEON",),file=f)
     print(hifdefNEON % ("defined(ARM_MATH_NEON)"),file=h)
     print('#include "arm_neon_tables.h"',file=f)

     for s in SIZES:
         neonRFFTTwiddle(F32,f,h,s)

     for s in SIZES_RFFT_Q31:
         neonRFFTTwiddle(Q31,f,h,s)

     for s in SIZES_RFFT_Q15:
         neonRFFTTwiddle(Q15,f,h,s)

     for s in SIZES:
         neonCFFTTwiddle(F32,False,f,h,s)
     
     for s in SIZES:
         neonCFFTTwiddle(Q31,True,f,h,s)

     for s in SIZES:
         neonCFFTTwiddle(Q15,True,f,h,s)

    

     print(cfooterNEON % ("defined(ARM_MATH_NEON)"),file=f)
     print(hfooterNEON % ("defined(ARM_MATH_NEON)"),file=h)


     print(hfooter % "",file=h)
