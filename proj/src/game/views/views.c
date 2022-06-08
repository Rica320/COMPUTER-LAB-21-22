#include "views.h"

extern uint8_t rtc_data[6];
static int lookUpTable[] = {50, 144, 238, 332, 426, 520, 614, 708};

// Para animar peças
extern uint32_t n_interrupts;
uint8_t animation_counter = 0;

// Para animar explosoes
bool isExploding;
uint8_t count_exploding = 10;
unsigned exploding_x, exploding_y;

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
    if (piece->color == BLACK && y == 7) {
      sprite_t *sp = make_sprite(xpm_bQ, XPM_8_8_8_8);
      AnimSprite *ani_sp = create_animSprite(sp, 12, 6, 94, 94);
      board[y][x] = make_piece(ani_sp, Queen, BLACK);
    }
    else if (piece->color == WHITE && y == 0) {
      sprite_t *sp = make_sprite(xpm_wQ, XPM_8_8_8_8);
      AnimSprite *ani_sp = create_animSprite(sp, 12, 6, 94, 94);
      board[y][x] = make_piece(ani_sp, Queen, WHITE);
    }
  }

  // draw_piece_in_mode_14c(piece->map, lookUpTable[x], lookUpTable[y], BOARD_SCREEN_CASE_SIZE);
  draw_animSprite(piece->animSprite, animation_counter % piece->animSprite->num_fig + 1, lookUpTable[x], lookUpTable[y]);
}

