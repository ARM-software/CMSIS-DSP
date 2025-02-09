# Vector {#dsppp_vector}

The use of vectors has been explained in @ref dsppp_vector_example "example with vector operations" and focusing on `float32_t`.

The vector template is defined as:

```cpp
template<typename P,
         int L=DYNAMIC,
         template<int> typename Allocator = TMP_ALLOC>
struct Vector:Vector_Base<P>
```

* `P` is the datatype of vector elements
* `L` is the static length of the vector (length known at build time). `L<0` when the length is dynamic and not known at build time. It is the default value.
* `Allocator` is the memory allocator. By default it is `TMP_ALLOC` that you can redefine since it is a macro
* `Vector_Base<P>` is providing the storage. A vector owns its storage buffer.

## Q15 example

Example with `Q15` is very similar:

The vectors are defined:

```cpp
Vector<Q15,NB> aQ15;
Vector<Q15,NB> bQ15;
Vector<Q15,NB> cQ15;
```

They are initialized:

```cpp
for(int i = 0;i< NB;i++)
{
   aQ15[i] = bQ15[i] = cQ15[i] = Q15(i);
}
```

Here, the `Q15` value is initialized from the int value `i` and thus represents \f$ i/2^{15} \f$

Some computation is done

```cpp
Vector<Q15,NB> dQ15 = aQ15 + bQ15 * cQ15;
```

The result is displayed:

```cpp
std::cout << "Result = " << dQ15 ;
```

## VectorView

A vector view is a virtual vector : a view of a vector.

One can define a `VectorView` with:

```cpp
auto subD = d.sub(2);
```

This is creating a virtual vector starting at index `2` (3rd element) of vector `d`.

You can then operate with this virtual vector:

```cpp
subD = subD + 2.0f;
```

If you display the vector `d`, you'll see that `2.0f` has been added to all elements starting from the 2nd one.

`VectorView` do not own their memory. It is owned by the original vector. 

If you write:

```cpp
x = y
```

and `x` and `y` are `VectorView`, no copy will occur. `x` will just reference the same data as `y`. If you want to copy you have to be explicit and write:

```cpp
x = copy(y)
```

It is advised to always use the `copy` operator (even with normal vectors).

Virtual vectors can have a stride:

```cpp
d.sub<2>(1) = 0.0f;
```

This line sets the odd elements of the vector to `0.0f`. It is creating a virtual vector with stride `2` and starting at index `1` of first vector.

Then, all elements of this virtual vector are set to `0.0f`.

The `sub` API is:

```cpp
template<int S=1>
VectorView<P,S> sub(const index_t start=0,const index_t stop=L)
```

You can define:

* The stride `S` : statically known and by default `1`.
* The start of the view (`0` by default)
* The end of the view (`L` by default : the length known at build time). Note that it is the first index **after** the end of the vector.

