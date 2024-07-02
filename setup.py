#from distutils.core import setup, Extension
from setuptools import setup, Extension,find_packages
import io
import glob
import numpy
import sys
import os
import os.path
import re
import pathlib


here = pathlib.Path(__file__).parent.resolve()
ROOT = here
ROOT=""

PYTHON_MOD = os.path.join(ROOT,"cmsisdsp")
version_path = os.path.join(PYTHON_MOD,"version.py")

__version__ = re.search(
    r'__version__\s*=\s*[\'"]([^\'"]*)[\'"]',  # It excludes inline comment too
    io.open(version_path, encoding='utf_8_sig').read()
    ).group(1)


includes = [os.path.join(ROOT,"Include"),os.path.join(ROOT,"PrivateInclude"),os.path.join("PythonWrapper","cmsisdsp_pkg","src")]

if sys.platform == 'win32':
  cflags = ["-DWIN","-DCMSISDSP","-DUNALIGNED_SUPPORT_DISABLE"] 
else:
  cflags = ["-Wno-attributes","-Wno-unused-function","-Wno-unused-variable","-Wno-implicit-function-declaration","-DCMSISDSP","-D__GNUC_PYTHON__"]

# Add dependencies
transformMod = [] # transform + common + basic + complexf + fastmath + matrix + statistics
statisticsMod = [] # statistics + common + fastmath + basic
interpolationMod = [] # interpolation + common
filteringMod = [] # filtering + common + support + fastmath + basic
controllerMod = [] # controller + common

matrixMod = [] # matrix + basic
supportMod = [] # support 
complexfMod = [] # complexf + fastmath + common + basic
basicMod = [] # basic
quaternionMod = [] # quaternion
fastmathMod = [] # basic + fastmath + common
distanceMod = [] # distance + common + basic + statistics + fastmath
bayesMod = [] # bayes + fastmath + common + statistics + basic
svmMod = [] # svm + fastmath + common + basic

windowMod = [] # window

filteringMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_filtering.c"))
matrixMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_matrix.c"))
supportMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_support.c"))
statisticsMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_statistics.c"))
complexfMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_complexf.c"))
basicMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_basic.c"))
controllerMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_controller.c"))
transformMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_transform.c"))
interpolationMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_interpolation.c"))
quaternionMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_quaternion.c"))
fastmathMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_fastmath.c"))
distanceMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_distance.c"))
bayesMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_bayes.c"))
svmMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_svm.c"))
windowMod.append(os.path.join("PythonWrapper","cmsisdsp_pkg","src","cmsisdsp_window.c"))




missing=set([
  ])

def notf16(number):
  if re.search(r'f16',number):
     return(False)
  if re.search(r'F16',number):
     return(False)
  return(True)

def isnotmissing(src):
  name=os.path.splitext(os.path.basename(src))[0]
  return(not (name in missing))

# If there are too many files, the linker command is failing on Windows.
# So f16 functions are removed since they are not currently available in the wrapper.
# A next version will have to structure this wrapper more cleanly so that the
# build can work even with more functions

filtering = list(filter(isnotmissing,list(filter(notf16, filteringMod))))
matrix = list(filter(isnotmissing,list(filter(notf16, matrixMod))))
support = list(filter(isnotmissing,list(filter(notf16, supportMod))))
statistics = list(filter(isnotmissing,list(filter(notf16, statisticsMod))))
complexf = list(filter(isnotmissing,list(filter(notf16, complexfMod))))
basic = list(filter(isnotmissing,list(filter(notf16, basicMod))))
controller = list(filter(isnotmissing,list(filter(notf16, controllerMod))))
transform = list(filter(isnotmissing,list(filter(notf16, transformMod))))
interpolation = list(filter(isnotmissing,list(filter(notf16, interpolationMod))))
quaternion = list(filter(isnotmissing,list(filter(notf16, quaternionMod))))
fastmath = list(filter(isnotmissing,list(filter(notf16, fastmathMod))))
distance = list(filter(isnotmissing,list(filter(notf16, distanceMod))))
bayes = list(filter(isnotmissing,list(filter(notf16, bayesMod))))
svm = list(filter(isnotmissing,list(filter(notf16, svmMod))))
window = list(filter(isnotmissing,list(filter(notf16, windowMod))))

#for l in filtering:
#  print(os.path.basename(l))
#quit()

