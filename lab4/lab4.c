// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include <kbc.h>
#include <keyboard.h>
#include <handlers.h>
#include <mouse.h>
#include <i8042.h>
#include <state_machine.h>

// Any header files included below this line should have been created by you
extern uint32_t n_interrupts;
extern int (* state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

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
  uint8_t kbc_bit_no = 12;
  int kbc_hook_id = 12, ipc_status;
  bool r;
  uint16_t irq_set = BIT(kbc_bit_no);

  uint8_t time = 0 ;
  uint8_t frequency = sys_hz();

  uint8_t timer_id = 0; // THE WAY WE IMPLEMENTED WE ALREADY KNOW THE TIMER ID
  uint16_t irq_timer; // TODO: 16 bits ?

  message msg;

  unsigned char scan[3];

  CHECKCall(_mouse_enable_data_reporting_()); // TODO: MAKE OUR OWN FUNCTION
  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, MOUSE_IRQ));
  CHECKCall(timer_subscribe_int(&timer_id)); 

  irq_timer = BIT(timer_id);

  while (time < idle_time) { 

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer)
          {
            timer_int_handler();
            if ( n_interrupts % frequency == 0)
            {
              time++;
            }
            
          }
          if (msg.m_notify.interrupts & irq_set) { 
            mouse_ih();
            if (!kbc_get_error()) { // TODO: MAKE THE FUNC RETURN THE TYPES THAT WE READ
              if (kbc_mouse_ready()) {
                kbc_get_mouse_data(scan);

                struct packet pp = mouse_data_to_packet(scan);
                mouse_print_packet(&pp);
                time = 0;
                n_interrupts = 0;
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

  CHECKCall(timer_unsubscribe_int());
  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(mouse_disable_data_reporting());


  return EXIT_SUCCESS;
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  
  uint8_t kbc_bit_no = 12;
  int kbc_hook_id = 12, ipc_status;
  bool r;
  uint16_t irq_set = BIT(kbc_bit_no);

  message msg;

  unsigned char scan[3];

  CHECKCall(_mouse_enable_data_reporting_()); // TODO: MAKE OUR OWN FUNCTION
  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, MOUSE_IRQ));

  enum state_codes cur_state = ENTRY_STATE;
  enum ret_codes rc;
  int (* state_fun)(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
  struct mouse_ev *event;
  bool ended =false;
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

  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(mouse_disable_data_reporting());


  return EXIT_SUCCESS;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
