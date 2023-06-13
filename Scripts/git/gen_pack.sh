#!/bin/bash
# Version: 1.5
# Date: 2022-06-01
# This bash script generates a CMSIS Software Pack:
#
# Pre-requisites:
# - bash shell (for Windows: install git for Windows)
# - git in path (for Windows: install git for Windows)
# - 7z in path (zip archiving utility)
#   e.g. Ubuntu: sudo apt-get install p7zip-full p7zip-rar)
# - packchk is taken from latest install CMSIS Pack installed in $CMSIS_PACK_ROOT
# - xmllint in path (XML schema validation)

############### EDIT BELOW ###############
# Extend Path environment variable locally
#

set -o pipefail

DIRNAME=$(dirname $(readlink -f $0))
CHANGELOG=$(readlink -f ${DIRNAME}/gen_changelog.sh)
DESCRIBE=$(readlink -f ${DIRNAME}/git_describe.sh)
VERSION=$(/bin/bash ${DESCRIBE} v)

function usage {
  echo "$(basename $0) [-h|--help] [<pdsc>]"
  echo ""
  echo "Arguments:"
  echo "  -k|--keep  Keep build directory."
  echo "  -h|--help  Print this usage message and exit."
  echo "  pdsc       The pack description to generate the pack for."
  echo ""
  echo "Environment:"
  echo " 7z"
  echo " packchk"
  echo " xmllint (optional)"
  echo ""
}

KEEP=0
POSITIONAL=()
while [[ $# -gt 0 ]]
do
  key="$1"

  case $key in
    '-h'|'--help')
      usage
      exit 1
    ;;
    '-k'|'--keep')
      shift
      KEEP=1
    ;;
    *)    # unknown option
      POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
    ;;
  esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

OS=$(uname -s)
case $OS in
  'Linux')
    if [ -z ${CMSIS_PACK_ROOT+x} ] ; then
      CMSIS_PACK_ROOT="/home/$USER/.arm/Packs"
    fi
    CMSIS_TOOLSDIR="$(ls -drv ${CMSIS_PACK_ROOT}/ARM/CMSIS/* | head -1)/CMSIS/Utilities/Linux64"
    ;;
  'WindowsNT'|MINGW*|CYGWIN*)
    if [ -z ${CMSIS_PACK_ROOT+x} ] ; then
      CMSIS_PACK_ROOT="$LOCALAPPDATA/Arm/Packs"
    fi
    CMSIS_PACK_ROOT="/$(echo ${CMSIS_PACK_ROOT} | sed -e 's/\\/\//g' -e 's/://g' -e 's/\"//g')"
    CMSIS_TOOLSDIR="$(ls -drv ${CMSIS_PACK_ROOT}/ARM/CMSIS/* | head -1)/CMSIS/Utilities/Win32"
    ;;
  'Darwin')
    echo "Error: CMSIS Tools not available for Mac at present."
    exit 1
    ;;
  *)
    echo "Error: unrecognized OS $OS"
    exit 1
    ;;
esac

PATH_TO_ADD="$CMSIS_TOOLSDIR"

[[ ":$PATH:" != *":${PATH_TO_ADD}:"* ]] && PATH="${PATH}:${PATH_TO_ADD}"
echo $PATH_TO_ADD appended to PATH
echo " "

# Pack warehouse directory - destination
PACK_WAREHOUSE=./output

# Temporary pack build directory
PACK_BUILD=./build

# Specify directory names to be added to pack base directory
PACK_DIRS="
  Documentation
  Include
  Source
  PrivateInclude
  ComputeLibrary
  Examples
"

# Specify file names to be added to pack base directory
PACK_BASE_FILES="
  LICENSE
"

# Specify file names to be deleted from pack build directory
PACK_DELETE_FILES=""

# Specify patches to be applied
PACK_PATCH_FILES=""

############ DO NOT EDIT BELOW ###########
echo Starting CMSIS-Pack Generation: `date`
# Zip utility check
ZIP=7z
if ! type -a "${ZIP}"; then
  echo "Error: No 7zip Utility found"
  echo "Action: Add 7zip to your path"
  echo " "
  exit 1
fi

# Pack checking utility check
PACKCHK=packchk
if ! type -a ${PACKCHK}; then
  echo "Error: No PackChk Utility found"
  echo "Action: Add PackChk to your path"
  echo "Hint: Included in CMSIS Pack:"
  echo "$CMSIS_PACK_ROOT/ARM/CMSIS/<version>/CMSIS/Utilities/<os>/"
  echo " "
  exit 1
fi
echo " "

# Locate Package Description file
# check whether there is more than one pdsc file
NUM_PDSCS=$(ls -1 *.pdsc | wc -l)
PACK_DESCRIPTION_FILE=$(ls *.pdsc)
if [[ -n $1 && -f $1 ]]; then
  PACK_DESCRIPTION_FILE=$1
elif [ ${NUM_PDSCS} -lt 1 ]; then
  echo "Error: No *.pdsc file found in current directory"
  echo " "
  exit 1
elif [ ${NUM_PDSCS} -gt 1 ]; then
  echo "Error: Only one PDSC file allowed in directory structure:"
  echo "Found:"
  echo "$PACK_DESCRIPTION_FILE"
  echo "Action: Provide PDSC file explicitly!"
  echo " "
  usage
  exit 1
fi

