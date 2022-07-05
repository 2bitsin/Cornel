#include "types.h"
#include "x86assembly.h"
#include "memory.h"
#include "print.h"
#include "bioskey.h"
#include "kbdctrl.h"
#include "flatreal.h"

static const char test_string [] = "FLAT REAL MODE TEST STRING!";
static char buffer[2048];

void FLAT_test()
{
  print_string("Testing flat real mode ...\n");

  print_string("Test string : ");
  print_string(test_string);
  print_string("\n");

  print_string("Copying test string above 2MB mark...\n");
  FLAT_copy1(0x200000, FLAT_neartolinear(&test_string[0]), sizeof (test_string));

  print_string("Copying test string from abov 2MB mark, back to below 1MB buffer...\n");
  FLAT_copy1(FLAT_neartolinear(&buffer[0]), 0x200000, sizeof (test_string));

  print_string("Test string in buffer : ");
  print_string(buffer);
  print_string("\n");
}


void STUB_init ()
{     
  print_string("Starting Cornel OS ...\n");  
#ifdef DEBUG
  print_string("Initializing flat real mode ...\n");
#endif
  FLAT_init();    

//#ifdef FALT_REAL_MODE_TEST
  FLAT_test();
//#endif

  print_string("Initializing memory map ...\n");
  MEM_init();
}

void STUB_exit () 
{
  print_string("Press any key to reboot...\n");
  wait_for_key();  
  KBC_hardware_reset();
}

__declspec(naked)
void STUB_main ()
{ 
  x86_cli();
  x86_load_sp((void*)G_STACK_SIZE);
  x86_load_all_seg(x86_cs());
  x86_sti();
  STUB_init();
  STUB_exit();
}
