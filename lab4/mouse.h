#ifndef _MOUSE_H
#define _MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"


int(mouse_subscribe_int)(uint8_t *bit_no);

int(mouse_unsubscribe_int)();

void(kbc_ih)();

int(mouse_poll)(uint8_t code[], uint8_t *size);

int(mouse_restore)();

int(kbc_read)(uint8_t *code);

#endif //_MOUSE_H
