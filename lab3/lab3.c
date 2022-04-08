#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <i8042.h>
#include <i8254.h>
#include <keyboard.h>
#include <handlers.h>

extern uint32_t inb_counter;
extern bool two_byte_scancode;
extern uint32_t n_interrupts;

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
  
  unsigned char scan[2];
  int scan_size;

  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id));

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
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == ESC_BREAK_CODE) {
                  esc_pressed = true;
                }

                CHECKCall(kbd_print_scancode(!(scan[scan_size - 1] & BREAK_CODE_BIT), scan_size, scan));
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

  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
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

int (subscribe_interrupt)(uint8_t bit_no, int *hook_id, int irq_line) { // TODO: THIS SHOULD BE IN TIMER.H
  NullSafety(&bit_no);
  NullSafety(hook_id);
  *hook_id = bit_no;
  CHECKCall(sys_irqsetpolicy(irq_line, IRQ_REENABLE, hook_id));
  return EXIT_SUCCESS;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t kbc_bit_no = 1,timer_bit_no = 2;
  int kbc_hook_id = 0,  ipc_status, timer_hook_id = 0;
  bool esc_pressed = false, r;
  uint16_t irq_set = BIT(kbc_bit_no);
  uint16_t irq_timer_set = BIT(timer_bit_no);

  CHECKCall(subscribe_interrupt(timer_bit_no ,&timer_hook_id, TIMER0_IRQ)); 
  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id));
  
  message msg;

  unsigned char scan[2];
  int scan_size;

  while (n_interrupts < n * TIMER_ASEC_FREQ && !esc_pressed) { 

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) { 
            n_interrupts = 0;
            kbc_ih();
            if (!kbc_get_error()) {
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == ESC_BREAK_CODE) {
                  esc_pressed = true;
                }

                CHECKCall(kbd_print_scancode(!(scan[scan_size - 1] & BREAK_CODE_BIT), scan_size, scan));
              }
            }
          }

          if (msg.m_notify.interrupts & irq_timer_set) {
            timer_int_handler();
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  CHECKCall(unsubscribe_interrupt(&timer_hook_id));
  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(kbd_print_no_sysinb(inb_counter));

  return EXIT_SUCCESS;

  return 1;
}
