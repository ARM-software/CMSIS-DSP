import cmsisdsp as dsp
import cmsisdsp.datatype as dt


DT=[dt.F64,dt.F32, dt.F16, dt.Q31, dt.Q15]
ARCH=[dsp.ARM_MATH_SCALAR_ARCH, 
      dsp.ARM_MATH_DSP_EXTENSIONS_ARCH,
      dsp.ARM_MATH_HELIUM_ARCH,
      dsp.ARM_MATH_NEON_ARCH]
NBSAMPLES=100

def arch_to_str(arch):
    if arch == dsp.ARM_MATH_SCALAR_ARCH:
        return "SCALAR"
    elif arch == dsp.ARM_MATH_DSP_EXTENSIONS_ARCH:
        return "DSP EXTENSIONS"
    elif arch == dsp.ARM_MATH_HELIUM_ARCH:
        return "HELIUM"
    elif arch == dsp.ARM_MATH_NEON_ARCH:
        return "NEON"
    else:
        raise ValueError("Unknown architecture")
    
HEADER_DELTA=4

def spacer(s):
    spacer = f" :{'-'*(len(s)-HEADER_DELTA)}: "
    return spacer
    
def dt_to_str(d):
    if d == dt.F64:
        return "F64"
    if d == dt.F32:
        return "F32"
    elif d == dt.F16:
        return "F16"
    elif d == dt.Q31:
        return "Q31"
    elif d == dt.Q15:
        return "Q15"
    else:
        raise ValueError("Unknown datatype")


def _joinit(iterable, delimiter):
    try:
        it = iter(iterable)
        yield next(it)
        for x in it:
            yield delimiter
            yield x
    except StopIteration:
        return
    
def joinit(iterable, delimiter):
    return ''.join(_joinit(iterable, delimiter))

def arch_header():
    res=[""," Datatype / Architecture  "]
    for arch in ARCH:
        res.append(f" {arch_to_str(arch)}   ")
    res.append("")

    print(joinit(res,"|"))

    res2=[]
    nbs = []
    for s in res:
        if s:
          nbs.append(len(s)-3)
          res2.append(spacer(s))
    res2.append("")
    res2 = [""] + res2
    print(joinit(res2,"|"))
    return(nbs)
    


def gen_buffer_size(name,f,comment=None):
    print(f"### {name}\n")
    if comment is not None:
        print(f"{comment}\n")
    nbs=arch_header()

    for d in DT:
        
        n = dt_to_str(d)
        spc = " "*(nbs[0]-len(n)+1)
        res = [f" {n} {spc}"]

        for arch,nb in zip(ARCH,nbs[1:]):
            n = f(d,arch)
            if n == 0:
                n = ""
            elif n < 0:
                 n = "X"
            elif n == 100:
                n="N"
            elif n == 200:
                n="2N"
            elif n == 102:
                n="N+2"
            else:
                n="?"
                
            spc = " "*(nb-len(n)+1)
            res.append(f" {n} {spc}")
        res.append("")
        res = [""] + res
        print(joinit(res,"|"))
    print("")

print("Transform buffer sizes {#transformbuffers}")
print("======================")
print("")
print("An empty entry in a table means 'Not Applicable'.")
print("For instance, no temporary buffer is needed for a specific configuration.\n")
print("'X' means configuration is not supported.\n")
print("All returned lengths are in real numbers. The number of input samples is the transform length.\n")
print("")
print("Several functions are available in the C and Python API to compute the buffer sizes at runtime.")

print("## CFFT\n")
print("CFFT of length N\n")

f = lambda d,arch: dsp.arm_cfft_tmp_buffer_size(d,NBSAMPLES,1,arch=arch)
gen_buffer_size("CFFT Temporary Buffer Size",f)

f = lambda d,arch: dsp.arm_cfft_output_buffer_size(d,NBSAMPLES,arch=arch)
gen_buffer_size("CFFT Output Buffer Size",f)

f = lambda d,arch: dsp.arm_cifft_output_buffer_size(d,NBSAMPLES,arch=arch)
gen_buffer_size("CIFFT Output Buffer Size",f)

print("## RFFT\n")

print("RFFT of length N\n")

f = lambda d,arch: dsp.arm_rfft_tmp_buffer_size(d,NBSAMPLES,1,arch=arch)
gen_buffer_size("RFFT/RIFFT Temporary Buffer Size",f,comment="Apply to both RFFT and RIFFT")

f = lambda d,arch: dsp.arm_rfft_output_buffer_size(d,NBSAMPLES,arch=arch)
gen_buffer_size("RFFT Output Buffer Size",f)

f = lambda d,arch: dsp.arm_rifft_input_buffer_size(d,NBSAMPLES,arch=arch)
gen_buffer_size("RIFFT Input Buffer Size",f)

print("## MFCC (CFFT based)\n")
print("MFCC of length N\n")

f = lambda d,arch: dsp.arm_mfcc_tmp_buffer_size(d,NBSAMPLES,1,arch=arch,use_cfft=1)
gen_buffer_size("MFCC Temporary Buffer Size 1",f)

f = lambda d,arch: dsp.arm_mfcc_tmp_buffer_size(d,NBSAMPLES,2,arch=arch,use_cfft=1)
gen_buffer_size("MFCC Temporary Buffer Size 2",f)

print("## MFCC (RFFT based)\n")
print("MFCC of length N\n")

f = lambda d,arch: dsp.arm_mfcc_tmp_buffer_size(d,NBSAMPLES,1,arch=arch,use_cfft=0)
gen_buffer_size("MFCC Temporary Buffer Size 1",f)

f = lambda d,arch: dsp.arm_mfcc_tmp_buffer_size(d,NBSAMPLES,2,arch=arch,use_cfft=0)
gen_buffer_size("MFCC Temporary Buffer Size 2",f)