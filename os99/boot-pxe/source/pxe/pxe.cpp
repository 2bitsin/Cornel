#include "pxe/pxe.h"

#include <string.h>
#include <stdio.h>
#include <i86.h>

#include "x86/debug.h"

static PXENVplus far *s_PXENVplus;
static PXEbang far *s_PXEbang;

bool PXE_checksum(void far *data, size_t size)
{
  uint8_t sum = 0;
  for (size_t i = 0; i < size; i++)
    sum += ((uint8_t far *)data)[i];
  return sum == 0;
}


void PXE_print_dhcp(PXE_bootph_type const far *packet)
{
#if 0
  puts(" --- PXE Cached Info ---");
  printf("Opcode ........... : %d\n",   packet->Opcode);
  printf("Hardware ......... : %d\n",   packet->Hardware);
  printf("Hardlen .......... : %d\n",   packet->Hardlen);
  printf("Gatehops ......... : %d\n",   packet->Gatehops);
  printf("Ident ............ : %08X\n", packet->Ident);
  printf("Seconds .......... : %d\n",   packet->Seconds);
  printf("Flags ............ : %02X\n", packet->Flags);
  printf("Client IP ........ : %d.%d.%d.%d\n", 
         packet->Cip[0],
         packet->Cip[1],
         packet->Cip[2],
         packet->Cip[3]);
  printf("Your IP .......... : %d.%d.%d.%d\n", 
         packet->Yip[0],
         packet->Yip[1],
         packet->Yip[2],
         packet->Yip[3]);
  printf("Server IP ........ : %d.%d.%d.%d\n", 
         packet->Sip[0],
         packet->Sip[1],
         packet->Sip[2],
         packet->Sip[3]);
  printf("Gateway IP ....... : %d.%d.%d.%d\n", 
         packet->Gip[0],
         packet->Gip[1],
         packet->Gip[2],
         packet->Gip[3]);
  printf("Client MAC ....... : "
         "%02X:%02X:%02X:%02X:%02X:%02X\n",
         packet->CAddr[0],
         packet->CAddr[1],
         packet->CAddr[2],
         packet->CAddr[3],
         packet->CAddr[4],
         packet->CAddr[5]);
  static char buff[128];
  _fstrncpy(&buff[0], &packet->Sname[0], sizeof(packet->Sname));
  printf("Server Name ...... : %s\n", buff); 
  _fstrncpy(&buff[0], &packet->Bootfile[0], sizeof(packet->Bootfile));
  printf("Boot File Name ... : %s\n", buff); 
#endif
}

void PXE_print_info()
{
#if 0
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
#endif
#if 0
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
#endif
#if 0
  PXE_print_dhcp(s_cached_reply_ptr);
#endif
}

PXE_status PXE_init()
{
  s_PXEbang = NULL;
  s_PXENVplus = NULL;
  uint16_t status = 0;
  REGPACK r;
  r.w.ax = 0x5650;
  intr(0x1A, &r);
  if (r.w.ax != 0x564E || (r.w.flags & 1))
    return pxe_not_available;
  s_PXENVplus = (PXENVplus far *)MK_FP(r.w.es, r.w.bx);
  if (!PXE_checksum(s_PXENVplus, s_PXENVplus->Length))
    return pxenv_bad_checksum;
  if (_fmemcmp(s_PXENVplus->Signature, "PXENV+", 6) != 0)
    return pxenv_bad_signature;
  if (s_PXENVplus->Version < 0x0201)
    return version_not_supported;  
  s_PXEbang = s_PXENVplus->PXEPtr;
  printf("StatusCallout=%04X:%04X\n", 
    s_PXEbang->StatusCallout.seg,
    s_PXEbang->StatusCallout.off);
  _fmemset(&s_PXEbang->StatusCallout, 0, sizeof(s_PXEbang->StatusCallout));
  return success;
}

PXE_status PXE_call_api(uint16_t v_command, void far *v_params)
{
  if (!s_PXEbang)
    return pxe_not_available;
  if (!v_params)
    return invalid_parameter;
  s_PXEbang->EntryPointSP(v_command, v_params);
  return (PXE_status)((uint16_t far *)v_params)[0];
}

PXE_status PXE_get_cached_info(PXENV_packet_type type, uint16_t* length, PXE_bootph_type_pfar* packet)
{
  uint16_t status = 0;
  PXENV_get_cached_info_type args;
  _fmemset(&args, 0, sizeof(args));
  args.PacketType = type;
  status = PXE_call_api(PXENV_GET_CACHED_INFO, &args);
  if (status != success || args.Status != success)
    return (PXE_status)status;  
  if (packet) *packet = (PXE_bootph_type far *)args.Buffer;
  if (length) *length = args.BufferSize;   
  return success;
}

PXE_status PXE_query_size(char const far* file_name, uint32_t *size) 
{
  TFTP_get_file_size_type args;
  PXE_bootph_type_pfar packet;
  PXE_status status;
  uint16_t length;
  status = PXE_get_cached_info(cached_reply, &length, &packet);
  if (status != success || !packet) {
    return status;
  }
  _fmemset(&args, 0, sizeof(args));
  _fmemcpy(args.ServerIPAddress, packet->Sip, sizeof(packet->Sip));
  _fmemcpy(args.GatewayIPAddress, packet->Gip, sizeof(packet->Gip));
  _fstrncpy(args.FileName, file_name, sizeof(args.FileName));
  status = PXE_call_api(TFTP_GET_FILE_SIZE, &args);
  if (status != success) {
    return status;
  }
  *size = args.FileSize;
  return success;
}

PXE_status PXE_download(char const far *file_name, uint32_t target_address, uint32_t* target_size) 
{
  TFTP_read_file_type args;
  PXE_bootph_type_pfar packet;
  PXE_status status;
  uint16_t length;
  status = PXE_get_cached_info(cached_reply, &length, &packet);
  if (status != success || !packet) {
    return status;
  }
  _fmemset(&args, 0, sizeof(args));
  _fmemcpy(args.ServerIPAddress, packet->Sip, sizeof(packet->Sip));
  _fmemcpy(args.GatewayIPAddress, packet->Gip, sizeof(packet->Gip));
  _fstrncpy(args.FileName, file_name, sizeof(args.FileName));
  args.Buffer = target_address;
  args.BufferSize = *target_size;
  args.TFTPClntPort = 69;
  args.TFTPSrvPort = 69;
  args.TFTPOpenTimeOut = 10;
  args.TFTPReopenDelay = 10;
  status = PXE_call_api(TFTP_READ_FILE, &args);
  if (status != success) {
    return status;
  }
  *target_size = args.BufferSize;
  return success;
}
