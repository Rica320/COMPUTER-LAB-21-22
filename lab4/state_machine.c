#include <state_machine.h>

static int x_length = 0;
static int y_length = 0;

int (*state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance) = {entry_state, drag_up_state, vertex_state, drag_down_state, exit_state};

int entry_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance) {
  if (event->type == LB_PRESSED) {
    return ok;
  }
  return repeat;
}
int drag_up_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance) {
  if (event->type == MOUSE_MOV) {
    if (event->delta_x < -tolerance || event->delta_y < -tolerance) {
      x_length = 0;
      y_length = 0;
      return fail;
    }

    x_length += event->delta_x;
    y_length += event->delta_y;
  }
  else if (event->type == LB_RELEASED) {
    if (y_length < x_length || x_length < x_len) {
      x_length = 0;
      y_length = 0;
      return fail;
    }
    x_length = 0;
    y_length = 0;
    return ok;
  }
  else {
    x_length = 0;
    y_length = 0;
    return fail;
  }

  return repeat;
}

int vertex_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance) {
  if (event->type == MOUSE_MOV) {
    x_length += event->delta_x;
    y_length += event->delta_y;
    if (abs(x_length) > tolerance || abs(y_length) > tolerance) {
      x_length = 0;
      y_length = 0;
      return fail;
    }
  }
  else if (event->type == RB_PRESSED) {
    x_length = 0;
    y_length = 0;
    return ok;
  }
  else if (event->type == LB_PRESSED) {
    x_length = 0;
    y_length = 0;
    return back;
  }
  else {
    x_length = 0;
    y_length = 0;
    return fail;
  }

  return repeat;
}

int drag_down_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance) {
  if (event->type == MOUSE_MOV) {
    if (event->delta_x < -tolerance || event->delta_y > tolerance) {
      x_length = 0;
      y_length = 0;
      return fail;
    }

    x_length += event->delta_x;
    y_length += event->delta_y;
  }
  else if (event->type == RB_RELEASED) {
    if (x_length == 0 || y_length == 0) {
      return fail;
      ;
    }

    if (y_length > -x_length || x_length < x_len) {
      x_length = 0;
      y_length = 0;
      return fail;
    }

    x_length = 0;
    y_length = 0;
    return ok;
  }
  else {
    x_length = 0;
    y_length = 0;
    return fail;
  }
  return repeat;
}

int exit_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance) {
  return ok;
}

enum state_codes lookup_transitions(int cur_state, int rc) {

  ST_TRANS state_transitions[] = {
    {entry, ok, drag_up},
    {entry, repeat, entry},
    {entry, fail, entry},
    {drag_up, ok, vertex},
    {drag_up, fail, entry},
    {drag_up, repeat, drag_up},
    {vertex, ok, drag_down},
    {vertex, fail, entry},
    {vertex, repeat, vertex},
    {vertex, back, drag_up},
    {drag_down, ok, end},
    {drag_down, repeat, drag_down},
    {drag_down, fail, entry},
  };

  for (size_t i = 0; i < 13; i++) {
    if (state_transitions[i].src_state == cur_state && state_transitions[i].ret_code == rc) {
      return state_transitions[i].dst_state;
    }
  }
  return cur_state;
}
