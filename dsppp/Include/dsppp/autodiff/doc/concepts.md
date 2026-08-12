# Concepts and memory model

## Tape and arena

Reverse-mode AD first computes values and records the operations that produced
them. It then visits those records in reverse order to propagate gradients.
This ordered record is conventionally called a **tape**: operations are
recorded during the forward pass and played backward during differentiation.

`Tape<T>` manages this record and the gradient buffers. `Arena<Bytes, T>` supplies
exactly `Bytes` bytes of storage to a `Tape`:

```cpp
Arena<2048, float> arena;
Tape<float> &tape = arena.tape();
```

The capacity is fixed and no dynamic memory allocation is performed. If the
arena is too small for a gradient or operation record, the tape reports
`Status::out_of_memory` instead of trying to grow it.

## Values, views, and gradients

The application owns every value buffer, including intermediates and final
outputs. A `BufferView` is only a non-owning description containing value and
gradient pointers, a length, its tape, its role, and the operation that
produced it.

- `tape.input(values)` creates an ordinary input. It has no gradient storage.
- `tape.parameter(values)` creates a trainable parameter and allocates its
  gradient from the arena.
- `tape.output(values)` creates an intermediate or result and allocates its
  gradient from the arena.

Caller-owned gradients can be supplied with overloads such as
`tape.parameter(values, gradients, length)` and
`tape.output(values, gradients, length)`. Those views consume no arena space
for their gradient arrays.

Values and caller-owned gradients must remain alive until `backward()` has
finished. Values read by a backward rule must not be changed after the forward
operation. Output storage must not alias an operator's inputs, and a live
intermediate must not be overwritten by a later operation before
`backward()`, `rewind_graph()`, or `reset()`.

True C arrays preserve their extent, so `tape.input(array)` can deduce the
length. A pointer does not carry a length and requires an explicit overload:

```cpp
BufferView<float> input = tape.input(pointer, number_of_elements);
MatrixView<float> weights = tape.parameter(pointer, rows, columns);
```

## Operator registration

The core contains no numerical operators. Each operator is a separate header
and class. An application includes and registers only what it uses. This keeps
code size down: operators that are not referenced can be removed by the
compiler and linker.

```cpp
#include <dsppp/autodiff/operators/relu.hpp>

tape.register_operator<ReluOperator<float>>();
```

Using an operator that has not been registered sets
`Status::operator_not_registered`. A tape can register 16 operator types by
default. Define `DSPPP_AUTODIFF_MAX_OPERATORS` to a larger power of two before
including `reverse.hpp` when more are needed. Registrations survive
`Tape::reset()`.

See [Operators](operators.md) for the current operator API and formulas.

## Recording and `RecordingScope`

Normally an assignment such as `y = relu(x)` computes `y` and appends a record
for its backward rule. When recording is disabled, the numerical result is
still computed but no record is appended. This is useful for inference and
consumes no additional arena space.

`RecordingScope` temporarily changes this tape setting for one block of code:

```cpp
{
    RecordingScope inference(tape, false);
    probability = softmax(logits);
    // No operation record is added here.
} // The recording state that existed before the scope is restored.
```

The name means "the scope in which tape recording has this setting." Creating
the object saves the previous setting and applies the requested one. Leaving
the block restores the previous setting, including when the block exits early.

A value computed without recording has no `producer`, so it cannot be passed
as the root of `backward()`. Doing so sets `Status::invalid_output`.

## Reusing arena storage

`Tape::reset()` releases all arena-managed gradients and records, invalidating
all views. Operator registrations are retained.

Training normally needs to retain parameter and output gradient buffers while
rebuilding the operation records on every iteration:

1. Create persistent views.
2. Call `begin_graph()`. This saves the current arena position.
3. At the start of each iteration, call `rewind_graph()`. It returns to that
   saved position and clears the record list without invalidating the views.
4. Evaluate the forward graph again, which records fresh operations pointing
   at the current values.

Both marking and rewinding take constant time.

## Failure model

The first error is sticky until `clear_status()`, `reset()`, `begin_graph()`, or
`rewind_graph()` clears it as specified by the API. Check `tape.good()` or the
Boolean result of `backward()` before using gradients.

- `out_of_memory`: a gradient or operation record did not fit in the arena.
- `tape_mismatch`: views, roles, shapes, pointers, or aliasing are invalid for
  an operation.
- `invalid_output`: the backward root or seed is invalid, or graph rewinding
  was requested without a mark.
- `operator_not_registered`: an expression's operator was not registered.
- `operator_registry_full`: no registration slot remains.

If record allocation fails after a forward kernel runs, its numerical result
may already have been written, but the output is detached from the tape.
Measure `tape.used()` on representative worst-case graphs to choose an arena
capacity with suitable margin.
