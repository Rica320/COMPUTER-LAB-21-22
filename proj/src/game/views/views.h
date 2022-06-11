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

static Board board[BOARD_SIZE][BOARD_SIZE];
static Board empty_case;

static enum menu_state_codes game_cur_state = ENTRY_MENU_STATE;
static int kbd_selected_opt = 0;

// =============== <Funcs> ===============

/**
 * @brief Set the connected boolean in online gaming
 *
 * @param isconnected new value to be set
 */
void set_connected(bool isconnected);

/**
 * @brief Draws the chess board
 */
void draw_board();

/**
 * @brief Draws the pieces on the given board
 * @param table 8x8 Board received for chess game
 */
void draw_pieces(Board table[8][8]);

/**
 * @brief Draws a specific piece object in the specified coordinates
 *
 * @param piece chess piece to be drawn
 * @param x X Coordinate where the piece should be drawn
 * @param y Y Coordinate where the piece should be drawn
 */
void draw_piece(Board piece, unsigned int x, unsigned int y);

/**
 * @brief Draws RTC Clock in the Main menu
 */
void draw_clock();

/**
 * @brief Get the selected valid moves and returns them in the given matrix
 * @param arr matrix where valid moves should be stored
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

/**
 * @brief Gets the mouse current hovering piece/ board square
 *
 * @param m_y mouse y cord
 * @param m_x mouse x cord
 * @param col board hovered col by the mouse
 * @param lin board hovered lin by the mouse
 */
void get_mouse_case(int m_y, int m_x, uint8_t *col, uint8_t *lin);

/**
 * @brief Move selected piece to given cords
 *
 * @param lin lin where to move selected piece
 * @param col col where to move selected piece
 */
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

/**
 * @brief Memory allocation for essential game sprites and positions
 *
 */
void set_up_view();

/**
 * @brief Frees all allocated memory of the game sprites
 *
 */
void free_view();

/**
 * @brief Move up kbd menu pointed option
 *
 * @param up if menu option should be changed
 */
void set_kbd_selected_opt(bool up);

/**
 * @brief Get the kbd selected menu option
 *
 * @return int index of the transition state table
 */
int get_kbd_selected_opt();

/**
 * @brief Get the cursor X pos
 *
 * @return int mouse x pos
 */
int get_cursor_X();

/**
 * @brief Get the cursor Y pos
 *
 * @return int mouse y pos
 */
int get_cursor_Y();

/**
 * @brief Get the selected col in the chess board
 *
 * @return uint8_t number of the selected col
 */
uint8_t get_selected_col();

/**
 * @brief Get the selected lin in the chess board
 *
 * @return uint8_t number of the selected lin
 */
uint8_t get_selected_lin();

/**
 * @brief Draws all game components. That includes the board, pieces
 * timers, available positions and respective animations
 *
 * @param startClock boolean if chess game clocks should be ticking
 */
void draw_game(bool startClock);

/**
 * @brief Updates the frame by drawing a new menu frame and
 * the new mouse cursor position
 *
 */
void draw_update();

/**
 * @brief Updates the game state to the given state
 *
 * @param state new game state to be set
 */
void game_set_state(enum menu_state_codes state);

/**
 * @brief Update mouse cords to the given x,y position
 *
 * @param x new mouse x cord
 * @param y new mouse y cord
 */
void mouse_update_pos(int x, int y);

/**
 * @brief Allocates all pieces animation sprites and sets
 * their position on the chess board
 *
 */
void set_up_board();

/**
 * @brief Frees all memory allocated for the animated pieces
 *
 */
void free_board();

/**
 * @brief Draws both W and B game clocks current time
 *
 * @param game_started boolean if the times should be updated
 */
void draw_game_clock(bool game_started);

/**
 * @brief Updates the game timer based on RTC second changes
 *
 * @param white boolean if white's clock should be updated. False updates Black's clock
 */
void update_timer(bool white);

/**
 * @brief Set the online color for the current player's turn
 *
 * @param isWhite boolean if is white's turn to play
 */
void set_online_color(bool isWhite);

/**
 * @brief Get the online color for the current player's turn
 *
 * @return true is white's turn to play in online mode
 * @return false is black's turn to play in online mode
 */
bool get_online_color();

/**
 * @brief Get the piece's color at the given positon on the game board
 *
 * @param lin line where to look the color
 * @param col collumn where to look the color
 * @return Piece_Color color of the piece in the given pos
 */
Piece_Color get_piece_at_pos_color(uint8_t lin, uint8_t col);

/**
 * @brief Draws the given sprite in the given position
 * if the mouse is hover its drawn position or the kbd selected that button
 * position in the game menus
 *
 * @param sprite button sprite to be drawn
 * @param x where to draw X
 * @param y where to draw Y
 * @param rc kbd currently selected button in the menu
 */
void buttonHoverDraw(sprite_t *sprite, unsigned x, unsigned y, int rc);

#endif
