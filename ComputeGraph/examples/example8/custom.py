import numpy as np 

class MyComplex:
    def __init__(self,re=0,im=0):
        self.re = re 
        self.im = im 

    def __str__(self):
        return("%f + I %f" % (self.re, self.im))

    def __repr__(self):
        return("MyComplex(%f,%f)" % (self.re, self.im))

