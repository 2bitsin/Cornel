#include "pxe/pxe.h"

#include <i86.h>
#include <stdio.h>
#include <string.h>

static PXENVplus far* s_PXENVplus;
static PXEbang far* s_PXEbang;

bool PXE_checksum(void far* data, size_t size) 
{
  uint8_t sum = 0;
  for (size_t i = 0; i < size; i++)
    sum += ((uint8_t far*)data)[i];
  return sum == 0;
}

void PXE_print_info()
{
  puts(" --- PXENV+ ---");

  printf("PXENV+ Version ... : v%d.%d\n", s_PXENVplus->Version >> 8, s_PXENVplus->Version & 0xFF);
  printf("Length ........... : %d\n", s_PXENVplus->Length);
  printf("Checksum ......... : %d\n", s_PXENVplus->Checksum);
  printf("RMEntry .......... : %04X:%04X\n", s_PXENVplus->RMEntry.seg, s_PXENVplus->RMEntry.off);
  printf("PMOffset ......... : %08X\n", s_PXENVplus->PMOffset);
  printf("PMSelector ....... : %04X\n", s_PXENVplus->PMSelector);
  printf("StackSeg ......... : %04X\n", s_PXENVplus->StackSeg);
  printf("StackSize ........ : %04X\n", s_PXENVplus->StackSize);
  printf("BC_CodeSeg ....... : %04X\n", s_PXENVplus->BC_CodeSeg);
  printf("BC_CodeSize ...... : %04X\n", s_PXENVplus->BC_CodeSize);
  printf("BC_DataSeg ....... : %04X\n", s_PXENVplus->BC_DataSeg);
  printf("BC_DataSize ...... : %04X\n", s_PXENVplus->BC_DataSize);
  printf("UNDIDataSeg ...... : %04X\n", s_PXENVplus->UNDIDataSeg);
  printf("UNDIDataSize ..... : %04X\n", s_PXENVplus->UNDIDataSize);
  printf("UNDICodeSeg ...... : %04X\n", s_PXENVplus->UNDICodeSeg);
  printf("UNDICodeSize ..... : %04X\n", s_PXENVplus->UNDICodeSize);
  printf("PXEPtr ........... : %04X:%04X\n", s_PXENVplus->PXEPtr->UNDIROMID.seg, s_PXENVplus->PXEPtr->UNDIROMID.off);  

  puts(" --- PXE! ---");

  printf("Signature ........ : %c%c%c%c\n", s_PXEbang->Signature[0], s_PXEbang->Signature[1], s_PXEbang->Signature[2], s_PXEbang->Signature[3]);
  printf("StructLength ..... : %d\n", s_PXEbang->StructLength);
  printf("StructCksum ...... : %d\n", s_PXEbang->StructCksum);
  printf("StructRev ........ : %d\n", s_PXEbang->StructRev);
  printf("UNDIROMID ........ : %04X:%04X\n", s_PXEbang->UNDIROMID.seg, s_PXEbang->UNDIROMID.off);
  printf("BaseROMID ........ : %04X:%04X\n", s_PXEbang->BaseROMID.seg, s_PXEbang->BaseROMID.off);
  printf("EntryPointSP ..... : %04X:%04X\n", s_PXEbang->EntryPointSP, s_PXEbang->EntryPointSP);
  printf("EntryPointESP .... : %04X:%04X\n", s_PXEbang->EntryPointESP.seg, s_PXEbang->EntryPointESP.off);
  printf("StatusCallout .... : %04X:%04X\n", s_PXEbang->StatusCallout.seg, s_PXEbang->StatusCallout.off);
  printf("SegDescCnt ....... : %d\n", s_PXEbang->SegDescCnt);
  printf("FirstSelector .... : %04X\n", s_PXEbang->FirstSelector);
  printf("Stack ............ : %04X:%08X:%04X\n", s_PXEbang->Stack.segment_address, s_PXEbang->Stack.physical_address, s_PXEbang->Stack.segment_length);
  printf("UNDIData ......... : %04X:%08X:%04X\n", s_PXEbang->UNDIData.segment_address, s_PXEbang->UNDIData.physical_address, s_PXEbang->UNDIData.segment_length);
  printf("UNDICode ......... : %04X:%08X:%04X\n", s_PXEbang->UNDICode.segment_address, s_PXEbang->UNDICode.physical_address, s_PXEbang->UNDICode.segment_length);
  printf("UNDICodeWrite .... : %04X:%08X:%04X\n", s_PXEbang->UNDICodeWrite.segment_address, s_PXEbang->UNDICodeWrite.physical_address, s_PXEbang->UNDICodeWrite.segment_length);  
}

PXE_status PXE_init() 
{  
  REGPACK r;  
  r.w.ax = 0x5650;
  intr(0x1A, &r);
  if (r.w.ax != 0x564E || (r.w.flags & 1))
    return pxe_not_available;
  s_PXENVplus = (PXENVplus far*)MK_FP(r.w.es, r.w.bx);
  if (!PXE_checksum(s_PXENVplus, s_PXENVplus->Length))
    return pxenv_bad_checksum;
  if (_fmemcmp(s_PXENVplus->Signature, "PXENV+", 6) != 0)
    return pxenv_bad_signature;
  if (s_PXENVplus->Version < 0x0201)
    return version_not_supported;
  s_PXEbang = s_PXENVplus->PXEPtr;
  return success;
}

PXE_status PXE_get_cached_info(PXENV_packet_type packet_type, PXE_bootph_type far* info_buff, size_t far* info_size)
{
  static PXENV_get_cached_info_type __far v_params;
  v_params.PacketType = packet_type;
  v_params.Buffer = info_buff;
  v_params.BufferSize = *info_size;
  s_PXEbang->EntryPointSP(0x71u, &v_params);
  if (v_params.Status == success) {    
    *info_size = v_params.BufferSize; }
  return (PXE_status)v_params.Status;
}
