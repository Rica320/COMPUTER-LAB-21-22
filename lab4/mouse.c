#include <lcom/lcf.h>

#include <mouse.h>
#include <handlers.h>
#include <i8042.h>
#include <kbc.h>

void (mouse_ih)(void) {
  kbc_ih();
}

packet_t mouse_data_to_packet(uint8_t* data) {
  struct packet pp;
  pp.bytes[0] = data[0];
  pp.bytes[1] = data[1];
  pp.bytes[2] = data[2];
  pp.rb = pp.bytes[0] & RB_MOUSE;
  pp.mb = pp.bytes[0] & MB_MOUSE;
  pp.lb = pp.bytes[0] & LB_MOUSE;
  pp.delta_x  = (data[0] & MSB_X_DELTA)  ?  pp.bytes[1] - 256 : pp.bytes[1];
  pp.delta_y  = (data[0] & MSB_Y_DELTA)  ?  pp.bytes[2] - 256 : pp.bytes[2];
  pp.x_ov = pp.bytes[0] & X_OVERFLOW;
  pp.y_ov = pp.bytes[0] & Y_OVERFLOW;
  return pp;
}

int mouse_disable_data_reporting() {

  return kbc_send_mouse_cmd(DIS_DATA_REP);
}

int _mouse_enable_data_reporting_() {
  return kbc_send_mouse_cmd(ENA_DATA_REP);
}
