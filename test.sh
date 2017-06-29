#!/bin/bash

make

TOOL_NAME=./exifprint

if [ ! -e ./exifprint ]; then
  # check if exifprint.exe exists, needed on windows
  if [ -e ./exifprint.exe ]; then
    TOOL_NAME=./exifprint.exe
  else
    echo Cannot find executable.
    exit 1
  fi
fi

for jpeg in `ls test-images/*.jpg`; do
  $TOOL_NAME $jpeg > /tmp/`basename $jpeg`.actual
  diff $jpeg.expected /tmp/`basename $jpeg`.actual > /tmp/diff.out
  if [[ -s /tmp/diff.out ]] ; then
    echo "FAILED ON $jpeg"
    cat /tmp/diff.out
    exit
  fi ;
  echo "PASS $jpeg"
done
