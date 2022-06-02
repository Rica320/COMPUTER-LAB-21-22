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
  uint32_t *clr = (uint32_t *)sprite->map;

  for (unsigned height = 0; height < 94; height++) {

    /*     if (height + y >= v_res)
          break; */

    for (unsigned width = 0; width < 94; width++) {

      /*       if (width + x >= h_res)
              break; */

    //if (!(*clr & 0xff000000))
      fill_pixel(x + width, y + height, *clr);
      clr++;
    }

    clr += 94 * 4;
  }

  return EXIT_SUCCESS;
}
