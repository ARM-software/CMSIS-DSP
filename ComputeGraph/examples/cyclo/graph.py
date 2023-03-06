# Include definitions from the Python package to
# define datatype for the IOs and to have access to the
# Graph class
from cmsisdsp.cg.scheduler import *
# Include definition of the nodes
from nodes import * 

# Define the datatype we are using for all the IOs in this
# example
floatType=CType(F32)

# Instantiate a Source node with a float datatype and
# working with packet of 5 samples (each execution of the
# source in the C code will generate 5 samples)
# "source" is the name of the C variable that will identify
# this node
src=Source("source",floatType,[3,2])
# Instantiate a Processing node using a float data type for
# both the input and output. The number of samples consumed
# on the input and produced on the output is 7 each time
# the node is executed in the C code
# "processing" is the name of the C variable that will identify
# this node
processing=ProcessingNode("processing",floatType,7,7)
# Instantiate a Sink node with a float datatype and consuming
# 5 samples each time the node is executed in the C code
# "sink" is the name of the C variable that will identify
# this node
sink=Sink("sink",floatType,5)

# Create a Graph object
the_graph = Graph()

# Connect the source to the processing node
the_graph.connect(src.o,processing.i)
# Connect the processing node to the sink
the_graph.connect(processing.o,sink.i)


