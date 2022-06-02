#include "views.h"

extern uint8_t rtc_data[6];
static int lookUpTable[] = {50, 144, 238, 332, 426, 520, 614, 708};

void draw_board() {
  for (size_t i = 0; i < BOARD_SIZE; i++)
    for (size_t j = 0; j < BOARD_SIZE; j++) {
      vg_draw_rectangle(lookUpTable[i], lookUpTable[j], BOARD_SCREEN_CASE_SIZE, BOARD_SCREEN_CASE_SIZE, get_square_color(j, i) == WHITE ? BOARD_WHITE_CASE_COLOR : BOARD_BLACK_CASE_COLOR);
    }
}

void draw_pieces(Board table[8][8]) { // remove table
  get_valid_moves(table, select_lin, select_col, moves, isWhitesTurn);
  for (size_t i = 0; i < BOARD_SIZE; i++) {
    for (size_t j = 0; j < BOARD_SIZE; j++) {
      if (table[i][j]->p_type != Blank_space) {
        draw_piece(table[i][j], j, i);
      }
      if (moves[i][j]) {
        set_sprite_pos(play_square_select, lookUpTable[j], lookUpTable[i]);
        draw_sprite_in_mode_14c(play_square_select);
      }
    }
  }
}

void draw_piece(Board piece, unsigned int x, unsigned int y) {
  if (piece == NULL) {
    return;
  }

  // Pawn Promotion
  if (piece->p_type == Pawn) {
    if (piece->color == BLACK && y == 7)
      board[y][x] = make_piece(xpm_bQ, Queen, BLACK);
    else if (piece->color == WHITE && y == 0)
      board[y][x] = make_piece(xpm_wQ, Queen, WHITE);
  }

  draw_piece_in_mode_14c(piece->map, lookUpTable[x], lookUpTable[y], BOARD_SCREEN_CASE_SIZE);
}

void draw_clock() {
  char temp[20];
  sprintf(temp, "%d:%d:%d  %d/%d/%d", rtc_data[2], rtc_data[1], rtc_data[0], rtc_data[3], rtc_data[4], rtc_data[5]);
  draw_text(temp, 200, 780, 0xFF88FF);
}

void get_selected_valid_moves(bool arr[8][8]) {
  get_valid_moves(board, select_lin, select_col, moves, isWhitesTurn);
  arr = moves;
}

bool is_valid_move(int lin, int col) {

  get_valid_moves(board, select_lin, select_col, moves, isWhitesTurn);
  return moves[lin][col];
}

bool is_selected_case(int lin, int col) {
  return lin == select_lin && col == select_col;
}

void set_selected_case(int lin, int col) {
  for (size_t i = 0; i < 8; i++)
    if (lin < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && lin > lookUpTable[i])
      select_lin = i;

  for (size_t i = 0; i < 8; i++)
    if (col < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && col > lookUpTable[i])
      select_col = i;
}

void get_mouse_case(int m_y, int m_x, uint8_t *col, uint8_t *lin) { // TODO: REPEATED CODE
  for (size_t i = 0; i < 8; i++)
    if (m_y < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && m_y > lookUpTable[i])
      *lin = i;

  for (size_t i = 0; i < 8; i++)
    if (m_x < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && m_x > lookUpTable[i])
      *col = i;
}

// Atomic mode control
void move_piece(int lin, int col) {

  isWhitesTurn = !isWhitesTurn;

  if (GAME_MODE) {
    Board sel_piece = board[select_lin][select_col];

    if (board[lin][col]->p_type == King) {
      gameStateFlag = board[lin][col]->color + 1;
    }

    board[lin][col] = sel_piece;

    board[select_lin][select_col] = empty_case;

    return;
  }

  Board sel_piece = board[select_lin][select_col];

  if (board[lin][col]->p_type != Blank_space) {
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (is_inside_board(lin + i, col + j) && board[lin + i][col + j]->p_type != Pawn) {
          if (board[lin + i][col + j]->p_type == King) {
            gameStateFlag = board[lin + i][col + j]->color + 1;
          }
          board[lin + i][col + j] = empty_case;
        }
      }
    }
    if (sel_piece->p_type == Pawn) {
      board[lin][col] = sel_piece;
    }
  }
  else {
    board[lin][col] = sel_piece;
  }
  board[select_lin][select_col] = empty_case;
}

