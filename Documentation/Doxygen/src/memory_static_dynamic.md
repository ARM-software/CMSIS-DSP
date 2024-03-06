# Static / dynamic {#dsppp_memory_static_dynamic}

As we have seen in the previous sections, there are two kind of vectors:

* `Vector<T>` with a dimension known at runtime
* `Vector<T,NB>` with a dimension known at build time

The former vectors are called "dynamic" in this library. The later are called "static". 

This naming "static" / "dynamic" is referring to the dimension. With "dynamic" vectors the same code can, at runtime, create vectors of different length based on a runtime length. 

With "static" vectors : the length is fixed at build time and will never change at runtime.

Note that the library also have "static" / "dynamic" matrixes. So, we are going to use the name "object" to cover both cases in the below explanations.

# Static objects

The advantage of static objects is that the dimension is known at build time. The compiler can thus generate an algorithm that is specialized for those dimensions and thus more efficient.

With static objects it is also possible to use different memory allocator with better performances and determinism.

But, with static objects, objects of different dimension are considered as different types. The compiler will generate different implementation so it will have an impact on the code size.

If you need lots of objects of different dimensions, or if the dimensions are not known at build time, then you need to use dynamic object

# Dynamic objects

With dynamic objects, the dimension is know at runtime. So object of different dimensions have the same datatype and the compiler is generating only one implementation for all those objects. It cannot generate specialized implementations based on the dimension. It is better for code size, but the implementations will be less efficient.

Also when dimension is not know at build time, some instruction selection made by the C++ library at build time is no more possible. It has an effect on performance since at runtime one must decides what's possible or not. It is mostly impacting matrixes where stride information is needed.

With vector instructions one can use scatter / gather instructions and they require a stride. But there are constraints depending on the datatype and when the stride is too big for those instructions, they cannot be used. This check has to be done at runtime for dynamic object.

Finally, with dynamic object, memory allocation can be an issue. You can mitigate the problem by reusing temporaries in your algorithms instead of re-allocating them. But it makes the implementation more difficult. See section about @ref dsppp_guidelines.

