# Cyclo static scheduling

This feature is illustrated in the  [cyclo](examples/cyclo/README.md) example.

Beginning with the version `1.7.0` of the Python wrapper and version >= `1.12` of CMSIS-DSP, cyclo static scheduling has been added.

## What is the problem it is trying to solve ?

Let's consider a sample rate converter from 48 kHz to 44.1 kHz.

For each input sample, on average it produces 44.1 / 48 = 0.91875 samples.

There are two ways to do this:

- One can observe that 48000/44100 = 160/147. So each time 160 samples are consumed, 147 samples are produced
- The number of sample produced can vary from one execution of the node to the other so that on average, 0.91875 samples are generated per execution

In the first case, it is synchronous but you need to wait for 160 input samples before being able to do some processing. It is introducing a latency and depending on the sample rate and use case, this latency may be too big. We need more flexibility.

In the second case, we have the flexibility but it is no more synchronous because the resampler is not producing the same amount of samples at each execution.

But we can observe that even if is is no more stationary, it is periodic. After consuming 160 samples the behavior should repeat.

One can use the resampler in the [SpeexDSP](https://gitlab.xiph.org/xiph/speexdsp) project to test. If we decide to consume only 40 samples in input to have less latency, then the resampler of SpeexDSP will produce 37,37,37 and 36 samples for the first 4 executions.

And (40+40+40+40)/(37+37+37+36) = 160 / 147.

So the flow of data on the output is not static but it is periodic.

This is now supported in the CMSIS-DSP compute graph and on each IO one can define a period. For this example, it could be:

```python
b=Sampler("sampler",floatType,40,[37,37,37,36])
```

Note that in the C++ class, the template parameters giving the number of samples produced or consumed on an IO cannot be used any more in this case. The value is still generated but now represent the maximum on a period.

And, in the run function you need to pass the number of sample read or written to the read and write buffer functions:

```c
this->getWriteBuffer(nbOfSamplesForCurrentExecution)
```

For synchronous node, nothing is changed and they are coded as in the previous versions.

The drawback of cyclo static scheduling is that the schedule length is increased. If we take the first example with a source producing 5 samples and a node consuming 7 samples and if the source is replaced by another source producing [5,5] then it is not equivalent. In the first case we can have only one execution of the source. In the second case, the scheduling will always contain an even number of executions of the sources. So the schedule length will be bigger. But memory usage will be the same (FIFOs of same size).

Since schedule tend to be bigger with cyclo static scheduling, a new code generation mode has been introduced and is enabled by default : now instead of having a sequence of function calls, the schedule is coded by an array of number and there is a switch / case to select the function to be called.