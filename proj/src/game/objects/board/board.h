#ifndef _LCOM_BOARD_H_
#define _LCOM_BOARD_H_

#include "../pieces/pieces.h"

#define BOARD_SIZE 8
extern board* table;
typedef struct Piece board[BOARD_SIZE][BOARD_SIZE];
bool get_square_color(uint8_t lin, uint8_t col); //returns true if white and false otherwise.
#endif
