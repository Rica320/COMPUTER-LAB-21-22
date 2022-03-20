#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  

    *lsb = val;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

    *msb = (val >> 8);


  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t p;

  int st = sys_inb(port, &p);

  *value = (uint8_t)p;

  return st;
}
