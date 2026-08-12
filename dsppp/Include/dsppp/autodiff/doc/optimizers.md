# Optimizers

`Adam` and `RMSProp` update caller-owned parameter values from tape-managed or
caller-owned gradients. Their third template argument selects the scalar type
(`float` by default, or `float16_t`), matching the tape views. All optimizer
metadata and numerical state are fixed arrays inside the optimizer object;
neither optimizer allocates memory.

## Capacity arguments

Both types have the same capacity template arguments:

```cpp
Optimizer<MaximumElements, MaximumParameters>
```

- `MaximumElements` is the total number of scalar values across all added
  parameter views. It has no default.
- `MaximumParameters` is the number of separately added views. It defaults to
  16.

A single vector parameter of length 10 therefore needs 10 element slots but
only one parameter-view slot:

```cpp
float value[10] = {};
BufferView<float> parameter = tape.parameter(value);
RMSProp<10, 1> optimizer;
optimizer.add(parameter);
```

A matrix also counts as one view, while all `rows*columns` entries count toward
`MaximumElements`. A three-element coefficient vector plus a separate scalar
bias fits exactly in either `RMSProp<4, 2>` or `Adam<4, 2>`. Writing
`Adam<100>` reserves 100 scalar state positions and the default 16 view slots.
For half precision, use for example `Adam<100, 16, float16_t>`.

Adding the same value pointer twice is idempotent. A frozen parameter continues
to occupy both capacities.

## RMSProp

```cpp
RMSProp<4, 2> optimizer(
    1.0e-3F, // learning_rate
    0.99F,   // alpha
    1.0e-8F  // epsilon
);
```

For each trainable scalar, this implementation performs RMSProp without
momentum or centering:

```text
square_average = alpha * square_average
               + (1 - alpha) * gradient^2
parameter -= learning_rate * gradient
           / (sqrt(square_average) + epsilon)
```

`square_average` starts at zero. `alpha` controls how slowly squared-gradient
history changes; values near one produce longer memory. `epsilon` prevents a
zero or very small denominator. The default is `1e-8` for float32 and `1e-4`
for float16, where `1e-8` would round to zero. These are conventional starting
points, but learning rate normally requires tuning for the model and loss
scale.

Storage consists principally of one
`T square_average_[MaximumElements]` plus
`Entry entries_[MaximumParameters]`. Each entry stores value and gradient
pointers, length, state offset, and whether the parameter is trainable.

## Adam

```cpp
Adam<4, 2> optimizer(
    1.0e-3F, // learning_rate
    0.9F,    // beta1
    0.999F,  // beta2
    1.0e-8F  // epsilon
);
```

On successful step `t`:

```text
first_moment  = beta1 * first_moment
              + (1 - beta1) * gradient
second_moment = beta2 * second_moment
              + (1 - beta2) * gradient^2

corrected_first  = first_moment  / (1 - beta1^t)
corrected_second = second_moment / (1 - beta2^t)

parameter -= learning_rate * corrected_first
           / (sqrt(corrected_second) + epsilon)
```

Both moment arrays and their powers are initialized so the first successful
step applies the usual bias correction. `beta1` controls first-moment memory,
`beta2` controls squared-gradient memory, and `epsilon` stabilizes the
denominator. The default epsilon is `1e-8` for float32 and `1e-4` for
float16, where `1e-8` would round to zero. The other defaults are standard
initial choices.

Adam stores
`T first_moment_[MaximumElements]`,
`T second_moment_[MaximumElements]`, and
`entries_[MaximumParameters]`, so its principal per-element state is twice
RMSProp's. Its global step advances each time `step()` succeeds.

## Adding, clearing, and stepping

Add each parameter once after creating its view:

```cpp
float coefficient_value[3] = {};
float bias_value = 0.0F;
BufferView<float> coefficients = tape.parameter(coefficient_value);
BufferView<float> bias = tape.parameter(bias_value);

RMSProp<4, 2> optimizer;
optimizer.add(coefficients);
optimizer.add(bias);
```

Here `coefficients` is a three-element parameter `BufferView`, and `bias` is a
separate one-element parameter `BufferView`.

The usual iteration order is:

```cpp
evaluate_forward_graph();
optimizer.zero_grad();
if (!tape.backward(loss) || !optimizer.step())
    handle_error();
```

`zero_grad()` clears all gradients for parameters known to the optimizer.
`backward()` then writes the gradients for the current graph, and `step()`
consumes them. See [Training loop](training_loop.md) for why the explicit clear
is useful even though backward resets gradients referenced by its records.

## Freezing

Freezing prevents updates without changing the recorded graph. In this
example, `coefficients` is a parameter `BufferView` created with
`tape.parameter()` and previously passed to `optimizer.add()`:

```cpp
freeze_parameters(optimizer, coefficients);   // Train other parameters.
unfreeze_parameters(optimizer, coefficients); // Resume updates later.
```

The same functions also accept parameter `MatrixView` objects. Multiple views
can be passed when a layer owns more than one parameter, such as weights and a
bias.

The parameter still participates in forward and backward propagation, and its
gradient may be computed. `step()` skips its value and optimizer state, so both
remain unchanged while frozen. Operators are not frozen because they are
stateless; freeze every parameter view owned by the logical layer instead.

## Status and errors

Optimizer errors are sticky. After the first error, `good()` is false,
`status()` reports it, and `step()` returns false.

- `too_many_parameters`: another distinct view would exceed
  `MaximumParameters`.
- `too_many_elements`: its scalar length would exceed the remaining
  `MaximumElements` capacity.
- `invalid_parameter`: `add()` received a non-parameter or a view without a
  gradient, or `freeze()` received a view that had not been added.

Parameter values and gradient arrays are referenced, not copied. They must
remain alive as long as the optimizer uses them. A checkpoint containing only
parameter values is sufficient for inference. Reproducing the exact continuation
of training also requires saving the optimizer's moment state and, for Adam,
its step-dependent powers; the current optimizer classes do not provide a
serialization API.
