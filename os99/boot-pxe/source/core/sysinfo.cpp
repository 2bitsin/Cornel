#include "core/sysinfo.h"
#include "x86/bios.h"
#include "pxe/pxe.h"
#include "core/error.h"

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <i86.h>

static inline system_info far *info()
{
  return (system_info far *)MK_FP(0x0050, 0x0000);
}

static inline error_type SYSINFO_acquire_memory_map()
{
  uint16_t status = 0;
  uint16_t index = 0;
  uint32_t next = 0;
  uint32_t size = 0;
  _fmemset(&info()->memory_map, 0, 
    sizeof(info()->memory_map));
  do
  {
    if (index >= NUMBER_OF_MEMMAP_ITEMS)
      return make_error(ACQUIRE_BIOS_MEMMAP_OVERFLOW);
    size = sizeof(info()->memory_map.data[index]);
    status = BIOS_query_memmap(&info()->memory_map.data[index], &size, &next);
    if (0u != status)
      return make_error(ACQUIRE_BIOS_MEMMAP_FAILED, status);
    index += 1;
  } while (next != 0);
  info()->memory_map.size = index;
  return make_error(NO_ERROR_SUCCESS);
}

static inline error_type SYSINFO_acquire_dhcp_info()
{
  PXE_bootph_type far *packet = NULL;
  uint16_t length = 0;
  uint16_t status = 0;
  _fmemset(&info()->dhcp_info, 0, sizeof(info()->dhcp_info));
  status = PXE_get_cached_info(cached_reply, &length, (PXE_bootph_type_pfar *)&packet);
  if (status != success || packet == NULL)
    return make_error(ACQUIRE_PXE_DHCP_INFO_FAILED, status);
  _fmemcpy(&info()->dhcp_info.server_address[0], &packet->Sip[0], sizeof(packet->Sip));
  _fmemcpy(&info()->dhcp_info.gateway_address[0], &packet->Gip[0], sizeof(packet->Gip));
  _fmemcpy(&info()->dhcp_info.client_address[0], &packet->Cip[0], sizeof(packet->Cip));
  _fmemcpy(&info()->dhcp_info.client_mac[0], &packet->CAddr[0], sizeof(packet->CAddr));
  _fmemcpy(&info()->dhcp_info.server_name[0], &packet->Sname[0], sizeof(packet->Sname));
  _fmemcpy(&info()->dhcp_info.boot_file[0], &packet->Bootfile[0], sizeof(packet->Bootfile));
  return make_error(NO_ERROR_SUCCESS);
}

static inline error_type SYSINFO_probe_serial() 
{
  static uint16_t far* port_addr = (uint16_t far*)0x400u;
  int i = 0;

  while (!port_addr[i]) i+=1u;

  if (i < 4)
  {
    info()->serial.port = i;
    info()->serial.conf = 0xe3;
  }
  
  return NO_ERROR_SUCCESS;
}

error_type SYSINFO_acquire()
{
  error_type status = NO_ERROR_SUCCESS;
  status = SYSINFO_acquire_memory_map();
  if (status != NO_ERROR_SUCCESS)
    return status;
  status = SYSINFO_acquire_dhcp_info();
  if (status != NO_ERROR_SUCCESS)
    return status;
  status = SYSINFO_probe_serial();
  if (status != NO_ERROR_SUCCESS)
    return status;
  return status;
}

system_info far *SYSINFO_info()
{
  return info();
}