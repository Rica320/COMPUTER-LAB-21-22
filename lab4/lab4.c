// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include <kbc.h>
#include <keyboard.h>
#include <handlers.h>
#include <mouse.h>
#include <i8042.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

  uint8_t kbc_bit_no = 12;
  int kbc_hook_id = 12, ipc_status;
  bool r;
  uint16_t irq_set = BIT(kbc_bit_no);

  message msg;

  unsigned char scan[3];

  CHECKCall(_mouse_enable_data_reporting_()); // TODO: MAKE OUR OWN FUNCTION
  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, MOUSE_IRQ));

  while (cnt) { 

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
                cnt--;
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
  CHECKCall(mouse_disable_data_reporting());


  return EXIT_SUCCESS;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
