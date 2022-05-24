#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_

#include "../../drivers/graph/video_graphic.h"
#include "../objects/board/board.h"
#include "../objects/pieces/pieces.h"
#include "../sprite/sprite.h"
#include "../state_machine/menu_st.h"
#include "../../drivers/mouse/mouse.h"
#include "font.h"

// Menus XPM
#include "../../assets/menus/play_menu.xpm"
#include "../../assets/menus/instructions_menu.xpm"
#include "../../assets/menus/start_menu.xpm"
#include "../../assets/menus/base_bg.xpm"

// Pieces XPM
#include "../../assets/pieces/bB.xpm"
#include "../../assets/pieces/wB.xpm"
#include "../../assets/pieces/bK.xpm"
#include "../../assets/pieces/wK.xpm"
#include "../../assets/pieces/bN.xpm"
#include "../../assets/pieces/wN.xpm"
#include "../../assets/pieces/bP.xpm"
#include "../../assets/pieces/wP.xpm"
#include "../../assets/pieces/bQ.xpm"
#include "../../assets/pieces/wQ.xpm"
#include "../../assets/pieces/bR.xpm"
#include "../../assets/pieces/wR.xpm"


#include <lcom/lcf.h>

#define BOARD_START_POS_X 0
#define BOARD_START_POS_Y 0

#define BOARD_SCREEN_CASE_SIZE 94
#define BOARD_SCREEN_SIZE 800

#define BOARD_NUMBER_OF_CASES 64
#define BOARD_NUMBER_OF_COL 8
#define BOARD_NUMBER_OF_LIN 8

#define BOARD_BLACK_CASE_COLOR 0x769656
#define BOARD_WHITE_CASE_COLOR 0xeeeed2

static bool moves[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0}};

static int lookUpTable[] = {
    0, 94, 188, 282, 376, 470, 564, 658};

static uint8_t select_lin = 0;
static uint8_t select_col = 0;

void draw_board();
void draw_pieces(Board table[8][8]);
void draw_piece(Board piece, unsigned int x, unsigned int y);
void draw_clock();

void get_selected_valid_moves(bool arr[8][8]);
bool is_valid_move(int lin, int col);
void set_selected_case(int lin, int col);
void get_mouse_case(int m_y, int m_x, uint8_t *col, uint8_t *lin);
void move_piece(int lin, int col);

void set_up_view();
void free_view();

static mouse_ptr cursor;

static sprite_t *bg_base;
static sprite_t *bg_start;
static sprite_t *bg_play;
static sprite_t *bg_instructions;


static Board board[BOARD_SIZE][BOARD_SIZE];
static Board empty_case;

static enum menu_state_codes game_cur_state = ENTRY_MENU_STATE;

int get_cursor_X();
int get_cursor_Y();

void draw_update();

void game_set_state(enum menu_state_codes state);

void mouse_update_pos(int x, int y);


void set_up_board();

#endif
