#ifndef _LCOM_PIECES_H_
#define _LCOM_PIECES_H_

#include <lcom/lcf.h>


enum PIECES {Blanck_space,Pawn, Bishop, Queen, King, Rook, Knight};
typedef enum PIECES PIECE_T;

enum Color_e {BLACK, WHITE};
typedef enum Color_e Color;

struct Piece{
    uint8_t pos; // 4 fist bits -> collum , the other 4 -> row ... ex: 13 -> coluna 1, linha 3 ou 
    // seja A3 em notação oficial ... note-se que A significa que se começa a contar com 1
    // É importante aproveitar o espaço ao máximo para que na serial port seja mais fácil de passar
    // informação
    // VER a MAcro LSHUB_IN_BYTE
    PIECE_T p_type;
    Color color;
    uint8_t *map;
};
typedef struct Piece Piece_t; 
// TODO :::

// THIS SHOUD BE A FUNC with a switch that will call the apropriated get_valid_func according to type
void get_valid_moves(Piece_t piece); // IGNORE THIS FOR NOW

// do this types of functs for every piece ...
void get_Pawn_valid_moves();


#endif
