#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "state_machine.h"

extern int (*state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

/* ===================== FUNCS TO IMPLEMENT ===================== */

struct packet packet;
extern uint8_t scancode;

int(mouse_test_packet)(uint32_t cnt) {

  int ipc_status;
  message msg;
  uint8_t bit_no;

  mouse_option(ENA_DATA_REP);
  mouse_subscribe_int(&bit_no);

  int packCnt = 0;

  while (cnt) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no)) {

        mouse_ih();
        packet.bytes[packCnt++] = scancode;

        if (packCnt == 3) {
          makePack(&packet);
          mouse_print_packet(&packet);
          packCnt = 0;
          cnt--;
        }
      }
  }

  mouse_unsubscribe_int();
  mouse_option(DIS_DATA_REP);
  return 0;
}

// ________________________________________________________________________________________

extern unsigned counter;

int(mouse_test_async)(uint8_t idle_time) {

  int ipc_status;
  message msg;
  uint8_t bit_no_mouse, bit_no_timer;

  mouse_option(ENA_DATA_REP);
  mouse_subscribe_int(&bit_no_mouse);
  timer_subscribe_int(&bit_no_timer);

  int packCnt = 0;
  uint8_t remainingTime = idle_time;

  while (remainingTime) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      if (msg.m_notify.interrupts & BIT(bit_no_mouse)) {

        mouse_ih();
        packet.bytes[packCnt++] = scancode;

        if (packCnt == 3) {
          makePack(&packet);
          mouse_print_packet(&packet);
          packCnt = 0;
        }
        remainingTime = idle_time;
        counter = 0;
      }

      if (msg.m_notify.interrupts & BIT(bit_no_timer)) {
        timer_int_handler();
        if (counter % 60 == 0)
          remainingTime--;
      }
    }
  }

  timer_unsubscribe_int();
  mouse_unsubscribe_int();
  mouse_option(DIS_DATA_REP);
  return 0;
}

// ________________________________________________________________________________________

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {

  uint8_t bit_no_mouse;
  int ipc_status;
  message msg;

  mouse_option(ENA_DATA_REP);
  mouse_subscribe_int(&bit_no_mouse);

  // Cenas para controlar o state
  enum state_codes cur_state = ENTRY_STATE;
  enum ret_codes rc;
  int (*state_fun)(struct mouse_ev * event, uint8_t x_len, uint8_t tolerance);
  struct mouse_ev *event;

  int packCnt = 0;
  while (1) {

    if ((driver_receive(ANY, &msg, &ipc_status))) {
      printf("driver_receive failed");
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no_mouse)) {

        mouse_ih();

        packet.bytes[packCnt++] = scancode;

        if (packCnt == 3) {
          packCnt = 0;

          makePack(&packet);
          mouse_print_packet(&packet);

          // STATE TRANSITION STUFF
          event = mouse_get_event(&packet);

          state_fun = state[cur_state];

          rc = state_fun(event, x_len, tolerance);

          cur_state = lookup_transitions(cur_state, rc);

          if (EXIT_STATE == cur_state)
            break;
        }
      }
  }

  // desativar mouse
  mouse_unsubscribe_int();
  mouse_option(DIS_DATA_REP);

  return 0;
}
