# Description of the nodes

The generic and function nodes are the basic nodes that you use to create other kind of nodes in the graph.

There are 3 generic classes provided by the framework to be used to create new nodes.

To create a new kind of node, you inherit from one of those classes:

* `GenericSource`
* `GenericNode`
* `GenericSink`

They are defined in `cmsisdsp.cg.scheduler`.

There are 3 other classes that can be used to create new nodes from functions. A function has no state and a C++ wrapper is not required. In this case, the tool is generating code for calling the function directly rather than using a C++ wrapper.

* `Unary` (unary operators like `negate`, `inverse` ...)
* `Binary` (binary operators like `add`, `mul` ...)
* `Dsp` (Some CMSIS-DSP function either binary or unary)

# Generic Nodes

When you define a new kind of node, it must inherit from one of those classes. Those classes are providing the methods `addInput` and/or `addOutput` to define new inputs / outputs.

A new kind of node is generally defined as:

```python
class ProcessingNode(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("i",theType,inLength)
        self.addOutput("o",theType,outLength)

    @property
    def typeName(self):
        return "ProcessingNode"
```

The method `typeName` from the parent class must be overridden and provide the name of the `C++` wrapper to be used for this node.

The object constructor is defining the inputs / outputs : number of samples and datatype.

The object constructor is also defining the name used to identity this node in the generated code (so it must be a valid C variable name).

`GenericSink` is only providing the `addInput` function.

`GenericSource` is only providing the `addOutput` function

`GenericNode` is providing both.

You can use each function as much as you want to create several inputs and / or several outputs for a node.

See the [simple](../examples/simple/README.md) example for more explanation about how to define a new node.

## Methods

The constructor of the node is using the `addInput` and/or `addOutput` to define new IOs.

```python
def addInput(self,name,theType,theLength):
```

* `name` is the name of the input. It will becomes a property of the Python object so it must not conflict with existing properties. If `name` is, for instance, `"i"` then it can be accessed with `node.i` in the code
* `theType` is the datatype of the IO. It must inherit from `CGStaticType` (see below for more details about defining the types)
* `theLength` is the amount of **samples** consumed by this IO at each execution of the node

```python
def addOutput(self,name,theType,theLength):
```

* `name` is the name of the output. It will becomes a property of the Python object so it must not conflict with existing properties. If `name` is, for instance, `"o"` then it can be accessed with `node.o` in the code
* `theType` is the datatype of the IO. It must inherit from `CGStaticType` (see below for more details about defining the types)
* `theLength` is the amount of **samples** produced by this IO at each execution of the node

```python
@property
def typeName(self):
    return "ProcessingNode"
```

This method defines the name of the C++ class implementing the wrapper for this node.

# Datatypes

Datatypes for the IOs are inheriting from `CGStaticType`.

Currently there are 3 classes defined in the project:

* `CType` for the standard CMSIS-DSP types like `q15_t`, `float32_t` ...
* `CStructType` for a C struct
* `PythonClassType` to create structured datatype for the Python scheduler

## CType

You create such a type with `CType(id)` where `id` is one of the constant coming from the Python wrapper:

* F64
* F32
* F16
* Q31
* Q15
* Q7
* UINT32
* UINT16
* UINT8
* SINT32
* SINT16
* SINT8

For instance, to define a `float32_t` type for an IO you can use `CType(F32)`

## CStructType

The constructor has the following definition

```python
def __init__(self,name,size_in_bytes): 
```

* `name` is the name of the C struct
* `size_in_bytes` is the size of the C struct. It should take into account padding. It is used when the compute graph memory optimization is used since size of the datatype is needed. 

## PythonClassType

```python
def __init__(self,python_name)
```

In Python, there is no `struct`. This datatype is mapped to an object. Object have reference type. Compute graph FIFOs are assuming a value type semantic.

As consequence, in Python side you should never copy those structs since it would copy the reference. You should instead copy the members of the struct and they should be scalar values.

# Function and constant nodes

A Compute graph C++ wrapper is useful when the software components you use have a state that needs to be initialized in the C++ constructor, and preserved between successive calls to the `run` method of the wrapper.

Most CMSIS-DSP functions have no state. The compute graph framework is providing some ways to easily use functions in the graph without having to write a wrapper.

This feature is relying on the nodes:

* `Unary`
  * To use an unary operator like `negate`, `inverse` ...

* `Binary`
  * To use a binary operator like `add`, `mul` ...

* `Dsp`
  * Should detect if the CMSIS-DSP operator is unary or binary and use the datatype to compute the name of the function. In practice, only a subset of CMSIS-DSP function is supported so you should use `Unary` or `Binary` nodes

* `Constant`
  * Special node to be used **only** with function nodes when some arguments cannot be connected to a FIFO. For instance, with `arm_scale_f32` the scaling factor is a scalar value and a FIFO cannot be connected to this argument. The function is a binary operator but between a stream and a scalar.


All of this is explained in detail in the [simple example with CMSIS-DSP](../examples/simpledsp/README.md).

