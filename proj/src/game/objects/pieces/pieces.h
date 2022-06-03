#ifndef _LCOM_PIECES_H_
#define _LCOM_PIECES_H_

#include <lcom/lcf.h>

// ================BOARD======================

#define BOARD_SIZE 8

typedef struct Piece *Board;

// returns the color of the square in lin/col
int get_square_color(uint16_t lin, uint16_t col);

// ================/BOARD======================

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
  uint8_t pos; // 4 fist bits -> collum , the other 4 -> row ... ex: 13 -> coluna 1, linha 3 ou
  // seja A3 em notação oficial ... note-se que A significa que se começa a contar com 1
  // É importante aproveitar o espaço ao máximo para que na serial port seja mais fácil de passar
  // informação
  // VER a MAcro LSHUB_IN_BYTE
  PIECE_T p_type;
  Piece_Color color;
  uint8_t *map;
};
typedef struct Piece Piece_t;
// TODO :::

// THIS SHOUD BE A FUNC with a switch that will call the apropriated get_valid_func according to type

Piece_t *make_piece(const xpm_map_t xpm, PIECE_T p_t, Piece_Color color);
void free_piece(Piece_t *p);

//**
 * @brief Get the valid moves object
 * 
 * @param board 
 * @param lin 
 * @param col 
 * @param valid_moves 
 * @param isWhitesTurn 
 * @return uint64_t 
 */
uint64_t get_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8], bool isWhitesTurn); // IGNORE THIS FOR NOW

// do this types of functs for every piece ...
/**
 * @brief Get the valid moves for a Pawn
 * 
 * @param board Current board struct
 * @param lin Line where the Pawn is
 * @param col Column where the Pawn is
 * @param valid_moves Struct with bools that are true when move is valid
 * @return uint64_t with valid positions set to 1
 */
uint64_t get_Pawn_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the valid moves for a Bishop
 * 
 * @param board Current board struct
 * @param lin Line where the Bishop is
 * @param col Column where the Bishop is
 * @param valid_moves Struct with bools that are true when move is valid
 * @return uint64_t with valid positions set to 1
 */
uint64_t get_Bishop_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the valid moves for a Queen
 * 
 * @param board Current board struct
 * @param lin Line where the Queen is
 * @param col Column where the Queen is
 * @param valid_moves Struct with bools that are true when move is valid
 * @return uint64_t with valid positions set to 1
 */
uint64_t get_Queen_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the valid moves for a King
 * 
 * @param board Current board struct
 * @param lin Line where the King is
 * @param col Column where the King is
 * @param valid_moves Struct with bools that are true when move is valid
 * @return uint64_t with valid positions set to 1
 */
uint64_t get_King_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the valid moves for a Rook
 * 
 * @param board Current board struct
 * @param lin Line where the Rook is
 * @param col Column where the Rook is
 * @param valid_moves Struct with bools that are true when move is valid
 * @return uint64_t with valid positions set to 1
 */
uint64_t get_Rook_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the valid moves for a Knight
 * 
 * @param board Current board struct
 * @param lin Line where the Knight is
 * @param col Column where the Knight is
 * @param valid_moves Struct with bools that are true when move is valid
 * @return uint64_t with valid positions set to 1
 */
uint64_t get_Knight_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

// Auxiliar
/**
 * @brief Checks if there is an Enemy Piece in position pos
 * 
 * @param pos Struct with the piece position
 * @param color Represents the piece color
 * @return true if there is an Enemy Piece in pos
 * @return false otherwise
 */
bool isEnemyPiecePos(Piece_t *pos, int color);
/**
 * @brief Checks if there is another Ally Piece in position pos
 * 
 * @param pos Struct with the piece position
 * @param color Represents the piece color
 * @return true if there is an Ally Piece in pos
 * @return false otherwise
 */
bool isOwnPiecePos(Piece_t *pos, int color);
/**
 * @brief Checks if a certain position is inside the board
 * 
 * @param lin Line of the position
 * @param col Column of the position
 * @return true if (lin,col) is inside the board
 * @return false otherwise
 */
bool is_inside_board(uint8_t lin, uint8_t col);

#endif
