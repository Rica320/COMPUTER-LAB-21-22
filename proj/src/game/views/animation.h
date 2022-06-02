#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "../sprite/sprite.h"
#include <lcom/lcf.h>

typedef struct {
  sprite_t *sp;                   // sprite containing the animation
  unsigned hsize, vsize;          // resolution of each sub sprite
  unsigned num_fig, num_fig_line; // number of subsprites / num subsprites in single line
  unsigned cur_fig;               // current sub sprite being drawn
} AnimSprite;

/**
 * Creates a AnimSprite given a set of xpm pics
 * @param no_pic number of xpm pictures
 * @param pics pointer to a cllection of images (lines of strings)
 * @return
 */

AnimSprite *create_animSprite(sprite_t *sprite, uint32_t num_sprites, uint32_t lin_count, uint32_t hsize, uint32_t vsize);

int draw_animSprite(AnimSprite *animSprite, uint8_t num_fig);

#endif // _ANIMATION_H_
