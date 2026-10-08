# DSP++ extension {#dsppp_main}

DSP++ is an entirely optional, header-only C++ extension to CMSIS-DSP.

Applications using only the C API are unaffected: no code or build changes are
needed, and the extension adds no code size, memory or runtime overhead.

**DSP++ is currently unsupported in builds with `ARM_MATH_NEON` enabled.**
On Cortex-A, enabling this macro to benefit from Neon in the C API also affects
DSP++: its incomplete Neon implementation can cause compilation errors, with
no automatic scalar fallback. Applications using only the C API can continue
to use Neon normally.

The headers are included in the CMSIS-DSP pack starting with version 1.18.0 and are also available in the [CMSIS-DSP GitHub repository](https://github.com/ARM-software/CMSIS-DSP/tree/main/dsppp/Include/dsppp). The extension itself requires no separate build: include the headers to use its C++ API with CMSIS-DSP.

* @subpage dsppp_intro "Introduction"
* @subpage dsppp_template "C++ template for C programmer"
* @subpage dsppp_vector_example "Vector operation example"
* @subpage dsppp_memory_allocator "Memory allocation"
* @subpage dsppp_memory_static_dynamic "Static / Dynamic objects"
* @subpage dsppp_code_size "Code size"
* @subpage dsppp_fusion "Fusion mechanism"
* @subpage dsppp_vector "Vector operators"
* @subpage dsppp_matrix "Matrix operators"
* @subpage dsppp_building "Building and running examples"
* @subpage dsppp_guidelines "Usage guidelines"
* @subpage dsppp_complex "Complex numbers"

