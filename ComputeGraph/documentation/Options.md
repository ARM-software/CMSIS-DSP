## Options

Several options can be used in the Python to control the schedule generation. Some options are used by the scheduling algorithm and other options are used by the code generators or graphviz generator:

### Options for the graph

Those options needs to be used on the graph object created with `Graph()`.

For instance :

```python
g = Graph()
g.defaultFIFOClass = "FIFO"
```

#### defaultFIFOClass (default = "FIFO")

Class used for FIFO by default. Can also be customized for each connection (`connect` of `connectWithDelay` call) with something like:

`g.connect(src.o,b.i,fifoClass="FIFOClassNameForThisConnection")`

#### duplicateNodeClassName(default="Duplicate")

Prefix used to generate the duplicate node classes like `Duplicate2`, `Duplicate3` ...

### Options for the scheduling

Those options needs to be used on a configuration objects passed as argument of the scheduling function. For instance:

```python
conf = Configuration()
conf.debugLimit = 10
sched = g.computeSchedule(config = conf)
```

Note that the configuration object also contain options for the code generators.

#### memoryOptimization (default = False)

When the amount of data written to a FIFO and read from the FIFO is the same, the FIFO is just an array. In this case, depending on the scheduling, the memory used by different arrays may be reused if those arrays are not needed at the same time.

This option is enabling an analysis to optimize the memory usage by merging some buffers when it is possible.

#### sinkPriority (default = True)

Try to prioritize the scheduling of the sinks to minimize the latency between sources and sinks.

When  this option is enabled, the tool may not be able to find a schedule in all cases. If it can't find a schedule, it will raise a `DeadLock` exception.

#### displayFIFOSizes (default = False)

During computation of the schedule, the evolution of the FIFO sizes is generated on `stdout`.

#### dumpSchedule (default = False)

During computation of the schedule, the human readable schedule is generated on `stdout`.

### Options for the code generator

#### debugLimit (default = 0)

When `debugLimit` is > 0, the number of iterations of the scheduling is limited to  `debugLimit`. Otherwise, the scheduling is running forever or until an error has occured.

#### dumpFIFO (default = False)

When true, generate some code to dump the FIFO content at runtime. Only useful for debug.

In C++ code generation, it is only available when using the mode `codeArray == False`.

When this mode is enabled, the first line of the scheduler file is :

`#define DEBUGSCHED 1`

and it also enable some debug code in `GenericNodes.h`

#### schedName (default = "scheduler")

Name of the scheduler function used in the generated code.

#### prefix (default = "")

Prefix to add before the FIFO buffer definitions. Those buffers are not static and are global. If you want to use several schedulers in your code, the buffer names used by each should be different.

Another possibility would be to make the buffer static by redefining the macro `CG_BEFORE_BUFFER`

#### Options for C Code Generation only

##### cOptionalArgs (default = "")

Optional arguments to pass to the C API of the scheduler function

It can either use a `string` or a list of `string` where an element is an argument of the function (and should be valid `C`).

##### codeArray (default = True)

When true, the scheduling is defined as an array. Otherwise, a list of function calls is generated.

A list of function call may be easier to read but if the schedule is long, it is not good for code size. In that case, it is better to encode the schedule as an array rather than a list of functions.

When `codeArray` is True, the option `switchCase`can also be used.

##### switchCase (default = True)

`codeArray` must be true or this option is ignored.

When the schedule is encoded as an array, it can either be an array of function pointers (`switchCase` false) or an array of indexes for a state machine (`switchCase` true)

##### eventRecorder (default = False)

Enable the generation of `CMSIS EventRecorder` intrumentation in the code. The CMSIS-DSP Pack is providing definition of 3 events:

* Schedule iteration
* Node execution
* Error

##### customCName (default = "custom.h")

Name of custom header in generated C code. If you use several scheduler, you may want to use different headers for each one.

##### postCustomCName (default = "")

Name of custom header in generated C code coming after all of the other includes. 

##### genericNodeCName (default = "GenericNodes.h")

Name of GenericNodes header in generated C code. If you use several scheduler, you may want to use different headers for each one.

##### appNodesCName (default = "AppNodes.h")

Name of AppNodes header in generated C code. If you use several scheduler, you may want to use different headers for each one.

##### schedulerCFileName (default = "scheduler")

Name of scheduler cpp and header in generated C code. If you use several scheduler, you may want to use different headers for each one.

If the option is set to `xxx`, the names generated will be `xxx.cpp` and `xxx.h`

##### CAPI (default = True)

By default, the scheduler function is callable from C. When false, it is a standard C++ API.

##### CMSISDSP (default = True)

If you don't use any of the datatypes or functions of the CMSIS-DSP, you don't need to include the `arm_math.h` in the scheduler file. This option can thus be set to `False`.

##### asynchronous (default = False)

When true, the scheduling is for a dynamic / asynchronous flow. A node may not always produce or consume the same amount of data. As consequence, a scheduling can fail. Each node needs to implement a `prepareForRunning` function to identify and recover from FIFO underflows and overflows.

A synchronous schedule is used as start and should describe the average case.

This implies `codeArray` and `switchCase`. This disables `memoryOptimizations`.

Synchronous FIFOs that are just buffers will be considered as FIFOs in asynchronous mode.

More info are available in the documentation for [this mode](Dynamic.md).

##### FIFOIncrease (default 0)

In case of dynamic / asynchronous scheduling, the FIFOs may need to be bigger than what is computed assuming a static / synchronous scheduling. This option is used to increase the FIFO size. It represents a percent increase.

For instance, a value of 10 means the FIFO will have their size updated from `oldSize` to `1.1 * oldSize` which is ` (1 + 10%)* oldSize`

If the value is a `float` instead of an `int` it will be used as is. For instance, `1.1` would increase the size by `1.1` and be equivalent to the setting `10` (for 10 percent).

##### asyncDefaultSkip (default True)

Behavior of a pure function (like CMSIS-DSP) in asynchronous mode. When `True`, the execution is skipped if the function can't be executed. If `False`, an error is raised.

If another error recovery is needed, the function must be packaged into a C++ class to implement a `prepareForRun` function.

#### Options for Python code generation only

##### pyOptionalArgs (default = "")

Optional arguments to pass to the Python version of the scheduler function

##### customPythonName (default = "custom")

Name of custom header in generated Python code. If you use several scheduler, you may want to use different headers for each one.

##### appNodesPythonName (default = "appnodes")

Name of AppNodes header in generated Python code. If you use several scheduler, you may want to use different headers for each one.

##### schedulerPythonFileName (default = "sched")

Name of scheduler file in generated Python code. If you use several scheduler, you may want to use different headers for each one.

If the option is set to `xxx`, the name generated will be `xxx.py`

### Options for the graphviz generator

#### horizontal (default = True)

Horizontal or vertical layout for the graph.

#### displayFIFOBuf (default = False)

By default, the graph is displaying the FIFO sizes. If you want to know with FIFO variable is used in the code, you can set this option to true and the graph will display the FIFO variable names.

### Options for connections

It is now possible to write something like:

```python
g.connect(src.o,b.i,fifoClass="FIFOSource")
```

The `fifoClass` argument allows to choose a specific FIFO class in the generated C++ or Python.

Only the `FIFO` class is provided by default. Any new implementation must inherit from `FIFObase<T>`

There is also an option to set the scaling factor when used in asynchronous mode:

```python
g.connect(odd.o,debug.i,fifoScale=3.0)
```

When this option is set, it will be used (instead of the global setting). This must be a float.
