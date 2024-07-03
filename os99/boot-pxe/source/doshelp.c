#include <stdint.h>
#include <stddef.h>
#include <i86.h>

#include "doshelp.h"
#include "asm.h"
#include "intr.h"

void init_dos_helper()
{
  pfvoid_t far* v_table = (pfvoid_t far*)get_ivt_entry(0xff);
}

