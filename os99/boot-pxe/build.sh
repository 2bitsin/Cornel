#!/bin/bash

# local variables
TEMPORARY_PATH=$WORKSPACE_ROOT/temp
PREABLE_PATH=$TEMPORARY_PATH/preamble.bin
TARGET_PATH=$WORKSPACE_ROOT/boot-pxe.sys
CONFIG_PATH=$WORKSPACE_ROOT/config.ini

mkdir -p $TEMPORARY_PATH

fasm preamble/preamble.asm $PREABLE_PATH

cp $PREABLE_PATH $TARGET_PATH
cp ./config.ini $CONFIG_PATH