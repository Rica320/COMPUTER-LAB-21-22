#ifndef _LCOM_STATEMACHINE_H_
#define _LCOM_STATEMACHINE_H_

#include <lcom/lcf.h>
#include "../views/views.h"
#include "../../drivers/serial_port/uart.h"
#include "../../drivers/serial_port/uart_defines.h"
#include "../protocol/communication_protocol.h"
#include "menu_st.h"

static bool can_move;

//https://stackoverflow.com/questions/1371460/state-machines-tutorials

int entry_state(struct mouse_ev *event);
int grab_state(struct mouse_ev *event);
int pick_state(struct mouse_ev *event);
int exit_state(struct mouse_ev *event);
enum state_codes { entry, grab, pick, end};

enum ret_codes { ok, fail, repeat, back};
typedef struct transition {
    enum state_codes src_state;
    enum ret_codes   ret_code;
    enum state_codes dst_state;
}ST_TRANS;
/* transitions from end state aren't needed */

enum state_codes lookup_transitions(int cur_state, int rc);

#define EXIT_STATE end
#define ENTRY_STATE entry


void set_can_move(bool move);

#endif
