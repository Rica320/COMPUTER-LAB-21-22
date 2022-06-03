#include "animation.h"

AnimSprite *create_animSprite(sprite_t *sprite, uint32_t num_sprites, uint32_t lin_count, uint32_t hsize, uint32_t vsize) {

  AnimSprite *asp = malloc(sizeof(AnimSprite));

  asp->sp = sprite;

  asp->num_fig = num_sprites;
  asp->num_fig_line = lin_count;

  asp->hsize = hsize;
  asp->vsize = vsize;

  asp->cur_fig = 0;

  return asp;
}

int draw_animSprite(AnimSprite *animSprite, uint8_t num_fig, int x, int y) {

  sprite_t *sprite = animSprite->sp;

  uint8_t col_pos = num_fig % animSprite->num_fig_line;
  uint8_t line_pos = (num_fig - 1) / animSprite->num_fig_line;

  uint32_t *clr = (uint32_t *) sprite->map;
  clr += sprite->w * animSprite->vsize * line_pos;

  for (unsigned height = 0; height < animSprite->hsize; height++) {
    clr += (col_pos - 1) * animSprite->hsize;
    for (unsigned width = 0; width < animSprite->vsize; width++)
      // if (!(*clr & 0xff000000))
      fill_pixel_transp(x + width, y + height, *clr++);

    clr += animSprite->hsize * (animSprite->num_fig_line - col_pos);
  }

  return EXIT_SUCCESS;
}
