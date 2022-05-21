#ifndef MAIN_H
#define MAIN_H

#include <lcom/lcf.h>

#include "../assets/menu_bg.h"
#include "../drivers/graph/video_graphic.h"
#include "../drivers/kbc/i8042.h"
#include "../drivers/keyboard/kbd_keys.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "int_handlers/dependent/dependent_ih.h"
#include "int_handlers/independent/independent_ih.h"
#include "sprite/sprite.h"
#include "state_machine/mouse_state.h"
#include "views/views.h"
#include "views/font.h"
#include "objects/board/board.h"
#include "../assets/wB.xpm"

// include assets
#include "../assets/Back.xpm"
#include "../assets/Exit.xpm"
#include "../assets/Instructions.xpm"
#include "../assets/Multiplayer.xpm"
#include "../assets/Online.xpm"
#include "../assets/Play.xpm"
#include "../assets/menu_bg.h"



static mouse_ptr cursor;
static sprite_t *menu_img;

static Board board[BOARD_SIZE][BOARD_SIZE];

static enum menu_state_codes game_cur_state = ENTRY_MENU_STATE;

int get_cursor_X();

int get_cursor_Y();

void game_loop();

void draw_update();

void mouse_update_pos(int x, int y);

void set_up_board();

void game_set_state(enum menu_state_codes state);



#endif
