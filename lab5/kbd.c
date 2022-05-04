#include "kbd.h"

unsigned count_inb = 0;

uint8_t scancode;

static int hook_id;

/*    FUNCTIONS FOR 1    */

int(kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id = KBD_IRQ;
  sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  return 0;
}

int(kbd_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void(kbc_ih)() {

  uint8_t st;

  while (1) {
    if (util_sys_inb(REG_PORT, &st))
      scancode = 0;

    if (st & OBF) {

      if (util_sys_inb(BUF_PORT, &scancode))
        scancode = 0;

      if (st & (SR_PARITY_ERROR | SR_TO_ERROR))
        scancode = 0;

      break;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

/*    FUNCTIONS FOR 2    */

int(kbd_poll)(uint8_t code[], uint8_t *size) {

  uint8_t byte;
  if (kbc_read(&byte))
    return 1;

  code[0] = byte;
  *size = 1;

  if (byte == MSB_2B_SCANCODE) {

    if (kbc_read(&byte))
      return 1;

    code[1] = byte;
    *size = 2;
  }

  return 0;
}

int(kbd_restore)() {

  sys_outb(REG_PORT, KBC_READ_CMD);

  uint8_t cmd;

  util_sys_inb(BUF_PORT, &cmd);

  cmd |= (INT_MOUSE | INT_KBD);

  sys_outb(REG_PORT, KBC_WRITE_CMD);

  sys_outb(BUF_PORT, cmd);

  return 0;
}

int(kbc_read)(uint8_t *code) {

  uint8_t st;

  while (1) {
    util_sys_inb(REG_PORT, &st);

    if (st & OBF) {

      if (util_sys_inb(BUF_PORT, code))
        scancode = 0;

      return (st & (SR_PARITY_ERROR | SR_TO_ERROR));
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}
