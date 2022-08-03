# CMSIS-DSP

![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/ARM-software/CMSIS-DSP?include_prereleases) ![GitHub](https://img.shields.io/github/license/ARM-software/CMSIS-DSP)


## About

CMSIS-DSP is an optimized compute library for embedded systems (DSP is in the name for legacy reasons).

It provides optimized compute kernels for Cortex-M and for Cortex-A.

Different variants are available according to the core and most of the functions are using a vectorized version when the Helium or Neon extension is available.


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

### Synchronous Data Flow (SDF)

CMSIS-DSP is also providing an experimental [synchronous data flow scheduler](SDFTools/README.md):

* You define your compute graph in Python
* A static schedule (computed by the Python script) is generated
* The static schedule can be run on the device with very low overhead

The Python scripts for the synchronous data flow (SDF) are part of the CMSIS-DSP Python wrapper. 

The header files are part of the CMSIS-DSP pack (version 1.10.2 and above).

The audio streaming nodes on top of CMSIS-RTOS2 are not part of the CMSIS-DSP pack but can be found in the repository. They are demo quality only. They can be used with Arm Virtual Hardware.

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

## Half float support

`f16` data type (half float) has been added to the library. It is useful only if your Cortex has some half float hardware acceleration (for instance with Helium extension). If you don't need `f16`, you should disable it since it may cause compilation problems. Just define `-DDISABLEFLOAT16` when building.

## How to build

### Building with MDK or Open CMSIS-Pack

The standard way to build is by using the CMSIS pack technology. CMSIS-DSP is available as a pack.

This pack technology is supported by some IDE like [Keil MDK](https://www.keil.com/download/product/) or [Keil studio](https://www.keil.arm.com/).

You can also use those packs using the [Open CMSIS-Pack](https://www.open-cmsis-pack.org/) technology and from command line on any platform.

cmake can also be used to build CMSIS-DSP.

### How to build with Open CMSIS-Pack

You should first install the tools from https://github.com/Open-CMSIS-Pack/devtools

You can get the CMSIS-Toolbox which is containing the package installer, cmsis build and cmsis project manager. Here is some documentation:

* Documentation about [CMSIS Build](https://open-cmsis-pack.github.io/devtools/buildmgr/latest/index.html)
* Documentation about [CMSIS Pack](https://open-cmsis-pack.github.io/Open-CMSIS-Pack-Spec/main/html/index.html)
* Documentation about [CMSIS Project manager](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/Overview.md#software-layers)

Once you have installed the tools, you'll need to download the pack index using the `cpackget` tool.

Then, you'll need to convert the solution file into `.cprj`. For instance, for the CMSIS-DSP Examples, you can go to: 

`Examples/cmsis_build` 

and then type 

`csolution convert -s examples.csolution_ac6.yml`

This command processes the `examples.csolution_ac6.yml` describing how to build the examples for several platforms. It will generate lots of `.cprj` files that can be built with `cbuild`.

If you want to build the `FFT` example for the `Corstone-300` virtual hardware platform, you could just do:

`cbuild "fftbin.Release+VHT-Corstone-300.cprj"`

### How to build with Make

There is an example `Makefile` in `Source`.

In each source folder (like `BasicMathFunctions`), you'll see files with no `_datatype` suffix (like `BasicMathFunctions.c` and `BasicMathFunctionsF16.c`).

Those files are all you need in your makefile. They are including all other C files from the source folders.

Then, for the includes you'll need to add the paths: `Include`, `PrivateInclude` and, since there is a dependency to CMSIS Core, `Core/Include` from `CMSIS_5/CMSIS`.

If you are building for `Cortex-A` and want to use Neon, you'll also need to include `ComputeLibrary/Include` and the source file in `ComputeLibrary/Source`.

### How to build CMSIS-DSP with cmake

Create a `CMakeLists.txt` and inside add a project.

Add CMSIS-DSP as a subdirectory. The variable `CMSISDSP` is the path to the CMSIS-DSP repository in below example.

```cmake
cmake_minimum_required (VERSION 3.14)

# Define the project
project (testcmsisdsp VERSION 0.1)

add_subdirectory(${CMSISDSP}/Source bin_dsp)
```

CMSIS-DSP is dependent on the CMSIS Core includes. So, you should define `CMSISCORE` on the cmake command line. The path used by CMSIS-DSP will be `${CMSISCORE}/Include`.

You should also set the compilation options to use to build the library.

#### Launching the build

Once cmake has generated the makefiles, you can use a GNU Make to build.

    make VERBOSE=1



### How to build for aarch64

The intrinsics defined in `Core_A/Include` are not available on recent Cortex-A processors.

But you can still build for those Cortex-A cores and benefit from the Neon intrinsics.

You need to build with `-D__GNUC_PYTHON__` on the compiler command line. This flag was introduced for building the Python wrapper and is disabling the use of CMSIS Core includes.

When this flag is enabled, CMSIS-DSP is defining a few macros used in the library for compiler portability:

```C
#define  __ALIGNED(x) __attribute__((aligned(x)))
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline)) 
#define __STATIC_INLINE static inline
```

If the compiler you are using is requiring different definitions, you can add them to `arm_math_types.h` in the `Include` folder of the library. MSVC and XCode are already supported and in those case, you don't need to define `-D__GNUC_PYTHON__`

Then, you need to define `-DARM_MATH_NEON`

For cmake the equivalent options are:

* -DHOST=ON
* -DNEON=ON

cmake is automatically including the `ComputeLibrary` folder. If you are using a different build, you need to include this folder too to build with Neon support.

### Running

The generated executable can be run on a fast model. 
For instance, if you built for m7, you could just do:

    FVP_MPS2_Cortex-M7.exe -a arm_variance_example

The final executable has no extension in the filename. 

Of course, on your fast model or virtual hardware you should use the right configuration file (to enable float, to enable FVP, to enable semihosting if needed for the examples ...)

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

