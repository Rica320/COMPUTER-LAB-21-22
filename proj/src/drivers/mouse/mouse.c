#include <lcom/lcf.h>

#include "mouse.h"

void(mouse_ih)(void) {
  kbc_ih();
}

packet_t mouse_data_to_packet(uint8_t *data) {
  struct packet pp;

  pp.bytes[0] = data[0];
  pp.bytes[1] = data[1];
  pp.bytes[2] = data[2];

  pp.rb = pp.bytes[0] & RB_MOUSE;
  pp.mb = pp.bytes[0] & MB_MOUSE;
  pp.lb = pp.bytes[0] & LB_MOUSE;

  pp.x_ov = pp.bytes[0] & X_OVERFLOW;
  pp.y_ov = pp.bytes[0] & Y_OVERFLOW;

  pp.delta_x = (data[0] & MSB_X_DELTA) ? pp.bytes[1] - 256 : pp.bytes[1];
  pp.delta_y = (data[0] & MSB_Y_DELTA) ? pp.bytes[2] - 256 : pp.bytes[2];

  return pp;
}

int mouse_disable_data_reporting() {
  return kbc_send_mouse_cmd(DIS_DATA_REP);
}

int _mouse_enable_data_reporting_() {
  return kbc_send_mouse_cmd(ENA_DATA_REP);
}

struct mouse_ev *mouse_get_event(struct packet *pp) {
  static struct mouse_ev event;
  NullSafety(pp);

  static bool lb_pressed = false, rb_pressed = false, mb_pressed = false;

  if ((lb_pressed ^ pp->lb) && !(lb_pressed)) {
    event.type = LB_PRESSED;
    lb_pressed = true;
  }
  else if ((pp->rb ^ rb_pressed) && !(rb_pressed)) {
    event.type = RB_PRESSED;
    rb_pressed = true;
  }
  else if ((pp->lb ^ lb_pressed) && (lb_pressed)) {
    event.type = LB_RELEASED;
    lb_pressed = false;
  }
  else if ((pp->rb ^ rb_pressed) && (rb_pressed)) {
    event.type = RB_RELEASED;
    rb_pressed = false;
  }
  else if (!mb_pressed && pp->mb) {
    mb_pressed = true;
    event.type = BUTTON_EV;
  }
  else if (mb_pressed && !pp->mb) {
    mb_pressed = false;
    event.type = BUTTON_EV;
  }
  else
    event.type = MOUSE_MOV;

  event.delta_x = pp->delta_x;
  event.delta_y = pp->delta_y;

  return &event;
}
