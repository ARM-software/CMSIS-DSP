#from distutils.core import setup, Extension
from setuptools import setup, Extension,find_packages
from setuptools.command.build_ext import build_ext
import io
import glob
import numpy
import sys
import os
import os.path
import re
import pathlib
import platform
import shutil
import subprocess


here = pathlib.Path(__file__).parent.resolve()
ROOT = here
ROOT=""

BUILD_ROOT = here / "build" / "pythonwrapper"

PYTHON_MOD = os.path.join(ROOT,"cmsisdsp")
version_path = os.path.join(PYTHON_MOD,"version.py")

__version__ = re.search(
    r'__version__\s*=\s*[\'"]([^\'"]*)[\'"]',  # It excludes inline comment too
    io.open(version_path, encoding='utf_8_sig').read()
    ).group(1)


includes = [os.path.join(ROOT,"Include"),os.path.join(ROOT,"PrivateInclude"),os.path.join("PythonWrapper","cmsisdsp_pkg","src")]

linkargs=[]

machine = platform.machine().lower()
ARM = False
if machine == "aarch64" or machine == "arm64":
    ARM = True

if sys.platform == 'win32':
  cflags = ["-DWIN",
            "-DCMSISDSP",
            "-DUNALIGNED_SUPPORT_DISABLE"] 
elif sys.platform == 'darwin':
  if ARM:
     os.environ["ARCHFLAGS"] = "-arch arm64"
     linkargs = ["-arch", "arm64"]
     cflags = ["-DARM_MATH_NEON",
               "-Wno-attributes",
               "-Wno-unused-function",
               "-Wno-unused-variable",
               "-Wno-implicit-function-declaration",
               "-DCMSISDSP",
               "-arch", "arm64",
               "-D__GNUC_PYTHON__"]
  else:
    cflags = ["-Wno-attributes",
              "-Wno-unused-function",
              "-Wno-unused-variable",
              "-Wno-implicit-function-declaration",
              "-DCMSISDSP",
              "-D__GNUC_PYTHON__"]

else:
  if ARM:
    cflags = ["-DARM_MATH_NEON",
              "-Wno-attributes",
              "-Wno-unused-function",
              "-Wno-unused-variable",
              "-Wno-implicit-function-declaration",
              "-DCMSISDSP",
              "-D__GNUC_PYTHON__"]
  else:
    cflags = ["-Wno-attributes",
              "-Wno-unused-function",
              "-Wno-unused-variable",
              "-Wno-implicit-function-declaration",
              "-DCMSISDSP",
              "-D__GNUC_PYTHON__"]
  
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

def cmsisdsp_library_path():
  if sys.platform == 'win32':
    return(BUILD_ROOT / "bin_dsp" / "Release" / "CMSISDSP.lib")

  return(BUILD_ROOT / "bin_dsp" / "libCMSISDSP.a")


def built_cmsisdsp_library_candidates():
  if sys.platform == 'win32':
    return([
      BUILD_ROOT / "bin_dsp" / "Release" / "CMSISDSP.lib",
      BUILD_ROOT / "bin_dsp" / "CMSISDSP.lib",
    ])

  return([BUILD_ROOT / "bin_dsp" / "libCMSISDSP.a"])


class BuildCMSISDSPExtensions(build_ext):
  def run(self):
    self.build_cmsisdsp_library()
    super().run()

  def build_cmsisdsp_library(self):
    library_path = cmsisdsp_library_path()
    candidates = built_cmsisdsp_library_candidates()
    existing = next((path for path in candidates if path.exists()), None)
    if existing is not None:
      return

    BUILD_ROOT.mkdir(parents=True, exist_ok=True)

    configure = [
      "cmake",
      "-S", str(here / "PythonWrapper"),
      "-B", str(BUILD_ROOT),
      f"-DCMSISDSP={here}",
      "-DHOST=YES",
      "-DWRAPPER=YES",
      "-DLOOPUNROLL=ON",
      "-DCMAKE_BUILD_TYPE=Release",
    ]

    if sys.platform != 'win32':
      configure.append("-DCMAKE_POSITION_INDEPENDENT_CODE=YES")

    if sys.platform == 'darwin' and ARM:
      configure.extend([
        "-DNEON=YES",
        "-DCMAKE_OSX_ARCHITECTURES=arm64",
        "-DCMAKE_OSX_DEPLOYMENT_TARGET=10.9",
      ])

    if shutil.which("ninja"):
      configure.extend(["-G", "Ninja"])

    subprocess.run(configure, check=True)
    subprocess.run([
      "cmake",
      "--build", str(BUILD_ROOT),
      "--config", "Release",
    ], check=True)

    existing = next((path for path in candidates if path.exists()), None)
    if existing is None:
      raise RuntimeError(f"CMSIS-DSP wrapper library was not generated in any expected location under {BUILD_ROOT / 'bin_dsp'}")

    if existing != library_path:
      library_path.parent.mkdir(parents=True, exist_ok=True)
      shutil.copyfile(existing, library_path)


def mkModule(name,srcs,funcDir):
  localinc = os.path.join(ROOT,"Source",funcDir)
 
  return(Extension(name,
                    sources = (srcs
                              )
                              ,
                    include_dirs =  [localinc] + includes + [numpy.get_include()],
                    extra_compile_args = cflags,
                    extra_link_args = linkargs,
                    libraries=[],
                    extra_objects=[str(cmsisdsp_library_path())]
                              ))

flagsForCommonWithoutFFT=[]

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
         author = 'Copyright (C) 2010-2025 ARM Limited or its affiliates. All rights reserved.',
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
                "Development Status :: 5 - Production/Stable",
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
         ,
         cmdclass={'build_ext': BuildCMSISDSPExtensions}
          )
       

build()
