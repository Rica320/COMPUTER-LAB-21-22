#ifndef _KBD_H
#define _KBD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include "../macros/i8042.h"

int(kbd_subscribe_int)(uint8_t *bit_no);

int(kbd_unsubscribe_int)();

void(kbc_ih)();

int(kbd_poll)(uint8_t code[], uint8_t *size);

int(kbd_restore)();

int(kbc_read)(uint8_t *code);

#endif //_KBD_H
