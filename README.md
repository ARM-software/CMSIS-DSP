# CMSIS-DSP

![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/ARM-software/CMSIS-DSP?include_prereleases) ![GitHub](https://img.shields.io/github/license/ARM-software/CMSIS-DSP)


## About

CMSIS-DSP is an optimized compute library for embedded systems (DSP is in the name for legacy reasons).

It provides optimized compute kernels for Cortex-M and for Cortex-A.

Different variants are available according to the core and most of the functions are using a vectorized version when the Helium or Neon extension is available.

This repository contains the CMSIS-DSP library and several other projects:

* Test framework for bare metal Cortex-M or Cortex-A
* Examples for bare metal Cortex-M
* ComputeGraph
* PythonWrapper

You don't need any of the other projects to build and use CMSIS-DSP library. Building the other projects may require installation of other libraries (CMSIS), other tools (Arm Virtual Hardware) or CMSIS build tools.

### License Terms

CMSIS-DSP is licensed under [Apache License 2.0](LICENSE).

### CMSIS-DSP Kernels

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

With this wrapper you can design your algorithm in Python using an API as close as possible to the C API. The wrapper is compatible with NumPy. The wrapper is supporting fixed point arithmetic. This wrapper works in google colab.

The goal is to make it easier to move from a design to a final implementation in C.

## Support / Contact

For any questions or to reach the CMSIS-DSP  team, please create a new issue in https://github.com/ARM-software/CMSIS-DSP/issues

## Table of  content

* [Building for speed](#building-for-speed)
  * [Options to use](#options-to-use)
  * [Options to avoid](#options-to-avoid)
* [Half float support](#half-float-support)
* [How to build](#how-to-build)
  * [How to build with MDK or Open CMSIS-Pack](#how-to-build-with-mdk-or-open-cmsis-pack)
  * [How to build with Make](#how-to-build-with-make)
  * [How to build with cmake](#how-to-build-with-cmake)
  * [How to build with any other build system](#how-to-build-with-any-other-build-system)
  * [How to build for aarch64](#how-to-build-for-aarch64)
* [Code size](#code-size)
* [Folders and files](#folders-and-files)
  * [Folders](#folders)
  * [Files](#files)

## Building for speed

CMSIS-DSP is used when you need performance. As consequence CMSIS-DSP should be compiled with the options giving the best performance:

### Options to use

* `-Ofast` must be used for best performances.
* When using Helium it is strongly advised to use `-Ofast`
* `GCC` is currently not giving good performances when targeting Helium. You should use the Arm compiler

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

You can build CMSIS-DSP with the open CMSIS-Pack, or cmake, or Makefile and it is also easy to build if you use any other build tool.

### How to build with MDK or Open CMSIS-Pack

The standard way to build is by using the CMSIS pack technology. CMSIS-DSP is available as a pack.

This pack technology is supported by some IDE like [Keil MDK](https://www.keil.com/download/product/) or [Keil studio](https://www.keil.arm.com/).

You can also use those packs using the [Open CMSIS-Pack](https://www.open-cmsis-pack.org/) technology and from command line on any platform.

You should first install the tools from https://github.com/Open-CMSIS-Pack/devtools/tree/main/tools

You can get the CMSIS-Toolbox which is containing the package installer, cmsis build and cmsis project manager. Here is some documentation:

* Documentation about [CMSIS Build](https://open-cmsis-pack.github.io/devtools/buildmgr/latest/index.html)
* Documentation about [CMSIS Pack](https://open-cmsis-pack.github.io/Open-CMSIS-Pack-Spec/main/html/index.html)
* Documentation about [CMSIS Project manager](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/Overview.md)

Once you have installed the tools, you'll need to download the pack index using the `cpackget` tool.

Then, you'll need to convert a solution file into `.cprj`. For instance, for the CMSIS-DSP Examples, you can go to: 

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

### How to build with cmake

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

If you build for Helium, you should use any of the option `MVEF`, `MVEI` or `HELIUM`.

If you build for Neon, use `NEON` and/or `NEONEXPERIMENTAL`.

#### Launching the build

Once cmake has generated the makefiles, you can use a GNU Make to build.

    make VERBOSE=1

### How to build with any other build system

You need the following folders:

* Source
* Include
* PrivateInclude
* ComputeLibrary (only if you target Neon)

In `Source` subfolders, you may either build all of the source file with a datatype suffix (like `_f32.c`), or just compile the files without a datatype suffix. For instance for `BasicMathFunctions`, you can build all the C files except `BasicMathFunctions.c` and `BasicMathFunctionsF16.c`, or you can just build those two files (they are including all of the other C files of the folder).

`f16` files are not mandatory. You can build with `-DDISABLEFLOAT16`

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

* `-DHOST=ON`
* `-DNEON=ON`

cmake is automatically including the `ComputeLibrary` folder. If you are using a different build, you need to include this folder too to build with Neon support.

## Code size

Previous versions of the library were using compilation directives to control the code size. It was too complex and not available in case CMSIS-DSP is only delivered as a static library.

Now, the library relies again on the linker to do the code size optimization. But, this implies some constraints on the code you write and new functions had to be introduced.

If you know the size of your FFT in advance, use initializations functions like `arm_cfft_init_64_f32` instead of using the generic initialization functions `arm_cfft_init_f32`. Using the generic function will prevent the linker from being able to deduce which functions and tables must be kept for the FFT and everything will be included.

There are similar functions for RFFT, MFCC ...

If the flag `ARM_DSP_CONFIG_TABLES` is still set, you'll now get a compilation error to remind you that this flag no more have any effect on code size and that you may have to rework the initializations.

## Folders and files

The only folders required to build and use CMSIS-DSP Library are:

* Source
* Include
* PrivateInclude
* ComputeLibrary (only when using Neon)

Other folders are part of different projects, tests or examples.

### Folders

* cmsisdsp
  * Required to build the CMSIS-DSP PythonWrapper for the Python repository
  * It contains all Python packages
* ComputeLibrary:
  * Some kernels required when building CMSIS-DSP with Neon acceleration
* Examples:
  * Examples of use of CMSIS-DSP on bare metal Cortex-M
  * Require the use of CMSIS Build tools
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
* Source:
  * CMSIS-DSP source
* Testing:
  * CMSIS-DSP Test framework for bare metal Cortex-M and Cortex-A
  * Require the use of CMSIS build tools

### Files

Some files are needed to generate the PythonWrapper:

* PythonWrapper_README.md
* LICENSE
* MANIFEST.in
* pyproject.toml
* setup.py
