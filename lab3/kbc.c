#include <lcom/lcf.h>
#include "kbc.h"
#include "utils.c"

static int hook_id;
int scancode_sz = 1;
bool two_byte_scancode = false;

//Interrupt Handler - shouldn't receive or return anything
//                  - passing of data should be made through global variables
 void (kbc_ih)(void){
  uint8_t *status_register = 0;
  util_sys_inb(KBC_STATUS,status_register);
  uint8_t scan_code = 0;
  if ((status_register && BIT7) || (status_register && BIT6) || !(status_register && BIT0)){
    return;
  }
  else{
      util_sys_inb(KBC_OUTPUT_BUFFER,&scan_code);
      scancode[scancode_sz-1] = scan_code;
      two_byte_scancode = scan_code == TWO_BYTE_SCANCODE;
      if(two_byte_scancode) scancode_sz = 2;  //se o scan_code for o primeiro de dois bytes, o tamanho passa a 2
  }
 }

int kbc_subscribe_int(uint8_t *bit_no){
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
