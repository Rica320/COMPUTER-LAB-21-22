#include "sprite.h"

struct sprite{
    unsigned int x, y;
    unsigned int w, h;
    uint8_t *map;
};

sprite_t* make_sprite(const xpm_map_t xpm, enum xpm_image_type type) {
  sprite_t* new_sprite = (sprite_t*) malloc(sizeof(sprite_t));
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

unsigned int get_sprite_W(sprite_t * sprite){
  return sprite->w;
}
unsigned int get_sprite_H(sprite_t * sprite){
  return sprite->h;
}


unsigned int get_sprite_Y(sprite_t * sprite) {
  return sprite->y;
}
unsigned int get_sprite_X(sprite_t * sprite) {
  return sprite->x;
}

void set_sprite_Y(sprite_t * sprite, unsigned int y) {
  sprite->y = y;
}
void set_sprite_X(sprite_t * sprite, unsigned int x) {
  sprite->x = x;
}

uint8_t* get_sprite_map(sprite_t * sprite) {
  return sprite->map;
}

int draw_sprite_in_mode_14c(sprite_t *sprite) {

  uint8_t *map = get_sprite_map(sprite);
  for (unsigned int height = 0; height < get_sprite_H(sprite); height++) {
    if (height + sprite->y >= get_vres())
      break;
    for (unsigned int width = 0; width < get_sprite_W(sprite); width++) {

      if (width + sprite->x >= get_hres())
        break;

      RGB rgb = RGB_new(0);

      rgb.setBlue(&rgb, *map);
      map++;
      rgb.setGreen(&rgb, *map);
      map++;
      rgb.setRed(&rgb, *map);
      map++;

      if (!*map) {
        fill_pixel(get_sprite_X(sprite) + width, get_sprite_Y(sprite) + height, rgb.value);
      }

      map++;
    }
  }

  return EXIT_SUCCESS;
}

int draw_sprite_in_mode_105(sprite_t * sprite) {
  uint8_t *map = get_sprite_map(sprite);
  for (unsigned int height = 0; height < get_sprite_H(sprite); height++) {
    for (unsigned int width = 0; width < get_sprite_W(sprite); width++) {
      
      fill_pixel(get_sprite_X(sprite)+ width, get_sprite_Y(sprite) + height, *map);

      map++;
    }
  }

  return EXIT_SUCCESS;
}
