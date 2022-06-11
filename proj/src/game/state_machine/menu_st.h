#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <lcom/lcf.h>

#define EXIT_MENU_STATE menu_end
#define ENTRY_MENU_STATE menu_entry

// Based on https://stackoverflow.com/questions/1371460/state-machines-tutorials

/**
 * @brief Detects Left Clicking Events in the Main Menu
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return ret code of state
 */
int menu_entry_state(struct mouse_ev *event, int x, int y);

/**
 * @brief Detects Left Clicking Events In Mode Option Menu
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return ret code of state
 */
int menu_play_state(struct mouse_ev *event, int x, int y);

/**
 * @brief Detects Left Clicking Events during Multiplayer Play
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return ret code of state
 */
int menu_multiplayer_state(struct mouse_ev *event, int x, int y);

/**
 * @brief Detects Left Clicking Events during Online Play
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return ret code of state
 */
int menu_online_state(struct mouse_ev *event, int x, int y);

/**
 * @brief Detects Left Clicking Events in the Instructions Menu
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return ret code of state
 */
int menu_instructions_state(struct mouse_ev *event, int x, int y);

/**
 * @brief Detects Left Clicking Events while exiting game
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return ret code of state
 */
int menu_exit_state(struct mouse_ev *event, int x, int y);

/**
 * @brief Menu possible states
 *
 */
enum menu_state_codes { menu_entry,
                        menu_play,
                        multiplayer,
                        online,
                        instructions,
                        menu_end };

/**
 * @brief Menu possible transition state codes
 *
 */
enum menu_ret_codes {
  OP1,
  OP2,
  OP3,
  menu_back,
  menu_fail,
  menu_repeat,
};

/**
 * @brief Menu transition struct
 *
 */
typedef struct menu_transition {
  enum menu_state_codes src_state;
  enum menu_ret_codes ret_code;
  enum menu_state_codes dst_state;
} MENU_ST_TRANS;

/**
 * @brief Given a current state and a transition returns
 * the new menu machine state
 *
 * @param cur_state current menu state
 * @param rc kbd selected state
 * @return enum menu_state_codes new menu state
 */
enum menu_state_codes menu_lookup_transitions(int cur_state, int rc);

static enum menu_state_codes menu_cur_state = ENTRY_MENU_STATE;

/**
 * @brief Returns Menu State
 * @return Current Menu State
 */
enum menu_state_codes get_menu_state();

/**
 * @brief Set the Menu State
 * @param st New menu state
 */
void set_menu_state(enum menu_state_codes st);

#endif
