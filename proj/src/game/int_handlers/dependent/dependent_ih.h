#ifndef _LCOM_DEPENDENT_IH_H_
#define _LCOM_DEPENDENT_IH_H_

#include <lcom/lcf.h>

#include "../../../drivers/kbc/kbc.h"
#include "../../../drivers/keyboard/kbd_keys.h"
#include "../../../drivers/mouse/mouse.h"
#include "../../protocol/communication_protocol.h"
#include "../../state_machine/menu_st.h"
#include "../../state_machine/mouse_st.h"
#include "../../views/font/font.h"
#include "../../views/sprite/sprite.h"
#include "../../views/views.h"
#include "../independent/independent_ih.h"

static bool move_right = false, move_up = false, move_down = false, move_left = false;
static unsigned char scan[2];
static int scan_size;

static bool pendingMsg = false;
static char user_msg[6][15];
static int row = 0;

static enum state_codes cur_state = ENTRY_STATE;
static enum ret_codes rc;
static int (*state_fun)(struct mouse_ev *event);

static enum Communication_Status com_status = no_one;

static enum menu_ret_codes menu_rc;
static int (*menu_state_fun)(struct mouse_ev *event, int x, int y);

static struct mouse_ev *m_event;

EVENTS handle_evt(EVENTS);

EVENTS handle_timer_evt(EVENTS event);
EVENTS handle_kbd_evt(EVENTS event);
EVENTS handle_mouse_evt(EVENTS event);
EVENTS handle_ser_evt(EVENTS events);

#endif
