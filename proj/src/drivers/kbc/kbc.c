#include <lcom/lcf.h>
#include <lcom/utils.h>

#include "kbc.h"

int scancode_sz = 1;
bool two_byte_scancode = false;
uint8_t mouse_received_count = 0;
bool mouse_packet_completed = false;
bool error_flag = false;

int(subscribe_kbc_interrupt)(uint8_t bit_no, int *hook_id, int kbc_irq) {
  NullSafety(&bit_no);
  NullSafety(hook_id);
  *hook_id = bit_no;
  CHECKCall(sys_irqsetpolicy(kbc_irq, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_id));
  return EXIT_SUCCESS;
}

int(unsubscribe_interrupt)(int *hook_id) {
  NullSafety(hook_id);
  CHECKCall(sys_irqrmpolicy(hook_id));
  return EXIT_SUCCESS;
}

int(kbc_read_i)() {

  uint8_t st;

  mouse_packet_completed = false;

  CHECKCall(util_sys_inb(KBC_ST_REG, &st));

  if (st & (PARITY_ERROR | TIMEOUT_ERROR))
    return EXIT_FAILURE;

  if (!(st & OUT_BUF_FULL))
    return EXIT_FAILURE;

  uint8_t byte = 0;

  CHECKCall(util_sys_inb(OUT_BUF, &byte));

  if (st & AUX_MOUSE) {
    if (mouse_received_count == 0 && !(byte & BIT(3)))
      return EXIT_FAILURE;

    mouse_data[mouse_received_count] = byte;
    mouse_received_count++;
    if (mouse_received_count == 3) {
      mouse_received_count = 0;
      mouse_packet_completed = true;
    }
    return EXIT_SUCCESS;
  }

  if (two_byte_scancode)
    scancode_sz = 2;
  else
    scancode_sz = 1;

  scancode[scancode_sz - 1] = byte;
  two_byte_scancode = byte == TWO_BYTE_CODE;

  return EXIT_SUCCESS;
}

void(kbc_ih)(void) {
  if (kbc_read_i())
    error_flag = true;

  else
    error_flag = false;
}

bool(kbc_ready)() {
  return !two_byte_scancode;
}

void(kbc_get_scancode)(unsigned char *scan, int *scan_sz) {
  for (int i = 0; i < scancode_sz; i++)
    scan[i] = scancode[i];
  *scan_sz = scancode_sz;
}

void(kbc_get_mouse_data)(unsigned char *scan) {
  for (int i = 0; i < 3; i++)
    scan[i] = mouse_data[i];
}

bool(kbc_get_error)() {
  return error_flag;
}

int(kbc_read)(uint8_t *code) {
  uint8_t st;

  for (int i = 0; i < 3; ++i) {

    CHECKCall(util_sys_inb(KBC_ST_REG, &st));

    if (st & OUT_BUF_FULL) {

      CHECKCall(util_sys_inb(OUT_BUF, code));

      if (st & (PARITY_ERROR | TIMEOUT_ERROR))
        return EXIT_FAILURE;
      else
        return EXIT_SUCCESS;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return EXIT_FAILURE;
}

int(kbc_issue_cmd)(uint8_t cmd) {

  uint8_t stat;

  while (1) {
    CHECKCall(util_sys_inb(KBC_ST_REG, &stat));
    if (!(stat & IN_BUF_FULL)) {
      CHECKCall(sys_outb(KBC_CMD_REG, cmd));
      return EXIT_SUCCESS;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return EXIT_FAILURE;
}

int(kbc_read_cmd)(uint8_t *cmd) {
  CHECKCall(kbc_issue_cmd(KBC_READ_CMD));
  CHECKCall(kbc_read(cmd));
  return EXIT_SUCCESS;
}

int(kbc_write_cmd)(uint8_t cmd) {
  CHECKCall(kbc_issue_cmd(KBC_WRITE_CMD));
  CHECKCall(sys_outb(OUT_BUF, cmd));
  return EXIT_SUCCESS;
}

int(kbc_check_cmd)() {
  uint8_t state;
  CHECKCall(kbc_issue_cmd(KBC_CHECK));
  CHECKCall(kbc_read(&state));

  if (state == KBC_CHECK_ERROR) {
    printf("KeyBoard Controller is not ok\n");
    exit(1);
  }

  return EXIT_SUCCESS;
}

bool(kbc_mouse_ready)() {
  return mouse_packet_completed;
}

int(kbc_cmd_arg)(uint8_t arg) {
  uint8_t stat;

  while (1) {
    CHECKCall(util_sys_inb(KBC_ST_REG, &stat));
    if (!(stat & IN_BUF_FULL)) {
      CHECKCall(sys_outb(KBC_CMD_ARG, arg));
      return EXIT_SUCCESS;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return EXIT_SUCCESS;
}

int(kbc_send_mouse_cmd)(uint8_t cmd) {
  uint8_t resp;
  uint8_t numTries = 0;

  do {

    CHECKCall(kbc_issue_cmd(WRITE_BYTE_MOUSE));
    CHECKCall(kbc_cmd_arg(cmd));
    CHECKCall(util_sys_inb(OUT_BUF, &resp));

    if (resp == ACK)
      return EXIT_SUCCESS;
    else if (resp == NACK)
      numTries++;
    else if (resp == KBC_CHECK_ERROR)
      return EXIT_FAILURE;

  } while (numTries < 2);

  return EXIT_FAILURE;
}
