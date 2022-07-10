#!/bin/bash
cmd.exe /c bochs -f bochs/bochs.ini -q &
sleep 2
cmd.exe /c "putty -serial \\\\.\\pipe\\bochs-com1"
