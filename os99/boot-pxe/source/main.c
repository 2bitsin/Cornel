#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

int main(int argc, char** argv) 
{       
  void init_dos_helper();
  (void)argc;
  (void)argv;
  printf("Hello %s!\n", "World");
  return 0;
}

