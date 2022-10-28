###########################################
# Project:      CMSIS DSP Library
# Title:        description.py
# Description:  Schedule generation
# 
# $Date:        29 July 2021
# $Revision:    V1.10.0
# 
# Target Processor: Cortex-M and Cortex-A cores
# -------------------------------------------------------------------- */
# 
# Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
# 
# SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
############################################
"""Description of the graph"""

import networkx as nx
import numpy as np 
from sympy import Matrix
from sympy.core.numbers import ilcm,igcd

import cmsisdsp.cg.static.scheduler.graphviz
import cmsisdsp.cg.static.scheduler.ccode
import cmsisdsp.cg.static.scheduler.pythoncode

from .node import *
from .config import *
from .standard import Duplicate2,Duplicate3

from ..types import *

# To debug graph coloring for memory optimization
#import matplotlib.pyplot as plt

class IncompatibleIO(Exception):
    pass

class GraphIsNotConnected(Exception):
    pass

class NotSchedulableError(Exception):
    pass

class DeadlockError(Exception):
    pass

class CannotDelayConstantError(Exception):
    pass

class TooManyNodesOnOutput(Exception):
    pass

class FifoBuffer:
    """Buffer used by a FIFO"""
    def __init__(self,bufferID,theType,length):
        self._length=length 
        self._theType=theType 
        self._bufferID=bufferID

class FIFODesc:
    """A FIFO connecting two nodes"""
    def __init__(self,fifoid):
        # The FIFO is in fact just an array
        self.isArray=False 
        # FIFO length
        self.length=0
        # FIFO type
        self.theType=None 
        # Buffer used by FIFO
        self.buffer=None 
        # Used for plot in graphviz
        self.bufferID=-1
        self._fifoID=fifoid 
        # Source IO
        self.src = None 
        # Dest IO
        self.dst = None 
        # FIFO delay
        self.delay=0
        self.fifoClass = "FIFO"

        # Used for liveliness analysis
        # To share buffers between FIFO in memory optimization
        # mode, we need to know when a FIFO is in use.
        # We compute the maximum extent : so the biggest interval
        # and not a disconnected union of intervals
        # This could be improved. We could use
        # a disjoint union of intervals but they should be mapped
        # to the same node in the interference graph
        self._liveInterval=(-1,-1)

        # shared buffer number not yet allocated
        self.sharedNB=-1

    # For c code generation 
    @property
    def isArrayAsInt(self):
        if self.isArray:
            return(1)
        else:
            return(0)

    @property
    def hasDelay(self):
        return(self.delay>0)

    def dump(self):
        
        print("array %d len %d %s id %d src %s:%s dst %s:%s  " % 
            (self.isArray,
             self.length,
             self.theType.ctype,
             self.fifoID,
             self.src.owner.nodeID,
             self.src.name,
             self.dst.owner.nodeID,
             self.dst.name))

    @property
    def fifoID(self):
        return self._fifoID
    
    def recordWrite(self,theTime):
        start,stop=self._liveInterval  
        if start==-1:
            self._liveInterval=(theTime,stop)

    def recordRead(self,theTime):
        start,stop=self._liveInterval 
        if (theTime > stop):
            self._liveInterval=(start,theTime)


def analyzeStep(vec,allFIFOs,theTime):
    """Analyze an evolution step to know which FIFOs are read and written to"""
    fifoID = 0 
    for fifo in (vec > 0):
        if fifo:
            allFIFOs[fifoID].recordWrite(theTime) 
        fifoID = fifoID + 1

    fifoID = 0 
    for fifo in (vec < 0):
        if fifo:
            allFIFOs[fifoID].recordRead(theTime) 
        fifoID = fifoID + 1

