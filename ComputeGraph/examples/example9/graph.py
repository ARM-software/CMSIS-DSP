from cmsisdsp.cg.scheduler import *

### Define new types of Nodes 

class Node(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("ia",theType,inLength)
        self.addInput("ib",theType,inLength)
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
floatType=CType(F32)
src=Source("source",floatType,5)
b=ProcessingNode("filter",floatType,5,5)
sink=Sink("sink",floatType,5)

g = Graph()

g.connect(src.o,b.ia)
g.connect(b.o,sink.i)
# With less than 5, the tool cannot find a possible schedule
# and is generating a DeadLock error
g.connectWithDelay(b.o,b.ib,5)


print("Generate graphviz and code")

conf=Configuration()
conf.debugLimit=2
conf.cOptionalArgs="int someVariable"
#conf.displayFIFOSizes=True
# Prefix for global FIFO buffers
#conf.prefix="sched1"

#conf.dumpSchedule = True 
sched = g.computeSchedule(config=conf)
#print(sched.schedule)
print("Schedule length = %d" % sched.scheduleLength)
print("Memory usage %d bytes" % sched.memory)
#

#conf.codeArray=True
sched.ccode("generated",conf)

with open("test.dot","w") as f:
    sched.graphviz(f)

