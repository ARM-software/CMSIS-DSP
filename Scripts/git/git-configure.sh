#!/bin/bash

DIRNAME=$(git rev-parse --show-toplevel)

if ! git config --local -l | egrep -q include.path=../.gitconfig$; then
  git config --local --add include.path ../.gitconfig
  rm ${DIRNAME}/.git/index
  git checkout HEAD -- ${DIRNAME}
fi
