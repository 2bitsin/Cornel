#!/bin/bash
set -e

make -C source all
cp source/config.ini workspace/
cp build/coreload.sys workspace/
