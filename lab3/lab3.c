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
extern bool two_byte_scancode;

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
  
  CHECKCall(keyboard_subscribe_kbc_interrupts(kbc_bit_no, &kbc_hook_id));

  while (!esc_pressed) { 

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) { 
            kbc_ih();
            if (!kbc_get_error()) {
              if (scancode[scancode_sz - 1] == ESC_BREAK_CODE) {
                esc_pressed = true;
              }
              if (!two_byte_scancode) {
                CHECKCall(kbd_print_scancode(!(scancode[scancode_sz - 1] & BREAK_CODE_BIT), scancode_sz, scancode));
                scancode_sz = 1; // TODO: BAD DESIGN
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

  CHECKCall(unsubscribe_kbc_interrupt(&kbc_hook_id));
  CHECKCall(kbd_print_no_sysinb(inb_counter));

  return EXIT_SUCCESS;
}

int(kbd_test_poll)() {

  uint8_t code[2] = { 0, 0 };
  uint8_t size = 0;

  while (code[0] != ESC_BREAK_CODE)
  {
    CHECKCall(kbd_poll(code, &size)); // DO I WANNA RETURN AFTER AN ERROR ???
    CHECKCall(kbd_print_scancode(!(code[size -1] & BREAK_CODE_BIT), size, code));
  }
  
  CHECKCall(kbd_restore());

  CHECKCall(kbd_print_no_sysinb(inb_counter));

  return EXIT_SUCCESS;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
