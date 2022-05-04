#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "../macros/i8254.h"
#include "utils.h"

// sets a timer [0,2] freq (must be >19)
int(timer_set_frequency)(uint8_t timer, uint32_t freq);

// subscribes timer interrupts
int (timer_subscribe_int)(uint8_t *bit_no);

// unsbscribes timer interrupts
int (timer_unsubscribe_int)();

// increases the timer_counter
void (timer_int_handler)();

// gets timer config status
int(timer_get_conf)(uint8_t timer, uint8_t *st);


// prints timer config
int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);


int(timer_print_config)(uint8_t timer, enum timer_status_field field);


