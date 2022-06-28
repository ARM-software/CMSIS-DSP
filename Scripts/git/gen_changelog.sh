#!/bin/bash

DIRNAME=$(dirname $(readlink -f $0))
DESCRIBE=$(readlink -f ${DIRNAME}/git_describe.sh)

function usage {
  echo "$(basename $0) [-h|--help] [-f|--format <format>] [tag-prefix]"
  echo ""
  echo "Arguments:"
  echo "  -h|--help               Print this usage message and exit."
  echo "  -f|--format <format>    Print changelog in given format."
  echo "               text       Release notes in plain text."  
  echo "               pdsc       Release notes for PDSC"
  echo "               dxy        Release notes for Doxygen"
  echo "               html       Release notes for HTML"
  echo "  -p|--pre                Include latest pre-release."
  echo "  tag-prefix              Prefix to filter tags."
  echo ""
}

function print_text_head {
  true
}

function print_text {
  if [ -z "$2" ]; then
    echo "$1"
  else
    echo "$1 ($2)"
  fi
  
  echo -e "$3"
}

function print_text_tail {
  true
}

function print_pdsc_head {
  echo "<releases>"
}

function print_pdsc {
  if [ -z "$2" ]; then
    echo "  <release version=\"$1\">"
  else
    echo "  <release version=\"$1\" date=\"$2\">"
  fi
  echo -e "$3" | \
    sed "s/^/    /" | \
    sed "s/<br>//" | \
    sed "s/<ul>//" | \
    sed "s/<\/ul>//" | \
    sed "s/<li>/- /" | \
    sed "s/<\/li>//" | \
    sed "s/[ ]*$//" | \
    sed "/^$/d"
  echo -e "  </release>"
}

function print_pdsc_tail {
  echo "</releases>"
}

function print_dxy_head {
  echo "/**"
  echo "\page ChangeLog_pg Revision History of CMSIS-DSP"
  echo ""
  echo "Version    | Description"
  echo ":----------|:------------------------------------------"
}

function print_dxy {
  printf "%-10s | %s\n" "$1" "$3"
}

function print_dxy_tail {
  echo ""
  echo "*/"
}

function print_html_head {
  echo "/**"
  echo "\page ChangeLog_pg Revision History"
  echo ""
  echo "<table class=\"cmtable\" summary=\"Revision History\">"
  echo "<tr>"
  echo "  <th>Version</th>"
  echo "  <th>Description</th>"
  echo "</tr>"
}

function print_html {
  echo "<tr>"
  echo "  <td>$1</td>"
  echo "  <td>"
  echo -e "$3" | sed "s/^/    /"
  echo "  </td>"
  echo "</tr>"
}

function print_html_tail {
  echo "</table>"
  echo ""
  echo "*/"
}

POSITIONAL=()
FORMAT="text"
PRERELEASE=0
while [[ $# -gt 0 ]]
do
  key="$1"

  case $key in
    '-h'|'--help')
      usage
      exit 1
    ;;
    '-f'|'--format')
      shift
      FORMAT=$1
      shift
    ;;
    '-p'|'--pre')
      PRERELEASE=1
      shift
    ;;
    *)    # unknown option
      POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
    ;;
  esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

echo "Generating changelog ..." >&2

PREFIX="v"
if [ -n "$1" ]; then
  PREFIX=$1
fi
TAGS=$(git for-each-ref --format "%(objecttype) %(refname)" --sort="-v:refname" "refs/tags/${PREFIX}*" 2>/dev/null | cut -d\  -f2)
LATEST=$(/bin/bash ${DESCRIBE} "${PREFIX}")

print_${FORMAT}_head

if [[ $PRERELEASE != 0 ]] && ! git rev-list "${PREFIX}${LATEST}" >/dev/null; then
  print_$FORMAT "${LATEST}" "" "Active development ..."
fi

for TAG in $TAGS; do
  TAG="${TAG#refs/tags/}" 
  DESC=$(git tag -l -n99 --format "%(contents)" ${TAG} 2>/dev/null)
  DATE=$(git tag -l -n99 --format "%(taggerdate:short)" ${TAG} 2>/dev/null)
  print_$FORMAT "${TAG#${PREFIX}}" "${DATE}" "${DESC}"       
done

print_${FORMAT}_tail

exit 0
