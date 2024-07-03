#include "pxe/pxe.h"

#include <i86.h>
#include <stdio.h>
#include <string.h>

bool PXE_checksum(void far* data, size_t size) 
{
  uint8_t sum = 0;
  for (size_t i = 0; i < size; i++)
    sum += ((uint8_t far*)data)[i];
  return sum == 0;
}

uint16_t PXE_init() 
{
  PXENVplus far* pxep;
  REGPACK r;
  
  r.w.ax = 0x5650;
  intr(0x1A, &r);
  if (r.w.ax != 0x564E || (r.w.flags & 1))
    return 0xffff;
  pxep = (PXENVplus far*)MK_FP(r.w.es, r.w.bx);
  if (!PXE_checksum(pxep, pxep->Length))
    return 0xffff;
  if (_fmemcmp(pxep->Signature, "PXENV+", 6) != 0)
    return 0xffff;  

  printf("PXE-VERSION .... : %d.%d\n", pxep->Version >> 8, pxep->Version & 0xff);
  printf("RMEntry ........ : %04X:%04X\n", pxep->RMEntry.seg, pxep->RMEntry.off);
  printf("PMOffset ....... : %08X\n", pxep->PMOffset);
  printf("PMSelector ..... : %04X\n", pxep->PMSelector);
  printf("StackSeg ....... : %04X\n", pxep->StackSeg);
  printf("StackSize ...... : %04X\n", pxep->StackSize);
  printf("BC_CodeSeg ..... : %04X\n", pxep->BC_CodeSeg);
  printf("BC_CodeSize .... : %04X\n", pxep->BC_CodeSize);
  printf("BC_DataSeg ..... : %04X\n", pxep->BC_DataSeg);
  printf("BC_DataSize .... : %04X\n", pxep->BC_DataSize);
  printf("UNDIDataSeg .... : %04X\n", pxep->UNDIDataSeg);
  printf("UNDIDataSize ... : %04X\n", pxep->UNDIDataSize);
  printf("UNDICodeSeg .... : %04X\n", pxep->UNDICodeSeg);
  printf("UNDICodeSize ... : %04X\n", pxep->UNDICodeSize);
  printf("PXEPtr ......... : %Fp\n", pxep->PXEPtr);


 
  return pxep->Version;
}
