# Fusion {#dsppp_fusion}

```cpp
Vector<float32_t,NB> d = a + b * c;
```

With this line of code, there is loop fusion : instead of having one loop per operator there is one loop for the whole computation.

It is important to have some ideas of how it works to avoid some mistake in the use of the library.

In above code, `a + b * c` is not computing anything !
`a + b * c` is creating a representation of the expression : an abstract syntax tree (AST) at build time.

When this AST is assigned to the variable `d` it is evaluated.
The evaluation forces the inlining of the expression operators in one loop. The code generated thus contains only one loop with a fusion of all the operators : `+` and `*`.

The library is supporting virtual vectors. They are a view on an existing part of a vector. You can use a virtual vector for instance to read some samples with a stride. Or write some samples with a stride. A virtual vector does not own its memory.

If you write:
```cpp
d = a;
```

and `d` and `a` are virtual vectors then nothing will be written to `d` !

`d` will becomes `a` and `a` will no more be valid.

If you want to copy a virtual vector you need to make an expression and write:

```cpp
d = copy(a);
```

Note that this problem occurs only for virtual vectors who do not own their memory.

For real vectors, a copy would occur. But since there is no overhead in adding `copy` it is better to do it to avoid problems.



