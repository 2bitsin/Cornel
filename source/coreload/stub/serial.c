#include "serial.h"
#include "x86asm.h"
#include "irqstubs.h"
#include "print.h"
#include "longdiv.h"

#define SERIAL_PORT_BDR_LSB       0x100
#define SERIAL_PORT_BDR_MSB       0x101

#define SERIAL_PORT_DR            0x000
#define SERIAL_PORT_IER           0x001
#define SERIAL_PORT_IIR           0x002
#define SERIAL_PORT_FCR           0x002
#define SERIAL_PORT_LCR           0x003
#define SERIAL_PORT_MCR           0x004
#define SERIAL_PORT_LSR           0x005
#define SERIAL_PORT_MSR           0x006
#define SERIAL_PORT_SCR           0x007

#define SERIAL_IER_RDA_BIT        0x01
#define SERIAL_IER_THRE_BIT       0x02
#define SERIAL_IER_RLS_BIT        0x04
#define SERIAL_IER_MS_BIT         0x08
#define SERIAL_IER_SM_BIT         0x10
#define SERIAL_IER_LPM_BIT        0x20

#define SERIAL_LCR_WLEN5_BIT      0x00
#define SERIAL_LCR_WLEN6_BIT      0x01
#define SERIAL_LCR_WLEN7_BIT      0x02
#define SERIAL_LCR_WLEN8_BIT      0x03

#define SERIAL_LCR_2STOP_BIT      0x04
#define SERIAL_LCR_PARENB_BIT     0x08
#define SERIAL_LCR_PAR_ODD_BIT    0x00
#define SERIAL_LCR_PAR_EVEN_BIT   0x10
#define SERIAL_LCR_PAR_MARK_BIT   0x20
#define SERIAL_LCR_PAR_SPACE_BIT  0x30
#define SERIAL_LCR_BREAK_BIT      0x40
#define SERIAL_LCR_DLAB_BIT       0x80

#define SERIAL_FCR_FIFO_EN_BIT    0x01
#define SERIAL_FCR_CLR_RCVR_BIT   0x02
#define SERIAL_FCR_CLR_XMIT_BIT   0x04
#define SERIAL_FCR_DMA_MODE_BIT   0x08

#define SERIAL_FCR_1BYTE_BIT      0x00
#define SERIAL_FCR_4BYTE_BIT      0x40
#define SERIAL_FCR_8BYTE_BIT      0x80
#define SERIAL_FCR_14BYTE_BIT     0xC0

#define SERIAL_MCR_DTR_BIT        0x01
#define SERIAL_MCR_RTS_BIT        0x02
#define SERIAL_MCR_OUT1_BIT       0x04
#define SERIAL_MCR_MIDI_BIT       0x04
#define SERIAL_MCR_OUT2_BIT       0x08
#define SERIAL_MCR_IRQ_BIT        0x08
#define SERIAL_MCR_LOOP_BIT       0x10
#define SERIAL_MCR_XOFF_BIT       0x20

#define SERIAL_LSR_DR_BIT         0x01
#define SERIAL_LSR_OE_BIT         0x02
#define SERIAL_LSR_PE_BIT         0x04
#define SERIAL_LSR_FE_BIT         0x08
#define SERIAL_LSR_BI_BIT         0x10
#define SERIAL_LSR_THRE_BIT       0x20
#define SERIAL_LSR_TEMT_BIT       0x40
#define SERIAL_LSR_ERR_BIT        0x80

#define SERIAL_MSR_DCTS_BIT       0x01
#define SERIAL_MSR_DDSR_BIT       0x02
#define SERIAL_MSR_TERI_BIT       0x04
#define SERIAL_MSR_DDCD_BIT       0x08
#define SERIAL_MSR_CTS_BIT        0x10
#define SERIAL_MSR_DSR_BIT        0x20
#define SERIAL_MSR_RI_BIT         0x40
#define SERIAL_MSR_DCD_BIT        0x80

static uint8_t G_port_mask;

uint16_t __cdecl SER_irq(int16_t irq_n)
{
  return IRQ_CALL_DEFAULT;
}

