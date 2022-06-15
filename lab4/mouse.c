#include "mouse.h"

static int hook_id;
uint8_t scancode;

int(mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id = MOUSE_IRQ;
  sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  return 0;
}

int(mouse_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void(mouse_ih)() {
  while (1) {

    if (check_out_buf()) {
      if (util_sys_inb(BUF_PORT, &scancode))
        scancode = 0;
      break;
    }

    scancode = 0;
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

bool(check_in_buf)() {

  uint8_t stat;
  util_sys_inb(REG_PORT, &stat);

  return !(stat & IBF);
}

bool(check_out_buf)() {

  uint8_t stat;
  util_sys_inb(REG_PORT, &stat);

  if (stat & OBF)
    return !(stat & (SR_PARITY_ERROR | SR_TO_ERROR));

  return false;
}

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp) {

  for (int i = 0; i < 3; i++)
    if (check_in_buf()) {
      sys_outb(KBC_WRITE_CMD, cmd);
      util_sys_inb(BUF_PORT, resp);
    }

  return 1;
}

int(mouse_option)(uint8_t cmd) {
  uint8_t resp, error2 = 0;

  do {
    sys_outb(REG_PORT, WRITE_B_MOUSE);
    mouse_write_cmd(cmd, &resp);

    if (resp == ACK)
      return 0;
    else if (resp == NACK)
      error2++;
    else if (resp == ERROR)
      return 1;

  } while (error2 < 2);

  return 1;
}

void(makePack)(struct packet *pack) {

  uint8_t b0 = pack->bytes[0];

  pack->lb = b0 & LB;
  pack->mb = b0 & MB;
  pack->rb = b0 & RB;

  pack->x_ov = b0 & X_OVFL;
  pack->y_ov = b0 & Y_OVFL;

  // complemento para 2
  pack->delta_x = (b0 & MSB_X_DELTA) ? (pack->bytes[1] - 256) : pack->bytes[1];
  pack->delta_y = (b0 & MSB_Y_DELTA) ? (pack->bytes[2] - 256) : pack->bytes[2];
}

struct mouse_ev *mouse_get_event(struct packet *pp) {
  static struct mouse_ev event;

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
