#!/bin/bash

usage() {
    echo ""
    echo "Usage: $(basename $0) <index> <src>"
    echo " <index>  Index.html file to start link scanning at."
    echo " <src>    Directory with doxygen source files."
    echo ""
}

if [ ! -f "$1" ]; then
    if [ -z "$1" ]; then
        echo "No index file provided!" >&2
    else
        echo "Index file not found: '$1'" >&2
    fi
    usage
    exit 1
fi

if [ ! -d "$2" ]; then
    if [ -z "$2" ]; then
        echo "No source directory provided!" >&2
    else
        echo "Source dir not found: '$2'" >&2
    fi
    usage
    exit 1
fi

linkchecker -F csv --timeout 3 --check-extern $1

OFS=$IFS
IFS=$'\n'

for line in $(grep -E '^[^#]' linkchecker-out.csv | tail -n +2); do
    link=$(echo $line | cut -d';' -f 1)
    msg=$(echo $line | cut -d';' -f 4)
    origin=$(grep -Ern "href=['\"]${link}['\"]" $2)
    for o in $origin; do
        ofile=$(echo $o | cut -d':' -f 1)
        oline=$(echo $o | cut -d':' -f 2)
        match=$(echo $o | cut -d':' -f 3-)
        rest="${match#*$link}"
        ocolumn=$((${#match} - ${#rest} - ${#link}))
        echo "$(readlink -f -n $ofile):${oline}:${ocolumn};${link};${msg};URL '${link}' results to '${msg}'" >&2
    done
done

IFS=$OFS

exit 0
