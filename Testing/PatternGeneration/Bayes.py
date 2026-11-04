import os.path
import itertools
import Tools
import random
import numpy as np
from sklearn.naive_bayes import GaussianNB

def printS(a):
    print("Interpreter[\"Number\"][\"%.9g\"]" % a,end="")

def printV(v):
    start = False
    print("{",end="")
    for r in v:
        if start:
            print(",",end="")
        start = True
        printS(r)
    print("}",end="")

def printM(v):
    start = False
    print("{",end="")
    for r in v:
        if start:
            print(",",end="")
        start = True
        printV(r)
    print("}",end="")

NBTESTSAMPLES = 10

VECDIM = [12,14,20]
BAYESCLASSES= [3,5,4]

NBTRAININGSAMPLES = 30
    
# Distance between the two centers (training vectors are gaussianly 
# distributed around the centers)
CENTER_DISTANCE = 1

TRAININGRATIO = 6.0
PREDICTRATIO = 12.0

# Generate random points distributed around one cluster.
# Cluster are on each axis like (1,0,0,0), (0,1,0,0), (0,0,1,0) etc ...
def newRandomVector(nbClasses,vecDim,ratio):
        v = np.random.randn(vecDim)
        v = v * CENTER_DISTANCE/2.0/ratio
        c = np.random.choice(range(0,nbClasses))
        c0 = np.zeros(vecDim)
        c1 = np.copy(c0)
        c1[c] = c0[0] + CENTER_DISTANCE
        return((v + c1).tolist(),c)

def trainGaussian(nbClasses,vecDim):
        inputs=[] 
        outputs=[]

        # Generate test patterns for this classifier
        for i in range(0,NBTRAININGSAMPLES):
            v,c=newRandomVector(nbClasses,vecDim,TRAININGRATIO)

            inputs.append(v)
            outputs.append(c)

        gnb = GaussianNB()
        gnb.fit(inputs, outputs)
        return(gnb)

def generateNewTest(config,nb):
    dims=[] 
    inputs=[] 
    referenceproba=[] 
    referencepredict=[]
    params=[]
    dims.append(NBTESTSAMPLES)
    classNb = BAYESCLASSES[nb % len(BAYESCLASSES)]
    vecDim = VECDIM[nb % len(VECDIM)]
    dims.append(classNb)
    dims.append(vecDim)
    # Train a classifier for a given vector dimension and
    # given number of classes
    gb = trainGaussian(classNb,vecDim)
    params += list(np.reshape(gb.theta_,np.size(gb.theta_)))
    params += list(np.reshape(gb.sigma_,np.size(gb.sigma_)))
    params += list(np.reshape(gb.class_prior_,np.size(gb.class_prior_)))
    params.append(gb.epsilon_)

    #print("theta=",end="")
    #printM(gb.theta_)
    #print(";",end="")
    #
    #print("sigma=",end="")
    #printM(gb.sigma_)
    #print(";",end="")
    #
    #print("prior=",end="")
    #printV(gb.class_prior_)
    #print(";",end="")
    #
    #print("epsilon=",end="")
    #printS(gb.epsilon_)
    #print(";",end="")

    #print(classNb,vecDim)
    for _ in range(0,NBTESTSAMPLES):
        # Generate a test pattern for this classifier
        v,c=newRandomVector(classNb,vecDim,PREDICTRATIO)
        inputs += v
        #print("inputs=",end="")
        #printV(v)
        #print(";",end="")
       
        y_pred = gb.predict([v])
        referencepredict.append(y_pred[0])
       
        probas = gb._joint_log_likelihood([v])
        probas = probas[0]
        referenceproba += list(probas)
        

    inputs = np.array(inputs)
    params = np.array(params)
    referenceproba = np.array(referenceproba)
    referencepredict = np.array(referencepredict)
    dims = np.array(dims)

    config.writeInput(nb, inputs,"Inputs")
    config.writeInputS16(nb, dims,"Dims")
    config.writeReference(nb, referenceproba,"Probas")
    config.writeReferenceS16(nb, referencepredict,"Predicts")
    config.writeReference(nb, params,"Params")
    
    #print(inputs)
    #print(dims)
    #print(referencepredict)
    #print(referenceproba)
    #print(params)


def writeTests(config):
    generateNewTest(config,1)