SAVEIFS=$IFS
IFS=.
set ${PACK_DESCRIPTION_FILE}
# Pack Vendor
PACK_VENDOR=$1
# Pack Name
PACK_NAME=$2
echo "Generating Pack: for $PACK_VENDOR.$PACK_NAME"
echo " "
IFS=$SAVEIFS

#if $PACK_BUILD directory does not exist, create it.
if [ ! -d "$PACK_BUILD" ]; then
  mkdir -p "$PACK_BUILD"
fi

# Copy files into build base directory: $PACK_BUILD
# pdsc file is mandatory in base directory:
first=$(grep -n "<releases>" ${PACK_VENDOR}.${PACK_NAME}.pdsc | cut -d: -f1)
last=$(grep -n "</releases>" ${PACK_VENDOR}.${PACK_NAME}.pdsc | cut -d: -f1)
let first-=1
let last+=1
head -n ${first} "./${PACK_VENDOR}.${PACK_NAME}.pdsc" > "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc"
/bin/bash "${CHANGELOG}" -p -f pdsc 2>/dev/null | sed "s/^/  /" >> "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc"
tail -n +${last} "./${PACK_VENDOR}.${PACK_NAME}.pdsc" | \
  sed -e "s/Cversion=\"[^\"]*\"/Cversion=\"${VERSION}\"/" >> "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc"

# Add directories
echo Adding directories to pack:
#echo "${PACK_DIRS}"
echo " "
for d in ${PACK_DIRS}; do
  echo "$d"
  cp -r --parents "$d" "${PACK_BUILD}"
done

# Add files
echo Adding files to pack:
#echo "${PACK_BASE_FILES}"
echo " "
if [ ! -x ${PACK_BASE_FILES+x} ]; then
  for f in ${PACK_BASE_FILES}; do
    echo "$f"
    cp -f --parents "$f" $PACK_BUILD/
  done
fi

# Delete files
echo Deleting files from pack:
echo "${PACK_DELETE_FILES}"
echo " "
if [ ! -x ${PACK_DELETE_FILES+x} ]; then
  for f in ${PACK_DELETE_FILES}; do
    find $PACK_BUILD/$(dirname "$f") -name $(basename "$f") -delete
  done
fi

# Apply patches
echo Applying patches to pack:
echo "${PACK_PATCH_FILES}"
echo " "
if [ ! -x ${PACK_PATCH_FILES+x} ]; then
  CWD=$(pwd)
  pushd $PACK_BUILD > /dev/null
  for f in ${PACK_PATCH_FILES}; do
    patch -p0 -t -i "${CWD}/${f}"
  done
  popd > /dev/null
fi

# Run Schema Check (for Linux only):
# sudo apt-get install libxml2-utils


if type -a xmllint; then
  echo "Running schema check for ${PACK_VENDOR}.${PACK_NAME}.pdsc"
  SCHEMA=$(realpath -m ${CMSIS_TOOLSDIR}/../PACK.xsd)
  if [ ! -f $SCHEMA ]; then
    SCHEMAURL=$(grep -Pio "xs:noNamespaceSchemaLocation=\"\K[^\"]+" ${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc)
    curl -L $SCHEMAURL --output PACK.xsd
    SCHEMA="PACK.xsd"
  fi
  xmllint --noout --schema ${SCHEMA} "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc"
  errorlevel=$?
  if [ $errorlevel -ne 0 ]; then
    echo "build aborted: Schema check of $PACK_VENDOR.$PACK_NAME.pdsc against PACK.xsd failed"
    echo " "
    exit 1
  fi
else
  echo "Use MDK PackInstaller to run schema validation for $PACK_VENDOR.$PACK_NAME.pdsc"
fi

# Run Pack Check and generate PackName file with version
"${PACKCHK}" "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc" \
  -i "${CMSIS_PACK_ROOT}/.Web/ARM.CMSIS.pdsc" \
  -n PackName.txt
errorlevel=$?
if [ $errorlevel -ne 0 ]; then
  echo "build aborted: pack check failed"
  echo " "
  exit 1
fi

PACKNAME=$(cat PackName.txt)
rm -rf PackName.txt

# Create checksum file
echo Creating checksum file:
pushd $PACK_BUILD > /dev/null
find . -type f -exec sha1sum -b {} + > ../${PACK_VENDOR}.${PACK_NAME}.sha1
mv ../${PACK_VENDOR}.${PACK_NAME}.sha1 .
popd > /dev/null

# Archiving
# $ZIP a $PACKNAME
echo "creating pack file $PACKNAME"
#if $PACK_WAREHOUSE directory does not exist create it
if [ ! -d "$PACK_WAREHOUSE" ]; then
  mkdir -p "$PACK_WAREHOUSE"
fi
pushd "$PACK_WAREHOUSE" > /dev/null
PACK_WAREHOUSE=$(pwd)
popd  > /dev/null
pushd "$PACK_BUILD" > /dev/null
PACK_BUILD=$(pwd)
"$ZIP" a "$PACK_WAREHOUSE/$PACKNAME" -tzip
popd  > /dev/null
errorlevel=$?
if [ $errorlevel -ne 0 ]; then
  echo "build aborted: archiving failed"
  exit 1
fi

echo "build of pack succeeded"
# Clean up
echo "cleaning up ..."

if [[ $KEEP == 0 ]]; then
  rm -rf "$PACK_BUILD"
fi
echo " "

echo Completed CMSIS-Pack Generation: $(date)
