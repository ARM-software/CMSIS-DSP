import os.path
import numpy as np
import itertools
import Tools
import scipy.fftpack
import argparse
import copy

# Special FFT sizes to test radix-3 and 5 only available
# for Neon implementation and CFFT F32, CFFt F16 and CFFT Q31.
parser = argparse.ArgumentParser(description='Gen Neon patterns')
parser.add_argument('-d', action='store_true', help="Gen test description")
parser.add_argument('-c', action='store_true', help="Gen C code")
parser.add_argument('others', nargs=argparse.REMAINDER)

args = parser.parse_args()


NB_RADIX_3 = 3
NB_RADIX_5 = 3

radix2 = [2**n for n in range(2,14)]
radix3 = [3**n for n in range(0,NB_RADIX_3+1)]
radix5 = [5**n for n in range(0,NB_RADIX_5+1)]

FFTSIZES = sorted(list(filter(lambda a : a > 16 and a < 5700,[int(np.prod(x)) for x in itertools.product(radix2,radix3,radix5)])))

FFTSIZES_F32 = copy.deepcopy(FFTSIZES)

FFTSIZES_F32.append(8192)
FFTSIZES_F32.append(16384)
FFTSIZES_F32.append(32768)

FFTSIZES_F32=sorted(FFTSIZES_F32)

FFTSIZES_Q15 = [2**n for n in range(5,14)]

DATATYPES = ["F32","F16","Q31","Q15"]

def fftsizes(d):
    if d == "Q15":
        return FFTSIZES_Q15
    if d == "F32":
        return FFTSIZES_F32
    return FFTSIZES

def disabled(d,l,s):
    if d == "F16" and l > 500:
        return f"disabled{{{s}}}"
    if d == "Q15" and l > 1024:
        return f"disabled{{{s}}}"
    if d != "F32" and l >= 4800:
        return f"disabled{{{s}}}"
    return s

def ifft_testname(d):
    if d == "Q15":
        return "test_cifft_"
    if d == "Q31":
        return "test_cifft_"
    return "test_cfft_"

def cifft(d):
    if d == "F32":
        return ""
    if d == "F16":
        return ""
    return f"""    void TransformNeonC{d}::test_cifft_{d.lower()}()
    {{
       const {d.lower()}_t *inp = input.ptr();

       {d.lower()}_t *outfftp = outputfft.ptr();
       {d.lower()}_t *refp = ref.ptr();

        memcpy(outfftp,inp,sizeof({d.lower()}_t)*input.nbSamples());
   
        {d.lower()}_t *tmp2p = tmp2.ptr();
        arm_cfft_{d.lower()}(
             this->varInstCfft{d}Ptr,
             inp,
             outfftp,
             tmp2p,
             this->ifft);


        for(unsigned long i=0; i < outputfft.nbSamples();i++)
        {{
          refp[i] = ({d.lower()}_t)(1.0*refp[i]/this->scaling);
        }}
          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        ASSERT_NEAR_EQ(outputfft,ref,ABS_ERROR);
        ASSERT_EMPTY_TAIL(outputfft);

       
        
    }} 
"""

def scaling_init(d,l):
    if d == "F32":
        return ""
    if d == "F16":
        return ""
    return f"this->scaling = {l};"

if args.d:

    path=os.path.join(args.others[0],f"desc_neon.txt")
    with open(path,"w") as c:
        print(f"""group Root {{
    class = Root

    group DSP Tests {{
        class = DSPTests
        folder = DSP
""",file=c)
        print(f"""        group Transform Tests {{
            class = TransformTests
            folder = TransformNeon 
""",file=c)
        for d in DATATYPES:
                print(f"            suite Transform Complex {d} {{",file=c)
                print(f"""               class = TransformNeonC{d}
               folder = TransformNeon{d}

""",file=c)
                nb = 1
                for l in fftsizes(d):
                    print(f"""               Pattern INPUTS_CFFT_NOISY_{l}_{d}_ID : ComplexInputSamples_Noisy_{l}_{nb}_{d.lower()}.txt 
               Pattern INPUTS_CIFFT_NOISY_{l}_{d}_ID : ComplexInputIFFTSamples_Noisy_{l}_{nb}_{d.lower()}.txt 
               Pattern REF_CFFT_NOISY_{l}_{d}_ID : ComplexFFTSamples_Noisy_{l}_{nb}_{d.lower()}.txt 
""",file=c)
                    nb = nb + 1

                for l in fftsizes(d):
                    print(f"""               Pattern INPUTS_CFFT_STEP_{l}_{d}_ID : ComplexInputSamples_Step_{l}_{nb}_{d.lower()}.txt 
               Pattern INPUTS_CIFFT_STEP_{l}_{d}_ID : ComplexInputIFFTSamples_Step_{l}_{nb}_{d.lower()}.txt 
               Pattern REF_CFFT_STEP_{l}_{d}_ID : ComplexFFTSamples_Step_{l}_{nb}_{d.lower()}.txt 
""",file=c)
                    nb = nb + 1

                print(f"               Output  OUTPUT_CFFT_{d}_ID : ComplexFFTSamples\n",file=c)
                print("               Functions {",file=c)
                for l in fftsizes(d):
                    print(f"                  ",file=c,end="")
                    print(disabled(d,l,f"cfft_noisy_{l}_{d.lower()}:test_cfft_{d.lower()}"),file=c)
                for l in fftsizes(d):
                    print(f"                  ",file=c,end="")
                    print(disabled(d,l,f"cfft_step_{l}_{d.lower()}:test_cfft_{d.lower()}"),file=c)
                for l in fftsizes(d):
                    print(f"                  ",file=c,end="")
                    print(disabled(d,l,f"cifft_noisy_{l}_{d.lower()}:{ifft_testname(d)}{d.lower()}"),file=c)
                for l in fftsizes(d):
                    print(f"                  ",file=c,end="")
                    print(disabled(d,l,f"cifft_step_{l}_{d.lower()}:{ifft_testname(d)}{d.lower()}"),file=c)
        
                print("               }",file=c)
                print("           }",file=c)
        print("       }",file=c)
        print("   }",file=c)
        print("}",file=c)

    exit(0)

