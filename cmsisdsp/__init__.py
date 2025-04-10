import cmsisdsp.version

from cmsisdsp_filtering import *
from cmsisdsp_matrix import *
from cmsisdsp_support import *
from cmsisdsp_statistics import *
from cmsisdsp_complexf import *
from cmsisdsp_basic import *
from cmsisdsp_controller import *
from cmsisdsp_transform import *
from cmsisdsp_interpolation import *
from cmsisdsp_quaternion import *
from cmsisdsp_fastmath import *
from cmsisdsp_distance import *
from cmsisdsp_bayes import *
from cmsisdsp_svm import *
from cmsisdsp_window import *


__version__ = cmsisdsp.version.__version__

# CMSIS-DSP Version used to build the wrapper
cmsis_dsp_version="1.16.2"


# CMSIS-DSP Commit hash used to build the wrapper
commit_hash="1b2ba1d88ec21bf0bc4eeae694193aeb057fdcc2"

# True if development version of CMSIS-DSP used
# (So several CMSIS-DSP versions may have same version number hence the commit hash)
developmentVersion=True

__all__ = ["datatype", "fixedpoint", "mfcc","demo"]

# Default values
DEFAULT_HOUSEHOLDER_THRESHOLD_F64=1.0e-16
DEFAULT_HOUSEHOLDER_THRESHOLD_F32=1.0e-12

# DTW Window Types
ARM_DTW_SAKOE_CHIBA_WINDOW = 1
ARM_DTW_SLANTED_BAND_WINDOW = 3

# Target architectures to compute the size of temporary or output buffers
# for the transform functions
ARM_MATH_SCALAR_ARCH = 1
ARM_MATH_DSP_EXTENSIONS_ARCH = 2
ARM_MATH_HELIUM_ARCH = 3
ARM_MATH_NEON_ARCH = 4