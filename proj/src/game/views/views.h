#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_

#include "../../drivers/graph/video_graphic.h"
#include "../../drivers/mouse/mouse.h"
#include "../objects/pieces/pieces.h"
#include "../sprite/sprite.h"
#include "../state_machine/menu_st.h"
#include "font.h"

// Menus XPM
#include "../../assets/menus/Back.xpm"
#include "../../assets/menus/base_bg.xpm"
#include "../../assets/menus/instructions_menu.xpm"
#include "../../assets/menus/play_menu.xpm"
#include "../../assets/menus/start_menu.xpm"
#include "../../assets/pieces/select.xpm"

// New buttons menus
#include "../../assets/menus/buttons/button_back_S.xpm"
#include "../../assets/menus/buttons/button_exit_NS.xpm"
#include "../../assets/menus/buttons/button_exit_S.xpm"
#include "../../assets/menus/buttons/button_instructions_S.xpm"
#include "../../assets/menus/buttons/button_multiplayer_S.xpm"
#include "../../assets/menus/buttons/button_online_S.xpm"
#include "../../assets/menus/buttons/button_play_S.xpm"

// ===================== Animation Sprites =============================
#include "../../assets/animation/explosion.xpm"
#include "animation.h"

sprite_t *explosion_sp;
AnimSprite *explosion;

// Pieces XPM
#include "../../assets/animation/pieces/bB.xpm"
#include "../../assets/animation/pieces/bK.xpm"
#include "../../assets/animation/pieces/bN.xpm"
#include "../../assets/animation/pieces/bP.xpm"
#include "../../assets/animation/pieces/bQ.xpm"
#include "../../assets/animation/pieces/bR.xpm"
#include "../../assets/animation/pieces/wB.xpm"
#include "../../assets/animation/pieces/wK.xpm"
#include "../../assets/animation/pieces/wN.xpm"
#include "../../assets/animation/pieces/wP.xpm"
#include "../../assets/animation/pieces/wQ.xpm"
#include "../../assets/animation/pieces/wR.xpm"

// ===================== \Animation Sprites =============================

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

static uint8_t select_lin = 0;
static uint8_t select_col = 0;

#define GAME_MODE 1           // 0 --> ATOMIC MODE | 1 --> NORMAL CHESS
static int gameStateFlag = 0; // 0 -> playing | 1-> White Won | 2 -> Black Won
static bool isWhitesTurn = true;
static bool isWhiteInOnline = true;
static bool hasconnected = false;

void draw_board();
void draw_pieces(Board table[8][8]);
void draw_piece(Board piece, unsigned int x, unsigned int y);
void draw_clock();

void get_selected_valid_moves(bool arr[8][8]);
bool is_valid_move(int lin, int col);
void set_selected_case(int lin, int col);
bool is_selected_case(int lin, int col);
void get_mouse_case(int m_y, int m_x, uint8_t *col, uint8_t *lin);
void move_piece(int lin, int col);
void move_piece_from_to(uint8_t i_line, uint8_t i_col, uint8_t f_line, uint8_t final_col);

void set_up_view();
void free_view();

static mouse_ptr cursor;

// ========================== Menus ==========================

static sprite_t *bg_base;
static sprite_t *bg_start;
static sprite_t *bg_play;
static sprite_t *bg_instructions;

// ========================== Menus Buttons ==========================

static sprite_t *buton_back_S;
static sprite_t *buton_exit_NS;
static sprite_t *buton_exit_S;
static sprite_t *buton_instructions_S;
static sprite_t *buton_multiplayer_S;
static sprite_t *buton_online_S;
static sprite_t *buton_play_S;

// ------------------------------------------------

static sprite_t *play_square_select;
static sprite_t *game_exit_sprite;
static sprite_t *game_win;
static sprite_t *game_lose;

static Board board[BOARD_SIZE][BOARD_SIZE];
static Board empty_case;

static enum menu_state_codes game_cur_state = ENTRY_MENU_STATE;

int get_cursor_X();
int get_cursor_Y();
uint8_t get_selected_col();
uint8_t get_selected_lin();

void draw_game(bool startClock);
void draw_update();

void game_set_state(enum menu_state_codes state);

void mouse_update_pos(int x, int y);

void set_up_board();
void free_board();

void draw_game_clock(bool game_started);
void update_timer(bool white);
void set_online_color(bool isWhite);
bool get_online_color();
Piece_Color get_piece_at_pos_color(uint8_t lin, uint8_t col);

// ============================ Game Clocks ============================

#define GAME_DURATION 300 // seconds => 5 min (300s)

static int white_clock = GAME_DURATION + 1;
static int black_clock = GAME_DURATION;

static int startTime;

int get_current_time();
void set_start_time();
void set_connected(bool isconnected);

#endif
