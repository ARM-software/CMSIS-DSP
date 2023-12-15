#!/usr/bin/env bash
# Version: 3.0
# Date: 2023-11-06
# This bash script generates CMSIS-DSP Documentation:
#
# Pre-requisites:
# - bash shell (for Windows: install git for Windows)
# - doxygen 1.9.6
# - linkchecker (can be skipped with -s)

set -o pipefail

# Set version of gen pack library
# For available versions see https://github.com/Open-CMSIS-Pack/gen-pack/tags.
# Use the tag name without the prefix "v", e.g., 0.7.0
REQUIRED_GEN_PACK_LIB="0.9.1"

DIRNAME=$(dirname "$(readlink -f "$0")")
GENDIR=../html
REQ_DXY_VERSION="1.9.6"

RUN_LINKCHECKER=1

function usage() {
  echo "Usage: $(basename "$0") [-h] [-s] [-c <comp>]"
  echo " -h,--help               Show usage"
  echo " -s,--no-linkcheck       Skip linkcheck"
  echo "                         Can be given multiple times. Defaults to all components."
}

while [[ $# -gt 0 ]]; do
  case $1 in
    '-h'|'help')
      usage
      exit 1
    ;;
    '-s'|'--no-linkcheck')
      RUN_LINKCHECKER=0
    ;;
    *)
      echo "Invalid command line argument: $1" >&2
      usage
      exit 1
    ;;
  esac
  shift # past argument
done

############ DO NOT EDIT BELOW ###########

# Set GEN_PACK_LIB_PATH to use a specific gen-pack library root
# ... instead of bootstrap based on REQUIRED_GEN_PACK_LIB
if [[ -f "${GEN_PACK_LIB_PATH}/gen-pack" ]]; then
  . "${GEN_PACK_LIB_PATH}/gen-pack"
else
  . <(curl -sL "https://raw.githubusercontent.com/Open-CMSIS-Pack/gen-pack/main/bootstrap")
fi

find_git
find_doxygen "${REQ_DXY_VERSION}"
[[ ${RUN_LINKCHECKER} != 0 ]] && find_linkchecker

if [ -z "${VERSION_FULL}" ]; then
  VERSION_FULL=$(git_describe "v")
fi

pushd "${DIRNAME}" > /dev/null || exit 1

echo "Generating documentation ..."

projectName=$(grep -E "PROJECT_NAME\s+=" dsp.dxy.in | sed -r -e 's/[^"]*"([^"]+)".*/\1/')
projectNumberFull="${VERSION_FULL}"
projectNumber="${projectNumberFull%+*}"
datetime=$(date -u +'%a %b %e %Y %H:%M:%S')
year=$(date -u +'%Y')

sed -e "s/{projectNumber}/${projectNumber}/" dsp.dxy.in \
  | sed -e "s/{cmsisProjectNumber}/${cmsisProjectNumber}/" \
  > dsp.dxy

git_changelog -f html -p "v" > src/history.txt

echo_log "\"${UTILITY_DOXYGEN}\" dsp.dxy"
"${UTILITY_DOXYGEN}" dsp.dxy

mkdir -p "${DIRNAME}/${GENDIR}/search/"
cp -f "${DIRNAME}/style_template/search.css" "${DIRNAME}/${GENDIR}/search/"
cp -f "${DIRNAME}/style_template/navtree.js" "${DIRNAME}/${GENDIR}/"
cp -f "${DIRNAME}/style_template/resize.js" "${DIRNAME}/${GENDIR}/"

sed -e "s/{datetime}/${datetime}/" "${DIRNAME}/style_template/footer.js.in" \
  | sed -e "s/{year}/${year}/" \
  | sed -e "s/{projectName}/${projectName}/" \
  | sed -e "s/{projectNumber}/${projectNumber}/" \
  | sed -e "s/{projectNumberFull}/${projectNumberFull}/" \
  > "${DIRNAME}/${GENDIR}/footer.js"

popd > /dev/null || exit 1

[[ ${RUN_LINKCHECKER} != 0 ]] && check_links "${DIRNAME}/../html/index.html" "${DIRNAME}"


exit 0
