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

extern int counter;
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

#include "state_machine.h"
extern int (*state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {

  uint8_t bit_no_mouse;
  int ipc_status;
  message msg;

  // ativar mouse
  mouse_option(ENA_DATA_REP, 0);
  mouse_subscribe_int(&bit_no_mouse);

  // Cenas para controlar o state
  enum state_codes cur_state = ENTRY_STATE;
  enum ret_codes rc;
  int (*state_fun)(struct mouse_ev * event, uint8_t x_len, uint8_t tolerance);
  struct mouse_ev *event;

  int readTurn = 0;
  while (1) {

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

          // STATE TRANSITION STUFF

          event = mouse_get_event(&pack);

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
  mouse_option(DIS_DATA_REP, 0);

  return 0;
}

/*

  while (!ended) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
            if (!kbc_get_error()) { // TODO: MAKE THE FUNC RETURN THE TYPES THAT WE READ
              if (kbc_mouse_ready()) {
                kbc_get_mouse_data(scan);

                struct packet pp = mouse_data_to_packet(scan);
                mouse_print_packet(&pp);

                event = mouse_get_event(&pp);

                state_fun = state[cur_state];
                rc = state_fun(event, x_len, tolerance);

                cur_state = lookup_transitions(cur_state, rc);
                if (EXIT_STATE == cur_state)
                  ended = true;
              }
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }
*/
