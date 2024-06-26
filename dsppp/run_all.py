import re
import argparse
import os.path 
import itertools
import subprocess 
import sys 
from os import environ

from colorama import init,Fore, Back, Style

try:
   os.mkdir("AC6_results")
except:
    pass 

try:
   os.mkdir("GCC_results")
except:
    pass 

try:
   os.mkdir("CLANG_results")
except:
    pass 

DEBUG = False
ERROR_OCCURED = False

all_errors = []

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print(Fore.YELLOW + Style.BRIGHT + s + Style.RESET_ALL)

def printError(s):
    print(Fore.RED + Style.BRIGHT +  s + Style.RESET_ALL+"\n")

class Result:
    def __init__(self,msg,error=False):
        self._error = error
        self._msg = msg

    @property
    def error(self):
        return self._error

    @property
    def msg(self):
        return self._msg

def is_error(res,test_name,err):
    if res.error:
        printError("Error")
        all_errors.append(test_name)
        print(test_name,file=err)
        print(res.msg,file=err)
        print("--------------",file=err)
        return(True)
    return(False)

def run(args,mustPrint=False,dumpStdErr=True,timeout=20,printCmd=False):
    global ERROR_OCCURED
    global DEBUG
    try:
        if DEBUG or printCmd:
            print(" ".join(args))
        result=subprocess.run(args,text=True,capture_output=True,timeout=timeout)
        if result.returncode !=0 :
             ERROR_OCCURED = True
             if dumpStdErr:
                return(Result(result.stderr + "\n\nSTDOUT:\n\n" + result.stdout,error=True))
             else:
                return(Result(result.stdout,error=True))

        if mustPrint:
            print(result.stdout)
        return(Result(result.stdout))
    except Exception as e:
        printError("Exception occured")
        ERROR_OCCURED = True
        return(Result(str(e),error=True))

parser = argparse.ArgumentParser(description='Parse test description')
parser.add_argument('-c', nargs='?',type = str, default="M55",help="M55/M4/M0")
parser.add_argument('-p', nargs='?',type = str, default="VHT",help="VHT/MPS3")
parser.add_argument('-a', action='store_true', help="Generate allocator definitions")
parser.add_argument('-i', action='store_true', help="Refresh global allocator index")
parser.add_argument('-b', action='store_true', help="Only benchmarks")
parser.add_argument('-d', action='store_true', help="Dry run")
parser.add_argument('-g', nargs='?',type = str, default="AC6",help="AC6 / CLANG / GCC")
parser.add_argument('-u', nargs='?',type = str, default="L85986697A",help="Debug UUID")
parser.add_argument('-t', action='store_true', help="Enable test mode")
parser.add_argument('-avh', nargs='?',type = str, default="C:/Keil_v5/ARM/avh-fvp/bin/models", help="AVH folder")

args = parser.parse_args()

GHACTION = False 

if "AVH_FVP_PLUGINS" in os.environ:
    GHACTION = True

if not GHACTION:
   import mps3run

init()

if args.a:
    printTitle("Mode allocator generations")

if args.i:
    printTitle("Allocator test index refresh")

NAME_TO_BOARD = {
    "M55": "Corstone-300",
    "Corstone-300": "Corstone-300",
    "M4": "M4",
    "M0" : "M0P"
}

def results():
    if args.g == "AC6":
        return("AC6_results")

    if args.g == "GCC":
        return("GCC_results")

    if args.g == "CLANG":
        return("CLANG_results")

    print(f"Compiler {args.g} not known")
    exit(1)

def target_name():
    return(f"{args.p}-{NAME_TO_BOARD[args.c]}")

def cmd_args():
    # cbuild -O cprj test.csolution.yml -r --toolchain AC6 -c test.Release+MPS3-Corstone-300 
    toolchain = args.g
    target = f"test.Release+{target_name()}"
 
    command = ["-O", "cprj", 
               "test.csolution.yml", 
               "--toolchain", toolchain, 
               "-c", target]

    return(command)



if args.g == "AC6":
    ext = ".axf"
else:
    ext = ".elf"

fvpWin = {"M55":"FVP_Corstone_SSE-300_Ethos-U55.exe",
          "M4":"FVP_MPS2_Cortex-M4.exe",
          "M0":"FVP_MPS2_Cortex-M0plus.exe"}

fvpUnix = {"M55":"FVP_Corstone_SSE-300_Ethos-U55",
          "M4":"FVP_MPS2_Cortex-M4",
          "M0":"FVP_MPS2_Cortex-M0plus"}

