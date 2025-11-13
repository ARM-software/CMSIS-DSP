# Overview

CMSIS-DSP is an optimized compute library for embedded systems (DSP is in the name for legacy reasons). It provides
optimized compute kernels for Arm Cortex-M and for Cortex-A. Different variants are available according to the core and
most of the functions are using a vectorized version when the [Helium](https://developer.arm.com/Architectures/Helium)
or [Neon](https://developer.arm.com/Architectures/Neon) extension is available.

This CMSIS-Pack contains the CMSIS-DSP library and several other projects:

- Test framework for bare-metal usage on Arm Cortex-M or Arm Cortex-A
- Bare-metal examples for Arm Cortex-M
- [PythonWrapper](https://pypi.org/project/cmsisdsp/)

## CMSIS-DSP Kernels

Kernels provided by CMSIS-DSP (list not exhaustive):

- Basic mathematics (real, complex, quaternion, linear algebra, fast math functions)
- DSP (filtering)
- Transforms (FFT, MFCC, DCT)
- Statistics
- Classical ML (Support vector machine, distance functions for clustering ...)

Kernels are provided with several data types: f64, f32, f16, q31, q15, q7.

## Links

- [Documentation](https://arm-software.github.io/CMSIS-DSP)
- [Examples](https://github.com/ARM-software/CMSIS-DSP/tree/main/Examples)
- [Repository](https://github.com/ARM-software/CMSIS-DSP)
- [Issues](https://github.com/ARM-software/CMSIS-DSP/issues)
