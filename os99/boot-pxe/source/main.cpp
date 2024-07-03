#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "x86/asm.h"
#include "pxe/pxe.h"

static PXE_bootph_type s_bootph_reply;
static size_t s_size;

int main(int, char**) 
{       
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  if (PXE_init() != success) {
    puts("PXE not available or unusable.");
    return -1;
  }
  PXE_print_info();
  s_size = sizeof(PXE_bootph_type);
  _fmemset(&s_bootph_reply, 0, sizeof(PXE_bootph_type));    
  if (PXE_get_cached_info(cached_reply, &s_bootph_reply, &s_size) != success) {
    puts("Failed to get PXE info.");
    return -1;
  }
  
  puts(" --- PXE BOOTP ---");
  printf("Client IP ....... : %d.%d.%d.%d\n", s_bootph_reply.cip.value_u8[0], s_bootph_reply.cip.value_u8[1], s_bootph_reply.cip.value_u8[2], s_bootph_reply.cip.value_u8[3]);
  printf("Your IP ......... : %d.%d.%d.%d\n", s_bootph_reply.yip.value_u8[0], s_bootph_reply.yip.value_u8[1], s_bootph_reply.yip.value_u8[2], s_bootph_reply.yip.value_u8[3]);
  printf("Your MAC ........ : %02X:%02X:%02X:%02X:%02X:%02X\n", s_bootph_reply.CAddr[0], s_bootph_reply.CAddr[1], s_bootph_reply.CAddr[2], s_bootph_reply.CAddr[3], s_bootph_reply.CAddr[4], s_bootph_reply.CAddr[5]);
  printf("Server IP ....... : %d.%d.%d.%d\n", s_bootph_reply.sip.value_u8[0], s_bootph_reply.sip.value_u8[1], s_bootph_reply.sip.value_u8[2], s_bootph_reply.sip.value_u8[3]);
  printf("Gateway IP ...... : %d.%d.%d.%d\n", s_bootph_reply.gip.value_u8[0], s_bootph_reply.gip.value_u8[1], s_bootph_reply.gip.value_u8[2], s_bootph_reply.gip.value_u8[3]);        
  return 0;
}