void draw_clock() {
  char temp[20];
  sprintf(temp, "%02d:%02d:%02d  %02d/%02d/%02d", rtc_data[2], rtc_data[1], rtc_data[0], rtc_data[3], rtc_data[4], rtc_data[5]);
  draw_text(temp, 230, 780, 0xFFFFFF, false);
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
  Board sel_piece = board[select_lin][select_col];

  // Normal Mode
  if (GAME_MODE) {

    if (board[lin][col]->p_type == King) {
      gameStateFlag = board[lin][col]->color + 1;
    }

    // TODO we should free the mem of the eaten piece
    if (board[lin][col]->p_type != Blank_space) {
      exploding_x = lookUpTable[col];
      exploding_y = lookUpTable[lin];
      count_exploding = 0;
      isExploding = true;
    }

    board[lin][col] = sel_piece;

    board[select_lin][select_col] = empty_case;

    return;
  }

  // Atomic Mode
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

void draw_sprite(sprite_t *sprite, int x, int y) {
  set_sprite_pos(sprite, x, y);
  draw_sprite_in_mode_14c(sprite);
}

void buttonHoverDraw(sprite_t *sprite, unsigned x, unsigned y, int rc) {
  if ((cursor->x > x && cursor->x < x + 270 && cursor->y > y && cursor->y < y + 75) || rc == kbd_selected_opt)
    draw_sprite(sprite, x, y);
}

void draw_menu() {
  switch (game_cur_state) {
    case menu_entry:

      draw_bg(bg_start);
      draw_clock();

      buttonHoverDraw(buton_play_S, 441, 259, OP1);
      buttonHoverDraw(buton_instructions_S, 441, 394, OP2);
      buttonHoverDraw(buton_exit_S, 441, 529, menu_back);

      break;
    case menu_play:

      draw_bg(bg_play);

      buttonHoverDraw(buton_multiplayer_S, 441, 194, OP1);
      buttonHoverDraw(buton_online_S, 441, 329, OP2);
      buttonHoverDraw(buton_back_S, 441, 464, menu_back);
      buttonHoverDraw(buton_exit_S, 441, 599, OP3);

      break;
    case instructions:

      draw_bg(bg_instructions);

      break;
    case multiplayer:

      draw_game(true);

      break;
    case online:

      draw_game(hasconnected);

      break;
    case menu_end:
      break;
    default:
      break;
  }
}

void draw_game(bool startClock) {

  // increment the animation counter while game doesnt end
  if (!gameStateFlag)
    if (n_interrupts % 3 == 0)
      animation_counter++;

  // draw main elements
  draw_bg(bg_base);
  draw_board();
  draw_pieces(board);
  draw_game_clock(startClock);

  // draw explosion animattion when taking a piece
  if (n_interrupts % 2 == 0)
    if (count_exploding < 10)
      draw_animSprite(explosion, count_exploding++ % explosion->num_fig + 1, exploding_x, exploding_y);

  if (isWhitesTurn)
    vg_draw_rectangle(10, 700, 30, 30, 0xFFFFFF);
  else
    vg_draw_rectangle(10, 120, 30, 30, 0xFFFFFF);

  draw_sprite(buton_exit_NS, 845, 770);
  buttonHoverDraw(buton_exit_S, 845, 770, OP1);

  // draw_sprite_in_mode_14c(game_exit_sprite);
  if (gameStateFlag == 1) {
    vg_draw_rectangle(240, 290, 320, 220, 0);
    draw_text("WHITE", 300, 300, 0xFFFFFF, false);
    draw_text(" WON", 300, 400, 0xFFFFFF, false);
  }
  else if (gameStateFlag == 2) {
    vg_draw_rectangle(240, 290, 320, 220, 0);
    draw_text("BLACK", 300, 300, 0xFFFFFF, false);
    draw_text(" WON", 300, 400, 0xFFFFFF, false);
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

  // ==================================================================

  buton_back_S = make_sprite(button_back_S_xpm, XPM_8_8_8_8);
  buton_exit_S = make_sprite(button_exit_S_xpm, XPM_8_8_8_8);
  buton_instructions_S = make_sprite(button_instructions_S_xpm, XPM_8_8_8_8);
  buton_multiplayer_S = make_sprite(button_multiplayer_S_xpm, XPM_8_8_8_8);
  buton_online_S = make_sprite(button_online_S_xpm, XPM_8_8_8_8);
  buton_play_S = make_sprite(button_play_S_xpm, XPM_8_8_8_8);
  buton_exit_NS = make_sprite(button_exit_NS_xpm, XPM_8_8_8_8);

  explosion_sp = make_sprite(xpm_explosion, XPM_8_8_8_8);
  explosion = create_animSprite(explosion_sp, 10, 5, 94, 94);
}

void free_view() {
  free_sprite(cursor);
  free_sprite(bg_base);
  free_sprite(bg_start);
  free_sprite(bg_play);
  free_sprite(bg_instructions);
}

void set_up_board() {

  white_clock = GAME_DURATION + 1;
  black_clock = GAME_DURATION;

  sprite_t *sp;
  AnimSprite *ani_sp;

  empty_case = make_piece(NULL, Blank_space, BLACK);

  // -------------------------------------------

  sp = make_sprite(xpm_bR, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 16, 8, 94, 94);

  board[0][0] = make_piece(ani_sp, Rook, BLACK);
  board[0][7] = make_piece(ani_sp, Rook, BLACK);

  // -------------------------------------------

  sp = make_sprite(xpm_bB, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 18, 9, 94, 94);

  board[0][2] = make_piece(ani_sp, Bishop, BLACK);
  board[0][5] = make_piece(ani_sp, Bishop, BLACK);

  // -------------------------------------------

  sp = make_sprite(xpm_bQ, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 12, 6, 94, 94);

  board[0][3] = make_piece(ani_sp, Queen, BLACK);

  // -------------------------------------------

  sp = make_sprite(xpm_bK, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 18, 9, 94, 94);

  board[0][4] = make_piece(ani_sp, King, BLACK);

  // -------------------------------------------

  sp = make_sprite(xpm_bN, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 12, 6, 94, 94);

  board[0][6] = make_piece(ani_sp, Knight, BLACK);
  board[0][1] = make_piece(ani_sp, Knight, BLACK);

  // -------------------------------------------

  sp = make_sprite(xpm_bP, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 12, 6, 94, 94);

  board[1][0] = make_piece(ani_sp, Pawn, BLACK);
  board[1][1] = make_piece(ani_sp, Pawn, BLACK);
  board[1][2] = make_piece(ani_sp, Pawn, BLACK);
  board[1][3] = make_piece(ani_sp, Pawn, BLACK);
  board[1][4] = make_piece(ani_sp, Pawn, BLACK);
  board[1][5] = make_piece(ani_sp, Pawn, BLACK);
  board[1][6] = make_piece(ani_sp, Pawn, BLACK);
  board[1][7] = make_piece(ani_sp, Pawn, BLACK);

  // -------------------------------------------

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

  // -------------------------------------------

  sp = make_sprite(xpm_wP, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 12, 6, 94, 94);

  board[6][0] = make_piece(ani_sp, Pawn, WHITE);
  board[6][1] = make_piece(ani_sp, Pawn, WHITE);
  board[6][2] = make_piece(ani_sp, Pawn, WHITE);
  board[6][3] = make_piece(ani_sp, Pawn, WHITE);
  board[6][4] = make_piece(ani_sp, Pawn, WHITE);
  board[6][5] = make_piece(ani_sp, Pawn, WHITE);
  board[6][6] = make_piece(ani_sp, Pawn, WHITE);
  board[6][7] = make_piece(ani_sp, Pawn, WHITE);

  // -------------------------------------------

  sp = make_sprite(xpm_wR, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 18, 9, 94, 94);

  board[7][0] = make_piece(ani_sp, Rook, WHITE);
  board[7][7] = make_piece(ani_sp, Rook, WHITE);

  // -------------------------------------------

  sp = make_sprite(xpm_wN, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 12, 6, 94, 94);

  board[7][6] = make_piece(ani_sp, Knight, WHITE);
  board[7][1] = make_piece(ani_sp, Knight, WHITE);

  // -------------------------------------------

  sp = make_sprite(xpm_wB, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 18, 9, 94, 94);

  board[7][5] = make_piece(ani_sp, Bishop, WHITE);
  board[7][2] = make_piece(ani_sp, Bishop, WHITE);

  // -------------------------------------------

  sp = make_sprite(xpm_wQ, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 12, 6, 94, 94);

  board[7][3] = make_piece(ani_sp, Queen, WHITE);

  // -------------------------------------------

  sp = make_sprite(xpm_wK, XPM_8_8_8_8);
  ani_sp = create_animSprite(sp, 18, 9, 94, 94);

  board[7][4] = make_piece(ani_sp, King, WHITE);
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

static int startTime;

void updateTimer(bool white) {

  // if game ended, stop moving clock
  if (gameStateFlag)
    return;

  // if second has passed
  if (rtc_data[0] - startTime) {

    // set the new second time
    startTime = rtc_data[0];

    // decrement the current player's turn clock
    if (white)
      white_clock--;
    else
      black_clock--;
  }

  // if timer reached zero, raises game flag
  if (white_clock == 0)
    gameStateFlag = 2;

  if (black_clock == 0)
    gameStateFlag = 1;
}

void draw_game_clock(bool game_started) {

  if (game_started)
    updateTimer(isWhitesTurn);

  /*         White Clock        */

  int min = white_clock / 60;
  int sec = white_clock - min * 60;

  char temp[10];
  sprintf(temp, "W %02d:%02d ", min, sec);

  draw_text(temp, 80, 785, 0xFFFFFF, false);

  /*         Black Clock        */

  min = black_clock / 60;
  sec = black_clock - min * 60;

  sprintf(temp, "B %02d:%02d ", min, sec);

  draw_text(temp, 500, 785, 0xFFFFFF, false);
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

void set_connected(bool isconnected) {
  hasconnected = isconnected;
}

void set_kbd_selected_opt(bool up) {
  kbd_selected_opt = (up) ? ((kbd_selected_opt + 1) % 6) : ((kbd_selected_opt - 1 + 6) % 6);
}

int get_kbd_selected_opt() {
  return kbd_selected_opt;
}
