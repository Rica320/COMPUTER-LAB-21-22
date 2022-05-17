#ifndef _LCOM_DEPENDENT_IH_H_
#define _LCOM_DEPENDENT_IH_H_

#include "../independent/independent_ih.h"

#include "../../sprite/sprite.h"
#include "../../../drivers/keyboard/kbd_keys.h"
#include "../../state_machine/mouse_state.h"
#include "../../state_machine/menu_st.h"
#include "../../game.h"

static bool move_right = false, move_up = false, move_down = false, move_left = false;
static unsigned char scan[2];
static int scan_size;

static enum state_codes cur_state = ENTRY_STATE;
static enum ret_codes rc;
static int (* state_fun)(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

static enum menu_state_codes menu_cur_state = ENTRY_MENU_STATE;
static enum menu_ret_codes menu_rc;
static int (* menu_state_fun)(struct mouse_ev *event);

static struct mouse_ev *m_event;

EVENT_T handle_evt(EVENT_T);

EVENT_T handle_timer_evt(EVENT_T event);
EVENT_T handle_kbd_evt(EVENT_T event);
EVENT_T handle_mouse_evt(EVENT_T event);

#endif
