#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <i8042.h>
#include <keyboard.h>
#include <handlers.h>

extern uint8_t scancode[];
extern int scancode_sz;
extern uint32_t inb_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

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
  uint8_t kbc_bit_no = 1;
  int kbc_hook_id = 0, ipc_status;
  bool esc_pressed = false, r;
  uint16_t irq_set = BIT(kbc_bit_no);
  message msg;
  bool two_byte_scancode = false;
  
  CHECKCall(keyboard_subscribe_kbc_interrupts(kbc_bit_no, &kbc_hook_id));

  while (!esc_pressed) { /* You may want to use a different condition */
              /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih();
            if (scancode[scancode_sz -1] == ESC_BREAK_CODE)
              esc_pressed = true;
            if (two_byte_scancode) {
              two_byte_scancode = false;
              kbd_print_scancode(!(scancode[scancode_sz -1] & BREAK_CODE_BIT), 2, scancode);
              scancode_sz = 1;
            }
            else {
              if (scancode[scancode_sz -1] & TWO_BYTE_CODE) {
                two_byte_scancode = true;
                scancode_sz = 2;
              }
              else
                kbd_print_scancode(!(scancode[scancode_sz -1] & BREAK_CODE_BIT), 1, scancode);
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

  CHECKCall(unsubscribe_kbc_interrupt(&kbc_hook_id));
  CHECKCall(kbd_print_no_sysinb(inb_counter));

  return 1;
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
