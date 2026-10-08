# Autodiff examples

The solution contains two f32 reverse-mode automatic differentiation projects:

- `Iris` (`Iris/Iris.cproject.yml`) trains an Iris classifier.
- `Diff` (`Diff/Diff.cproject.yml`) illustrates the derivative of vector addition,
  printing each step without training or updating parameters.

## Iris

The Iris example trains a small classifier with CMSIS-DSP reverse-mode
automatic differentiation, using only `float` (f32). The Iris dataset is
included in `Iris/iris_data.hpp`. The example requires a CMSIS-DSP pack
providing the DSP++ autodiff headers and CMSIS-DSP C functions.

The network has four inputs, eight hidden units with ReLU, and three softmax
outputs. Adam minimizes cross-entropy over 120 epochs using 120 training
samples. Every fifth sample of each class is held out, giving 30 test samples.
The program prints the mean loss every 20 epochs, each test prediction, and
the final test accuracy. Dataset normalization uses fixed statistics over
all 150 samples.

## Diff

The input is `x = (1, 2, 3, 4)` and the example parameters are
`a = (10, 20, 30, 40)`. The program registers the addition operator and the
buffers, then evaluates `y = a + x = (11, 22, 33, 44)`.

A vector output has a Jacobian: `dy[i]/da[j]` is 1 when `i == j` and 0 otherwise.
Each backward pass seeds one output with 1 and the others with 0, then prints
the derivatives with respect to all four parameters:

```text
seed = (1, 0, 0, 0)  ->  dy[0]/da = (1, 0, 0, 0)
seed = (0, 1, 0, 0)  ->  dy[1]/da = (0, 1, 0, 0)
seed = (0, 0, 1, 0)  ->  dy[2]/da = (0, 0, 1, 0)
seed = (0, 0, 0, 1)  ->  dy[3]/da = (0, 0, 0, 1)
```

The tape clears gradients on each backward pass. `x` is registered with
`tape.input()`, so it is fixed data and has no gradient buffer. Mathematically,
`dy/dx` is also the identity matrix. The final printed values of `a`, `x`, and
`y` show that differentiation does not update the parameters.

## Build

The solution targets Cortex-M55. Startup and memory
configuration are in each project's `RTE/Device/ARMCM55` directory. Both projects
share the CMSIS-Compiler custom stdio and exit handlers in `Common`, which
use semihosting. The stack is 16 KiB and the heap is
64 KiB. The training buffers and optimizer state are allocated on the heap.

Activate the tools listed in `vcpkg-configuration.json` and install the packs
listed in the solution/project. Use the CMSIS Toolbox bundled with the CMSIS
Solution VS Code extension. From the installed example directory, build the
default CLANG configuration:

```sh
cbuild autodiff.csolution.yml --update-rte --context Iris.Release+ARMCM55
cbuild autodiff.csolution.yml --update-rte --context Diff.Release+ARMCM55
```

AC6 and GCC can be selected with `--toolchain AC6` or `--toolchain GCC`.
The examples have no precision switch or float16 buffers; all autodiff operators
and the optimizer are instantiated with `float`.

## Run on the FVP

Add the CMSIS Toolbox `bin` directory, the FVP `bin` directory, and the Python
directory containing `python3.dll` to `PATH`. From the installed example
directory, run the following commands in PowerShell:

```powershell
$env:AVH_FVP_PLUGINS = ''
FVP_Corstone_SSE-300_Ethos-U55.exe -f fvp_config.txt -a cpu0=out/Iris/ARMCM55/Release/Iris.elf
FVP_Corstone_SSE-300_Ethos-U55.exe -f fvp_config.txt -a cpu0=out/Diff/ARMCM55/Release/Diff.elf
```

For AC6 builds, use `Iris.axf` and `Diff.axf` instead of the `.elf` filenames above.

The default target set runs Iris; select the `Diff` target set to run the
derivative illustration from the CMSIS Solution extension.

Output appears in the FVP console. Each program calls
`std::exit` explicitly to terminate the model through the
semihosting exit handler.
