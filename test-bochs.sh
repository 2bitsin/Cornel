#!/bin/bash

rm -rf workspace/*
cp source/config.ini workspace/
cp build/coreload.sys workspace/
cmd.exe /c bochs -f bochs/bochs.ini -q &
cmd.exe /c 'cd workspace && start ../tools/bin/bootpd.exe -C config.ini -O v4_bind_address=192.168.1.1'

#sleep 2
#cmd.exe /c "putty -serial \\\\.\\pipe\\bochs-com1"