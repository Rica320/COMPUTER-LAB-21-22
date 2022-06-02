#include "menu_st.h"

int (*menu_state[])(struct mouse_ev *event, int x, int y) = {
  menu_entry_state, menu_play_state, menu_multiplayer_state, menu_online_state, menu_instructions_state, menu_exit_state};

int menu_entry_state(struct mouse_ev *event, int x, int y) {
  if (event->type == LB_PRESSED)
    if ((x > 400) && (x < 900)) {
      if (y >= 200 && y < 400)
        return OP1;
      if (y >= 400 && y < 600)
        return OP2;
      if (y >= 600 && y < 800)
        return menu_back;
    }
  return menu_repeat;
}

int menu_play_state(struct mouse_ev *event, int x, int y) {
  if (event->type == LB_PRESSED)
    if ((x > 400) && (x < 900)) {
      if (y >= 200 && y < 400)
        return OP1;
      if (y >= 400 && y < 600)
        return OP2;
      if (y >= 600 && y < 800)
        return menu_back;
    }
  return menu_repeat;
}

int menu_multiplayer_state(struct mouse_ev *event, int x, int y) {
  if (event->type == LB_PRESSED)
    if ((x > 850) && (x < 1000))
      if (y >= 750 && y < 1000)
        return menu_back;
  return menu_repeat;
}

int menu_online_state(struct mouse_ev *event, int x, int y) {
  if (event->type == LB_PRESSED)
    if ((x > 850) && (x < 1000))
      if (y >= 750 && y < 1000)
        return menu_back;
  return menu_repeat;
}

int menu_instructions_state(struct mouse_ev *event, int x, int y) {
  if (event->type == LB_PRESSED)
    if ((x > 400) && (x < 900)) {
      if (y >= 200 && y < 400)
        return menu_repeat;
      if (y >= 400 && y < 600)
        return menu_back;
    }
  return menu_repeat;
}

int menu_exit_state(struct mouse_ev *event, int x, int y) {
  return menu_repeat;
}
enum menu_state_codes menu_lookup_transitions(int cur_state, int rc) {

  MENU_ST_TRANS state_transitions[] = {

    // repeat states
    {menu_entry, menu_repeat, menu_entry},
    {menu_play, menu_repeat, menu_play},
    {multiplayer, menu_repeat, multiplayer},
    {online, menu_repeat, online},
    {instructions, menu_repeat, instructions},

    {menu_entry, OP1, menu_play},
    {menu_entry, OP2, instructions},
    {menu_entry, menu_back, menu_end},

    {menu_play, OP1, multiplayer},
    {menu_play, OP2, online},
    {menu_play, menu_back, menu_entry},

    {instructions, menu_back, menu_entry},
    {multiplayer, menu_back, menu_play},
    {online, menu_back, menu_play},

  };

  for (size_t i = 0; i < 14; i++) {
    if (state_transitions[i].src_state == cur_state && state_transitions[i].ret_code == rc) {
      return state_transitions[i].dst_state;
    }
  }
  return cur_state;
}

enum menu_state_codes get_menu_state() {
  return menu_cur_state;
}

void set_menu_state(enum menu_state_codes st) {
  menu_cur_state = st;
}
