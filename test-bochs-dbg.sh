#!/bin/bash

rm -rf workspace/*
cp source/config.ini workspace/
cp build/coreload.sys workspace/
cd workspace
cmd.exe /c 'bochsdbg -f ../bochs/bochs.dbg.ini -q'