void mouse_update_pos(int x, int y) {
  int nx = get_sprite_X(cursor) + x;
  int ny = get_sprite_Y(cursor) - y;
  set_sprite_X(cursor, (nx > 0 ? nx : get_hres() + nx) % get_hres());
  set_sprite_Y(cursor, (ny > 0 ? ny : get_vres() + ny) % get_vres());
}

int get_cursor_X() {
  return get_sprite_X(cursor);
}

int get_cursor_Y() {
  return get_sprite_Y(cursor);
}

void draw_bg(sprite_t *sprite) {
  draw_sprite_in_mode_14c(sprite);
}

void draw_cursor() {
  draw_sprite_in_mode_14c(cursor);
}

void draw_sprite(const char *xpm[], int x, int y) {
  sprite_t *sprite = make_sprite(xpm, XPM_8_8_8_8);
  set_sprite_pos(sprite, x, y);
  draw_sprite_in_mode_14c(sprite);
  free_sprite(sprite);
}

void draw_menu() {
  switch (game_cur_state) {
    case menu_entry:

      draw_bg(bg_start);
      draw_clock();
      break;
    case menu_play:

      draw_bg(bg_play);

      break;
    case instructions:

      draw_bg(bg_instructions);

      break;
    case multiplayer:
      draw_bg(bg_base);
      draw_board();
      draw_pieces(board);
      draw_game_clock();
      // draw_sprite_in_mode_14c(game_exit_sprite);
      if (gameStateFlag == 1) {
        vg_draw_rectangle(240, 290, 320, 220, 0);
        draw_text("WHITE", 300, 300, 0x00ffff);
        draw_text(" WON", 300, 400, 0x00ffff);
      }
      else if (gameStateFlag == 2) {
        vg_draw_rectangle(240, 290, 320, 220, 0);
        draw_text("BLACK", 300, 300, 0xff00ff);
        draw_text(" WON", 300, 400, 0xff00ff);
      }

      break;
    case online:
      draw_bg(bg_base);
      draw_board();
      draw_pieces(board);
      draw_game_clock();
      // draw_sprite_in_mode_14c(game_exit_sprite);
      if (gameStateFlag == 1) {
        vg_draw_rectangle(240, 290, 320, 220, 0);
        draw_text("WHITE", 300, 300, 0x00ffff);
        draw_text(" WON", 300, 400, 0x00ffff);
      }
      else if (gameStateFlag == 2) {
        vg_draw_rectangle(240, 290, 320, 220, 0);
        draw_text("BLACK", 300, 300, 0xff00ff);
        draw_text(" WON", 300, 400, 0xff00ff);
      }
      break;
    case menu_end:
      break;
    default:
      break;
  }
}

void game_set_state(enum menu_state_codes state) {
  game_cur_state = state;
}

void draw_update() {
  draw_menu();
  draw_cursor();
}

void set_up_view() {

  bg_start = make_sprite(xpm_menu_start, XPM_8_8_8_8);
  bg_play = make_sprite(xpm_menu_play, XPM_8_8_8_8);
  bg_instructions = make_sprite(xpm_menu_instructions, XPM_8_8_8_8);
  bg_base = make_sprite(xpm_base_bg, XPM_8_8_8_8);

  cursor = make_sprite(xpm_cursor, XPM_8_8_8_8);
  play_square_select = make_sprite(xpm_play_select, XPM_8_8_8_8);

  // ==================================================================
  game_exit_sprite = make_sprite(back_b_xpm, XPM_8_8_8_8);
  game_win = make_sprite(back_b_xpm, XPM_8_8_8_8);
  game_lose = make_sprite(back_b_xpm, XPM_8_8_8_8);

  set_sprite_pos(game_exit_sprite, 850, 750);
  set_sprite_pos(game_win, 800, 200);
  set_sprite_pos(game_lose, 800, 400);
  // ==================================================================

  set_sprite_pos(cursor, 200, 200);

  set_sprite_pos(bg_start, 0, 0);
  set_sprite_pos(bg_instructions, 0, 0);
  set_sprite_pos(bg_base, 0, 0);

  draw_sprite_in_mode_14c(bg_start);
  draw_sprite_in_mode_14c(cursor);
}

