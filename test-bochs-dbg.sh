#!/bin/bash
cmd.exe /c bochsdbg -f bochs/bochs.dbg.ini -q &
sleep 2
cmd.exe /c "putty -serial \\\\.\\pipe\\bochs-com1"