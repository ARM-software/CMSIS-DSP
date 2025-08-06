#!/usr/bin/env bash
# Version: 3.0
# Date: 2023-11-06
# This bash script generates a CMSIS-DSP Software Pack:
#

set -o pipefail

# Set version of gen pack library
# For available versions see https://github.com/Open-CMSIS-Pack/gen-pack/tags.
# Use the tag name without the prefix "v", e.g., 0.7.0
REQUIRED_GEN_PACK_LIB="0.11.1"

# Set default command line arguments
DEFAULT_ARGS=(-c "v")

# Pack warehouse directory - destination
# Default: ./output
#
# PACK_OUTPUT=./output

# Temporary pack build directory,
# Default: ./build
#
# PACK_BUILD=./build

# Specify directory names to be added to pack base directory
# An empty list defaults to all folders next to this script.
# Default: empty (all folders)
#
PACK_DIRS="
  ComputeLibrary
  Documentation
  Examples
  Include
  PrivateInclude
  Source
"

# Specify file names to be added to pack base directory
# Default: empty
#
PACK_BASE_FILES="
  LICENSE
"

# Specify file names to be deleted from pack build directory
# Default: empty
#
PACK_DELETE_FILES="
  Documentation/Doxygen
  Documentation/README.md
  Source/CMakeLists.txt
  Source/configDsp.cmake
  Source/Makefile
  Source/BasicMathFunctions/Config.cmake
  Source/BayesFunctions/Config.cmake
  Source/CommonTables/Config.cmake
  Source/ComplexMathFunctions/Config.cmake
  Source/ControllerFunctions/Config.cmake
  Source/DistanceFunctions/Config.cmake
  Source/FastMathFunctions/Config.cmake
  Source/FilteringFunctions/Config.cmake
  Source/InterpolationFunctions/Config.cmake
  Source/MatrixFunctions/Config.cmake
  Source/QuaternionMathFunctions/Config.cmake
  Source/StatisticsFunctions/Config.cmake
  Source/SupportFunctions/Config.cmake
  Source/SVMFunctions/Config.cmake
  Source/TransformFunctions/Config.cmake
  Source/WindowFunctions/Config.cmake
"

# Specify patches to be applied
# Default: empty
#
# PACK_PATCH_FILES=""

# Specify addition argument to packchk
# Default: empty
#
# PACKCHK_ARGS=()

# Specify additional dependencies for packchk
# Default: empty
#
PACKCHK_DEPS="
  ARM.CMSIS.pdsc
  ARM.Cortex_DFP.pdsc
"

# Optional: restrict fallback modes for changelog generation
# Default: full
# Values:
# - full      Tag annotations, release descriptions, or commit messages (in order)
# - release   Tag annotations, or release descriptions (in order)
# - tag       Tag annotations only
#
PACK_CHANGELOG_MODE="release"

#
# custom pre-processing steps
#
# usage: preprocess <build>
#   <build>  The build folder
#
function preprocess() {
  # add custom steps here to be executed
  # before populating the pack build folder
  ./Documentation/Doxygen/gen_doc.sh
  return 0
}

#
# custom post-processing steps
#
# usage: postprocess <build>
#   <build>  The build folder
#
function postprocess() {
  # add custom steps here to be executed
  # after populating the pack build folder
  # but before archiving the pack into output folder
  return 0
}

############ DO NOT EDIT BELOW ###########

# Set GEN_PACK_LIB_PATH to use a specific gen-pack library root
# ... instead of bootstrap based on REQUIRED_GEN_PACK_LIB
if [[ -f "${GEN_PACK_LIB_PATH}/gen-pack" ]]; then
  . "${GEN_PACK_LIB}/gen-pack"
else
  . <(curl -sL "https://raw.githubusercontent.com/Open-CMSIS-Pack/gen-pack/main/bootstrap")
fi

gen_pack "${DEFAULT_ARGS[@]}" "$@"

exit 0