AVHROOT = args.avh

TESTS=["DOT_TEST",
       "VECTOR_TEST",
       "ROW_TEST",
       "COL_TEST",
       "MATRIX_TEST",
       "FUSION_TEST"
       ]

# Some tests are too big (code size) and needs to be decomposed
# They contain SUBTEST1, SUBTEST2 ... #if in the code
# This script must know how many subtests are defined in each test
# suite
# No need to define an entry in this dictionary when no
# subtest is defined
SUBTESTS = {"MATRIX_TEST":19}
# Subtests that are only for testing and not benchmarks
ONLY_TESTS = {"MATRIX_TEST":[3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19]}

def is_only_test(n,i):
    if n[0] in ONLY_TESTS:
       return(i in ONLY_TESTS[n[0]])
    return False

DATATYPES = ["F64_DT",
             "F32_DT",
             "F16_DT",
             "Q31_DT",
             "Q15_DT",
             "Q7_DT"
             ]

MODE = ["STATIC_TEST",
        "DYNAMIC_TEST"
        ]

# Restricted tests for debugging
#TESTS=["FUSION_TEST"]
#DATATYPES=["Q15_DT"]
#MODE = ["STATIC_TEST"]

all_tests = list(itertools.product(TESTS,DATATYPES,MODE))



ALLOC = "#define POOL_ALLOCATOR"
TESTMODE = ""
if args.t:
    TESTMODE = "#define TESTMODE"

if args.a:
    # Stat allocator enabled and we do stats on VHT CS300 only
    ALLOC = "//#define POOL_ALLOCATOR"
    args.c = "M55"
    args.p = "VHT"

BENCH = "//#define ONLY_BENCHMARKS"
if args.b:
    BENCH = "#define ONLY_BENCHMARKS"

HEADER = f"""#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

{TESTMODE}
{ALLOC}
{BENCH}

#define %s
#define %s
#define %s
%s

#endif
"""



def out_path():
    return(os.path.join("cprj","out","test",target_name(),"Release","test"+ ext))

def configure_and_build_test(test_name,test,err,subtest,first):
    if subtest is not None:
        subteststr = f"#define SUBTEST{subtest}"
    else:
        subteststr = ""
    with open("test_config.h","w") as c:
        print(HEADER % (test + (subteststr,)),file=c)
    if first:
       res = run(["cbuild"] + cmd_args() + ["-r","--update-rte"],timeout=600,printCmd=True)
    else:
       res = run(["cbuild"] +cmd_args(),timeout=600,printCmd=True)
    if not is_error(res,test_name,err):
        if DEBUG:
            print(res.msg)
        return(True)
    return(False)

def process_allocator_data(test_name,test,msg,subtest):
    lines = msg.splitlines()
    state = 0
    alloc_cpp = []
    alloc_h = []
    for l in lines:
        if re.match(r"^ALLOC_POOL.*$",l):
            alloc_cpp.append(l.strip())
        if re.match(r"^POOL.*$",l):
            alloc_h.append(l.strip())
    if subtest is not None:
        HEADER=f"#if defined({test[0]}) && defined({test[1]}) && defined({test[2]}) && defined(SUBTEST{subtest})"
    else:
        HEADER=f"#if defined({test[0]}) && defined({test[1]}) && defined({test[2]})"
    # Gen h
    with open(os.path.join("allocation",test_name)+".h","w") as h:
        print(HEADER,file=h)
        for l in alloc_h:
            print(l,file=h)
        print("#endif",file=h)

    # Gen cpp
    with open(os.path.join("allocation",test_name)+".cpp","w") as h:
         print(HEADER,file=h)
         for l in alloc_cpp:
             print(l,file=h)
         print("#endif",file=h)

def process_bench(test_name,test,msg,subtest):
    global DEBUG
    lines = msg.splitlines()
    test_name = args.p +"_" + args.c + "_" + test_name
    if DEBUG:
       print(os.path.join(results(),test_name)+".txt")
    with open(os.path.join(results(),test_name)+".txt","w") as h:
       for l in lines:
           print(l.rstrip(),file=h)


def process_result(test_name,test,msg,subtest):
    printSubTitle("Process result")
    if args.a:
        process_allocator_data(test_name,test,msg,subtest)
    else:
        process_bench(test_name,test,msg,subtest)

