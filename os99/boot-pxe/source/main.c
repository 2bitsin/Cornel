#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "asm.h"

extern  char    **environ;

int main(int argc, char** argv) 
{       
  int i ;
  char* s_name;
  
  for(i = 0; i < argc; ++i) {
    printf("ARG[%i] = \"%s\"\n", i, argv[i]);
  }
  for(i = 0; environ[i]!=0; ++i) {
    printf("ENV[%i] = \"%s\"\n", i, environ[i]);
  }

  s_name = strdup("World");
  printf("Hello %s [%04x]!\n", s_name, (uint16_t)s_name);
  free(s_name);

  __debugbreak();
  return 0;
}

