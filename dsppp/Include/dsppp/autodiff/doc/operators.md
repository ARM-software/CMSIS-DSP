# Operators

Each operator header owns its validation, forward computation, fixed-size tape
record, gradient reset, backward rule, and expression adapter. Operator classes
are templated on the scalar type (`float` by default and `float16_t` when
enabled). Include the header and register the matching specialization before
evaluating the expression.

In the formulas below, `g` is the gradient arriving from the operator's output.
Every input gradient is accumulated with `+=` when that input has gradient
storage. A view created by `tape.input()` has none, so its contribution is
skipped.

## Element-wise arithmetic

| Expression | Forward | Backward |
| --- | --- | --- |
| `z = x + y` | `z[i] = x[i] + y[i]` | `dx[i] += g[i]`; `dy[i] += g[i]` |
| `z = x - y` | `z[i] = x[i] - y[i]` | `dx[i] += g[i]`; `dy[i] -= g[i]` |
| `z = x * y` | `z[i] = x[i] * y[i]` | `dx[i] += g[i]*y[i]`; `dy[i] += g[i]*x[i]` |

These expressions require equal-length views. The forward paths dispatch to
the matching f32 or f16 CMSIS-DSP kernels. Multiply's backward path uses fused
CMSIS-DSP C++ expressions so multiplication and accumulation need no temporary
product vector.

## Dot, scalar scale, and scalar offset

`dot(x, y)` returns one value:

```text
z = sum(x[i] * y[i])
dx[i] += g * y[i]
dy[i] += g * x[i]
```

Its forward pass dispatches to `arm_dot_prod_f32` or `arm_dot_prod_f16`.

`scale(x, a)` requires `a` to be a one-element parameter:

```text
z[i] = a * x[i]
da += sum(g[i] * x[i])
dx[i] += a * g[i]
```

The forward pass dispatches to `arm_scale_f32` or `arm_scale_f16`; the scalar
gradient uses the C++ dot expression. See the [worked implementation
flow](implementation_flow.md) for a line-by-line explanation.

`offset(x, b)` likewise requires a one-element parameter:

```text
z[i] = x[i] + b
db += sum(g[i])
dx[i] += g[i]
```

Its forward pass dispatches to `arm_offset_f32` or `arm_offset_f16`; the bias
gradient uses the matching accumulate kernel.

## ReLU and softmax

ReLU computes `y[i] = max(0, x[i])` with the matching f32 or f16 clip kernel.
Its backward rule passes `g[i]` only when the saved input value is strictly
positive. The derivative at zero is defined as zero.

Softmax uses a log-sum-exp forward calculation for numerical stability:

```text
y[i] = exp(x[i] - log(sum(exp(x))))
projection = dot(g, y)
dx[i] += y[i] * (g[i] - projection)
```

The forward path dispatches to the matching f32 or f16 log-sum-exp, offset, and
vector-exponential kernels.

## Losses

Quadratic error returns a scalar sum, not a mean:

```text
loss = sum((prediction[i] - target[i])^2)
d_prediction[i] += g * 2 * (prediction[i] - target[i])
```

Categorical cross entropy also returns a scalar sum. The probability floor is
`1e-7` for float32 and `1e-4` for float16; the larger half-precision floor keeps
the reciprocal used by the derivative finite:

```text
p_safe[i] = max(probability[i], floor)
loss = -sum(target[i] * log(p_safe[i]))
d_probability[i] = -g * target[i] / p_safe[i]
```

For both losses, the target must be an input view. Cross entropy is intended
for a probability vector from softmax and a one-hot target. The probability
floor prevents non-finite loss and gradient values. Its forward pass reuses the
probability gradient buffer as scratch; `backward()` resets that buffer before
propagating gradients.

## Fully connected

`fully_connected(x, W, b)` computes `y = W*x + b`, where `W` is a row-major
matrix parameter and `b` is a parameter vector:

