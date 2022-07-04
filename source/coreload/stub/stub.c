#include "x86assembly.h"
#include "memory.h"
#include "print.h"
#include "keyboard.h"
#include "flatreal.h"

static const char test_string [] = "FLAT REAL MODE TEST STRING!";
static char buffer[2048];

void FLAT_test()
{
  print_string("Test string : ");
  print_string(test_string);
  print_string("\n");

  print_string("Copying test string above 2MB mark...\n");
  FLAT_copy(2ul*1024ul*1024ul, G_LOAD_ADDRESS - G_STACK_SIZE + (unsigned int)&test_string[0], sizeof (test_string));

  print_string("Copying test string from abov 2MB mark, back to below 1MB buffer...\n");
  FLAT_copy(G_LOAD_ADDRESS - G_STACK_SIZE + (unsigned int)&buffer[0], 2ul*1024ul*1024ul, sizeof (test_string));

  print_string("Test string in buffer : ");
  print_string(buffer);
  print_string("\n");

  print_string("Press any key to continue...\n");
  wait_for_key();
}


void STUB_init ()
{     
  print_string("Starting Cornel OS ...\n");  

  print_string("Initializing flat real mode ...\n");
  FLAT_init();    

  print_string("Testing flat real mode ...\n");
  FLAT_test();

  print_string("Initializing memory map ...\n");
  MEM_init();


  print_string("Boot failed.\nPress any key to reboot.\n");
  wait_for_key();
}
