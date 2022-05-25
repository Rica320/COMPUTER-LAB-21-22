#include "mouse.h"
#include "state_machine.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

extern int (*state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

/* ===================== FUNCS TO IMPLEMENT ===================== */

int(mouse_test_packet)(uint32_t cnt) {
  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  return 0;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  return 0;
}
