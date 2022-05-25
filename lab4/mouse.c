#include "mouse.h"

int(mouse_subscribe_int)(uint8_t *bit_no) {
  return 0;
}

int(mouse_unsubscribe_int)() {
  return 0;
}

void(mouse_ih)() {
}

bool(check_in_buf)() {
  return 0;
}

bool(check_out_buf)() {
  return 0;
}

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp) {
  return 0;
}

void(makePack)(struct packet *pack) {
}

struct mouse_ev *mouse_get_event(struct packet *pp) {
  static struct mouse_ev event;

  return &event;
}
