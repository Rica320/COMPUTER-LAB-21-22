#ifndef _LCOM_PIECES_H_
#define _LCOM_PIECES_H_

#include <lcom/lcf.h>

#include "../views/animation/animation.h"

#define BOARD_SIZE 8

typedef struct Piece *Board;

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

/**
 * @brief Chess game pieces options
 *
 */
enum PIECES { Blank_space,
              Pawn,
              Bishop,
              Queen,
              King,
              Rook,
              Knight };

typedef enum PIECES PIECE_T;
typedef struct Piece Piece_t;

/**
 * @brief Chess game pieces available colors
 *
 */
enum Color_e { BLACK,
               WHITE };

typedef enum Color_e Piece_Color;

/**
 * @brief Chess game piece struct
 *
 */
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
 * @brief Checks available moves for a pawn in the given board in the given position
 * and updates the valid_moves matrix with the computed results
 * @param board Current game Board
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves
 * @param lin Current lin position of the piece
 * @param col Current col position of the piece
 * @param color Color of the piece
 * @return true if it is a empty space
 * @return false otherwise
 */
bool valid_pawn_move(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);

/**
 * @brief Checks if (lin,col) contains an enemy piece, saving value in valid_moves
 * @param board Current game Board
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves
 * @param lin Line of the Position to Check
 * @param col Column of the Position to Check
 * @param color Color of the piece
 */
void valid_pawn_eat(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);

/**
 * @brief Checks if a piece can be moved to (lin,col), saving value in valid_moves
 * @param board Current game Board
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves
 * @param lin Line of the Position to Check
 * @param col Column of the Position to Check
 * @param color Color of the piece
 */
void valid_king_move(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);

void valid_knight_move(Board board[8][8], bool valid_moves[8][8], uint8_t lin, uint8_t col, uint8_t color);

/**
 * @brief Creates a new Piece in the board
 *
 * @param animSprite Sprite of the piece
 * @param p_t Struct where Piece Data will be Stored
 * @param color Piece Color
 * @return Piece_t* Piece Data
 */
Piece_t *make_piece(AnimSprite *animSprite, PIECE_T p_t, Piece_Color color);

/**
 * @brief Get the current valid moves for a specific piece
 *
 * @param board Board in its current state
 * @param lin Line where the piece is
 * @param col Column where the piece is
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves where it is not
 * @param isWhitesTurn True if it's White turn or False if it's Black turn
 */
void get_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8], bool isWhitesTurn);

/**
 * @brief Get the current valid moves for a Pawn in the given pos
 *
 * @param board Board in its current state
 * @param lin Line where the Pawn is
 * @param col Column where the Pawn is
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves where it is not
 */
void get_Pawn_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

/**
 * @brief Get the current valid moves for a Bishop in the given pos
 *
 * @param board Board in its current state
 * @param lin Line where the Bishop is
 * @param col Column where the Bishop is
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves where it is not
 */
void get_Bishop_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

/**
 * @brief Get the current valid moves for a Queen in the given pos
 *
 * @param board Board in its current state
 * @param lin Line where the Queen is
 * @param col Column where the Queen is
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves where it is not
 */
void get_Queen_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

/**
 * @brief Get the current valid moves for a King in the given pos
 *
 * @param board Board in its current state
 * @param lin Line where the King is
 * @param col Column where the King is
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves where it is not
 */
void get_King_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

/**
 * @brief Get the current valid moves for a Rook in the given pos
 *
 * @param board Board in its current state
 * @param lin Line where the Rook is
 * @param col Column where the Rook is
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves where it is not
 */
void get_Rook_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

/**
 * @brief Get the current valid moves for a Knight in the given pos
 *
 * @param board Board in its current state
 * @param lin Line where the Knight is
 * @param col Column where the Knight is
 * @param valid_moves Valid Moves matrix to be filled with the given piece valid moves where it is not
 */
void get_Knight_valid_moves(Board board[8][8], uint8_t lin, uint8_t col, bool valid_moves[8][8]);

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
 * @return false if outside the board
 */
bool is_inside_board(uint8_t lin, uint8_t col);

#endif
