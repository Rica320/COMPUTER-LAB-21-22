#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <lcom/lcf.h>

// https://stackoverflow.com/questions/1371460/state-machines-tutorials

int menu_entry_state(struct mouse_ev *event);
int menu_play_state(struct mouse_ev *event);
int menu_multiplayer_state(struct mouse_ev *event);
int menu_online_state(struct mouse_ev *event);
int menu_instructions_state(struct mouse_ev *event);
int menu_exit_state(struct mouse_ev *event);

enum menu_state_codes { menu_entry,
                        menu_play,
                        instructions,
                        multiplayer,
                        online,
                        menu_end };

enum menu_ret_codes { menu_fail,
                      menu_repeat,
                      menu_back,
                      OP1,
                      OP2 };

typedef struct menu_transition {
  enum menu_state_codes src_state;
  enum menu_ret_codes ret_code;
  enum menu_state_codes dst_state;
} MENU_ST_TRANS;

enum menu_state_codes menu_lookup_transitions(int cur_state, int rc);

#define EXIT_MENU_STATE menu_end
#define ENTRY_MENU_STATE menu_entry


enum menu_buttons{  
  PLAY_BUTTON,
  INSTRUCTIONS_BUTTON,
  EXIT_BUTTON
};

bool isSelecting(enum menu_buttons button);


#endif