int16_t SER_init()
{ 
  uint16_t value;  
  G_port_mask = 0;

  DBG_print_string("\nInitializing serial COM's ...... ");                  
  DBG_print_string("\n* Base addresses:");    
  if (value = read_bda_word(0)) {
    DBG_print_string("\n  COM1: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ4_BIT;
  }
  if (value = read_bda_word(2)) {
    DBG_print_string("\n  COM2: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ3_BIT;
  }
  if (value = read_bda_word(4)) {
    DBG_print_string("\n  COM3: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ4_BIT;
  }
  if (value = read_bda_word(6)) {
    DBG_print_string("\n  COM4: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ3_BIT;
  }
  if (!G_port_mask)
  {
    DBG_print_string("\n  No COM ports found!");
    return -1;
  }
  IRQ_init(G_port_mask);
  DBG_print_char('\n');  
  return 0;
}

static uint16_t SER_get_port_base(uint16_t port)
{
  switch (port)
  {
    case SERIAL_PORT_COM1:
      return read_bda_word(0);
    case SERIAL_PORT_COM2:
      return read_bda_word(2);
    case SERIAL_PORT_COM3:
      return read_bda_word(4);
    case SERIAL_PORT_COM4:
      return read_bda_word(6);
    default:
      return 0;
  }
  return 0;
}

static uint16_t SER_get_port_irq_mask(uint16_t port)
{
  switch (port)
  {
    case SERIAL_PORT_COM1:
      return IRQ_INIT_IRQ4_BIT;
    case SERIAL_PORT_COM2:
      return IRQ_INIT_IRQ3_BIT;
    case SERIAL_PORT_COM3:
      return IRQ_INIT_IRQ4_BIT;
    case SERIAL_PORT_COM4:
      return IRQ_INIT_IRQ3_BIT;
    default:
      return 0;
  }
  return 0;
}

static uint8_t SER_get_line_control_bits(const serial_port_init_type* init)
{
  uint8_t bits = 0;

  switch (init->data_bits)
  {
  case 5: bits |= SERIAL_LCR_WLEN5_BIT; break;
  case 6: bits |= SERIAL_LCR_WLEN6_BIT; break;
  case 7: bits |= SERIAL_LCR_WLEN7_BIT; break;
  case 8: bits |= SERIAL_LCR_WLEN8_BIT; break;
  }

  if (init->stop_bits == 2)
    bits |= SERIAL_LCR_2STOP_BIT;
  
  switch(init->parity)
  {
  case SERIAL_PARITY_NONE:    
    break;
  case SERIAL_PARITY_ODD:
    bits |= SERIAL_LCR_PAR_ODD_BIT|SERIAL_LCR_PARENB_BIT;
    break;
  case SERIAL_PARITY_EVEN:
    bits |= SERIAL_LCR_PAR_EVEN_BIT|SERIAL_LCR_PARENB_BIT;
    break;
  case SERIAL_PARITY_MARK:
    bits |= SERIAL_LCR_PAR_MARK_BIT|SERIAL_LCR_PARENB_BIT;
    break;
  case SERIAL_PARITY_SPACE:
    bits |= SERIAL_LCR_PAR_SPACE_BIT|SERIAL_LCR_PARENB_BIT;
    break;
  }

  return bits;
}

static int16_t SER_self_test(uint16_t base, uint8_t value)
{
  x86_outb(base + SERIAL_PORT_DR, value);
  if (x86_inb(base + SERIAL_PORT_DR) != value)
    return SERIAL_ERROR_SELF_TEST;
  return 0;
}

int16_t SER_init_port(uint16_t port, const serial_port_init_type* init)
{
  static u_longdiv_type ldt;
  static uint32_t di;

  uint16_t baud_div;
  uint16_t base_port;

  if (!(base_port = SER_get_port_base(port)))
    return SERIAL_ERROR_BAD_PORT;

  if (init->baud > 115200 || init->baud < 2)
    return SERIAL_ERROR_BAD_BAUD_RATE;

  if (init->data_bits > 8 || init->data_bits < 5)
    return SERIAL_ERROR_BAD_DATA_BITS;
   
  if (init->parity > SERIAL_PARITY_MARK)
    return SERIAL_ERROR_BAD_PARITY;

  if (init->stop_bits > 2 || init->stop_bits < 1)
    return SERIAL_ERROR_BAD_STOP_BITS;
    
  ldt.d = 115200;
  di = init->baud;
  long_64_udiv_32(&ldt, &di);
  if (ldt.r != 0) {
    DBG_print_string("\nWARNING! Non-standard baud rate, rounding up to nearest standard rate.");
    ldt.q += 1;
  }  

  baud_div = ldt.q;

#ifdef DEBUG
  DBG_print_string("\n* Initializing COM port ... : ");
  DBG_print_string("\n  Base address ............ : ");
  DBG_print_hex16(port);  
  ldt.d = 115200;
  di = baud_div;
  long_64_udiv_32(&ldt, &di);
  DBG_print_string("\n  Baud rate ............... : ");
  DBG_print_dec32(ldt.q);
  DBG_print_string("\n  Data bits ............... : ");
  DBG_print_dec32(init->data_bits);
  DBG_print_string("\n  Parity .................. : ");
  switch(init->parity)
  {
    case SERIAL_PARITY_NONE:
      DBG_print_string("None");
      break;
    case SERIAL_PARITY_ODD:
      DBG_print_string("Odd");
      break;
    case SERIAL_PARITY_EVEN:
      DBG_print_string("Even");
      break;
    case SERIAL_PARITY_MARK:
      DBG_print_string("Mark");
      break;
    default:
      DBG_print_string("Unknown");
      break;
  }
  DBG_print_string("\n  Stop bits ............... : ");
  DBG_print_dec32(init->stop_bits);
#endif

  x86_outb(base_port + SERIAL_PORT_IER, 0x00);
  x86_outb(base_port + SERIAL_PORT_LCR, SERIAL_LCR_DLAB_BIT);
  x86_outb(base_port + SERIAL_PORT_BDR_LSB, baud_div & 0xFF);
  x86_outb(base_port + SERIAL_PORT_BDR_MSB, (baud_div >> 8) & 0xFF);
  x86_outb(base_port + SERIAL_PORT_LCR, SER_get_line_control_bits(init));
  x86_outb(base_port + SERIAL_PORT_FCR, SERIAL_FCR_FIFO_EN_BIT|SERIAL_FCR_CLR_RCVR_BIT|SERIAL_FCR_CLR_XMIT_BIT|SERIAL_FCR_14BYTE_BIT);
  x86_outb(base_port + SERIAL_PORT_MCR, SERIAL_MCR_DTR_BIT|SERIAL_MCR_RTS_BIT|SERIAL_MCR_IRQ_BIT);

  x86_outb(base_port + SERIAL_PORT_MCR, SERIAL_MCR_RTS_BIT|SERIAL_MCR_OUT1_BIT|SERIAL_MCR_OUT2_BIT|SERIAL_MCR_LOOP_BIT);

  if (SER_self_test(base_port, 0x55) || SER_self_test(base_port, 0xAA)) {
    DBG_print_string("\n  ERROR! Self-test failed!");
    return SERIAL_ERROR_SELF_TEST;
  }  

  x86_outb(base_port + SERIAL_PORT_MCR, SERIAL_MCR_DTR_BIT|SERIAL_MCR_RTS_BIT|SERIAL_MCR_OUT1_BIT|SERIAL_MCR_OUT2_BIT);
  IRQ_enable(SER_get_port_irq_mask(port));

  return 0;  
}

static int16_t SER_is_transmit_empty(uint16_t base)
{
  if (x86_inb(base + SERIAL_PORT_LSR) & SERIAL_LSR_THRE_BIT)
    return 1;
  return 0;
}

int16_t SER_sync_send_char(uint16_t port, char value)
{
  uint16_t base;  
  if (!(base = SER_get_port_base(port)))
    return SERIAL_ERROR_BAD_PORT;
  while (!SER_is_transmit_empty(base));  
  x86_outb(base + SERIAL_PORT_DR, (uint8_t)value);
  return 0;
}

int16_t SER_sync_send_string(uint16_t port, const char* value)
{
  uint16_t base;  
  if (!(base = SER_get_port_base(port)))
    return SERIAL_ERROR_BAD_PORT;
  while (*value) {
    SER_sync_send_char(port, *value);
    ++value;
  }
  return 0;
}

