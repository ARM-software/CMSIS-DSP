# Overview {#mainpage}

## Introduction

This user manual describes the CMSIS DSP software library, a suite of common compute processing functions for use on Cortex-M and Cortex-A processor based devices.

The library is divided into a number of functions each covering a specific category:

 - \ref groupMath "Basic math functions"
 - \ref groupFastMath "Fast math functions"
 - \ref groupCmplxMath "Complex math functions"
 - \ref groupFilters "Filtering functions"
 - \ref groupMatrix "Matrix functions"
 - \ref groupTransforms "Transform functions"
 - \ref groupController "Motor control functions"
 - \ref groupStats "Statistical functions"
 - \ref groupSupport "Support functions"
 - \ref groupInterpolation "Interpolation functions"
 - \ref groupSVM "Support Vector Machine functions (SVM)"
 - \ref groupBayes "Bayes classifier functions"
 - \ref groupDistance "Distance functions"
 - \ref groupQuaternionMath "Quaternion functions"
 - \ref groupWindow "Window functions"

The library has generally separate functions for operating on 8-bit integers, 16-bit integers, 32-bit integer and 32-bit floating-point values and 64-bit floating-point values.

The library is providing vectorized versions of most algorithms for Helium and of most f32 algorithms for Neon.

When using a vectorized version, provide a little bit of padding after the end of a buffer (3 words) because the vectorized code may read a little bit after the end of a buffer. You don't have to modify your buffers but just ensure that the end of buffer + padding is not outside of a memory region.

## Related projects

### Python wrapper

A Python wrapper is also available with a Python API as close as possible to the C one. It can be used to start developing and testing an algorithm with NumPy and SciPy before writing the C version. Is is available on [PyPI.org](https://pypi.org/project/cmsisdsp/). It can be installed with: `pip install cmsisdsp`.

### Experimental C++ template extension

This extension is a set of C++ headers. They just need to included to start using the features.

Those headers are not yet part of the pack and you need to get them from the [github repository](https://github.com/ARM-software/CMSIS-DSP/tree/main/dsppp/Include)

More documentation about the @ref dsppp_main "DSP++" extension.

## Using the CMSIS-DSP Library {#using}

The library is released in source form. It is strongly advised to compile the library using `-Ofast` optimization to have the best performances.

The library functions are declared in the public file `Include/arm_math.h`. Simply include this file to use the CMSIS-DSP library. If you don't want to include everything, you can also rely on individual header files from the `Include/dsp/` folder and include only those that are needed in the project.

## Examples {#example}

The library ships with a number of examples which demonstrate how to use the library functions. Please refer to \ref groupExamples.

## Toolchain Support {#toolchain}

The library is now tested on Fast Models building with cmake. Core M0, M4, M7, M33, M55, A32 are tested.

## Access to CMSIS-DSP {#pack}

CMSIS-DSP is actively maintained in the [**CMSIS-DSP GitHub repository**](https://github.com/ARM-software/CMSIS-DSP) and is released as a standalone [**CMSIS-DSP pack**](https://www.keil.arm.com/packs/cmsis-dsp-arm/versions/) in the [CMSIS-Pack format](https://open-cmsis-pack.github.io/Open-CMSIS-Pack-Spec/main/html/index.html).

The table below explains the content of **ARM::CMSIS-DSP** pack.

 Directory                             | Description
:--------------------------------------|:------------------------------------------------------
 📂 ComputeLibrary                     | Small Neon kernels when building on Cortex-A
 📂 Documentation                      | Folder with this CMSIS-DSP documenation
 📂 Example                            | Example projects demonstrating the usage of the library functions
 📂 Include                            | Include files for using and building the lib
 📂 PrivateInclude                     | Private include files for building the lib
 📂 Source                             | Source files
 📂 dsppp                              | Experimental C++ teamplate extension
 📄 ARM.CMSIS-DSP.pdsc                 | CMSIS-Pack description file
 📄 LICENSE                            | License Agreement (Apache 2.0)

See [CMSIS Documentation](https://arm-software.github.io/CMSIS_6/) for an overview of CMSIS software components, tools and specifications.


## Preprocessor Macros {#preprocessor}

Each library project has different preprocessor macros.

 - `ARM_MATH_BIG_ENDIAN`:
   - Define macro ARM_MATH_BIG_ENDIAN to build the library for big endian targets. By default library builds for little endian targets.

 - `ARM_MATH_MATRIX_CHECK`:
   - Define macro ARM_MATH_MATRIX_CHECK for checking on the input and output sizes of matrices

 - `ARM_MATH_ROUNDING`:
   - Define macro ARM_MATH_ROUNDING for rounding on support functions

 - `ARM_MATH_LOOPUNROLL`:
   - Define macro ARM_MATH_LOOPUNROLL to enable manual loop unrolling in DSP functions

 - `ARM_MATH_NEON`:
   - Define macro ARM_MATH_NEON to enable Neon versions of the DSP functions. It is not enabled by default when Neon is available because performances are dependent on the compiler and target architecture.

 - `ARM_MATH_NEON_EXPERIMENTAL`:
   - Define macro ARM_MATH_NEON_EXPERIMENTAL to enable experimental Neon versions of of some DSP functions. Experimental Neon versions currently do not have better performances than the scalar versions.

 - `ARM_MATH_HELIUM`:
   - It implies the flags ARM_MATH_MVEF and ARM_MATH_MVEI and ARM_MATH_MVE_FLOAT16.

 - `ARM_MATH_HELIUM_EXPERIMENTAL`:
   - Only taken into account when ARM_MATH_MVEF, ARM_MATH_MVEI or ARM_MATH_MVE_FLOAT16 are defined. Enable some vector versions which may have worse performance than scalar depending on the core / compiler configuration.

 - `ARM_MATH_MVEF`:
   - Select Helium versions of the f32 algorithms. It implies ARM_MATH_FLOAT16 and ARM_MATH_MVEI.

 - `ARM_MATH_MVEI`:
   - Select Helium versions of the int and fixed point algorithms.

 - `ARM_MATH_MVE_FLOAT16`:
   - MVE Float16 implementations of some algorithms (Requires MVE extension).

 - `DISABLEFLOAT16`:
   - Disable float16 algorithms when __fp16 is not supported for a specific compiler / core configuration. This is only valid for scalar. When vector architecture is supporting f16 then it can't be disabled.

 - `ARM_MATH_AUTOVECTORIZE`:
   - With Helium or Neon, disable the use of vectorized code with C intrinsics and use pure C instead. The vectorization is then done by the compiler.

## License {#license}

The CMSIS-DSP is provided free of charge under the [Apache 2.0 License](https://raw.githubusercontent.com/ARM-software/CMSIS-DSP/main/LICENSE).
