#include "mouse_st.h"

int (*state[])(struct mouse_ev *event) = {entry_state, grab_state, pick_state, exit_state};

int entry_state(struct mouse_ev *event) {
  if (event->type == LB_PRESSED) {
    set_selected_case(get_cursor_Y(), get_cursor_X());
    return ok;
  }
  return repeat;
}

int grab_state(struct mouse_ev *event) {
  if (event->type == MOUSE_MOV) {

    return repeat;
  }
  else if (event->type == LB_RELEASED) {
    uint8_t lin = 9, col = 9;

    get_mouse_case(get_cursor_Y(), get_cursor_X(), &col, &lin);

    if (lin == 9 || col == 9) {
      return fail;
    }

    if (is_selected_case(lin, col)) {
      return back;
    }

    if (is_valid_move(lin, col)) {
      if (can_move && get_menu_state() == online) {
        Piece_Color color = get_piece_at_pos_color(get_selected_lin(), get_selected_col());

        if (color == WHITE && !get_online_color()) {
          return fail;
        }
        else if (get_online_color() && color == BLACK) {
          return fail;
        }

        move_piece(lin, col);

        Protocol proC = {
          .origin = get_selected_col(),
          .dest = col,
          .move = true};

        Protocol proL = {
          .origin = get_selected_lin(),
          .dest = lin,
          .move = true};

        can_move = false;
        CHECKCall(ser_writeb(COM1_ADDR, encode_protocol(proC)));
        tickdelay(4);
        CHECKCall(ser_writeb(COM1_ADDR, encode_protocol(proL)));
      }
      else {
        move_piece(lin, col);
      }
    }
    else {
      return fail;
    }

    return ok;
  }

  return fail;
}

int pick_state(struct mouse_ev *event) {
  if (event->type == MOUSE_MOV) {
    return repeat;
  }
  else if (event->type == LB_PRESSED) {
    uint8_t lin = 9, col = 9;
    get_mouse_case(get_cursor_Y(), get_cursor_X(), &col, &lin);

    if (lin == 9 || col == 9) {
      return fail;
    }

    if (is_valid_move(lin, col)) {
      if (can_move && get_menu_state() == online) {
        Piece_Color color = get_piece_at_pos_color(get_selected_lin(), get_selected_col());

        if (color == WHITE && !get_online_color()) {
          return fail;
        }
        else if (get_online_color() && color == BLACK) {
          return fail;
        }

        move_piece(lin, col);

        Protocol proC = {
          .origin = get_selected_col(),
          .dest = col,
          .move = true};

        Protocol proL = {
          .origin = get_selected_lin(),
          .dest = lin,
          .move = true};

        can_move = false;
        CHECKCall(ser_writeb(COM1_ADDR, encode_protocol(proC)));
        tickdelay(4);
        CHECKCall(ser_writeb(COM1_ADDR, encode_protocol(proL)));
      }
      else {
        move_piece(lin, col);
      }
    }
    else {
      return fail;
    }

    return ok;
  }

  return fail;
}

int exit_state(struct mouse_ev *event) {
  return ok;
}

enum state_codes lookup_transitions(int cur_state, int rc) {

  ST_TRANS state_transitions[] = {
    {entry, ok, grab},
    {entry, repeat, entry},
    {entry, fail, entry},

    {grab, ok, end},
    {grab, fail, entry},
    {grab, repeat, grab},
    {grab, back, pick},

    {pick, ok, end},
    {pick, fail, entry},
    {pick, repeat, pick},
    
    {end, ok, entry},
    {end, repeat, entry},
    {end, fail, entry},
  };

  for (size_t i = 0; i < 13; i++) {
    if (state_transitions[i].src_state == cur_state && state_transitions[i].ret_code == rc) {
      return state_transitions[i].dst_state;
    }
  }
  return cur_state;
}

void set_can_move(bool move) {
  can_move = true;
}

bool get_can_move() {
  return can_move;
}
