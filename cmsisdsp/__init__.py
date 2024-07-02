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
cmsis_dsp_version="1.15.0"


# CMSIS-DSP Commit hash used to build the wrapper
commit_hash="04c0b534edf16bd2898ecc66c1c660c27158f286"

# True if development version of CMSIS-DSP used
# (So several CMSIS-DSP versions may have same version number hence the commit hash)
developmentVersion=True

__all__ = ["datatype", "fixedpoint", "mfcc"]

# Default values
DEFAULT_HOUSEHOLDER_THRESHOLD_F64=1.0e-16
DEFAULT_HOUSEHOLDER_THRESHOLD_F32=1.0e-12

# DTW Window Types
ARM_DTW_SAKOE_CHIBA_WINDOW = 1
ARM_DTW_SLANTED_BAND_WINDOW = 3
