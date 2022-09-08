from cmsisdsp.sdf.scheduler import *

### Define new types of Nodes 

class Node(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("i",theType,inLength)
        self.addOutput("o",theType,outLength)

class Sink(GenericSink):
    def __init__(self,name,theType,inLength):
        GenericSink.__init__(self,name)
        self.addInput("i",theType,inLength)

    @property
    def typeName(self):
        return "Sink"

class Source(GenericSource):
    def __init__(self,name,theType,inLength):
        GenericSource.__init__(self,name)
        self.addOutput("o",theType,inLength)

    @property
    def typeName(self):
        return "Source"

class ProcessingNode(Node):
    @property
    def typeName(self):
        return "ProcessingNode"



### Define nodes
# Node datatype
# WARNING : In Python there is reference semantic for
# the objects. But in C++, the struct have value semantic.
# So in Python implementation of the node, the reference
# shoudl never be shared. 
# Modify the fields of the objects, or create a totally new
# object.

complexType=CStructType("complex","MyComplex",8)

src=Source("source",complexType,5)
b=ProcessingNode("filter",complexType,7,5)
b.addLiteralArg(4)
b.addLiteralArg("Test")
b.addVariableArg("someVariable")
na = Sink("sa",complexType,5)
nb = Sink("sb",complexType,5)
nc = Sink("sc",complexType,5)
dup=Duplicate3("dup",complexType,5)

g = Graph()

g.connect(src.o,b.i)
g.connect(b.o,dup.i)
g.connect(dup.oa,na.i)
g.connect(dup.ob,nb.i)
g.connect(dup.oc,nc.i)


print("Generate graphviz and code")

conf=Configuration()
conf.debugLimit=1
conf.cOptionalArgs="int someVariable"
#conf.displayFIFOSizes=True
# Prefix for global FIFO buffers
#conf.prefix="sched1"

#print(g.nullVector())
sched1 = g.computeSchedule()
#print(sched.schedule)
print("Schedule length = %d" % sched1.scheduleLength)
print("Memory usage %d bytes" % sched1.memory)
#

#conf.codeArray=True
# C++ implementation
sched1.ccode("generated",conf)

sched2 = g.computeSchedule()
# Python implementation
#conf.prefix="sched1"
conf.pyOptionalArgs="someVariable"
#conf.dumpFIFO=True
sched2.pythoncode(".",config=conf)

with open("test.dot","w") as f:
    sched1.graphviz(f)

