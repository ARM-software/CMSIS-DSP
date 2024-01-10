import argparse
import pickle
import sys
import subprocess 
import os 
import colorama
from colorama import init,Fore, Back, Style

parser = argparse.ArgumentParser(description='Parse test description')
parser.add_argument('-avh', nargs='?',type = str, default="C:/Keil_v5/ARM/VHT", help="AVH folder")
parser.add_argument('-d', action='store_true', help="Debug log")
parser.add_argument('-n', action='store_true', help="No force rebuild")
parser.add_argument('-r', action='store_true', help="Raw results only")
parser.add_argument('-c', action='store_true', help="Display cycles (so passing test are displayed)")

args = parser.parse_args()

DEBUG=False 
if args.d:
    DEBUG=True

init()

sys.path.append("..")

from TestScripts.Parser import TreeElem

ERROR_OCCURED = False

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print("\n" + Fore.YELLOW + Style.BRIGHT + s + Style.RESET_ALL)

def printError(s):
    print("\n" + Fore.RED + Style.BRIGHT +  s + Style.RESET_ALL)

# Load Output.pickle files for the test description
def loadRoot(f):
    root = None
    with open(f,"rb") as inf:
         root=pickle.load(inf)
    return(root)

# Get test suites from the test descriptions
def getSuites(node,filterList,currentList=[]):
    if node.kind==TreeElem.SUITE:
        currentList.append(node.data["class"])
    if node.kind==TreeElem.GROUP:
       if not node.data["class"] in filterList:
          for c in node.children:
             getSuites(c,filterList,currentList)
          
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
    
    

# Run a command and get error or result
# For the test report we don't need the stderr
# in case of error since the test report is giving
# all the details. So, there is an option to
# disable the dump of stderr
def run(*args,mustPrint=False,dumpStdErr=True):
    global ERROR_OCCURED
    global DEBUG
    try:
        if DEBUG:
            print(" ".join(args))
        result=subprocess.run(args,text=True,capture_output=True,timeout=600)
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




   
# Configuration file for AVH core
configFiles={
    "CS310":"ARM_VHT_Corstone_310_config.txt",
    "CS300":"ARM_VHT_Corstone_300_config.txt",
    "M55":"ARM_VHT_MPS2_M55_config.txt",
    "M33_DSP_FP":"ARM_VHT_MPS2_M33_DSP_FP_config.txt",
    "M7DP":"ARM_VHT_MPS2_M7DP_config.txt",
    "M4FP":"ARM_VHT_MPS2_M4FP_config.txt",
    "M3":"ARM_VHT_MPS2_M3_config.txt",
    "M23":"ARM_VHT_MPS2_M23_config.txt",
    "M0plus":"ARM_VHT_MPS2_M0plus_config.txt",
}

# Windows executable
# (At some point this script will also support
# unix)
avhExe={
    "CS310":"VHT_Corstone_SSE-310.exe",
    "CS300":"VHT_Corstone_SSE-300_Ethos-U55.exe",
    "M55":"VHT_MPS2_Cortex-M55.exe",
    "M33_DSP_FP":"VHT_MPS2_Cortex-M33.exe",
    "M7DP":"VHT_MPS2_Cortex-M7.exe",
    "M4FP":"VHT_MPS2_Cortex-M4.exe",
    "M3":"VHT_MPS2_Cortex-M3.exe",
    "M23":"VHT_MPS2_Cortex-M23.exe",
    "M0plus":"VHT_MPS2_Cortex-M0plus.exe",
}

AVHROOT = args.avh

# Run AVH
def runAVH(build,core):
    axf="out/test/%s/Release/test.axf" % (build,)
    elf="out/test/%s/Release/test.elf" % (build,)
    app = axf 
    if os.path.exists(axf):
        app = axf 
    if os.path.exists(elf):
        app = elf
    config = os.path.join("configs",configFiles[core])
    avh = os.path.join(AVHROOT,avhExe[core])
    res=run(avh,"-f",config,app)
    return(res)
   
####################

# Test descriptions to use
tests=["../Output.pickle","../Output_f16.pickle"]
# Test group to avoid
filterList=["NNTests","ExampleTests"]
allSuites=[]

# Load all the test suite to run
for t in tests:
    root=loadRoot(t)
    suites=[]
    getSuites(root,filterList,suites)
    allSuites += [(x,t) for x in suites ]

# Test suite and output pickle needed to decode the result
#print(allSuites)

#allSuites=[
#("MFCCQ15","../Output.pickle"),
#("MFCCQ31","../Output.pickle"),
#("SupportTestsF16","../Output_f16.pickle"),
#]

#allSuites=[("ComplexTestsF32","../Output.pickle"),
#("DistanceTestsF32","../Output.pickle"),
#("UnaryTestsF32","../Output.pickle"),
#("QuaternionTestsF32","../Output.pickle"),
#("StatsTestsF32","../Output.pickle")
#]

#allSuites=[
#("TransformCF64","../Output.pickle"),
#("TransformCF32","../Output.pickle"),
#("TransformCQ31","../Output.pickle"),
#("TransformCQ15","../Output.pickle"),
#
#("TransformRF64","../Output.pickle"),
#("TransformRF32","../Output.pickle"),
#("TransformRQ31","../Output.pickle"),
#("TransformRQ15","../Output.pickle"),
#
#("MFCCF32","../Output.pickle"),
#("MFCCQ31","../Output.pickle"),
#("MFCCQ15","../Output.pickle"),
#
#("MFCCF16","../Output_f16.pickle"),
#("TransformCF16","../Output_f16.pickle"),
#("TransformRF16","../Output_f16.pickle")
#]

