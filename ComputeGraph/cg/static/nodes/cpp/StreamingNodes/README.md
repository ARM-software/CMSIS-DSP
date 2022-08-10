# Streaming Nodes

This is an example implementation to be used with Arm Virtual Hardware (AVH).

It is requiring headers and source files provided by AVH.

Those files are not needed at all to use the Compute Graph.

Those files are kept because they are used in the AVH-SystemModeling example.

But there are simpler way to interface the compute graph to an audio interrupt.

## RingBuffer

It is a way to connect the compute graph with static flow to an audio source or sink.

The node part is implemented in the AudioSource and AudioSink parts.


The main implementation is in `RingBuffer`.


To port it to new environments:

- A file RingPrivate.h must be written. It is included by RingBuffer.cpp to customize some part of the implementation 
- A file managing the audio interrupts must be written an interact with the Ring buffers
- The file RingConfig.h must be provided to size the buffers
- Most of the time the Ring buffer will have only 2 buffers and will be a ping-pong buffer.

