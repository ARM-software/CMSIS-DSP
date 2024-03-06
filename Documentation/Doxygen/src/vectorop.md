# Vector operation example {#dsppp_vector_example}

To compute:

\f[

\overrightarrow{d} = \overrightarrow{a} + \overrightarrow{b} * \overrightarrow{c}

\f]

we need to:
1. Include the right header files
2. allocate the vectors
3. initialize the vectors
4. make the computation.

# Include the headers

The headers are not yet part of the CMSIS-DSP packs since they are experimental. You can get them from the [CMSIS-DSP github](https://github.com/ARM-software/CMSIS-DSP/tree/main/dsppp/Include/dsppp)

```cpp
#include <dsppp/memory_pool>
#include <dsppp/matrix>

using namespace arm_cmsis_dsp;
```

If fixed point datatypes are required, `#include <dsppp/fixed_point>` should be used before `<dsppp/matrix>`

Fixed point requires the use of CMSIS-DSP.

# Creation of the vectors

To create a vector `a` you would write:

```cpp
constexpr int NB = 32;

Vector<float32_t,NB> a;
Vector<float32_t,NB> b;
Vector<float32_t,NB> c;
```

`Vector<float32_t,NB>` is creating a vector of dimension `NB` (known at build time) and datatype `float32_t`. This creation is requiring some memory allocation and by default it is done with a `malloc`. 

It is possible to change the memory allocator for the vectors (and it is advised) to avoid using `malloc` and instead have deterministic allocation without fragmentation.

See section @ref dsppp_memory_allocator "Memory allocation".

Vectors of different dimensions are considered as being different types.

If you don't know the dimension at build time, you can use a different type of vector with:

```cpp
Vector<float32_t> a(NB);
```

For the trade-off between vector with build time dimension or runtime dimension please see the section @ref dsppp_memory_static_dynamic .

# Initialization of the vectors

You can index the vectors as normal C arrays.

```cpp
for(int i = 0;i< NB;i++)
{
        a[i] = b[i] = c[i] = i;
}
```

# Computation

The computation can be written normally as :

```cpp
Vector<float32_t,NB> d = a + b * c;
```

Note that the computation can be parametrized with template arguments so the same computation could be used with any datatype or length. In that case you would have to define a template (and not just a normal function) and inside you would use something like:

```cpp
Vector<T,NB> d = a + b * c;
```

where `T` is a type variable coming from the template.

The operators `+`, `*` are computed in one pass with one loop : we have loop fusion and instead of having a loop per operator we have a loop for the whole computation.

To understand fusion and how to extend it with new operators, see section @ref dsppp_fusion .

For an overview of vector operators, see section @ref dsppp_vector .
For an overview of matrix operators, see section @ref dsppp_matrix .

# Displaying the result

The vectors can be displayed on `stdout` for debug purpose.

```cpp
std::cout << "Result = " << d ;
```
