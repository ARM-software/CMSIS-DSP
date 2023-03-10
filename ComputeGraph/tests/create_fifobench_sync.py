# Include definition of the nodes
from nodes import * 
# Include definition of the graph
from graph_bench_sync import * 

# Create a configuration object
conf=Configuration()
# The number of schedule iteration is limited to 1
# to prevent the scheduling from running forever
# (which should be the case for a stream computation)
conf.debugLimit=10
conf.customCName = "custom_bench.h"
conf.appNodesCName = "BenchAppNodes.h"
conf.cOptionalArgs=["float32_t* inputArray",
                    "float32_t* outputArray"
                   ]

# Compute a static scheduling of the graph 
# The size of FIFO is also computed
scheduling = the_graph.computeSchedule(config=conf)

# Print some statistics about the compute schedule
# and the memory usage
print("Schedule length = %d" % scheduling.scheduleLength)
print("Memory usage %d bytes" % scheduling.memory)

# Generate the C++ code for the static scheduler
scheduling.ccode("fifobench_sync",conf)

# Generate a graphviz representation of the graph
with open("dot/fifobench_sync.dot","w") as f:
    scheduling.graphviz(f)

