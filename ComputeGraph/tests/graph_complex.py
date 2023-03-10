# Include definitions from the Python package to
# define datatype for the IOs and to have access to the
# Graph class
from cmsisdsp.cg.scheduler import *
# Include definition of the nodes
from nodes import * 

class ProcessingNode12(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("i",theType,inLength)
        self.addOutput("oa",theType,outLength)
        self.addOutput("ob",theType,outLength)

    @property
    def typeName(self):
        return "ProcessingNode12"

class ProcessingNode13(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("i",theType,inLength)
        self.addOutput("oa",theType,outLength)
        self.addOutput("ob",theType,outLength)
        self.addOutput("oc",theType,outLength)

    @property
    def typeName(self):
        return "ProcessingNode13"

class ProcessingNode21(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("ia",theType,inLength)
        self.addInput("ib",theType,inLength)
        self.addOutput("o",theType,outLength)

    @property
    def typeName(self):
        return "ProcessingNode21"


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

class ProcessingNode(GenericNode):
    def __init__(self,name,theType,inLength,outLength):
        GenericNode.__init__(self,name)
        self.addInput("i",theType,inLength)
        self.addOutput("o",theType,outLength)

    @property
    def typeName(self):
        return "ProcessingNode"



### Define nodes
floatType=CType(F32)
src=Source("source",floatType,128)
srcb=Source("sourceb",floatType,16)
srcc=Source("sourcec",floatType,16)

pa=ProcessingNode("procA",floatType,128,128)
pb=ProcessingNode("procB",floatType,128,128)
pc=ProcessingNode("procC",floatType,128,128)
pd=ProcessingNode("procD",floatType,128,128)
pe=ProcessingNode("procE",floatType,128,256)

p12=ProcessingNode12("proc12",floatType,16,16)
p13=ProcessingNode13("proc13",floatType,16,16)
p21A=ProcessingNode21("proc21A",floatType,16,16)
p21B=ProcessingNode21("proc21B",floatType,16,16)

#dsp=Dsp("add",floatType,NB)
sink=Sink("sink",floatType,100)
sinkb=Sink("sinkB",floatType,16)
sinkc=Sink("sinkC",floatType,16)
sinkd=Sink("sinkD",floatType,16)
sinke=Sink("sinkE",floatType,16)

the_graph = Graph()

the_graph.connect(src.o,pa.i)
the_graph.connect(pa.o,pb.i)
the_graph.connect(pb.o,pc.i)
the_graph.connect(pc.o,pd.i)
the_graph.connect(pd.o,pe.i)
the_graph.connect(pe.o,sink.i)

the_graph.connect(pc.o,p12.i)
the_graph.connect(pc.o,p13.i)

the_graph.connect(pd.o,p21A.ia)
the_graph.connect(p12.oa,p21A.ib)
the_graph.connect(p12.ob,p21B.ia)

the_graph.connect(p13.oa,p21B.ib)
the_graph.connect(p13.ob,sinkb.i)
the_graph.connect(p13.oc,sinkc.i)

the_graph.connect(p21A.o,sinkd.i)
the_graph.connect(p21B.o,sinke.i)


