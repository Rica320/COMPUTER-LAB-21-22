#include "board.h"

bool get_square_color(uint8_t lin, uint8_t col){//returns true if white and false otherwise
  
  if((lin+col)%2) != 0 return true;
  return false;  
}
