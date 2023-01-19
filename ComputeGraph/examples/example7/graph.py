import numpy as np 

from cmsisdsp.cg.scheduler import *


class Processing(GenericNode):
    def __init__(self,name,outLength):
        GenericNode.__init__(self,name)
        self.addInput("i",CType(Q15),outLength)
        self.addOutput("o",CType(Q15),outLength)

    @property
    def typeName(self):
        return "Processing"


BUFSIZE=128
### Define nodes
src=VHTSource("src",BUFSIZE,0)
processing=Processing("proc",BUFSIZE)
sink=VHTSink("sink",BUFSIZE,0)


g = Graph()

g.connect(src.o, processing.i)
g.connect(processing.o, sink.i)



print("Generate graphviz and code")



conf=Configuration()
#conf.dumpSchedule = True 
sched = g.computeSchedule(conf)
#print(sched.schedule)
print("Schedule length = %d" % sched.scheduleLength)
print("Memory usage %d bytes" % sched.memory)
#

# Pass the source and sink objects used to communicate with the VHT Modelica block
#conf.pyOptionalArgs=""
conf.pathToSDFModule="C:\\\\benchresults\\\\cmsis_docker\\\\CMSIS\\\\DSP\\\\SDFTools"
#conf.dumpFIFO=True
#conf.prefix="sched1"
sched.pythoncode(".",config=conf)

with open("test.dot","w") as f:
    sched.graphviz(f)

