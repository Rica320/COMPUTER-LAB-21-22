#include "game.h"
#include <lcom/lcf.h>

void game_loop() {
  flush_screen();

  uint8_t kbc_bit_no = 1;
  int kbc_hook_id = 0, ipc_status;
  bool esc_pressed = false, r;
  uint16_t irq_set = BIT(kbc_bit_no);

  message msg;

  unsigned char scan[2];
  int scan_size;

  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, KBC_IRQ));

  while (!esc_pressed) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            if (!kbc_get_error()) {
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == ESC_BREAK_CODE) {
                  esc_pressed = true;
                }
              }
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
}
