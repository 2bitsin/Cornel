#include <stdio.h>


int main(int argc, char** argv) 
{     
  puts("Hello World!\n");
  __asm{ xchg bx, bx }
  return 0;
}

