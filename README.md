# CMSIS-DSP

![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/ARM-software/CMSIS-DSP?include_prereleases) ![GitHub](https://img.shields.io/github/license/ARM-software/CMSIS-DSP)


## About

CMSIS-DSP is an optimized compute library for embedded systems (DSP is in the name for legacy reasons).

It provides optimized compute kernels for Cortex-M and for Cortex-A.

On Cortex-M, different variants are available according to the core and most of the functions are using a vectorized version when the Helium extension is available.


### Kernels

Kernels provided by CMSIS-DSP (list not exhaustive):

* Basic mathematics (real, complex, quaternion, linear algebra, fast math functions)
* DSP (filtering)
* Transforms (FFT, MFCC, DCT)
* Statistics 
* Classical ML (Support Vector Machine, Distance functions for clustering ...)

Kernels are provided with several datatypes : f64, f32, f16, q31, q15, q7.

### Python wrapper

A [PythonWrapper](https://pypi.org/project/cmsisdsp/) is also available and can be installed with:

`pip install cmsisdsp`

With this wrapper you can design your algorithm in Python using an API as close as possible to the C API. The wrapper is compatible with NumPy. The wrapper is supporting fixed point arithmetic.

The goal is to make it easier to move from a design to a final implementation in C.

### Synchronous Data Flow

CMSIS-DSP is also providing an experimental [synchronous data flow scheduler](SDFTools/README.md):

* You define your compute graph in Python
* A static schedule (computed by the Python script is generated)
* The static schedule can be run on the device

The scripts for the synchronous data flow (SDF) are part of the CMSIS-DSP Python wrapper. 

The SDF is making it easier to implement a streaming solution : connecting different compute kernels each consuming and producing different amount of data.

## Support / Contact

For any questions or to reach the CMSIS-DSP  team, please create a new issue in https://github.com/ARM-software/CMSIS-DSP/issues

## Building for speed

CMSIS-DSP is used when you need performance. As consequence CMSIS-DSP should be compiled with the options giving the best performance:

### Options to use

* `-Ofast` must be used for best performances.
* When using Helium it is strongly advised to use `-Ofast`

When float are used, then the fpu should be selected to ensure that the compiler is not using a software float emulation.

When building with Helium support, it will be automatically detected by CMSIS-DSP. For Neon, it is not the case and you must enable the option `-DARM_MATH_NEON` for the C compilation. With `cmake` this option is controlled with `-DNEON=ON`.

* `-DLOOPUNROLL=ON` can also be used when compiling with cmake
* It corresponds to the C options `-DARM_MATH_LOOPUNROLL`

Compilers are doing unrolling. So this option may not be needed but it is highly dependent on the compiler. With some compilers, this option is needed to get better performances.

Speed of memory is important. If you can map the data and the constant tables used by CMSIS-DSP in `DTCM` memory then it is better. If you have a cache, enable it.

### Options to avoid

* `-fno-builtin`
* `-ffreestanding` because it enables previous options

The library is doing some type [punning](https://en.wikipedia.org/wiki/Type_punning) to process word 32 from memory as a pair of `q15` or a quadruple of `q7`.  Those type manipulations are done through `memcpy` functions. Most compilers should be able to optimize out those function calls when the length to copy is small (4 bytes).

This optimization will **not** occur when `-fno-builtin` is used and it will have a **very bad** impact on the performances.

Some compiler may also require the use of option `-munaligned-access` to specify that unaligned accesses are used.

## How to build

The standard way to build is through the [Open CMSIS-Pack](https://www.open-cmsis-pack.org/) included in the repository (or available in your IDE).

But cmake can also be used.

### How to build CMSIS-DSP with cmake

Create a `CMakeLists.txt` and inside add a project.

Add CMSIS-DSP as a subdirectory. The variable `CMSISDSP` is the path to the CMSIS-DSP repository in below example.

```cmake
cmake_minimum_required (VERSION 3.14)

# Define the project
project (testcmsisdsp VERSION 0.1)

add_subdirectory(${CMSISDSP}/Source bin_dsp)
```

CMSIS-DSP is dependent on the CMSIS Core includes. So, you should use a `target_include_directories` to define where the `CMSIS_5\CMSIS\Core\Include` is located. Or you can also define `CMSISCORE` on the cmake command line.

You should also set the compilation options to use to build the library.

You can also rely on the CMSIS-DSP test framework. The framework will analyze the `ARM_CPU` option and deduce from its values the compilation options and the location of the CMSIS Core includes.

Note that the test framework is only supporting a subset of all the cores.

The following lines are:

* Adding the test framework to the cmake module path
* Loading the module `configLib`
* Defining the compilation options and core includes using the `configLib` function

`configLib` is requiring the variable `CMSIS` to be defined (on cmake command line) with the path to the CMSIS repository.

```cmake
list(APPEND CMAKE_MODULE_PATH "${CMSISDSP}/Testing")
include(configLib)
configLib(CMSISDSP)
```

A typical `cmake` command (when using CMSIS-DSP test framework) may be:

```
cmake -DCMAKE_PREFIX_PATH="path to compiler" \
  -DCMAKE_TOOLCHAIN_FILE="path_to_cmsisdsp/Testing/armac6.cmake" \
  -DARM_CPU="cortex-m55" \
  -DLOOPUNROLL=ON \
  -DCMSISDSP="path_to_cmsisdsp" \
  -DCMSIS="path_to_cmsis" \
  -DCMAKE_C_FLAGS_RELEASE="-std=c11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra" \
  -DCMAKE_CXX_FLAGS_RELEASE="-fno-rtti -std=c++11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra -Wno-unused-parameter" \
  -DHELIUM=ON \
  -G "Unix Makefiles" ..
```

It is also possible to build on the host PC:

```
cmake -DHOST=YES \
  -DLOOPUNROLL=ON \
  -DCMSISDSP="path_to_cmsisdsp" \
  -DCMSIS="path_to_cmsis" \
  -DCMAKE_C_FLAGS_RELEASE="-std=c11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra" \
  -DCMAKE_CXX_FLAGS_RELEASE="-fno-rtti -std=c++11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra -Wno-unused-parameter" \
  -G "Unix Makefiles" ..
```



### How to build CMSIS-DSP examples with cmake and CMSIS-DSP test framework

Building examples with cmake is similar to building only the CMSIS-DSP library but in addition to that we also rely on the CMSIS-DSP test framework for the boot code.

In addition to the `CMSIS` variable, the variable `CMSISDSPFOLDER` must also be defined.

`Examples/CMakeLists.txt` can be used to build all the examples.

A possible `cmake` command may be:

```
cmake -DLOOPUNROLL=ON \
      -DMATRIXCHECK=ON \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS_RELEASE="-Ofast -ffast-math -DNDEBUG -Wall -Wextra  " \
      -DCMAKE_CXX_FLAGS_RELEASE="-fno-rtti -Ofast -ffast-math -DNDEBUG -Wall -Wextra" \
      -DCMAKE_PREFIX_PATH="path to compiler" \
      -DCMAKE_TOOLCHAIN_FILE=../../Testing/armac6.cmake \
      -DCMSIS="path_to_cmsis" \
      -DCMSISDSPFOLDER="path_to_cmsisdsp" \
      -DARM_CPU="cortex-m55" \
      -DPLATFORM="FVP" \
      -DHELIUM=ON \
      -DFLOAT16=OFF \
      -DBASICMATH=ON \
      -DCOMPLEXMATH=ON \
      -DQUATERNIONMATH=ON \
      -DCONTROLLER=ON \
      -DFASTMATH=ON \
      -DFILTERING=ON \
      -DMATRIX=ON \
      -DSTATISTICS=ON \
      -DSUPPORT=ON \
      -DTRANSFORM=ON \
      -DSVM=ON \
      -DBAYES=ON \
      -DDISTANCE=ON \
      -DINTERPOLATION=ON \
      -G "Unix Makefiles" "../ARM"
```

### Building 

Once cmake has generated the makefiles, you can use a GNU Make to build.

    make VERBOSE=1

### Running

The generated executable can be run on a fast model. 
For instance, if you built for m7, you could just do:

    FVP_MPS2_Cortex-M7.exe -a arm_variance_example

The final executable has no extension in the filename. 

## Folders and files

### Folders

* cmsisdsp
  * Required to build the CMSIS-DSP PythonWrapper for the Python repository
  * It contains all Python packages
* ComputeLibrary:
  * Some kernels required when building CMSIS-DSP with Neon acceleration
* Examples:
  * Examples of use of CMSIS-DSP
* Include:
  * Include files for CMSIS-DSP
* PrivateInclude:
  * Some include needed to build CMSIS-DSP
* PythonWrapper:
  * C code for the CMSIS-DSP PythonWrapper
  * Examples for the PythonWrapper
* Scripts:
  * Debugging scripts
  * Script to generate some coefficient tables used by CMSIS-DSP
* SDFTools:
  * Examples for the Synchronous Data Flow
  * C++ templates for the Synchronous Data Flow
* Source:
  * CMSIS-DSP source
* Testing:
  * CMSIS-DSP Test framework

### Files

Some files are needed to generate the PythonWrapper:

* PythonWrapper_README.md
* LICENSE.txt
* MANIFEST.in
* pyproject.toml
* setup.py

And we have a script to make it easier to customize the build:

* cmsisdspconfig.py:
  * Web browser UI to generate build configurations (temporary until the CMSIS-DSP configuration is reworked to be simpler and more maintainable)


## Compilation symbols for tables

Some new compilations symbols have been introduced to avoid including all the tables if they are not needed.

If no new symbol is defined, everything will behave as usual. If `ARM_DSP_CONFIG_TABLES` is defined then the new symbols will be taken into account.

It is strongly suggested to use the new Python script `cmsisdspconfig.py` to generate the -D options to use on the compiler command line.

    pip install streamlit
    streamlit run cmsisdspconfig.py

If you use `cmake`, it is also easy since high level options are defined and they will select the right compilation symbols. 

For instance, if you want to use the `arm_rfft_fast_f32`, in` fft.cmake` you'll see an option `RFFT_FAST_F32_32`.

If you don't use cmake nor the Python script, you can just look at `fft.cmake` or `interpol.cmake` in `Source` to see which compilation symbols are needed.

We see, for `arm_rfft_fast_f32`, that the following symbols need to be enabled :

* `ARM_TABLE_TWIDDLECOEF_F32_16 `
* `ARM_TABLE_BITREVIDX_FLT_16`
* `ARM_TABLE_TWIDDLECOEF_RFFT_F32_32`
* `ARM_TABLE_TWIDDLECOEF_F32_16`

In addition to that, `ARM_DSP_CONFIG_TABLES` must be enabled and finally `ARM_FFT_ALLOW_TABLES` must also be defined.

This last symbol is required because if no transform functions are included in the build, then by default all flags related to FFT tables are ignored.


## Bit Reverse Tables for FFTs in CMSIS DSP

It is a question coming often.

It is now detailed [in this github issue](https://github.com/ARM-software/CMSIS_5/issues/858)

Someone from the community has written a [Python script to help](https://gist.github.com/rosek86/d0d709852fddf36193071d7f61987bae)

