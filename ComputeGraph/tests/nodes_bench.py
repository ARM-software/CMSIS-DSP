# Include definitions from the Python package
from cmsisdsp.cg.scheduler import GenericNode,GenericSink,GenericSource


class ArraySink(GenericSink):
    def __init__(self,name,theType,inLength):
        GenericSink.__init__(self,name)
        self.addInput("i",theType,inLength)

    @property
    def typeName(self):
        """The name of the C++ class implementing this node"""
        return "ArraySink"

class ArraySource(GenericSource):
    def __init__(self,name,theType,outLength):
        GenericSource.__init__(self,name)
        self.addOutput("o",theType,outLength)

    @property
    def typeName(self):
        """The name of the C++ class implementing this node"""
        return "ArraySource"

