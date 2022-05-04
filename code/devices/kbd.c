#include "kbd.h"

uint8_t scancode, stat;
int hook_id;


int(kbd_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no = KBD_IRQ;

  return (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0);
}

int(kbd_unsubscribe_int)() {
  return (sys_irqrmpolicy(&hook_id) != 0);
}

void(kbc_ih)() {

  while (true) {
    if (utils_sys_inb(REG_PORT, &stat) != 0)
      scancode = 0;
      
    if (OBF & stat) {
        if (utils_sys_inb(BUF_PORT, &scancode) != 0)
          scancode = 0;
        if ((stat & (SR_PARITY_ERROR | SR_TO_ERROR)) != 0)
          scancode = 0;
        break;
    }
  }
}

int(kbd_poll)(uint8_t code[], uint8_t *size) {
  uint8_t byte;

  kbc_read(&byte);
  code[0] = byte;
  *size = 1;

  if (byte == MSB_2B_SCANCODE) {
    kbc_read(&byte);
    code[1] = byte;
    *size = 2;
  }

  return EXIT_SUCCESS;
}

int(kbd_restore)() {

  uint8_t cmd;

  sys_outb(REG_PORT, KBC_READ_CMD);
  utils_sys_inb(BUF_PORT, &cmd);

  cmd |= (INT_MOUSE | INT_KBD);

  sys_outb(REG_PORT, KBC_WRITE_CMD);
  sys_outb(BUF_PORT, cmd);

  return EXIT_SUCCESS;
}

int(kbc_read)(uint8_t *code) {
  uint8_t st;

  while (1) {
    utils_sys_inb(REG_PORT, &st);

    if (st & OBF) {
      utils_sys_inb(BUF_PORT, code);
  
      return (st & (SR_PARITY_ERROR | SR_TO_ERROR));
 
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return EXIT_FAILURE;
}
