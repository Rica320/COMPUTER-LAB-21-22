#include "mouse.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

extern uint8_t scancode;
uint8_t bytes[3];
struct packet pack;

int(mouse_test_packet)(uint32_t cnt) {

  uint8_t bit_no_mouse;
  int ipc_status;
  message msg;

  // ativar mouse
  mouse_option(ENA_DATA_REP, 0);
  mouse_subscribe_int(&bit_no_mouse);

  int readTurn = 0;
  while (cnt) {

    if ((driver_receive(ANY, &msg, &ipc_status))) {
      printf("driver_receive failed");
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no_mouse)) {

        mouse_ih();

        bytes[readTurn++] = scancode; // ler os 3 bytes

        // quando lemos o terceiro byte, fazer print do pack e resetar contagem
        if (readTurn == 3) {
          readTurn = 0;

          for (unsigned i = 0; i < 3; i++)
            pack.bytes[i] = bytes[i];

          makePack(&pack);
          mouse_print_packet(&pack);
          cnt--;
        }
      }
  }

  // desativar mouse
  mouse_unsubscribe_int();
  mouse_option(DIS_DATA_REP, 0);
  return 0;
}

int counter = 0; // should include the timer.... and make it extern

int(mouse_test_async)(uint8_t idle_time) {

  int ipc_status;
  message msg;

  uint8_t bit_no_timer, bit_no_mouse;

  mouse_option(ENA_DATA_REP, 0);

  mouse_subscribe_int(&bit_no_mouse);
  timer_subscribe_int(&bit_no_timer);

  uint8_t idletime = idle_time;

  int readTurn = 0;
  while (idletime) {

    if ((driver_receive(ANY, &msg, &ipc_status))) {
      printf("driver_receive failed");
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      if (msg.m_notify.interrupts & BIT(bit_no_mouse)) {

        mouse_ih();

        bytes[readTurn++] = scancode; // ler os 3 bytes

        // quando lemos o terceiro byte, fazer print do pack e resetar contagem
        if (readTurn == 3) {
          readTurn = 0;

          for (unsigned i = 0; i < 3; i++)
            pack.bytes[i] = bytes[i];

          makePack(&pack);
          mouse_print_packet(&pack);
        }

        idletime = idle_time;
        counter = 0;
        continue;
      }

      if (msg.m_notify.interrupts & BIT(bit_no_timer)) {
        timer_int_handler();
        if (counter % 60 == 0)
          idletime--;
      }
    }
  }

  timer_unsubscribe_int();
  mouse_unsubscribe_int();

  return (mouse_option(DIS_DATA_REP, 0));
}
