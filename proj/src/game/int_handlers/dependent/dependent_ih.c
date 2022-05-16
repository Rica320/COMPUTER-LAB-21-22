#include "dependent_ih.h"

extern int (* state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
extern int (* menu_state[])(struct mouse_ev *event);

EVENT_T handle_evt(EVENT_T event) {
  switch (event)
  {
  case TIMER_EVT:
    return handle_timer_evt(event);
  case MOUSE_EVT:
    return handle_mouse_evt(event);
  case KBD_EVT:
    return handle_kbd_evt(event);
  default:
    break;
  }

  return NO_EVT;
}

EVENT_T handle_timer_evt(EVENT_T event) {
  int counter = 0;
  int frames = 0;
  int ticks_frame = sys_hz() / 10;
  int16_t speed = 10;
  int mov = 1;
  if (speed > 0)
    if (counter % ticks_frame == 0) {
      //vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x0);
      if (mov == 1) {
        frames++;
        // if (frames % (-speed) != 0)
        //   return BREAK_EVT;
      }

      if (move_right) // TODO: CHECK THE BOUNDARIES
      {
        //if (get_sprite_X(sprite) + speed + get_sprite_W(sprite) <= get_hres())
        //  set_sprite_X(sprite, get_sprite_X(sprite) + speed);
      }//
      if (move_left) {
       //if (get_sprite_X(sprite) >= (uint32_t) speed)
       //  set_sprite_X(sprite, get_sprite_X(sprite) - speed);
      }
      if (move_down) {
        //if (get_sprite_Y(sprite) + speed + get_sprite_H(sprite) <= get_vres())
        //  set_sprite_Y(sprite, get_sprite_Y(sprite) + speed);
      }
      if (move_up) {
       // if (get_sprite_Y(sprite) >= (uint32_t) speed)
       //   set_sprite_Y(sprite, get_sprite_Y(sprite) - speed);
      }
    }
  //draw_sprite_in_mode_14c(sprite);
  //flush_screen();
  return NO_EVT;
}
EVENT_T handle_kbd_evt(EVENT_T event) {
  if (!kbc_get_error()) {
  if (kbc_ready()) {
    kbc_get_scancode(scan, &scan_size);
    if (scan[scan_size - 1] == (ESC_BREAK_CODE)) {
      // esc_pressed = true;
      return BREAK_EVT;
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
  return NO_EVT;
}
EVENT_T handle_mouse_evt(EVENT_T event) {
  if (!kbc_get_error()) { // TODO: MAKE THE FUNC RETURN THE TYPES THAT WE READ
    if (kbc_mouse_ready()) {
      kbc_get_mouse_data(scan);
      struct packet pp = mouse_data_to_packet(scan);
      // mouse_print_packet(&pp);
      m_event = mouse_get_event(&pp);
      state_fun = state[cur_state];
      rc = state_fun(m_event, 10, 10);

      cur_state = lookup_transitions(cur_state, rc);
      if (EXIT_STATE == cur_state)
        return BREAK_EVT;

      menu_state_fun = menu_state[cur_state];
      menu_rc = menu_state_fun(m_event);
      menu_cur_state = menu_lookup_transitions(menu_cur_state, rc);
      if (multiplayer == cur_state)
        return BREAK_EVT;
    }
  }
  return NO_EVT;
}
