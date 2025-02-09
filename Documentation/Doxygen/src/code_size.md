# Code size {#dsppp_code_size}

It was explained in previous sections that types `Vector<T,NB1>` and `Vector<T,NB2>` are considered as different types if `NB1` and `NB2` are different.

A template algorithm is like a code generator that will generate different code for different values of the template arguments : the types.

If you use a template algorithm with different vector datatypes, it will generate different code for those two datatypes. The generated code will be specialized for the specific datatypes used and thus is likely to be more efficient.

But then it means you get different implementations so more code size.

If you have a lot of different sizes in your system, then you're likely to get too much code size and in that case it may be better to use dynamic objects instead of static ones.

dynamic objects are less efficient so it is a trade-off between code size / speed.

