#include <lcom/lcf.h>
#include "kbc.h"

static int hook_id;
int scancode_sz = 1;
bool two_byte_scancode = false;

//Interrupt Handler - shouldn't receive or return anything
//                  - passing of data should be made through global variables
 void (kbc_ih)(void){
  uint8_t *st = 0;  //st --> status register
  util_sys_inb(KBC_STATUS, st);
  if ((st && BIT7) || (st && BIT6) || !(st && BIT0)){
    return;
  }
  else{
    uint8_t byte = 0;
    util_sys_inb(KBC_OUTPUT_BUFFER,&byte);
    scancode[scancode_sz-1] = byte;
    two_byte_scancode = byte == TWO_BYTE_SCANCODE;
    if(two_byte_scancode) scancode_sz = 2;  //se o scan_code for o primeiro de dois bytes, o tamanho passa a 2
  }
 }

int kbc_subscribe_int(int *bit_no){
  hook_id = *bit_no;
  sys_irqsetpolicy(KEYBOARD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook_id);
  return 0;
}

int kbc_unsubscribe_int(){
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void kbc_get_scan_code(unsigned char * scan, int* size){
  for(int i = 0; i < *size; i++){
    scan[i] = scancode[i];
  }
  *size = scancode_sz;
}
