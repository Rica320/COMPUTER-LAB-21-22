#include "board.h"

// returns the color of the square in lin/col
int get_square_color(uint8_t lin, uint8_t col) {
  if ((lin + col) % 2 != 0)
    return WHITE;
  return BLACK;
}
