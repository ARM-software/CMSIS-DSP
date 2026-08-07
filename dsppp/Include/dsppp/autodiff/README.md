# Reverse automatic differentiation reference

`reverse.hpp` is a deliberately small reverse-mode automatic differentiation
(AD) implementation for embedded use. It currently handles `float` buffers,
vector addition, vector dot products, affine vector scaling
`alpha * x + beta`, fully connected and ReLU nodes, and a quadratic-error
loss. Fixed-storage Adam and RMSProp optimizers support user-written training
loops. The structure is intended to be extended with tensor operators.

## Why this implementation uses CMSIS-DSP

Automatic differentiation needs high performance in both directions. The
forward pass evaluates the model, while the backward pass propagates and
accumulates gradients. This implementation uses CMSIS-DSP for both rather than
treating it only as a collection of forward inference kernels.

Where an operation maps directly to an optimized CMSIS-DSP C kernel, the
forward pass uses that kernel. For example, dot products use
`arm_dot_prod_f32`, and fully connected matrix-vector products use
`arm_mat_vec_mult_f32`. These kernels provide implementations optimized for
the selected Arm target, including Helium implementations where available.

The CMSIS-DSP C++ extension is particularly valuable during the backward pass.
Reverse rules frequently combine several element-wise operations with an
accumulation. A typical example is:

```text
gradient += input * output_gradient
```

Calling separate C kernels for the multiplication and addition would normally
require an intermediate buffer and two loops: one loop produces the products
and another accumulates them. The C++ expression system can fuse the complete
expression into one loop. That loop performs more useful computation for each
load and store, avoids the temporary buffer, reduces memory traffic, and gives
the compiler a larger loop body to vectorize effectively.

The fully connected backward pass illustrates this approach:

```text
bias_gradient   += output_gradient
weight_gradient += outer(output_gradient, input_value)
input_gradient  += transpose(weight_value) * output_gradient
```

Bias and outer-product accumulation use fused CMSIS-DSP C++ expressions. The
input gradient uses optimized dot products over strided column views, avoiding
a materialized matrix transpose. The same principle applies to future vector,
matrix, and tensor reverse rules: express the whole local gradient update as a
fused accumulation whenever the C++ extension supports it.

This fusion capability is an important distinction when selecting a Helium
math library for training. A library may provide individually optimized
Helium primitives but still require multiple loops and intermediate buffers
for a compound backward expression. CMSIS-DSP combines optimized kernels with
a C++ expression mechanism capable of fusing those loops, which is especially
important for backward passes because they contain more compound operations
and accumulations than typical forward inference code.

## What are the arena and the tape?

Reverse AD needs to remember the operations performed during the forward
calculation. It later visits those operations in the opposite order to
propagate derivatives from the output back to the inputs. This ordered record
of operations is traditionally called a **tape**, by analogy with recording a
sequence on magnetic tape and playing it backward. The name is standard AD
terminology; it is not related to a C++ container type.

In this implementation, `Tape` does two jobs:

1. During the forward calculation, it writes one small, fixed-size record for
   each operation. A record contains non-owning buffer pointers and the
   information needed by that operation's derivative rule.
2. `backward()` follows those records in reverse order and accumulates the
   gradients.

The tape core contains no numerical operators. Each operator is a separate,
self-contained class and header containing its validation, forward rule, tape
record, gradient reset, backward rule, and expression adapter:

| Header | Operator class | Expression |
| --- | --- | --- |
| `operators/add.hpp` | `AddOperator` | `a + b` |
| `operators/dot.hpp` | `DotOperator` | `dot(a, b)` |
| `operators/scale.hpp` | `ScaleOperator` | `scale(x, alpha, beta)` |
| `operators/fully_connected.hpp` | `FullyConnectedOperator` | `fully_connected(x, m, b)` |
| `operators/relu.hpp` | `ReluOperator` | `relu(x)` |
| `operators/quadratic_error.hpp` | `QuadraticErrorOperator` | `quadratic_error(prediction, target)` |

An application includes and registers only the operators it uses. An operator
header that is not included is not part of that translation unit. Registration
uses an allocation-free, open-addressed hash set of type tokens stored directly
in `Tape`; it does not instantiate or retain an operator object. Registration
and expression checks are expected O(1), with collision resolution by linear
probing and no deletion or tombstones.

