#ifndef _LCOM_PIECES_H_
#define _LCOM_PIECES_H_

#include <lcom/lcf.h>

#include "../views/animation/animation.h"

#define BOARD_SIZE 8

typedef struct Piece *Board;

// returns the color of the square in lin/col
/**
 * @brief Returns Color of Square in Pos (lin,col)
 * 
 * @param lin Line of the position
 * @param col Column of the position
 * @return 0 if black
 * @return 1 if white
 */
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
/**
 * @brief Frees Memory Allocated to a Piece
 * @param p Piece whose memory we want to free
 */
void free_piece(Piece_t *p);
/**
 * @brief Checks if (lin,col) is an empty square in the board, saving value in valid_moves and returning it
 * @param board Current Board
 * @param valid_moves Bi-Dimensional Boolean Array with valid moves set to True
 * @param lin Line of the Position to Check
 * @param col Column of the Position to Check
 * @param color Color of the piece
 * @return true if it is a empty space
 * @return false otherwise
 */
bool valid_pawn_move(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);
/**
 * @brief Checks if (lin,col) contains a enemy piece, saving value in valid_moves
 * @param board Current Board
 * @param valid_moves Bi-Dimensional Boolean Array with valid moves set to True
 * @param lin Line of the Position to Check
 * @param col Column of the Position to Check
 * @param color Color of the piece
 */
void valid_pawn_eat(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);
/**
 * @brief Checks if a piece can be moved to (lin,col), saving value in valid_moves
 * @param board Current Board
 * @param valid_moves Bi-Dimensional Boolean Array with valid moves set to true
 * @param lin Line of the Position to Check
 * @param col Column of the Position to Check
 * @param color Color of the piece
 */
void valid_king_move(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);
void valid_knight_move(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);

typedef struct Piece Piece_t;
/**
 * @brief Creates a new Piece in the board
 * 
 * @param animSprite Sprite of the piece
 * @param p_t Struct where Piece Data will be Stored
 * @param color Piece Color
 * @return Piece_t* Piece Data
 */
Piece_t *make_piece(AnimSprite *animSprite, PIECE_T p_t, Piece_Color color);
void free_piece(Piece_t *p);
/**
 * @brief Get the current valid moves for a specific piece
 * 
 * @param board Board in its current state
 * @param lin Line where the piece is
 * @param col Column where the piece is
 * @param valid_moves 8x8 Bidimensional Boolean Array with True where the move is valid and false where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8], bool isWhitesTurn);

// Calculate valid pieces movements inside the given board
/**
 * @brief Get the current valid moves for a Pawn
 * 
 * @param board Board in its current state
 * @param lin Line where the Pawn is
 * @param col Column where the Pawn is
 * @param valid_moves 8x8 Bidimensional Boolean Array with True where the move is valid and false where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_Pawn_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the current valid moves for a Bishop
 * 
 * @param board Board in its current state
 * @param lin Line where the Bishop is
 * @param col Column where the Bishop is
 * @param valid_moves 8x8 Bidimensional Boolean Array with True where the move is valid and false where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_Bishop_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the current valid moves for a Queen
 * 
 * @param board Board in its current state
 * @param lin Line where the Queen is
 * @param col Column where the Queen is
 * @param valid_moves 8x8 Bidimensional Boolean Array with True where the move is valid and false where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_Queen_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the current valid moves for a King
 * 
 * @param board Board in its current state
 * @param lin Line where the King is
 * @param col Column where the King is
 * @param valid_moves 8x8 Bidimensional Boolean Array with True where the move is valid and false where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_King_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the current valid moves for a Rook
 * 
 * @param board Board in its current state
 * @param lin Line where the Rook is
 * @param col Column where the Rook is
 * @param valid_moves 8x8 Bidimensional Boolean Array with True where the move is valid and false where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_Rook_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);
/**
 * @brief Get the current valid moves for a Knight
 * 
 * @param board Board in its current state
 * @param lin Line where the Knight is
 * @param col Column where the Knight is
 * @param valid_moves 8x8 Bidimensional Boolean Array with True where the move is valid and false where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_Knight_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);


// Auxiliar
/**
 * @brief Checks if there is an Enemy Piece in a certain position
 * 
 * @param pos Position to Check
 * @param color Color of the Player from whomst perspective we want to check
 * @return true if piece is the opposite color
 * @return false if pos is empty or is a piece of the color of the player
 */
bool isEnemyPiecePos(Piece_t *pos, int color);
/**
 * @brief Checks if there is an Ally Piece in a certain position
 * 
 * @param pos Position to Check
 * @param color Color of the Player from whomst perspective we want to check
 * @return true if piece is the same color
 * @return false if pos is empty or is a piece of the opposite
 */
bool isOwnPiecePos(Piece_t *pos, int color);
/**
 * @brief Checks if a certain position is inside the board
 * 
 * @param lin Line of the position to Check
 * @param col Column of the position to check
 * @return true if position is inside the board
 * @return false otherwise
 */
bool is_inside_board(uint8_t lin, uint8_t col);

#endif
