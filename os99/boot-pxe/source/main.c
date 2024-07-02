#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "doshelp.h"

int main(int argc, char** argv) 
{       
  (void)argc;
  (void)argv;
  init_dos_helper();
  printf("Hello %s!\n", "World");
  return 0;
}