Expression evaluation checks registration before validation or forward
computation. Using an included but unregistered operator leaves the output
unchanged and sets the sticky status to `Status::operator_not_registered`.
The registry defaults to 16 distinct operator types. Define
`DSPPP_AUTODIFF_MAX_OPERATORS` to a larger power of two before including the
core header when an application needs more slots. Exceeding the configured
capacity sets `Status::operator_registry_full`. `Tape::reset()` preserves
registrations, so they normally need to be installed only once during
application setup.

The tape needs memory for gradients and operation records. An `Arena<Bytes>`
owns exactly `Bytes` bytes of fixed storage and constructs a `Tape` that uses
that storage. It does
**not** allocate a `std::vector`, call the heap, or grow at runtime. For example,
`Arena<2048>` contains a 2048-byte array directly inside the `Arena` object. If
the object is a local variable, that storage is normally on the stack; if it is
static, the storage is static as well.

All value arrays, including intermediate and final outputs, are allocated by
the user. The way a buffer is registered tells the tape whether it needs a
gradient:

- `tape.input(values)` registers ordinary algorithm input. It allocates no
  gradient because the application does not request derivatives for it.
- `tape.parameter(values)` registers trainable parameters and allocates their
  gradients from the arena.
- `tape.output(values)` registers an intermediate or final output and allocates
  its adjoint from the arena because it is needed during reverse propagation.

These functions never copy or take ownership of values. This role-based rule
is used by vector and matrix operators and is intended for future tensors as
well. Operation
records contain only pointers, dimensions, and small operator-specific
metadata, so their cost does not grow with the amount of numerical data.
Gradient storage naturally requires one `float` per parameter or intermediate
element, but no storage is spent on input gradients.

Applications that need exact placement of every buffer can use the overloads
`tape.parameter(values, gradients, length)` and
`tape.output(values, gradients, length)` with caller-owned gradient storage.
These advanced overloads perform no arena allocation for the view.

Large buffers may live on the stack, in static memory, or in an
application-owned memory pool. They must remain alive until `backward()`
returns. Input values must not change between their use in the forward pass and
the backward pass, because derivative rules may read them. Output buffers must
be distinct from input buffers in this reference implementation. Recorded
computations use SSA-style storage: do not reuse or overwrite an earlier live
output buffer for another operation before `backward()` or `Tape::reset()`.

## Basic use

```cpp
#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/add.hpp>
#include <dsppp/autodiff/operators/dot.hpp>
#include <dsppp/autodiff/operators/scale.hpp>

using namespace arm_cmsis_dsp::autodiff;

Arena<2048> arena; // Gradient and operation-record storage.
Tape &tape = arena.tape();
tape.register_operator<AddOperator>();
tape.register_operator<DotOperator>();
tape.register_operator<ScaleOperator>();

float x_value[] = {1.0F, 2.0F};
float alpha_value = 2.0F;
float beta_value[] = {3.0F, 4.0F};
float scaled_value[2] = {};
float sum_value[2] = {};
float result_value[1] = {};

BufferView x = tape.input(x_value);             // No gradient for x.
BufferView alpha = tape.parameter(alpha_value); // Scalar parameter.
BufferView beta = tape.parameter(beta_value);   // Vector parameter.
BufferView scaled = tape.output(scaled_value);
BufferView sum = tape.output(sum_value);
BufferView result = tape.output(result_value);

scaled = scale(x, alpha, beta); // scaled = alpha * x + beta
sum = scaled + x;
result = dot(sum, x);           // result[0] == 26

if (tape.backward(result)) {
    // x.has_gradient() == false
    // alpha.gradient(0) == 5
    // beta gradients are {1, 2}
}
```

## Training, optimizers, and frozen parameters

`Adam<MaximumElements, MaximumParameters>` and
`RMSProp<MaximumElements, MaximumParameters>` keep all optimizer state in
fixed-size arrays inside the optimizer object. `MaximumElements` is the total
number of scalar parameter values and `MaximumParameters` is the maximum
number of separately registered parameter views (16 by default). Neither
optimizer allocates memory or throws exceptions.

### RMSProp template and constructor arguments

The two RMSProp template arguments are compile-time storage capacities, not
algorithm hyperparameters:

```cpp
RMSProp<MaximumElements, MaximumParameters> optimizer;
```

