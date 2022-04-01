#include <lcom/lcf.h>
#include <lcom/lab2.h>

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
  /* To be implemented by the students */
  uint8_t st;

  if(timer_get_conf(timer, &st)==0)
    if(timer_display_conf(timer, st, field)==0) return 0;
  printf("Error in function timer_test_read_config.\n");
  return 1;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
 if (timer_set_frequency(timer, freq) == 1) {
    printf("Error setting frequency.\n");
    return 1;
  }
  return 0;
}

int(timer_test_int)(uint8_t time) {
  /* To be implemented by the students */
  
  uint8_t bit_no;
  int ipc_status, r;
  message msg;
  count = 0;

  if (timer_subscribe_int(&bit_no)!=0) {
      printf("Error subscribing timer interrupts.\n");
      return 1;
  }

  while (count/sys_hz() < time) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(bit_no)) { /* subscribed interrupt*/
            timer_int_handler();
            if( (count%sys_hz()) == 0 ) {
              timer_print_elapsed_time();
            }
          }
          break;
        
        default:
          break;
      }
    }
  }

  if (timer_unsubscribe_int()!=0) {
    printf("Error unsubscribing timer interrupts\n");
    return 1;
  }

  return 0;
}