def runVHT(test_name,test,err,subtest):
    core = args.c
    target = target_name()
    config = os.path.join("fvp_configs",target) + ".txt"
    #print(target)
    #print(config)
    if core == "M55":
        exe = "cpu0=" + out_path()
    else:
        exe = out_path()

    if AVHROOT:
       avhAttempt = os.path.join(AVHROOT,fvpWin[core])
       if os.path.exists(avhAttempt):
          avh = avhAttempt
   
       avhAttempt = os.path.join(AVHROOT,fvpUnix[core])
       if os.path.exists(avhAttempt):
          avh = avhAttempt
    else:
       avh = avhUnixExe[core]

    res=run([avh,"-f",config,"-a",exe],printCmd=True)
    if not is_error(res,test_name,err):
       process_result(test_name,test,res.msg,subtest)

def runMPS3(test_name,test,err,subtest):
    lines=""
    res = None
    try:
        exe = out_path()
        lines = mps3run.run_out(exe,args.u)
        res = Result(lines)
    except Exception as e:    
        res = Result(str(e),error = True)
    if not is_error(res,test_name,err):
       process_result(test_name,test,res.msg,subtest)
   
def runATest(test,file_err,nb,NB_MAX,current_nb_axf,nb_axf,first=True,subtest=None):
    global DEBUG
    if subtest is not None:
       maxsub = SUBTESTS[test[0]]
       test_name=f"{test[0]}_{test[1]}_{test[2]}_{subtest}"
       printTitle(test_name + f" : AXF {current_nb_axf} / {nb_axf}, TEST {nb}/{NB_MAX} (subtest {subtest}/{maxsub})")
    else:
       test_name=f"{test[0]}_{test[1]}_{test[2]}"
       printTitle(test_name + f" : AXF {current_nb_axf} / {nb_axf}, TEST {nb}/{NB_MAX}")
    if args.d:
        return
    printSubTitle("Configure and build")
    if configure_and_build_test(test_name,test,file_err,subtest,first):
        printSubTitle("Run")
        if args.p == "VHT":
            runVHT(test_name,test,file_err,subtest)
        if args.p == "MPS3" and args.c == "M55":
            runMPS3(test_name,test,file_err,subtest)     

nb_axf = 0
for test in all_tests:
    if test[0] in SUBTESTS:
        for subtestnbb in range(SUBTESTS[test[0]]):
            if not args.b or not is_only_test(test,subtestnbb+1):
               nb_axf = nb_axf + 1
    else:
            nb_axf = nb_axf + 1
print(f"Number of axf to test = {nb_axf}")

with open(os.path.join(results(),f"errors_{args.c}.txt"),"w") as err:
    # Generate include for allocations
    if args.a or args.i:
        with open(os.path.join("allocation","all.h"),"w") as fh:
            for test in all_tests:
                if test[0] in SUBTESTS:
                   for subtestnbb in range(SUBTESTS[test[0]]):
                      test_name=f"{test[0]}_{test[1]}_{test[2]}_{subtestnbb+1}"
                      print(f"#include \"{test_name}.h\"",file=fh)
                else:
                    test_name=f"{test[0]}_{test[1]}_{test[2]}"
                    print(f"#include \"{test_name}.h\"",file=fh)
    
        with open(os.path.join("allocation","all.cpp"),"w") as fc:
            for test in all_tests:
                if test[0] in SUBTESTS:
                   for subtestnbb in range(SUBTESTS[test[0]]):
                       test_name=f"{test[0]}_{test[1]}_{test[2]}_{subtestnbb+1}"
                       print(f"#include \"{test_name}.cpp\"",file=fc)
                else:
                   test_name=f"{test[0]}_{test[1]}_{test[2]}"
                   print(f"#include \"{test_name}.cpp\"",file=fc)
    
    if not args.i:
        NB_MAX = len(all_tests)
        nb = 1 # test cases
        current_axf = 1
        first = True
        for test in all_tests:
            if test[0] in SUBTESTS:
                for subtestnbb in range(SUBTESTS[test[0]]):
                    if not args.b or not is_only_test(test,subtestnbb+1):
                       runATest(test,err,nb,NB_MAX,current_axf,nb_axf,first,subtestnbb+1)
                       current_axf = current_axf + 1
                       first = False
            else:
                runATest(test,err,nb,NB_MAX,current_axf,nb_axf,first)
                current_axf = current_axf + 1
                first = False
            nb = nb + 1


if not GHACTION:
   if ERROR_OCCURED:
       printError("Error in tests:")
       for n in all_errors:
           printError(n)
       sys.exit("Error occurred")
   else:
       sys.exit(0)
