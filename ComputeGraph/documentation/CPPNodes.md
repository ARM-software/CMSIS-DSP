# CPP Nodes and classes

## Mandatory classes

Those classes are defined in `GenericNodes.h` a header that is always included by the scheduler.

As consequence, the definition for those classes is always included : that's the meaning of mandatory.

### FIFO

FIFO classes are inheriting from the virtual class `FIFOBase`:

```C++
template<typename T>
class FIFOBase{
public:
    virtual T* getWriteBuffer(int nb)=0;
    virtual T* getReadBuffer(int nb)=0;
    virtual bool willUnderflowWith(int nb) const = 0;
    virtual bool willOverflowWith(int nb) const = 0;
    virtual int nbSamplesInFIFO() const = 0;

};
```

The functions `willUnderflowWith`, `willOverflowWith` and `nbSamplesInFIFO` are only used in asynchronous mode.

If you implement a FIFO for synchronous mode you only need to implement `getWriteBuffer` and `getReadBuffer`.

FIFO must be templates with a type defined as:

```C++
template<typename T, int length, int isArray=0, int isAsync = 0>
class FIFO;
```

* `T` is a C datatype that must have value semantic : standard C type like `float` or `struct`
* `length` is the length of the FIFO in **samples**
* `isArray` is set to 1 when the scheduler has identified that the FIFO is always used as a buffer. So it is possible to provide a more optimized implementation for this case
* `isAsync` is set to 1 for the asynchronous mode

If you implement you own FIFO class, it should come from a template with the same arguments. For instance:

```C++
template<typename T, int length, int isArray=0, int isAsync = 0>
class MyCustomFIFO;
```

and it should inherit from `FIFOBase<T>`.

`GenericNodes.h` is providing 3 default implementations. Their are specialization of the FIFO template:

#### FIFO for synchronous mode

```C++
template<typename T, int length>
class FIFO<T,length,0,0>: public FIFOBase<T> 
```

#### Buffer for synchronous mode

In some case a FIFO is just used as a buffer. An optimized implementation for this case is provided

```C++
template<typename T, int length>
class FIFO<T,length,1,0>: public FIFOBase<T> 
```

In this mode, the FIFO implementation is very light. For instance, for `getWriteBuffer` we have:

```C++
T * getWriteBuffer(int nb) const final
{
    return(mBuffer);
};
```

#### FIFO for asynchronous mode

```C++
template<typename T, int length>
class FIFO<T,length,0,1>: public FIFOBase<T> 
```

This implementation is a bit more heavy and is providing implementations of following function that is doing something useful :

```C++
bool willUnderflowWith(int nb) const;
bool willOverflowWith(int nb) const;
int nbSamplesInFIFO() const;
```

### Nodes

Nodes are inheriting from the virtual class:

```C++
class NodeBase
{
public:
    virtual int run()=0;
    virtual int prepareForRunning()=0;
};
```

`GenericNode`, `GenericSource` and `GenericSink` are providing accesses to the FIFOs for each IO. The goal of those wrappers is to define the IOs (number of IO, their type and length) and hide the API to the FIFOs.

There are different versions depending on the number of inputs and/or output. Other nodes of that kind can be created by the user if different IO configurations are required:

#### GenericNode

The template is:

```C++
template<typename IN,  int inputSize,
         typename OUT, int outputSize>
class GenericNode:public NodeBase
```

There is one input and one output.

The constructor is:

```C++
GenericNode(FIFOBase<IN> &src,FIFOBase<OUT> &dst);
```

It is taking the input and output FIFOs as argument. The real type of the FIFO is hidden since the type `FIFOBase` is used. So `GenericNode` can be used with any FIFO implementation.

The main role of this `GenericNode` class is to provide functions to connect to the FIFOs.

The functions to access the FIFO buffers are:

```C++
OUT * getWriteBuffer(int nb = outputSize);
IN * getReadBuffer(int nb = inputSize);
```

