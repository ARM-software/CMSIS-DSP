# API of the Graph Class

## Creating a connection

Those methods must be applied to a graph object created with `Graph()`. The `Graph` class is defined inside `cmsisdsp.cg.scheduler` from the CMSIS-DSP Python wrapper.

```python
def connect(self,input_io,output_io,fifoClass=None,fifoScale = 1.0):
```

Typically this method is used as:

```python
the_graph = Graph()

# Connect the source output to the processing node input
the_graph.connect(src.o,processing.i)
```

There are two optional arguments:

* `fifoClass` : To use a different C++ class for implementing the connection between the two IOs. (it is also possible to change the FIFO class globally by setting an option on the graph. See below). Only the `FIFO` class is provided by default. Any new implementation must inherit from `FIFObase<T>`
* `fifoScale` : In asynchronous mode, it is a scaling factor to increase the length of the FIFO compared to what has been computed by the synchronous approximation. This setting can also be set globally using the scheduler options. `fifoScale` is overriding the global setting. It must be a `float` (not an `int`).

```python
def connectWithDelay(self,input_io,output_io,delay,fifoClass=None,fifoScale=1.0):  
```

The only difference with the previous function is the `delay` argument. It could be used like:

```python
the_graph.connect(src.o,processing.i, 10)
```

The `delay` is the number of samples contained in the FIFO at start (initialized to zero). The FIFO length (computed by the scheduling) is generally bigger by this amount of sample. The result is that it is delaying the output by `delay` samples.

It is generally useful when the graph has some loops to make it schedulable.

## Options for the graph

Those options needs to be used on the graph object created with `Graph()`.

For instance :

```python
g = Graph()
g.defaultFIFOClass = "FIFO"
```

### defaultFIFOClass (default = "FIFO")

Class used for FIFO by default. Can also be customized for each connection (`connect` of `connectWithDelay` call).

### duplicateNodeClassName(default="Duplicate")

Prefix used to generate the duplicate node classes like `Duplicate2`, `Duplicate3` ...

