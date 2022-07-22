#!/bin/bash
# Version: 1.0
# Date: 2022-05-31
# This bash script generates CMSIS-NN Documentation:
#
# Pre-requisites:
# - bash shell (for Windows: install git for Windows)
# - doxygen 1.9.2

set -o pipefail

DIRNAME=$(dirname $(readlink -f $0))
DOXYGEN=$(which doxygen)
DESCRIBE=$(readlink -f ${DIRNAME}/../Scripts/git/git_describe.sh)
CHANGELOG=$(readlink -f ${DIRNAME}/../Scripts/git/gen_changelog.sh)


if [[ ! -f "${DOXYGEN}" ]]; then
  echo "Doxygen not found!" >&2
  echo "Did you miss to add it to PATH?"
  exit 1
else
  version=$("${DOXYGEN}" --version)
  echo "DOXYGEN is ${DOXYGEN} at version ${version}"
  if [[ "${version}" != "1.9.2" ]]; then
    echo " >> Version is different from 1.9.2 !" >&2
  fi
fi

if [ -z $VERSION ]; then
  VERSION_FULL=$(/bin/bash ${DESCRIBE} "v")
  VERSION=${VERSION_FULL%+*}
fi

echo "Generating documentation ..."

pushd $DIRNAME > /dev/null

rm -rf ${DIRNAME}/../Documentation/html
sed -e "s/{projectNumber}/${VERSION}/" "${DIRNAME}/dsp.dxy.in" \
  > "${DIRNAME}/dsp.dxy"

echo "${CHANGELOG} -f html > history.txt"
/bin/bash "${CHANGELOG}" -f html 1> history.txt 2>/dev/null

echo "${DOXYGEN} dsp.dxy"
"${DOXYGEN}" dsp.dxy
popd > /dev/null

if [[ $2 != 0 ]]; then
  cp -f "${DIRNAME}/templates/search.css" "${DIRNAME}/../Documentation/html/search/"
fi

projectName=$(grep -E "PROJECT_NAME\s+=" "${DIRNAME}/dsp.dxy" | sed -r -e 's/[^"]*"([^"]+)"/\1/')
datetime=$(date -u +'%a %b %e %Y %H:%M:%S')
year=$(date -u +'%Y')
if [[ "${year}" != "2022" ]]; then 
  year="2022-${year}"
fi
sed -e "s/{datetime}/${datetime}/" "${DIRNAME}/templates/footer.js.in" \
  | sed -e "s/{year}/${year}/" \
  | sed -e "s/{projectName}/${projectName}/" \
  | sed -e "s/{projectNumber}/${VERSION}/" \
  | sed -e "s/{projectNumberFull}/${VERSION_FULL}/" \
  > "${DIRNAME}/../Documentation/html/footer.js"

exit 0