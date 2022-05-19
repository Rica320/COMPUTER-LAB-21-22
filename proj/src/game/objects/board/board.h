#ifndef _LCOM_BOARD_H_
#define _LCOM_BOARD_H_

#include "../pieces/pieces.h"

#define BOARD_SIZE 8

typedef struct Piece *Board;
Board table[BOARD_SIZE][BOARD_SIZE];


// returns the color of the square in lin/col
int get_square_color(uint16_t lin, uint16_t col);

#endif
