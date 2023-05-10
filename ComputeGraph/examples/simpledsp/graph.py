# Include definitions from the Python package to define data types for the IOs 
# and to access to the classes for the Graph Graph.
from cmsisdsp.cg.scheduler import *

# Include definition of the processing nodes defined in 'nodes.py'.
from nodes import * 

# Define data type 'float' used for all IOs in this example
floatType=CType(F32)

# Instantiate a Source processing node that creates a packet of 5 samples.
# Each execution of the C function "source" generates 5 samples.
src=Source("source",floatType,5)

# Instantiate a binary operation processing node with 2 inputs and 1 output.
# Each execution of the C function "arm_offset_f32" consumes and produces 7 samples. 
processing=Binary("arm_offset_f32",floatType,7)

# Instantiate a processing node that produces a constant value which is defined
# with the C identifier "OFFSET_VALUE".
offsetValue=Constant("OFFSET_VALUE")

# Instantiate a Sink processing node that consumes a packet of 5 samples.
# Each execution of the C function "sink" gets 5 samples as input.
sink=Sink("sink",floatType,5)

# Create a Compute Graph object.
the_graph = Graph()

# Connect the source output to the input ia of the processing node.
the_graph.connect(src.o,processing.ia)

# Connect the constant offsetValues to the input ib of the processing node.
the_graph.connect(offsetValue,processing.ib)

# Connect the ouput of the processing node to the input of the sink.
the_graph.connect(processing.o,sink.i)


