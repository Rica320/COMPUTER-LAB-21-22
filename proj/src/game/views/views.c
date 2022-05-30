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
  get_valid_moves(table, select_lin, select_col, moves);
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

  

  draw_piece_in_mode_14c(piece->map, lookUpTable[x], lookUpTable[y], BOARD_SCREEN_CASE_SIZE);
}

void draw_clock() {
  char temp[20];
  sprintf(temp, "%d:%d:%d  %d/%d/%d", rtc_data[2], rtc_data[1], rtc_data[0], rtc_data[3], rtc_data[4], rtc_data[5]);
  draw_text(temp, 200, 780, 0xFF88FF);
}

void get_selected_valid_moves(bool arr[8][8]) {
  get_valid_moves(board, select_lin, select_col, moves);
  arr = moves;
}

bool is_valid_move(int lin, int col) {
  get_valid_moves(board, select_lin, select_col, moves);
  return moves[lin][col];
}

bool is_selected_case(int lin, int col) {
  return lin == select_lin && col == select_col;
}

void set_selected_case(int lin, int col) {
  for (size_t i = 0; i < 8; i++) {
    if (lin < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && lin > lookUpTable[i]) {
      select_lin = i;
    }
  }

  for (size_t i = 0; i < 8; i++) {
    if (col < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && col > lookUpTable[i]) {
      select_col = i;
    }
  }
}

void get_mouse_case(int m_y, int m_x, uint8_t *col, uint8_t *lin) { // TODO: REPEATED CODE
  for (size_t i = 0; i < 8; i++) {
    if (m_y < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && m_y > lookUpTable[i]) {
      *lin = i;
    }
  }

  for (size_t i = 0; i < 8; i++) {
    if (m_x < lookUpTable[i] + BOARD_SCREEN_CASE_SIZE && m_x > lookUpTable[i]) {
      *col = i;
    }
  }
}

void move_piece(int lin, int col) {
  Board sel_piece = board[select_lin][select_col];

  board[lin][col] = sel_piece;

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

      break;
    case online:
      draw_board();
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
  flush_screen();
}

void set_up_view() {

  bg_start = make_sprite(xpm_menu_start, XPM_8_8_8_8);
  bg_play = make_sprite(xpm_menu_play, XPM_8_8_8_8);
  bg_instructions = make_sprite(xpm_menu_instructions, XPM_8_8_8_8);
  bg_base = make_sprite(xpm_base_bg, XPM_8_8_8_8);

  cursor = make_sprite(xpm_cursor, XPM_8_8_8_8);
  play_square_select = make_sprite(xpm_play_select, XPM_8_8_8_8);

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