def writeBenchmark(config):
    someLists=[VECDIM,BAYESCLASSES]
    
    r=np.array([element for element in itertools.product(*someLists)])
    nbtests=len(VECDIM)*len(BAYESCLASSES)*2
    config.writeParam(2, r.reshape(nbtests))

    params=[]
    inputs=[] 
    referencepredict=[]
    dims=[] 
    nbin=0
    nbparam=0;

    for vecDim, classNb in r:
        gb = trainGaussian(classNb,vecDim)
        p = []
        p += list(np.reshape(gb.theta_,np.size(gb.theta_)))
        p += list(np.reshape(gb.sigma_,np.size(gb.sigma_)))
        p += list(np.reshape(gb.class_prior_,np.size(gb.class_prior_)))
        p.append(gb.epsilon_)

        params += p
        dims += [nbin,nbparam]
        nbparam = nbparam + len(p)

        v,c=newRandomVector(classNb,vecDim,PREDICTRATIO)
        inputs += v

        nbin = nbin + len(v)

        y_pred = gb.predict([v])
        referencepredict.append(y_pred[0])
       

    inputs = np.array(inputs)
    params = np.array(params)
    referencepredict = np.array(referencepredict)
    dims = np.array(dims)

    config.writeInput(2, inputs,"Inputs")
    config.writeReferenceS16(2, referencepredict,"Predicts")
    config.writeReference(2, params,"Params")
    config.writeInputS16(2, dims,"DimsBench")


# float64 reference of arm_sefr_train_f32, one-vs-rest
def trainSefr(x,y,nbClasses):
    low, high = x.min(0), x.max(0)
    span = np.where(high > low, high - low, 1.0)
    z = (x - low) / span
    n = np.bincount(y, minlength=nbClasses).astype(float)
    sums = np.zeros((nbClasses, x.shape[1]))
    np.add.at(sums, y, z)
    p = sums / n[:, None]
    r = (sums.sum(0) - sums) / (len(y) - n)[:, None]
    w = (p - r) / (p + r + 1e-7)
    t = ((len(y) - n) * (w * p).sum(1) + n * (w * r).sum(1)) / len(y)
    w = w / span
    return(w, -(t + w @ low))

# float64 reference of arm_sefr_lda_f32
def ldaSefr(x,y,nbClasses,w,b):
    scores = x @ w.T + b
    n = np.bincount(y, minlength=nbClasses).astype(float)
    means = np.zeros((nbClasses, nbClasses))
    np.add.at(means, y, scores)
    means /= n[:, None]
    rest = scores - means[y]
    cov = rest.T @ rest / len(y)
    cov += 1e-6 * np.trace(cov) / nbClasses * np.eye(nbClasses)
    a = np.linalg.solve(cov, means.T).T
    return(a @ w, np.log(n / len(y)) + a @ b - 0.5 * (a * means).sum(1))

def generateSefrTest(config,nb,nbClasses,vecDim):
    rng = np.random.default_rng(nb)
    NBTRAIN = 60
    NBTEST = 20
    centers = rng.uniform(-2.0, 2.0, (nbClasses, vecDim))
    # Every class is in the training set. The last class is more common than the
    # others because the class counts and priors would have no effect otherwise.
    y = np.concatenate([np.minimum(np.arange(NBTRAIN) % (2 * nbClasses), nbClasses - 1),
                        rng.integers(0, nbClasses, NBTEST)])
    # The reference has to see the same float32 values as the C code
    x = (centers[y] + rng.normal(0.0, 1.5, (NBTRAIN + NBTEST, vecDim))).astype(np.float32).astype(np.float64)
    xtrain, ytrain, xtest = x[:NBTRAIN], y[:NBTRAIN], x[NBTRAIN:]

    w, b = trainSefr(xtrain, ytrain, nbClasses)
    wlda, blda = ldaSefr(xtrain, ytrain, nbClasses, w, b)

    config.writeInputS16(nb, np.array([NBTRAIN, NBTEST, nbClasses, vecDim]), "SefrDims")
    config.writeInput(nb, x.reshape(-1), "SefrInputs")
    config.writeInputU32(nb, ytrain, "SefrLabels")
    config.writeReference(nb, w.reshape(-1), "SefrWeights")
    config.writeReference(nb, b, "SefrBias")
    config.writeReferenceS16(nb, np.argmax(xtest @ w.T + b, 1), "SefrPredicts")
    config.writeReference(nb, wlda.reshape(-1), "SefrLdaWeights")
    config.writeReference(nb, blda, "SefrLdaBias")
    config.writeReferenceS16(nb, np.argmax(xtest @ wlda.T + blda, 1), "SefrLdaPredicts")

def generateSefrPatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","Bayes","Bayes")
    PARAMDIR = os.path.join("Parameters","DSP","Bayes","Bayes")

    configf32=Tools.Config(PATTERNDIR,PARAMDIR,"f32")

    generateSefrTest(configf32,3,2,6)
    generateSefrTest(configf32,4,5,12)

def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","Bayes","Bayes")
    PARAMDIR = os.path.join("Parameters","DSP","Bayes","Bayes")
    
    configf32=Tools.Config(PATTERNDIR,PARAMDIR,"f32")
    configf16=Tools.Config(PATTERNDIR,PARAMDIR,"f16")
    
    writeTests(configf32)
    writeTests(configf16)

    writeBenchmark(configf32)
    writeBenchmark(configf16)

    generateSefrPatterns()

if __name__ == '__main__':
  generatePatterns()