## How to build the examples

In folder `ComputeGraph/example/build`, type the `cmake` command:

```bash
cmake -DHOST=YES \
   -DDOT="path to dot.EXE" \
   -DCMSISCORE="path to cmsis core include directory" \
   -G "Unix Makefiles" ..
```

The Graphviz dot tool is requiring a recent version supporting the HTML-like labels.

If cmake is successful, you can type `make` to build the examples. It will also build CMSIS-DSP for the host.

If you don't have graphviz, the option -DDOT can be removed.

If for some reason it does not work, you can go into an example folder (for instance example1), and type the commands:

```bash
python graph.py 
dot -Tpdf -o test.pdf test.dot
```

It will generate the C++ files for the schedule and a pdf representation of the graph.

Note that the Python code is relying on the CMSIS-DSP PythonWrapper which is now also containing the Python scripts for the Synchronous Data Flow.

For `example3` which is using an input file, `cmake` should have copied the input test pattern `input_example3.txt` inside the build folder. The output file will also be generated in the build folder.

`example4` is like `example3` but in pure Python and using the CMSIS-DSP Python wrapper (which must already be installed before trying the example). To run a Python example, you need to go into an example folder and type:

```bash
python main.py
```

`example7` is communicating with `OpenModelica`. You need to install the VHTModelica blocks from the [VHT-SystemModeling](https://github.com/ARM-software/VHT-SystemModeling) project on our GitHub