class Graph():

    def __init__(self):
        self._nodes={}
        self._edges={}
        self._delays={}
        self._constantEdges={}
        self._g = nx.Graph()
        self._sortedNodes=None
        self._totalMemory=0
        self._allFIFOs = None 
        self._allBuffers = None
        self._FIFOClasses = {}
        # Topological sorting of nodes
        # computed during topology matrix
        # and used for some scheduling
        # (prioritizing the scheduling of nodes
        # close to the graph output)
        self._topologicalSort=[]

    def computeTopologicalSortOfNodes(self):
        remaining = self._sortedNodes.copy()

        while len(remaining)>0:
            toremove = []
            for n in remaining:
                if n.nbOutputsForTopologicalSort == 0:
                    toremove.append(n)

            if (len(toremove)==0):
                # There are some loops so topological sort is
                # not possible 
                self._topologicalSort = [] 
                return


            self._topologicalSort.append(toremove)
            for x in toremove:
                remaining.remove(x)

            # Update output count for predecessors of the
            # removed nodes 
            for x in toremove:
                for i in x._inputs:
                    theFifo = x._inputs[i].fifo
                    #  Fifo empty means connection to a
                    # constant node so should be ignoredf
                    # for topological sort
                    if len(theFifo)>0:
                       theInputNodeOutput = theFifo[0]
                       theInputNode = theInputNodeOutput.owner
                       theInputNode.nbOutputsForTopologicalSort = theInputNode.nbOutputsForTopologicalSort - 1
        #print(self._topologicalSort)


    def connectDup(self,destination,outputIO,theId,fifoClass="FIFO"):
        if (destination[theId][1]!=0):
            self.connectWithDelay(outputIO,destination[theId][0],destination[theId][1],dupAllowed=False,fifoClass=destination[theId][2])
        else:
            self.connect(outputIO,destination[theId][0],dupAllowed=False,fifoClass=destination[theId][2])



    def insertDuplicates(self):
        # Insert dup nodes (Duplicate2 and Duplicate3) to
        # ensure that an output is connected to only one input
        dupNb = 0
        # Scan all nodes
        allNodes = list(self._g)
        for n in allNodes:
            # For each nodes, get the IOs
            for k in n._outputs.keys():
                output = n._outputs[k]
                fifo = output.fifo
                # Check if the FIFO list has only 1 element
                # In this case, we convert it into a value
                # since code using the graph is expecting an
                # IO to be connected top one and only one other IO
                # so the FIFO must be a value and not a list
                if len(fifo)==1:
                    # Extract first element of list
                    fifo = fifo[0]
                    fifo[0].fifo = fifo 
                    fifo[1].fifo = fifo
                # If the FIFO has more elements, we need to
                # restructure the graph and add Duplicate nodes
                else:
                    # Currently the library is only providing
                    # Duplicate2 and Duplicate3 nodes.
                    # So an output cannot be connected to more than
                    # 3 inputs
                    if (len(fifo)>3):
                        raise TooManyNodesOnOutput

                    dup = None 
                    # We extract from the IO the nb of produced
                    # samples and the data type
                    # Duplicate will use the same
                    inputSize = output.nbSamples
                    theType = output.theType

                    # We create a duplicate node
                    if len(fifo)==2:
                        dup = Duplicate2("dup%d" % dupNb,theType,inputSize)

                    if len(fifo)==3:
                        dup = Duplicate3("dup%d" % dupNb,theType,inputSize)

                    #print(dup)



                    dupNb = dupNb + 1
                    # We disconnect all the fifo element (a,b)
                    # and remember the destination b
                    # We must rewrite the edges of self._g
                    # self._edges 
                    # self._nodes
                    # the node fifo 
                    destinations = []
                    delays = []

                    self._sortedNodes = None
                    self._sortedEdges = None
                    for f in fifo:
                        # IO connected to the FIFOs
                        # nodea is the IO belowing to nodea
                        # but not the node itself
                        nodea = f[0]
                        nodeb = f[1]

                        fifoClass = "FIFO"
                        if (nodea,nodeb) in self._FIFOClasses:
                            fifoClass = self._FIFOClasses[(nodea,nodeb)]

                        
                        if (nodea,nodeb) in self._delays:
                           delay = self._delays[(nodea,nodeb)]
                        else:
                           delay = 0

                        destinations.append((nodeb,delay,fifoClass))

                        nodea.fifo=None 
                        nodeb.fifo=None
                        # Since we are not using a multi graph
                        # and there no parallel edges, it will
                        # remove all edges between two nodes.
                        # But some edges may come from other IO
                        # and be valid.
                        # Anyway, those edges are not used
                        # in the algorithm at all
                        # Instead we have our own graph with self._edges
                        # (This script will have to be rewritten in a much
                        # cleaner way)
                        if self._g.has_edge(nodea.owner,nodeb.owner):
                           self._g.remove_edge(nodea.owner,nodeb.owner)
                        del self._edges[(nodea,nodeb)]
                        if (nodea,nodeb) in self._FIFOClasses:
                            del self._FIFOClasses[(nodea,nodeb)]
                        if (nodea,nodeb) in self._delays:
                           del self._delays[(nodea,nodeb)]

                        
                    
                    # Now for each fifo destination we need 
                    # create a new
                    # connection dup -> b
                    # And we create a new connection a -> dup

                    self.connect(output,dup.i,dupAllowed=False)

                    if len(destinations)==2:
                       self.connectDup(destinations,dup.oa,0)
                       self.connectDup(destinations,dup.ob,1)
                       
                    if len(destinations)==3:
                       self.connectDup(destinations,dup.oa,0)
                       self.connectDup(destinations,dup.ob,1)
                       self.connectDup(destinations,dup.oc,2)
                        

               

    def connect(self,nodea,nodeb,dupAllowed=True,fifoClass="FIFO"):
        # When connecting to a constant node we do nothing
        # since there is no FIFO in this case
        # and it does not participate to the scheduling.
        if (isinstance(nodea,Constant)):
            nodeb.constantNode = nodea
            self._constantEdges[(nodea,nodeb)]=True
        else:
            if nodea.compatible(nodeb):
                self._sortedNodes = None
                self._sortedEdges = None
                self._g.add_edge(nodea.owner,nodeb.owner)
    
                if dupAllowed:
                   nodea.fifo.append((nodea,nodeb))
                   nodeb.fifo.append((nodea,nodeb))
                else:
                   nodea.fifo=(nodea,nodeb)
                   nodeb.fifo=(nodea,nodeb)
                self._edges[(nodea,nodeb)]=True
                self._FIFOClasses[(nodea,nodeb)] = fifoClass
                if not (nodea.owner in self._nodes):
                   self._nodes[nodea.owner]=True
                if not (nodeb.owner in self._nodes):
                   self._nodes[nodeb.owner]=True
            else:
                raise IncompatibleIO

    def connectWithDelay(self,nodea,nodeb,delay,dupAllowed=True,fifoClass="FIFO"):
        # We cannot connect with delay to a constant node
        if (isinstance(nodea,Constant)):
            raise CannotDelayConstantError
        else:
            self.connect(nodea,nodeb,dupAllowed=dupAllowed,fifoClass=fifoClass)
            self._delays[(nodea,nodeb)] = delay
    
    def __str__(self):
        res=""
        for (a,b) in self._edges: 
            nodea = a.owner
            nodeb = b.owner 

            res += ("%s.%s -> %s.%s\n" % (nodea.nodeID,a.name, nodeb.nodeID,b.name))

        return(res)

    def initializeFIFODescriptions(self,config,allFIFOs, fifoLengths,maxTime):
        """Initialize FIFOs datastructure""" 
        for fifo in allFIFOs:
            edge = self._sortedEdges[fifo.fifoID]
            fifo.length = fifoLengths[fifo.fifoID]
            src,dst = edge
            if edge in self._FIFOClasses:
               fifoClass = self._FIFOClasses[edge]
               fifo.fifoClass = fifoClass
            fifo.src=src
            fifo.dst=dst 
            fifo.delay=self.getDelay(edge)
            # When a FIFO is working as an array then its buffer may
            # potentially be shared with other FIFOs workign as arrays
            if src.nbSamples == dst.nbSamples:
                if fifo.delay==0:
                   fifo.isArray = True 
            fifo.theType = src.theType
            #fifo.dump()


        bufferID=0
        allBuffers=[]

        # Compute a graph describing when FIFOs are used at the same time
        # Then use graph coloring to allocate buffer to those FIFOs.
        # Then size the buffer based on the longest FIFO using it
        if config.memoryOptimization:
            G = nx.Graph()

            for fifo in allFIFOs: 
                    if fifo.isArray:
                        G.add_node(fifo)

            # Create the interference graph

            # Dictionary of active FIFOs at a given time.
            # The time is a scheduling step
            active={}
            currentTime=0
            while currentTime<=maxTime:
                # Remove fifo no more active.
                # Their stop time < currentTime
                toDelete=[]
                for k in active:
                    start,stop=k._liveInterval 
                    if stop<currentTime:
                        toDelete.append(k)
                for k in toDelete:
                    del active[k]   
    
                # Check FIFOs becoming active.
                # They are added to the active list
                # and an interference edge is added between thus FIFO
                # and all the FIFOs active at same time.
                for fifo in allFIFOs: 
                    if fifo.isArray:
                        start,stop=fifo._liveInterval
                        # If a src -> node -> dst
                        # At time t, node will read for src and the stop time
                        # will be currentTime t because once read
                        # the buffer can be reused.
                        # And it will write to dst and the start time will be
                        # currentTime because once written the buffer
                        # cannot be used again until it has been read.
                        # So, src and dst are both live at this time.
                        # Which means the condition on the stop time must be 
                        # stop >= currentTime and not a strict comparison
                        if start<=currentTime and stop >= currentTime:
                            if not (fifo in active):
                                for k in active:
                                    G.add_edge(k,fifo)
                                active[fifo]=True 
    
                currentTime = currentTime + 1

            # To debug and display the graph
            if False:
               import matplotlib.pyplot as plt
               labels={}
               for n in G.nodes:
                  labels[n]="%s -> %s" % (n.src.owner.nodeName,n.dst.owner.nodeName)
       
               pos = nx.spring_layout(G, seed=3113794652)
               subax1 = plt.subplot(121)
               nx.draw_networkx_edges(G, pos, width=1.0, alpha=0.5)
               
               nx.draw_networkx_labels(G, pos, labels, font_size=10)
               plt.show()
               quit()

        
            # Graph coloring
            d = nx.coloring.greedy_color(G, strategy="largest_first")

            # Allocate the colors (buffer ID) to the FIFO
            # and keep track of the max color number
            # Since other buffers (for real FIFOs) will have their
            # numbering start after this one.
            for fifo in d:
                fifo.sharedNB=d[fifo]
                bufferID=max(bufferID,fifo.sharedNB)



            # Compute the max size for each shared buffer
            maxSizes={} 
            for fifo in d:
                lengthInBytes = fifo.theType.bytes * fifo.length
                if fifo.sharedNB in maxSizes:
                    maxSizes[fifo.sharedNB] = max(maxSizes[fifo.sharedNB],lengthInBytes) 
                else:
                    maxSizes[fifo.sharedNB]=lengthInBytes

            # Create the buffers
            for theID in maxSizes:
              sharedA = FifoBuffer(theID,CType(UINT8),maxSizes[theID])
              allBuffers.append(sharedA)


        # bufferID must start after all shared buffers
        bufferID = bufferID + 1
        for fifo in allFIFOs:
            # Use shared buffer if memory optimization
            if fifo.isArray and config.memoryOptimization:
                fifo.buffer=allBuffers[fifo.sharedNB] 
                fifo.bufferID=fifo.sharedNB
            # Create a new buffer for a real FIFO
            # Use bufferID which is starting after the numbers allocated
            # to shared buffers
            else:
                buf = FifoBuffer(bufferID,fifo.theType,fifo.length)
                allBuffers.append(buf)
                fifo.buffer=buf
                fifo.bufferID = bufferID
                bufferID = bufferID + 1

        # Compute the total memory used in bytes
        self._totalMemory = 0
        for buf in allBuffers:
            self._totalMemory = self._totalMemory + buf._theType.bytes * buf._length

        #for fifo in allFIFOs:
        #    fifo.dump()
        
        return(allBuffers)




    @property
    def constantEdges(self):
        return list(self._constantEdges.keys())
    
    @property
    def nodes(self):
        return list(self._nodes.keys())

    @property
    def edges(self):
        return list(self._edges.keys())
    

    def hasDelay(self,edge):
        return(edge in self._delays)

    def getDelay(self,edge):
        if self.hasDelay(edge):
           return(self._delays[edge])
        else:
           return(0)

    def checkGraph(self):
        if not nx.is_connected(self._g):
            raise GraphIsNotConnected

    def topologyMatrix(self):
        self.checkGraph()
        # For cyclo static scheduling : compute the node periods
        for n in self.nodes:
            n.computeCyclePeriod()

        rows=[]
        # This is used in schedule generation
        # and all functions must use the same node ordering
        self._sortedNodes = sorted(self.nodes, key=lambda x: x.nodeID)
        # Arbitrary order but used for now
        self._sortedEdges = self.edges.copy()
        #for x in self._sorted:
        #    print(x.nodeID)

        # Compute sorted node ID
        nID = 0
        for node in self._sortedNodes:
            node.sortedNodeID = nID
            node.nbOutputsForTopologicalSort = node.nbOutputs
            nID = nID + 1

        for edge in self._sortedEdges: 
            na,nb = edge
            currentRow=[0] * len(self._sortedNodes) 

            ia=self._sortedNodes.index(na.owner)
            ib=self._sortedNodes.index(nb.owner)

            # Produced by na on the edge
            # for execution of one cycle of the na.owner node
            totalProduced = na.cycleTotal * na.owner.cyclePeriod // na.cyclePeriod
            currentRow[ia] = totalProduced

            # Consumed by nb on the edge
            # for execution of a full cycle of the node
            totalProduced = nb.cycleTotal * nb.owner.cyclePeriod // nb.cyclePeriod
            currentRow[ib] = -totalProduced

            rows.append(currentRow)

        return(np.array(rows))

    def nullVector(self,m):
        #print("Null vector")
        # m is topology matrix computed with toplogyMatrix
        r=Matrix(m).nullspace()
        if len(r) != 1:
           raise NotSchedulableError
        result=list(r[0])
        #print(result)
        denominators = [x.q for x in result]
        # Remove denominators
        ppcm = ilcm(*denominators)
        #print(ppcm)
        intValues = [x * ppcm for x in result]
        # Convert intValues to the smallest possible values
        gcd = igcd(*intValues)
        return([x // gcd for x in intValues])

    @property
    def initEvolutionVector(self):
        """Initial FIFO state taking into account delays"""
        return(np.array([self.getDelay(x) for x in self.edges]))

    def evolutionVectorForNode(self,nodeID,test=True):
        """Return the evolution vector corresponding to a selected node"""
        # For a simple static scheduling, the toplogy matrix T
        # is enough. If we know which node is executed, we have
        # a node vector V where there is a 1 at the position of the
        # execute node.
        # And the data generated on the fifos is:
        # T . V so the new fifo vector B' is given with
        # B' = T .V + B
        # But in case of cyclo static scheduling the topology
        # matrix is for a full period of the node
        # so 1 or several periods of each IO.
        # And we don't have the granularity corresponding to 
        # one node execution.
        # For one node execution, we need to know where we are
        # in the cycle on each IO
        # and where we are in the cycle for the node to
        # track how many full eriods of the node execution have been
        # done.
        # So this function is not just returning the node vector
        # and letting the main function updating the fifos.
        # This function is returning the new fifo state
        # Node vector would have been
        # v = np.zeros(len(self._sortedNodes))
        # v[nodeID] = 1 
        # for cyclo static scheduling
        n = self._sortedNodes[nodeID]
        #print(n)
        v = np.zeros(len(self._sortedEdges))

        # In test mode we are testing several nodes
        # to find the best one to schedule.
        # So we should not advance the cycle
        # of the IOs
        for i in n._inputs:
            io = n._inputs[i]
            edge = io._fifo
            # If 0, it is a connection to a constant node
            # so there is no FIFO
            if len(edge)>0:
               pos = self._sortedEdges.index(edge)
               v[pos] = -io.cycleValue
               if not test:
                  io.advanceCycle()
            

        for o in n._outputs:
            io = n._outputs[o]
            edge = io._fifo
            # If 0 it is a connection to a constant edge
            # so there is no FIFO
            if len(edge)>0:
               pos = self._sortedEdges.index(edge)
               v[pos] = io.cycleValue
               if not test:
                  io.advanceCycle()
            

        #print(v)
        return(v)

    def computeTopologicalOrderSchedule(self,normV,allFIFOs,initB,bMax,initN,config):
        b = np.array(initB)
        n = np.array(initN)


        schedule=[]

        zeroVec = np.zeros(len(self._sortedNodes))
        evolutionTime = 0
        #print(self._sortedNodes)
        # While there are remaining node periods to schedule
        while (n != zeroVec).any():
            #print("")
            #print(n)
            # Look for the best node to schedule
            # which is the one giving the minimum FIFO increase
            
            # None selected
            selected = -1

            # Min FIFO size found

            for layer in self._topologicalSort:
                minVal = 10000000
                selected = -1
                for node in layer:
                    # If the node can be scheduled
                    if n[node.sortedNodeID] > 0:
                       # Evolution vector for static scheduling
                       # v = self.evolutionVectorForNode(nodeID)
                       # for cyclo static we need the new fifo state
                       newB = self.evolutionVectorForNode(node.sortedNodeID) + b
                       # New fifos size after this evolution
                       # For static scheduling, fifo update would have been
                       # newB = np.dot(t,v) + b
                       #print(newB)
                
                       # Check that there is no FIFO underflow or overfflow:
                       if np.all(newB >= 0) and np.all(newB <= bMax):
                          # Total FIFO size for this possible execution
                          # We normalize to get the occupancy number as explained above
                          theMin = (1.0*np.array(newB) / normV).max()
                          # If this possible evolution is giving smaller FIFO size
                          # (measured in occupancy number) then it is selected
                          if theMin < minVal:
                             minVal = theMin
                             selected = node.sortedNodeID 
    
                       if selected != -1:
                          # We put the selected node at the end of the layer
                          # so that we do not always try the same node
                          # each time we analyze this layer
                          # If several nodes can be selected it should allow
                          # to select them with same probability
                          layer.remove(node)
                          layer.append(node)
                          break
                # For breaking from the outer loop too
                else:
                    continue
                break
                # No node could be scheduled because of not enough data
                # in the FIFOs. It should not occur if there is a null
                # space of dimension 1. So, it is probably a bug if
                # this exception is raised
            if selected < 0:
               raise DeadlockError
            
            # Now  we have evaluated all schedulable nodes for this run
            # and selected the one giving the smallest FIFO increase
    
            # Implementation for static scheduling
            # Real evolution vector for selected node
            # evol =  self.evolutionVectorForNode(selected)
            # Keep track that this node has been schedule
            # n = n - evol
            # Compute new fifo state
            # fifoChange = np.dot(t,evol)
            # b = fifoChange + b
    
            # Implementation for cyclo static scheduling
            #print("selected")
            fifoChange = self.evolutionVectorForNode(selected,test=False)
            b = fifoChange + b
            # For cyclo static, we create an evolution vector 
            # which contains a 1
            # at a node position only if this node has executed
            # its period.
            # Otherwise the null vector is not decreased
            v = np.zeros(len(self._sortedNodes))
            v[selected] = self._sortedNodes[selected].executeNode() 
            n = n - v
    
    
            if config.displayFIFOSizes:
               print(b)
                
            schedule.append(selected)
    
            # Analyze FIFOs to know if a FIFOs write is
            # followed immediately by a FIFO read of same size
            analyzeStep(fifoChange,allFIFOs,evolutionTime)
            evolutionTime = evolutionTime + 1
        return(schedule)

    def computeSchedule(self,config=Configuration()):
        # First we must rewrite the graph and insert duplication
        # nodes when an ouput is connected to several inputs.
        # After this transform, each output should be connected to
        # only one output.
        self.insertDuplicates()

        networkMatrix = self.topologyMatrix()
        #print(networkMatrix)

        if config.sinkPriority:
           self.computeTopologicalSortOfNodes()

        mustDoSinkPrioritization = config.sinkPriority and len(self._topologicalSort)>0
        if config.sinkPriority and not mustDoSinkPrioritization:
            print("Sink prioritization has been disabled. The graph has some loops")

        # Init values
        initB = self.initEvolutionVector
        initN = self.nullVector(networkMatrix)
        #print(initB)
        #print(initN)
        #print(np.dot(networkMatrix,initN))

        # nullVector is giving the number of repetitions
        # for a node cycle.
        # So the number of iteration of the node must be multiplied
        # by the cycle period for this node.
        #for nodeID in range(len(self._sortedNodes)):
        #    initN[nodeID] = initN[nodeID] * self._sortedNodes[nodeID].cyclePeriod


        # Current values (copys)
        b = np.array(initB)
        n = np.array(initN)

        if config.displayFIFOSizes:
           for edge in self._sortedEdges:
             print("%s:%s -> %s:%s" % (edge[0].owner.nodeID,edge[0].name,edge[1].owner.nodeID,edge[1].name))
           print(b)

        # Topology matrix
        t = np.array(networkMatrix)
        #print(t)
        #print(n)

        # Define the list of FIFOs objects
        nbFIFOS = t.shape[0]
        allFIFOs = [] 
        for i in range(nbFIFOS):
            allFIFOs.append(FIFODesc(i))

        # Normalization vector
        # For static scheduling it is
        #normV =  1.0*np.apply_along_axis(abs,1,t).max(axis=1)
        # For cyclo static scheduling we need the max per execution
        # and not the accumulated value for the run of a node
        # period which may be several periods of an IO
        #print(normV)
        normV = np.zeros(len(self._sortedEdges)) 
        for e in range(len(self._sortedEdges)):
            (src,dst) = self._sortedEdges[e]
            normV[e] = max(src.cycleMax, dst.cycleMax)

        #print(normV)

        # bMax below is used to track maximum FIFO size 
        # occuring during a run of the schedule
        #
        # The heuristric is:
        #
        # First we compute on each edge the maximum absolute value
        # It is the minimum amount of data an edge must contain
        # for the system to work either because it is produced
        # by a node or consumed by another.
        # We use this value as an unit of measurement for the edge.
        # So, we normalize the FIFO lengths by this size.
        # If this occupancy number is > 1 then it means
        # that enough data is available on the FIFO for the
        # consumer to consume it.
        # When we select a node for scheduling later we try
        # to minimize the occupancy number of all FIFOs by
        # selecting the scheduling which is giving the
        # minimum maximum occupancy number after the run.
        bMax = 1.0*np.array(initB)


        schedule=[]

        zeroVec = np.zeros(len(self._sortedNodes))
        evolutionTime = 0
        #print(self._sortedNodes)
        # While there are remaining node periods to schedule
        while (n != zeroVec).any():
            #print("")
            #print(n)
            # Look for the best node to schedule
            # which is the one giving the minimum FIFO increase
            
            # None selected
            selected = -1

            # Min FIFO size found
            minVal = 10000000

            
            for node in self._sortedNodes:
                # If the node can be scheduled
                if n[node.sortedNodeID] > 0:
                   # Evolution vector for static scheduling
                   # v = self.evolutionVectorForNode(nodeID)
                   # for cyclo static we need the new fifo state
                   newB = self.evolutionVectorForNode(node.sortedNodeID) + b
                   # New fifos size after this evolution
                   # For static scheduling, fifo update would have been
                   # newB = np.dot(t,v) + b
                   #print(newB)
            
                   # Check that there is no FIFO underflow:
                   if np.all(newB >= 0):
                      # Total FIFO size for this possible execution
                      # We normalize to get the occupancy number as explained above
                      theMin = (1.0*np.array(newB) / normV).max()
                      # If this possible evolution is giving smaller FIFO size
                      # (measured in occupancy number) then it is selected
                      
                      if theMin <= minVal:
                         minVal = theMin
                         selected = node.sortedNodeID 


            # No node could be scheduled because of not enough data
            # in the FIFOs. It should not occur if there is a null
            # space of dimension 1. So, it is probably a bug if
            # this exception is raised
            if selected < 0:
               raise DeadlockError
            # Now  we have evaluated all schedulable nodes for this run
            # and selected the one giving the smallest FIFO increase

            # Implementation for static scheduling
            # Real evolution vector for selected node
            # evol =  self.evolutionVectorForNode(selected)
            # Keep track that this node has been schedule
            # n = n - evol
            # Compute new fifo state
            # fifoChange = np.dot(t,evol)
            # b = fifoChange + b

            # Implementation for cyclo static scheduling
            #print("selected")
            fifoChange = self.evolutionVectorForNode(selected,test=False)
            b = fifoChange + b
            # For cyclo static, we create an evolution vector 
            # which contains a 1
            # at a node position only if this node has executed
            # its period.
            # Otherwise the null vector is not decreased
            v = np.zeros(len(self._sortedNodes))
            v[selected] = self._sortedNodes[selected].executeNode() 
            n = n - v


            if config.displayFIFOSizes and not mustDoSinkPrioritization:
               print(b)
            
            bMax = np.maximum(b,bMax)
            schedule.append(selected)

            # Analyze FIFOs to know if a FIFOs write is
            # followed immediately by a FIFO read of same size
            if not mustDoSinkPrioritization:
               analyzeStep(fifoChange,allFIFOs,evolutionTime)
            evolutionTime = evolutionTime + 1

        fifoMax=np.floor(bMax).astype(np.int32)

        if mustDoSinkPrioritization:
           schedule = self.computeTopologicalOrderSchedule(normV,allFIFOs,initB,bMax,initN,config)
        
        allBuffers=self.initializeFIFODescriptions(config,allFIFOs,fifoMax,evolutionTime)
        self._allFIFOs = allFIFOs 
        self._allBuffers = allBuffers

        if config.dumpSchedule:
            print("Schedule")
            for nodeID in schedule:
                print(self._sortedNodes[nodeID].nodeName)
            print("")
        return(Schedule(self,self._sortedNodes,self._sortedEdges,schedule))


class Schedule:
    def __init__(self,g,sortedNodes,sortedEdges,schedule):
        self._sortedNodes=sortedNodes
        self._sortedEdges=sortedEdges
        self._schedule = schedule 
        self._graph = g
        # Nodes containing pure functions (no state) like some
        # CMSIS-DSP functions.
        # When scheduling is using the option codeArray, the
        # schedule is encoded as an array.
        # Function calls cannot be inlined anymore and we need
        # to create new nodes for those function calls.
        # The pureNode structure is done for this.
        # It is a record because we want to reuse nodes for same
        # types.
        self._pureNodes={}
        nodeCodeID = 0
        pureClassID = 1
        for n in self.nodes:
            n.codeID = nodeCodeID
            nodeCodeID = nodeCodeID + 1
            # Constant nodes are ignored since they have
            # no arcs, and are connected to no FIFOs
            theArgs=[] 
            theArgTypes=[]
            i,o=n.allIOs()
            for io in i:
                # An io connected to a constant node has no fifo 
                if len(io.fifo) > 0:
                   theArgs.append(self.fifoID(io.fifo))
                   theArgTypes.append(io.ctype)
                else:
                # Instead the arg is the name of a constant node
                # instead of being a fifo ID
                   theArgs.append(io.constantNode.name)
                   theArgTypes.append(io.constantNode.name)
            for io in o:
                theArgs.append(self.fifoID(io.fifo))
                theArgTypes.append(io.ctype)
            n.args=theArgs

            # Analyze the nature of arguments for pure functions
            # The information created during this analysis
            # is useful when generating a class containing the
            # pure function
            if not n.hasState:
               theType=(n.nodeName,tuple(theArgTypes))
               if not theType in self._pureNodes:
                  self._pureNodes[theType]=n
                  n.pureClassID = pureClassID 
                  pureClassID = pureClassID + 1
               else:
                  n.pureClassID = self._pureNodes[theType].pureClassID
               n.pureNodeType=theType
               n.analyzeArgs()

    def hasDelay(self,edge):
        return(self._graph.hasDelay(edge))

    def getDelay(self,edge):
        return(self._graph.getDelay(edge))

    @property
    def pureNodes(self):
        return self._pureNodes
    

    @property
    def constantEdges(self):
        return self._graph.constantEdges

    @property
    def nodes(self):
        return self._sortedNodes

    @property
    def edges(self):
        return self._sortedEdges

    @property
    def schedule(self):
        return self._schedule

    #@property
    #def fifoLengths(self):
    #    return self._fifos

    @property 
    def scheduleLength(self):
        return len(self.schedule)

    @property 
    def memory(self):
        #theBytes=[x[0].theType.bytes for x in self.edges]
        #theSizes=[x[0]*x[1] for x in zip(self.fifoLengths,theBytes)]
        #return(np.sum(theSizes))
        return(self._graph._totalMemory)

    @property
    def graph(self):
        return self._graph

    def fifoID(self,edge):
        return(self.edges.index(edge))

    def outputFIFOs(self,node):
        outs=[]
        for io in node.outputNames:
            x = node._outputs[io]
            fifo=(self.fifoID(x.fifo),io)
            outs.append(fifo)
            
        return(outs)

    def ccode(self,directory,config=Configuration()):
        """Write graphviz into file f""" 
        cmsisdsp.cg.static.scheduler.ccode.gencode(self,directory,config)

    def pythoncode(self,directory,config=Configuration()):
        """Write graphviz into file f""" 
        cmsisdsp.cg.static.scheduler.pythoncode.gencode(self,directory,config)

    def graphviz(self,f,config=Configuration()):
        """Write graphviz into file f""" 
        cmsisdsp.cg.static.scheduler.graphviz.gengraph(self,f,config)
    
    
    
    