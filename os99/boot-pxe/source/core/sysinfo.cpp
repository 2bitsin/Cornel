#include "core/sysinfo.h"
#include "x86/bios.h"

#include <i86.h>

static inline system_info far* info() {
  return (system_info far*)MK_FP(0x0050, 0x0000);
} 

combined_error_type SYSINFO_acquire() 
{
  uint16_t status = 0;
  uint32_t next = 0;
  uint32_t size = 0;
  uint16_t index = 0;
_collect_memmap:
  size = sizeof(info()->memory_map[index]);
  status = BIOS_query_memmap(&info()->memory_map[index], &size, &next);
  if (0u != status) {
    return make_combined_error(
      ACQUIRE_BIOS_MEMMAP_FAILED, status);
  }
  if (0u != next) {
    goto _collect_memmap;
  }
  if (index >= NUMBER_OF_MEMMAP_ITEMS) {
    return make_combined_error(
      ACQUIRE_BIOS_MEMMAP_OVERFLOW, 0);
  }
  return make_combined_error(NO_ERROR_SUCCESS,0);
}