def mkModule(name,srcs,funcDir):
  localinc = os.path.join(ROOT,"Source",funcDir)
  libdir = [os.path.join(ROOT,"PythonWrapper","build","bin_dsp")]
  lib = ["CMSISDSP"]
  extraobjs=[]
  
  if sys.platform.startswith('linux'):
    lib = []
    extraobjs = [os.path.join(ROOT,"PythonWrapper","build_linux","bin_dsp","libCMSISDSP.a")]
    libdir = []
 
  if sys.platform.startswith('darwin'):
    lib = []
    extraobjs = [os.path.join(ROOT,"PythonWrapper","build_darwin","bin_dsp","libCMSISDSP.a")]
    libdir = []
 
  return(Extension(name,
                    sources = (srcs
                              )
                              ,
                    include_dirs =  [localinc] + includes + [numpy.get_include()],
                    extra_compile_args = cflags,
                    library_dirs = libdir,
                    libraries=lib,
                    extra_objects=extraobjs
                              ))

flagsForCommonWithoutFFT=["-DARM_DSP_CONFIG_TABLES", 
    "-DARM_FAST_ALLOW_TABLES", 
    "-DARM_ALL_FAST_TABLES"]

moduleFiltering = mkModule('cmsisdsp_filtering',filtering,"FilteringFunctions")
moduleMatrix = mkModule('cmsisdsp_matrix',matrix,"MatrixFunctions")
moduleSupport = mkModule('cmsisdsp_support',support,"SupportFunctions")
moduleStatistics = mkModule('cmsisdsp_statistics',statistics,"StatisticsFunctions")
moduleComplexf= mkModule('cmsisdsp_complexf',complexf,"ComplexMathFunctions")
moduleBasic = mkModule('cmsisdsp_basic',basic,"BasicMathFunctions")
moduleController = mkModule('cmsisdsp_controller',controller,"ControllerFunctions")
moduleTransform = mkModule('cmsisdsp_transform',transform,"TransformFunctions")
moduleInterpolation = mkModule('cmsisdsp_interpolation',interpolation,"InterpolationFunctions")
moduleQuaternion = mkModule('cmsisdsp_quaternion',quaternion,"QuaternionMathFunctions")
moduleFastmath = mkModule('cmsisdsp_fastmath',fastmath,"FastMathFunctions")
moduleDistance = mkModule('cmsisdsp_distance',distance,"DistanceFunctions")
moduleBayes = mkModule('cmsisdsp_bayes',bayes,"BayesFunctions")
moduleSVM = mkModule('cmsisdsp_svm',svm,"SVMFunctions")
moduleWindow = mkModule('cmsisdsp_window',window,"WindowFunctions")




def build():
  if sys.version_info.major < 3:
      print('setup.py: Error: This package only supports Python 3.', file=sys.stderr)
      sys.exit(1)
  
  setup (name = 'cmsisdsp',
         version = __version__,
         packages=["cmsisdsp"],
         description = 'CMSIS-DSP Python API',
         long_description=open("PythonWrapper_README.md").read(),
         long_description_content_type='text/markdown',
         ext_modules = [moduleFiltering ,
                        moduleMatrix,
                        moduleSupport,
                        moduleStatistics,
                        moduleComplexf,
                        moduleBasic,
                        moduleController,
                        moduleTransform,
                        moduleInterpolation, 
                        moduleQuaternion,
                        moduleFastmath,
                        moduleDistance,
                        moduleBayes,
                        moduleSVM,
                        moduleWindow
                        ],
         include_package_data=True,
         author = 'Copyright (C) 2010-2024 ARM Limited or its affiliates. All rights reserved.',
         author_email = 'christophe.favergeon@arm.com',
         url="https://github.com/ARM-software/CMSIS-DSP",
         python_requires='>=3.9',
         license="License :: OSI Approved :: Apache Software License",
         platforms=['any'],
         classifiers=[
                "Programming Language :: Python :: 3",
                "Programming Language :: Python :: Implementation :: CPython",
                "Programming Language :: C",
                "License :: OSI Approved :: Apache Software License",
                "Operating System :: OS Independent",
                "Development Status :: 4 - Beta",
                "Topic :: Software Development :: Embedded Systems",
                "Topic :: Scientific/Engineering :: Mathematics",
                "Environment :: Console",
                "Intended Audience :: Developers",
          ],
          keywords=['development','dsp','cmsis','cmsis-dsp','Arm','signal processing','maths','ml','cortex-m','cortex-a'],
          install_requires=[
          'numpy>=1.23.5',
          ],
          project_urls={  # Optional
             'Bug Reports': 'https://github.com/ARM-software/CMSIS-DSP/issues',
             'Source': 'https://github.com/ARM-software/CMSIS-DSP',
            }
          )
       

build()
