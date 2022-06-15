#include <lcom/lcf.h>
#include <lcom/utils.h>

#include "keyboard.h"

int(kbd_poll)(uint8_t code[], uint8_t *size) {
  NullSafety(code);
  NullSafety(size);

  uint8_t byte;

  CHECKCall(kbc_read(&byte));
  code[0] = byte;
  *size = 1;

  if (byte == TWO_BYTE_CODE) {
    CHECKCall(kbc_read(&byte));
    code[1] = byte;
    *size = 2;
  }

  return EXIT_SUCCESS;
}

int(kbc_kbd_interface_cmd)() {
  uint8_t state;
  CHECKCall(kbc_issue_cmd(KBC_INTERFACE_TEST));
  CHECKCall(kbc_read(&state));

  return state;
}

int(kbc_enable_kbd_cmd)() {
  CHECKCall(kbc_issue_cmd(KBC_INTERFACE_ENABLE));

  return EXIT_SUCCESS;
}

int(kbc_disable_kbd_cmd)() {
  CHECKCall(kbc_issue_cmd(KBC_INTERFACE_DISABLE));

  return EXIT_SUCCESS;
}

int(kbd_restore)() {
  uint8_t cmd;

  CHECKCall(kbc_read_cmd(&cmd));

  cmd |= (INT_KBD | INT_MOUSE);

  CHECKCall(kbc_write_cmd(cmd));

  return EXIT_SUCCESS;
}
