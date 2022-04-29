#include "i8042.h"
#include "i8254.h"
#include "kbd.h"
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

extern uint8_t scancode;
extern unsigned count_inb;

int(kbd_test_scan)() {

  int ipc_status;
  message msg;
  uint8_t bit_no;

  kbd_subscribe_int(&bit_no);

  bool another_read = false;
  uint8_t codes[2];

  while (scancode != ESC_BREAKCODE) {
    // wait for any kind of message
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    // if there is a message from an i/o
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no)) {

        kbc_ih();

        if (!another_read) {
          codes[0] = scancode;

          if (scancode == MSB_2B_SCANCODE)
            another_read = true;
          else
            kbd_print_scancode(!(scancode & BIT(7)), 1, codes);
        }
        else {
          codes[1] = scancode;
          another_read = false;
          kbd_print_scancode(!(scancode & BIT(7)), 2, codes);
        }
      }
  }

  kbd_unsubscribe_int();
  kbd_print_no_sysinb(count_inb);
  return 0;
}

int(kbd_test_poll)() {

  uint8_t codes[2], size;

  while (codes[0] != ESC_BREAKCODE) {

    kbd_poll(codes, &size);

    kbd_print_scancode(!(codes[size - 1] & BIT(7)), size, codes);
  }

  kbd_restore();
  kbd_print_no_sysinb(count_inb);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  return 0;
}
