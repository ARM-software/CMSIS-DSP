# Guidelines {#dsppp_guidelines}

If you use dynamic objects in your algorithms and some temporaries need to be allocated, they'll generally be allocated through a `malloc` since the size is not known at build time. It can be an issue:

* Cost of the memory allocation
* Fragmentations

If you need to allocate those temporaries very often then it may be better to write the algorithm in such a way that the temporary can be reused between different calls.

The function implementing your algorithm would have additional arguments for the temporary matrixes and vectors required in the algorithm.