```text
dW += outer(g, x)
db += g
dx += transpose(W) * g
```

The number of columns in `W` must equal the input length; its rows must equal
the bias and output lengths. Dimensions must fit the `uint16_t` CMSIS-DSP C
matrix descriptor.

The forward matrix-vector product dispatches to `arm_mat_vec_mult_f32` or
`arm_mat_vec_mult_f16`, followed by a fused C++ bias accumulation. Backward bias
and outer-product updates are fused C++ expressions. If `x` needs a gradient, the current implementation evaluates
the lazy expression `dot(transpose_view(W), g)` and accumulates it into `dx`.
The transpose is a view: no transposed numerical matrix is allocated.

## Matrix multiply

`matrix_multiply(X, W)` computes `Y = W*X`. `W` is a row-major parameter matrix;
`X` is a row-major input matrix flattened into an input `BufferView`. If `W`
has shape `rows x inner`, the input length must be divisible by `inner`, and
the inferred input shape is `inner x columns`. The output length is
`rows*columns`.

Only `W` is differentiated:

```text
dW += dY * transpose(X)
```

The forward pass dispatches to `arm_mat_mult_f32` or `arm_mat_mult_f16`. In the
backward pass, each row of `dW` is accumulated with the lazy C++ expression
`matvec(X, corresponding_row_of_dY)`. This fuses matrix-vector evaluation with
gradient accumulation; it does not call a scalar dot kernel once per weight and
does not materialize `transpose(X)`.

## Dropout

Dropout is a training regularizer. With drop probability `p`, each element is
kept independently with probability `1-p`. **Inverted dropout** scales kept
values during training:

```text
mask[i] is 1 with probability (1-p), otherwise 0
scale = 1 / (1-p)
y[i] = mask[i] * x[i] * scale
dx[i] += mask[i] * g[i] * scale
```

This is the standard inverted-dropout convention. Its expected training output
is `x`, so inference can be an identity operation without an extra scale:

```text
E[y[i]] = (1-p) * x[i] / (1-p) = x[i]
```

The probability must satisfy `0 <= p < 1`. At `p == 0`, forward and backward
are identity operations. When tape recording is disabled, dropout copies its
input unchanged, which implements inference behavior.

Random state is explicit and caller-owned:

```cpp
DropoutGenerator generator(1234U);
tape.register_operator<DropoutOperator<float>>();
hidden = dropout(hidden_linear, generator, 0.2F);
```

Initialize the generator once with a nonzero seed for a reproducible sequence.
A zero seed is replaced with the class's nonzero default because the xorshift
generator would otherwise remain zero. Each forward record saves the initial
random state, not an entire mask. Backward regenerates the identical mask from
that state. If record allocation fails, the generator is restored so a failed
operation does not consume random numbers.

## Parameter initialization

The AD layer does not initialize parameter values. The application must do so
before the first forward pass. Biases commonly start at zero. For neural-network
weight matrices, use a small random initialization appropriate to the
activation (for example Xavier/Glorot for many tanh or linear networks, or He
initialization for ReLU networks); identical zero weights can prevent hidden
units from learning distinct features. Optimizer moment/state arrays themselves
start at zero.

## Common validation rules

Operator argument validation is disabled by default so it adds no overhead to
the evaluation path. Define `DSPPP_AUTODIFF_ENABLE_VALIDATION` to `1` before
including the autodiff headers, or define it consistently for the complete
build, to enable these checks.

When enabled, views used by one expression must belong to the same tape and
satisfy the operator's role and shape requirements. Output and input value
storage must be distinct, and output gradient storage must not alias an input
gradient. An invalid combination sets the sticky
`Status::tape_mismatch`. When validation is disabled, the application is
responsible for meeting these preconditions; invalid arguments may cause
out-of-bounds access or otherwise undefined results.

A successfully computed value has a producer only when recording is enabled
and its record was appended successfully. Operator registration and arena
allocation failures are checked in both validation configurations.
