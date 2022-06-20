#include "x86assembly.h"
#include "memquery.h"
#include "print.h"

extern void __cdecl G_stack_top ();

void bootload_probe_memory();

__declspec(noreturn) 
void bootload ()
{    
  x86_load_ss_sp(&G_stack_top);  
  
  x86_load_ds(x86_cs());
  x86_load_es(x86_cs());
  x86_load_gs(x86_cs());
  x86_load_fs(x86_cs());

  print_string("Starting bootload.sys\n");

  bootload_probe_memory();

  x86_cli();
  x86_hlt();
}

void bootload_probe_memory()
{
  MQ_e820_params_type params;
  params.size = sizeof(params);
  params.next = 0;
  do
  {
    MQ_e820(&params);
    print_hex64(params.entry.base);
    print_string(" | ");
    print_hex64(params.entry.size);
    print_string(" | ");
    print_hex64(params.entry.type);
    print_string("\n");   
  }
  while(params.next != 0);
}