`getWriteBuffer` is getting a pointer to a buffer of length `nb` to write the output samples.

`getReadBuffer` is getting a pointer to a buffer of length `nb` to read the input samples.

`nb` must be chosen so that there is no underflow / overflow. In synchronous mode, it will work by design if the length defined in the template argument is used.  The template length is thus chosen as default value for `nb`.

This value may be changed in cyclo-static or asynchronous mode. In asynchronous mode, additional functions are provided to test for a possibility of underflow / overflow **before** getting a pointer to the buffer.

It is done with following function that are also provided by `GenericNode`:

```C++
bool willOverflow(int nb = outputSize);
bool willUnderflow(int nb = inputSize);
```

All of those functions introduced by `GenericNode` are doing nothing more than calling the underlying FIFO methods. But they hide those FIFOs from the user code. The FIFO can only be accessed through those APIs.

#### GenericNode12

Same as `GenericNode` but with two outputs.

```C++
template<typename IN, int inputSize,
         typename OUT1, int output1Size
         typename OUT2, int output2Size>
class GenericNode12:public NodeBase
```

It provides:

```C++
IN * getReadBuffer(int nb=inputSize);
OUT1 * getWriteBuffer1(int nb=output1Size);
OUT2 * getWriteBuffer2(int nb=output2Size);

bool willUnderflow(int nb = inputSize);
bool willOverflow1(int nb = output1Size);
bool willOverflow2(int nb = output2Size);
```

#### GenericNode13

Same but with 3 outputs.

#### GenericNode21

Same but with 2 inputs and 1 output.

#### GenericSource

Similar to a `GenericNode` but there is no inputs.

#### GenericSink

Similar to a `GenericNode` but there is no outputs.

#### Duplicate2

This node is duplicating its input to 2 outputs.

The template is:

```C++
template<typename IN, int inputSize,
         typename OUT1,int output1Size,
         typename OUT2,int output2Size>
class Duplicate2;
```

Only one specialization of this template makes sense : the output must have same type and same length as the input. 

```C++
template<typename IN, int inputSize>
class Duplicate2<IN,inputSize,
                 IN,inputSize,
                 IN,inputSize> : 
public GenericNode12<IN,inputSize,
                     IN,inputSize,
                     IN,inputSize>
```



#### Duplicate3

Similar to `Duplicate2` but with 3 outputs.

## Optional nodes

Those nodes are not included by default. They can be found in `ComputeGraph/cg/nodes/cpp`

To use any of them you just need to include the header (for instance in your `AppNodes.h` file):

```C++
#include "CFFT.h"
```

### CFFT / CIFFT

Those nodes are for using the CMSIS-DSP FFT.

Template:

```C++
template<typename IN, int inputSize,
         typename OUT,int outputSize>
class CFFT;
```

Specialization provided only for `float32_t`, `float16_t`,`q15_t`.

The wrapper is copying the input buffer before doing the FFT (since CMSIS-DSP FFT is modifying the input buffer). It is normally possible to modify the input buffer even if it is in the input FIFO.

This implementation has made the choice of not touching the input FIFO with the cost of an additional copy.

Other data types can be easily added based on the current provided example. The user can just implement other specializations.

`CIFFT` is defined with class `CIFFT`.

### InterleavedStereoToMono

Deinterleave a stream of stereo samples to **one** stream of mono samples.

Template:

```C++
template<typename IN, int inputSize,
         typename OUT,int outputSize>
class InterleavedStereoToMono;
```

For specialization `q15_t` and `q31_t`, the inputs are divided by 2 before being added to avoid any overflow.

For specialization `float32_t` : The output is multiplied by `0.5f` for consistency for the fixed point version.

### MFCC

Those nodes are for using the CMSIS-DSP MFCC.

Template:

```C++
template<typename IN, int inputSize,
         typename OUT,int outputSize>
class MFCC;
```

Specializations provided for `float32_t`, `float16_t`, `q31_t` and `q15_t`.

