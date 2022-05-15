#include "game.h"
#include <lcom/lcf.h>
#include "../drivers/keyboard/kbd_keys.h"

extern int (* state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

void game_loop() {

  vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x00ff00ff);
  
  
  xpm_map_t m = minix3_xpm;
  sprite_t * minix = make_sprite(m , XPM_8_8_8_8);
  
  set_sprite_X(minix, 10);
  set_sprite_Y(minix, 10);

  draw_sprite_in_mode_14c(minix);


  flush_screen();

  /* THE KEYBOARD*/
  uint8_t kbc_bit_no = 1;
  int kbc_hook_id = 0, ipc_status;
  bool esc_pressed = false, r;
  uint16_t irq_set = BIT(kbc_bit_no);

  uint8_t kbc_mouse_bit_no = 12;
  int kbc_mouse_hook_id = 12;
  uint16_t irq_mouse_set = BIT(kbc_mouse_bit_no);

  uint8_t timer_id = 0; 
  uint16_t irq_timer;


  enum state_codes cur_state = ENTRY_STATE;
  enum ret_codes rc;
  int (* state_fun)(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
  struct mouse_ev *event;
  bool ended =false;

  message msg;

  unsigned char scan[2];
  int scan_size;

  CHECKCall(_mouse_enable_data_reporting_());
  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, KBC_IRQ));
  CHECKCall(subscribe_kbc_interrupt(kbc_mouse_bit_no, &kbc_mouse_hook_id, MOUSE_IRQ));
  CHECKCall(timer_subscribe_int(&timer_id)); 

  irq_timer = BIT(timer_id);

  while (!esc_pressed && !ended) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) {
            timer_int_handler();
            //vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x00ff00ff);
            draw_sprite_in_mode_14c(minix);
            flush_screen();
          }

          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            if (!kbc_get_error()) {
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == ESC_BREAK_CODE) {
                  esc_pressed = true;
                }
                if (scan[scan_size - 1] == RIGHT_ARROW)
                {
                  if (get_sprite_X(minix) + 2 + get_sprite_W(minix) <= get_hres())
                    set_sprite_X(minix, get_sprite_X(minix) + 2);
                }
                if (scan[scan_size - 1] == LEFT_ARROW)
                {
                  if (get_sprite_X(minix) >= 2)
                    set_sprite_X(minix, get_sprite_X(minix) - 2);
                }
                if (scan[scan_size - 1] == DOWN_ARROW)
                {
                  if (get_sprite_Y(minix) + 2 + get_sprite_H(minix) <= get_vres())
                    set_sprite_Y(minix, get_sprite_Y(minix) + 2);
                }
                if (scan[scan_size - 1] == UP_ARROW)
                {
                  if (get_sprite_Y(minix) >= 2)
                    set_sprite_Y(minix, get_sprite_Y(minix) - 2);
                }
              }
            }
          }
          if (msg.m_notify.interrupts & irq_mouse_set) { 
            mouse_ih();
            if (!kbc_get_error()) { // TODO: MAKE THE FUNC RETURN THE TYPES THAT WE READ
              if (kbc_mouse_ready()) {
                kbc_get_mouse_data(scan);

                struct packet pp = mouse_data_to_packet(scan);
                mouse_print_packet(&pp);

                event = mouse_get_event(&pp);

                state_fun = state[cur_state];
                rc = state_fun(event, 10, 10);
                
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

  CHECKCall(timer_unsubscribe_int());
  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(unsubscribe_interrupt(&kbc_mouse_hook_id));
  CHECKCall(mouse_disable_data_reporting());


}
