#include "pieces.h"

uint64_t get_Pawn_valid_moves(uint8_t lin, uint8_t col) {
	//Falta implementar movimentações para comer peças
  uint64_t answer = 0;
	if (lin == 6) { //se estão na posição inicial
    answer = answer || POS(5, col) || POS(4, col);
  }
	else {
    answer = answer || POS(5, col);
  }
  return answer;
}

uint64_t get_Bishop_valid_moves(uint8_t lin, uint8_t col) {
	//Not implemented yet!
}
  //Auxiliar
uint8_t get4LSB(uint8_t val) {
  return val && 0x0F;
}

uint8_t get4MSB(uint8_t val) {
  return val>>4;
}