- `MaximumElements` is the maximum total number of scalar values across all
  parameter views added to the optimizer. It has no default.
- `MaximumParameters` is the maximum number of separately added parameter
  views. It defaults to 16.

For example, the polynomial regression has a three-element coefficient vector
and a separate scalar bias:

```cpp
RMSProp<4, 2> optimizer;
optimizer.add(coefficients); // Three elements; first parameter view.
optimizer.add(bias);         // One element; second parameter view.
```

This consumes all four element slots and both parameter-view slots. A matrix
counts as one parameter view, while its flattened `rows * columns` values all
count toward `MaximumElements`. Frozen parameters still occupy their original
slots. Adding the same value buffer again is idempotent and does not consume
another slot.

These capacities determine the optimizer object's static memory footprint.
RMSProp contains one `float square_average_[MaximumElements]` array plus an
`Entry entries_[MaximumParameters]` metadata array. Each entry stores the
parameter value and gradient pointers, its length, its state-array offset, and
whether it is trainable. Parameter values and gradients are not copied into
the optimizer: values remain caller-owned and gradients remain in the tape
arena (or in caller storage when that overload is used). There is no heap
allocation and neither capacity can grow at runtime.

When only the first argument is specified, the 16-view default applies:

```cpp
RMSProp<100> optimizer; // Up to 100 scalar values in up to 16 views.
```

The constructor arguments configure the numerical RMSProp update:

```cpp
RMSProp<4, 2> optimizer(
    1.0e-3F, // learning_rate
    0.99F,   // alpha: squared-gradient moving-average decay
    1.0e-8F  // epsilon: denominator stabilization
);
```

For each trainable scalar parameter, `step()` performs:

```text
square_average = alpha * square_average
               + (1 - alpha) * gradient^2
parameter -= learning_rate * gradient / (sqrt(square_average) + epsilon)
```

If `add()` would exceed `MaximumParameters`, it returns `false` and sets
`OptimizerStatus::too_many_parameters`. If the total number of scalar values
would exceed `MaximumElements`, it sets `OptimizerStatus::too_many_elements`.
A non-parameter view, a missing gradient, or an unknown view passed to
`freeze()` sets `OptimizerStatus::invalid_parameter`. Optimizer errors are
sticky: after an error, `good()` is false, `status()` reports the first error,
and `step()` returns false.

### Adam template and constructor arguments

Adam uses the same two compile-time capacity arguments as RMSProp:

```cpp
Adam<MaximumElements, MaximumParameters> optimizer;
```

- `MaximumElements` is the maximum total number of scalar parameter values.
- `MaximumParameters` is the maximum number of parameter views and defaults
  to 16.

Consequently, the same regression parameters fit in:

```cpp
#include <dsppp/autodiff/optimizers/adam.hpp>

Adam<4, 2> optimizer;
optimizer.add(coefficients); // Three scalar values in one view.
optimizer.add(bias);         // One scalar value in a second view.
```

Matrices, frozen parameters, and duplicate registrations are counted in the
same way as for RMSProp. `Adam<100>` means up to 100 scalar values distributed
across the default maximum of 16 parameter views.

Adam keeps two state values per scalar parameter, so its principal state
storage is twice that of RMSProp:

```text
float first_moment_[MaximumElements]
float second_moment_[MaximumElements]
Entry entries_[MaximumParameters]
```

It additionally stores scalar configuration values and the current powers of
`beta1` and `beta2` used for bias correction. Parameter values and gradients
are referenced through the metadata entries and are not copied. Adam performs
no heap allocation.

The constructor arguments are:

```cpp
Adam<4, 2> optimizer(
    1.0e-3F, // learning_rate
    0.9F,    // beta1: first-moment decay
    0.999F,  // beta2: second-moment decay
    1.0e-8F  // epsilon: denominator stabilization
);
```

On successful optimizer step `t`, each trainable scalar is updated as follows:

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

The correction compensates for moments initialized to zero, particularly
during the first training steps. Adam's global step advances whenever
`step()` succeeds. Frozen entries are skipped: their parameter values and two
moment arrays remain unchanged, although they continue to occupy capacity.

