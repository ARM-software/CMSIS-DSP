# Schedule computation

## API

```python
def computeSchedule(self,config=Configuration()):
```

This is a method on the `Graph` object. It can take an optional `Configuration` object.

It returns a `Schedule` object. This object contains:

* A description of the static schedule
* The computed size of the FIFOs
* The FIFOs
* The buffers for the FIFOs (with sharing when possible if memory optimizations were enabled)
* A rewritten graph with `Duplicate` nodes inserted

## Options for the scheduling

Those options needs to be used on a configuration objects passed as argument of the scheduling function. For instance:

```python
conf = Configuration()
conf.debugLimit = 10
sched = g.computeSchedule(config = conf)
```

Note that the configuration object also contain options for the code generators. They are described in different part of the documentation.

### memoryOptimization (default = False)

When the amount of data written to a FIFO and read from the FIFO is the same, the FIFO is just an array. In this case, depending on the scheduling, the memory used by different arrays may be reused if those arrays are not needed at the same time.

This option is enabling an analysis to optimize the memory usage by merging some buffers when it is possible.

### sinkPriority (default = True)

Try to prioritize the scheduling of the sinks to minimize the latency between sources and sinks.

When  this option is enabled, the tool may not be able to find a schedule in all cases. If it can't find a schedule, it will raise a `DeadLock` exception.

### displayFIFOSizes (default = False)

During computation of the schedule, the evolution of the FIFO sizes is generated on `stdout`.

### dumpSchedule (default = False)

During computation of the schedule, the human readable schedule is generated on `stdout`.