void free_view() {
  free_sprite(cursor);
  free_sprite(bg_base);
  free_sprite(bg_start);
  free_sprite(bg_play);
  free_sprite(bg_instructions);
}

void set_up_board() {

  empty_case = make_piece(NULL, Blank_space, BLACK);
  board[0][0] = make_piece(xpm_bR, Rook, BLACK);
  board[0][1] = make_piece(xpm_bN, Knight, BLACK);
  board[0][2] = make_piece(xpm_bB, Bishop, BLACK);
  board[0][3] = make_piece(xpm_bQ, Queen, BLACK);
  board[0][4] = make_piece(xpm_bK, King, BLACK);
  board[0][5] = make_piece(xpm_bB, Bishop, BLACK);
  board[0][6] = make_piece(xpm_bN, Knight, BLACK);

  board[0][7] = make_piece(xpm_bR, Rook, BLACK);
  board[1][0] = make_piece(xpm_bP, Pawn, BLACK);
  board[1][1] = make_piece(xpm_bP, Pawn, BLACK);
  board[1][2] = make_piece(xpm_bP, Pawn, BLACK);
  board[1][3] = make_piece(xpm_bP, Pawn, BLACK);
  board[1][4] = make_piece(xpm_bP, Pawn, BLACK);
  board[1][5] = make_piece(xpm_bP, Pawn, BLACK);
  board[1][6] = make_piece(xpm_bP, Pawn, BLACK);
  board[1][7] = make_piece(xpm_bP, Pawn, BLACK);

  board[2][0] = empty_case;
  board[2][1] = empty_case;
  board[2][2] = empty_case;
  board[2][3] = empty_case;
  board[2][4] = empty_case;
  board[2][5] = empty_case;
  board[2][6] = empty_case;
  board[2][7] = empty_case;
  board[3][0] = empty_case;
  board[3][1] = empty_case;
  board[3][2] = empty_case;
  board[3][3] = empty_case;
  board[3][4] = empty_case;
  board[3][5] = empty_case;
  board[3][6] = empty_case;
  board[3][7] = empty_case;
  board[4][0] = empty_case;
  board[4][1] = empty_case;
  board[4][2] = empty_case;
  board[4][3] = empty_case;
  board[4][4] = empty_case;
  board[4][5] = empty_case;
  board[4][6] = empty_case;
  board[4][7] = empty_case;
  board[5][0] = empty_case;
  board[5][1] = empty_case;
  board[5][2] = empty_case;
  board[5][3] = empty_case;
  board[5][4] = empty_case;
  board[5][5] = empty_case;
  board[5][6] = empty_case;
  board[5][7] = empty_case;

  board[6][0] = make_piece(xpm_wP, Pawn, WHITE);
  board[6][1] = make_piece(xpm_wP, Pawn, WHITE);
  board[6][2] = make_piece(xpm_wP, Pawn, WHITE);
  board[6][3] = make_piece(xpm_wP, Pawn, WHITE);
  board[6][4] = make_piece(xpm_wP, Pawn, WHITE);
  board[6][5] = make_piece(xpm_wP, Pawn, WHITE);
  board[6][6] = make_piece(xpm_wP, Pawn, WHITE);
  board[6][7] = make_piece(xpm_wP, Pawn, WHITE);

  board[7][0] = make_piece(xpm_wR, Rook, WHITE);
  board[7][1] = make_piece(xpm_wN, Knight, WHITE);
  board[7][2] = make_piece(xpm_wB, Bishop, WHITE);
  board[7][3] = make_piece(xpm_wQ, Queen, WHITE);
  board[7][4] = make_piece(xpm_wK, King, WHITE);
  board[7][5] = make_piece(xpm_wB, Bishop, WHITE);
  board[7][6] = make_piece(xpm_wN, Knight, WHITE);
  board[7][7] = make_piece(xpm_wR, Rook, WHITE);
}

