# Matrix {#dsppp_matrix}

Matrixes can be used similarly to vectors:

```cpp
Matrix<float32_t,ROWS,COLS> a;
Matrix<float32_t,ROWS,COLS> b;
```

If the dimensions of the matrixes are not known at build time, you would instead write:

```
Matrix<float32_t> a(rows,cols);
Matrix<float32_t> b(rows,cols);
```

Once you have matrixes, you need to initialize them.  A matrix is also a vector, so you can initialize it by indexing into the vector:

```cpp
for(std::size_t i=0;i<ROWS*COLS;i++)
{
      a[i] = float32_t(i);
}
```

You can also use a bidimensional indexing:

```cpp
for(std::size_t row=0; row<ROWS; row++)
{
   for(std::size_t col=0; col<COLS; col++)
   {
            b(row,col) = float32_t(row*col);
   }
}
```

Once you have initialized matrixes, you can operate on them:

```cpp
Matrix<float32_t,ROWS,COLS> result = a * a + b;
```

The operators `+` and `*` are merged into the loop. `*` is the element-wise multiply. For the vector / matrix products you should use the operator `dot`.

Note that fusion of operators will not work with `dot(Matrix, Matrix`). It is only supported with vectors : `dot(Vector,Vector)` or `dot(Matrix,Vector)`.

## VectorView

We can create virtual vectors which are view of some slices of the matrix.

### Row vector

To set the second row to `0.0f`, you can do:

```
result.row(1) = 0.0f;
```

To set the odd elements of the 3rd row to `0.0f` we can do:

```
result.row<2>(2,1) = 0.0f;
```

The first argument `2` is the row number (starting from `0`).

The second argument `1` is where is the row we start the view : element `1`.

`<2>` is the stride known at built time.

The `row` API is:

```cpp
template<int S=1>
VectorView<P,S> row(const index_t i,const index_t start=0,const index_t stop=C)
   
```

`stop` is the index of the first element **after** the end of the view.

`i` is the row index

### Column vector

There is a similar API for columns.

Let's set the odd elements of columns 3 to `5.0f`:

```
result.col<2>(2,1) = 5.0f;
```

## MatrixView

It is also possible to create a virtual matrix : a view onto a subset of the matrix.

Let's add the bottom right corner of the matrix to itself:

```cpp
result.sub(4,8,4,8) = result.sub(4,8,4,8) + result.sub(4,8,4,8)
```

The API is:

```cpp
MatrixView<P,C> sub(const index_t rs,
                    const index_t re,
                    const index_t cs,
                    const index_t ce)
```

You specify the row start and row end, then column start and column end.

Note that the end is the first index **after** the end of your rows or columns.

No stride is supported for matrix view in this version of the library.

## Matrix operations

In addition to the vector operations `+`,`-` and `*`, matrixes are supporting more operations:

* `dot` for vector / matrix products
* `diagonal` to create a diagonal matrix from a vector.
* `identity` to create an identity matrix
* `tranpose` to create the transposed matrix
* `outer` for the outer product of two vectors

### dot

```cpp
result = dot(a,b);
```

The compiler may use the move semantic to copy the temporary result of the `dot` function to `result`.

In this case, no copy would occur and `result` after the assignment would be a vector allocated by `dot` so using the `TMP_ALLOC` .

### diagonal

```cpp
result = Matrix<float32_t,ROWS,COLS>::diagonal(c);
```

### identity

```cpp
result = Matrix<float32_t,ROWS,COLS>::identity();
```

### transpose

```cpp
result = a.transpose();
```

or

```cpp
transposeTo(result,a);
```

### outer product

```cpp
result = outer(c,c);
```

