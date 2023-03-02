# Generic Nodes

The generic node classes are used to build new kind of nodes. There are 3 classes provided by the framework :

* `GenericSource`
* `GenericNode`
* `GenericSink`

They are defined in `cmsisdsp.cg.scheduler`

Any new kind of node must inherit from one of those classes. Those classes are providing the methods `addInput` and/or `addOutput` to define new IOs.

The method `typeName` from the parent class must be overridden.

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

See the [simple](../examples/simple/README.md) example for more explanation about how to define a new node.

## Methods

The constructor of the node is using the `addInput` and/or `addOutput` to define new IOs.

```python
def addInput(self,name,theType,theLength):
```

* `name` is the name of the input. It will becomes a property of the Python object so it must not conflict with existing properties. If `name` is, for instance, "i" then it can be accessed with `node.i` in the code
* `theType` is the datatype of the IO. It must inherit from `CGStaticType` (see below for more details about defining the types)
* `theLength` is the amount of **samples** consumed by this IO at each execution of the node

```python
def addOutput(self,name,theType,theLength):
```

* `name` is the name of the input. It will becomes a property of the Python object so it must not conflict with existing properties. If `name` is, for instance, "o" then it can be accessed with `node.o` in the code
* `theType` is the datatype of the IO. It must inherit from `CGStaticType` (see below for more details about defining the types)
* `theLength` is the amount of **samples** produced by this IO at each execution of the node

```python
@property
def typeName(self):
    return "ProcessingNode"
```

This method defines the name of the C++ class implementing the wrapper for this node.

## Datatypes

Datatypes for the IOs are inheriting from `CGStaticType`.

Currently there are two classes defined:

* `CType` for the standard CMSIS-DSP types
* `CStructType` for a C struct

### CType

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

### CStructType

The constructor has the following definition

```python
def __init__(self,name,python_name,size_in_bytes): 
```

* `name` is the name of the C struct
* `python_name` is the name of the Python class implementing this type (when you generate a Python schedule)
* `size_in_bytes` is the size of the struct. It should take into account padding. It is used in case of buffer sharing since the datatype of the shared buffer is `int8_t`. The Python script must be able to compute the size of those buffers and needs to know the size of the structure.

In Python, there is no `struct`. This datatype is mapped to an object. Object have reference type. Compute graph FIFOs are assuming a value type semantic.

As consequence, in Python side you should never copy those structs since it would copy the reference. You should instead copy the members of the struct.

If you don't plan on generating a Python scheduler, you can just use whatever name you want for the `python_name`. It will be ignored by the C++ code generation.