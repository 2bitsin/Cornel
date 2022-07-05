#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "types.h"

#define SERIAL_PORT_COM1    0x1
#define SERIAL_PORT_COM2    0x2
#define SERIAL_PORT_COM3    0x3
#define SERIAL_PORT_COM4    0x4

void SER_init();
void SER_init_port(int16_t port);

#endif