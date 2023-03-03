# C Code generation

## API

```python
def ccode(self,directory,config=Configuration())
```

It is a method of the `Schedule` object returned by `computeSchedule`.

It generate C++ code implementing the static schedule.

* `directory` : The directory where to generate the C++ files
* `config` : An optional configuration object

## Options for C Code Generation

### cOptionalArgs (default = "")

Optional arguments to pass to the C API of the scheduler function

It can either use a `string` or a list of `string` where an element is an argument of the function (and should be valid `C`).

For instance:

```Python
conf.cOptionalArgs=["int someVariable"]
```

### codeArray (default = True)

When true, the scheduling is defined as an array. Otherwise, a list of function calls is generated.

A list of function call may be easier to read but if the schedule is long, it is not good for code size. In that case, it is better to encode the schedule as an array rather than a list of functions.

When `codeArray` is True, the option `switchCase`can also be used.

### switchCase (default = True)

`codeArray` must be true or this option is ignored.

When the schedule is encoded as an array, it can either be an array of function pointers (`switchCase` false) or an array of indexes for a state machine (`switchCase` true)

### eventRecorder (default = False)

Enable the generation of `CMSIS EventRecorder` intrumentation in the code. The CMSIS-DSP Pack is providing definition of 3 events:

* Schedule iteration
* Node execution
* Error

### customCName (default = "custom.h")

Name of custom header in generated C code. If you use several scheduler, you may want to use different headers for each one.

### postCustomCName (default = "")

Name of custom header in generated C code coming after all of the other includes.  By default none is used.

### genericNodeCName (default = "GenericNodes.h")

Name of GenericNodes header in generated C code. If you use several scheduler, you may want to use different headers for each one.

### appNodesCName (default = "AppNodes.h")

Name of AppNodes header in generated C code. If you use several scheduler, you may want to use different headers for each one.

### schedulerCFileName (default = "scheduler")

Name of scheduler `cpp` and header in generated C code. If you use several scheduler, you may want to use different headers for each one.

If the option is set to `xxx`, the names generated will be `xxx.cpp` and `xxx.h`

### CAPI (default = True)

By default, the scheduler function is callable from C. When false, it is a standard C++ API.

### CMSISDSP (default = True)

If you don't use any of the datatypes or functions of the CMSIS-DSP, you don't need to include the `arm_math.h` in the scheduler file. This option can thus be set to `False`.

### asynchronous (default = False)

When true, the scheduling is for a dynamic / asynchronous flow. A node may not always produce or consume the same amount of data. As consequence, a scheduling can fail. Each node needs to implement a `prepareForRunning` function to identify and recover from FIFO underflows and overflows.

A synchronous schedule is used as start and should describe the average case.

This implies `codeArray` and `switchCase`. This disables `memoryOptimizations`.

Synchronous FIFOs that are just buffers will be considered as FIFOs in asynchronous mode.

More info are available in the documentation for [this mode](../Async.md).

### FIFOIncrease (default 0)

In case of dynamic / asynchronous scheduling, the FIFOs may need to be bigger than what is computed assuming a static / synchronous scheduling. This option is used to increase the FIFO size. It represents a percent increase.

For instance, a value of `10` means the FIFO will have their size updated from `oldSize` to `1.1 * oldSize` which is ` (1 + 10%)* oldSize`

If the value is a `float` instead of an `int` it will be used as is. For instance, `1.1` would increase the size by `1.1` and be equivalent to the setting `10` (for 10 percent).

### asyncDefaultSkip (default True)

Behavior of a pure function (like CMSIS-DSP) in asynchronous mode. When `True`, the execution is skipped if the function can't be executed. If `False`, an error is raised.

If another error recovery is needed, the function must be packaged into a C++ class to implement a `prepareForRun` function.