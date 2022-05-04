#include "i8254.h"
#include <lcom/lab2.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {

  // ler o status do timer para ST
  uint8_t st;
  timer_get_conf(timer, &st);

  // chama a função para dar print do status do timer
  timer_display_conf(timer, st, field);

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  return timer_set_frequency(timer, freq);
}

extern unsigned counter;

int(timer_test_int)(uint8_t time) {

  int ipc_status;
  message msg;
  uint8_t bit_no;

  timer_subscribe_int(&bit_no);

  while (time) {

    // wait for any kind of message
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    // if there is a message from an i/o
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no)) {

        timer_int_handler();

        // 60hz => 1s
        if (counter % 60 == 0) {
          timer_print_elapsed_time();
          time--;
        }
        // process subscribed interrupt
      }
  }

  timer_unsubscribe_int();
  return 0;
}
