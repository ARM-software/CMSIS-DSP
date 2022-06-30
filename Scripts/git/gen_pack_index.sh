#!/bin/bash

cat <<EOF
<?xml version="1.0" encoding="UTF-8" ?>
<index schemaVersion="1.0.0" xs:noNamespaceSchemaLocation="PackIndex.xsd" xmlns:xs="http://www.w3.org/2001/XMLSchema-instance">
  <vendor>ARM</vendor>
  <url>https://raw.githubusercontent.com/ARM-software/CMSIS-DSP/pack-index/</url>
  <timestamp>$(date -u +%FT%TZ)</timestamp>
  <pindex>
    <pdsc url="https://raw.githubusercontent.com/ARM-software/CMSIS-DSP/pack-index/" vendor="ARM" name="CMSIS-DSP" version="$1"/>
  </pindex>
</index>
EOF
