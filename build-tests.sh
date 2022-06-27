gcc -g -O0 "-D__int64=long long" -Isource/bootload.sys/include source/bootload.sys/test_print/main.c source/bootload.sys/print.c -o test_print
./test_print