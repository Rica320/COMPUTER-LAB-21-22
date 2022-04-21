#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"
#include "utils.c"

extern int scancode_sz;
extern bool two_byte_scancode;
extern uint8_t scancode[];
extern uint32_t inb_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status, hook_id = 0;
  message msg;
  uint8_t kbc_bit_no = 1;
  bool r, esc_pressed = false;
  uint16_t irq_set = BIT(kbc_bit_no);
  kbc_subscribe_int(&hook_id);

  while(!esc_pressed){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih();
            if (scancode[scancode_sz -1] == ESC_BREAKCODE){
                esc_pressed = true;
            }
            if(!two_byte_scancode){
                kbd_print_scancode(!(scancode[scancode_sz-1] & BIT7),scancode_sz,scancode);
                scancode_sz = 1;
            }
          }
          break;
        default:
          break;
      }
    }
    else {}
  }

  kbc_unsubscribe_int(&hook_id);
  kbd_print_no_sysinb(inb_counter);
  return EXIT_SUCCESS;

}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
