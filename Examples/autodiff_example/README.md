# Autodiff Iris example

This example trains a small Iris classifier with CMSIS-DSP reverse-mode
automatic differentiation, using only `float` (f32). The Iris dataset is
included in `Autodiff/iris_data.hpp`. The example requires a CMSIS-DSP pack
providing the DSP++ autodiff headers and CMSIS-DSP C functions.

The network has four inputs, eight hidden units with ReLU, and three softmax
outputs. Adam minimizes cross-entropy over 120 epochs using 120 training
samples. Every fifth sample of each class is held out, giving 30 test samples.
The program prints the mean loss every 20 epochs, each test prediction, and
the final test accuracy. Dataset normalization uses fixed statistics over
all 150 samples.

## Build

The solution targets Cortex-M55. Startup and memory
configuration are in `Autodiff/RTE/Device/ARMCM55`, and CMSIS-Compiler custom
stdio and exit handlers use semihosting. The stack is 16 KiB and the heap is
64 KiB. The training buffers and optimizer state are allocated on the heap.

Activate the tools listed in `vcpkg-configuration.json` and install the packs
listed in the solution/project. Use the CMSIS Toolbox bundled with the CMSIS
Solution VS Code extension. From the installed example directory, build the
default CLANG configuration:

```sh
cbuild autodiff.csolution.yml --update-rte --context Autodiff.Release+ARMCM55
```

AC6 and GCC can be selected with `--toolchain AC6` or `--toolchain GCC`.
The example has no precision switch or float16 buffers; all autodiff operators
and the optimizer are instantiated with `float`.

## Run on the FVP

Add the CMSIS Toolbox `bin` directory, the FVP `bin` directory, and the Python
directory containing `python3.dll` to `PATH`. From the installed example
directory, run the following commands in PowerShell:

```powershell
$env:AVH_FVP_PLUGINS = ''
FVP_Corstone_SSE-300_Ethos-U55.exe -f fvp_config.txt -a cpu0=out/Autodiff/ARMCM55/Release/Autodiff.elf
```

Training and test output appear in the FVP console. The program calls
`std::exit` explicitly to terminate the model through the
semihosting exit handler.
