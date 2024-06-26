## Introduction {#dsppp_intro}

### Dot product example

If you want to compute the dot product:

\f[

<scale*(\overrightarrow{a}+\overrightarrow{b}),\overrightarrow{c}*\overrightarrow{d}>

\f]

with CMSIS-DSP, you would write:

```c
arm_add_f32(a,b,tmp1,NB);
arm_scale_f32(tmp1,scale,tmp2,NB);
arm_mult_f32(c,d,tmp3,NB);
arm_dot_prod_f32(tmp2,tmp3,NB,&r);
```

There are several limitations with this way of writing the code:

1. The code needs to be rewritten and the `_f32` suffix changed if the developer wants to use another datatype

2. Temporary buffers need to be allocated and managed (`tmp1`,`tmp2`,`tmp3`,`tmp4`)

3. The four function calls are four different loops. It is not good for data locality and caches. The computation is not done in one pass

4. Each loop contains a small number of instructions. For instance, for the `arm_add_f32`, two loads, an add instruction and a store. It is not enough to enable the compiler to reorder the instructions to improve the performance

With this new C++ template library, you can write:


```cpp
r = dot(scale*(a+b),c*d);
```

The code generated by this line computes the dot product in one pass with all the operators (`+`, `*`) included in the loop.
There is no more any temporary buffers.

### Vector operations

Let's look at another example:

\f[

\overrightarrow{d} = \overrightarrow{a} + \overrightarrow{b} * \overrightarrow{c}

\f]

With the C++ library, it can be written as:


```cpp
Vector<float32_t,NB> d = a + b * c;
```

Here again : all the vector operations (`+`,`*`) are done in one pass with one loop. There is no more any temporary buffer.

If you're coming from C and does not know anything about C++ templates, we have a very quick introduction : @ref dsppp_template "The minimum you need to know about C++ template to use this library".

You can also jump directly to an @ref dsppp_vector_example "example with vector operations".

