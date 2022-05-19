#include "views.h"

void draw_board() {
  for (uint16_t i = 0; i < BOARD_SCREEN_SIZE; i+=BOARD_SCREEN_CASE_SIZE) { // TODO ::: WE SHOULD USE A IMG ::: ITS EASIER AND FASTER TO RENDER
    for (uint16_t j = 0; j < BOARD_SCREEN_SIZE; j+=BOARD_SCREEN_CASE_SIZE) {
      vg_draw_rectangle(BOARD_START_POS_X + i, BOARD_START_POS_Y + j, BOARD_SCREEN_CASE_SIZE, BOARD_SCREEN_CASE_SIZE, get_square_color(j/BOARD_SCREEN_CASE_SIZE,i/BOARD_SCREEN_CASE_SIZE) == WHITE? BOARD_WHITE_CASE_COLOR : BOARD_BLACK_CASE_COLOR);
    }
  }
}

void draw_pieces(Board table[8][8]) {

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    for (size_t j = 0; j < BOARD_SIZE; j++)
    {
      if (table[i][j]->p_type != Blank_space)
      {
        draw_piece(table[i][j]);
      }
    }
  }
  
}

void draw_piece(Board piece) {
  if (piece == NULL)
  {
    return;
  }
  
  draw_piece_in_mode_14c(piece->map, (piece->pos>>4), piece->pos & 0x0f, BOARD_SCREEN_CASE_SIZE);
}

void draw_clock() {
  
}
