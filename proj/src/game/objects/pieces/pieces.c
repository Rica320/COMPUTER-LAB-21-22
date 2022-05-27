#include "pieces.h"

// returns the color of the square in lin/col
int get_square_color(uint16_t lin, uint16_t col) {
  if ((lin + col) % 2)
    return BLACK;
  return WHITE;
}

Piece_t *make_piece(const xpm_map_t xpm, PIECE_T p_t, Piece_Color color) {
  Piece_t *new_sprite = (Piece_t *) malloc(sizeof(Piece_t));

  if (xpm != NULL) {
    xpm_image_t img;
    new_sprite->map = xpm_load(xpm, XPM_8_8_8_8, &img);
    if (new_sprite->map == NULL) {
      free(new_sprite);
      return NULL;
    }
  }

  new_sprite->p_type = p_t;
  new_sprite->color = color;

  return new_sprite;
}

uint64_t get_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]) {
  memset((void *) valid_moves, 0, sizeof(bool) * BOARD_SIZE * BOARD_SIZE);
  switch (board[lin][col]->p_type) {
    case Pawn:
      return get_Pawn_valid_moves(board, lin, col, valid_moves);
    case Bishop:
      return get_Bishop_valid_moves(board, lin, col, valid_moves);
    case King:
      return get_King_valid_moves(board, lin, col, valid_moves);
    case Queen:
      return get_Queen_valid_moves(board, lin, col, valid_moves);
    case Knight:
      return get_Knight_valid_moves(board, lin, col, valid_moves);
    case Rook:
      return get_Rook_valid_moves(board, lin, col, valid_moves);
    default:
      break;
  }
  return 0;
}

uint64_t get_Pawn_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]) {

  if (board[lin][col]->color == WHITE) {
    if (lin == 6) {
      valid_moves[lin - 1][col] = true;
      valid_moves[lin - 2][col] = true;
    }
    else
      valid_moves[lin - 1][col] = true;
  }
  else {
    if (lin == 1) {
      valid_moves[lin + 1][col] = true;
      valid_moves[lin + 2][col] = true;
    }
    else
      valid_moves[lin + 1][col] = true;
  }

  return 0;
}

uint64_t get_Bishop_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]) {
  bool color = board[lin][col]->color;
  Piece_t *pos;

  for (int i = col + 1; i < 8; i++) {
    pos = board[lin][i];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[lin][i] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  for (int i = col - 1; i >= 0; i--) {
    pos = board[lin][i];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[lin][i] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  for (int i = lin + 1; i < 8; i++) {
    pos = board[i][col];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[i][col] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  for (int i = lin - 1; i >= 0; i--) {
    pos = board[i][col];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[i][col] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  return 0;

  return 0;
}

uint64_t get_Queen_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]) {
  return 0;
}

uint64_t get_King_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]) {

  bool color = board[lin][col]->color;

  if (!isOwnPiecePos(board[lin - 1][col - 1], color))
    valid_moves[lin - 1][col - 1] = true;

  if (!isOwnPiecePos(board[lin - 1][col], color))
    valid_moves[lin - 1][col] = true;

  if (!isOwnPiecePos(board[lin - 1][col + 1], color))
    valid_moves[lin - 1][col + 1] = true;

  if (!isOwnPiecePos(board[lin][col - 1], color))
    valid_moves[lin][col - 1] = true;

  if (!isOwnPiecePos(board[lin][col + 1], color))
    valid_moves[lin][col + 1] = true;

  if (!isOwnPiecePos(board[lin + 1][col - 1], color))
    valid_moves[lin + 1][col - 1] = true;

  if (!isOwnPiecePos(board[lin + 1][col], color))
    valid_moves[lin + 1][col] = true;

  if (!isOwnPiecePos(board[lin + 1][col + 1], color))
    valid_moves[lin + 1][col + 1] = true;

  return 0;
}

uint64_t get_Rook_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]) {

  bool color = board[lin][col]->color;
  Piece_t *pos;

  for (int i = col + 1; i < 8; i++) {
    pos = board[lin][i];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[lin][i] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  for (int i = col - 1; i >= 0; i--) {
    pos = board[lin][i];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[lin][i] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  for (int i = lin + 1; i < 8; i++) {
    pos = board[i][col];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[i][col] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  for (int i = lin - 1; i >= 0; i--) {
    pos = board[i][col];
    if (isOwnPiecePos(pos, color))
      break;
    valid_moves[i][col] = true;
    if (isEnemyPiecePos(pos, color))
      break;
  }

  return 0;
}

uint64_t get_Knight_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]) {
  if (is_inside_board(lin + 2, col + 1)) {
    valid_moves[lin + 2][col + 1] = true;
  }
  if (is_inside_board(lin + 2, col - 1)) {
    valid_moves[lin + 2][col - 1] = true;
  }
  if (is_inside_board(lin - 2, col + 1)) {
    valid_moves[lin - 2][col + 1] = true;
  }
  if (is_inside_board(lin - 2, col - 1)) {
    valid_moves[lin - 2][col - 1] = true;
  }
  if (is_inside_board(lin + 1, col + 2)) {
    valid_moves[lin + 1][col + 2] = true;
  }
  if (is_inside_board(lin + 1, col - 2)) {
    valid_moves[lin + 1][col - 2] = true;
  }
  if (is_inside_board(lin - 1, col - 2)) {
    valid_moves[lin - 1][col - 2] = true;
  }
  if (is_inside_board(lin - 1, col + 2)) {
    valid_moves[lin - 1][col + 2] = true;
  }
  return 0;
}

bool isEnemyPiecePos(Piece_t *pos, int color) {
  return ((pos->p_type != Blank_space) && (pos->color != color));
}

bool isOwnPiecePos(Piece_t *pos, int color) {
  return ((pos->p_type != Blank_space) && (pos->color == color));
}

bool is_inside_board(uint8_t lin, uint8_t col) {
  return !(lin < 0 || lin > 7 || col < 0 || col > 7);
}

uint8_t get4LSB(uint8_t val) {
  return val & 0x0F;
}

uint8_t get4MSB(uint8_t val) {
  return val >> 4;
}
