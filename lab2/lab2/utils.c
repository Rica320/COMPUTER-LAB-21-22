#include <lcom/lcf.h>

#include <stdint.h>

//Returns the LSB (least significant bit) of a 2 byte integer
//val = 2 byte integer
//lsb = address of memory location to be updated with val's LSB
//Return 0 upon success and non-zero otherwise
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val);					//Converte para inteiro de 8 bits (considera apenas o byte menos significativo como se pretende)
  return 0;
}

//Returns the MSB (most significant bit) of a 2 byte integer
//val = 2 byte integer
//msb = address of memory location to be updated with val's LSB
//Return 0 upon success and non-zero otherwise
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  val >> 8;									//Anda com o valor 8 bits para a direita, tornando o bit mais significativo no menos significativo
  *msb = (uint8_t)(val);					//Converte num só byte
  return 0;
}

//Invokes sys_inb() system call but reads the value into a uint8_t variable.
//port = the input port that is to be read
//value =address of 8-bit variable to be update with the value read
//Return 0 upon success and non-zero otherwise
int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  sys_inb(port, & val);
  *value = (uint8_t)(val);
}
