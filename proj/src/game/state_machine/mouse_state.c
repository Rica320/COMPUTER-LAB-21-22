#include "mouse_state.h"


int (*state[])(struct mouse_ev *event) = { entry_state, grab_state, exit_state};

int entry_state(struct mouse_ev *event) {
  if (event->type == LB_PRESSED){
    set_selected_case(get_cursor_X(),get_cursor_Y());
    return ok;
  }
  return repeat;
}
int grab_state(struct mouse_ev *event) {
  if (event->type == MOUSE_MOV) {
    return repeat;
  }
  else if (event->type == LB_RELEASED) {

    return ok;
  }

  return fail;
}

int exit_state(struct mouse_ev *event) {
  return ok;
}

enum state_codes lookup_transitions( int cur_state, int rc) {

  ST_TRANS state_transitions[] = {
    {entry, ok,     grab},
    {entry, repeat,     entry},
    {entry, fail,   entry},
    {grab,   ok,     end},
    {grab,   fail,   entry},
    {grab,   repeat, grab},
    {end,   ok,   entry},
    {end,   repeat, entry},
    {end,   fail,   entry},
  };

  for (size_t i = 0; i < 13; i++)
  {
    if (state_transitions[i].src_state == cur_state && state_transitions[i].ret_code == rc)
    {
      return state_transitions[i].dst_state;
    }
  }
  return cur_state;
}
