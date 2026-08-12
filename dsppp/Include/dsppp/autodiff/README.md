# Reverse automatic differentiation

Training an algorithm requires the derivative of a loss with respect to every
parameter being learned. Automatic differentiation (AD) computes these
gradients from the operations executed by the algorithm.

Reverse-mode AD is efficient when a computation has more inputs than outputs.
Training normally has many parameter inputs and one scalar loss output, so one
forward evaluation followed by one backward pass computes the gradients for
all parameters.

The purpose of adding reverse AD to CMSIS-DSP is to enable efficient on-device
learning on resource-constrained devices. It reuses accelerated CMSIS-DSP
kernels and the fusion capabilities of the CMSIS-DSP C++ extension, while
providing predictable fixed memory use, no heap allocation, and no exceptions.

## Why this implementation uses CMSIS-DSP

Automatic differentiation needs high performance in both directions. The
forward pass evaluates the model, while the backward pass propagates and
accumulates gradients. This implementation uses CMSIS-DSP for both rather than
treating it only as a collection of forward-inference kernels.

When an operation maps directly to an optimized CMSIS-DSP C kernel, its
forward pass uses that kernel. For example, dot products use
`arm_dot_prod_f32`, fully connected matrix-vector products use
`arm_mat_vec_mult_f32`, and matrix products use `arm_mat_mult_f32`. These
kernels provide implementations optimized for the selected Arm target,
including Helium implementations where available.

The CMSIS-DSP C++ expression system is particularly useful in backward rules,
which often combine element-wise computation with accumulation. A typical
update is:

```text
gradient += input * output_gradient
```

Calling separate multiplication and addition kernels would require a temporary
buffer and two loops. A C++ expression can fuse the complete update into one
loop, avoiding the temporary and reducing memory traffic while giving the
compiler a larger operation to optimize and vectorize.

For example, the fully connected backward pass computes:

```text
bias_gradient   += output_gradient
weight_gradient += outer(output_gradient, input_value)
input_gradient  += dot(transpose_view(weight_value), output_gradient)
```

These are the expressions used by `FullyConnectedOperator::backward`. The
outer-product expression fuses multiplication with weight-gradient
accumulation. For the input gradient, `transpose_view` is a zero-copy view and
the `dot` overload returns a lazy transposed-matrix/vector expression, so its
result is accumulated without materializing either a transposed weight matrix
or a temporary result vector. Matrix multiplication similarly accumulates its
weight gradient through lazy `matvec` expressions. This combination of
optimized C kernels and fusible C++ expressions is especially important for
training, because backward passes contain more compound updates and
accumulations than typical forward inference code.

## How reverse differentiation works here

During the **forward pass**, each operator computes its output and, when
recording is enabled, adds a small record to a `Tape`. The record identifies
the values and gradients needed by that operator's derivative rule. The
ordered collection is called a tape because the operations are recorded going
forward and replayed in reverse.

The **backward pass** starts at the selected output, normally a scalar loss. It
sets the loss gradient to one and visits the recorded operations in reverse
order. Each operation applies its local derivative rule and accumulates its
contribution into the gradients of earlier intermediate results and learnable
parameters. This reverse application of the chain rule produces all parameter
gradients needed by an optimizer.

Numerical values remain in buffers owned by the application. An `Arena<Bytes>`
provides a fixed amount of memory for gradients and tape records, so memory use
cannot grow unexpectedly at runtime. Ordinary inputs registered with
`tape.input()` do not receive gradient storage.

The implementation is modular: an application includes and registers only the
operator headers it uses. The core in `reverse.hpp` manages views, fixed arena
storage, operator registration, and reverse traversal; each operator header
contains its own forward computation and derivative rule.

## Minimal example

```cpp
#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/scale.hpp>

using namespace arm_cmsis_dsp::autodiff;

int main()
{
    Arena<512> arena;
    Tape &tape = arena.tape();
    tape.register_operator<ScaleOperator>();

    float x_value[] = {1.0F, 2.0F};
    float a_value = 3.0F;
    float y_value[2] = {};

    BufferView x = tape.input(x_value);
    BufferView a = tape.parameter(a_value);
    BufferView y = tape.output(y_value);

    // y = a * x
    y = scale(x, a); // y = {3, 6}; records the operation
    const float seed[] = {1.0F, 1.0F};
    tape.backward(y, seed, 2U); // da = 1*1 + 1*2 = 3
    return tape.good() ? 0 : 1;
}
```

## Documentation

- [Concepts and memory model](doc/concepts.md) explains the arena, tape,
  buffer roles, recording, buffer lifetime, and errors.
- [Training loop](doc/training_loop.md) gives a complete, defined example and
  explains setup, graph reuse, gradient clearing, reverse propagation, and the
  optimizer update.
- [Operators](doc/operators.md) documents the current operator families,
  formulas, shape rules, dropout behavior, and the CMSIS-DSP implementation
  paths.
- [Optimizers](doc/optimizers.md) documents Adam and RMSProp capacities,
  initialization, updates, freezing, and errors.
- [Worked implementation flow: `y = a * x`](doc/implementation_flow.md) follows
  one expression through `reverse.hpp`, including its tape record, `producer`,
  node links, gradient reset, seed, and backward rule.

## Examples and tests

`dsppp/Examples/autodiff_regression.cpp` trains a polynomial approximation to
`sin(x)` with RMSProp. `dsppp/Examples/autodiff_iris.cpp` trains a small fully
connected classifier with Adam.

Autodiff uses the existing dsppp board-test infrastructure. From `dsppp`, run:

```sh
python run_all.py --test AUTODIFF_TEST --dt F32_DT
```

It currently supports `float` and dynamic test mode. The test body is selected
when `AUTODIFF_TEST`, `F32_DT`, and `DYNAMIC_TEST` are defined.
