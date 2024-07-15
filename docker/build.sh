#!/bin/bash

# Build the os99 16bit tools image
if ! docker image inspect os99-ia16 &> /dev/null; then
  echo "Building os99-ia16 image"
  docker build -t os99-ia16 -f ./os99-ia16/Dockerfile .
fi

# Build the os99 main image
#if ! docker image inspect os99-main &> /dev/null; then
  echo "Building os99-main image"
  docker build -t os99-main -f ./os99-main/Dockerfile .
#fi
