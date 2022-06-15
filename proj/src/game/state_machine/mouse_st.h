#ifndef _LCOM_STATEMACHINE_H_
#define _LCOM_STATEMACHINE_H_

#include <lcom/lcf.h>

#include "../../drivers/serial_port/uart.h"
#include "../../drivers/serial_port/uart_defines.h"
#include "../protocol/communication_protocol.h"
#include "../views/views.h"
#include "menu_st.h"

#define EXIT_STATE end
#define ENTRY_STATE entry

static bool can_move;

// https://stackoverflow.com/questions/1371460/state-machines-tutorials

/**
 * @brief Sets the mouse entry state based on the given event
 *
 * @param event mouse event to be evaluated
 * @return ret code of state
 */
int entry_state(struct mouse_ev *event);

/**
 * @brief Sets the mouse grab state based on the given event
 *
 * @param event mouse event to be evaluated
 * @return ret code of state
 */
int grab_state(struct mouse_ev *event);

/**
 * @brief Sets the mouse pick state based on the given event
 *
 * @param event mouse event to be evaluated
 * @return ret code of state
 */
int pick_state(struct mouse_ev *event);

/**
 * @brief Sets the mouse exit state based on the given event
 *
 * @param event mouse event to be evaluated
 * @return ret code of state
 */
int exit_state(struct mouse_ev *event);

/**
 * @brief Mouse possible states
 *
 */
enum state_codes { entry,
                   grab,
                   pick,
                   end };

/**
 * @brief Mouse possible state transition codes
 *
 */
enum ret_codes { ok,
                 fail,
                 repeat,
                 back };

/**
 * @brief Mouse state transitionm struct
 *
 */
typedef struct transition {
  enum state_codes src_state;
  enum ret_codes ret_code;
  enum state_codes dst_state;
} ST_TRANS;

/**
 * @brief Updates the mouse machine state based on the given state and
 * the given kbd state
 *
 * @param cur_state current mouse state
 * @param rc kbd current state
 * @return enum state_codes new mouse state
 */
enum state_codes lookup_transitions(int cur_state, int rc);

/**
 * @brief Sets if the move can be done in online game, being an empty transmitter indicator
 *
 * @param move boolean if move can be done
 */
void set_can_move(bool move);

/**
 * @brief Get the can move boolean, indicating is move can be done
 *
 * @return true can move
 * @return false cannot move
 */
bool get_can_move();

#endif
