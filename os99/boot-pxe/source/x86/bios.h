#pragma once

#include <stddef.h>
#include <stdint.h>

#include "core/sysinfo.h"

uint16_t __watcall BIOS_query_memmap(memmap_item far* v_buff, uint32_t *v_size, uint32_t *v_next);
