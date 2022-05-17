#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include "lcom/lcf.h"

#include "rgb.h"

struct sprite;
typedef struct sprite sprite_t;

sprite_t* make_sprite(const xpm_map_t xpm, enum xpm_image_type type);
void free_sprite(sprite_t *p);


unsigned int get_sprite_W(sprite_t * sprite);
unsigned int get_sprite_H(sprite_t * sprite);


unsigned int get_sprite_Y(sprite_t * sprite);
unsigned int get_sprite_X(sprite_t * sprite);

void set_sprite_Y(sprite_t * sprite, unsigned int y);
void set_sprite_X(sprite_t * sprite, unsigned int x);

uint8_t* get_sprite_map(sprite_t * sprite);

// int draw_sprite(sprite_t * sprite);
int draw_sprite_in_mode_14c(sprite_t * sprite);
int draw_sprite_in_mode_105(sprite_t * sprite);

#endif //SPRITE_H_INCLUDED
