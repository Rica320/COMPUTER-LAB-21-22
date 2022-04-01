#include "utils.h"

uint8_t scancode, stat;
int hook_id, count_inb;

int(utils_sys_inb)(int port, uint8_t *value) {

  uint32_t c;

  if (sys_inb(port, &c) != 0)
    return 1;

  *value = (uint8_t) (c & 0x000000FF);

#ifdef LAB3
  count_inb++; // só precisamos disto no lab3 (o stor quer)
#endif

  return 0;
}

int(kbd_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no = KBD_IRQ;

  return (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0);
}

int(kbd_unsubscribe_int)() {
  return (sys_irqrmpolicy(&hook_id) != 0);
}

void(kbc_ih)() {
  /*
  read the status register and check if there was some communications error;
to read the scancode byte from the output buffer;
  */

  while (true) {
    if (utils_sys_inb(REG_PORT, &stat) != 0) // tenta ler do Status Register
      scancode = 0;                          // se falhar, n ha scancode

    if (OBF & stat) {
      // se o out buffer estiver cheio
      if (utils_sys_inb(BUF_PORT, &scancode) != 0)
        scancode = 0; // se falhar, n ha scancode

      if ((stat & (SR_PARITY_ERROR | SR_TO_ERROR)) != 0)
        scancode = 0; // se falhar, n ha scancode
      break;          // se ha erro, desiste
    }
  }
}

int(kbd_poll)(uint8_t code[], uint8_t *size) {
  uint8_t byte;

  kbc_read(&byte);
  code[0] = byte;
  *size = 1;

  //se for necessário um segundo byte
  if (byte == MSB_2B_SCANCODE) {
    kbc_read(&byte);
    code[1] = byte;
    *size = 2;
  }

  return EXIT_SUCCESS;
}

int(kbd_restore)() {

  // devolve o controlo ao minix do kbd com um cmd
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
