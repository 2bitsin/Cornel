#ifndef _BIOSKEY_H_
#define _BIOSKEY_H_

unsigned short wait_for_key();

#pragma aux wait_for_key = "xor ax, ax" "int 0x16" value [ax]

#endif
