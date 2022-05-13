#include "mouse.h"

int hook_id;
uint8_t scancode;

// ______________________________________________________

int(mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id = MOUSE_IRQ;
  sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  return 0;
}

int(mouse_unsubscribe_int)() {

  sys_irqrmpolicy(&hook_id);

  uint8_t temp, stat;

  // se sobrou conteudo que nao vamos usar => limpar
  util_sys_inb(REG_PORT, &stat);

  if (stat & OBF)
    util_sys_inb(BUF_PORT, &temp);
  return 0;
}

void(mouse_ih)() {
  while (1) {
    if (check_out_buf()) {
      if (util_sys_inb(BUF_PORT, &scancode))
        scancode = 0x00;
      break;
    }
    scancode = 0x00;
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

// ______________________________________________________

bool(check_in_buf)() {
  uint8_t stat;
  util_sys_inb(REG_PORT, &stat);
  
  return !(stat & IBF);
}

bool(check_out_buf)() {
  uint8_t stat;
  util_sys_inb(REG_PORT, &stat);

  if (stat & OBF)
    return !((stat & (SR_PARITY_ERROR | SR_TO_ERROR)));
  return false;
}

// ______________________________________________________

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp) {
  // Tenta mandar a info 3x e dps desiste
  for (int i = 0; i < 3; i++)
    if (check_in_buf()) {
      sys_outb(KBC_WRITE_CMD, cmd);
      util_sys_inb(BUF_PORT, resp);
      return 0;
    }
  return 1;
}

void(makePack)(struct packet *pack) {
  uint8_t b0 = pack->bytes[0];

  pack->lb = b0 & LB;
  pack->rb = b0 & RB;
  pack->mb = b0 & MB;
  pack->x_ov = b0 & X_OVFL;
  pack->y_ov = b0 & Y_OVFL;

  // remember they are in complement for 2!
  pack->delta_x = (b0 & MSB_X_DELTA) ? (pack->bytes[1] - 256) : pack->bytes[1];
  pack->delta_y = (b0 & MSB_Y_DELTA) ? (pack->bytes[2] - 256) : pack->bytes[2];
}

/*
OPTIONS:
    mouse_read_data - mouse_option(READ_DATA, 1)
    mouse_remote - mouse_option(REMOTE_MODE, 0)
    mouse_stream - mouse_option(STREAM_MODE, 0)
    mouse_disable_data_report - mouse_option(DIS_DATA_REP, 0)
    mouse_enable_data_report - mouse_option(ENA_DATA_REP, 0)
*/
int(mouse_option)(uint8_t cmd, bool use_ih) {
  uint8_t resp, error2 = 0;

  do {
    sys_outb(REG_PORT, WRITE_B_MOUSE);
    mouse_write_cmd(cmd, &resp);

    if (resp == ACK) {
      if (use_ih)
        mouse_ih();
      return 0;
    }
    else if (resp == NACK)
      error2++;
    else if (resp == ERROR)
      return 1;

  } while (error2 < 2);

  return 1;
}
