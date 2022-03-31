#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include <timer.c>

#include <stdbool.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t state;
  timer_get_conf(0, & state);
  timer_display_conf(timer,state, field);
  return 1;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  timer_set_frequency(timer, freq);
  return 0;
}

int(timer_test_int)(uint8_t time) {
  int ipc_status;
  message msg;
  int r;
  while (time > 0) { /* You may want to use a different condition */

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            timer_int_handler();
            if (counter % 60 == 0) {            //as it increments 60 times per second, if it is true, a second has passed
              timer_print_elapsed_time();       //called once per second
              time--;                           //an additional second has passed so time decreases in 1
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else {}
  }
  return 0;
}
