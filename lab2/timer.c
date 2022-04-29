#include "i8254.h"
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

/*
enum timer_status_field {
  tsf_all,	// Display status byte, in hexadecimal
  tsf_initial,	// Display the initialization mode, only
  tsf_mode,	// Display the counting mode, only
  tsf_base	// Display the counting base, only
};

union timer_status_field_val {
  uint8_t byte;			// The status byte
  enum timer_init in_mode;	// The initialization mode
  uint8_t count_mode;		// The counting mode: 0, 1,.., 5
  bool bcd;			// The counting base, true if BCD
};

enum timer_init{
  INVAL_val, 		// Invalid initialization mode
  LSB_only,		// Initialization only of the LSB
  MSB_only,		// Initialization only of the MSB
  MSB_after_LSB		// Initialization of LSB and MSB, in this order
};
*/

/*    FUNCTIONS FOR 1    */

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t cmd = 0;
  cmd |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  sys_outb(TIMER_CTRL, cmd);

  util_sys_inb(TIMER_0 + timer, st);

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val val;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;

    case tsf_initial:
      val.in_mode = CONF_INIT_MODE(st);
      break;

    case tsf_mode:
      val.count_mode = CONF_COUNT_MODE(st);
      if (val.count_mode > 5)
        val.count_mode &= ~BIT(2);
      break;

    case tsf_base:
      val.bcd = st & BIT(0);
      break;

    default:
      break;
  }

  timer_print_config(timer, field, val);

  return 0;
}

/*    FUNCTIONS FOR 2    */

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (freq < 20)
    return 1;

  uint16_t rel_freq = (uint16_t) (TIMER_FREQ / freq);

  uint8_t lsb, msb;
  util_get_LSB(rel_freq, &lsb);
  util_get_MSB(rel_freq, &msb);

  uint8_t st;
  timer_get_conf(timer, &st);

  st = (timer << 6) | TIMER_LSB_MSB | (st & 0x0F);

  sys_outb(TIMER_CTRL, st);

  sys_outb(TIMER_0 + timer, lsb);
  sys_outb(TIMER_0 + timer, msb);

  return 0;
}

/*    FUNCTIONS FOR 3    */

unsigned counter = 0;
static int hook_id;

int(timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no = TIMER0_IRQ;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
  return 0;
}

int(timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void(timer_int_handler)() {
  counter++;
}
