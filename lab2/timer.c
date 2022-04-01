#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
unsigned int count = 0;
int hook_id = 2;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
   uint8_t st;

  if (timer_get_conf(timer, &st) != 0) {
    printf("Error getting configuration in timer_set_frequency\n");
    return 1;
  }

  uint8_t ctrlw;
  uint8_t lsb, msb;

  uint16_t hrtz = TIMER_FREQ / freq;

  util_get_LSB(hrtz , &lsb);
  util_get_MSB(hrtz , &msb);

  switch (timer) {
    case 0:
      ctrlw = ( TIMER_SEL0 | TIMER_LSB_MSB | (st & 0x0F));
      sys_outb(TIMER_CTRL, ctrlw);
      sys_outb(TIMER_0, lsb);
      sys_outb(TIMER_0, msb);
      break;
    
    case 1:
      ctrlw = ( TIMER_SEL1 | TIMER_LSB_MSB | (st & 0x0F));
      sys_outb(TIMER_CTRL, ctrlw);
      sys_outb(TIMER_1, lsb);
      sys_outb(TIMER_1, msb);
      break;

    case 2:
      ctrlw = ( TIMER_SEL2 | TIMER_LSB_MSB | (st & 0x0F));
      sys_outb(TIMER_CTRL, ctrlw);
      sys_outb(TIMER_2, lsb);
      sys_outb(TIMER_2, msb);
      break;

    default:
      printf("Error selecting Timer\n");
      return 1;
  }

  return 0;
}


int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  *bit_no = hook_id;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)!=0){
    printf("Error subscribing int\n");
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
 if(sys_irqrmpolicy(&hook_id)!=0){
    printf("Error unsubscribing int\n");
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
