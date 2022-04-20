#include <lcom/lcf.h>
#include "kbc.h"
#include "utils.c"

uint8_t scancode[2];
static int hook_id;
int scan_code_size = 1;

//Interrupt Handler - shouldn't receive or return anything
//                  - passing of data should be made through global variables
 void (kbc_ih)(void){
  uint8_t *status_register = 0;
  util_sys_inb(KBC_STATUS,status_register);
  if ((status_register && BIT7) || (status_register && BIT6)){
    scan_code = 0;
  }
  else{
      util_sys_inb(KBC_OUTPUT_BUFFER,&scan_code);
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
  *size = scan_code_size;
}
