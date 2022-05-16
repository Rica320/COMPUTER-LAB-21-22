#include "independent_ih.h"

void subscribe_ihs() {
  uint8_t kbc_bit_no = 1;
  kbc_hook_id = 0;
  irq_set = BIT(kbc_bit_no);

  uint8_t kbc_mouse_bit_no = 12;
  kbc_mouse_hook_id = 12;
  irq_mouse_set = BIT(kbc_mouse_bit_no);

  uint8_t timer_id = 0; 

  CHECKCall(_mouse_enable_data_reporting_());
  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, KBC_IRQ));
  CHECKCall(subscribe_kbc_interrupt(kbc_mouse_bit_no, &kbc_mouse_hook_id, MOUSE_IRQ));
  CHECKCall(timer_subscribe_int(&timer_id)); 

  irq_timer = BIT(timer_id);
}

EVENT_T handle_ihs() {
  EVENT_T event = NO_EVT;

  if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      return NO_EVT;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) {
            timer_int_handler();
            event = TIMER_EVT;
          }

          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih(); // TODO: CHANGE NAME
            event = KBD_EVT;
          }
          if (msg.m_notify.interrupts & irq_mouse_set) { 
            mouse_ih();
            event = MOUSE_EVT;
          }

          break;
        default:
          break;
      }
    }
    else {
    }

  return event;
}

void unsubscribe_ihs() {
  CHECKCall(timer_unsubscribe_int());
  CHECKCall(unsubscribe_interrupt(&kbc_hook_id      ));
  CHECKCall(unsubscribe_interrupt(&kbc_mouse_hook_id));
  CHECKCall(mouse_disable_data_reporting());
}
