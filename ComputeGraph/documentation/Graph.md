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