The MFCC is requiring a temporary buffer. The wrappers are thus allocating a memory buffer during initialization of the node.

The buffer is allocated as a C++ vector. See the documentation of the MFCC in CMSIS-DSP to know more about the size of this buffer.

### NullSink

Template:

```C++
template<typename IN, int inputSize>
class NullSink: public GenericSink<IN, inputSize>
```

It is useful for development and debug. This node is doing nothing and just consuming its input.

### OverlapAndAdd

Template:

```c++
template<typename IN,int windowSize, int overlap>
class OverlapAdd: public GenericNode<IN,windowSize,IN,windowSize-overlap>
```

There are two sizes in the template arguments : `windowSize` and `overlap`.

From those size, the template is computing the number of samples consumed and produced by the node.

The implementation is generic but will only build for a type `IN` having an addition operator.

This node is using a little memory (C++ vector) of size `overlap` that is allocated during creation of the node.

This node will overlap input data by `overlap` samples and add the common overlapping samples.

### SlidingBuffer

Template:

```C++
template<typename IN,int windowSize, int overlap>
class SlidingBuffer: public GenericNode<IN,windowSize-overlap,IN,windowSize>

```

There are two sizes in the template arguments :  `windowSize` and `overlap`.

For those size, the template is computing the number of samples consumed and produced by the node.

The implementation is generic and will work with all types.

This node is using a little memory (C++ vector) of size `overlap` allocated during creation of the node.

This node is moving a window on the input data with an overlap. The output data is the content of the window.

Note that this node is not doing any multiplication with window functions that can be found in signal processing literature. This multiplication has to be implemented in the compute graph in a separate node.

### ToComplex

Template:

```C++
template<typename IN, int inputSize,
         typename OUT,int outputSize>
class ToComplex;
```

Convert a stream of reals a b c d ... to complexes a 0 b 0 c 0 d 0 ...

The implementation is generic and does not enforce the required size constraints.

### ToReal

Template:

```C++
template<typename IN, int inputSize,typename OUT,int outputSize>
class ToReal;
```

Convert a stream of complex a 0 b 0 c 0 ... to reals a b c ...

The implementation is generic and does not enforce the required size constraints.

### Unzip

Template:

```C++
template<typename IN, int inputSize,
         typename OUT1,int output1Size,
         typename OUT2,int output2Size>
class Unzip;
```

Unzip a stream a1 a2 b1 b2 c1 c2 ...

Into 2 streams:
a1 b1 c1 ...
a2 b2 c2 ...

The implementation is generic and does not enforce the required size constraints.

### Zip

Template:

```C++
template<typename IN1, int inputSize1,
         typename IN2,int inputSize2,
         typename OUT,int outputSize>
class Zip;
```

Transform two input streams:

a1 b1 c1 ...

a2 b2 c2 ...

into one output stream:

a1 a2 b1 b2 c1 c2 ...

The implementation is generic and does not enforce the required size constraints

### Host

Those nodes are for host (Windows, Linux, Mac). They can be useful to experiment with a compute graph.

By default there is no nodes to read / write `.wav` files but you can  easily add some if needed (`dr_wav.h` is a simple way to add `.wav` reading / writing and is freely available from the web).

#### FileSink

Template

```C++
template<typename IN, int inputSize>
class FileSink: public GenericSink<IN, inputSize>
```

Write the input samples to a file. The implementation is generic and use iostream for writing the datatype.

The constructor has an additional argument : the name/path of the output file:

```C++
FileSink(FIFOBase<IN> &src, std::string name)
```

#### FileSource

Template:

```C++
template<typename OUT,int outputSize> class FileSource;
```

There is only one specialization for the `float32_t` type.

It is reading text file with one float per file and generating a stream of float.

At the end of file, 0s are generated on the output indefinitely.

The constructor has an additional argument : the name/path of the input file:

```C++
FileSource(FIFOBase<float32_t> &dst,std::string name)
```

