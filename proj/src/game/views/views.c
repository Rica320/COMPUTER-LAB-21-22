#include "views.h"

void draw_board() {
  static int lookUpTable[] = {
    0, 94, 188, 282, 376, 470, 564, 658
  };

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    for (size_t j = 0; j < BOARD_SIZE; j++)
    {
      vg_draw_rectangle(lookUpTable[i], lookUpTable[j], BOARD_SCREEN_CASE_SIZE, BOARD_SCREEN_CASE_SIZE, get_square_color(j , i) == WHITE ? BOARD_WHITE_CASE_COLOR : BOARD_BLACK_CASE_COLOR);
    }
    
  }
  

}

void draw_pieces(Board table[8][8]) {
  static int lookUpTable[] = {
    0, 94, 188, 282, 376, 470, 564, 658
  };
  uint64_t p_moves = get_valid_moves(*(table[6][6]), 6, 6);
  for (size_t i = 0; i < BOARD_SIZE; i++) {
    for (size_t j = 0; j < BOARD_SIZE; j++) {
      if (table[i][j]->p_type != Blank_space) {
        draw_piece(table[i][j], j, i);
        if (p_moves & BIT((uint64_t)j + (uint64_t)(i<<3)))
        {
          vg_draw_rectangle(lookUpTable[j], lookUpTable[i], BOARD_SCREEN_CASE_SIZE, BOARD_SCREEN_CASE_SIZE, 0x00ff00);
        }
        
      }
    }
  }
}

void draw_piece(Board piece, unsigned int x, unsigned int y) {
  if (piece == NULL) {
    return;
  }

  static int lookUpTable[] = {
    0, 94, 188, 282, 376, 470, 564, 658
  };

  draw_piece_in_mode_14c(piece->map, lookUpTable[x], lookUpTable[y], BOARD_SCREEN_CASE_SIZE);
}

void draw_clock() {
}
