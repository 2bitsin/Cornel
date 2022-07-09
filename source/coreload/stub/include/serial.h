#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "types.h"

#define SERIAL_PORT_COM1            0x0
#define SERIAL_PORT_COM2            0x1
#define SERIAL_PORT_COM3            0x2
#define SERIAL_PORT_COM4            0x3

#define SERIAL_PARITY_NONE          0x0
#define SERIAL_PARITY_ODD           0x1
#define SERIAL_PARITY_EVEN          0x2
#define SERIAL_PARITY_MARK          0x3
#define SERIAL_PARITY_SPACE         0x4

#define SERIAL_ERROR_BAD_BAUD_RATE  -1
#define SERIAL_ERROR_BAD_PORT       -2
#define SERIAL_ERROR_BAD_DATA_BITS  -3
#define SERIAL_ERROR_BAD_PARITY     -4
#define SERIAL_ERROR_BAD_STOP_BITS  -5
#define SERIAL_ERROR_SELF_TEST      -6
#define SERIAL_ERROR_TIMEOUT        -7

typedef struct serial_port_init_t
{
  uint32_t baud;
  uint8_t  data_bits;
  uint8_t  parity;
  uint8_t  stop_bits;  
} serial_port_init_type;

int16_t   SER_init();
int16_t   SER_init_port(uint16_t port, const serial_port_init_type* init);
uint16_t  SER_get_port_irq_mask(uint16_t port);
uint16_t  SER_get_port_base(uint16_t port);

int16_t   SER_can_receive_now(uint16_t base);
int16_t   SER_can_transmit_now(uint16_t base);
int16_t   SER_sync_transmit_byte(uint16_t base, char value);
int16_t   SER_sync_transmit_string(uint16_t base, const char* stringz);
int16_t   SER_sync_receive_byte(uint16_t base);
int16_t   SER_self_test(uint16_t base, uint8_t value);

#endif