void free_board() {
  for (size_t i = 0; i < BOARD_SIZE; i++)
    for (size_t j = 0; j < BOARD_SIZE; j++)
      free_piece(board[i][j]);

  isWhitesTurn = true;
  gameStateFlag = 0;
}

void move_piece_from_to(uint8_t i_line, uint8_t i_col, uint8_t f_line, uint8_t f_col) {

  Board sel_piece = board[i_line][i_col];
  isWhitesTurn = !isWhitesTurn;

  if (GAME_MODE) {
    if (board[f_line][f_col]->p_type == King) {
      gameStateFlag = board[f_line][f_col]->color + 1;
    }

    board[f_line][f_col] = sel_piece;

    board[i_line][i_col] = empty_case;

    return;
  }

  if (board[f_line][f_col]->p_type != Blank_space) {
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (is_inside_board(f_line + i, f_col + j) && board[f_line + i][f_col + j]->p_type != Pawn) {
          if (board[f_line + i][f_col + j]->p_type == King) {
            gameStateFlag = board[f_line + i][f_col + j]->color + 1;
          }
          board[f_line + i][f_col + j] = empty_case;
        }
      }
    }
    if (sel_piece->p_type == Pawn) {
      board[f_line][f_col] = sel_piece;
    }
  }
  else {
    board[f_line][f_col] = sel_piece;
  }
  board[i_line][i_col] = empty_case;
}

uint8_t get_selected_col() {
  return select_col;
}
uint8_t get_selected_lin() {
  return select_lin;
}

// ============================ Game Clocks ============================

#define GAME_DURATION 300 // seconds => 5 min (300s)

static int white_clock = GAME_DURATION + 2;
static int black_clock = GAME_DURATION;

static int startTime;

int getCurrentTime() {
  return rtc_data[2] * 60 * 60 + rtc_data[1] * 60 + rtc_data[0];
}

void setStartTime() {
  startTime = getCurrentTime();
}

void updateTimer(bool white) {

  if (gameStateFlag)
    return;

  if (white_clock * black_clock == 0)
    return;

  if (getCurrentTime() - startTime > 0) {

    if (white)
      white_clock--;
    else
      black_clock--;

    setStartTime();
  }

  if (white_clock == 0)
    gameStateFlag = 2;

  if (black_clock == 0)
    gameStateFlag = 1;
}

void draw_game_clock() {

  updateTimer(isWhitesTurn);

  /*         White Clock        */

  int min = white_clock / 60;
  int sec = white_clock - min * 60;

  char temp[10];
  if (sec < 10)
    sprintf(temp, "W %d:0%d ", min, sec);
  else
    sprintf(temp, "W %d:%d ", min, sec);
  draw_text(temp, 80, 785, 0xFF88FF);

  /*         Black Clock        */

  min = black_clock / 60;
  sec = black_clock - min * 60;

  if (sec < 10)
    sprintf(temp, "B %d:0%d ", min, sec);
  else
    sprintf(temp, "B %d:%d ", min, sec);
  draw_text(temp, 500, 785, 0xFF88FF);
}

void set_online_color(bool isWhite) {
  isWhiteInOnline = isWhite;
}

bool get_online_color() {
  return isWhiteInOnline;
}

Piece_Color get_piece_at_pos_color(uint8_t lin, uint8_t col) {
  return board[lin][col]->color;
}