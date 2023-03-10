# Include definitions from the Python package to
# define datatype for the IOs and to have access to the
# Graph class
from cmsisdsp.cg.scheduler import *
from nodes_bench import * 

FS=16000
# You can try with 120
AUDIO_INTERRUPT_LENGTH = 192
WINSIZE=256
OVERLAP=128
floatType=CType(F32)


### Define nodes
src=ArraySource("src",floatType,AUDIO_INTERRUPT_LENGTH)
src.addVariableArg("inputArray")

sliding=SlidingBuffer("audioWin",floatType,WINSIZE,OVERLAP)
overlap=OverlapAdd("audioOverlap",floatType,WINSIZE,OVERLAP)
window=Dsp("mult",floatType,WINSIZE)

toCmplx=ToComplex("toCmplx",floatType,WINSIZE)
toReal=ToReal("toReal",floatType,WINSIZE)
fft=CFFT("cfft",floatType,WINSIZE)
ifft=ICFFT("icfft",floatType,WINSIZE)

hann=Constant("HANN")
sink=ArraySink("sink",floatType,AUDIO_INTERRUPT_LENGTH)
sink.addVariableArg("outputArray")

the_graph = Graph()

the_graph.connect(src.o, sliding.i)

# Windowinthe_graph
the_graph.connect(sliding.o, window.ia)
the_graph.connect(hann,window.ib)

# FFT
the_graph.connect(window.o,toCmplx.i)
the_graph.connect(toCmplx.o,fft.i)
the_graph.connect(fft.o,ifft.i)
the_graph.connect(ifft.o,toReal.i)


# Overlap add
the_graph.connect(toReal.o,overlap.i)
the_graph.connect(overlap.o,sink.i)