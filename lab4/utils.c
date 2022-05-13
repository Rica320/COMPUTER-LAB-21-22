#include "utils.h"

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) (0X00ff & val);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8);
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t c;
  if (sys_inb(port, &c))
    return 1;
  *value = (uint8_t) c;
  return 0;
}
