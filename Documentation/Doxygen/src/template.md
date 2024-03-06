# What you need to know about C++ templates {#dsppp_template}

## What is a template useful for ?

In CMSIS-DSP, you have functions like:

* `arm_add_f32`
* `arm_add_f64`

Without unrolling, the scalar implementation is the same but is duplicated (two different source files to maintain although they are nearly the same).

One could try to reuse the same source for both functions using C preprocessor. But we would still have two different functions with different names at the end (both generated from the same C + C preprocessor macros)

With C++ templates, we can achieve the same result in a better way since the C++ compiler will check the templates and typecheck them. In addition to that, both functions can share the same name.

With C++ template, we could have a *generic* function `arm_add` taking as argument a pointer `T *pSrc` where `T` is a type variable !

When the function is used with a `float32_t *`, the compiler would generate code for a function using `float32_t`.

And if the function is used with a `float64_t *`, the compiler would generate code for a function using `float64_t`.

The generic `arm_add` source code is a template used to generate different implementations. It is like a code generator.

And if the compiler is unable to generate an implementation because the type variable `T` is replaced by a type with no addition operator, then it would be detected by the compiler.

Note that in C++, you can also use overloading of functions. They'll use the same name (but different arguments) but they won't share the same source code.

## Templates for datatypes

C++ templates also apply to structs and classes.

For instance, we could have a template `Vector<T>` and thus different types `Vector<float32_t>`, `Vector<Q15>` ...

There is another aspect of C++ templates that may be surprising : the types can contain numbers. 

For instance, one could have a type
`Vector<float32_t,10>` for a vector of `float` and of length `10`. The length being known at build time.

The types `Vector<float32_t,10>` and `Vector<float32_t,5>` should be considered as different types because they have different lengths. The length is part of the type.

What we said above for code generation applies. For a template algorithm using any kind of vector, the compiler would generate different code for different vector types. The code for a template algorithm using `Vector<float32_t,10>` would be different from the code for `Vector<float32_t,5>` because those two types are different.
    

## Implicit parameters

A template can also have implicit parameters.

For instance one could use `Vector<float32_t>` or `Vector<float32_t,10>`.

In the first case, the length is an implicit parameter with a default value and it is equivalent to writing `Vector<float32_t,DYNAMIC>` where `DYNAMIC` could be a special value (negative for instance) used to tell the compiler that the length of the vector is not known at build time but only at runtime.

Both variants may use totally different implementations. The `DYNAMIC` variant may contain a `length` field in the `struct` definition whereas other variants do not need this field since the length is known at build time.

## How to use templates ?

A template is just a C++ header. You only need to include this header to start using the template. There is nothing to build.

## Example

Now you can look at an @ref dsppp_vector_example "example with vector operations" showing how to use the library

