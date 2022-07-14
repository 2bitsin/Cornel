#include "serial.h"
#include "x86asm.h"
#include "irqctrl.h"
#include "print.h"
#include "longdiv.h"
#include "memory.h"

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

#define SERIAL_LCR_WLEN5_BIT      0x00
#define SERIAL_LCR_WLEN6_BIT      0x01
#define SERIAL_LCR_WLEN7_BIT      0x02
#define SERIAL_LCR_WLEN8_BIT      0x03

#define SERIAL_LCR_2STOP_BIT      0x04
#define SERIAL_LCR_PAR_ENB_BIT    0x08
#define SERIAL_LCR_PAR_ODD_BIT    (0x00|SERIAL_LCR_PAR_ENB_BIT)
#define SERIAL_LCR_PAR_EVEN_BIT   (0x10|SERIAL_LCR_PAR_ENB_BIT)
#define SERIAL_LCR_PAR_MARK_BIT   (0x20|SERIAL_LCR_PAR_ENB_BIT)
#define SERIAL_LCR_PAR_SPACE_BIT  (0x30|SERIAL_LCR_PAR_ENB_BIT)
#define SERIAL_LCR_BREAK_BIT      0x40
#define SERIAL_LCR_DLAB_BIT       0x80

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



#define SERIAL_IRQ_ENABLE_NONE                        0x00

#define SERIAL_IRQ_ENABLE_RECEIVE_DATA_AVAILBLE_BIT   0x01
#define SERIAL_IRQ_ENABLE_TRANSMIT_HOLD_EMPTY_BIT     0x02
#define SERIAL_IRQ_ENABLE_RECEIVE_LINE_STATUS_BIT     0x04
#define SERIAL_IRQ_ENABLE_MODEM_STATUS_CHANGE_BIT     0x08

#define SERIAL_IRQ_ENABLE_ALL_16550                  (SERIAL_IRQ_ENABLE_RECEIVE_DATA_AVAILBLE_BIT|\
                                                      SERIAL_IRQ_ENABLE_TRANSMIT_HOLD_EMPTY_BIT|\
                                                      SERIAL_IRQ_ENABLE_RECEIVE_LINE_STATUS_BIT|\
                                                      SERIAL_IRQ_ENABLE_MODEL_STATUS_CHANGE_BIT)

#define SERIAL_IRQ_ENABLE_16750_SLEEP_MODE_BIT        0x10
#define SERIAL_IRQ_ENABLE_16750_LOW_POWER_MODE_BIT    0x20

#define SERIAL_IRQ_ENABLE_ALL                        (SERIAL_IRQ_ENABLE_ALL_16550|\
                                                      SERIAL_IRQ_ENABLE_16750_SLEEP_MODE_BIT|\
                                                      SERIAL_IRQ_ENABLE_16750_LOW_POWER_MODE_BIT)


#define SERIAL_FIFO_CLEAR_RECEIVE                     0x1
#define SERIAL_FIFO_CLEAR_TRANSMIT                    0x2
#define SERIAL_FIFO_CLEAR_ALL                        (SERIAL_FIFO_CLEAR_RECEIVE|SERIAL_FIFO_CLEAR_TRANSMIT)

#define SERIAL_FIFO_TRIGGER_LEVEL_NONE                -1
#define SERIAL_FIFO_TRIGGER_LEVEL_1BYTE               +0
#define SERIAL_FIFO_TRIGGER_LEVEL_4BYTE               +1
#define SERIAL_FIFO_TRIGGER_LEVEL_8BYTE               +2
#define SERIAL_FIFO_TRIGGER_LEVEL_14BYTE              +3

#define SERIAL_IRQ_STATUS_MODEM_STATUS_CHANGE         0x0
#define SERIAL_IRQ_STATUS_TRANSMIT_HOLD_EMPTY         0x1
#define SERIAL_IRQ_STATUS_RECEIVE_DATA_AVAILABLE      0x2
#define SERIAL_IRQ_STATUS_LINE_STATUS_CHANGE          0x3
#define SERIAL_IRQ_STATUS_RESERVED_0X4                0x4
#define SERIAL_IRQ_STATUS_RESERVED_0X5                0x5
#define SERIAL_IRQ_STATUS_CHARACTER_TIMEOUT           0x6
#define SERIAL_IRQ_STATUS_CLEAR                       0x7

#define SERIAL_IRQ_STATUS_FIFO_DISABLED               0x0
#define SERIAL_IRQ_STATUS_FIFO_ENABLED                0x1

