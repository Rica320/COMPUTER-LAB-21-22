#ifndef _LCOM_PIECES_H_
#define _LCOM_PIECES_H_

#include <lcom/lcf.h>

#include "../views/animation/animation.h"

#define BOARD_SIZE 8

typedef struct Piece *Board;

// returns the color of the square in lin/col
int get_square_color(uint16_t lin, uint16_t col);

#define LINE(n) (0xFF << (8 * (7 - n)))                                                                                                 // uint64_t com a Linha n do tabuleiro
#define COLUMN(n) (BIT(63 - n) || BIT(55 - n) || BIT(47 - n) || BIT(39 - n) || BIT(31 - n) || BIT(23 - n) || BIT(15 - n) || BIT(7 - n)) // uint64_t com a coluna n do tabuleiro
//#define POS(l, c) (LINE(l) & COLUMN(c))
#define POS(l, c) (1LL << ((c) + ((l) << 3)))

enum PIECES { Blank_space,
              Pawn,
              Bishop,
              Queen,
              King,
              Rook,
              Knight };

typedef enum PIECES PIECE_T;

enum Color_e { BLACK,
               WHITE };

typedef enum Color_e Piece_Color;

struct Piece {
  uint8_t pos;
  PIECE_T p_type;
  Piece_Color color;
  AnimSprite *animSprite;
};

typedef struct Piece Piece_t;

Piece_t *make_piece(AnimSprite *animSprite, PIECE_T p_t, Piece_Color color);
void free_piece(Piece_t *p);

void get_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8], bool isWhitesTurn);

// Calculate valid pieces movements inside the given board
void get_Pawn_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
void get_Bishop_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
void get_Queen_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
void get_King_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
void get_Rook_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
void get_Knight_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

// Auxiliar
bool isEnemyPiecePos(Piece_t *pos, int color);
bool isOwnPiecePos(Piece_t *pos, int color);
bool is_inside_board(uint8_t lin, uint8_t col);

#endif
