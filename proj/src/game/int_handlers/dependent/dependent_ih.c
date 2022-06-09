#include "dependent_ih.h"

extern int (*state[])(struct mouse_ev *event);
extern int (*menu_state[])(struct mouse_ev *event, int x, int y);

EVENTS handle_evt(EVENTS event) {
  EVENTS ret = NO_EVT;

  if (event & BIT(TIMER_EVT)) {
    ret |= handle_timer_evt(event);
  }
  if (event & BIT(KBD_EVT)) {
    ret |= handle_kbd_evt(event);
  }
  if (event & BIT(MOUSE_EVT)) {
    ret |= handle_mouse_evt(event);
  }
  if (event & BIT(UART_EVT)) {
    ret |= handle_ser_evt(event);
  }

  return ret;
}

EVENTS handle_timer_evt(EVENTS event) {
  static int counter = 0;
  static int ticks_frame = 2;
  static int16_t speed = 10;
  counter++;
  if (speed > 0)
    if (counter % ticks_frame == 0) {
      draw_update();

      if (pendingMsg) {
        int margin = 0;
        // for (int it = 0; it < row; it++)
        {

          // if (it > 0)
          {
            //   break;
          }

          draw_text(user_msg[0], 830, 50 + margin, 0x00ff00, false);

          // draw_text(user_msg[it], 800, 40 + margin, 0x00ff00, true);
          margin += 50;
        }
      }

      if ((com_status == no_one || com_status == waiting) && get_menu_state() == online) {
        vg_draw_rectangle(240, 290, 320, 220, 0);
      }

      flush_screen();
    }

  return BIT(NO_EVT);
}
EVENTS handle_kbd_evt(EVENTS event) {

  static uint8_t ascii;
  static char send_msg[15];
  static int index = 0;

  if (!kbc_get_error()) {
    if (kbc_ready()) {

      kbc_get_scancode(scan, &scan_size);

      unsigned char aux = scan[scan_size - 1];

      if (aux == ESC_BREAK_CODE)
        return BIT(BREAK_EVT);
      else if (aux == RIGHT_ARROW)
        move_right = true;
      else if (aux == LEFT_ARROW)
        move_left = true;
      else if (aux == DOWN_ARROW) {
        move_down = true;
        set_kbd_selected_opt(true);
      }
      else if (aux == UP_ARROW) {
        move_up = true;
        set_kbd_selected_opt(false);
      }
      else if (aux == RELEASE_RIGHT_ARROW)
        move_right = false;
      else if (aux == RELEASE_LEFT_ARROW)
        move_left = false;
      else if (aux == RELEASE_DOWN_ARROW)
        move_down = false;
      else if (aux == RELEASE_UP_ARROW) {
        move_up = false;
      }
      else if (((ascii = get_ascii_from_scancode(aux)) >= 65 && ascii <= 90)) {
        send_msg[index] = ascii;
        index = (index + 1) % 15;
      }
      else if (aux == 0x1c) {
        set_menu_state(menu_lookup_transitions(get_menu_state(), get_kbd_selected_opt()));
        enum menu_state_codes st = get_menu_state();
        game_set_state(st);

        if (st == menu_end)
          return BIT(BREAK_EVT);
      }
      
      else if (aux == 0x39) {
        if (get_can_move()) {
          set_can_move(false);
          for (int j = 0; j <= index; j++) {
            Protocol prot = {
              .message = send_msg[j] - 65,
              .more_chars = (j == index) ? false : true};
            CHECKCall(ser_writeb(COM1_ADDR, encode_protocol(prot)));
            tickdelay(4);
          }
          memset((void *) send_msg, 0, sizeof(char) * 15);
          index = 0;
        }
      }
    }
  }
  return NO_EVT;
}

EVENTS handle_mouse_evt(EVENTS event) {
  if (!kbc_get_error()) {
    if (kbc_mouse_ready()) {
      kbc_get_mouse_data(scan);
      struct packet pp = mouse_data_to_packet(scan);

      m_event = mouse_get_event(&pp);

      if (m_event->type == MOUSE_MOV)
        mouse_update_pos(m_event->delta_x, m_event->delta_y);

      if (get_menu_state() != online || com_status == connected) {
        state_fun = state[cur_state];
        rc = state_fun(m_event);
        cur_state = lookup_transitions(cur_state, rc);
      }

      menu_state_fun = menu_state[get_menu_state()];
      enum menu_state_codes prevSt = get_menu_state();

      menu_rc = menu_state_fun(m_event, get_cursor_X(), get_cursor_Y());

      set_menu_state(menu_lookup_transitions(get_menu_state(), menu_rc));

      enum menu_state_codes st = get_menu_state();

      if (st == menu_end)
        return BIT(BREAK_EVT);
      else if ((st == online || st == multiplayer) && prevSt != st) {
        set_up_board();
      }
      else if ((prevSt == online || prevSt == multiplayer) && prevSt != st) {
        free_board();
      }
      static bool notSend = true;
      if ((notSend && st == online && com_status == no_one)) {
        if (get_can_move()) {
          notSend = false;
          set_can_move(false);
          Protocol pro = {
            .com_status = true,
            .message = waiting};
          com_status = waiting;
          set_online_color(true);
          ser_writeb(COM1_ADDR, encode_protocol(pro));
        }
      }
      else if (notSend && (st == online && com_status == waiting)) {
        if (get_can_move()) {
          notSend = false;
          set_can_move(false);
          Protocol pro = {
            .com_status = true,
            .message = connected};
          com_status = connected;
          set_connected(true);
          set_online_color(false);
          ser_writeb(COM1_ADDR, encode_protocol(pro));
        }
      }
      static bool toSend = false;
      if (prevSt == online && prevSt != st) {
        toSend = true;
      }
      if (toSend) {
        if (get_can_move()) {
          toSend = false;
          set_can_move(false);
          Protocol pro = {
            .com_status = true,
            .message = no_one};
          com_status = no_one;
          notSend = true;
          ser_writeb(COM1_ADDR, encode_protocol(pro));
        }
      }

      game_set_state(get_menu_state());
    }
  }
  return NO_EVT;
}

EVENTS handle_ser_evt(EVENTS events) {
  enum INT_TYPE ser_type = get_int_type();
  Protocol proCol, proLin;
  static uint8_t bt;

  if (ser_type == RECEIVE_DATA) {

    ser_readb(COM1_ADDR, &bt);
    decode_protocol(&proCol, bt);
    if (proCol.move) {
      tickdelay(10);
      ser_readb(COM1_ADDR, &bt);
      decode_protocol(&proLin, bt);

      move_piece_from_to(proLin.origin, proCol.origin, proLin.dest, proCol.dest);
    }
    else if (proCol.com_status) {
      com_status = proCol.message;
      set_connected(com_status == connected);
    }
    else if (bt != 0) {

      int i = 0;
      memset((void *) user_msg[row], 0, sizeof(char) * 15);
      while (proCol.more_chars) {
        user_msg[row][i] = proCol.message + 65;
        tickdelay(10);
        ser_readb(COM1_ADDR, &bt);
        decode_protocol(&proCol, bt);
        i++;
      }

      // row = (row +1) %6;
      pendingMsg = true;
    }
  }

  if (ser_type == TX_EMPTY) {
    set_can_move(true);
  }

  return NO_EVT;
}
