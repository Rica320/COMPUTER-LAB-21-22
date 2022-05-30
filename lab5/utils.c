#include <lcom/lcf.h>

#include <stdint.h>

//Returns the LSB (least significant bit) of a 2 byte integer
//val = 2 byte integer
//lsb = address of memory location to be updated with val's LSB
//Return 0 upon success and non-zero otherwise
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val);
  return 0;
}

//Returns the MSB (most significant bit) of a 2 byte integer
//val = 2 byte integer
//msb = address of memory location to be updated with val's LSB
//Return 0 upon success and non-zero otherwise
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val >> 8);
  return 0;
}

#ifdef LAB3

uint32_t inb_counter = 0;

#endif

//Invokes sys_inb() system call but reads the value into a uint8_t variable.
//port = the input port that is to be read
//value =address of 8-bit variable to be update with the value read
//Return 0 upon success and non-zero otherwise
int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t a;

  #ifdef LAB3
  ++inb_counter;
  #endif

  sys_inb(port, & a);
  *value = (uint8_t) a;
  return 0;
}
