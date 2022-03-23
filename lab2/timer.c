#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "return_handler.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  uint8_t st, lsb, msb;
  uint16_t init_timer_value = (uint16_t)(TIMER_FREQ / freq);

  timer_get_conf(timer, &st);
  
  uint8_t cmd = TIMER_SEL(timer) | TIMER_LSB_MSB | LSHUB_IN_BYTE(st);

  sys_outb(TIMER_CTRL, cmd); // TODO: AGAIN THE QUESTION OF MACROS TO DEAL WITH THE RETURN VALUE

  int port = TIMER_0 + timer;

  CHECKCall(util_get_LSB(init_timer_value, &lsb));
  
  CHECKCall(util_get_MSB(init_timer_value, &msb));


  sys_outb(port, lsb);
  sys_outb(port, msb);

  return 0;
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