#define SERIAL_MODEM_CONTROL_DATA_TERMINAL_READY_BIT  0x01
#define SERIAL_MODEM_CONTROL_REQUEST_TO_SEND_BIT      0x02
#define SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_ONE_BIT  0x04
#define SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_TWO_BIT  0x08
#define SERIAL_MODEM_CONTROL_LOOP_BACK_MODE_BIT       0x10
#define SERIAL_MODEM_CONTROL_AUTO_FLOW_CONTROL_BIT    0x20

#define SERIAL_PORT_COUNT                             4
#define SERIAL_PORT_BUFFER_SIZE                       256

uint16_t G_port_mask = 'MP';

#ifdef USE_SERIAL_IRQS

volatile uint8_t G_modem_status[SERIAL_PORT_COUNT] = {'M', 'S', 'T', 'R'};
volatile uint8_t G_line_status[SERIAL_PORT_COUNT] = {'L', 'S', 'T', 'R'};
volatile uint8_t G_can_send_now = 0;

volatile uint8_t G_port_buffer[SERIAL_PORT_COUNT][SERIAL_PORT_BUFFER_SIZE] = 
{
#define B C, C, C, C, C, C, C, C
#define A B, B, B, B, B, B, B, B
#define C 'C', 'O', 'M', '1'
  { A },
#undef C 
#define C 'C', 'O', 'M', '2'
  { A },
#undef C
#define C 'C', 'O', 'M', '3'
  { A },
#undef C
#define C 'C', 'O', 'M', '4'
  { A }
#undef C 
#undef B 
#undef A 
};  
volatile uint16_t G_port_buffer_index[SERIAL_PORT_COUNT] = 
{
  '1C', '2C', '3C', '4C'
};
#endif

uint16_t SER_get_port_irq(uint16_t port)
{
  switch (port)
  {
    case SERIAL_PORT_COM1:
      return 4;
    case SERIAL_PORT_COM2:
      return 3;
    case SERIAL_PORT_COM3:
      return 4;
    case SERIAL_PORT_COM4:
      return 3;
    default:
      return 0;
  }
}

uint16_t SER_get_port_irq_mask(uint16_t port)
{
  return 1u << SER_get_port_irq(port);
}

uint16_t SER_get_port_base(uint16_t port)
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