def type_from_d(s):
    if s == "F32":
       return "float32_t"
    if s == "F16":
       return "float16_t"
    if s == "Q31":
       return "q31_t"
    if s == "Q15":
       return "q15_t"
    return "void"

def error(d):
    if d == "Q31":
        return "ASSERT_NEAR_EQ(outputfft,ref,ABS_ERROR);"
    if d == "Q15":
        return "ASSERT_NEAR_EQ(outputfft,ref,ABS_ERROR);"
    return "ASSERT_CLOSE_ERROR(outputfft,ref,ABS_ERROR,REL_ERROR);"


def threshold(d):
    if d == "Q15":
        return """#define SNR_THRESHOLD 20
#define ABS_ERROR ((q15_t)10)
"""
    if d == "Q31":
        return """#define SNR_THRESHOLD 100
#define ABS_ERROR ((q31_t)950)
"""
    if d == "F16":
        return """#define SNR_THRESHOLD 56
#define REL_ERROR (5.0e-2)
#define ABS_ERROR (1.0e-1)"""
    return """#define SNR_THRESHOLD 100
#define REL_ERROR (1.0e-4)
#define ABS_ERROR (2.0e-3)"""

def the_test(d):
    return f"""{threshold(d)}

    void TransformNeonC{d}::test_cfft_{d.lower()}()
    {{
       const {type_from_d(d)} *inp = input.ptr();

       {type_from_d(d)} *infftp = inputfft.ptr();

       {type_from_d(d)} *outfftp = outputfft.ptr();

        memcpy(infftp,inp,sizeof({type_from_d(d)})*input.nbSamples());

   
        {type_from_d(d)} *bufferp = bufferfft.ptr();
        arm_cfft_{d.lower()}(
             this->varInstCfft{d}Ptr,
             infftp,
             outfftp,
             bufferp,
             this->ifft);

          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        {error(d)}
        ASSERT_EMPTY_TAIL(outputfft);


        
    }}

    {cifft(d)}
"""

def test_specific_init(d):
    if d == "F32":
        return ""
    if d == "F16":
        return ""
    return f"tmp2.create(ref.nbSamples(),TransformNeonC{d}::OUTPUT_CFFT_{d}_ID,mgr);"

