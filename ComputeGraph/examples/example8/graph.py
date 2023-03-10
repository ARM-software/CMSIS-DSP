from cmsisdsp.cg.scheduler import *

### Define new types of Nodes 

class ProcessingNode(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("i",theType,inLength)
        self.addOutput("oa",theType,outLength)
        self.addOutput("ob",theType,outLength)

    @property
    def typeName(self):
        return "ProcessingNode"

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



### Define nodes
# Node datatype
# WARNING : In Python there is reference semantic for
# the objects. But in C++, the struct have value semantic.
# So in Python implementation of the node, the reference
# shoudl never be shared. 
# Modify the fields of the objects, or create a totally new
# object.

GEN_PYTHON = False 


if GEN_PYTHON:
   complexType=PythonClassType("MyComplex")
else:
   complexType=CStructType("complex",8)

src=Source("source",complexType,5)
b=ProcessingNode("filter",complexType,7,5)
b.addLiteralArg(4)
b.addLiteralArg("Test")
b.addVariableArg("someVariable")
na = Sink("sa",complexType,5)
nb = Sink("sb",complexType,5)
nc = Sink("sc",complexType,5)
nd = Sink("sd",complexType,5)


#dup=Duplicate3("dup",complexType,5)

g = Graph()

g.connect(src.o,b.i)
#g.connect(b.o,dup.i)
#g.connect(dup.oa,na.i)
#g.connect(dup.ob,nb.i)
#g.connect(dup.oc,nc.i)

g.connect(b.oa,na.i)
g.connect(b.oa,nb.i)
g.connect(b.oa,nc.i)

g.connect(b.ob,nd.i)


print("Generate graphviz and code")

conf=Configuration()
#conf.dumpSchedule = True
sched = g.computeSchedule(conf)
print("Schedule length = %d" % sched.scheduleLength)
print("Memory usage %d bytes" % sched.memory)

# Generation of the schedule is modifying the original graph
# (Introduction of duplicate nodes ...)
# So we cannot reuse the graph to compute the Python and the C
# code generation
conf.debugLimit=1
conf.cOptionalArgs="int someVariable"
#conf.codeArray=True
conf.memoryOptimization=True

if not GEN_PYTHON:
   # C++ implementation
   sched.ccode("generated",conf)
else:
   # Python implementation
   conf.pyOptionalArgs="someVariable"
   sched.pythoncode(".",config=conf)


# When true it is displaying the name of the FIFO buffer
# When false it is displaying the size of the FIFO (default)
conf.displayFIFOBuf=False
with open("test.dot","w") as f:
    sched.graphviz(f,config=conf)

