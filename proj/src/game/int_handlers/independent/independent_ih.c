#include "independent_ih.h"

void subscribe_ihs() {
  uint8_t kbc_bit_no = 1;
  kbc_hook_id = 1;
  irq_set = BIT(kbc_bit_no);

  uint8_t kbc_mouse_bit_no = 12;
  kbc_mouse_hook_id = 12;
  irq_mouse_set = BIT(kbc_mouse_bit_no);

  uint8_t timer_id = 2;
  uint8_t rtc_id = 8;
  uint8_t bit_n, bt;

  CHECKCall(_mouse_enable_data_reporting_());
  ser_subscribe_int(&bit_n, &ser_hook_id);
  set_ier(COM1_ADDR, IER_RECEIVED_INT | IER_RECEIVER_LINE_INT | IER_TRANSMITTER_INT, true);

  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, KBC_IRQ));
  CHECKCall(subscribe_kbc_interrupt(kbc_mouse_bit_no, &kbc_mouse_hook_id, MOUSE_IRQ));
  CHECKCall(timer_subscribe_int(&timer_id));
  CHECKCall(rtc_subscribe_int(&rtc_id));

  util_sys_inb(COM1_ADDR + UART_RBR, &bt);

  irq_ser = BIT(bit_n);
  irq_timer = BIT(timer_id);
  irq_rtc = BIT(rtc_id);
}

EVENTS handle_ihs() {
  EVENTS event = NO_EVT;

  if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    printf("driver_receive failed with: %d", r);
    return event;
  }
  if (is_ipc_notify(ipc_status)) {
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_timer) {
          timer_int_handler();
          event |= BIT(TIMER_EVT);
        }
        if (msg.m_notify.interrupts & irq_set) {
          kbc_ih();
          event |= BIT(KBD_EVT);
        }
        if (msg.m_notify.interrupts & irq_rtc) {
          rtc_ih();
          event |= BIT(RTC_EVT);
        }
        if (msg.m_notify.interrupts & irq_mouse_set) {
          mouse_ih();
          event |= BIT(MOUSE_EVT);
        }
        if (msg.m_notify.interrupts & irq_ser) {
          uart_ih();
          event |= BIT(UART_EVT);
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
  CHECKCall(rtc_unsubscribe_int());
  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(unsubscribe_interrupt(&kbc_mouse_hook_id));
  set_ier(COM1_ADDR, IER_RECEIVED_INT | IER_RECEIVER_LINE_INT | IER_TRANSMITTER_INT, false);
  CHECKCall(unsubscribe_interrupt(&ser_hook_id));
  CHECKCall(mouse_disable_data_reporting());
}