Adam reports the same sticky `OptimizerStatus` values as RMSProp:
`too_many_parameters`, `too_many_elements`, and `invalid_parameter`.
`add()`, `freeze()`, `good()`, `status()`, `zero_grad()`, and `step()` therefore
have the same usage pattern for both optimizer types. Switching optimizers in
a training loop normally requires only changing the included header, the
optimizer type, its capacities, and its numerical hyperparameters.

The user owns the training loop. Register each parameter once, evaluate the
graph, run the reverse pass, and then update the parameters:

```cpp
#include <dsppp/autodiff/optimizers/rmsprop.hpp>

RMSProp<4, 2> optimizer(1.0e-3F); // Four scalar values in two views.
optimizer.add(coefficients);
optimizer.add(bias);

tape.begin_graph(); // Everything allocated so far remains persistent.
for (std::size_t step = 0; step < number_of_steps; ++step) {
    tape.rewind_graph(); // Reclaim records from the preceding iteration.

    for (std::size_t sample = 0; sample < sample_count; ++sample) {
        // Scalar output views share the corresponding elements of the
        // persistent vector gradient buffers.
        BufferView x = tape.input(feature_value[sample]);
        BufferView p = tape.output(&polynomial_value[sample],
                                   &polynomial.gradients()[sample], 1);
        BufferView y = tape.output(&prediction_value[sample],
                                   &prediction.gradients()[sample], 1);
        p = dot(x, coefficients);
        y = p + bias;
    }

    // prediction and target cover the complete training set.
    loss = quadratic_error(prediction, target);

    optimizer.zero_grad();
    if (!tape.backward(loss) || !optimizer.step()) {
        handle_error();
    }
}
```

`begin_graph()` places an arena mark after persistent gradient buffers.
`rewind_graph()` returns to that mark in constant time, preserving the views,
gradient buffers, parameter values, and operator registrations while releasing
the previous iteration's operation records.

Parameters can be frozen without rebuilding the graph. A frozen parameter
still participates in forward and backward propagation, but `step()` leaves
its value and optimizer state unchanged:

```cpp
freeze_parameters(optimizer, coefficients);   // Bias-only fine tuning.
unfreeze_parameters(optimizer, coefficients); // Train it again later.
```

This API freezes parameter views rather than operator classes because an
operator is stateless and several invocations can use different parameters.
To freeze a layer, pass all parameter views owned by that layer. Both optimizer
implementations return `false` and set a sticky `OptimizerStatus` when an
operation exceeds capacity or receives an invalid parameter view.

Quadratic error computes the sum, not the mean:

```text
loss = sum((prediction[i] - target[i])^2)
d(loss)/d(prediction[i]) = 2 * (prediction[i] - target[i])
```

The target must be an input view; gradients are retained only for the
prediction path and ultimately for its parameters.

### Polynomial sinusoid regression

`dsppp/Examples/autodiff_regression.cpp` follows the polynomial PyTorch example without a
fully connected or ReLU node. For every sample it constructs the caller-owned
feature vector `{x, x^2, x^3}` and evaluates:

```cpp
polynomial = dot(features, coefficients);
prediction = polynomial + bias;
loss = quadratic_error(prediction, target);
```

RMSProp learns `bias + c1*x + c2*x^2 + c3*x^3` from 100 uniformly spaced
points over `[-pi, pi]`. Each training step first computes all 100 predictions.
It then creates one quadratic-error node over both complete vectors, calls
`backward()` once, and calls the optimizer once. The optimized objective is
therefore the global sum
`sum((prediction[sample] - target[sample])^2)`, rather than 100 independent
online updates. Dividing the reported value by 100 gives its mean quadratic
error without changing the optimum.

The example also demonstrates bias-only fine tuning and saving and restoring
values through the C-compatible `SinePolynomialParameters` struct.
The checkpoint deliberately contains parameter values only. Continuing
training with exactly the same optimizer trajectory would additionally require
persisting RMSProp or Adam state; inference does not need that state.

### Fully connected and ReLU

A fully connected node computes `y = m * x + b`. `m` is a row-major matrix
parameter, `b` is a vector parameter, and the number of matrix columns must
match the input length. The number of rows must match both the bias and output
lengths:

```cpp
#include <dsppp/autodiff/operators/fully_connected.hpp>
#include <dsppp/autodiff/operators/relu.hpp>

tape.register_operator<FullyConnectedOperator>();
tape.register_operator<ReluOperator>();
```

