#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "rtc.h"

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();

  return 0;
}

//________________________________________________________________________

extern uint8_t rtc_data[6];

int(proj_main_loop)(int argc, char *argv[]) {

  uint8_t bit_no;
  int ipc_status;
  message msg;

  rtc_subscribe_int(&bit_no);

  int i = 10;
  while (i) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    // rtc tem int a cada segundo
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no)) {

        rtc_ih();

        // print rtc info
        printf("\n%d:%d:%d\n", rtc_data[2], rtc_data[1], rtc_data[0]);
        printf("%d/%d/20%d\n", rtc_data[3], rtc_data[4], rtc_data[5]);

        i--;
      }
  }

  printf("Hello World!\n\n");

  rtc_unsubscribe_int();
  return 0;
}
