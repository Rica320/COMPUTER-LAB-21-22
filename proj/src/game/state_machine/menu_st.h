#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <lcom/lcf.h>

#define EXIT_MENU_STATE menu_end
#define ENTRY_MENU_STATE menu_entry

// Based on https://stackoverflow.com/questions/1371460/state-machines-tutorials
/**
 * @brief Detects Left Clicking Events in the Menu
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return OP1 If option 1 selected
 * @return OP2 If option 2 selected
 * @return menu_back If Back option selected
 * @return menu_repeat If no option was selected
 */
int menu_entry_state(struct mouse_ev *event, int x, int y);
/**
 * @brief Detects Left Clicking Events in the Menu
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return OP1 If option 1 selected
 * @return OP2 If option 2 selected
 * @return OP3 If option 3 selected
 * @return menu_back If Back option selected
 * @return menu_repeat If no option was selected
 */
int menu_play_state(struct mouse_ev *event, int x, int y);
/**
 * @brief Detects Left Clicking Events in the Menu
 * @param event Event Detected
 * @param x X Coordinates of Pixel Clicked
 * @param y Y Coordinates of Pixel Clicked
 * @return menu_back If Back option selected
 * @return menu_repeat If no option was selected
 */
int menu_multiplayer_state(struct mouse_ev *event, int x, int y);
int menu_online_state(struct mouse_ev *event, int x, int y);
int menu_instructions_state(struct mouse_ev *event, int x, int y);
int menu_exit_state(struct mouse_ev *event, int x, int y);

enum menu_state_codes { menu_entry,
                        menu_play,
                        multiplayer,
                        online,
                        instructions,
                        menu_end };

enum menu_ret_codes { menu_fail,
                      menu_repeat,
                      menu_back,
                      OP1,
                      OP2,
                      OP3 };

typedef struct menu_transition {
  enum menu_state_codes src_state;
  enum menu_ret_codes ret_code;
  enum menu_state_codes dst_state;
} MENU_ST_TRANS;

enum menu_state_codes menu_lookup_transitions(int cur_state, int rc);

static enum menu_state_codes menu_cur_state = ENTRY_MENU_STATE;
enum menu_state_codes get_menu_state();
void set_menu_state(enum menu_state_codes st);

#endif
