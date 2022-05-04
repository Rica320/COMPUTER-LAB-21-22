#ifndef _UTILS_H
#define _UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>

int(utils_get_LSB)(uint16_t val, uint8_t *lsb);
int(utils_get_MSB)(uint16_t val, uint8_t *msb);
int(utils_sys_inb)(int port, uint8_t *value);

#endif //_UTILS_H

