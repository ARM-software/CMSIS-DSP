# Complex numbers {#dsppp_complex}

Complex numbers are supported. They benefit from vector instructions only with Helium. When only DSP extensions are available, a scalar version of the algorithm is used.

To use them, you just need to use the `std::complex` datatype.

For instance, you can use:

```C
std::complex<float32_t>
std::complex<float16_t>
std::complex<Q31>
std::complex<Q15>
```

`Q7` complex numbers have not been implemented because there is no equivalent function in the C CMSIS-DSP to compare with.

With `std::complex`, you can declare a vector with:

```C
Vector<std::complex<Q31>,10> a
```

Operator `conjugate` is available. 

If you want an hermitian product, you can just write:

```C
auto result = dot(a,conjugate(b));
```

Mixed complex / real arithmetic is supported but may not yet be very efficient. In some cases, it may be faster to convert the real object to complex before doing the computation.

It is a temporary limitation.

Note that there is not yet any function to get the real or imaginary part of a vector or matrix. You'll need to loop on the elements and use the `std::complex` functions. As consequence, those operations cannot yet participate into the fusion mechanism.

