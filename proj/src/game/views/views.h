#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_

#include <lcom/lcf.h>

#include "../../drivers/graphics/video_graphic.h"
#include "../../drivers/mouse/mouse.h"
#include "../objects/pieces.h"
#include "../state_machine/menu_st.h"
#include "font/font.h"
#include "sprite/sprite.h"

// =============== <XPM Includes> ===============

// Menus XPM
#include "../../assets/menus/base_bg.xpm"
#include "../../assets/menus/instructions_menu.xpm"
#include "../../assets/menus/play_menu.xpm"
#include "../../assets/menus/start_menu.xpm"
#include "../../assets/select.xpm"

// New buttons menus
#include "../../assets/menus/buttons/button_back_S.xpm"
#include "../../assets/menus/buttons/button_exit_NS.xpm"
#include "../../assets/menus/buttons/button_exit_S.xpm"
#include "../../assets/menus/buttons/button_instructions_S.xpm"
#include "../../assets/menus/buttons/button_multiplayer_S.xpm"
#include "../../assets/menus/buttons/button_online_S.xpm"
#include "../../assets/menus/buttons/button_play_S.xpm"

// =============== <Animation Sprites> ===============

#include "../../assets/animation/explosion.xpm"
#include "animation/animation.h"

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

sprite_t *explosion_sp;
AnimSprite *explosion;

// =============== <Defines> ===============

#define BOARD_START_POS_X 0
#define BOARD_START_POS_Y 0

#define BOARD_SCREEN_CASE_SIZE 94
#define BOARD_SCREEN_SIZE 800

#define BOARD_NUMBER_OF_CASES 64
#define BOARD_NUMBER_OF_COL 8
#define BOARD_NUMBER_OF_LIN 8

#define BOARD_BLACK_CASE_COLOR 0x769656
#define BOARD_WHITE_CASE_COLOR 0xeeeed2

#define GAME_MODE 1 // 0 --> ATOMIC MODE | 1 --> NORMAL CHESS

// =============== <Static Vars> ===============

// Menus
static sprite_t *bg_base;
static sprite_t *bg_start;
static sprite_t *bg_play;
static sprite_t *bg_instructions;

// Menu Buttons
static sprite_t *buton_back_S;
static sprite_t *buton_exit_NS;
static sprite_t *buton_exit_S;
static sprite_t *buton_instructions_S;
static sprite_t *buton_multiplayer_S;
static sprite_t *buton_online_S;
static sprite_t *buton_play_S;

// Game Sprites
static sprite_t *play_square_select;
static sprite_t *game_exit_sprite;

// Mouse Cursor Icon
static mouse_ptr cursor;

// =============== <Game Vars> ===============

static bool moves[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0}};

static bool isWhitesTurn = true;
static bool isWhiteInOnline = true;
static bool hasconnected = false;

static uint8_t select_lin = 0;
static uint8_t select_col = 0;

static int gameStateFlag = 0; // 0 -> playing | 1-> White Won | 2 -> Black Won

// =============== Game Clocks ===============

#define GAME_DURATION 300 // seconds => 5 min (300s)

static int white_clock = GAME_DURATION + 1;
static int black_clock = GAME_DURATION;

static int startTime;

void set_connected(bool isconnected);

// =============== <Funcs> ===============
/**
 * @brief Draws the Playing Board
 */
void draw_board();
/**
 * @brief Draws the pieces of a given board
 * @param table 8x8 Board received for game
 */
void draw_pieces(Board table[8][8]);
/**
 * @brief Draws a specific piece object in the specified coordinates
 * 
 * @param piece Piece to be drawn
 * @param x X Coordinate where the piece should be drawn
 * @param y Y Coordinate where the piece should be drawn
 */
void draw_piece(Board piece, unsigned int x, unsigned int y);
/**
 * @brief Draws Clock
 */
void draw_clock();
/**
 * @brief Get the selected valid moves object
 * @param arr Array where valid moves are to be stored
 */
void get_selected_valid_moves(bool arr[8][8]);
/**
 * @brief Checks if moving to (lin,col) is valid
 * @param lin Line for the destination position
 * @param col Column for the destination position
 * @return true if position is valid
 * @return false otherwise
 */
bool is_valid_move(int lin, int col);
/**
 * @brief Set the selected case object as (lin,col)
 * @param lin Line of the selected case to be set
 * @param col Column of the selected case to be set
 */
void set_selected_case(int lin, int col);
/**
 * @brief Check if (lin,col) is the selected case
 * @param lin Line of the selected case to be set
 * @param col Column of the selected case to be set
 * @return true if (lin,col) is the select case
 * @return false otherwise
 */
bool is_selected_case(int lin, int col);
void get_mouse_case(int m_y, int m_x, uint8_t *col, uint8_t *lin);
void move_piece(int lin, int col);
/**
 * @brief Moves piece from (i_line,i_col) to (f_line,f_col)
 * 
 * @param i_line Line where piece is located
 * @param i_col Column where piece is located
 * @param f_line Line where we want the piece to move
 * @param final_col Column where we want the piece to move
 */
void move_piece_from_to(uint8_t i_line, uint8_t i_col, uint8_t f_line, uint8_t final_col);

void set_up_view();
void free_view();

static Board board[BOARD_SIZE][BOARD_SIZE];
static Board empty_case;

static enum menu_state_codes game_cur_state = ENTRY_MENU_STATE;
static int kbd_selected_opt = 0;

void set_kbd_selected_opt(bool up);
int get_kbd_selected_opt();

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
void buttonHoverDraw(sprite_t *sprite, unsigned x, unsigned y, int rc);

#endif
