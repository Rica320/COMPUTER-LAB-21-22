#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "../sprite/sprite.h"
#include <lcom/lcf.h>

/**
 * @brief Animated Sprite composed of many sub sprites
 *
 */
typedef struct {
  sprite_t *sp;                   // sprite containing the animation
  unsigned hsize, vsize;          // resolution of each sub sprite
  unsigned num_fig, num_fig_line; // number of subsprites / num subsprites in single line
  unsigned cur_fig;               // current sub sprite being drawn
} AnimSprite;

/**
 * @brief Create a animSprite object
 *
 * @param sprite sprite to be used for the animation
 * @param num_sprites number of sub-prites in the given sprite
 * @param lin_count number of sub-sprites in a single line
 * @param hsize horizontal size of each sub-sprite
 * @param vsize vertical size of each sub-sprite
 * @return AnimSprite*
 */
AnimSprite *create_animSprite(sprite_t *sprite, uint32_t num_sprites, uint32_t lin_count, uint32_t hsize, uint32_t vsize);

/**
 * @brief Draw a given animated sprite frame in the givem position
 *
 * @param animSprite animated sprite to be drawn
 * @param num_fig sprite frame to be drawn
 * @param x x position where to draw in the screen
 * @param y y position where to draw in the screen
 * @return int
 */
int draw_animSprite(AnimSprite *animSprite, uint8_t num_fig, int x, int y);

#endif // _ANIMATION_H_
