#include "game.h"
#include <lcom/lcf.h>
#include "../drivers/keyboard/kbd_keys.h"

extern int (* state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

void game_loop() {

  vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x00ff00ff);
  
  
  xpm_map_t m = minix3_xpm;
  sprite_t * sprite = make_sprite(m , XPM_8_8_8_8);
  
  set_sprite_X(sprite, 10);
  set_sprite_Y(sprite, 10);

  draw_sprite_in_mode_14c(sprite);


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

  bool move_right = false, move_up = false, move_down=false, move_left=false;
  int counter = 0;
  int frames = 0;
  int ticks_frame = sys_hz() / 10;
  int16_t speed = 10;
  int mov = 1;
  if (speed > 0)
    mov = speed;


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
            counter++;
            frames++;
            if (counter % ticks_frame == 0) {
              vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x0);
          
              if (mov == 1)
              {
                frames++;
                if (frames % (-speed) != 0)
                  break;
              }
              
              if (move_right) // TODO: CHECK THE BOUNDARIES
              {
                if (get_sprite_X(sprite) + speed + get_sprite_W(sprite) <= get_hres())
                  set_sprite_X(sprite, get_sprite_X(sprite) + speed);
              }
              if (move_left)
              {
                if (get_sprite_X(sprite) >= (uint32_t)speed)
                  set_sprite_X(sprite, get_sprite_X(sprite) - speed);
              }
              if (move_down)
              {
                if (get_sprite_Y(sprite) + speed + get_sprite_H(sprite) <= get_vres())
                  set_sprite_Y(sprite, get_sprite_Y(sprite) + speed);
              }
              if (move_up)
              {
                if (get_sprite_Y(sprite) >= (uint32_t)speed)
                  set_sprite_Y(sprite, get_sprite_Y(sprite) - speed);
              }
              
              
            }

            draw_sprite_in_mode_14c(sprite);
            flush_screen();
          }

          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            if (!kbc_get_error()) {
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == (ESC_BREAK_CODE)) {
                  esc_pressed = true;
                }
                if (scan[scan_size - 1] == RIGHT_ARROW)
                {
                  move_right = true;
                }
                if (scan[scan_size - 1] == LEFT_ARROW)
                {
                  move_left = true;
                }
                if (scan[scan_size - 1] == DOWN_ARROW)
                {
                  move_down = true;
                }
                if (scan[scan_size - 1] == UP_ARROW)
                {
                  move_up = true;
                }
                
                if (scan[scan_size - 1] == RELEASE_RIGHT_ARROW)
                {
                  move_right = false;
                }
                if (scan[scan_size - 1] == RELEASE_LEFT_ARROW)
                {
                  move_left = false;
                }
                if (scan[scan_size - 1] == RELEASE_DOWN_ARROW)
                {
                  move_down = false;
                }
                if (scan[scan_size - 1] == RELEASE_UP_ARROW)
                {
                  move_up = false;
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
                //mouse_print_packet(&pp);

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