```cpp
float x_value[] = {2.0F, -1.0F};
float m_value[2][2] = {{1.0F, 2.0F}, {-3.0F, 1.0F}};
float b_value[] = {1.0F, 0.0F};
float linear_value[2] = {};
float activation_value[2] = {};

BufferView x = tape.input(x_value);
MatrixView m = tape.parameter(m_value);
BufferView b = tape.parameter(b_value);
BufferView linear = tape.output(linear_value);
BufferView activation = tape.output(activation_value);

linear = fully_connected(x, m, b); // linear == {1, -7}
activation = relu(linear);          // activation == {1, 0}

const float seed[] = {1.0F, 1.0F};
tape.backward(activation, seed, 2);
```

The fully connected backward rule is:

```text
m_gradient[row, column] += y_gradient[row] * x_value[column]
b_gradient[row]         += y_gradient[row]
x_gradient[column]      += y_gradient[row] * m_value[row, column]
```

The forward matrix-vector product uses `arm_mat_vec_mult_f32`, followed by a
CMSIS-DSP C++ vector expression that adds the bias. The reverse pass uses fused
C++ expressions for bias accumulation and
`m_gradient += outer(y_gradient, x_value)`. When an input gradient is needed,
each row-major weight column is exposed as a strided view and accumulated with
the C++ dot implementation. This avoids a transposed matrix and all temporary
numerical buffers. Because the C matrix descriptor stores dimensions as
`uint16_t`, larger dimensions are rejected as a shape mismatch.

The last line is skipped when `x` is an `input`. It is used when `x` is an
intermediate output, allowing several fully connected and activation nodes to
be chained while gradients are ultimately retained only for parameters.

ReLU is element-wise. It propagates the output gradient when its input value is
strictly positive and propagates zero for negative values and at zero.

### How buffer length is determined

In the example, the vector buffers are actual fixed-size C arrays. The overload
below illustrates how each registration function receives an array by
reference, so the compiler deduces `Length` without storing runtime size
information in the array:

```cpp
template<std::size_t Length>
BufferView Tape::input(float (&values)[Length]);
```

This deduction only works while the expression still has an array type. Once
an array is converted to `float *`, its length is not available in C++ and
cannot be inferred safely. Buffers obtained dynamically, from a memory pool, or
through a pointer therefore use the explicit-length overload:

```cpp
float *values = application_pool_allocate(number_of_elements);
BufferView dynamic_input = tape.input(values, number_of_elements);
```

The autodiff implementation does not allocate or free `values`; it only
allocates the corresponding gradient buffer in its fixed arena. The same rule
applies to matrix and future tensor views: dimensions can be deduced from true
array types when available, but pointer-based storage must supply its shape.

For example, `tape.parameter(float_matrix)` deduces both dimensions from a true
`float[Rows][Columns]` array. A dynamically allocated row-major matrix uses
`tape.parameter(pointer, rows, columns)`.

The same tape may instead use any caller-owned buffer:

```cpp
alignas(std::max_align_t) unsigned char memory[2048];
Tape tape(memory, sizeof(memory));
```

`Tape::reset()` releases all arena-managed gradients and operation records in
constant time, without individual deallocation. It does not release or modify
caller-owned value buffers. All views become invalid after reset and must be
created again.

## Value-only evaluation

Recording can be disabled while an output is computed. Operators still write
the numerical result into the user-provided output buffer, but consume no
additional arena space for operation records and that output cannot be used as
the root of `backward()`:

```cpp
const std::size_t before = tape.used();
{
    RecordingScope no_gradient(tape, false);
    scaled = scale(x, alpha, beta);
    sum = scaled + x;
    result = dot(sum, x);
    use(result_value[0]);
}
// tape.used() == before
```

Calling `backward()` on a value-only output fails with
`Status::invalid_output`. The views and buffers may be reused later in a
recorded calculation.

## Arena and failure model

The implementation performs no `new`, `delete`, `malloc`, or standard-container
allocation. Placement `new` only starts the lifetime of records inside the
arena supplied by the caller. It does not request memory.

There are no C++ exceptions. Errors are reported through `Tape::status()` and
the Boolean result of `backward()`. The first error is sticky:

- `out_of_memory`: a gradient buffer or operation record did not fit in the
  arena;
- `tape_mismatch`: views came from different tapes, dimensions or required
  roles differ, or a required buffer is null;
