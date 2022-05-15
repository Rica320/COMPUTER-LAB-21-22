#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

#include "handlers.h"
#include "i8254.h"


extern uint32_t n_interrupts; // TODO: DOES NOT BELONG HERE

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
  
  uint8_t st;

  CHECKCall(timer_get_conf( timer, &st));
  
  CHECKCall(timer_display_conf(timer,st,field));

  return EXIT_SUCCESS;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {

  CHECKCall(timer_set_frequency(timer, freq));
  return EXIT_SUCCESS;
}

int(timer_test_int)(uint8_t time) {
  int ipc_status;
  message msg;
  uint8_t timer_id = 0, r; // THE WAY WE IMPLEMENTED WE ALREADY KNOW THE TIMER ID
  uint16_t irq_set; // TODO: 16 bits ?
  CHECKCall(timer_subscribe_int(&timer_id)); 
  irq_set = BIT(timer_id);

  while (time) { /* You may want to use a different condition */
              /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            timer_int_handler();
            if (!(n_interrupts % TIMER_ASEC_FREQ)) { /* second elapsed */
              timer_print_elapsed_time(); // WHAT TODO WITH THE RETURN VALUE
              time--;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  CHECKCall(timer_unsubscribe_int());

  return EXIT_SUCCESS;
}
