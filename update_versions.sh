#!/bin/bash

mv -f version.js version.js.bak

LINES=($(grep -n 'list of versions' version.js.bak | cut -d: -f1))
LINES[0]=$(expr ${LINES[0]} - 1)
LINES[1]=$(expr ${LINES[1]} + 1)

VERSIONS=($(find -maxdepth 1 -mindepth 1 -type l) $(find -maxdepth 1  -mindepth 1 -type d))
fields=()
for version in ${VERSIONS[*]#./}; do
  if [ -f "$version/footer.js" ]; then
    label=$(grep "Version" $version/footer.js | sed -r "s/.*'Version ([^']+)'.*/\1/")
    fields+=("\"${version}\": \"${label}\"")
  fi
done   

head -n ${LINES[0]} version.js.bak > version.js
echo "//--- list of versions ---" >> version.js
echo "const versions = {" >> version.js
echo -n "    " >> version.js
echo $(IFS=,; echo "${fields[*]}") | sed 's/,/,\n    /g'  >> version.js
echo "}" >> version.js
echo "//--- list of versions ---" >> version.js
tail -n +${LINES[1]} version.js.bak >> version.js

rm version.js.bak

exit 0