int16_t SER_init()
{ 
  uint16_t value;  
  G_port_mask = 0;

  DBG_print_string("\nInitializing serial COM's ...... ");                  
  DBG_print_string("\n* Base addresses:");    
  if (value = SER_get_port_base(SERIAL_PORT_COM1)) {
    DBG_print_string("\n  COM1: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ4_BIT;
  }
  if (value = SER_get_port_base(SERIAL_PORT_COM2)) {
    DBG_print_string("\n  COM2: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ3_BIT;
  }
  if (value = SER_get_port_base(SERIAL_PORT_COM3)) {
    DBG_print_string("\n  COM3: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ4_BIT;
  }
  if (value = SER_get_port_base(SERIAL_PORT_COM4)) {
    DBG_print_string("\n  COM4: ");
    DBG_print_hex16(value);
    G_port_mask |= IRQ_INIT_IRQ3_BIT;
  }
  if (!G_port_mask)
  {
    DBG_print_string("\n  No COM ports found!");
    return SERIAL_ERROR_NO_COM_PORTS;
  }
  
  IRQ_init(G_port_mask);  
  IRQ_unmask(G_port_mask);

#ifdef USE_SERIAL_IRQS
  G_can_send_now = 0;

  MEM_zero(G_modem_status, sizeof(G_modem_status));
  MEM_zero(G_line_status, sizeof(G_line_status));
  MEM_zero(G_port_buffer, sizeof(G_port_buffer));
  MEM_zero(G_port_buffer_index, sizeof(G_port_buffer_index));
#endif

  DBG_print_char('\n');  
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
    bits |= SERIAL_LCR_PAR_ODD_BIT;
    break;
  case SERIAL_PARITY_EVEN:
    bits |= SERIAL_LCR_PAR_EVEN_BIT;
    break;
  case SERIAL_PARITY_MARK:
    bits |= SERIAL_LCR_PAR_MARK_BIT;
    break;
  case SERIAL_PARITY_SPACE:
    bits |= SERIAL_LCR_PAR_SPACE_BIT;
    break;
  }

  return bits;
}

int16_t SER_update_baud_divisor(serial_port_init_type* init)
{
  static u_longdiv_type ldt;
  static uint32_t di;
  uint16_t baud_div;
  int16_t error;
 
  error = 0;
  ldt.d = 115200;
  di = init->baud;
  long_64_udiv_32(&ldt, &di);
  if (ldt.r != 0) {
    DBG_print_string("\n  Baud rate is not supported, rounding up to nearest supported value.");
    ldt.q += 1;
    error = SERIAL_ERROR_BAD_BAUD_RATE;
  }  
  init->_baud_div = ldt.q;
  ldt.d = 115200;
  di = init->_baud_div;
  long_64_udiv_32(&ldt, &di);
  init->baud = ldt.q;   
  return error;
}

void SER_set_params(uint16_t base, serial_port_init_type* init) 
{  
  x86_outb(base + SERIAL_PORT_LCR, SERIAL_LCR_DLAB_BIT);
  x86_outb(base + SERIAL_PORT_BDR_LSB, init->_baud_div & 0xFF);
  x86_outb(base + SERIAL_PORT_BDR_MSB, (init->_baud_div >> 8) & 0xFF);
  x86_outb(base + SERIAL_PORT_LCR, SER_get_line_control_bits(init));  
}

void SER_modem_control(uint16_t base, uint8_t bits)
{
  x86_outb(base + SERIAL_PORT_MCR, bits);
}

void SER_irq_enable(uint16_t base, uint8_t mask)
{
  x86_outb(base + SERIAL_PORT_IER, mask);
}

void SER_fifo_control(uint16_t base, int16_t trigger_level, uint8_t clear_mask)
{
  uint16_t bits = 0;
  if (trigger_level >= 0)
    bits |= (((trigger_level & 3u) << 6u) | 1u);
  bits |= (clear_mask << 1u);   
  x86_outb(base + SERIAL_PORT_FCR, bits);
}

uint8_t SER_irq_status(uint16_t base)
{
  return x86_inb(base + SERIAL_PORT_IIR);
}

uint8_t SER_irq_status_event(uint8_t value)
{
  if ((value & 1u) == 1u) 
    return SERIAL_IRQ_STATUS_CLEAR;
  return (value >> 1u) & 7u;
}

uint8_t SER_irq_status_fifo(uint8_t value)
{
  return (value >> 7u) & 1u;
}

uint8_t SER_line_status(uint16_t base)
{
  return x86_inb(base + SERIAL_PORT_LSR);
}

uint8_t SER_modem_status(uint16_t base)
{
  return x86_inb(base + SERIAL_PORT_MSR);
}

int16_t SER_can_transmit_now(uint16_t base)
{
  return (int16_t)!!(SER_line_status(base) & SERIAL_LSR_THRE_BIT);
}

int16_t SER_can_receive_now(uint16_t base)
{
  return (int16_t)!!(SER_line_status(base) & SERIAL_LSR_DR_BIT);
}

void SER_transmit_byte(uint16_t base, uint8_t value)
{
  x86_outb(base + SERIAL_PORT_DR, value);
}

int16_t SER_wait_transmit_byte(uint16_t base, uint8_t byte)
{
  while (!SER_can_transmit_now(base));
  SER_transmit_byte(base, byte);
  return 0;
}

int16_t SER_try_transmit_byte(uint16_t base, uint8_t byte)
{
  if (!SER_can_transmit_now(base)) 
    return SERIAL_ERROR_TRANSMIT_FULL;
  SER_transmit_byte(base, byte);
  return 0;
}

uint8_t SER_receive_byte(uint16_t base)
{
  return x86_inb(base + SERIAL_PORT_DR);
}

#ifdef USE_SERIAL_IRQS
int16_t SER_wait_data_available(uint16_t port)
{
  uint16_t size, flags;
  flags = x86_flags16(); 
L_repeat:
  x86_cli();
  size = G_port_buffer_index[port];
  if (size == 0) {
    x86_sti();
    // Wait until interrupt snaps us out of HLT
    x86_hlt();
    // Try reading again
    goto L_repeat;
  }  
  x86_load_flags16(flags);
  return size;
}

int16_t SER_async_receive(uint8_t port, uint8_t* obuff, uint16_t size, uint8_t wait)
{
  uint16_t i, j, index, flags;
  const uint8_t* ibuff;
  int16_t error;

  if (wait) 
  {
    if ((error = SER_wait_data_available(port)) < 0)      
      return error;
  }

  flags = x86_flags16();
  x86_cli();

  index = G_port_buffer_index[port];
  ibuff = G_port_buffer[port];

  if (index > 0)
  {
    for (i = 0; i < size && i < index; i++)
      obuff[i] = ibuff[i];    

    if (i < index) 
    {
      j = index - i;
      MEM_copy(ibuff, ibuff + i, j);
      G_port_buffer_index[port] = j;
    }
  }  

  x86_load_flags16(flags);
  return i;  
}
#endif

int16_t SER_try_receive_byte(uint16_t base)
{
  if (!SER_can_receive_now(base)) 
    return SERIAL_ERROR_RECEIVE_EMPTY;
  return SER_receive_byte(base);
}

int16_t SER_wait_receive_byte(uint16_t base)
{
  while (!SER_can_receive_now(base));
  return SER_receive_byte(base);
}

 int16_t SER_loop_back_test(uint16_t base, uint8_t value)
{
  SER_wait_transmit_byte(base, value);  
  if (SER_wait_receive_byte(base) != value) 
  {
    DBG_print_string("\n  ERROR: Self test failed with value : ");
    DBG_print_hex8(value);    
    return SERIAL_ERROR_SELF_TEST;
  }
  return 0;
}

int16_t SER_wait_transmit_string(uint16_t base, const char* value)
{
  while (*value) {
    SER_wait_transmit_byte(base, *value);
    ++value;
  }
  return 0;
}

int16_t SER_perform_port_self_test(serial_port_init_type* init)
{
  static char const test[] = "\xFF\xF0\x0F\xCC\x33\xAA\x55";
  uint16_t i;

  SER_modem_control(init->_base, SERIAL_MODEM_CONTROL_REQUEST_TO_SEND_BIT
                                |SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_ONE_BIT
                                |SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_TWO_BIT
                                |SERIAL_MODEM_CONTROL_LOOP_BACK_MODE_BIT);  

  DBG_print_string("\n  Self test ............... :");
  for(i = 0; test[i]; ++i) {
    DBG_print_char(' ');
    DBG_print_hex8(test[i]);
    if (SER_loop_back_test(init->_base, test[i])) 
      return SERIAL_ERROR_SELF_TEST;
  }
  SER_modem_control(init->_base, SERIAL_MODEM_CONTROL_DATA_TERMINAL_READY_BIT
                                |SERIAL_MODEM_CONTROL_REQUEST_TO_SEND_BIT
                                |SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_ONE_BIT
                                |SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_TWO_BIT);
  return 0;
}

int16_t SER_check_params(serial_port_init_type* init)
{
  if (!init->_base) {    
    DBG_print_string("\n  Error, invalid serial base port : ");
    DBG_print_hex16(init->_base);
    return SERIAL_ERROR_BAD_PORT;
  }

  if (init->baud > 115200 || init->baud < 2) {
    DBG_print_string("\n  Error, invalid serial baud rate : ");
    DBG_print_hex16(init->baud);    
    return SERIAL_ERROR_BAD_BAUD_RATE;
  }

  if (init->data_bits > 8 || init->data_bits < 5) {
    DBG_print_string("\n  Error, invalid serial data bits : ");
    DBG_print_dec8(init->data_bits);
    return SERIAL_ERROR_BAD_DATA_BITS;
  }
   
  if (init->parity > SERIAL_PARITY_MARK) {
    DBG_print_string("\n  Error, invalid serial parity : ");
    DBG_print_dec8(init->parity);
    return SERIAL_ERROR_BAD_PARITY;
  }

  if (init->stop_bits > 2 || init->stop_bits < 1) {
    DBG_print_string("\n  Error, invalid serial stop bits : ");
    DBG_print_dec8(init->stop_bits);
    return SERIAL_ERROR_BAD_STOP_BITS;
  }  

  return 0;
}

void SER_display_debug_info(serial_port_init_type* init)
{
  DBG_print_string("\n* Initializing COM port ... : COM");  
  DBG_print_dec16(init->_port+1);
  DBG_print_string("\n  Base address ............ : ");
  DBG_print_hex16(init->_base);
  DBG_print_string("\n  Baud rate ............... : ");
  DBG_print_dec32(init->baud);
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
}

int16_t SER_init_port(uint16_t port, serial_port_init_type* init)
{
  
  uint16_t i;
  uint8_t value;
  int16_t error;

  init->_port = port;
  init->_irq = SER_get_port_irq(port);
  init->_base = SER_get_port_base(init->_port);
  SER_update_baud_divisor(init); 
  if (error = SER_check_params(init))
    return error;
#ifdef DEBUG
  SER_display_debug_info(init);
#endif

  SER_irq_enable(init->_base, SERIAL_IRQ_ENABLE_NONE);
  SER_set_params(init->_base, init);
  SER_fifo_control(init->_base, SERIAL_FIFO_TRIGGER_LEVEL_14BYTE, SERIAL_FIFO_CLEAR_ALL);

  SER_modem_control(init->_base, SERIAL_MODEM_CONTROL_DATA_TERMINAL_READY_BIT
                                |SERIAL_MODEM_CONTROL_REQUEST_TO_SEND_BIT
                                |SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_ONE_BIT
                                |SERIAL_MODEM_CONTROL_AUXILARY_OUTPUT_TWO_BIT); 
  if (error = SER_perform_port_self_test(init))
    return error;
  DBG_print_char('\n');
#ifdef USE_SERIAL_IRQS
// Try to enable IRQ?
  SER_irq_enable(init->_base, SERIAL_IRQ_ENABLE_NONE);
L_again:
  value = SER_irq_status(init->_base);
  if (SER_irq_status_fifo(value) != SERIAL_IRQ_STATUS_FIFO_ENABLED)
    DBG_print_string("\n  No FIFO or FIFO unusable ?");
  if (SER_irq_status_event(value) != SERIAL_IRQ_STATUS_CLEAR) {
    DBG_print_string("\n  Pending IRQ? value = ");
    DBG_print_hex8(value);
    goto L_again;
  }
  while(SER_can_receive_now(init->_base))
    SER_receive_byte(init->_base);  
  SER_irq_enable(init->_base, SERIAL_IRQ_ENABLE_RECEIVE_DATA_AVAILBLE_BIT
                             |SERIAL_IRQ_ENABLE_RECEIVE_LINE_STATUS_BIT
                             |SERIAL_IRQ_ENABLE_MODEM_STATUS_CHANGE_BIT);
  while(SER_can_receive_now(init->_base))
    SER_receive_byte(init->_base);
#endif

  return 0;
}

#ifdef USE_SERIAL_IRQS

int16_t SER_buffer_is_full(uint16_t  port)
{
  uint16_t flags, index;
  flags = x86_flags16();
  x86_cli();
  index = G_port_buffer_index[port];
  x86_load_flags16(flags);
  return index >= SERIAL_PORT_BUFFER_SIZE;
}

int16_t SER_push_byte(uint16_t port, uint8_t byte)
{
  uint16_t index = G_port_buffer_index[port];
  if (index >= SERIAL_PORT_BUFFER_SIZE)
    return SERIAL_ERROR_BUFFER_FULL;
  G_port_buffer[port][index] = byte;
  ++G_port_buffer_index[port];  
  return 0;
}

int16_t __cdecl SER_process_port_irq(uint8_t port)
{
  uint16_t base;
  uint8_t status, value;
  int16_t error;

  base = SER_get_port_base(port);
  while(base != 0)
  {
    status = SER_irq_status(base);
    DBG_print_string("\n  IRQ status = ");
    DBG_print_hex8(SER_irq_status_event(status));
    switch(SER_irq_status_event(status))
    {
    case SERIAL_IRQ_STATUS_CLEAR:
    case SERIAL_IRQ_STATUS_RESERVED_0X4:
    case SERIAL_IRQ_STATUS_RESERVED_0X5:
      return 0;
    case SERIAL_IRQ_STATUS_CHARACTER_TIMEOUT:
    case SERIAL_IRQ_STATUS_RECEIVE_DATA_AVAILABLE:
      while (SER_can_receive_now(base)) 
      {
        if (SER_buffer_is_full(port))
          return SERIAL_ERROR_BUFFER_FULL;
        value = SER_receive_byte(base);
        if (error = SER_push_byte(port, value))
          return error;
      }            
      break;      
    case SERIAL_IRQ_STATUS_MODEM_STATUS_CHANGE:
      G_modem_status[port] = SER_modem_status(base);
      break;
    case SERIAL_IRQ_STATUS_LINE_STATUS_CHANGE:  
      G_line_status[port] = SER_line_status(base);
      break;
    case SERIAL_IRQ_STATUS_TRANSMIT_HOLD_EMPTY:
      G_can_send_now |= (1 << port);
      return 0;
    default:
      break;
    }
  } 
  return 0;
}
#endif

uint16_t __cdecl SER_irq(int16_t irq_n)
{
  uint16_t base;
  uint8_t value;

  DBG_print_string("\n" __FUNCTION__ " => ");
  DBG_print_dec16(irq_n);

#ifdef USE_SERIAL_IRQS
  if (irq_n != 3 && irq_n != 4) 
    return IRQ_ACKNOWLEDGE;

  if (irq_n == 3)
  {
    SER_process_port_irq(SERIAL_PORT_COM1);
    SER_process_port_irq(SERIAL_PORT_COM3);
  }

  if (irq_n == 4)
  {
    SER_process_port_irq(SERIAL_PORT_COM1);
    SER_process_port_irq(SERIAL_PORT_COM3);
  }
#endif

  return IRQ_ACKNOWLEDGE;
}
