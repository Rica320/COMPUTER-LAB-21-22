#include "sprite.h"

struct sprite {
  unsigned x, y;
  unsigned w, h;
  uint8_t *map;
};

sprite_t *make_sprite(const xpm_map_t xpm, enum xpm_image_type type) {
  sprite_t *new_sprite = (sprite_t *) malloc(sizeof(sprite_t));
  xpm_image_t img;
  new_sprite->map = xpm_load(xpm, type, &img);

  if (new_sprite->map == NULL) {
    free(new_sprite);
    return NULL;
  }

  new_sprite->w = img.width;
  new_sprite->h = img.height;
  return new_sprite;
}

void free_sprite(sprite_t *p) {
  free(p->map);
  free(p);
}

unsigned get_sprite_W(sprite_t *sprite) {
  return sprite->w;
}
unsigned get_sprite_H(sprite_t *sprite) {
  return sprite->h;
}

unsigned get_sprite_Y(sprite_t *sprite) {
  return sprite->y;
}
unsigned get_sprite_X(sprite_t *sprite) {
  return sprite->x;
}

void set_sprite_Y(sprite_t *sprite, unsigned y) {
  sprite->y = y;
}
void set_sprite_X(sprite_t *sprite, unsigned x) {
  sprite->x = x;
}

void set_sprite_pos(sprite_t *sprite, unsigned x, unsigned y) {
  sprite->x = x;
  sprite->y = y;
}

uint8_t *get_sprite_map(sprite_t *sprite) {
  return sprite->map;
}

int draw_sprite_in_mode_14c(sprite_t *sprite) {
  uint16_t h_res = get_hres(), v_res = get_vres();
  uint8_t *map = get_sprite_map(sprite);
  uint32_t *clr = (uint32_t *) map;

  for (unsigned height = 0; height < sprite->h; height++) {
    if (height + sprite->y >= v_res)
      break;
    for (unsigned width = 0; width < sprite->w; width++) {

      if (width + sprite->x >= h_res)
        break;
      // RGB rgb = RGB_new(0);
      //
      // rgb.setBlue(&rgb, *map);
      // map++;
      // rgb.setGreen(&rgb, *map);
      // map++;
      // rgb.setRed(&rgb, *map);
      // map++;

      // color |= *map;
      // map++;
      // color |= (*map << 8);
      // map++;
      // color |= (*map << 16);
      // map++;

      if (!(*clr & 0xff000000)) {
        fill_pixel(sprite->x + width, sprite->y + height, *clr);
      }

      clr++;
    }
  }

  return EXIT_SUCCESS;
}

int draw_text_sprite(sprite_t *sprite, int color_map) {

  uint8_t *map = get_sprite_map(sprite);
  uint16_t h_res = get_hres(), v_res = get_vres();
  for (unsigned height = 0; height < sprite->h; height++) {
    if (height + sprite->y >= v_res)
      break;
    for (unsigned width = 0; width < sprite->w; width++) {

      if (width + sprite->x >= h_res)
        break;

      map += 3;

      if (!*map++)
        fill_pixel(sprite->x + width, sprite->y + height, RGB_new(color_map).value);
    }
  }

  return EXIT_SUCCESS;
}

int draw_piece_in_mode_14c(uint8_t *map, int x, int y, unsigned size) {
  // static uint32_t color = 0;
  uint16_t h_res = get_hres(), v_res = get_vres();
  uint32_t *clr = (uint32_t *) map;
  for (unsigned height = 0; height < size; height++) {
    if (height + y >= v_res)
      break;
    for (unsigned width = 0; width < size; width++) {

      if (width + x >= h_res)
        break;

      // RGB rgb = RGB_new(0);
      //
      // rgb.setBlue(&rgb, *map);
      // map++;
      // rgb.setGreen(&rgb, *map);
      // map++;
      // rgb.setRed(&rgb, *map);
      // map++;

      // color = 0;
      // color |= *map;
      // map++;
      // color |= (*map << 8);
      // map++;
      // color |= (*map << 16);
      // map++;

      if (!(*clr & 0xff000000)) {
        fill_pixel(x + width, y + height, *clr);
      }

      clr++;
    }
  }
  return EXIT_SUCCESS;
}

int draw_sprite_in_mode_105(sprite_t *sprite) {
  uint8_t *map = get_sprite_map(sprite);
  for (unsigned height = 0; height < sprite->h; height++) {
    for (unsigned width = 0; width < sprite->w; width++) {

      fill_pixel(sprite->x + width, sprite->y + height, *map);

      map++;
    }
  }

  return EXIT_SUCCESS;
}
