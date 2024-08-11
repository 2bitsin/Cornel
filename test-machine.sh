#!/bin/bash
cmd.exe /c 'cd workspace && start ..\\bootpd\\workspace\\bin\\bootpd.exe -C config.ini -O v4_bind_address=10.0.0.1' &
cmd.exe /c 'cd workspace && plink -serial COM4 -sercfg 9600,8,n,1,N >serial.log' 
