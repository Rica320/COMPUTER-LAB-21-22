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

int draw_animSprite(AnimSprite *animSprite, uint8_t num_fig) {

  sprite_t *sprite = animSprite->sp;

  uint16_t h_res = get_hres(), v_res = get_vres();
  uint8_t *map = sprite->map;
  uint32_t *clr = (uint32_t *) map;

  for (unsigned height = 0; height < sprite->h; height++) {
    
    if (height + sprite->y >= v_res)
      break;

    for (unsigned width = 0; width < sprite->w; width++) {

      if (width + sprite->x >= h_res)
        break;

      if (!(*clr & 0xff000000))
        fill_pixel(sprite->x + width, sprite->y + height, *clr);

      clr++;
    }
  }

  return EXIT_SUCCESS;
}
