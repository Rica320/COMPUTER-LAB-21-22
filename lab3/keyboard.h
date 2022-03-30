#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

#include <lcom/lcf.h>

uint8_t scancode[2];
int scancode_sz;


int (keyboard_subscribe_kbc_interrupts)(uint8_t bit_no, int *hook_id);
int (unsubscribe_kbc_interrupt)(int *hook_id);
void (kbc_ih)(void);


#endif