- `invalid_output`: `backward()` received a value-only output or an invalid
  seed;
- `operator_not_registered`: an expression used an operator type that was not
  registered on this tape;
- `operator_registry_full`: the fixed registration list has no free slot.

If an operation record exhausts the remaining arena, the operation still
computes its numeric value but the result is detached. A view whose gradient
allocation fails is invalid. Always check `tape.good()` or `backward()` before
consuming derivatives. `used()` can be measured on representative worst-case
graphs to select a static arena size.

## How the reverse pass works

Each recorded operation appends one fixed-size record. The common `Node` prefix
stores links and pointers to the operation's gradient reset and backward rules.
The rest of an operation record contains non-owning buffer pointers and
dimensions.
`backward()` first clears the associated gradient buffers, then walks the
linked tape in reverse creation order.

For vector add `z[i] = x[i] + y[i]`, the local rule is:

```text
x_gradient[i] += z_gradient[i]
y_gradient[i] += z_gradient[i]
```

For `z = dot(x, y)`, it is:

```text
x_gradient[i] += z_gradient[0] * y_value[i]
y_gradient[i] += z_gradient[0] * x_value[i]
```

An addition or dot-product operand contributes to a gradient only when it is a
parameter or intermediate with gradient storage. An `input` has a null gradient
pointer, so the same backward rule simply skips that contribution.

For vector scaling `z[i] = alpha[0] * x[i] + beta[i]`, `x` is required to be an
input while `alpha` and `beta` are required to be parameters. Its rule is:

```text
alpha_gradient[0] += z_gradient[i] * x_value[i]  (summed over i)
beta_gradient[i] += z_gradient[i]
```

There is intentionally no `x_gradient`: the role declared by `tape.input(x)`
states that the caller does not request it.

All operators follow the same ownership rule: inputs and outputs stay in
caller storage, while their records retain non-owning pointers. Future matrix
and tensor operators should use views with shape and stride metadata rather
than copying numerical buffers into the tape.

## Adding an operator

The intended extension pattern is:

1. Create one header in `operators/` and one uniquely named operator class.
   That class identity is also its runtime registration token.
2. Define a trivially destructible record whose first member is `detail::Node`.
   Store only non-owning buffer pointers and small shape/stride metadata.
3. Keep the forward evaluator, gradient reset, and backward rule in the
   operator class. Every path must be `noexcept`.
4. At the start of evaluation, call
   `OperatorAccess::require<YourOperator>(tape)`. Use `OperatorAccess` for
   validation, recording state, status reporting, and appending the record.
5. Add a small expression class with `evaluate(BufferView&)`; the generic
   `BufferView::operator=` invokes it, so the core never needs modification.
6. Test registered execution, unregistered failure, value-only execution,
   derivatives, tape exhaustion, shapes, aliases, and buffer lifetimes.

Do not add an operator-specific method or record to `Tape`. Do not store
pointers to temporary caller data. If an operator needs a large forward
intermediate during its backward rule, make it an explicit caller-provided
workspace or output view rather than copying it into the tape arena.

This implementation is intentionally contiguous and sequential. It is not
thread safe, does not manage buffer lifetimes, and does not yet support strides
or higher derivatives. A training graph is reevaluated on every iteration;
only its arena storage is reused.

## Building and running the board test

Autodiff is tested only with the existing dsppp board-test infrastructure; it
has no standalone host CMake project. `dsppp/tests/autodiff_test.cpp` is listed
in `dsppp/tests/test.cproject.yml`, and `AUTODIFF_TEST` is a test category in
`dsppp/run_all.py`.

From the `dsppp` directory, select the category and its supported datatype:

```sh
python run_all.py --test AUTODIFF_TEST --dt F32_DT
```

`run_all.py` writes `test_config.h` and rebuilds when that generated
configuration changes. The test body is compiled and executed only when all
three generated selections are present:

```cpp
#if defined(AUTODIFF_TEST) && defined(F32_DT) && defined(DYNAMIC_TEST)
```

Autodiff currently supports only `float` and the dynamic test mode. Other
datatype or static-mode configurations retain an empty `autodiff_test()` entry
point, so the shared project can still build without executing unsupported
autodiff cases. Board compiler options, CMSIS-DSP sources, linking, and runtime
selection continue to come from the existing solution and layer files.