#allSuites=[("StatsTestsQ7","../Output.pickle")]

allSuites=[
("InterpolationTestsF32","../Output.pickle"),
("InterpolationTestsF16","../Output_f16.pickle"),
#("MISCQ15","../Output.pickle"),
#("MISCQ7","../Output.pickle"),
#("FIRF16","../Output_f16.pickle")
]

# Solution and build file for all
# the tests
# It is a pair : csolution target type and AVH identification
# AVH identification is used to find the executable
# and the configuration file
solutions={
    'test_ac6.csolution.yml':[
    #  ("VHT-Corstone-310","CS310"),
      ("VHT-Corstone-300","CS300"),
      ("VHT_M33","M33_DSP_FP"),
      ("VHT_M7","M7DP"),
      ("VHT_M7_UNROLLED","M7DP"),
      ("VHT_M4","M4FP"),
    #  #("VHT_M3","M3"),
    #  #("VHT_M23","M23"),
      ("VHT_M0P","M0plus")
    ],
    'test_gcc.csolution.yml':[
      #("VHT-Corstone-310","CS310"),
      #("VHT_M55","M55"),
      ##("VHT_M33","M33_DSP_FP"),
      ("VHT_M7","M7DP"),
      ("VHT_M7_UNROLLED","M7DP"),
      ("VHT_M4","M4FP"),
      ##("VHT_M3","M3"),
      ##("VHT_M23","M23"),
      ("VHT_M0P","M0plus")
    ]
}

# Override previous solutions for more restricted testing.
solutions={
    'test_ac6.csolution.yml':[
      ("VHT-Corstone-300","CS300"),
    ]
}

HTMLHEADER="""<html>
<header>
<title>CMSIS-DSP Test summary</title>
</header>
<body>
""" 

HTMLFOOTER="""
</body></html>
"""

# Run the tests and log the result
# in a summary.html file
with open("summary.html","w") as f:
    print(HTMLHEADER,file=f)
    for s in solutions:
        printTitle("Process solution %s" % s)
        res=run("csolution","convert","-s",s)
        if res.error:
            printError("Error csolution")
            print("<p><font color=\"red\">Error converting csolution %s</font></p><PRE>" % s,file=f)
            print(res.msg,file=f)
            print("</PRE>",file=f)
            continue
        print("<h1>Solution %s</h1>" % s,file=f)
        maxNbBuilds=len(solutions[s])
        buildNb=0
        for build,core in solutions[s]:
            buildNb = buildNb + 1
            print("<h2>Core %s</h2>" % build,file=f)
            printTitle("Process core %s (%d/%d)" % (build,buildNb,maxNbBuilds))
            buildFile="test.Release+%s.cprj" % build
            nb = 0 
            maxNb = len(allSuites)
            for s,pickle in allSuites:
                nb = nb + 1
                printSubTitle("Process suite %s (%d/%d)" % (s,nb,maxNb))
                res=run(sys.executable,"../processTests.py","-gen","..","-p","../Patterns","-d","../Parameters","-f",pickle,"-e",s,mustPrint=True)
                if res.error:
                    printError("Error processTests")
                    print("<p><font color=\"red\">Error generating %s</font></p><PRE>" % s,file=f)
                    print(res.msg,file=f)
                    print("</PRE>",file=f)
                    continue
                if nb==1:
                   # -r is needed for first
                   # build when we switch
                   # between different solutions
                   # (Like one using AC6 and the other
                   # using gcc)
                   if args.n:
                      res=run("cbuild",buildFile)
                   else:
                      res=run("cbuild","-r",buildFile)
                else:
                   res=run("cbuild",buildFile)
                if res.error:
                    printError("Error cbuild")
                    print("<p><font color=\"red\">Error building %s</font></p><PRE>" % s,file=f)
                    print(res.msg,file=f)
                    print("</PRE>",file=f)
                    continue
                printSubTitle("Run AVH")
                res=runAVH(build,core)
                if res.error:
                    printError("Error running AVH")
                    print("<p><font color=\"red\">Error running %s</font></p><PRE>" % s,file=f)
                    print(res.msg,file=f)
                    print("</PRE>",file=f)
                    continue
                else:
                    with open("results.txt","w") as o:
                        print(res.msg,file=o)
                    # Dump raw result
                    if args.r:
                        print(res.msg)
                # If raw result, no post processing
                if not args.r:
                    res=run(sys.executable,"../processResult.py","-f",pickle,"-e","-ahtml","-r","results.txt",dumpStdErr=False)
                    if res.error:
                        printError("Error processResult")
                        print("<p><font color=\"red\">Error processing %s result</font></p><PRE>" % s,file=f)
                        print(res.msg,file=f)
                        print("</PRE>",file=f)
                        continue
                    else:
                        # When no error the section is 
                        # included in final file on when 
                        # cycles are requested
                        if args.c:
                           print(res.msg,file=f)
    print(HTMLFOOTER,file=f)

if ERROR_OCCURED:
    sys.exit("Error occurred")
else:
    sys.exit(0)