#include "pieces.h"

Piece_t* make_piece(const xpm_map_t xpm, enum xpm_image_type type, uint8_t pos, PIECE_T p_t, Piece_Color color) {
  Piece_t* new_sprite = (Piece_t*) malloc(sizeof(Piece_t));
  xpm_image_t img;
  new_sprite->map = xpm_load(xpm, type, &img);

  if (new_sprite->map == NULL) {
    free(new_sprite);
    return NULL;
  }

  new_sprite->pos = pos;
  new_sprite->p_type = p_t;
  new_sprite->color = color;

  return new_sprite;
}

uint64_t get_valid_moves(Piece_t piece){
  uint8_t col = get4MSB(piece.pos);
  uint8_t lin = get4LSB(piece.pos);
  switch(piece.p_type){
    case Pawn:
      return get_Pawn_valid_moves(lin,col);
    case Bishop:
      return get_Bishop_valid_moves(lin,col);
    case King:
      return get_King_valid_moves(lin,col);
    case Queen:
      return get_Queen_valid_moves(lin,col);
    case Knight:
      return get_Knight_valid_moves(lin,col);
    case Rook:
      return get_Rook_valid_moves(lin,col);
    default:
      break;
  }
  return 0;
}

uint64_t get_Pawn_valid_moves(uint8_t lin, uint8_t col) {
  uint64_t answer = 0;
  // Verifica se pode avançar 2 casas ou apenas 1
  if (lin == 6) { // se estao na posicao inicial
    answer = answer || POS(5, col) || POS(4, col);
  }
  else {
    answer = answer || POS(lin + 1, col);
  }

  // Verifica se há peças que pode comer
  if (table[lin - 1][col - 1]->p_type != Blank_space) {
    answer = answer || POS(lin - 1, col - 1);
  }
  if (table[lin - 1][col + 1]->p_type != Blank_space) {
    answer = answer || POS(lin - 1, col + 1);
  }

  // Falta considerar jogadas en passant
  // Função considera que as peças que estão a ser verificadas começam sempre no fundo do tabuleiro
  return answer;
}

uint64_t get_Bishop_valid_moves(uint8_t lin, uint8_t col) {
  uint64_t answer = 0;
  int dist = 1, diagonalsDone = 0;
  while (diagonalsDone < 4) {
    // UPPER LEFT DIAGONAL
    if (is_inside_board(lin - dist, col - dist)) {
      // Se a casa na diagonal estiver livre:
      if (table[lin - dist][col - dist] == Blank_space) {
        answer = answer || POS(lin - dist, col - dist);
      }
      // Se a diagonal até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin - dist, col - dist);
        diagonalsDone++;
      }
    }
    else {
      diagonalsDone++;
    } // Caso seja fora do tabuleiro
    // UPPER RIGHT DIAGONAL
    if (is_inside_board(lin - dist, col + dist)) {
      // Se a casa na diagonal estiver livre:
      if (table[lin - dist][col + dist] == Blank_space) {
        answer = answer || POS(lin - dist, col + dist);
      }
      // Se a diagonal até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin - dist, col + dist);
        diagonalsDone++;
      }
    }
    else {
      diagonalsDone++;
    } // Caso seja fora do tabuleiro
    // LOWER LEFT DIAGONAL
    if (is_inside_board(lin + dist, col - dist)) {
      // Se a casa na diagonal estiver livre:
      if (table[lin + dist][col - dist] == Blank_space) {
        answer = answer || POS(lin + dist, col - dist);
      }
      // Se a diagonal até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin + dist, col - dist);
        diagonalsDone++;
      }
    }
    else {
      diagonalsDone++;
    } // Caso seja fora do tabuleiro
    // LOWER RIGHT DIAGONAL
    if (is_inside_board(lin + dist, col - dist)) {
      // Se a casa na diagonal estiver livre:
      if (table[lin + dist][col + dist] == Blank_space) {
        answer = answer || POS(lin + dist, col + dist);
      }
      // Se a diagonal até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin + dist, col + dist);
        diagonalsDone++;
      }
    }
    else {
      diagonalsDone++;
    } // Caso seja fora do tabuleiro
  }
  return answer;
}

