#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "doshelp.h"
#include "asm.h"

extern  char    **environ;

int main(int argc, char** argv) 
{       
  int i ;
  for(i = 0; i < argc; ++i) {
    printf("ARG[%i] = \"%s\"\n", i, argv[i]);
  }
  for(i = 0; environ[i]!=0; ++i) {
    printf("ENV[%i] = \"%s\"\n", i, environ[i]);
  }
  return 0;
}

