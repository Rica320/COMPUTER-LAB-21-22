#ifndef _LCOM_STATEMACHINE_H_
#define _LCOM_STATEMACHINE_H_

#include <lcom/lcf.h>

static int x_length;
static int y_length;

// https://stackoverflow.com/questions/1371460/state-machines-tutorials

int entry_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
int drag_up_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
int vertex_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
int drag_down_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
int exit_state(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);
enum state_codes { entry,
                   drag_up,
                   vertex,
                   drag_down,
                   end };

enum ret_codes { ok,
                 fail,
                 repeat,
                 back };
typedef struct transition {
  enum state_codes src_state;
  enum ret_codes ret_code;
  enum state_codes dst_state;
} ST_TRANS;
/* transitions from end state aren't needed */

enum state_codes lookup_transitions(int cur_state, int rc);

#define EXIT_STATE end
#define ENTRY_STATE entry

#endif