uint64_t get_Queen_valid_moves(uint8_t lin, uint8_t col) {
  return get_Rook_valid_moves(lin, col) || get_Bishop_valid_moves(lin, col);
}

uint64_t get_King_valid_moves(uint8_t lin, uint8_t col) {
  uint64_t possible_moves = POS(lin - 1, col - 1) || POS(lin - 1, col) || POS(lin - 1, col + 1) || POS(lin, col - 1) || POS(lin, col + 1) || POS(lin + 1, col - 1) || POS(lin + 1, col) || POS(lin + 1, col + 1);
  return possible_moves || get_Queen_valid_moves(lin, col);
}

uint64_t get_Rook_valid_moves(uint8_t lin, uint8_t col) {
  uint64_t answer = 0;
  int dist = 1, linesDone = 0;
  while (linesDone < 4) {
    // UP LINE
    if (is_inside_board(lin - dist, col)) {
      // Se a casa na linha estiver livre:
      if (table[lin - dist][col] == Blank_space) {
        answer = answer || POS(lin - dist, col);
      }
      // Se a linha até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin - dist, col);
        linesDone++;
      }
    }
    else {
      linesDone++;
    } // Caso seja fora do tabuleiro
    // DOWN LINE
    if (is_inside_board(lin + dist, col)) {
      // Se a casa na linha estiver livre:
      if (table[lin + dist][col] == Blank_space) {
        answer = answer || POS(lin + dist, col);
      }
      // Se a linha até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin + dist, col);
        linesDone++;
      }
    }
    else {
      linesDone++;
    } // Caso seja fora do tabuleiro
    // LEFT LINE
    if (is_inside_board(lin, col - dist)) {
      // Se a casa na linha estiver livre:
      if (table[lin][col - dist] == Blank_space) {
        answer = answer || POS(lin, col - dist);
      }
      // Se a linha até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin, col - dist);
        linesDone++;
      }
    }
    else {
      linesDone++;
    } // Caso seja fora do tabuleiro
    // RIGHT LINE
    if (is_inside_board(lin, col + dist)) {
      // Se a casa na diagonal estiver livre:
      if (table[lin][col + dist] == Blank_space) {
        answer = answer || POS(lin, col + dist);
      }
      // Se a diagonal até à casa estiver livre, mas estiver uma peça nela
      else {
        answer = answer || POS(lin, col + dist);
        linesDone++;
      }
    }
    else {
      linesDone++;
    } // Caso seja fora do tabuleiro
  }
  return answer;
}

uint64_t get_Knight_valid_moves(uint8_t lin, uint8_t col) {
  uint64_t answer = 0;
  // Desde que seja dentro do tabuleiro, pode mover-se para as seguintes opções:
  if (is_inside_board(lin + 2, col + 1)) {
    answer = answer || POS(lin + 2, col + 1);
  }
  if (is_inside_board(lin + 2, col - 1)) {
    answer = answer || POS(lin + 2, col - 1);
  }
  if (is_inside_board(lin - 2, col + 1)) {
    answer = answer || POS(lin - 2, col + 1);
  }
  if (is_inside_board(lin - 2, col - 1)) {
    answer = answer || POS(lin - 2, col - 1);
  }
  if (is_inside_board(lin + 1, col + 2)) {
    answer = answer || POS(lin + 1, col + 2);
  }
  if (is_inside_board(lin + 1, col - 2)) {
    answer = answer || POS(lin + 1, col - 2);
  }
  if (is_inside_board(lin - 1, col - 2)) {
    answer = answer || POS(lin - 1, col - 2);
  }
  if (is_inside_board(lin - 1, col + 2)) {
    answer = answer || POS(lin - 1, col + 2);
  }
  return answer;
}

// Auxiliares
bool is_inside_board(uint8_t lin, uint8_t col) {
  if (lin < 0 || lin > 7 || col < 0 || col > 7) {
    return false;
  }
  else {
    return true;
  }
}
uint8_t get4LSB(uint8_t val) {
  return val & 0x0F;
}

uint8_t get4MSB(uint8_t val) {
  return val >> 4;
}
