# Common options for the code generators

Global options for the code generators. There are specific options for the C, Python and Graphviz generators. They are described in different part of the documentation.

## debugLimit (default = 0)

When `debugLimit` is > 0, the number of iterations of the scheduling is limited to  `debugLimit`. Otherwise, the scheduling is running forever or until an error has occured.

## dumpFIFO (default = False)

When true, generate some code to dump the FIFO content at **runtime**. Only useful for debug.

In C++ code generation, it is only available when using the mode `codeArray == False`.

When this mode is enabled, the first line of the scheduler file is :

`#define DEBUGSCHED 1`

and it also enable some debug code in `GenericNodes.h`

## schedName (default = "scheduler")

Name of the scheduler function used in the generated code.

## prefix (default = "")

Prefix to add before the FIFO buffer definitions. Those buffers are not static and are global. If you want to use several schedulers in your code, the buffer names used by each should be different.

Another possibility would be to make the buffer static by redefining the macro `CG_BEFORE_BUFFER`