# Operators

Each operator header owns its validation, forward computation, fixed-size tape
record, gradient reset, backward rule, and expression adapter. Include the
header and register its operator class before evaluating the expression.

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

These expressions require equal-length views. The forward paths use
`arm_add_f32`, `arm_sub_f32`, and `arm_mult_f32`. Multiply's backward path uses
fused CMSIS-DSP C++ expressions so multiplication and accumulation need no
temporary product vector.

## Dot, scalar scale, and scalar offset

`dot(x, y)` returns one value:

```text
z = sum(x[i] * y[i])
dx[i] += g * y[i]
dy[i] += g * x[i]
```

Its forward pass uses `arm_dot_prod_f32`.

`scale(x, a)` requires `a` to be a one-element parameter:

```text
z[i] = a * x[i]
da += sum(g[i] * x[i])
dx[i] += a * g[i]
```

The forward pass uses `arm_scale_f32`; the scalar gradient uses the C++ dot
expression. See the [worked implementation flow](implementation_flow.md) for a
line-by-line explanation.

`offset(x, b)` likewise requires a one-element parameter:

```text
z[i] = x[i] + b
db += sum(g[i])
dx[i] += g[i]
```

Its forward pass uses `arm_offset_f32` and the bias gradient uses
`arm_accumulate_f32`.

## ReLU and softmax

ReLU computes `y[i] = max(0, x[i])` with `arm_clip_f32`. Its backward rule
passes `g[i]` only when the saved input value is strictly positive. The
derivative at zero is defined as zero.

Softmax uses a log-sum-exp forward calculation for numerical stability:

```text
y[i] = exp(x[i] - log(sum(exp(x))))
projection = dot(g, y)
dx[i] += y[i] * (g[i] - projection)
```

The forward path uses `arm_logsumexp_f32`, `arm_offset_f32`, and `arm_vexp_f32`.

## Losses

Quadratic error returns a scalar sum, not a mean:

```text
loss = sum((prediction[i] - target[i])^2)
d_prediction[i] += g * 2 * (prediction[i] - target[i])
```

Categorical cross entropy also returns a scalar sum:

```text
p_safe[i] = max(probability[i], 1e-7)
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

The forward matrix-vector product uses `arm_mat_vec_mult_f32`, followed by a
fused C++ bias accumulation. Backward bias and outer-product updates are fused
C++ expressions. If `x` needs a gradient, the current implementation evaluates
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

The forward pass uses `arm_mat_mult_f32`. In the backward pass, each row of
`dW` is accumulated with the lazy C++ expression
`matvec(X, corresponding_row_of_dY)`. This fuses matrix-vector evaluation with
gradient accumulation; it does not call `arm_dot_prod_f32` once per weight and
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
tape.register_operator<DropoutOperator>();
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

Views used by one expression must belong to the same tape and satisfy the
operator's role and shape requirements. Output and input value storage must be
distinct, and output gradient storage must not alias an input gradient. An
invalid combination sets the sticky `Status::tape_mismatch`. A successfully
computed value has a producer only when recording is enabled and its record was
appended successfully.
