#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "handlers.h"

uint32_t n_interrupts = 0;
int hook_id;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  Assert_cbet(freq, TIMER_BB_FREQ, TIMER_FREQ);

  uint8_t st, lsb, msb;
  uint16_t init_timer_value = (uint16_t)(TIMER_FREQ / freq);

  CHECKCall(timer_get_conf(timer, &st));
  
  uint8_t cmd = TIMER_SEL(timer) | TIMER_LSB_MSB | LSHUB_IN_BYTE(st);

  CHECKCall(sys_outb(TIMER_CTRL, cmd));

  int port = TIMER_0 + timer;

  CHECKCall(util_get_LSB(init_timer_value, &lsb));
  
  CHECKCall(util_get_MSB(init_timer_value, &msb));

  CHECKCall(sys_outb(port, lsb));
  CHECKCall(sys_outb(port, msb));

  return EXIT_SUCCESS; // CHECKCall checks if any of the function ended on failure, exiting the prog. if so
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  NullSafety(bit_no);
  hook_id = *bit_no = TIMER0_HOOK_ID;
  CHECKCall(sys_irqsetpolicy( TIMER0_IRQ ,IRQ_REENABLE, &hook_id));
  return EXIT_SUCCESS;
}

int (timer_unsubscribe_int)() {

  NullSafety(&hook_id);
  CHECKCall(sys_irqrmpolicy(&hook_id));

  return EXIT_SUCCESS;
}

void (timer_int_handler)() {
  n_interrupts++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  Assert_cbet(timer, FIRST_TIMER, THIRD_TIMER); // lcf already does this ... but who knows ?! better safe then sorry
  NullSafety(st);

  uint8_t wr = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  CHECKCall(sys_outb(TIMER_CTRL, wr));

  CHECKCall(util_sys_inb(TIMER_0 + timer, st));
  
  return EXIT_SUCCESS;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
                      
  Assert_cbet(timer, FIRST_TIMER, THIRD_TIMER);
  
  union timer_status_field_val conf;

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

  CHECKCall(timer_print_config(timer, field, conf)); 

  return EXIT_SUCCESS;
}

