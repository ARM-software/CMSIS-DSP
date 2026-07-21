# Overview

CMSIS-DSP is an optimized compute library for embedded systems (DSP is in the name for legacy reasons). It provides
optimized compute kernels for Arm Cortex-M and Cortex-A. Different variants are available according to the core and
most of the functions are using a vectorized version when vector instructions are available:

* Armv7-M DSP Extensions
* [Helium](https://developer.arm.com/Architectures/Helium) vector instructions
* [Neon](https://developer.arm.com/Architectures/Neon) vector instructions

Vector instructions can provide a huge boost to the performances of an algorithm.

This CMSIS-Pack contains the CMSIS-DSP library.

## CMSIS-DSP Kernels

Kernels provided by CMSIS-DSP (list not exhaustive):

* Basic mathematics (real, complex, quaternion, linear algebra, fast math functions)
* DSP (filtering like FIR, Biquad, IIR ...)
* Transforms (CFFT, RFFT, MFCC ...)
* Statistics
* Classical ML (Support vector machine, distance functions for clustering ...)

Kernels are provided with several data types: f64, f32, f16, q31, q15, q7.

## CMSIS-DSP and Python

A [Python package](https://pypi.org/project/cmsisdsp/) is available for CMSIS-DSP. It provides access to CMSIS-DSP functions (including fixed point) from Python with NumPy compatibility.

## Links

* [Documentation](https://arm-software.github.io/CMSIS-DSP)
* [Examples](https://github.com/ARM-software/CMSIS-DSP/tree/main/Examples)
* [Repository](https://github.com/ARM-software/CMSIS-DSP)
* [Issues](https://github.com/ARM-software/CMSIS-DSP/issues)
