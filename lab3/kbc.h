#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

/* Variables */
uint8_t scancode[2];
static int hook_id;
int scancode_sz;

/* Masks */
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80


/* Registers */

#define KBC_STATUS 0x64         /** Status Register */
#define KBC_INPUT_BUFFER 0x60   /** Input Buffer Register */
#define KBC_OUTPUT_BUFFER 0x60  /** Output Buffer Register */

/* Scancodes and Breakcodes */

#define TWO_BYTE_SCANCODE 0xE0      /*First of Two Byte  Scancodes*/

#define ESC_BREAKCODE 0x81      /* Esc Key Break Code*/

void (kbc_ih)(void);
int kbc_subscribe_int(int *bit_no);
int kbc_unsubscribe_int();
void kbc_get_scan_code(unsigned char * scan_code, int* res);

#endif
