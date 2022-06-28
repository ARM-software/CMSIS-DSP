import numpy as np
import Tools
from numpy.linalg import qr 
import math

def randomIsometry(rows,cols,rank):
    if rank==1:
       r=np.random.randn(rows)
       r = Tools.normalize(r)[np.newaxis]
       c=np.random.randn(cols)
       c = Tools.normalize(c)[np.newaxis]
       result=r.T.dot(c)
    else:
        a = np.random.randn(rows,rows)
        b = np.random.randn(cols,cols)

        diagDim = min(rows,cols)
        d = np.zeros((rows,cols))
        
        diag = np.ones(diagDim)
        diag[rank:] = 0 
        np.fill_diagonal(d,diag)

        qa,_ = qr(a)
        qb,_ = qr(b)

        result = qa .dot(d.dot(qb))
    return(result)

def kahan_matrix(rows):
    cols = rows
    eps = np.finfo(np.float32).eps
    s = math.pow(eps,1.0/rows)
    c = math.sqrt(1-s*s)
    m = np.zeros((rows,cols))
    sc = 1.0

    for i in range(rows-1):
        m[i,i] = sc
        m[i,i+1:] = - sc * c * np.ones(rows-i-1)
        sc = sc * s
    
    m = m + m.T

    return(m)

def householder(x,eps=1e-16):
    #print(x)
    v=np.hstack([[1],x[1:]])
    
    alpha = x[0]
    xnorm2=x[1:].dot(x[1:])
    epsilon=eps

    #print(sigma)

    if xnorm2<=epsilon:
       tau = 0.0
       v = np.zeros(len(x))
    else:
       if np.sign(alpha) <= 0:
          beta = math.sqrt(alpha*alpha + xnorm2)
       else:
          beta = -math.sqrt(alpha*alpha + xnorm2)

       r =  (alpha - beta)
       v = x / r
       tau = (beta - alpha) / beta
       v[0] = 1

    return(v,tau)

def QR(oldm,eps=1e-16):
    m=np.copy(oldm)
    (rows,cols) = m.shape
    hvects=[]
    tau=[]
    h=[]
    for c in range(cols):
        currentSize = rows - c 
        v,beta=householder(m[c:,c],eps=eps)
        tau.append(beta)
        h.append(v)
        hvects.append((v,beta))
        t = np.identity(currentSize) - beta * np.outer(v,v)
        m[c:,c:] = np.dot(t,m[c:,c:])

    hvects.reverse()
    q=np.identity(rows)
    c=cols - 1
    for (v,beta) in hvects:
       t = np.identity(len(v)) - beta * np.outer(v,v)
       q[c:,c:] = np.dot(t,q[c:,c:])
       c = c - 1

    return(q,m,tau,h)