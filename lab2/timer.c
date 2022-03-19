#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  uint8_t wr = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  sys_outb(TIMER_CTRL, wr);

  util_sys_inb(TIMER_0 + timer, st);
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
                      
  union timer_status_field_val conf;

  /*
  
  7 -> out. 6 -> NULL COUNT 5,4 -> TYPE OF ACCESS 3,2,1 -> prog mode 0 -> BCD

  IN A UNION DATA TYPE SHARE SAME LOCATION : BE CAREFUL ... check i8254.h

  */
  switch (field)
  {
  case tsf_all:
      conf.byte = st;
      break;
  case tsf_initial:
      conf.in_mode = CONF_IN_MODE(st);
      break;
  case tsf_mode:
      conf.count_mode = CONF_COUNT_MODE(st);
      break;
  case tsf_base:
      conf.bcd = CONF_BCD_MODE(st);
      break;
  }

  timer_print_config(timer, field, conf); // TODO: MACROS FOR TREATING RETURN VALUES

  return 0;
}
