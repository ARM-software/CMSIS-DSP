# Converting PyTorch modules

`scripts/pytorch_to_autodiff.py` converts a PyTorch `nn.Module` into a
reusable C++ class using the CMSIS-DSP autodiff API. The generated class owns
the trained parameter values, tape views, intermediate buffers, operator
registration, `forward()`, and an `add_parameters()` optimizer helper.

## Model constraints

The input has an implicit batch dimension of one. Pass only the embedded shape
to the converter. For example, a PyTorch input shaped `(1, 4)` uses
`--input-shape 4`. Every traced value is checked after removing that batch
dimension. Scalars, vectors, and matrices are accepted; higher-rank tensors or
a batch other than one stop conversion with an error.

The current mappings are:

| PyTorch | CMSIS-DSP autodiff |
| --- | --- |
| `nn.Linear` | `fully_connected` |
| `nn.ReLU`, `torch.relu`, functional ReLU | `relu` |
| vector `nn.Softmax`, `torch.softmax`, functional softmax | `softmax` |
| `nn.Dropout`, functional dropout | `dropout` |
| `nn.Identity` | no operation |
| equal-shaped `+`, `-`, `*` | elementwise add, subtract, multiply |
| tensor and scalar-parameter `+`, `*` | `offset`, `scale` |
| vector `matmul`, `inner`, `dot` | `dot` |

In-place operations, multiple inputs or outputs, unsupported ATen operators,
and incompatible shapes are rejected. A
bias-free `nn.Linear` receives a fixed zero bias because the autodiff fully
connected operator requires a bias view; that synthetic bias is not added to
the optimizer.

## Running the converter

From `dsppp`, explicitly select `dsppp/.venv`. The `--no-project` option is
important because otherwise uv finds the repository-level `pyproject.toml`
and may run with the repository-level `.venv` instead:

```text
uv run --no-project --python .venv/Scripts/python.exe \
  Include/dsppp/autodiff/scripts/iris_pytorch.py \
  --input-shape 4 --factory create_model -o generated_iris.hpp
```

Install or inspect packages in that same environment by specifying its Python
interpreter as well:

```text
uv pip install --python .venv\Scripts\python.exe torch numpy
uv pip list --python .venv\Scripts\python.exe
```

Alternatively, activate `dsppp/.venv` and pass `--active` to `uv run`. A plain
`uv run` is intentionally not shown because project discovery can select a
different environment even when `dsppp/.venv` exists.

The model file must provide a zero-argument factory returning an `nn.Module`:

```python
def create_model() -> torch.nn.Module:
    return MyNetwork()
```

Conversion uses `torch.export.export` with one zero-valued batch-1 input. The
result is a functional ATen graph with model parameters lifted into explicit
graph inputs. The converter reads their values and trainable state through the
exported graph signature and state dictionary, then accepts only its documented
ATen allowlist. It does not train the module. Dropout is always emitted with
its probability, independently of the PyTorch module's current training state.
On the C++ side it is active while the autodiff tape records training and is an
identity when recording is disabled for inference.
Use `--dtype float16` to emit a `float16_t` network instead of the default
float32 network. `--arena-bytes` selects the generated autodiff arena capacity.

The converter supports two explicit export modes. The default
`--export-mode trained` embeds the module's current parameters and preserves a
trained module exactly. For a network that will be trained with autodiff, use
`--export-mode empty`. Empty mode emits zero-initialized parameter storage and a compact C++
`initialize_parameters()` loop: fully connected weights use uniform
random initialization and biases start at zero.

## Using generated code for training

The generated constructor creates network buffers and registers network
operators, but does not call `tape.begin_graph()` or `tape.rewind_graph()`.
The application can therefore create target and loss buffers on the same tape,
register a loss operator, add parameters to an optimizer, and then begin and
reuse the graph in the normal training loop.

The input values are written to the public `input_value` array. `forward()`
evaluates the converted network, and `output()` returns its final
`BufferView`. `add_parameters(optimizer)` adds every trainable PyTorch
parameter while omitting frozen and synthetic parameters.

## Iris comparison

`scripts/iris_pytorch.py` implements the same `4 -> 8 -> 3` classifier,
normalization, per-sample Adam updates, 120/30 training/test split, and
120-epoch default used by `dsppp/Examples/autodiff_iris.cpp`. It reads the
exact dataset from `dsppp/Examples/iris_data.hpp`.

Export an empty network without running the Python training loop with:

```text
uv run --no-project --python .venv/Scripts/python.exe \
  Include/dsppp/autodiff/scripts/iris_pytorch.py \
  --export generated_iris.hpp
```

This export uses generated C++ random initialization rather than embedding the
random values assigned by the newly constructed PyTorch module.

With no arguments, the script runs the 120-epoch PyTorch comparison without
exporting. To train and then export the trained parameters, request the number
of epochs explicitly:

```text
uv run --no-project --python .venv/Scripts/python.exe \
  Include/dsppp/autodiff/scripts/iris_pytorch.py \
  --epochs 120 --export generated_trained_iris.hpp
```