if args.c:
    for d in DATATYPES:
        path=os.path.join(args.others[0],f"TransformNeonC{d}.cpp")
        with open(path,"w") as c:
             print(f"""#if defined(ARM_MATH_NEON)
#include "TransformNeonC{d}.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"           
""",file=c)
             print(the_test(d),file=c)

             print(f"""
   void TransformNeonC{d}::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
   {{

       (void)paramsArgs;
       switch(id)
       {{
""",file=c)

             nb = 0
             for l in fftsizes(d):
                nb = nb + 1
                print(f"""
         case TransformNeonC{d}::TEST_CFFT_{d}_{nb}:

            input.reload(TransformNeonC{d}::INPUTS_CFFT_NOISY_{l}_{d}_ID,mgr);
            ref.reload(  TransformNeonC{d}::REF_CFFT_NOISY_{l}_{d}_ID,mgr);

            this->varInstCfft{d}Ptr=arm_cfft_init_dynamic_{d.lower()}({l});

            this->ifft=0;

          break;
""",file=c)
             for l in fftsizes(d):
                nb = nb + 1
                print(f"""
         case TransformNeonC{d}::TEST_CFFT_{d}_{nb}:

            input.reload(TransformNeonC{d}::INPUTS_CFFT_STEP_{l}_{d}_ID,mgr);
            ref.reload(  TransformNeonC{d}::REF_CFFT_STEP_{l}_{d}_ID,mgr);

            this->varInstCfft{d}Ptr=arm_cfft_init_dynamic_{d.lower()}({l});

            this->ifft=0;

          break;
""",file=c)
             for l in fftsizes(d):
                nb = nb + 1
                print(f"""
         case TransformNeonC{d}::{ifft_testname(d).upper()}{d}_{nb}:

            input.reload(TransformNeonC{d}::INPUTS_CIFFT_NOISY_{l}_{d}_ID,mgr);
            ref.reload(  TransformNeonC{d}::INPUTS_CFFT_NOISY_{l}_{d}_ID,mgr);

            this->varInstCfft{d}Ptr=arm_cfft_init_dynamic_{d.lower()}({l});

            this->ifft=1;
            {scaling_init(d,l)}

          break;
""",file=c)
             for l in fftsizes(d):
                nb = nb + 1
                print(f"""
         case TransformNeonC{d}::{ifft_testname(d).upper()}{d}_{nb}:

            input.reload(TransformNeonC{d}::INPUTS_CIFFT_STEP_{l}_{d}_ID,mgr);
            ref.reload(  TransformNeonC{d}::INPUTS_CFFT_STEP_{l}_{d}_ID,mgr);

            this->varInstCfft{d}Ptr=arm_cfft_init_dynamic_{d.lower()}({l});

            this->ifft=1;
            {scaling_init(d,l)}

          break;
""",file=c)


             print(f"""
       }}
       inputfft.create(ref.nbSamples(),TransformNeonC{d}::OUTPUT_CFFT_{d}_ID,mgr);

       outputfft.create(ref.nbSamples(),TransformNeonC{d}::OUTPUT_CFFT_{d}_ID,mgr);
       bufferfft.create(ref.nbSamples(),TransformNeonC{d}::OUTPUT_CFFT_{d}_ID,mgr);
       {test_specific_init(d)}

    }}

    void TransformNeonC{d}::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {{
        (void)id;
        outputfft.dump(mgr);
        if (varInstCfft{d}Ptr)
        {{
            free(varInstCfft{d}Ptr);
        }}
    }}

#endif""",file=c)
    exit(0)

SINES=[0.25,0.5,0.9]
NOISES=[0.1,0.4]

def asReal(a):
    #return(a.view(dtype=np.float64))
    return(a.reshape(np.size(a)).view(dtype=np.float64))

def noiseSignal(nb):
    return(np.random.randn(nb))

def sineSignal(freqRatio,nb):
    fc = nb / 2.0
    f = freqRatio*fc 
    time = np.arange(0,nb)
    return(np.sin(2 * np.pi * f *  time/nb))

def noisySineSignal(noiseAmp,r,nb):
    return(noiseAmp*noiseSignal(nb) + r*sineSignal(0.25,nb))

def stepSignal(r,nb):
   n = int(nb/2)
   return(np.concatenate((np.zeros(n), r*np.ones(n))))

def writeFFTForSignal(config,sig,scaling,i,j,nb,signame):
    fft=scipy.fftpack.fft(sig)
    ifft = np.copy(fft)
    if scaling:
        fft = np.array([x/nb for x in fft])
    config.writeInput(i, asReal(sig),"ComplexInputSamples_%s_%d_" % (signame,nb))
    config.writeInput(i, asReal(fft),"ComplexFFTSamples_%s_%d_" % (signame,nb))
    config.writeInput(i, asReal(fft),"ComplexInputIFFTSamples_%s_%d_" % (signame,nb))


def writeTests(config,scaling,d):
    i = 1

    # Write FFT tests for sinusoid
    j = 0
    for nb in fftsizes(d):
        sig = noisySineSignal(0.05,0.7,nb)
        sigc = np.array([complex(x) for x in sig])
        
        writeFFTForSignal(config,sigc,scaling,i,j,nb,"Noisy")
        i = i + 1
        j = j + 1

    # Write FFT tests for step
    j = 0
    for nb in fftsizes(d):
        sig = stepSignal(0.9,nb)
        sigc = np.array([complex(x) for x in sig])
        writeFFTForSignal(config,sigc,scaling,i,j,nb,"Step")
        i = i + 1
        j = j + 1


   
def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","TransformNeon","TransformNeon")
    PARAMDIR = os.path.join("Parameters","DSP","TransformNeon","TransformNeon")
    
    configf32=Tools.Config(PATTERNDIR,PARAMDIR,"f32")
    configf16=Tools.Config(PATTERNDIR,PARAMDIR,"f16")
    configq31=Tools.Config(PATTERNDIR,PARAMDIR,"q31")
    configq15=Tools.Config(PATTERNDIR,PARAMDIR,"q15")

    
    writeTests(configf32,False,"F32")
    writeTests(configf16,False,"F16")
    writeTests(configq31,True,"Q31")
    writeTests(configq15,True,"Q15")



if __name__ == '__main__':
  generatePatterns()