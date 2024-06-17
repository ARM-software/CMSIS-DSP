# Bug corrections for version 1.9
import cmsisdsp as dsp
import cmsisdsp.fixedpoint as f
import numpy as np
import colorama
from colorama import init,Fore, Back, Style
from numpy.testing import assert_allclose

from numpy.linalg import norm

import matplotlib
import matplotlib as mpl
import matplotlib.pyplot as plt

init()

def printTitle(s):
    print("\n" + Fore.GREEN + Style.BRIGHT +  s + Style.RESET_ALL)

def printSubTitle(s):
    print("\n" + Style.BRIGHT + s + Style.RESET_ALL)

printTitle("DTW Window")

printSubTitle("SAKOE_CHIBA_WINDOW")

refWin1=np.array([[1, 1, 1, 0, 0],
       [1, 1, 1, 1, 0],
       [1, 1, 1, 1, 1],
       [0, 1, 1, 1, 1],
       [0, 0, 1, 1, 1],
       [0, 0, 0, 1, 1],
       [0, 0, 0, 0, 1],
       [0, 0, 0, 0, 0],
       [0, 0, 0, 0, 0],
       [0, 0, 0, 0, 0]], dtype=np.int8)

dtwWindow=np.zeros((10,5),dtype=np.int8)
wsize=2
status,w=dsp.arm_dtw_init_window_q7(dsp.ARM_DTW_SAKOE_CHIBA_WINDOW,wsize,dtwWindow)

assert (w==refWin1).all()

printSubTitle("SLANTED_BAND_WINDOW")

refWin2=np.array([[1, 1, 0, 0, 0],
       [1, 1, 0, 0, 0],
       [1, 1, 1, 0, 0],
       [0, 1, 1, 0, 0],
       [0, 1, 1, 1, 0],
       [0, 0, 1, 1, 0],
       [0, 0, 1, 1, 1],
       [0, 0, 0, 1, 1],
       [0, 0, 0, 1, 1],
       [0, 0, 0, 0, 1]], dtype=np.int8)

dtwWindow=np.zeros((10,5),dtype=np.int8)
wsize=1
status,w=dsp.arm_dtw_init_window_q7(dsp.ARM_DTW_SLANTED_BAND_WINDOW,wsize,dtwWindow)

assert (w==refWin2).all()


printTitle("DTW Cost Matrix and DTW Distance")

QUERY_LENGTH = 10 
TEMPLATE_LENGTH = 5

query=np.array([ 0.08387197,  0.68082274,  1.06756417,  0.88914541,  0.42513398, -0.3259053,
 -0.80934885, -0.90979435, -0.64026483,  0.06923695])

template=np.array([ 1.00000000e+00,  7.96326711e-04, -9.99998732e-01, -2.38897811e-03,
  9.99994927e-01])

cols=np.array([1,2,3])
rows=np.array([10,11,12])

printSubTitle("Without a window")

referenceCost=np.array([[0.91612804, 0.9992037 , 2.0830743 , 2.1693354 , 3.0854583 ],
       [1.2353053 , 1.6792301 , 3.3600516 , 2.8525472 , 2.8076797 ],
       [1.3028694 , 2.3696373 , 4.4372    , 3.9225004 , 2.875249  ],
       [1.4137241 , 2.302073  , 4.1912174 , 4.814035  , 2.9860985 ],
       [1.98859   , 2.2623994 , 3.6875322 , 4.115055  , 3.5609593 ],
       [3.3144953 , 2.589101  , 3.2631946 , 3.586711  , 4.8868594 ],
       [5.123844  , 3.3992462 , 2.9704008 , 3.7773607 , 5.5867043 ],
       [7.0336385 , 4.309837  , 3.0606053 , 3.9680107 , 5.8778    ],
       [8.673903  , 4.950898  , 3.420339  , 4.058215  , 5.698475  ],
       [9.604667  , 5.0193386 , 4.489575  , 3.563591  , 4.494349  ]],
      dtype=np.float32)

referenceDistance = 0.2996232807636261

# Each row is a new query
a,b = np.meshgrid(template,query)
distance=abs(a-b).astype(np.float32)

status,dtwDistance,dtwMatrix = dsp.arm_dtw_distance_f32(distance,None)


assert_allclose(referenceDistance,dtwDistance)
assert_allclose(referenceCost,dtwMatrix)

printSubTitle("Path")

path=dsp.arm_dtw_path_f32(np.copy(dtwMatrix))
#print(path)
pathMatrix=np.zeros(dtwMatrix.shape)
for x in list(zip(path[0::2],path[1::2])):
    pathMatrix[x] = 1 


fig, ax = plt.subplots()
im = ax.imshow(pathMatrix,vmax=2.0)

for i in range(QUERY_LENGTH):
    for j in range(TEMPLATE_LENGTH):
        text = ax.text(j, i, "%.1f" % dtwMatrix[i, j],
                       ha="center", va="center", color="w")
fig.tight_layout()
plt.show()

printSubTitle("With a window")

nan = np.nan 

referenceDistance = 0.617099940776825
referenceCost=np.array([[9.1612804e-01, 9.9920368e-01, nan, nan,
        nan],
       [1.2353053e+00, 1.6792301e+00, nan, nan,
        nan],
       [1.3028694e+00, 2.3696373e+00, 4.4372001e+00, nan,
        nan],
       [nan, 3.0795674e+00, 4.9687119e+00, nan,
        nan],
       [nan, 3.5039051e+00, 4.9290380e+00, 5.3565612e+00,
        nan],
       [nan, nan, 4.8520918e+00, 5.1756082e+00,
        nan],
       [nan, nan, 5.0427418e+00, 5.8497019e+00,
        7.6590457e+00],
       [nan, nan, nan, 6.7571073e+00,
        8.6668968e+00],
       [nan, nan, nan, 7.3949833e+00,
        9.0352430e+00],
       [nan, nan, nan, nan,
        9.2564993e+00]], dtype=np.float32)


status,dtwDistance,dtwMatrix = dsp.arm_dtw_distance_f32(distance,w)


assert_allclose(referenceDistance,dtwDistance)
assert_allclose(referenceCost[w==1],dtwMatrix[w==1])

