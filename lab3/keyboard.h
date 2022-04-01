#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

#include <lcom/lcf.h>

uint8_t scancode[2];
int scancode_sz;
bool two_byte_scancode;


int (keyboard_subscribe_kbc_interrupts)(uint8_t bit_no, int *hook_id);
int (unsubscribe_kbc_interrupt)(int *hook_id);
void (kbc_ih)(void);



int (kbd_read)(uint8_t * code);
int (kbd_poll)(uint8_t code[], uint8_t *size);
int (kbc_write_cmd)(uint8_t cmd, int port); // TODO 
int (kbd_restore)(); 


#endif
