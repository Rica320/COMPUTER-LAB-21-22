#include "board.h"

// returns the color of the square in lin/col
int get_square_color(uint16_t lin, uint16_t col) {
  if ((lin + col) % 2)
    return BLACK;
  return WHITE;
}
