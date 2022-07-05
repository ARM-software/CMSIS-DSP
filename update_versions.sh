#!/bin/bash

LINES=($(grep -n 'list of versions' version.js | cut -d: -f1))
LINES[0]=$(expr ${LINES[0]} - 1)
LINES[1]=$(expr ${LINES[1]} + 1)

VERSIONS=($(find -maxdepth 1 -mindepth 1 -type l) $(find -maxdepth 1  -mindepth 1 -type d))
fields=()
for version in ${VERSIONS[*]#./}; do
  label=$(grep "Version" $version/footer.js | sed -r "s/.*'Version ([^']+)'.*/\1/")
  fields+=("\"${version}\": \"${label}\"")
done   

mv version.js version.js.bak

head -n ${LINES[0]} version.js.bak > version.js
echo "//--- list of versions ---" >> version.js
echo "const versions = {" >> version.js
echo -n "    " >> version.js
echo $(IFS=,; echo "${fields[*]}") | sed 's/,/,\n    /'  >> version.js
echo "}" >> version.js
echo "//--- list of versions ---" >> version.js
tail -n +${LINES[1]} version.js.bak >> version.js

exit 0
