#include <stdio.h>

int main(int argc, char** argv) 
{    
  __asm{ xchg bx, bx }
  puts("Hello World!\n");
  return 0;
}

